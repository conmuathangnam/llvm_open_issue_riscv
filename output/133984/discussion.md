# MemcpyOpt doesn't strip TBAA correctly

**Author:** gbaraldi
**URL:** https://github.com/llvm/llvm-project/issues/133984
**Status:** Closed
**Labels:** miscompilation, release:backport, release:cherry-pick-failed, llvm:transforms
**Closed Date:** 2025-04-26T21:58:51Z

## Body

While debugging https://github.com/JuliaLang/julia/issues/57959. It got minimized to what looks to be a bug in MemCpyOpt, where it strips the noalias attribute of a memcpy https://github.com/llvm/llvm-project/blob/3c7a0e6c826b8bcfa4ec6154fd4247658ca3a03f/llvm/lib/Transforms/Scalar/MemCpyOptimizer.cpp#L1682-L1688 but not the other aliasing metadatas (tbaa,tbaa.struct). Probably using `combineMetadata` as used elsewhere in the pass
@khei4 was the one that originally added this optimization

## Comments

### Comment 1 - gbaraldi

https://godbolt.org/z/qv1Mqo7eP For a small reproducer 

---

### Comment 2 - nikic

There is a related PR here: https://github.com/llvm/llvm-project/pull/129537

---

### Comment 3 - nikic

/cherry-pick 716b02d8c575afde7af1af13df145019659abca2

---

### Comment 4 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: 716b02d8c575afde7af1af13df145019659abca2

https://github.com/llvm/llvm-project/actions/runs/14404185715

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 5 - tstellar

@nikic Were you able to manually backport this one?

---

### Comment 6 - nikic

@tstellar Manual backport: https://github.com/llvm/llvm-project/pull/135615

---

