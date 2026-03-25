# [OpenMP] Assertion `Idx < getNumArgs() && "Argument index out of range!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139266
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, clang:openmp
**Closed Date:** 2025-05-09T15:12:20Z

## Body

Reproducible on Godbolt:
https://godbolt.org/z/jjxM8vqYj
```cpp
void f(void) {
#pragma omp distribute dist_schedule(static, 0)
  for (int i = 0; i < 10; i++)
    ;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/jjxM8vqYj
```cpp
void f(void) {
#pragma omp distribute dist_schedule(static, 0)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

