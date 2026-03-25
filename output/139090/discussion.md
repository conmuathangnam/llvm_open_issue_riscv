# [SLP-Vectorizer] Miscompile on Given IR

**Author:** Nirhar
**URL:** https://github.com/llvm/llvm-project/issues/139090
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-05-12T15:01:59Z

## Body

Consider the IR:
```
; ModuleID = 'reduced.ll'
source_filename = "reduced.ll"
target triple = "x86_64-unknown-linux-gnu"

@global = global i32 0

define i64 @main() #0 {
bb:
  %alloca = alloca i32, align 8
  %load = load i32, ptr @global, align 8
  %add = add i32 1, %load
  %sub = sub i32 0, %add
  %mul = mul i32 %sub, 10
  %add1 = add i32 %mul, 10
  %mul2 = mul i32 0, 0
  %or = or i32 %mul2, 0
  %mul3 = mul i32 %add, 0
  %or4 = or i32 %mul3, 0
  %mul5 = mul i32 0, 0
  %or6 = or i32 %mul5, 0
  %add7 = add i32 1, 0
  %sub8 = sub i32 0, 0
  %mul9 = mul i32 %sub8, 0
  %or10 = or i32 %mul9, 0
  %sub11 = sub i32 0, %add7
  %mul12 = mul i32 %sub11, 0
  %or13 = or i32 %mul12, 0
  %sub14 = sub i32 0, 0
  %mul15 = mul i32 %sub14, 0
  %or16 = or i32 %mul15, 0
  %mul17 = mul i32 0, 0
  %or18 = or i32 %mul17, 0
  %mul19 = mul i32 0, 0
  %or20 = or i32 %mul19, 0
  br label %bb26

bb21:                                             ; preds = %bb26
  %getelementptr = getelementptr i8, ptr %alloca, i64 40
  %load22 = load i32, ptr %getelementptr, align 8
  %sext = sext i32 %load22 to i64
  %sub23 = sub i32 1, %sub11
  %sext24 = sext i32 %sub23 to i64
  %or25 = or i64 %sext, %sext24
  ret i64 %or25

bb26:                                             ; preds = %bb26, %bb
  %phi = phi i64 [ 1, %bb ], [ 0, %bb26 ]
  %getelementptr27 = getelementptr i32, ptr %alloca, i64 10
  store i32 %add1, ptr %getelementptr27, align 4
  %add28 = add i64 %phi, 10
  %getelementptr29 = getelementptr i32, ptr %alloca, i64 %add28
  store i32 %or, ptr %getelementptr29, align 4
  %add30 = add i64 %phi, 11
  %getelementptr31 = getelementptr i32, ptr %alloca, i64 %add30
  store i32 %or4, ptr %getelementptr31, align 4
  %add32 = add i64 %phi, 12
  %getelementptr33 = getelementptr i32, ptr %alloca, i64 %add32
  store i32 %or6, ptr %getelementptr33, align 4
  %add34 = add i64 %phi, 13
  %getelementptr35 = getelementptr i32, ptr %alloca, i64 %add34
  store i32 %or10, ptr %getelementptr35, align 4
  %add36 = add i64 %phi, 14
  %getelementptr37 = getelementptr i32, ptr %alloca, i64 %add36
  store i32 %or13, ptr %getelementptr37, align 4
  %add38 = add i64 %phi, 15
  %getelementptr39 = getelementptr i32, ptr %alloca, i64 %add38
  store i32 %or16, ptr %getelementptr39, align 4
  %add40 = add i64 %phi, 16
  %getelementptr41 = getelementptr i32, ptr %alloca, i64 %add40
  store i32 %or18, ptr %getelementptr41, align 4
  %add42 = add i64 %phi, 17
  %getelementptr43 = getelementptr i32, ptr %alloca, i64 %add42
  store i32 %or20, ptr %getelementptr43, align 4
  br i1 true, label %bb21, label %bb26
}

attributes #0 = { "target-features"="+avx2" }
```
running `lli` on the above IR returns `2`. However, if I transform the IR with `opt -passes=slp-vectorizer` the transformed IR:
```
source_filename = "reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@global = global i32 0

define i64 @main() #0 {
bb:
  %alloca = alloca i32, align 8
  %load = load i32, ptr @global, align 8
  %add7 = add i32 1, 0
  %0 = insertelement <4 x i32> <i32 poison, i32 0, i32 poison, i32 0>, i32 %load, i32 0
  %1 = insertelement <4 x i32> %0, i32 %add7, i32 2
  %2 = sub <4 x i32> <i32 -1, i32 0, i32 0, i32 0>, %1
  %3 = extractelement <4 x i32> %2, i32 0
  %sub = sub i32 0, %3
  %mul = mul i32 %sub, 10
  %add1 = add i32 %mul, 10
  %4 = shufflevector <4 x i32> %2, <4 x i32> poison, <8 x i32> <i32 0, i32 poison, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison>
  %5 = shufflevector <8 x i32> <i32 0, i32 poison, i32 0, i32 poison, i32 poison, i32 poison, i32 0, i32 0>, <8 x i32> %4, <8 x i32> <i32 0, i32 8, i32 2, i32 10, i32 11, i32 12, i32 6, i32 7>
  %6 = mul <8 x i32> zeroinitializer, %5
  %7 = or <8 x i32> %6, zeroinitializer
  br label %bb26

bb21:                                             ; preds = %bb26
  %getelementptr = getelementptr i8, ptr %alloca, i64 40
  %load22 = load i32, ptr %getelementptr, align 8
  %sext = sext i32 %load22 to i64
  %8 = extractelement <4 x i32> %2, i32 2
  %sub23 = sub i32 1, %8
  %sext24 = sext i32 %sub23 to i64
  %or25 = or i64 %sext, %sext24
  ret i64 %or25

bb26:                                             ; preds = %bb26, %bb
  %phi = phi i64 [ 1, %bb ], [ 0, %bb26 ]
  %getelementptr27 = getelementptr i32, ptr %alloca, i64 10
  store i32 %add1, ptr %getelementptr27, align 4
  %add28 = add i64 %phi, 10
  %getelementptr29 = getelementptr i32, ptr %alloca, i64 %add28
  store <8 x i32> %7, ptr %getelementptr29, align 4
  br i1 true, label %bb21, label %bb26
}

attributes #0 = { "target-features"="+avx2" }
```
returns `22` when run with `lli`

The proof of transformation is shown here: https://godbolt.org/z/TE9GTjaMr

## Comments

### Comment 1 - alexey-bataev

https://alive2.llvm.org/ce/z/YD7Ur4
alive reports that the original code is UB and the transformation is correct

---

### Comment 2 - alexey-bataev

Checked it anyway, looks like the issue with interchangeable transformation. For some reason `%add = add i32 1, %load` is replaced by `sub -1, %load`, though it shall be `sub %load, -1`. @HanKuanChen 

---

### Comment 3 - alexey-bataev

Fixed in https://github.com/llvm/llvm-project/commit/53df6400afdd271d88cec58bcd5bde3841b53521

---

