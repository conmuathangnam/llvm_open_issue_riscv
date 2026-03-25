# [DAG] isSplatValue - incorrect handling of undef element masks through binop

**Author:** mark-sed
**URL:** https://github.com/llvm/llvm-project/issues/134602
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-04-15T07:33:43Z

## Body

When working on a miscompile I have extracted this function bellow, which was already processed by LLVM with loop vectorization optimizations and when run in lli it returns 178.
After running it through llvm opt with just `-passes=simplifycfg`, which only converts this to a single basic block the function returns 0.

## Reproducer

```llvm
target triple = "x86_64-unknown-linux-gnu"

@val = internal global i16 0, align 2

define noundef i32 @test(i32 noundef %0) {
gc.safepoint_poll.exit:                           ; preds = %iter.check, %do_safepoint.i
  %.promoted3 = load atomic i16, ptr @val unordered, align 2
  %3 = trunc i32 %0 to i16
  %4 = sub i16 83, %3
  %reass.sub = sub i16 86, %3
  %broadcast.splatinsert = insertelement <16 x i16> poison, i16 %4, i64 0
  %broadcast.splat = shufflevector <16 x i16> %broadcast.splatinsert, <16 x i16> poison, <16 x i32> zeroinitializer
  %broadcast.splatinsert7 = insertelement <16 x i16> poison, i16 %reass.sub, i64 0
  %broadcast.splat8 = shufflevector <16 x i16> %broadcast.splatinsert7, <16 x i16> poison, <16 x i32> zeroinitializer
  %5 = insertelement <16 x i16> <i16 poison, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0>, i16 %.promoted3, i64 0
  br label %vector.body

vector.body:                                      ; preds = %gc.safepoint_poll.exit
  %6 = mul <16 x i16> %broadcast.splat, <i16 28, i16 27, i16 27, i16 27, i16 26, i16 26, i16 26, i16 25, i16 25, i16 25, i16 24, i16 24, i16 24, i16 23, i16 23, i16 23>
  %7 = add <16 x i16> %broadcast.splat8, %5
  %8 = add <16 x i16> %7, %6
  %9 = add <16 x i16> %8, <i16 -1134, i16 -1053, i16 -1053, i16 -1053, i16 -975, i16 -975, i16 -975, i16 -900, i16 -900, i16 -900, i16 -828, i16 -828, i16 -828, i16 -759, i16 -759, i16 -759>
  %10 = mul <16 x i16> %broadcast.splat, <i16 22, i16 22, i16 22, i16 21, i16 21, i16 21, i16 20, i16 20, i16 20, i16 19, i16 19, i16 19, i16 18, i16 18, i16 18, i16 17>
  %11 = add <16 x i16> %broadcast.splat8, %9
  %12 = add <16 x i16> %11, %10
  %13 = add <16 x i16> %12, <i16 -693, i16 -693, i16 -693, i16 -630, i16 -630, i16 -630, i16 -570, i16 -570, i16 -570, i16 -513, i16 -513, i16 -513, i16 -459, i16 -459, i16 -459, i16 -408>
  %14 = mul <16 x i16> %broadcast.splat, <i16 17, i16 17, i16 16, i16 16, i16 16, i16 15, i16 15, i16 15, i16 14, i16 14, i16 14, i16 13, i16 13, i16 13, i16 12, i16 12>
  %15 = add <16 x i16> %broadcast.splat8, %13
  %16 = add <16 x i16> %15, %14
  %17 = add <16 x i16> %16, <i16 -408, i16 -408, i16 -360, i16 -360, i16 -360, i16 -315, i16 -315, i16 -315, i16 -273, i16 -273, i16 -273, i16 -234, i16 -234, i16 -234, i16 -198, i16 -198>
  %18 = mul <16 x i16> %broadcast.splat, <i16 12, i16 11, i16 11, i16 11, i16 10, i16 10, i16 10, i16 9, i16 9, i16 9, i16 8, i16 8, i16 8, i16 7, i16 7, i16 7>
  %19 = add <16 x i16> %broadcast.splat8, %17
  %20 = add <16 x i16> %19, %18
  %21 = add <16 x i16> %20, <i16 -198, i16 -165, i16 -165, i16 -165, i16 -135, i16 -135, i16 -135, i16 -108, i16 -108, i16 -108, i16 -84, i16 -84, i16 -84, i16 -63, i16 -63, i16 -63>
  %22 = mul <16 x i16> %broadcast.splat, <i16 6, i16 6, i16 6, i16 5, i16 5, i16 5, i16 4, i16 4, i16 4, i16 3, i16 3, i16 3, i16 2, i16 2, i16 2, i16 1>
  %23 = add <16 x i16> %broadcast.splat8, %21
  %24 = add <16 x i16> %23, %22
  %25 = add <16 x i16> %24, <i16 -45, i16 -45, i16 -45, i16 -30, i16 -30, i16 -30, i16 -18, i16 -18, i16 -18, i16 -9, i16 -9, i16 -9, i16 -3, i16 -3, i16 -3, i16 0>
  %26 = call i16 @llvm.vector.reduce.add.v16i16(<16 x i16> %25)
  %broadcast.splatinsert9 = insertelement <4 x i16> poison, i16 %4, i64 0
  %broadcast.splat10 = shufflevector <4 x i16> %broadcast.splatinsert9, <4 x i16> poison, <4 x i32> zeroinitializer
  %broadcast.splatinsert11 = insertelement <4 x i16> poison, i16 %reass.sub, i64 0
  %broadcast.splat12 = shufflevector <4 x i16> %broadcast.splatinsert11, <4 x i16> poison, <4 x i32> zeroinitializer
  %27 = insertelement <4 x i16> <i16 poison, i16 0, i16 0, i16 0>, i16 %26, i64 0
  br label %vec.epilog.vector.body

vec.epilog.vector.body:                           ; preds = %vector.body
  %28 = mul <4 x i16> %broadcast.splat10, <i16 1, i16 1, i16 0, i16 0>
  %29 = add <4 x i16> %broadcast.splat12, %27
  %30 = add <4 x i16> %29, %28
  %31 = call i16 @llvm.vector.reduce.add.v4i16(<4 x i16> %30)
  br label %bci_11.preheader

bci_11.preheader:                                 ; preds = %vec.epilog.vector.body
  %32 = add i16 %reass.sub, %31
  store atomic i16 %32, ptr @val unordered, align 2
  %ret_32 = sext i16 %32 to i32
  ret i32 %ret_32
}

define i32 @main() {
    %rval = call i32 @test(i32 0)
    ret i32 %rval
}


; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v16i16(<16 x i16>) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v4i16(<4 x i16>) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

## Runner script:
```sh
opt -passes=simplifycfg test.ll -S > test_simplify.ll
lli test.ll
echo "           test.ll returned: " $?
lli test_simplify.ll
echo "simplified test.ll returned: " $?
```

I can see the following output:
```
           test.ll returned:  178
simplified test.ll returned:  0
```

And here is the `test_simplify.ll`:
```llvm
; ModuleID = 'test.ll'
source_filename = "test.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@val = internal global i16 0, align 2

define noundef i32 @test(i32 noundef %0) {
gc.safepoint_poll.exit:
  %.promoted3 = load atomic i16, ptr @val unordered, align 2
  %1 = trunc i32 %0 to i16
  %2 = sub i16 83, %1
  %reass.sub = sub i16 86, %1
  %broadcast.splatinsert = insertelement <16 x i16> poison, i16 %2, i64 0
  %broadcast.splat = shufflevector <16 x i16> %broadcast.splatinsert, <16 x i16> poison, <16 x i32> zeroinitializer
  %broadcast.splatinsert7 = insertelement <16 x i16> poison, i16 %reass.sub, i64 0
  %broadcast.splat8 = shufflevector <16 x i16> %broadcast.splatinsert7, <16 x i16> poison, <16 x i32> zeroinitializer
  %3 = insertelement <16 x i16> <i16 poison, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0>, i16 %.promoted3, i64 0
  %4 = mul <16 x i16> %broadcast.splat, <i16 28, i16 27, i16 27, i16 27, i16 26, i16 26, i16 26, i16 25, i16 25, i16 25, i16 24, i16 24, i16 24, i16 23, i16 23, i16 23>
  %5 = add <16 x i16> %broadcast.splat8, %3
  %6 = add <16 x i16> %5, %4
  %7 = add <16 x i16> %6, <i16 -1134, i16 -1053, i16 -1053, i16 -1053, i16 -975, i16 -975, i16 -975, i16 -900, i16 -900, i16 -900, i16 -828, i16 -828, i16 -828, i16 -759, i16 -759, i16 -759>
  %8 = mul <16 x i16> %broadcast.splat, <i16 22, i16 22, i16 22, i16 21, i16 21, i16 21, i16 20, i16 20, i16 20, i16 19, i16 19, i16 19, i16 18, i16 18, i16 18, i16 17>
  %9 = add <16 x i16> %broadcast.splat8, %7
  %10 = add <16 x i16> %9, %8
  %11 = add <16 x i16> %10, <i16 -693, i16 -693, i16 -693, i16 -630, i16 -630, i16 -630, i16 -570, i16 -570, i16 -570, i16 -513, i16 -513, i16 -513, i16 -459, i16 -459, i16 -459, i16 -408>
  %12 = mul <16 x i16> %broadcast.splat, <i16 17, i16 17, i16 16, i16 16, i16 16, i16 15, i16 15, i16 15, i16 14, i16 14, i16 14, i16 13, i16 13, i16 13, i16 12, i16 12>
  %13 = add <16 x i16> %broadcast.splat8, %11
  %14 = add <16 x i16> %13, %12
  %15 = add <16 x i16> %14, <i16 -408, i16 -408, i16 -360, i16 -360, i16 -360, i16 -315, i16 -315, i16 -315, i16 -273, i16 -273, i16 -273, i16 -234, i16 -234, i16 -234, i16 -198, i16 -198>
  %16 = mul <16 x i16> %broadcast.splat, <i16 12, i16 11, i16 11, i16 11, i16 10, i16 10, i16 10, i16 9, i16 9, i16 9, i16 8, i16 8, i16 8, i16 7, i16 7, i16 7>
  %17 = add <16 x i16> %broadcast.splat8, %15
  %18 = add <16 x i16> %17, %16
  %19 = add <16 x i16> %18, <i16 -198, i16 -165, i16 -165, i16 -165, i16 -135, i16 -135, i16 -135, i16 -108, i16 -108, i16 -108, i16 -84, i16 -84, i16 -84, i16 -63, i16 -63, i16 -63>
  %20 = mul <16 x i16> %broadcast.splat, <i16 6, i16 6, i16 6, i16 5, i16 5, i16 5, i16 4, i16 4, i16 4, i16 3, i16 3, i16 3, i16 2, i16 2, i16 2, i16 1>
  %21 = add <16 x i16> %broadcast.splat8, %19
  %22 = add <16 x i16> %21, %20
  %23 = add <16 x i16> %22, <i16 -45, i16 -45, i16 -45, i16 -30, i16 -30, i16 -30, i16 -18, i16 -18, i16 -18, i16 -9, i16 -9, i16 -9, i16 -3, i16 -3, i16 -3, i16 0>
  %24 = call i16 @llvm.vector.reduce.add.v16i16(<16 x i16> %23)
  %broadcast.splatinsert9 = insertelement <4 x i16> poison, i16 %2, i64 0
  %broadcast.splat10 = shufflevector <4 x i16> %broadcast.splatinsert9, <4 x i16> poison, <4 x i32> zeroinitializer
  %broadcast.splatinsert11 = insertelement <4 x i16> poison, i16 %reass.sub, i64 0
  %broadcast.splat12 = shufflevector <4 x i16> %broadcast.splatinsert11, <4 x i16> poison, <4 x i32> zeroinitializer
  %25 = insertelement <4 x i16> <i16 poison, i16 0, i16 0, i16 0>, i16 %24, i64 0
  %26 = mul <4 x i16> %broadcast.splat10, <i16 1, i16 1, i16 0, i16 0>
  %27 = add <4 x i16> %broadcast.splat12, %25
  %28 = add <4 x i16> %27, %26
  %29 = call i16 @llvm.vector.reduce.add.v4i16(<4 x i16> %28)
  %30 = add i16 %reass.sub, %29
  store atomic i16 %30, ptr @val unordered, align 2
  %ret_32 = sext i16 %30 to i32
  ret i32 %ret_32
}

define i32 @main() {
  %rval = call i32 @test(i32 0)
  ret i32 %rval
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v16i16(<16 x i16>) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v4i16(<4 x i16>) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

## Comments

### Comment 1 - nikic

Doesn't reproduce under [llubi](https://github.com/dtcxzyw/llvm-ub-aware-interpreter), so this is probably a backend bug, not related to simplifycfg. Keep in mind that lli uses JIT. Using `-force-interpreter` fails because `llvm.vector.reduce.add.v16i16` is not supported.

---

### Comment 2 - mark-sed

> Doesn't reproduce under [llubi](https://github.com/dtcxzyw/llvm-ub-aware-interpreter), so this is probably a backend bug, not related to simplifycfg. Keep in mind that lli uses JIT. Using `-force-interpreter` fails because `llvm.vector.reduce.add.v16i16` is not supported.

I see. I was suspecting LLI being the possible issue here as well, but at the same time, this has arose from looking into a miscompile and using opt-bisect. I'll look into llubi, thanks for the link.

---

### Comment 3 - antoniofrighetto

Transformation seems correct: https://alive2.llvm.org/ce/z/hzctk4. I confirm I managed to reproduce this on a x86-based machine, but not on AArch64 (Apple M2), so likely a backend issue, as already stated by `@nikic`.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Marek Sedláček (mark-sed)

<details>
When working on a miscompile I have extracted this function bellow, which was already processed by LLVM with loop vectorization optimizations and when run in lli it returns 178.
After running it through llvm opt with just `-passes=simplifycfg`, which only converts this to a single basic block the function returns 0.

## Reproducer

```llvm
target triple = "x86_64-unknown-linux-gnu"

@<!-- -->val = internal global i16 0, align 2

define noundef i32 @<!-- -->test(i32 noundef %0) {
gc.safepoint_poll.exit:                           ; preds = %iter.check, %do_safepoint.i
  %.promoted3 = load atomic i16, ptr @<!-- -->val unordered, align 2
  %3 = trunc i32 %0 to i16
  %4 = sub i16 83, %3
  %reass.sub = sub i16 86, %3
  %broadcast.splatinsert = insertelement &lt;16 x i16&gt; poison, i16 %4, i64 0
  %broadcast.splat = shufflevector &lt;16 x i16&gt; %broadcast.splatinsert, &lt;16 x i16&gt; poison, &lt;16 x i32&gt; zeroinitializer
  %broadcast.splatinsert7 = insertelement &lt;16 x i16&gt; poison, i16 %reass.sub, i64 0
  %broadcast.splat8 = shufflevector &lt;16 x i16&gt; %broadcast.splatinsert7, &lt;16 x i16&gt; poison, &lt;16 x i32&gt; zeroinitializer
  %5 = insertelement &lt;16 x i16&gt; &lt;i16 poison, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0&gt;, i16 %.promoted3, i64 0
  br label %vector.body

vector.body:                                      ; preds = %gc.safepoint_poll.exit
  %6 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 28, i16 27, i16 27, i16 27, i16 26, i16 26, i16 26, i16 25, i16 25, i16 25, i16 24, i16 24, i16 24, i16 23, i16 23, i16 23&gt;
  %7 = add &lt;16 x i16&gt; %broadcast.splat8, %5
  %8 = add &lt;16 x i16&gt; %7, %6
  %9 = add &lt;16 x i16&gt; %8, &lt;i16 -1134, i16 -1053, i16 -1053, i16 -1053, i16 -975, i16 -975, i16 -975, i16 -900, i16 -900, i16 -900, i16 -828, i16 -828, i16 -828, i16 -759, i16 -759, i16 -759&gt;
  %10 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 22, i16 22, i16 22, i16 21, i16 21, i16 21, i16 20, i16 20, i16 20, i16 19, i16 19, i16 19, i16 18, i16 18, i16 18, i16 17&gt;
  %11 = add &lt;16 x i16&gt; %broadcast.splat8, %9
  %12 = add &lt;16 x i16&gt; %11, %10
  %13 = add &lt;16 x i16&gt; %12, &lt;i16 -693, i16 -693, i16 -693, i16 -630, i16 -630, i16 -630, i16 -570, i16 -570, i16 -570, i16 -513, i16 -513, i16 -513, i16 -459, i16 -459, i16 -459, i16 -408&gt;
  %14 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 17, i16 17, i16 16, i16 16, i16 16, i16 15, i16 15, i16 15, i16 14, i16 14, i16 14, i16 13, i16 13, i16 13, i16 12, i16 12&gt;
  %15 = add &lt;16 x i16&gt; %broadcast.splat8, %13
  %16 = add &lt;16 x i16&gt; %15, %14
  %17 = add &lt;16 x i16&gt; %16, &lt;i16 -408, i16 -408, i16 -360, i16 -360, i16 -360, i16 -315, i16 -315, i16 -315, i16 -273, i16 -273, i16 -273, i16 -234, i16 -234, i16 -234, i16 -198, i16 -198&gt;
  %18 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 12, i16 11, i16 11, i16 11, i16 10, i16 10, i16 10, i16 9, i16 9, i16 9, i16 8, i16 8, i16 8, i16 7, i16 7, i16 7&gt;
  %19 = add &lt;16 x i16&gt; %broadcast.splat8, %17
  %20 = add &lt;16 x i16&gt; %19, %18
  %21 = add &lt;16 x i16&gt; %20, &lt;i16 -198, i16 -165, i16 -165, i16 -165, i16 -135, i16 -135, i16 -135, i16 -108, i16 -108, i16 -108, i16 -84, i16 -84, i16 -84, i16 -63, i16 -63, i16 -63&gt;
  %22 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 6, i16 6, i16 6, i16 5, i16 5, i16 5, i16 4, i16 4, i16 4, i16 3, i16 3, i16 3, i16 2, i16 2, i16 2, i16 1&gt;
  %23 = add &lt;16 x i16&gt; %broadcast.splat8, %21
  %24 = add &lt;16 x i16&gt; %23, %22
  %25 = add &lt;16 x i16&gt; %24, &lt;i16 -45, i16 -45, i16 -45, i16 -30, i16 -30, i16 -30, i16 -18, i16 -18, i16 -18, i16 -9, i16 -9, i16 -9, i16 -3, i16 -3, i16 -3, i16 0&gt;
  %26 = call i16 @<!-- -->llvm.vector.reduce.add.v16i16(&lt;16 x i16&gt; %25)
  %broadcast.splatinsert9 = insertelement &lt;4 x i16&gt; poison, i16 %4, i64 0
  %broadcast.splat10 = shufflevector &lt;4 x i16&gt; %broadcast.splatinsert9, &lt;4 x i16&gt; poison, &lt;4 x i32&gt; zeroinitializer
  %broadcast.splatinsert11 = insertelement &lt;4 x i16&gt; poison, i16 %reass.sub, i64 0
  %broadcast.splat12 = shufflevector &lt;4 x i16&gt; %broadcast.splatinsert11, &lt;4 x i16&gt; poison, &lt;4 x i32&gt; zeroinitializer
  %27 = insertelement &lt;4 x i16&gt; &lt;i16 poison, i16 0, i16 0, i16 0&gt;, i16 %26, i64 0
  br label %vec.epilog.vector.body

vec.epilog.vector.body:                           ; preds = %vector.body
  %28 = mul &lt;4 x i16&gt; %broadcast.splat10, &lt;i16 1, i16 1, i16 0, i16 0&gt;
  %29 = add &lt;4 x i16&gt; %broadcast.splat12, %27
  %30 = add &lt;4 x i16&gt; %29, %28
  %31 = call i16 @<!-- -->llvm.vector.reduce.add.v4i16(&lt;4 x i16&gt; %30)
  br label %bci_11.preheader

bci_11.preheader:                                 ; preds = %vec.epilog.vector.body
  %32 = add i16 %reass.sub, %31
  store atomic i16 %32, ptr @<!-- -->val unordered, align 2
  %ret_32 = sext i16 %32 to i32
  ret i32 %ret_32
}

define i32 @<!-- -->main() {
    %rval = call i32 @<!-- -->test(i32 0)
    ret i32 %rval
}


; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @<!-- -->llvm.vector.reduce.add.v16i16(&lt;16 x i16&gt;) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @<!-- -->llvm.vector.reduce.add.v4i16(&lt;4 x i16&gt;) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

## Runner script:
```sh
opt -passes=simplifycfg test.ll -S &gt; test_simplify.ll
lli test.ll
echo "           test.ll returned: " $?
lli test_simplify.ll
echo "simplified test.ll returned: " $?
```

I can see the following output:
```
           test.ll returned:  178
simplified test.ll returned:  0
```

And here is the `test_simplify.ll`:
```llvm
; ModuleID = 'test.ll'
source_filename = "test.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@<!-- -->val = internal global i16 0, align 2

define noundef i32 @<!-- -->test(i32 noundef %0) {
gc.safepoint_poll.exit:
  %.promoted3 = load atomic i16, ptr @<!-- -->val unordered, align 2
  %1 = trunc i32 %0 to i16
  %2 = sub i16 83, %1
  %reass.sub = sub i16 86, %1
  %broadcast.splatinsert = insertelement &lt;16 x i16&gt; poison, i16 %2, i64 0
  %broadcast.splat = shufflevector &lt;16 x i16&gt; %broadcast.splatinsert, &lt;16 x i16&gt; poison, &lt;16 x i32&gt; zeroinitializer
  %broadcast.splatinsert7 = insertelement &lt;16 x i16&gt; poison, i16 %reass.sub, i64 0
  %broadcast.splat8 = shufflevector &lt;16 x i16&gt; %broadcast.splatinsert7, &lt;16 x i16&gt; poison, &lt;16 x i32&gt; zeroinitializer
  %3 = insertelement &lt;16 x i16&gt; &lt;i16 poison, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0, i16 0&gt;, i16 %.promoted3, i64 0
  %4 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 28, i16 27, i16 27, i16 27, i16 26, i16 26, i16 26, i16 25, i16 25, i16 25, i16 24, i16 24, i16 24, i16 23, i16 23, i16 23&gt;
  %5 = add &lt;16 x i16&gt; %broadcast.splat8, %3
  %6 = add &lt;16 x i16&gt; %5, %4
  %7 = add &lt;16 x i16&gt; %6, &lt;i16 -1134, i16 -1053, i16 -1053, i16 -1053, i16 -975, i16 -975, i16 -975, i16 -900, i16 -900, i16 -900, i16 -828, i16 -828, i16 -828, i16 -759, i16 -759, i16 -759&gt;
  %8 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 22, i16 22, i16 22, i16 21, i16 21, i16 21, i16 20, i16 20, i16 20, i16 19, i16 19, i16 19, i16 18, i16 18, i16 18, i16 17&gt;
  %9 = add &lt;16 x i16&gt; %broadcast.splat8, %7
  %10 = add &lt;16 x i16&gt; %9, %8
  %11 = add &lt;16 x i16&gt; %10, &lt;i16 -693, i16 -693, i16 -693, i16 -630, i16 -630, i16 -630, i16 -570, i16 -570, i16 -570, i16 -513, i16 -513, i16 -513, i16 -459, i16 -459, i16 -459, i16 -408&gt;
  %12 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 17, i16 17, i16 16, i16 16, i16 16, i16 15, i16 15, i16 15, i16 14, i16 14, i16 14, i16 13, i16 13, i16 13, i16 12, i16 12&gt;
  %13 = add &lt;16 x i16&gt; %broadcast.splat8, %11
  %14 = add &lt;16 x i16&gt; %13, %12
  %15 = add &lt;16 x i16&gt; %14, &lt;i16 -408, i16 -408, i16 -360, i16 -360, i16 -360, i16 -315, i16 -315, i16 -315, i16 -273, i16 -273, i16 -273, i16 -234, i16 -234, i16 -234, i16 -198, i16 -198&gt;
  %16 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 12, i16 11, i16 11, i16 11, i16 10, i16 10, i16 10, i16 9, i16 9, i16 9, i16 8, i16 8, i16 8, i16 7, i16 7, i16 7&gt;
  %17 = add &lt;16 x i16&gt; %broadcast.splat8, %15
  %18 = add &lt;16 x i16&gt; %17, %16
  %19 = add &lt;16 x i16&gt; %18, &lt;i16 -198, i16 -165, i16 -165, i16 -165, i16 -135, i16 -135, i16 -135, i16 -108, i16 -108, i16 -108, i16 -84, i16 -84, i16 -84, i16 -63, i16 -63, i16 -63&gt;
  %20 = mul &lt;16 x i16&gt; %broadcast.splat, &lt;i16 6, i16 6, i16 6, i16 5, i16 5, i16 5, i16 4, i16 4, i16 4, i16 3, i16 3, i16 3, i16 2, i16 2, i16 2, i16 1&gt;
  %21 = add &lt;16 x i16&gt; %broadcast.splat8, %19
  %22 = add &lt;16 x i16&gt; %21, %20
  %23 = add &lt;16 x i16&gt; %22, &lt;i16 -45, i16 -45, i16 -45, i16 -30, i16 -30, i16 -30, i16 -18, i16 -18, i16 -18, i16 -9, i16 -9, i16 -9, i16 -3, i16 -3, i16 -3, i16 0&gt;
  %24 = call i16 @<!-- -->llvm.vector.reduce.add.v16i16(&lt;16 x i16&gt; %23)
  %broadcast.splatinsert9 = insertelement &lt;4 x i16&gt; poison, i16 %2, i64 0
  %broadcast.splat10 = shufflevector &lt;4 x i16&gt; %broadcast.splatinsert9, &lt;4 x i16&gt; poison, &lt;4 x i32&gt; zeroinitializer
  %broadcast.splatinsert11 = insertelement &lt;4 x i16&gt; poison, i16 %reass.sub, i64 0
  %broadcast.splat12 = shufflevector &lt;4 x i16&gt; %broadcast.splatinsert11, &lt;4 x i16&gt; poison, &lt;4 x i32&gt; zeroinitializer
  %25 = insertelement &lt;4 x i16&gt; &lt;i16 poison, i16 0, i16 0, i16 0&gt;, i16 %24, i64 0
  %26 = mul &lt;4 x i16&gt; %broadcast.splat10, &lt;i16 1, i16 1, i16 0, i16 0&gt;
  %27 = add &lt;4 x i16&gt; %broadcast.splat12, %25
  %28 = add &lt;4 x i16&gt; %27, %26
  %29 = call i16 @<!-- -->llvm.vector.reduce.add.v4i16(&lt;4 x i16&gt; %28)
  %30 = add i16 %reass.sub, %29
  store atomic i16 %30, ptr @<!-- -->val unordered, align 2
  %ret_32 = sext i16 %30 to i32
  ret i32 %ret_32
}

define i32 @<!-- -->main() {
  %rval = call i32 @<!-- -->test(i32 0)
  ret i32 %rval
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @<!-- -->llvm.vector.reduce.add.v16i16(&lt;16 x i16&gt;) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @<!-- -->llvm.vector.reduce.add.v4i16(&lt;4 x i16&gt;) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```
</details>


---

### Comment 5 - mark-sed

Here is the test reduced by llvm-reduce
```llvm
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i16 %0) {
gc.safepoint_poll.exit:
  %broadcast.splatinsert9 = insertelement <4 x i16> zeroinitializer, i16 %0, i64 0
  br label %vec.epilog.vector.body

vec.epilog.vector.body:                           ; preds = %gc.safepoint_poll.exit
  %1 = mul <4 x i16> %broadcast.splatinsert9, <i16 1, i16 1, i16 0, i16 0>
  %2 = or <4 x i16> splat (i16 1), %1
  %3 = call i16 @llvm.vector.reduce.add.v4i16(<4 x i16> %2)
  %ret_32 = sext i16 %3 to i32
  ret i32 %ret_32
}

define i32 @main() {
  %rval1 = call i32 @test(i16 0)
  ret i32 %rval1
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v4i16(<4 x i16>) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

And here is the output from llc:
```s
	.file	"test.ll"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          # -- Begin function test
.LCPI0_0:
	.short	1                               # 0x1
	.short	1                               # 0x1
	.short	1                               # 0x1
	.short	1                               # 0x1
	.zero	2
	.zero	2
	.zero	2
	.zero	2
	.text
	.globl	test
	.p2align	4
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# %bb.0:                                # %gc.safepoint_poll.exit
	movzwl	%di, %eax
	movd	%eax, %xmm0
	xorps	%xmm1, %xmm1
	movss	%xmm0, %xmm1                    # xmm1 = xmm0[0],xmm1[1,2,3]
	orps	.LCPI0_0(%rip), %xmm1
	pshufd	$85, %xmm1, %xmm0               # xmm0 = xmm1[1,1,1,1]
	paddw	%xmm1, %xmm0
	movdqa	%xmm0, %xmm1
	psrld	$16, %xmm1
	paddw	%xmm0, %xmm1
	movd	%xmm1, %eax
	cwtl
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	xorl	%edi, %edi
	callq	test@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits

```

Here is the same function just simplified into 1 BB by simplifycfg:
```llvm
; ModuleID = 'test.ll'
source_filename = "test.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i16 %0) {
gc.safepoint_poll.exit:
  %broadcast.splatinsert9 = insertelement <4 x i16> zeroinitializer, i16 %0, i64 0
  %1 = mul <4 x i16> %broadcast.splatinsert9, <i16 1, i16 1, i16 0, i16 0>
  %2 = or <4 x i16> splat (i16 1), %1
  %3 = call i16 @llvm.vector.reduce.add.v4i16(<4 x i16> %2)
  %ret_32 = sext i16 %3 to i32
  ret i32 %ret_32
}

define i32 @main() {
  %rval1 = call i32 @test(i16 0)
  ret i32 %rval1
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v4i16(<4 x i16>) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

```

After simplify which just makes this into 1 basic block this is the assembly:
```s
	.file	"test.ll"
	.text
	.globl	test                            # -- Begin function test
	.p2align	4
	.type	test,@function
test:                                   # @test
	.cfi_startproc
# %bb.0:                                # %gc.safepoint_poll.exit
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	xorl	%edi, %edi
	callq	test@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
```
It just returns 0 without any calculations (the above returns 4).

Using print after all this removal of all the code is present after `X86 DAG->DAG Instruction Selection (x86-isel)`.
I did some autotriaging for the possible faulty commit and the code generates correctly before https://github.com/llvm/llvm-project/commit/d92ce344bf641e6bb025b41b3f1a77dd25e2b3e9 (by @RKSimon), which is a revert of https://github.com/llvm/llvm-project/commit/faecc736e2ac3cd8c77bebf41b1ed2e2d8cb575f.
Unfortunately I cannot run this on the original commit because of parsing issues of the IR (with splat).

---

### Comment 6 - RKSimon

Got a tentative fix - its a very old bug inside isSplatValue's UNDEF value handling through binops

---

### Comment 7 - mark-sed

@RKSimon Many thanks for the quick fix.

---

