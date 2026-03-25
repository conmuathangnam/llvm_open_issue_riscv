# [SLPVectorizer] Miscompile in SLP vectorizer

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/135410
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-04-11T18:43:33Z

## Body

The following IR
```llvm
define <4 x float> @foo(i64 %0) {
entry:
  %1 = trunc i64 0 to i32
  %2 = sitofp i32 %1 to float
  %3 = sitofp i64 0 to float
  %4 = fadd float %3, %2
  %to_max.i = fcmp ogt float %4, 0.000000e+00
  %5 = trunc i64 0 to i32
  %6 = sitofp i32 %5 to float
  %7 = sitofp i64 0 to float
  %8 = fadd float %7, %6
  %to_max.i.1 = fcmp ogt float %8, 0.000000e+00
  %9 = trunc i64 %0 to i32
  %10 = sitofp i32 %9 to float
  %11 = sitofp i64 %0 to float
  %12 = fadd float %11, %10
  %to_max.i.2 = fcmp ogt float %12, 0.000000e+00
  %13 = trunc i64 0 to i32
  %14 = sitofp i32 %13 to float
  %15 = sitofp i64 0 to float
  %16 = fadd float %15, %14
  %to_max.i.3 = fcmp ogt float %16, 0.000000e+00
  %r0 = insertelement <4 x float> poison, float %4, i32 0
  %r1 = insertelement <4 x float> %r0, float %8, i32 1
  %r2 = insertelement <4 x float> %r1, float %12, i32 2
  %r3 = insertelement <4 x float> %r2, float %16, i32 3
  ret <4 x float> %r3
}
```
Is optimized by `opt -passes=slp-vectorizer` to
```llvm
define <4 x float> @foo(i64 %0) {
entry:
  %1 = trunc i64 %0 to i32
  %2 = insertelement <4 x i32> <i32 0, i32 0, i32 poison, i32 0>, i32 %1, i32 2
  %3 = sext <4 x i32> %2 to <4 x i64>
  %4 = sitofp <4 x i64> %3 to <4 x float>
  %5 = sitofp <4 x i32> %2 to <4 x float>
  %6 = fadd <4 x float> %4, %5
  %7 = fcmp ogt <4 x float> %6, zeroinitializer
  ret <4 x float> %6
}
```
Which is incorrect as can be seen when passing `i64 2147516416` as an argument to this function.
Third element of the returned vector is `65536.000000` before optimization and `-4294901760.000000` after.

Checked by running `llvm-lli` on
```llvm
target triple = "x86_64-unknown-linux-gnu"

declare noundef i32 @printf(ptr noundef readonly captures(none), ...)
@str = private unnamed_addr constant [8 x i8] c"%f, %f\0A\00", align 1

define <4 x float> @foo(i64 %0) {
entry:
  %1 = trunc i64 0 to i32
  %2 = sitofp i32 %1 to float
  %3 = sitofp i64 0 to float
  %4 = fadd float %3, %2
  %to_max.i = fcmp ogt float %4, 0.000000e+00
  %5 = trunc i64 0 to i32
  %6 = sitofp i32 %5 to float
  %7 = sitofp i64 0 to float
  %8 = fadd float %7, %6
  %to_max.i.1 = fcmp ogt float %8, 0.000000e+00
  %9 = trunc i64 %0 to i32
  %10 = sitofp i32 %9 to float
  %11 = sitofp i64 %0 to float
  %12 = fadd float %11, %10
  %to_max.i.2 = fcmp ogt float %12, 0.000000e+00
  %13 = trunc i64 0 to i32
  %14 = sitofp i32 %13 to float
  %15 = sitofp i64 0 to float
  %16 = fadd float %15, %14
  %to_max.i.3 = fcmp ogt float %16, 0.000000e+00
  %r0 = insertelement <4 x float> poison, float %4, i32 0
  %r1 = insertelement <4 x float> %r0, float %8, i32 1
  %r2 = insertelement <4 x float> %r1, float %12, i32 2
  %r3 = insertelement <4 x float> %r2, float %16, i32 3
  ret <4 x float> %r3
}

define <4 x float> @bar(i64 %0) {
entry:
  %1 = trunc i64 %0 to i32
  %2 = insertelement <4 x i32> <i32 0, i32 0, i32 poison, i32 0>, i32 %1, i32 2
  %3 = sext <4 x i32> %2 to <4 x i64>
  %4 = sitofp <4 x i64> %3 to <4 x float>
  %5 = sitofp <4 x i32> %2 to <4 x float>
  %6 = fadd <4 x float> %4, %5
  %7 = fcmp ogt <4 x float> %6, zeroinitializer
  ret <4 x float> %6
}

define i32 @main() {
  %r1 = call <4 x float> @foo(i64 2147516416)
  %r10 = extractelement <4 x float> %r1, i32 2
  %res1 = fpext float %r10 to double
  %r2 = call <4 x float> @bar(i64 2147516416)
  %r20 = extractelement <4 x float> %r2, i32 2
  %res2 = fpext float %r20 to double
  %n = call i32 (ptr, ...) @printf(ptr @str, double %res1, double %res2)
  ret i32 0
}
```
bisect pointed to https://github.com/llvm/llvm-project/commit/6425f2d66740b84fc3027b649cd4baf660c384e8 commit.

