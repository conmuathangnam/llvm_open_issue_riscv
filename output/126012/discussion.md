# [IndVarSimplify] Miscompilation at O2

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/126012
**Status:** Closed
**Labels:** miscompilation, llvm:SCEV
**Closed Date:** 2025-02-10T09:07:23Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/UTnMBe
```
; bin/opt -passes=indvars reduced.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  br label %for.cond1.preheader.i.i

for.cond1.preheader.i.i:                          ; preds = %for.inc18.i.i, %entry
  %.pre.i.i = phi i64 [ 0, %entry ], [ %1, %for.inc18.i.i ]
  %j.sroa.0.043.i.i = phi i64 [ 1, %entry ], [ %storemerge42.i.i, %for.inc18.i.i ]
  %storemerge42.i.i = phi i64 [ 0, %entry ], [ %inc22.i.i, %for.inc18.i.i ]
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i

for.end.i.i:                                      ; preds = %for.cond1.preheader.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0
  %dec.348.i.i = add i64 %.pre.i.i, 1
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0
  br label %for.inc18.i.i

for.inc18.i.i:                                    ; preds = %for.end.i.i, %for.cond1.preheader.i.i
  %1 = phi i64 [ %spec.select.i.i, %for.end.i.i ], [ 0, %for.cond1.preheader.i.i ]
  %inc22.i.i = add i64 %storemerge42.i.i, 1
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i

g.exit:                                           ; preds = %for.inc18.i.i
  %conv = trunc i64 %.pre.i.i to i32
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
Before:
```
Entering function main
  br label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 0
    phi i64 %j.sroa.0.043.i.i -> i64 1
    phi i64 %storemerge42.i.i -> i64 0
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> T
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 0
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 1
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 0
    phi i64 %j.sroa.0.043.i.i -> i64 0
    phi i64 %storemerge42.i.i -> i64 1
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> F
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 1
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 0
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 0
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 2
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 0
    phi i64 %j.sroa.0.043.i.i -> i64 1
    phi i64 %storemerge42.i.i -> i64 2
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 1
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 1
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 1
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 3
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 1
    phi i64 %j.sroa.0.043.i.i -> i64 2
    phi i64 %storemerge42.i.i -> i64 3
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 2
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 2
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 2
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 4
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 2
    phi i64 %j.sroa.0.043.i.i -> i64 3
    phi i64 %storemerge42.i.i -> i64 4
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 3
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 3
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 3
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 5
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 3
    phi i64 %j.sroa.0.043.i.i -> i64 4
    phi i64 %storemerge42.i.i -> i64 5
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 4
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 4
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 4
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 6
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 4
    phi i64 %j.sroa.0.043.i.i -> i64 5
    phi i64 %storemerge42.i.i -> i64 6
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 5
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 5
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 5
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 7
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 5
    phi i64 %j.sroa.0.043.i.i -> i64 6
    phi i64 %storemerge42.i.i -> i64 7
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 6
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 6
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 6
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 8
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 6
    phi i64 %j.sroa.0.043.i.i -> i64 7
    phi i64 %storemerge42.i.i -> i64 8
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 7
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 7
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 7
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 9
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 7
    phi i64 %j.sroa.0.043.i.i -> i64 8
    phi i64 %storemerge42.i.i -> i64 9
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %0 = icmp sgt i64 %j.sroa.0.043.i.i, 0 -> T
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 8
  %spec.select.i.i = select i1 %0, i64 %dec.348.i.i, i64 0 -> i64 8
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %1 -> i64 8
  %inc22.i.i = add i64 %storemerge42.i.i, 1 -> i64 10
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> T
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %g.exit
  %conv = trunc i64 %.pre.i.i to i32 -> i32 7
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
    Printf: 7
   -> i32 2
  ret i32 0
Exiting function main
```
After:
```
; ModuleID = 'reduced.ll'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  br label %for.cond1.preheader.i.i

for.cond1.preheader.i.i:                          ; preds = %for.inc18.i.i, %entry
  %.pre.i.i = phi i64 [ 0, %entry ], [ %0, %for.inc18.i.i ]
  %storemerge42.i.i = phi i64 [ 0, %entry ], [ %inc22.i.i, %for.inc18.i.i ]
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i

for.end.i.i:                                      ; preds = %for.cond1.preheader.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0
  br label %for.inc18.i.i

for.inc18.i.i:                                    ; preds = %for.end.i.i, %for.cond1.preheader.i.i
  %0 = phi i64 [ %spec.select.i.i, %for.end.i.i ], [ 0, %for.cond1.preheader.i.i ]
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i

g.exit:                                           ; preds = %for.inc18.i.i
  %.pre.i.i.lcssa = phi i64 [ %.pre.i.i, %for.inc18.i.i ]
  %conv = trunc i64 %.pre.i.i.lcssa to i32
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
```
Entering function main
  br label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 0
    phi i64 %storemerge42.i.i -> i64 0
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> T
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 0
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 1
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 0
    phi i64 %storemerge42.i.i -> i64 1
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 1
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 1
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 1
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 2
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 1
    phi i64 %storemerge42.i.i -> i64 2
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 2
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 2
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 2
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 3
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 2
    phi i64 %storemerge42.i.i -> i64 3
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 3
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 3
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 3
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 4
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 3
    phi i64 %storemerge42.i.i -> i64 4
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 4
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 4
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 4
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 5
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 4
    phi i64 %storemerge42.i.i -> i64 5
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 5
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 5
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 5
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 6
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 5
    phi i64 %storemerge42.i.i -> i64 6
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 6
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 6
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 6
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 7
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 6
    phi i64 %storemerge42.i.i -> i64 7
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 7
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 7
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 7
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 8
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 7
    phi i64 %storemerge42.i.i -> i64 8
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 8
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 8
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 8
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 9
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> F
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %for.cond1.preheader.i.i
    phi i64 %.pre.i.i -> i64 8
    phi i64 %storemerge42.i.i -> i64 9
  %tobool.not.i.i = icmp eq i64 %storemerge42.i.i, 0 -> F
  br i1 %tobool.not.i.i, label %for.inc18.i.i, label %for.end.i.i jump to %for.end.i.i
  %dec.348.i.i = add i64 %.pre.i.i, 1 -> i64 9
  %spec.select.i.i = select i1 true, i64 %dec.348.i.i, i64 0 -> i64 9
  br label %for.inc18.i.i jump to %for.inc18.i.i
    phi i64 %0 -> i64 9
  %inc22.i.i = add nuw nsw i64 %storemerge42.i.i, 1 -> i64 10
  %exitcond50.not.i.i = icmp eq i64 %storemerge42.i.i, 9 -> T
  br i1 %exitcond50.not.i.i, label %g.exit, label %for.cond1.preheader.i.i jump to %g.exit
    phi i64 %.pre.i.i.lcssa -> i64 8
  %conv = trunc i64 %.pre.i.i.lcssa to i32 -> i32 8
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
    Printf: 8
   -> i32 2
  ret i32 0
Exiting function main
```
llvm version: 52fc6ffcda0895c0c7b976ad1f5cb5a282b571d2

## Comments

### Comment 1 - dtcxzyw

Reduced reproducer: https://alive2.llvm.org/ce/z/_x96gs
```
; bin/opt -passes=indvars reduced.ll -S --debug
define i32 @src() {
entry:
  br label %for.preheader

for.preheader:
  %indvar1 = phi i32 [ 0, %entry ], [ %phi, %for.inc ]
  %indvar2 = phi i32 [ 1, %entry ], [ %indvar3, %for.inc ]
  %indvar3 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cond1 = icmp eq i32 %indvar3, 0
  br i1 %cond1, label %for.inc, label %for.end

for.end:
  %cmp = icmp sgt i32 %indvar2, 0
  %ext = zext i1 %cmp to i32
  br label %for.inc

for.inc:
  %phi = phi i32 [ %ext, %for.end ], [ 0, %for.preheader ]
  %inc = add i32 %indvar3, 1
  %exitcond = icmp eq i32 %indvar3, 2
  br i1 %exitcond, label %for.exit, label %for.preheader

for.exit:
  ret i32 %indvar1
}

```
```
INDVARS: Eliminated comparison:   %cmp = icmp sgt i32 %indvar2, 0

define i32 @src() {
entry:
  br label %for.preheader

for.preheader:                                    ; preds = %for.inc, %entry
  %indvar1 = phi i32 [ 0, %entry ], [ %phi, %for.inc ]
  %indvar3 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cond1 = icmp eq i32 %indvar3, 0
  br i1 %cond1, label %for.inc, label %for.end

for.end:                                          ; preds = %for.preheader
  %ext = zext i1 true to i32
  br label %for.inc

for.inc:                                          ; preds = %for.end, %for.preheader
  %phi = phi i32 [ %ext, %for.end ], [ 0, %for.preheader ]
  %inc = add nuw nsw i32 %indvar3, 1
  %exitcond = icmp eq i32 %indvar3, 2
  br i1 %exitcond, label %for.exit, label %for.preheader

for.exit:                                         ; preds = %for.inc
  %indvar1.lcssa = phi i32 [ %indvar1, %for.inc ]
  ret i32 %indvar1.lcssa
}
```
`ScalarEvolution::isImpliedViaMerge(sgt, %indvar2, 0, {0,+,1}<nuw><nsw><%for.preheader>(= %indvar3), 0)` wrongly returns true.
I can't quite understand why `%indvar3` properly dominates `%for.preheader`: https://github.com/llvm/llvm-project/blob/26ecddb05d13c101ccd840a6710eb5f8b82de841/llvm/lib/Analysis/ScalarEvolution.cpp#L14131-L14141

@nikic @fhahn Can you help fix this issue?




---

### Comment 2 - nikic

Related previous fix: https://reviews.llvm.org/D101829

---

