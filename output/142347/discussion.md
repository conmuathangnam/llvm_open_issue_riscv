# slp-vectorizer miscompile

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/142347
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-06-17T14:58:58Z

## Body

llvm commit: 9801fdf669b
Reproduce with: ```opt -passes="slp-vectorizer" bbi-107513.ll -S -o -```
Result:
```
define i16 @foo() {
entry:
  %cond3 = select i1 false, i16 1, i16 0
  ret i16 %cond3
}
```
The function should return the value 1, but with slp-vectorizer we get 0 instead.

This starts happening with d41e517748e
```
[SLP] Make getSameOpcode support interchangeable instructions. (#135797)

We use the term "interchangeable instructions" to refer to different
operators that have the same meaning (e.g., `add x, 0` is equivalent to
`mul x, 1`).
Non-constant values are not supported, as they may incur high costs with
little benefit.

---------

Co-authored-by: Alexey Bataev <a.bataev@gmx.com>
```

[bbi-107513.ll.gz](https://github.com/user-attachments/files/20547856/bbi-107513.ll.gz)

## Comments

### Comment 1 - HanKuanChen

I try 9d5e1449f7902329cdf448a1d238529836989582 but it works.

---

### Comment 2 - mikaelholmen

> I try [9d5e144](https://github.com/llvm/llvm-project/commit/9d5e1449f7902329cdf448a1d238529836989582) but it works.

It "works" in what way?

If I run the reproducer at 9d5e144 I get
```
define i16 @foo() {
entry:
  %cond3 = select i1 false, i16 1, i16 0
  ret i16 %cond3
}
```
which means it will return 0, which is wrong.

Compare
```opt -passes="instcombine" bbi-107513.ll -S -o -```
which correctly gives
```define i16 @foo() {
entry:
  ret i16 1
}
```
and
```opt -passes="slp-vectorizer,instcombine" bbi-107513.ll -S -o -```
which gives
```
define i16 @foo() {
entry:
  ret i16 0
}
```
which is wrong.

---

### Comment 3 - mikaelholmen

Also see https://llvm.godbolt.org/z/8Yrx4ejGa which exposes the miscompile.

---

### Comment 4 - bjope

Here is another godbolt example showing that we get different result if reordering some instructions in the input IR: https://godbolt.org/z/z3aPhPnod


---

### Comment 5 - HanKuanChen

https://github.com/llvm/llvm-project/pull/143094

---

### Comment 6 - alexey-bataev

Fixed in https://github.com/llvm/llvm-project/commit/414710c753d87d314529857e15d1ad01a76c6605

---

