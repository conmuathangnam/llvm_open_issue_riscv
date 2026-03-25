# [SLPVectorizer] Miscompile at O3

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/120076
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2024-12-16T16:04:15Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/_88AAB
```
; bin/opt -passes=slp-vectorizer reduced.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i8 @func_129() {
entry:
  %conv4.i.i = zext i16 0 to i32
  %conv7.i.i = sext i16 0 to i32
  %cmp8.i.i = icmp slt i32 %conv7.i.i, %conv4.i.i
  %conv9.i.i = zext i1 %cmp8.i.i to i32
  %or10.i.i = or i32 %conv9.i.i, %conv4.i.i
  %cmp11.i.i = icmp eq i32 %or10.i.i, %conv4.i.i
  %sub.i.i79.peel.i = sub i16 0, 1
  %xor5.i81.peel.i = zext i16 %sub.i.i79.peel.i to i32
  %conv7.i84.peel.i = sext i16 0 to i32
  %cmp8.i85.peel.i = icmp slt i32 %conv7.i84.peel.i, %xor5.i81.peel.i
  %conv9.i86.peel.i = zext i1 %cmp8.i85.peel.i to i32
  %or10.i87.peel.i = or i32 %conv9.i86.peel.i, %xor5.i81.peel.i
  %cmp11.i88.peel.i = icmp eq i32 %or10.i87.peel.i, %xor5.i81.peel.i
  %conv13.i89.peel.i = zext i1 %cmp8.i85.peel.i to i8
  ret i8 %conv13.i89.peel.i
}
```
```
define i8 @func_129() {
entry:
  %sub.i.i79.peel.i = sub i16 0, 1
  %0 = insertelement <2 x i16> <i16 poison, i16 0>, i16 %sub.i.i79.peel.i, i32 0
  %1 = icmp slt <2 x i16> zeroinitializer, %0
  %2 = zext <2 x i1> %1 to <2 x i16>
  %3 = or <2 x i16> %2, %0
  %4 = icmp eq <2 x i16> %3, %0
  %5 = extractelement <2 x i1> %1, i32 0
  %conv13.i89.peel.i = zext i1 %5 to i8
  ret i8 %conv13.i89.peel.i
}
```
```
----------------------------------------
define i8 @src() {
entry:
  %sub.i.i79.peel.i = sub i16 0, 1
  %xor5.i81.peel.i = zext i16 %sub.i.i79.peel.i to i32
  %conv7.i84.peel.i = sext i16 0 to i32
  %cmp8.i85.peel.i = icmp slt i32 %conv7.i84.peel.i, %xor5.i81.peel.i
  %conv13.i89.peel.i = zext i1 %cmp8.i85.peel.i to i8
  ret i8 %conv13.i89.peel.i
}
=>
define i8 @tgt() {
entry:
  %sub.i.i79.peel.i = sub i16 0, 1
  %#0 = insertelement <2 x i16> { poison, 0 }, i16 %sub.i.i79.peel.i, i32 0
  %#1 = icmp slt <2 x i16> { 0, 0 }, %#0
  %#5 = extractelement <2 x i1> %#1, i32 0
  %conv13.i89.peel.i = zext i1 %#5 to i8
  ret i8 %conv13.i89.peel.i
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:

Source:
i16 %sub.i.i79.peel.i = #xffff (65535, -1)
i32 %xor5.i81.peel.i = #x0000ffff (65535)
i32 %conv7.i84.peel.i = #x00000000 (0)
i1 %cmp8.i85.peel.i = #x1 (1)
i8 %conv13.i89.peel.i = #x01 (1)

Target:
i16 %sub.i.i79.peel.i = #xffff (65535, -1)
<2 x i16> %#0 = < #xffff (65535, -1), #x0000 (0) >
<2 x i1> %#1 = < #x0 (0), #x0 (0) >
i1 %#5 = #x0 (0)
i8 %conv13.i89.peel.i = #x00 (0)
Source value: #x01 (1)
Target value: #x00 (0)
```


## Comments

### Comment 1 - fhahn

cc @alexey-bataev 


---

