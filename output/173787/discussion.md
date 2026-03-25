# [InstCombine] `foldBitCeil` miscompiles non-power-of-two integer widths

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/173787
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-12-29T12:17:37Z

## Body

There exists an incorrect transformation for non-power-of-two integer widths in `foldBitCeil`.

The rewrite replaces the original computation `shift = BitWidth - ctlz(x)` to `shift = (-ctlz(x)) & (BitWidth - 1)`. 

This masking trick is only valid when `BitWidth` is a power of two, because in that case `(BitWidth - 1)` is an all-ones mask and the expression effectively computes `(BitWidth - ctlz) mod BitWidth`.

For non-power-of-two widths (e.g. `i33`), `BitWidth - 1` is not an all-ones mask. As a result, `(-ctlz) & (BitWidth - 1)` does not compute `BitWidth - ctlz` and can produce an entirely different shift amount.

For example, with `i33` and `x = 7`:

- `ctlz(7) = 30`
- Source computes `shift = 33 - 30 = 3`, yielding `1 << 3 = 8`
- Transformed code computes `(-30) & 32 = 32`, yielding` 1 << 32`

This causes a value mismatch and makes the transformation incorrect for non-power-of-two bit widths. 

Alive2 proof: https://alive2.llvm.org/ce/z/Z3Gq8Q

Exposed from https://github.com/llvm/llvm-project/commit/2c094ac761912eea0d7e8ccb140bc647b5378bdf, cc @dtcxzyw 

## Comments

### Comment 1 - cardigan1008

I can propose a patch for this. 

---

