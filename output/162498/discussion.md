# [LoopVectorize] Miscompile since uniform pointers change.

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/162498
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-10-15T12:46:10Z

## Body

After this https://github.com/llvm/llvm-project/commit/0c028bbf33d96045871f254fa1810f9767292506 commit the following IR snippet:
```llvm
target triple = "x86_64-unknown-linux-gnu"

define ptr @foo(ptr %0) {
entry:
  %1 = call noalias ptr null(i64 0, i32 0, i32 0, i32 0)
  %2 = getelementptr i8, ptr %1, i64 16
  br label %loop

loop:
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %loop ]
  %3 = lshr i64 %indvars.iv, 1
  %addr = getelementptr i64, ptr %2, i64 %indvars.iv
  store i64 0, ptr %addr, align 8
  %addr19 = getelementptr i32, ptr %0, i64 %3
  %4 = trunc i64 %indvars.iv to i32
  store i32 %4, ptr %addr19, align 4
  %indvars.iv.next = add i64 %indvars.iv, 1
  %5 = icmp ult i64 %indvars.iv, 97
  br i1 %5, label %loop, label %unreached

unreached:
  ret ptr null
}
```
Is getting miscompiled by `opt -passes=loop-vectorize`.
It gets vectorized as
```llvm
define ptr @foo(ptr %0) {
entry:
  %1 = call noalias ptr null(i64 0, i32 0, i32 0, i32 0)
  %2 = getelementptr i8, ptr %1, i64 16
  br label %vector.ph

vector.ph:
  br label %vector.body

vector.body:
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %3 = add i64 %index, 2
  %4 = trunc i64 %index to i32
  %5 = add i32 %4, 2
  %6 = add i32 %4, 3
  %7 = lshr i64 %index, 1
  %8 = lshr i64 %3, 1
  %9 = getelementptr i64, ptr %2, i64 %index
  %10 = getelementptr i64, ptr %9, i32 2
  store <2 x i64> zeroinitializer, ptr %9, align 8
  store <2 x i64> zeroinitializer, ptr %10, align 8
  %11 = getelementptr i32, ptr %0, i64 %7
  %12 = getelementptr i32, ptr %0, i64 %8
  store i32 %6, ptr %11, align 4
  store i32 %6, ptr %12, align 4
  %index.next = add nuw i64 %index, 4
  %13 = icmp eq i64 %index.next, 96
  br i1 %13, label %middle.block, label %vector.body

middle.block:
  br label %scalar.ph

scalar.ph:
  br label %loop

loop:
  %indvars.iv = phi i64 [ 96, %scalar.ph ], [ %indvars.iv.next, %loop ]
  %14 = lshr i64 %indvars.iv, 1
  %addr = getelementptr i64, ptr %2, i64 %indvars.iv
  store i64 0, ptr %addr, align 8
  %addr19 = getelementptr i32, ptr %0, i64 %14
  %15 = trunc i64 %indvars.iv to i32
  store i32 %15, ptr %addr19, align 4
  %indvars.iv.next = add i64 %indvars.iv, 1
  %16 = icmp ult i64 %indvars.iv, 97
  br i1 %16, label %loop, label %unreached

unreached:
  ret ptr null
}
```
Note the double stores of the same value:
```llvm
  store i32 %6, ptr %11, align 4
  store i32 %6, ptr %12, align 4
```
while before vectorization new value (indvar) was stored on each iteration.

Godbolt link:  https://godbolt.org/z/beeraoETf

## Comments

### Comment 1 - fhahn

Looks like a VPlan transform incorrectly assuming uniform-per-part (single-scalar) implies unifirom across UF and VF. Will fix

---

