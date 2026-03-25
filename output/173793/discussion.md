# [InstCombine] Vector `fshl` retains invalid `range` after demanded-bits simplification

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/173793
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-12-29T16:05:51Z

## Body

In https://github.com/llvm/llvm-project/pull/124429, the patch correctly drops `range` for scalar `fshl` when operands are simplified based on demanded bits, but it does not consistently do so for vector `fshl`.

In the vector case, `SimplifyDemandedBits` can still change the operands such that the original `range` no longer holds, yet the `range` attribute is preserved, causing the `fshl` result to become poison.

For example, we have this vector case:

```llvm
%zext = zext <2 x i1> %x to <2 x i32>
%or   = or disjoint <2 x i32> %zext, <i32 -2, i32 -2>
%fshl = call <2 x i32> @llvm.fshl.v2i32(
           <2 x i32> %or,
           <2 x i32> <i32 -2, i32 -2>,
           <2 x i32> <i32 1,  i32 1>),
         !range !{i32 -4, i32 2}
%tr   = trunc <2 x i32> %fshl to <2 x i8>
```

After demanded-bits simplification, `%or` is rewritten to `<126, 126>`, which changes the result of fshl to `<253, 253>`.

However, the original `range [-4, 2)` attribute is still preserved on the vector `fshl`, even though the new value clearly violates it. As a result, the `fshl` produces `poison`. 

Alive2 proof: https://alive2.llvm.org/ce/z/JTXAYD

Exposed from https://github.com/llvm/llvm-project/commit/2131115be5b9d8b39af80973d9b64c0adc41d38d, cc @MaskRay 

## Comments

### Comment 1 - cardigan1008

I can propose a patch for this. Please assign it to me, thanks. 

---

