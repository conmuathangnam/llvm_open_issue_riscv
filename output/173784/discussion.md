# [SLP] Miscompilation when vectorizing boolean selects with poison

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/173784
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-12-28T19:54:38Z

## Body

In https://github.com/llvm/llvm-project/commit/899336735aeb2b41a48b6ac2c895da5e0f22dbf0, there seems to be an uncovered correctness issue in the handling of boolean select chains involving poison. 

Consider the following scalar IR:

```llvm
%op1 = select i1 %a, i1 true, i1 poison
%op2 = select i1 %b, i1 true, i1 poison
%res = select i1 %op1, i1 true, i1 %op2
ret i1 %res
```

Semantically, this code is safe:
if `%a` is true, `%op1` is true, and `%res` immediately becomes true.
In this case, the value of `%op2` (and the poison on its false branch) is never observed.

After the current transformation, this pattern is rewritten by grouping `%op1` and `%op2` into a vector and lowering the logic to a `llvm.vector.reduce.or`. This transformation eagerly evaluates all vector lanes.

As a result, if `%op2` is poison, the reduction observes that poison and produces a poison result, even when `%op1` is true. This introduces poison in cases where the original scalar code would have produced a well-defined true.

Alive2 proof: https://alive2.llvm.org/ce/z/oE2h69

cc @alexey-bataev 


