# Assertion `assert(isa<GlobalAlias>(GV))` failed in `LTOModule.cpp`

**Author:** Thibault-Monnier
**URL:** https://github.com/llvm/llvm-project/issues/179131
**Status:** Open
**Labels:** crash, LTO

## Body

This issue was found when trying to merge #175452, and broke chromium CI : https://github.com/llvm/llvm-project/issues/175452#issuecomment-3775431831.

Perhaps it is due to the usage of function multiversioning?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Thibault Monnier (Thibault-Monnier)

<details>
This issue was found when trying to merge #<!-- -->175452: https://github.com/llvm/llvm-project/issues/175452#issuecomment-3775431831.

Perhaps it is due to the usage of function multiversioning?
</details>


---

