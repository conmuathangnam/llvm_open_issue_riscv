# [clang] LazyGenerationalUpdatePtr PLTT overclaims bits available on 32-bit systems

**Author:** kuhar
**URL:** https://github.com/llvm/llvm-project/issues/188269
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2026-03-24T20:58:57Z

## Body

https://github.com/llvm/llvm-project/blob/26fcbd5aaed125ab99b19299f4ec686912a7af92/clang/include/clang/AST/ExternalASTSource.h#L521-L534

`PointerLikeTypeTraits` claims `T_bits - 1` spare bits, but the inner `PointerUnion<T, LazyData*>` actually has `min(T_bits, LazyData*_bits) - 1`.

On 64-bit these are equal (`LazyData*` is 3 bits). On 32-bit, `LazyData*` has only 2 bits (`alignof = 4`), so with `T = Decl*` (3 bits, `alignas(8)`): `claimed = 2` while `actual = 1`.

Discovered during `PointerUnion` refactoring: https://github.com/llvm/llvm-project/pull/187950#issuecomment-4116974450

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jakub Kuderski (kuhar)

<details>
https://github.com/llvm/llvm-project/blob/26fcbd5aaed125ab99b19299f4ec686912a7af92/clang/include/clang/AST/ExternalASTSource.h#L521-L534

`PointerLikeTypeTraits` claims `T_bits - 1` spare bits, but the inner `PointerUnion&lt;T, LazyData*&gt;` actually has `min(T_bits, LazyData*_bits) - 1`.

On 64-bit these are equal (`LazyData*` is 3 bits). On 32-bit, `LazyData*` has only 2 bits (`alignof = 4`), so with `T = Decl*` (3 bits, `alignas(8)`): `claimed = 2` while `actual = 1`.

Discovered during `PointerUnion` refactoring: https://github.com/llvm/llvm-project/pull/187950#issuecomment-4116974450
</details>


---

