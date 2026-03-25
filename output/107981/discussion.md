# [Clang] Bogus types when using __builtin_shufflevector on bool vectors

**Author:** philnik777
**URL:** https://github.com/llvm/llvm-project/issues/107981
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-08-21T21:13:53Z

## Body

```c++
using vec __attribute__((ext_vector_type(4))) = bool;

auto test() {
  return __builtin_shufflevector(vec{}, vec{}, 0, 1, 2, 3, 4, 5, 6, 7);
}
```
`test()` returns a `__attribute__((__vector_size__(8 * sizeof(bool))))`, which is obviously incorrect. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nikolas Klauser (philnik777)

<details>
```c++
using vec __attribute__((ext_vector_type(4))) = bool;

auto test() {
  return __builtin_shufflevector(vec{}, vec{}, 0, 1, 2, 3, 4, 5, 6, 7);
}
```
`test()` returns a `__attribute__((__vector_size__(8 * sizeof(bool))))`, which is obviously incorrect. 
</details>


---

### Comment 2 - jhuber6

Looked into this, it's because when the size is greater it creates a new vector type. The problem is that we only pass `VectorType::Generic` which then creates the GNU vector type https://github.com/llvm/llvm-project/blob/main/clang/lib/AST/ASTContext.cpp#L4661. I'm not sure how this is intended to work, if this was always returning a GNU vector I feel like this would be more broken, so I'm wondering if this is just an edge case and I'm missing something. Maybe someone more familiar with the Clang vector handling can chime in like @efriedma-quic or @erichkeane.

---

