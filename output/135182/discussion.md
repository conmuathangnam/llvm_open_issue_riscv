# [IndVarSimplify] Miscompilation at -O3

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/135182
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-04-11T01:03:07Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/fvxiS3
```
; bin/opt -passes=indvars test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @src(ptr %p) {
entry:
  br label %for.body21

for.body21:
  %storemerge629 = phi i8 [ -12, %entry ], [ %dec, %for.body21 ]
  %conv29 = sext i8 %storemerge629 to i16
  %conv1.i = sext i8 %storemerge629 to i16
  %add.i = add i16 %conv29, %conv1.i
  %conv55 = zext i16 %add.i to i32
  store i32 %conv55, ptr %p, align 4
  %dec = add i8 %storemerge629, 1
  br i1 false, label %for.body21, label %for.end234

for.end234:
  ret void
}
```
Output:
```
define void @tgt(ptr %p) {
  br label %for.body21

for.body21:                                       ; preds = %for.body21, %entry
  store i32 131048, ptr %p, align 4
  br i1 false, label %for.body21, label %for.end234

for.end234:                                       ; preds = %for.body21
  ret void
}
```
`store i32 131048, ptr @g_150, align 4` should be `store i32 65512, ptr @g_150, align 4`
llvm version: f819f46284f2a79790038e1f6649172789734ae8


