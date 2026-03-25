# [OpenMP] Assertion `!isTokenSpecial() && "Should consume special tokens with Consume*Token"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139360
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, clang:openmp
**Closed Date:** 2025-05-12T17:44:04Z

## Body

Reproducer:
https://godbolt.org/z/6M3soqfGo
```cpp
void f(){
#pragma omp cancel(
```

I don't know if this issue is a dulicate of https://github.com/llvm/llvm-project/issues/113722.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6M3soqfGo
```cpp
void f(){
#pragma omp cancel(
```

I don't know if this issue is a dulicate of https://github.com/llvm/llvm-project/issues/113722.
</details>


---

### Comment 2 - AaronBallman

Same class of problem, but a distinct issue from #113722

---

