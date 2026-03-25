# [HLSL] Fix issue with implicit bound overflow validation in frontend

**Author:** joaosaffran
**URL:** https://github.com/llvm/llvm-project/issues/159478
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-09-22T22:18:15Z

## Body

Currently, HLSL frontend seems to be wrongfully calculating when an offset overflows: https://hlsl.godbolt.org/z/16sWWGf97. This issue is to investigate and fix this bug.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (joaosaffran)

<details>
Currently, HLSL frontend seems to be wrongfully calculating when an offset overflows: https://hlsl.godbolt.org/z/16sWWGf97. This issue is to investigate and fix this bug.
</details>


---

