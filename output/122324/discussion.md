# [SLPVectorizer] Miscompilation

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/122324
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-01-09T19:25:02Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/KF8JBo
This issue was originally reported on LoongArch.

```
; bin/opt -passes=slp-vectorizer test.ll -S
target triple = "x86_64-unknown-linux-gnu"

define i1 @src() {
entry:
  %h.promoted118.i.fr = freeze i32 1
  %invariant.op.i51 = add i32 %h.promoted118.i.fr, 0
  %conv25.i = xor i32 0, 0
  %add.i.i = add i32 %conv25.i, %h.promoted118.i.fr
  %sext.i.mask = and i32 %add.i.i, 0
  %cmp27.i = icmp eq i32 %sext.i.mask, 1
  %0 = or i1 %cmp27.i, false
  %conv25.i.1 = add i32 0, 0
  %add.i.i.1 = add i32 %conv25.i.1, %h.promoted118.i.fr
  %sext.i.1.mask = and i32 %add.i.i.1, 1
  %cmp27.i.1 = icmp eq i32 %sext.i.1.mask, 0
  %conv25.1.i.1 = xor i32 0, 0
  %add.i.1.i.1 = add i32 %conv25.1.i.1, %h.promoted118.i.fr
  %sext.1.i.1.mask = and i32 %add.i.1.i.1, 1
  %cmp27.1.i.1 = icmp eq i32 %sext.1.i.1.mask, 0
  %add.i.2.reass.i.1 = add i32 %invariant.op.i51, %conv25.i.1
  %sext.2.i.1.mask = and i32 %add.i.2.reass.i.1, 1
  %cmp27.2.i.1 = icmp eq i32 %sext.2.i.1.mask, 0
  %1 = or i1 %cmp27.1.i.1, %cmp27.2.i.1
  %2 = or i1 %cmp27.i.1, %1
  %3 = or i1 %0, %2
  ret i1 %3
}
```
```

----------------------------------------
define i1 @src() {
entry:
  %h.promoted118.i.fr = freeze i32 1
  %invariant.op.i51 = add i32 %h.promoted118.i.fr, 0
  %conv25.i = xor i32 0, 0
  %add.i.i = add i32 %conv25.i, %h.promoted118.i.fr
  %sext.i.mask = and i32 %add.i.i, 0
  %cmp27.i = icmp eq i32 %sext.i.mask, 1
  %#0 = or i1 %cmp27.i, 0
  %conv25.i.1 = add i32 0, 0
  %add.i.i.1 = add i32 %conv25.i.1, %h.promoted118.i.fr
  %sext.i.1.mask = and i32 %add.i.i.1, 1
  %cmp27.i.1 = icmp eq i32 %sext.i.1.mask, 0
  %conv25.1.i.1 = xor i32 0, 0
  %add.i.1.i.1 = add i32 %conv25.1.i.1, %h.promoted118.i.fr
  %sext.1.i.1.mask = and i32 %add.i.1.i.1, 1
  %cmp27.1.i.1 = icmp eq i32 %sext.1.i.1.mask, 0
  %add.i.2.reass.i.1 = add i32 %invariant.op.i51, %conv25.i.1
  %sext.2.i.1.mask = and i32 %add.i.2.reass.i.1, 1
  %cmp27.2.i.1 = icmp eq i32 %sext.2.i.1.mask, 0
  %#1 = or i1 %cmp27.1.i.1, %cmp27.2.i.1
  %#2 = or i1 %cmp27.i.1, %#1
  %#3 = or i1 %#0, %#2
  ret i1 %#3
}
=>
define i1 @src() {
entry:
  %h.promoted118.i.fr = freeze i32 1
  %#0 = insertelement <4 x i32> { 0, 0, poison, 0 }, i32 %h.promoted118.i.fr, i32 2
  %#1 = xor <4 x i32> { 0, 0, 0, 0 }, %#0
  %#2 = add <4 x i32> { 0, 0, 0, 0 }, %#0
  %#3 = shufflevector <4 x i32> %#1, <4 x i32> %#2, 0, 1, 6, 7
  %#6 = shufflevector <4 x i32> %#0, <4 x i32> %#3, 0, 7
  %#7 = shufflevector <2 x i32> %#6, <2 x i32> poison, 0, 0, 1, 0
  %#8 = add <4 x i32> %#3, %#7
  %#9 = and <4 x i32> %#8, { 0, 1, 1, 1 }
  %#10 = icmp eq <4 x i32> %#9, { 1, 0, 0, 0 }
  %#11 = reduce_or <4 x i1> %#10
  %op.rdx = or i1 %#11, 0
  ret i1 %op.rdx
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:

Source:
i32 %h.promoted118.i.fr = #x00000001 (1)
i32 %invariant.op.i51 = #x00000001 (1)
i32 %conv25.i = #x00000000 (0)
i32 %add.i.i = #x00000001 (1)
i32 %sext.i.mask = #x00000000 (0)
i1 %cmp27.i = #x0 (0)
i1 %#0 = #x0 (0)
i32 %conv25.i.1 = #x00000000 (0)
i32 %add.i.i.1 = #x00000001 (1)
i32 %sext.i.1.mask = #x00000001 (1)
i1 %cmp27.i.1 = #x0 (0)
i32 %conv25.1.i.1 = #x00000000 (0)
i32 %add.i.1.i.1 = #x00000001 (1)
i32 %sext.1.i.1.mask = #x00000001 (1)
i1 %cmp27.1.i.1 = #x0 (0)
i32 %add.i.2.reass.i.1 = #x00000001 (1)
i32 %sext.2.i.1.mask = #x00000001 (1)
i1 %cmp27.2.i.1 = #x0 (0)
i1 %#1 = #x0 (0)
i1 %#2 = #x0 (0)
i1 %#3 = #x0 (0)

Target:
i32 %h.promoted118.i.fr = #x00000001 (1)
<4 x i32> %#0 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<4 x i32> %#1 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<4 x i32> %#2 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<4 x i32> %#3 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<2 x i32> %#6 = < #x00000000 (0), #x00000000 (0) >
<4 x i32> %#7 = < #x00000000 (0), #x00000000 (0), #x00000000 (0), #x00000000 (0) >
<4 x i32> %#8 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<4 x i32> %#9 = < #x00000000 (0), #x00000000 (0), #x00000001 (1), #x00000000 (0) >
<4 x i1> %#10 = < #x0 (0), #x1 (1), #x0 (0), #x1 (1) >
i1 %#11 = #x1 (1)
i1 %op.rdx = #x1 (1)
Source value: #x0 (0)
Target value: #x1 (1)

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```
llvm version: d80bdf7261e7d785b98f827eeed950b2147f70e6


