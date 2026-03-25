# [Clang] -Wpsabi complains too eagerly

**Author:** philnik777
**URL:** https://github.com/llvm/llvm-project/issues/128361
**Status:** Closed
**Labels:** backend:X86, ABI, clang:diagnostics
**Closed Date:** 2025-09-11T16:06:22Z

## Body

Clang currently complains when calling a function that the ABI changes if a feature is enabled, but ignores that the function is marked `[[gnu::always_inline]]`, which ensures that the type will never be part of the ABI. e.g.
```c++
using vec [[gnu::vector_size(32)]] = int;

[[gnu::always_inline]] vec test() {
  return {};
}

void call() {
  (void)test(); // complains that test() changes the ABI with avx enabled
}
```

Ironically, it doesn't complain if the address is taken, which is one of the few cases where this would actually matter, since LLVM might not be able to inline a call to the function pointer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Nikolas Klauser (philnik777)

<details>
Clang currently complains when calling a function that the ABI changes if a feature is enabled, but ignores that the function is marked `[[gnu::always_inline]]`, which ensures that the type will never be part of the ABI. e.g.
```c++
using vec [[gnu::vector_size(32)]] = int;

[[gnu::always_inline]] vec test() {
  return {};
}

void call() {
  (void)test(); // complains that test() changes the ABI with avx enabled
}
```

Ironically, it doesn't complain if the address is taken, which is one of the few cases where this would actually matter, since LLVM might not be able to inline a call to the function pointer.
</details>


---

