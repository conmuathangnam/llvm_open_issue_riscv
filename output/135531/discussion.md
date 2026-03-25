# [SCEV] Incorrect scev result

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/135531
**Status:** Closed
**Labels:** miscompilation, llvm:SCEV
**Closed Date:** 2025-04-13T12:22:49Z

## Body

Reproducer:
```
; bin/opt -passes="print<scalar-evolution>" test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@g = global i64 1013947741822887439

define i32 @main() {
entry:
  %.promoted60.i.i = load i64, ptr @g, align 8
  %0 = trunc i64 %.promoted60.i.i to i32
  %conv661.i.i = and i32 %0, 16382
  %cond.i1408.i.i = sub nsw i32 0, %conv661.i.i
  %1 = and i32 %cond.i1408.i.i, 268431360
  ret i32 %1
}
```
```
Printing analysis 'Scalar Evolution Analysis' for function 'main':
Classifying expressions for: @main
  %.promoted60.i.i = load i64, ptr @g, align 8
  -->  %.promoted60.i.i U: full-set S: full-set
  %0 = trunc i64 %.promoted60.i.i to i32
  -->  (trunc i64 %.promoted60.i.i to i32) U: full-set S: full-set
  %conv661.i.i = and i32 %0, 16382
  -->  (2 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> U: [0,16383) S: [0,16383)
  %cond.i1408.i.i = sub nsw i32 0, %conv661.i.i
  -->  (-2 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nsw> U: [0,-1) S: [-16382,1)
  %1 = and i32 %cond.i1408.i.i, 268431360
  -->  (4096 * (zext i16 (trunc i32 ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) to i16) to i32))<nuw><nsw> U: [0,268431361) S: [0,268431361)
```
llubi output:
```
Entering function main
  %.promoted60.i.i = load i64, ptr @g, align 8 -> i64 1013947741822887439
  %0 = trunc i64 %.promoted60.i.i to i32 -> i32 1546233359
  %conv661.i.i = and i32 %0, 16382 -> i32 9742
  %cond.i1408.i.i = sub nsw i32 0, %conv661.i.i -> i32 -9742
  %1 = and i32 %cond.i1408.i.i, 268431360 -> i32 268423168  (4096 * (zext i16 (trunc i32 ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) to i16) to i32))<nuw><nsw> = poison
  (zext i16 (trunc i32 ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) to i16) to i32) = poison
  (trunc i32 ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) to i16) = poison
  ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) = poison
  (2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> = poison
  (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32) = 4871
  (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) = -3321
  ((trunc i64 %.promoted60.i.i to i32) /u 2) = 773116679
  (trunc i64 %.promoted60.i.i to i32) = 1546233359
  %.promoted60.i.i = 1013947741822887439

UB triggered: SCEV result is more poisonous than real value
  %1 = and i32 %cond.i1408.i.i, 268431360 = i32 268423168
(4096 * (zext i16 (trunc i32 ((2147483647 * (zext i13 (trunc i32 ((trunc i64 %.promoted60.i.i to i32) /u 2) to i13) to i32))<nuw><nsw> /u 2048) to i16) to i32))<nuw><nsw> = poison

Exited with immediate UB.
Stacktrace:
    %1 = and i32 %cond.i1408.i.i, 268431360 at @main
```

