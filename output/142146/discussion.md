# opt -passes=codegenprepare crashes on a null TargetMachine

**Author:** arsenm
**URL:** https://github.com/llvm/llvm-project/issues/142146
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-10-23T04:57:04Z

## Body

The new pass manager just crashes on CodeGenPrepare (and probably other IR codegen passes) if there is no target machine: https://godbolt.org/z/qoP6qTKG8

The old pass manager version does not crash, but it explicitly checks in the pass and fatal errors if the TargetMachine is null. The new PM could maybe guard this for all codegen passes?

## Comments

### Comment 1 - arsenm

It still crashes even with -mtriple 

---

### Comment 2 - arsenm

Also crashes with -mtriple from null ProfileSummaryInfo: https://godbolt.org/z/1YTxzx86W

---

### Comment 3 - paperchalice

> Also crashes with -mtriple from null ProfileSummaryInfo: https://godbolt.org/z/1YTxzx86W

It works with `-passes='require<profile-summary>,function(codegenprepare)'`, we may need a better error reporting mechanism for required outer analysis results?

---

