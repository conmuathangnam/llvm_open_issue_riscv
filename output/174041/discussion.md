# [SLP] Miscompile when vectorizing `xor x, 0` with `and y, -1`

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/174041
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-12-31T16:31:00Z

## Body

In https://github.com/llvm/llvm-project/commit/1f82553e385f449efee92da3dca43facb4a1ee66, SLPVectorizer may incorrectly vectorize a pair of identity operations `xor x, 0` and `and y, -1` into a single vector and instruction. Because the identity constants differ (0 vs -1), SLP forms a mixed constant vector <0, -1> and emits:

```llvm
and <2 x i32> <x, y>, <0, -1>
```

This forces the first lane to `0` instead of `x`. When the original code stores results separately, this changes observable memory behavior (e.g. with `y = poison`).

Alive2 proof: https://alive2.llvm.org/ce/z/CtTFRH

Exposed from https://github.com/llvm/llvm-project/commit/1f82553e385f449efee92da3dca43facb4a1ee66, cc @alexey-bataev 

