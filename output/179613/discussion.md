# [InstCombine] Dropping `fabs` around `fmul (fabs nnan x)`, `+inf` changes `NaN` sign for `x=+0.0`

**Author:** ariskeirio
**URL:** https://github.com/llvm/llvm-project/issues/179613
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, llvm:support, floating-point
**Closed Date:** 2026-02-19T11:34:47Z

## Body

**Description**

I observed a transformation involving `fabs` and `fmul` with `+inf` that cannot pass the verification of Alive2 due to a **NaN payload mismatch**, even though both sides evaluate to NaN.https://alive2.llvm.org/ce/z/7yBXf4

**Input Program**

**Source**

```llvm
define float @src(float %x) {
  %fabs.x = call nnan float @llvm.fabs.f32(float %x)
  %mul.fabs.x = fmul float %fabs.x, 0x7FF0000000000000
  %x.is.zero = fcmp oeq float %x, 0.000000e+00
  %2 = call float @llvm.fabs.f32(float %mul.fabs.x)
  %select = select i1 %x.is.zero, float %2, float %fabs.x
  ret float %select
}
```

This is transformed to:
**Target**

```llvm
define float @tgt(float %x) {
  %fabs.x = call nnan float @llvm.fabs.f32(float %x)
  %mul.fabs.x = fmul float %fabs.x, 0x7FF0000000000000
  %x.is.zero = fcmp oeq float %x, 0.000000e+00
  %select = select i1 %x.is.zero, float %mul.fabs.x, float %fabs.x
  ret float %select
}
```



## Comments

### Comment 1 - Nathiyaa-Sengodan

Hi, I would like to work on this issue. 

This is my first LLVM contribution, so any guidance or pointers would be greatly appreciated.

---

### Comment 2 - ActuallyaDeviloper

I think this is working as specified.

[LangRef](https://llvm.org/docs/LangRef.html#behavior-of-floating-point-nan-values) says:
> Unless specified otherwise, the following rules apply whenever the IEEE 754 semantics say that a NaN value is returned: the result has a non-deterministic sign, the quiet bit and payload are non-deterministically chosen from the following set of options: ...

---

### Comment 3 - arsenm

> I think this is working as specified.

The nan sign bit still needs to be well behaved through select and fabs (as well as the other sign bit operations). The sign bit and payload bits are only nondeterministic when that value passes through a canonicalizing operation. Since the select reads a fabs value for the second input, any value taken from there must have a 0 sign-bit, including nans 

---

