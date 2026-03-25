# [SLPVectorizer] Miscompile in slp vectorizer (shift produces poison)

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/165878
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-10-31T22:25:20Z

## Body

SLPVectorizer (`opt -passes=slp-vectorizer -slp-threshold=-99999`) turns the following IR:
```llvm
target triple = "x86_64-unknown-linux-gnu"

define float @foo(i8 %0) {
entry:
  %1 = sext i8 0 to i32
  %2 = lshr i32 %1, 27
  %3 = sext i8 %0 to i32
  %reass.add.epil = mul i32 %3, 2
  %4 = or i32 %reass.add.epil, %2
  switch i32 %4, label %exit [
    i32 0, label %exit
    i32 1, label %exit
  ]

exit:                                      ; preds = %body, %body, %body
  ret float 0.000000e+00
}
```
into
```llvm
define float @foo(i8 %0) {
entry:
  %1 = insertelement <2 x i8> <i8 poison, i8 0>, i8 %0, i32 0
  %2 = sext <2 x i8> %1 to <2 x i16>
  %3 = mul <2 x i16> %2, <i16 2, i16 27>
  %4 = lshr <2 x i16> %2, <i16 2, i16 27>
  %5 = shufflevector <2 x i16> %3, <2 x i16> %4, <2 x i32> <i32 0, i32 3>
  %6 = extractelement <2 x i16> %5, i32 0
  %7 = sext i16 %6 to i32
  %8 = extractelement <2 x i16> %5, i32 1
  %9 = zext i16 %8 to i32
  %10 = or i32 %7, %9
  switch i32 %10, label %exit [
    i32 0, label %exit
    i32 1, label %exit
  ]

exit:
  ret float 0.000000e+00
}
```

Note that it now has `i16` shifts by `27` which later get turned into poison.

Godbolt link: https://godbolt.org/z/shj5GK5nd
Alive2 link: https://alive2.llvm.org/ce/z/CM7Pmp

