# Miscompile within optimization pipeline

**Author:** WillFroom
**URL:** https://github.com/llvm/llvm-project/issues/125259
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-01-31T19:24:10Z

## Body

Hi All, we are seeing a miscompile somewhere in the optimization pipeline when using O3, we do not see the issue with O2 (I am currently bisecting the commit and will update when I have it)

I've copied the input and optimized output below (and godbolt link https://godbolt.org/z/ae6heox5M)

We are seeing undefined behaviour in the optimized output: the second half the the variable `%21` is `poison` which are then passed to a `store` (via a `icmp` and `zext`)

<details>
<summary>Input</summary>

```llvm
source_filename = "example"
target triple = "x86_64-grtev4-linux-gnu"

%CallFrameType = type { ptr, ptr, i64, ptr }
%ArgType = type { ptr, i64 }

; Function Attrs: uwtable
define ptr @fn(ptr %0) #0 {
  %fn.invar_address.dim.4 = alloca i64, align 8
  %fn.invar_address.dim.3 = alloca i64, align 8
  %fn.invar_address.dim.2 = alloca i64, align 8
  %fn.invar_address.dim.1 = alloca i64, align 8
  %fn.invar_address.dim.0 = alloca i64, align 8
  %args_gep = getelementptr inbounds nuw %CallFrameType, ptr %0, i32 0, i32 3
  %args = load ptr, ptr %args_gep, align 8
  %arg0_gep = getelementptr %ArgType, ptr %args, i32 0, i32 0
  %arg0 = load ptr, ptr %arg0_gep, align 8, !invariant.load !0, !dereferenceable !1, !align !2
  %args_gep1 = getelementptr inbounds nuw %CallFrameType, ptr %0, i32 0, i32 3
  %args2 = load ptr, ptr %args_gep1, align 8
  %arg1_gep = getelementptr %ArgType, ptr %args2, i32 1, i32 0
  %arg1 = load ptr, ptr %arg1_gep, align 8, !invariant.load !0, !dereferenceable !3, !align !2
  %args_gep3 = getelementptr inbounds nuw %CallFrameType, ptr %0, i32 0, i32 3
  %args4 = load ptr, ptr %args_gep3, align 8
  %arg2_gep = getelementptr %ArgType, ptr %args4, i32 2, i32 0
  %arg2 = load ptr, ptr %arg2_gep, align 8, !invariant.load !0, !dereferenceable !4, !align !2
  store i64 0, ptr %fn.invar_address.dim.0, align 4
  br label %fn.loop_header.dim.0

fn.loop_header.dim.0:       ; preds = %fn.loop_exit.dim.1, %1
  %fn.indvar.dim.0 = load i64, ptr %fn.invar_address.dim.0, align 4
  %2 = icmp uge i64 %fn.indvar.dim.0, 3
  br i1 %2, label %fn.loop_exit.dim.0, label %fn.loop_body.dim.0

fn.loop_body.dim.0:         ; preds = %fn.loop_header.dim.0
  store i64 0, ptr %fn.invar_address.dim.1, align 4
  br label %fn.loop_header.dim.1

fn.loop_header.dim.1:       ; preds = %fn.loop_exit.dim.2, %fn.loop_body.dim.0
  %fn.indvar.dim.1 = load i64, ptr %fn.invar_address.dim.1, align 4
  %3 = icmp uge i64 %fn.indvar.dim.1, 2
  br i1 %3, label %fn.loop_exit.dim.1, label %fn.loop_body.dim.1

fn.loop_body.dim.1:         ; preds = %fn.loop_header.dim.1
  store i64 0, ptr %fn.invar_address.dim.2, align 4
  br label %fn.loop_header.dim.2

fn.loop_header.dim.2:       ; preds = %fn.loop_exit.dim.3, %fn.loop_body.dim.1
  %fn.indvar.dim.2 = load i64, ptr %fn.invar_address.dim.2, align 4
  %4 = icmp uge i64 %fn.indvar.dim.2, 2
  br i1 %4, label %fn.loop_exit.dim.2, label %fn.loop_body.dim.2

fn.loop_body.dim.2:         ; preds = %fn.loop_header.dim.2
  store i64 0, ptr %fn.invar_address.dim.3, align 4
  br label %fn.loop_header.dim.3

fn.loop_header.dim.3:       ; preds = %fn.loop_exit.dim.4, %fn.loop_body.dim.2
  %fn.indvar.dim.3 = load i64, ptr %fn.invar_address.dim.3, align 4
  %5 = icmp uge i64 %fn.indvar.dim.3, 6
  br i1 %5, label %fn.loop_exit.dim.3, label %fn.loop_body.dim.3

fn.loop_body.dim.3:         ; preds = %fn.loop_header.dim.3
  store i64 0, ptr %fn.invar_address.dim.4, align 4
  br label %fn.loop_header.dim.4

fn.loop_header.dim.4:       ; preds = %fn.loop_body.dim.4, %fn.loop_body.dim.3
  %fn.indvar.dim.4 = load i64, ptr %fn.invar_address.dim.4, align 4
  %6 = icmp uge i64 %fn.indvar.dim.4, 1
  br i1 %6, label %fn.loop_exit.dim.4, label %fn.loop_body.dim.4

fn.loop_body.dim.4:         ; preds = %fn.loop_header.dim.4
  %7 = getelementptr inbounds [3 x [2 x [1 x [1 x i32]]]], ptr %arg1, i64 0, i64 %fn.indvar.dim.0, i64 %fn.indvar.dim.2, i64 0, i64 0
  %8 = load i32, ptr %7, align 4, !invariant.load !0, !noalias !5
  %9 = getelementptr inbounds [3 x [2 x [1 x [6 x [1 x i32]]]]], ptr %arg0, i64 0, i64 %fn.indvar.dim.0, i64 %fn.indvar.dim.1, i64 0, i64 %fn.indvar.dim.3, i64 0
  %10 = load i32, ptr %9, align 4, !invariant.load !0, !noalias !5
  %11 = icmp eq i32 %8, %10
  %12 = zext i1 %11 to i8
  %13 = getelementptr inbounds [3 x [2 x [2 x [6 x [1 x i8]]]]], ptr %arg2, i64 0, i64 %fn.indvar.dim.0, i64 %fn.indvar.dim.1, i64 %fn.indvar.dim.2, i64 %fn.indvar.dim.3, i64 0
  store i8 %12, ptr %13, align 1, !alias.scope !5
  %invar.inc8 = add nuw nsw i64 %fn.indvar.dim.4, 1
  store i64 %invar.inc8, ptr %fn.invar_address.dim.4, align 4
  br label %fn.loop_header.dim.4

fn.loop_exit.dim.4:         ; preds = %fn.loop_header.dim.4
  %invar.inc7 = add nuw nsw i64 %fn.indvar.dim.3, 1
  store i64 %invar.inc7, ptr %fn.invar_address.dim.3, align 4
  br label %fn.loop_header.dim.3

fn.loop_exit.dim.3:         ; preds = %fn.loop_header.dim.3
  %invar.inc6 = add nuw nsw i64 %fn.indvar.dim.2, 1
  store i64 %invar.inc6, ptr %fn.invar_address.dim.2, align 4
  br label %fn.loop_header.dim.2

fn.loop_exit.dim.2:         ; preds = %fn.loop_header.dim.2
  %invar.inc5 = add nuw nsw i64 %fn.indvar.dim.1, 1
  store i64 %invar.inc5, ptr %fn.invar_address.dim.1, align 4
  br label %fn.loop_header.dim.1

fn.loop_exit.dim.1:         ; preds = %fn.loop_header.dim.1
  %invar.inc = add nuw nsw i64 %fn.indvar.dim.0, 1
  store i64 %invar.inc, ptr %fn.invar_address.dim.0, align 4
  br label %fn.loop_header.dim.0

fn.loop_exit.dim.0:         ; preds = %fn.loop_header.dim.0
  br label %return

return:                                           ; preds = %fn.loop_exit.dim.0
  ret ptr null
}

attributes #0 = { uwtable "frame-pointer"="all" "prefer-vector-width"="256" }

!0 = !{}
!1 = !{i64 144}
!2 = !{i64 32}
!3 = !{i64 24}
!4 = !{i64 72}
!5 = !{!6}
!6 = !{!"result slice: {}", !7}
!7 = !{!"fn AA domain"}

```
</details>

<details>
<summary>Optimized output</summary>

```llvm
source_filename = "example"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

define noalias noundef ptr @fn(ptr readonly captures(none) %0) local_unnamed_addr #0 {
  %args_gep = getelementptr inbounds nuw i8, ptr %0, i64 24
  %args = load ptr, ptr %args_gep, align 8
  %arg0 = load ptr, ptr %args, align 8, !invariant.load !0, !dereferenceable !1, !align !2
  %arg1_gep = getelementptr i8, ptr %args, i64 16
  %arg1 = load ptr, ptr %arg1_gep, align 8, !invariant.load !0, !dereferenceable !3, !align !2
  %arg2_gep = getelementptr i8, ptr %args, i64 32
  %arg2 = load ptr, ptr %arg2_gep, align 8, !invariant.load !0, !dereferenceable !4, !align !2
  %1 = getelementptr inbounds nuw i8, ptr %arg0, i64 32
  %2 = load <2 x i32>, ptr %arg1, align 32, !invariant.load !0, !noalias !5
  %3 = shufflevector <2 x i32> %2, <2 x i32> poison, <16 x i32> <i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 0, i32 0, i32 0, i32 0>
  %4 = load <8 x i32>, ptr %arg0, align 32, !invariant.load !0, !noalias !5
  %5 = load <2 x i32>, ptr %1, align 32, !invariant.load !0, !noalias !5
  %6 = shufflevector <2 x i32> %5, <2 x i32> poison, <12 x i32> <i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 0, i32 1, i32 poison, i32 poison>
  %7 = shufflevector <8 x i32> %4, <8 x i32> poison, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 poison, i32 poison, i32 poison, i32 poison>
  %8 = shufflevector <12 x i32> %7, <12 x i32> %6, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 20, i32 21, i32 poison, i32 poison>
  %9 = shufflevector <12 x i32> %8, <12 x i32> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9>
  %10 = icmp eq <16 x i32> %3, %9
  %11 = zext <16 x i1> %10 to <16 x i8>
  store <16 x i8> %11, ptr %arg2, align 32, !alias.scope !5
  %12 = getelementptr inbounds nuw i8, ptr %arg0, i64 40
  %13 = getelementptr inbounds nuw i8, ptr %arg2, i64 16
  %14 = getelementptr inbounds nuw i8, ptr %arg0, i64 48
  %15 = getelementptr inbounds nuw i8, ptr %arg1, i64 8
  %16 = getelementptr inbounds nuw i8, ptr %arg0, i64 56
  %17 = load <2 x i32>, ptr %1, align 32, !invariant.load !0, !noalias !5
  %18 = load <2 x i32>, ptr %12, align 8, !invariant.load !0, !noalias !5
  %19 = load <2 x i32>, ptr %14, align 16, !invariant.load !0, !noalias !5
  %20 = load <4 x i32>, ptr %16, align 8, !invariant.load !0, !noalias !5
  %21 = shufflevector <2 x i32> %2, <2 x i32> poison, <16 x i32> <i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %22 = shufflevector <8 x i32> %4, <8 x i32> poison, <12 x i32> <i32 poison, i32 poison, i32 6, i32 7, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %23 = shufflevector <4 x i32> %20, <4 x i32> poison, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %24 = shufflevector <12 x i32> %22, <12 x i32> %23, <12 x i32> <i32 poison, i32 poison, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 12, i32 13, i32 14, i32 15>
  %25 = shufflevector <2 x i32> %18, <2 x i32> poison, <12 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %26 = shufflevector <12 x i32> %25, <12 x i32> %24, <12 x i32> <i32 0, i32 1, i32 14, i32 15, i32 poison, i32 poison, i32 poison, i32 poison, i32 20, i32 21, i32 22, i32 23>
  %27 = shufflevector <2 x i32> %17, <2 x i32> poison, <12 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %28 = shufflevector <12 x i32> %26, <12 x i32> %27, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 12, i32 13, i32 poison, i32 poison, i32 8, i32 9, i32 10, i32 11>
  %29 = shufflevector <2 x i32> %19, <2 x i32> poison, <12 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %30 = shufflevector <12 x i32> %28, <12 x i32> %29, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 12, i32 13, i32 8, i32 9, i32 10, i32 11>
  %31 = shufflevector <12 x i32> %30, <12 x i32> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 0, i32 1, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 6, i32 7>
  %32 = icmp eq <16 x i32> %21, %31
  %33 = zext <16 x i1> %32 to <16 x i8>
  store <16 x i8> %33, ptr %13, align 16, !alias.scope !5
  %34 = getelementptr inbounds nuw i8, ptr %arg2, i64 32
  %35 = getelementptr inbounds nuw i8, ptr %arg0, i64 72
  %36 = getelementptr inbounds nuw i8, ptr %arg0, i64 88
  %37 = load <2 x i32>, ptr %15, align 8, !invariant.load !0, !noalias !5
  %38 = shufflevector <2 x i32> %37, <2 x i32> poison, <16 x i32> <i32 1, i32 1, i32 1, i32 1, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>
  %39 = load <4 x i32>, ptr %35, align 8, !invariant.load !0, !noalias !5
  %40 = load <2 x i32>, ptr %36, align 8, !invariant.load !0, !noalias !5
  %41 = shufflevector <4 x i32> %39, <4 x i32> poison, <16 x i32> <i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison>
  %42 = shufflevector <4 x i32> %20, <4 x i32> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %43 = shufflevector <16 x i32> %42, <16 x i32> %41, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 26, i32 27, i32 28, i32 29, i32 poison, i32 poison>
  %44 = shufflevector <4 x i32> %39, <4 x i32> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %45 = shufflevector <16 x i32> %43, <16 x i32> %44, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 16, i32 17, i32 18, i32 19, i32 poison, i32 poison, i32 10, i32 11, i32 12, i32 13, i32 poison, i32 poison>
  %46 = shufflevector <2 x i32> %40, <2 x i32> poison, <16 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %47 = shufflevector <16 x i32> %45, <16 x i32> %46, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 16, i32 17, i32 10, i32 11, i32 12, i32 13, i32 poison, i32 poison>
  %48 = shufflevector <2 x i32> %40, <2 x i32> poison, <16 x i32> <i32 0, i32 1, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %49 = shufflevector <16 x i32> %47, <16 x i32> %48, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 16, i32 17>
  %50 = icmp eq <16 x i32> %38, %49
  %51 = zext <16 x i1> %50 to <16 x i8>
  store <16 x i8> %51, ptr %34, align 32, !alias.scope !5
  %52 = getelementptr inbounds nuw i8, ptr %arg0, i64 96
  %53 = getelementptr inbounds nuw i8, ptr %arg1, i64 16
  %54 = getelementptr inbounds nuw i8, ptr %arg2, i64 48
  %55 = getelementptr inbounds nuw i8, ptr %arg0, i64 128
  %56 = load <2 x i32>, ptr %53, align 16, !invariant.load !0, !noalias !5
  %57 = shufflevector <2 x i32> %56, <2 x i32> poison, <16 x i32> <i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 0, i32 0, i32 0, i32 0>
  %58 = load <8 x i32>, ptr %52, align 32, !invariant.load !0, !noalias !5
  %59 = load <2 x i32>, ptr %55, align 32, !invariant.load !0, !noalias !5
  %60 = shufflevector <2 x i32> %59, <2 x i32> poison, <12 x i32> <i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 0, i32 1, i32 poison, i32 poison>
  %61 = shufflevector <8 x i32> %58, <8 x i32> poison, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 poison, i32 poison, i32 poison, i32 poison>
  %62 = shufflevector <12 x i32> %61, <12 x i32> %60, <12 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 20, i32 21, i32 poison, i32 poison>
  %63 = shufflevector <12 x i32> %62, <12 x i32> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9>
  %64 = icmp eq <16 x i32> %57, %63
  %65 = zext <16 x i1> %64 to <16 x i8>
  store <16 x i8> %65, ptr %54, align 16, !alias.scope !5
  %66 = getelementptr inbounds nuw i8, ptr %arg2, i64 64
  %67 = load <4 x i32>, ptr %55, align 32, !invariant.load !0, !noalias !5
  %68 = shufflevector <2 x i32> %56, <2 x i32> poison, <8 x i32> <i32 0, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>
  %69 = shufflevector <4 x i32> %67, <4 x i32> poison, <8 x i32> <i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %70 = shufflevector <8 x i32> %58, <8 x i32> %69, <8 x i32> <i32 8, i32 9, i32 6, i32 7, i32 poison, i32 poison, i32 poison, i32 poison>
  %71 = shufflevector <4 x i32> %67, <4 x i32> poison, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison>
  %72 = shufflevector <8 x i32> %70, <8 x i32> %71, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 8, i32 9, i32 10, i32 11>
  %73 = icmp eq <8 x i32> %68, %72
  %74 = zext <8 x i1> %73 to <8 x i8>
  store <8 x i8> %74, ptr %66, align 32, !alias.scope !5
  ret ptr null
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none) uwtable "frame-pointer"="all" "prefer-vector-width"="256" }

!0 = !{}
!1 = !{i64 144}
!2 = !{i64 32}
!3 = !{i64 24}
!4 = !{i64 72}
!5 = !{!6}
!6 = !{!"result slice: {}", !7}
!7 = !{!"fn AA domain"}
```

</details>

## Comments

### Comment 1 - WillFroom

@alexey-bataev I haven't 100% confirmed but I think 1d5fbe83c3bcdd27a8ce1d57a68fb7be35e6cbbf might be the root cause (or at least reveals an underlying issue), if you have any ideas what the cause might be that would be great, thanks in advance!

---

