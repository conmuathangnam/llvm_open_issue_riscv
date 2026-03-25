# [LoopVectorize] Miscompilation

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/122602
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-01-12T22:03:43Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/PASME7
```
; bin/opt -passes=loop-vectorize reduced.ll -S -o opt.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@e = global [1 x [4 x i16]] zeroinitializer
@crc32_tab = global [256 x i32] zeroinitializer

define i32 @src() {
entry:
  store i32 1, ptr @crc32_tab, align 4
  br label %for.cond4.preheader.outer.i.i.split

for.cond4.preheader.outer.i.i.split:              ; preds = %for.cond4.preheader.outer.i.i.split, %entry
  %0 = phi i64 [ %1, %for.cond4.preheader.outer.i.i.split ], [ 3, %entry ]
  %indvars.iv.i.i23 = phi i64 [ %indvars.iv.next.i.i, %for.cond4.preheader.outer.i.i.split ], [ 2, %entry ]
  %arrayidx9.i.i = getelementptr i16, ptr @e, i64 %0
  store i16 1, ptr %arrayidx9.i.i, align 2
  %indvars.iv.next.i.i = add i64 %indvars.iv.i.i23, -1
  %tobool7.not.i.i = icmp eq i64 %indvars.iv.next.i.i, 0
  %1 = add i64 %indvars.iv.next.i.i, 1
  br i1 %tobool7.not.i.i, label %for.end.thread.i.i.1, label %for.cond4.preheader.outer.i.i.split

for.end.thread.i.i.1:                             ; preds = %for.cond4.preheader.outer.i.i.split
  %arrayidx947.i.i = getelementptr i16, ptr @e, i64 %1
  store i16 0, ptr %arrayidx947.i.i, align 2
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.end.thread.i.i.1
  %indvars.iv = phi i64 [ 0, %for.end.thread.i.i.1 ], [ %indvars.iv.next, %for.body3 ]
  %xor2.i70.i.i1718 = phi i32 [ 0, %for.end.thread.i.i.1 ], [ %3, %for.body3 ]
  %arrayidx5 = getelementptr i16, ptr @e, i64 %indvars.iv
  %2 = load i16, ptr %arrayidx5, align 2
  %conv = zext i16 %2 to i64
  %idxprom.i.i.i = and i64 %conv, 1
  %arrayidx.i.i.i = getelementptr i32, ptr @crc32_tab, i64 %idxprom.i.i.i
  %3 = load i32, ptr %arrayidx.i.i.i, align 4
  %indvars.iv.next = add i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 4
  br i1 %exitcond.not, label %for.end8, label %for.body3

for.end8:                                         ; preds = %for.body3
  ret i32 %xor2.i70.i.i1718
}
```
```
> bin/lli reduced.ll
> echo $?
0
> bin/lli opt.ll
> echo $?
1
```
llvm version: ab9a80a3ad78f611fd06cd6f7215bd828809310c

## Comments

### Comment 1 - fhahn

I think this is the same issue as https://github.com/llvm/llvm-project/issues/122496

Should have a fix soon

---

