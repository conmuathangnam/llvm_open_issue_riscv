# miscompilation: incorrect vectorization of loop copying i32 values

**Author:** mlugg
**URL:** https://github.com/llvm/llvm-project/issues/186922
**Status:** Open
**Labels:** miscompilation, vectorizers

## Body

We have recently experienced a miscompilation in the [Zig](https://codeberg.org/ziglang/zig) compiler which appears to be related to loop vectorization. It only reproduces for some values of `target-cpu` (this repro uses `znver4`). The bug was first encountered in LLVM 21, but @alexrp has been able to confirm that it still reproduces in LLVM 22.

I was able to reduce the repro to this IR (and some simple C code calling it):

```llvm
; ModuleID = 'reduced.bc'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux6.12.561-gnu2.41.0"

@0 = constant { i32, i8, [3 x i8] } { i32 undef, i8 0, [3 x i8] undef }

declare ptr @malloc(i64)

define i32 @repro_entry(ptr %arg_ptr, i64 %1) #0 {
entry:
  %3 = alloca { i32, i8, [3 x i8] }, align 4
  %4 = shl i64 %1, 2
  %5 = icmp eq i64 %4, 0
  br i1 %5, label %pre_loop, label %len_nonzero

pre_loop:                                                ; preds = %len_nonzero, %entry
  %buf = phi ptr [ %9, %len_nonzero ], [ %arg_ptr, %entry ]
  br label %loop_check

len_nonzero:                                                ; preds = %entry
  %9 = call ptr @malloc(i64 %4)
  br label %pre_loop

loop_check:                                               ; preds = %loop_body, %pre_loop
  %index = phi i64 [ 0, %pre_loop ], [ %18, %loop_body ]
  %11 = icmp ult i64 %index, %1
  br i1 %11, label %loop_body, label %end

loop_body:                                               ; preds = %loop_check
  %buf_elem_ptr = getelementptr i32, ptr %buf, i64 %index
  %arg_elem_ptr = getelementptr i32, ptr %arg_ptr, i64 %index
  %arg_elem = load i32, ptr %arg_elem_ptr, align 4
  %16 = call fastcc i32 @repro.sameIfNonzero2(ptr %3, i32 %arg_elem)
  %17 = load i32, ptr %3, align 4
  store i32 %17, ptr %buf_elem_ptr, align 4
  %18 = add i64 %index, 1
  br label %loop_check

end:                                               ; preds = %loop_check
  %20 = load i32, ptr %buf, align 4
  ret i32 %20
}

define fastcc i32 @repro.sameIfNonzero2(ptr %0, i32 %1) {
  call fastcc void @repro.sameIfNonzero(ptr %0, i32 %1)
  ret i32 0
}

define fastcc void @repro.sameIfNonzero(ptr %0, i32 %1) {
entry:
  %3 = alloca { i32, i8, [3 x i8] }, align 4
  %zero_cond = icmp eq i32 %1, 0
  br i1 %zero_cond, label %yes_zero, label %non_zero

yes_zero:                                                ; preds = %non_zero, %entry
  %5 = phi ptr [ %3, %non_zero ], [ @0, %entry ]
  call void @llvm.memcpy.p0.p0.i64(ptr %0, ptr %5, i64 8, i1 false)
  ret void

non_zero:                                                ; preds = %entry
  store i32 %1, ptr %3, align 4
  br label %yes_zero
}

attributes #0 = { "target-cpu"="znver4" }
```
```c
/* main.c. I have this in a separate compilation unit to prevent inlining interfering with the repro. */
#include <stdint.h>
#include <stdio.h>
uint32_t repro_entry(uint32_t *ptr, size_t len);
uint32_t args[32] = {
    100, 101, 102, 103, 104, 105, 106, 107,
    108, 109, 110, 111, 112, 113, 114, 115,
    116, 117, 118, 119, 120, 121, 122, 123,
    124, 125, 126, 127, 128, 129, 130, 131,
};
int main(void) {
    uint32_t res = repro_entry(args, 32);
    printf("expect %u, got %u\n", args[0], res);
    return 0;
}
```
With `clang`:
```shell-session
$ clang-21 -O0 main.c reduced.ll && ./a.out
expect 100, got 100
$ clang-21 -O1 main.c reduced.ll && ./a.out
expect 100, got 100
$ clang-21 -O2 main.c reduced.ll && ./a.out
expect 100, got 107
```
Invoking `opt` instead of using `clang -O2`:
```shell-session
$ opt reduced.ll -S -o reduced_optimized.ll -passes='inline,loop-rotate,sroa,instcombine,loop-vectorize'
$ clang -O0 main.c reduced_optimized.ll && ./a.out
expect 100, got 107
```
It looks to me like the `loop-vectorize` pass is probably at fault, since it appears that the 8th input element (value 107) is copied to the first 8 elements of the output buffer which certainly sounds like a vectorization issue.

This is a quite serious problem for the Zig project: the bug is triggering in our normal compiler bootstrapping process for many users, and there's no clear way to work around this in the code which is triggering the miscompilation, so the only option I can see is disabling the `loop-vectorize` pass, which will presumably seriously pessimize certain codegen.

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 2 - andrewrk

Reproduces with 6121df77a781d2e6f9a8e569aa45ccfffd6c7e0e (current tip of release/22.x):

```
$ ~/local/llvm22-assert/bin/clang-22 --version
clang version 22.1.2 (https://github.com/llvm/llvm-project 6121df77a781d2e6f9a8e569aa45ccfffd6c7e0e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/andy/local/llvm22-assert/bin
Build config: +assertions

$ ~/local/llvm22-assert/bin/clang-22 -isystem /nix/store/1k8f0lkj10gsbb1fh0idr1423l37yj69-glibc-2.40-218-dev/include -B /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib -B /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/0p8b2lqk47fvxm9hc6c8mnln5l8x51q1-gcc-14.3.0-lib/lib/ -O0 main.c reduced.ll
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
1 warning generated.

$ patchelf --set-interpreter /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib/ld-linux-x86-64.so.2 a.out

$ ./a.out
expect 100, got 100

$ ~/local/llvm22-assert/bin/clang-22 -isystem /nix/store/1k8f0lkj10gsbb1fh0idr1423l37yj69-glibc-2.40-218-dev/include -B /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib -B /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/0p8b2lqk47fvxm9hc6c8mnln5l8x51q1-gcc-14.3.0-lib/lib/ -O1 main.c reduced.ll
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
1 warning generated.

$ patchelf --set-interpreter /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib/ld-linux-x86-64.so.2 a.out

$ ./a.out
expect 100, got 100

$ ~/local/llvm22-assert/bin/clang-22 -isystem /nix/store/1k8f0lkj10gsbb1fh0idr1423l37yj69-glibc-2.40-218-dev/include -B /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib -B /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/yps9cjmi8g58mh0f7pw902br7gxwg2dz-gcc-14.3.0/lib/gcc/x86_64-unknown-linux-gnu/14.3.0/ -L /nix/store/0p8b2lqk47fvxm9hc6c8mnln5l8x51q1-gcc-14.3.0-lib/lib/ -O2 main.c reduced.ll
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
1 warning generated.

$ patchelf --set-interpreter /nix/store/vr7ds8vwbl2fz7pr221d5y0f8n9a5wda-glibc-2.40-218/lib/ld-linux-x86-64.so.2 a.out

$ ./a.out
expect 100, got 107
```

---

### Comment 3 - lukel97

There's a store of a non-uniform value to a uniform address, which is then loaded from and then stored again. I don't think we can vectorize this and normally LAA can detect this. But there's a phi in the loads address which I think is throwing LAA off. Here's a slightly more reduced version that `opt -p loop-vectorize` vectorizes:


```llvm
; ModuleID = 'reduced.bc'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux6.12.561-gnu2.41.0"

define i32 @repro_entry(ptr %p0, ptr %p1, ptr %p2, i64 %0, i1 %c1) #1 {
entry:
  %phip = select i1 %c1, ptr %p1, ptr %p0
  br label %loop

loop:
  %iv = phi i64 [ %iv.next, %latch ], [ 0, %entry ]
  %gep0 = getelementptr i32, ptr %p0, i64 %iv
  %x = load i32, ptr %gep0
  %c2 = icmp eq i32 %x, 0
  br i1 %c2, label %latch, label %if

if:
  store i32 %x, ptr %p2
  br label %latch

latch:
  %phi = phi ptr [ %p2, %if ], [ %p2, %loop ] ; ALWAYS %p2
  %y = load i32, ptr %phi
  %gep1 = getelementptr i32, ptr %phip, i64 %iv
  store i32 %y, ptr %gep1
  %iv.next = add nuw i64 %iv, 1
  %exitcond.not = icmp eq i64 %iv.next, %0
  br i1 %exitcond.not, label %exit, label %loop

exit:
  %8 = load i32, ptr %p0
  ret i32 %8
}

attributes #1 = { "target-cpu"="znver4" }
```

If %phi is removed and just replaced with %p2, we don't vectorize:

```llvm
; ModuleID = 'reduced.bc'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux6.12.561-gnu2.41.0"

define i32 @repro_entry(ptr %p0, ptr %p1, ptr %p2, i64 %0, i1 %c1) #1 {
entry:
  %phip = select i1 %c1, ptr %p1, ptr %p0
  br label %loop

loop:
  %iv = phi i64 [ %iv.next, %latch ], [ 0, %entry ]
  %gep0 = getelementptr i32, ptr %p0, i64 %iv
  %x = load i32, ptr %gep0
  %c2 = icmp eq i32 %x, 0
  br i1 %c2, label %latch, label %if

if:
  store i32 %x, ptr %p2
  br label %latch

latch:
  %y = load i32, ptr %p2
  %gep1 = getelementptr i32, ptr %phip, i64 %iv
  store i32 %y, ptr %gep1
  %iv.next = add nuw i64 %iv, 1
  %exitcond.not = icmp eq i64 %iv.next, %0
  br i1 %exitcond.not, label %exit, label %loop

exit:
  %8 = load i32, ptr %p0
  ret i32 %8
}

attributes #1 = { "target-cpu"="znver4" }
```

```
LV: Not vectorizing: We don't allow storing to uniform addresses.
LV: Interleaving disabled by the pass manager
LV: Can't vectorize due to memory conflicts
```

---

### Comment 4 - fhahn

Interesting, I can take a look

---

### Comment 5 - fhahn

Proposed fix: https://github.com/llvm/llvm-project/pull/187023

---

