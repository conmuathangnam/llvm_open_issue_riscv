# Assertion `(!R2 || (Kind <= REX2 || Kind == EVEX)) && "invalid setting"

**Author:** ashermancinelli
**URL:** https://github.com/llvm/llvm-project/issues/129740
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-03-08T19:23:57Z

## Body

```
> clang++ -march=znver4 -v -O3 -c reduced.ll
llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp:173: 
void {anonymous}::X86OpcodePrefixHelper::setR2(unsigned int):
Assertion `(!R2 || (Kind <= REX2 || Kind == EVEX)) && "invalid setting"' failed.
```

```
;; reduced.ll
; ModuleID = '<bc file>'
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@extfloat1 = external global float
@extfloat2 = external global [220 x [250 x float]]

define void @foo(ptr %0, i32 %1, i64 %2, float %3, float %4, ptr %5, i64 %6, i1 %7, ptr %8) {
  %10 = alloca [0 x [0 x [0 x float]]], i32 0, align 4
  %11 = alloca float, i64 %2, align 4
  %12 = alloca float, i64 %2, align 4
  call void @bar(ptr %10)
  br label %13

13:                                               ; preds = %40, %9
  br label %14

14:                                               ; preds = %35, %13
  %.027 = phi float [ 0.000000e+00, %13 ], [ %.1, %35 ]
  %15 = phi i32 [ %1, %13 ], [ %19, %35 ]
  %16 = phi i64 [ %2, %13 ], [ %39, %35 ]
  %17 = icmp sgt i64 %16, 0
  br i1 %17, label %18, label %40

18:                                               ; preds = %14
  %19 = add i32 %15, 1
  %20 = sext i32 %15 to i64
  %21 = getelementptr float, ptr %11, i64 %20
  %22 = load float, ptr %21, align 4
  %23 = sext i32 %19 to i64
  %24 = getelementptr float, ptr %11, i64 %23
  store float %22, ptr %24, align 4
  call void @baz(ptr %24, ptr %10, ptr null)
  %25 = load float, ptr %10, align 4
  %26 = getelementptr float, ptr %5, i64 %23
  %27 = load float, ptr %26, align 4
  %28 = getelementptr float, ptr null, i64 %23
  store float 0.000000e+00, ptr %28, align 4
  br i1 %7, label %29, label %35

29:                                               ; preds = %18
  %30 = fadd float %3, %27
  %31 = fmul float %25, %30
  %32 = fdiv arcp float %31, %3
  %33 = fmul float %32, %4
  %34 = fadd reassoc float %.027, %33
  br label %35

35:                                               ; preds = %29, %18
  %.1 = phi float [ %34, %29 ], [ %.027, %18 ]
  %36 = getelementptr float, ptr %8, i64 %23
  %37 = getelementptr float, ptr %12, i64 %6
  call void @qux(ptr %0, ptr %36, ptr %37)
  %38 = load float, ptr %12, align 4
  store float %38, ptr %11, align 4
  %39 = add i64 %16, -1
  br label %14

40:                                               ; preds = %14
  %41 = fcmp ogt float %.027, 0.000000e+00
  br i1 %41, label %42, label %13

42:                                               ; preds = %40
  ret void
}

declare void @bar(ptr)

define void @baz(ptr %0, ptr %1, ptr %extfloat2) {
  %3 = load float, ptr null, align 4
  %4 = call float @llvm.trunc.f32(float %3)
  %5 = fsub float 0.000000e+00, %4
  %6 = load float, ptr %0, align 4
  %7 = load float, ptr @extfloat1, align 4
  %8 = fmul float %6, %7
  %9 = fptosi float %6 to i32
  %10 = add i32 %9, 1
  %11 = load float, ptr @extfloat2, align 4
  %12 = load float, ptr getelementptr (i8, ptr @extfloat2, i64 -4), align 4
  %13 = sext i32 %10 to i64
  %14 = getelementptr float, ptr %extfloat2, i64 %13
  %15 = getelementptr i8, ptr %14, i64 -4
  %16 = load float, ptr %15, align 4
  %17 = fsub float %12, 1.000000e+00
  %18 = fmul float %17, %6
  %19 = fmul float %6, %5
  %20 = fadd float %18, %19
  %21 = fadd float %12, %16
  %22 = fsub float %11, %21
  %23 = fadd float %22, 0.000000e+00
  %24 = fmul float %23, %8
  %25 = fmul float %24, 0.000000e+00
  %26 = fadd float %20, %25
  store float %26, ptr %1, align 4
  ret void
}

define void @qux(ptr %0, ptr %1, ptr %2) {
  %4 = load float, ptr %1, align 4
  %5 = load float, ptr %0, align 4
  %6 = fdiv ninf arcp float %5, %4
  %7 = fptosi float %6 to i32
  %8 = add i32 %7, 1
  %9 = sext i32 %8 to i64
  %10 = getelementptr float, ptr null, i64 %9
  %11 = getelementptr i8, ptr %10, i64 -4
  %12 = load float, ptr %11, align 4
  %13 = fneg float %12
  %14 = fmul reassoc nsz float %5, %13
  %15 = fdiv ninf arcp contract float %4, %14
  %16 = call float @llvm.exp.f32(float %15)
  %17 = fmul float %16, 0.000000e+00
  store float %17, ptr %2, align 4
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.trunc.f32(float) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.exp.f32(float) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Asher Mancinelli (ashermancinelli)

<details>
```
&gt; clang++ -march=znver4 -v -O3 -c reduced.ll
llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp:173: 
void {anonymous}::X86OpcodePrefixHelper::setR2(unsigned int):
Assertion `(!R2 || (Kind &lt;= REX2 || Kind == EVEX)) &amp;&amp; "invalid setting"' failed.
```

```
;; reduced.ll
; ModuleID = '&lt;bc file&gt;'
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@<!-- -->extfloat1 = external global float
@<!-- -->extfloat2 = external global [220 x [250 x float]]

define void @<!-- -->foo(ptr %0, i32 %1, i64 %2, float %3, float %4, ptr %5, i64 %6, i1 %7, ptr %8) {
  %10 = alloca [0 x [0 x [0 x float]]], i32 0, align 4
  %11 = alloca float, i64 %2, align 4
  %12 = alloca float, i64 %2, align 4
  call void @<!-- -->bar(ptr %10)
  br label %13

13:                                               ; preds = %40, %9
  br label %14

14:                                               ; preds = %35, %13
  %.027 = phi float [ 0.000000e+00, %13 ], [ %.1, %35 ]
  %15 = phi i32 [ %1, %13 ], [ %19, %35 ]
  %16 = phi i64 [ %2, %13 ], [ %39, %35 ]
  %17 = icmp sgt i64 %16, 0
  br i1 %17, label %18, label %40

18:                                               ; preds = %14
  %19 = add i32 %15, 1
  %20 = sext i32 %15 to i64
  %21 = getelementptr float, ptr %11, i64 %20
  %22 = load float, ptr %21, align 4
  %23 = sext i32 %19 to i64
  %24 = getelementptr float, ptr %11, i64 %23
  store float %22, ptr %24, align 4
  call void @<!-- -->baz(ptr %24, ptr %10, ptr null)
  %25 = load float, ptr %10, align 4
  %26 = getelementptr float, ptr %5, i64 %23
  %27 = load float, ptr %26, align 4
  %28 = getelementptr float, ptr null, i64 %23
  store float 0.000000e+00, ptr %28, align 4
  br i1 %7, label %29, label %35

29:                                               ; preds = %18
  %30 = fadd float %3, %27
  %31 = fmul float %25, %30
  %32 = fdiv arcp float %31, %3
  %33 = fmul float %32, %4
  %34 = fadd reassoc float %.027, %33
  br label %35

35:                                               ; preds = %29, %18
  %.1 = phi float [ %34, %29 ], [ %.027, %18 ]
  %36 = getelementptr float, ptr %8, i64 %23
  %37 = getelementptr float, ptr %12, i64 %6
  call void @<!-- -->qux(ptr %0, ptr %36, ptr %37)
  %38 = load float, ptr %12, align 4
  store float %38, ptr %11, align 4
  %39 = add i64 %16, -1
  br label %14

40:                                               ; preds = %14
  %41 = fcmp ogt float %.027, 0.000000e+00
  br i1 %41, label %42, label %13

42:                                               ; preds = %40
  ret void
}

declare void @<!-- -->bar(ptr)

define void @<!-- -->baz(ptr %0, ptr %1, ptr %extfloat2) {
  %3 = load float, ptr null, align 4
  %4 = call float @<!-- -->llvm.trunc.f32(float %3)
  %5 = fsub float 0.000000e+00, %4
  %6 = load float, ptr %0, align 4
  %7 = load float, ptr @<!-- -->extfloat1, align 4
  %8 = fmul float %6, %7
  %9 = fptosi float %6 to i32
  %10 = add i32 %9, 1
  %11 = load float, ptr @<!-- -->extfloat2, align 4
  %12 = load float, ptr getelementptr (i8, ptr @<!-- -->extfloat2, i64 -4), align 4
  %13 = sext i32 %10 to i64
  %14 = getelementptr float, ptr %extfloat2, i64 %13
  %15 = getelementptr i8, ptr %14, i64 -4
  %16 = load float, ptr %15, align 4
  %17 = fsub float %12, 1.000000e+00
  %18 = fmul float %17, %6
  %19 = fmul float %6, %5
  %20 = fadd float %18, %19
  %21 = fadd float %12, %16
  %22 = fsub float %11, %21
  %23 = fadd float %22, 0.000000e+00
  %24 = fmul float %23, %8
  %25 = fmul float %24, 0.000000e+00
  %26 = fadd float %20, %25
  store float %26, ptr %1, align 4
  ret void
}

define void @<!-- -->qux(ptr %0, ptr %1, ptr %2) {
  %4 = load float, ptr %1, align 4
  %5 = load float, ptr %0, align 4
  %6 = fdiv ninf arcp float %5, %4
  %7 = fptosi float %6 to i32
  %8 = add i32 %7, 1
  %9 = sext i32 %8 to i64
  %10 = getelementptr float, ptr null, i64 %9
  %11 = getelementptr i8, ptr %10, i64 -4
  %12 = load float, ptr %11, align 4
  %13 = fneg float %12
  %14 = fmul reassoc nsz float %5, %13
  %15 = fdiv ninf arcp contract float %4, %14
  %16 = call float @<!-- -->llvm.exp.f32(float %15)
  %17 = fmul float %16, 0.000000e+00
  store float %17, ptr %2, align 4
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @<!-- -->llvm.trunc.f32(float) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @<!-- -->llvm.exp.f32(float) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```
</details>


---

### Comment 2 - phoebewang

No crash on both trunk and 20.1.0

---

### Comment 3 - ashermancinelli

I'm sorry, could you confirm again that you saw no crash with those flags? I tried again this morning and see the same thing on a new build:
```
> clang++ -march=znver4 -v -O3 -c reduced.ll
llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp:173: 
void {anonymous}::X86OpcodePrefixHelper::setR2(unsigned int):
Assertion `(!R2 || (Kind <= REX2 || Kind == EVEX)) && "invalid setting"' failed.

> clang++ --version
clang version 21.0.0git (https://github.com/llvm/llvm-project d1bcac06c7c11699e7931bb7315a1bb9b9784179)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir:
Build config: +assertions
```
on this commit
```
'd1bcac06c7c1 Wed Mar 5 18:25:06 2025 +0800  [Clang] Bump `__cpp_constexpr` to `202002L` in C++20 mode (#129814)'
```


---

### Comment 4 - ashermancinelli

I can't get this to reproduce in compiler explorer, we will try to get a better reproducer. Thanks for looking at this!

---

### Comment 5 - vzakhari

It seems to be reproducing only with `-DLLVM_ENABLE_ASSERTIONS=ON` build.

---

### Comment 6 - vzakhari

The failure resolved after reverting https://github.com/llvm/llvm-project/commit/8476a5d480304bf7bd934c660a159e1c6906a69d

@arsenm can you please take a look?

---

### Comment 7 - vzakhari

> I can't get this to reproduce in compiler explorer, we will try to get a better reproducer. Thanks for looking at this!

This is just because it only reproduces with `-c` and not with `-S`.

---

### Comment 8 - RKSimon

@vzakhari Does #129727 fix the failure>

---

### Comment 9 - vzakhari

> [@vzakhari](https://github.com/vzakhari) Does [#129727](https://github.com/llvm/llvm-project/pull/129727) fix the failure>

Yes, it does. Thank you!

---

### Comment 10 - RKSimon

Could this be an EXPENSIVE_CHECKS only failure?

---

### Comment 11 - phoebewang

> I'm sorry, could you confirm again that you saw no crash with those flags? I tried again this morning and see the same thing on a new build:
> 
> ```
> > clang++ -march=znver4 -v -O3 -c reduced.ll
> llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp:173: 
> void {anonymous}::X86OpcodePrefixHelper::setR2(unsigned int):
> Assertion `(!R2 || (Kind <= REX2 || Kind == EVEX)) && "invalid setting"' failed.
> 
> > clang++ --version
> clang version 21.0.0git (https://github.com/llvm/llvm-project d1bcac06c7c11699e7931bb7315a1bb9b9784179)
> Target: x86_64-unknown-linux-gnu
> Thread model: posix
> InstalledDir:
> Build config: +assertions
> ```
> 
> on this commit
> 
> ```
> 'd1bcac06c7c1 Wed Mar 5 18:25:06 2025 +0800  [Clang] Bump `__cpp_constexpr` to `202002L` in C++20 mode (#129814)'
> ```

Sorry for the noisy, I didn't sync the code before checking it.

---

### Comment 12 - RKSimon

@ashermancinelli can you confirm this is fixed in trunk please?

---

### Comment 13 - ashermancinelli

Fixed, thank you! Tested with 46d218d1af5d285a646a1e5d3be6a43940fb4a9d

---

