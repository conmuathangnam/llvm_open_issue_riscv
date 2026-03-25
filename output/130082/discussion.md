# [SLPVectorizer] Occasional miscompile with a switch statement

**Author:** usatiuk
**URL:** https://github.com/llvm/llvm-project/issues/130082
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-03-06T16:51:39Z

## Body

Reduced reproducer:

```
; opt -S reduced.ll --passes=slp-vectorizer -slp-threshold=-99999
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i32 %0) {
entry:
  switch i32 0, label %bci_130 [
    i32 -4, label %bci_96
    i32 3, label %bci_130
    i32 1, label %bci_130
    i32 9, label %bci_108
    i32 0, label %bci_130
  ]

bci_130:                                          ; preds = %bci_96, %bci_108, %entry, %entry, %entry, %entry
  %1 = phi i32 [ 0, %bci_108 ], [ %0, %bci_96 ], [ %0, %entry ], [ %0, %entry ], [ %0, %entry ], [ %0, %entry ]
  %local_2_10 = phi i32 [ 0, %bci_108 ], [ 0, %bci_96 ], [ 0, %entry ], [ 0, %entry ], [ 0, %entry ], [ 0, %entry ]
  ret i32 0

bci_108:                                          ; preds = %entry
  br label %bci_130

bci_96:                                           ; preds = %entry
  br label %bci_130
}
```

Expected output:
```
; ModuleID = 'reduced.ll'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i32 %0) {
entry:
  %1 = insertelement <2 x i32> <i32 0, i32 poison>, i32 %0, i32 1
  switch i32 0, label %bci_130 [
    i32 -4, label %bci_96
    i32 3, label %bci_130
    i32 1, label %bci_130
    i32 9, label %bci_108
    i32 0, label %bci_130
  ]

bci_130:                                          ; preds = %bci_96, %bci_108, %entry, %entry, %entry, %entry
  %2 = phi <2 x i32> [ zeroinitializer, %bci_108 ], [ %1, %bci_96 ], [ %1, %entry ], [ %1, %entry ], [ %1, %entry ], [ %1, %entry ]
  ret i32 0

bci_108:                                          ; preds = %entry
  br label %bci_130

bci_96:                                           ; preds = %entry
  br label %bci_130
}
```

But sometimes the result is:
```
; ModuleID = 'reduced.ll'
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i32 %0) {
entry:
  %1 = insertelement <2 x i32> <i32 0, i32 poison>, i32 %0, i32 1
  switch i32 0, label %bci_130 [
    i32 -4, label %bci_96
    i32 3, label %bci_130
    i32 1, label %bci_130
    i32 9, label %bci_108
    i32 0, label %bci_130
  ]

bci_130:                                          ; preds = %bci_96, %bci_108, %entry, %entry, %entry, %entry
  %2 = phi <2 x i32> [ zeroinitializer, %bci_108 ], [ %3, %bci_96 ], [ %1, %entry ], [ %1, %entry ], [ %1, %entry ], [ %1, %entry ]
  ret i32 0

bci_108:                                          ; preds = %entry
  br label %bci_130

bci_96:                                           ; preds = %entry
  %3 = load <2 x i32>, ptr poison, align 8
  br label %bci_130
}
```

I have bisected the issue to commit https://github.com/llvm/llvm-project/commit/894935cb5146fd2ac6334cc8b11e6d6e0e264fe6

## Comments

### Comment 1 - bgra8

@usatiuk by `sometimes the result is ..` do you mean the compilation is non-deterministic? You don't get the same result at each compilation?

---

### Comment 2 - usatiuk

Yep, that's the case for me

---

### Comment 3 - bgra8

This is double the  problem then: the patch introduces miscompile _and_ nondeterminism. @alexey-bataev can you please have a look?


---

### Comment 4 - alexey-bataev

Most probably, the issue is not directly related to the patch itself. Looks like it reveals again the issue with the phi nodes sorting. I will fix it today

---

### Comment 5 - usatiuk

Many thanks!

---

