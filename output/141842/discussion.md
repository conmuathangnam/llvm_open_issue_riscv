# [HLSL] Read Only Resources incorrectly allow writing

**Author:** V-FEXrt
**URL:** https://github.com/llvm/llvm-project/issues/141842
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-07-11T22:30:11Z

## Body

Resources like `Buffer` and `StructuredBuffer` are supposed to be read only but this is not currently enforced.

Ex: https://hlsl.godbolt.org/z/sqMMffh6K fails under DXC but compiles under clang.

This is a bug that should be fixed.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Ashley Coleman (V-FEXrt)

<details>
Resources like `Buffer` and `StructuredBuffer` are supposed to be read only but this is not currently enforced.

Ex: https://hlsl.godbolt.org/z/sqMMffh6K fails under DXC but compiles under clang.

This is a bug that should be fixed.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ashley Coleman (V-FEXrt)

<details>
Resources like `Buffer` and `StructuredBuffer` are supposed to be read only but this is not currently enforced.

Ex: https://hlsl.godbolt.org/z/sqMMffh6K fails under DXC but compiles under clang.

This is a bug that should be fixed.
</details>


---

