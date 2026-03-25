# [MIPS64] Miscompilation at O1 in trunk

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/157521

## Body

Testcase:
```llvm
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "mips64el-unknown-linux-gnu"

@.str = constant [9 x i8] c"%016llx\0A\00"
@f = global [1 x i64] zeroinitializer

define i32 @func() {
entry:
  ret i32 0
}

define i32 @o() {
entry:
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %m [label %l]

m:                                                ; preds = %if.then, %entry
  %k.0 = phi i64 [ 3090282716504613, %entry ], [ 0, %if.then ]
  br label %ab

ab:                                               ; preds = %l, %if.then, %asm.fallthrough1, %m
  %k.1 = phi i64 [ %k.0, %m ], [ %k.3, %l ], [ 0, %if.then ], [ 0, %asm.fallthrough1 ]
  %j.1 = phi <8 x i32> [ zeroinitializer, %m ], [ %j.3, %l ], [ zeroinitializer, %if.then ], [ zeroinitializer, %asm.fallthrough1 ]
  %vecins = insertelement <8 x i32> %j.1, i32 0, i64 0
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough1 [label %l]

asm.fallthrough1:                                 ; preds = %ab
  %sext = shl i64 %k.1, 32
  %conv2 = ashr i64 %sext, 32
  store i64 %conv2, ptr @f, align 8
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough3 [label %ab]

asm.fallthrough3:                                 ; preds = %asm.fallthrough1
  %cmp = icmp eq ptr @func, inttoptr (i64 1 to ptr)
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %asm.fallthrough3
  callbr void asm sideeffect "", "!i,!i,!i,~{$1}"()
          to label %ad [label %ab, label %l, label %m]

ad:                                               ; preds = %if.then
  br label %l

l:                                                ; preds = %ad, %if.then, %ab, %entry
  %k.3 = phi i64 [ 0, %if.then ], [ 3090282716504613, %entry ], [ %k.1, %ab ], [ 0, %ad ]
  %j.3 = phi <8 x i32> [ zeroinitializer, %if.then ], [ zeroinitializer, %entry ], [ zeroinitializer, %ab ], [ %vecins, %ad ]
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough10 [label %ab]

asm.fallthrough10:                                ; preds = %l
  %vecext11 = extractelement <8 x i32> %j.3, i64 0
  %conv12 = zext i32 %vecext11 to i64
  store i64 %conv12, ptr @f, align 8
  br label %if.end

if.end:                                           ; preds = %asm.fallthrough10, %asm.fallthrough3
  ret i32 0
}

define i32 @main() {
entry:
  %call = call i32 @o()
  %0 = load i64, ptr @f, align 8
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i64 %0)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```

Compile with O1 and run with qemu-mips64el:
```
000afa988392a225
```

Compile with O0/O2/O3 and run with qemu-mips64el:
```
ffffffff8392a225
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Hongyu Chen (XChy)

<details>
Testcase:
```llvm
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "mips64el-unknown-linux-gnu"

@.str = constant [9 x i8] c"%016llx\0A\00"
@<!-- -->f = global [1 x i64] zeroinitializer

define i32 @<!-- -->func() {
entry:
  ret i32 0
}

define i32 @<!-- -->o() {
entry:
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %m [label %l]

m:                                                ; preds = %if.then, %entry
  %k.0 = phi i64 [ 3090282716504613, %entry ], [ 0, %if.then ]
  br label %ab

ab:                                               ; preds = %l, %if.then, %asm.fallthrough1, %m
  %k.1 = phi i64 [ %k.0, %m ], [ %k.3, %l ], [ 0, %if.then ], [ 0, %asm.fallthrough1 ]
  %j.1 = phi &lt;8 x i32&gt; [ zeroinitializer, %m ], [ %j.3, %l ], [ zeroinitializer, %if.then ], [ zeroinitializer, %asm.fallthrough1 ]
  %vecins = insertelement &lt;8 x i32&gt; %j.1, i32 0, i64 0
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough1 [label %l]

asm.fallthrough1:                                 ; preds = %ab
  %sext = shl i64 %k.1, 32
  %conv2 = ashr i64 %sext, 32
  store i64 %conv2, ptr @<!-- -->f, align 8
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough3 [label %ab]

asm.fallthrough3:                                 ; preds = %asm.fallthrough1
  %cmp = icmp eq ptr @<!-- -->func, inttoptr (i64 1 to ptr)
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %asm.fallthrough3
  callbr void asm sideeffect "", "!i,!i,!i,~{$1}"()
          to label %ad [label %ab, label %l, label %m]

ad:                                               ; preds = %if.then
  br label %l

l:                                                ; preds = %ad, %if.then, %ab, %entry
  %k.3 = phi i64 [ 0, %if.then ], [ 3090282716504613, %entry ], [ %k.1, %ab ], [ 0, %ad ]
  %j.3 = phi &lt;8 x i32&gt; [ zeroinitializer, %if.then ], [ zeroinitializer, %entry ], [ zeroinitializer, %ab ], [ %vecins, %ad ]
  callbr void asm sideeffect "", "!i,~{$1}"()
          to label %asm.fallthrough10 [label %ab]

asm.fallthrough10:                                ; preds = %l
  %vecext11 = extractelement &lt;8 x i32&gt; %j.3, i64 0
  %conv12 = zext i32 %vecext11 to i64
  store i64 %conv12, ptr @<!-- -->f, align 8
  br label %if.end

if.end:                                           ; preds = %asm.fallthrough10, %asm.fallthrough3
  ret i32 0
}

define i32 @<!-- -->main() {
entry:
  %call = call i32 @<!-- -->o()
  %0 = load i64, ptr @<!-- -->f, align 8
  %call1 = call i32 (ptr, ...) @<!-- -->printf(ptr @.str, i64 %0)
  ret i32 0
}

declare i32 @<!-- -->printf(ptr, ...)
```

Compile with O1 and run with qemu-mips64el:
```
000afa988392a225
```

Compile with O0/O2/O3 and run with qemu-mips64el:
```
ffffffff8392a225
```

</details>


---

### Comment 2 - wzssyqa

Do you have the C code?

---

### Comment 3 - XChy

Hi, @wzssyqa. Yes, I have one; the output may be different from the LLVM-IR one, which is reduced further:
```c
#include <stdint.h>
#include <stdio.h>
typedef int32_t a;
typedef uint32_t b;
typedef int64_t c;
typedef uint64_t uint64;
typedef b d __attribute__((vector_size(32)));
typedef a e __attribute__((vector_size(32)));
uint64 f[];
struct {
  uint64_t g
} i, aa;
int func() {}
int ae, af;
int32_t o() {
  e j;
  c k = 3090282716504613;
  asm goto("" : : : : l);
m:
ab:
  j[1] = k;
  int32_t ac = 1;
  asm goto("" : : : : ad);
  *f = j[1];
  k = 0;
  asm goto("" : : : : ab);
  j = (e)__builtin_shufflevector(
      __builtin_shufflevector((d){}, (d){}, 1, 6, 6, 9, 5, 7, 5, 0), (d){}, 5,
      3, 9, 5, 2, 1, 0, 8);
  if (func == ac) {
    asm goto("" : : : : ab, l, m);
  ad:
    i = aa;
  l:
    asm goto("" : : : : ab);
    *f = j[1];
  }
}
int main() {
  o();
  printf("%016llx\n", f[ae]);
}
```

---

