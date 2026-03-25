# [SLPVectorizer] `samesign` flags should be dropped after narrowing down the width of operands

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/120823
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2024-12-24T00:58:50Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/AvBhy9
```
; bin/opt -passes=slp-vectorizer test.ll -S
target triple = "x86_64-unknown-linux-gnu"

define i1 @test() {
entry:
  %and.i1698.1.i = zext i16 0 to i32
  %and19.i1699.2.i = and i32 %and.i1698.1.i, 0
  %and.i1698.2.i = zext i16 0 to i32
  %cmp25.i1700.2.i2 = icmp samesign uge i32 %and19.i1699.2.i, %and.i1698.1.i
  %and19.i1699.11841.i = and i32 %and.i1698.2.i, 0
  %cmp25.i1700.11842.i3 = icmp samesign uge i32 %and19.i1699.11841.i, %and.i1698.2.i
  %and.i1698.1.1.i = zext i16 0 to i32
  %and19.i1699.2.1.i = and i32 %and.i1698.1.1.i, 0
  %0 = add i16 1, 0
  %and.i1698.2.1.i = zext i16 %0 to i32
  %cmp25.i1700.2.1.i4 = icmp samesign uge i32 %and19.i1699.2.1.i, %and.i1698.1.1.i
  %and19.i1699.21846.i = and i32 %and.i1698.2.1.i, 0
  %cmp25.i1700.21847.i = icmp samesign uge i32 %and19.i1699.21846.i, %and.i1698.2.1.i
  ret i1 %cmp25.i1700.21847.i
}
```
```
define i1 @test() {
entry:
  %0 = add i16 1, 0
  %1 = insertelement <4 x i16> <i16 poison, i16 0, i16 0, i16 0>, i16 %0, i32 0
  %2 = trunc <4 x i16> %1 to <4 x i1>
  %3 = and <4 x i1> %2, zeroinitializer
  %4 = icmp samesign uge <4 x i1> %3, %2
  %5 = extractelement <4 x i1> %4, i32 0
  ret i1 %5
}
```
```

----------------------------------------
define i1 @src() {
entry:
  %#0 = add i16 1, 0
  %and.i1698.2.1.i = zext i16 %#0 to i32
  %and19.i1699.21846.i = and i32 %and.i1698.2.1.i, 0
  %cmp25.i1700.21847.i = icmp samesign uge i32 %and19.i1699.21846.i, %and.i1698.2.1.i
  ret i1 %cmp25.i1700.21847.i
}
=>
define i1 @src() {
entry:
  %#0 = add i16 1, 0
  %#1 = insertelement <4 x i16> { poison, 0, 0, 0 }, i16 %#0, i32 0
  %#2 = trunc <4 x i16> %#1 to <4 x i1>
  %#3 = and <4 x i1> %#2, { 0, 0, 0, 0 }
  %#4 = icmp samesign uge <4 x i1> %#3, %#2
  %#5 = extractelement <4 x i1> %#4, i32 0
  ret i1 %#5
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:

Source:
i16 %#0 = #x0001 (1)
i32 %and.i1698.2.1.i = #x00000001 (1)
i32 %and19.i1699.21846.i = #x00000000 (0)
i1 %cmp25.i1700.21847.i = #x0 (0)

Target:
i16 %#0 = #x0001 (1)
<4 x i16> %#1 = < #x0001 (1), #x0000 (0), #x0000 (0), #x0000 (0) >
<4 x i1> %#2 = < #x1 (1), #x0 (0), #x0 (0), #x0 (0) >
<4 x i1> %#3 = < #x0 (0), #x0 (0), #x0 (0), #x0 (0) >
<4 x i1> %#4 = < poison, #x1 (1), #x1 (1), #x1 (1) >
i1 %#5 = poison
Source value: #x0 (0)
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```


