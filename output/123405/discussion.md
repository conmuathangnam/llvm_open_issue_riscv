# [clang++] Functions aren't marked immediate-escalating

**Author:** katzdm
**URL:** https://github.com/llvm/llvm-project/issues/123405
**Status:** Closed
**Labels:** clang:frontend, confirmed, consteval
**Closed Date:** 2025-01-27T14:50:52Z

## Body

https://godbolt.org/z/rvMWrhhYs

It seems that `PopExpressionEvaluationContext`, which triggers the marking of expressions as immediate-escalating, is performed _after_ the check for whether the function body contains an immediate-escalating expression. Consequently, immediate-escalating functions (i.e., lambdas and constexpr function template specializations) that contain a reference to a consteval function are never marked as immediate.

The RAII object whose teardown invokes `PopExpressionEvaluationContext` is [here](https://github.com/llvm/llvm-project/blob/d6315afff078cb4309b5614562b32520f6e3a2eb/clang/lib/Sema/SemaDecl.cpp#L15992), whereas the call that checks whether the function body contains an immediate-escalating expression is [here](https://github.com/llvm/llvm-project/blob/d6315afff078cb4309b5614562b32520f6e3a2eb/clang/lib/Sema/SemaDecl.cpp#L16006) (a few lines later, but within scope of the RAII guard).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Daniel M. Katz (katzdm)

<details>
https://godbolt.org/z/rvMWrhhYs

It seems that `PopExpressionEvaluationContext`, which triggers the marking of expressions as immediate-escalating, is performed _after_ the check for whether the function body contains an immediate-escalating expression. Consequently, immediate-escalating functions (i.e., lambdas and constexpr function template specializations) that contain a reference to a consteval function are never marked as immediate.

The RAII object whose teardown invokes `PopExpressionEvaluationContext` is [here](https://github.com/llvm/llvm-project/blob/d6315afff078cb4309b5614562b32520f6e3a2eb/clang/lib/Sema/SemaDecl.cpp#L15992), whereas the call that checks whether the function body contains an immediate-escalating expression is [here](https://github.com/llvm/llvm-project/blob/d6315afff078cb4309b5614562b32520f6e3a2eb/clang/lib/Sema/SemaDecl.cpp#L16006) (a few lines later, but within scope of the RAII guard).
</details>


---

