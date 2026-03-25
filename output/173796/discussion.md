# [SLP] Incorrect poison propagation when vectorizing boolean select chains

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/173796
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-12-28T21:52:20Z

## Body

In https://github.com/llvm/llvm-project/commit/889215a30ed60474e573f9632d1fa362dfa1b04e, there is still an uncovered correctness issue in SLP vectorization of boolean select chains involving poison, specifically when an intermediate reduction node is used in mixed positions within the same reduction tree.

Consider the following IR:

```llvm
%i1 = select i1 %p, i1 true, i1 false
%safe   = select i1 %f, i1 %i1, i1 false
%unsafe = select i1 %i1, i1 true, i1 false
%res    = select i1 %safe, i1 %unsafe, i1 false
ret i1 %res
```

If `%f` is false, `%safe` evaluates to false, and `%res` immediately becomes false. 
In this case, `%unsafe` is never observed, and the poison carried by `%i1` is correctly suppressed by the scalar semantics. 

However, after the current SLP transformation, `%safe` and `%unsafe` are combined into a single boolean reduction tree.
The intermediate value `%i1` is treated as a reduction operand and participates directly in the vectorized reduction logic. 

Because `%i1` is used as a condition operand in `%unsafe`, the current logic assumes that `%i1` is poison-propagating and therefore skips inserting a freeze.
This classification ignores the fact that `%i1` is also used as a value operand in `%safe`, where poison is semantically suppressed by a dominating constant false.

Alive2 proof: https://alive2.llvm.org/ce/z/qPCjs6

Exposed from https://github.com/llvm/llvm-project/commit/889215a30ed60474e573f9632d1fa362dfa1b04e, cc @alexey-bataev 

