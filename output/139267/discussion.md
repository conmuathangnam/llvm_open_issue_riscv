# [OpenMP] Assertion 'this->_M_is_engaged()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139267
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, clang:openmp
**Closed Date:** 2025-05-09T16:32:45Z

## Body

Reproducer:
https://godbolt.org/z/T9dscdfaq
```cpp
void f(void) {
#pragma omp unroll partial(a)
  for (int i = 0; i < 10; i++)
    ;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/T9dscdfaq
```cpp
void f(void) {
#pragma omp unroll partial(a)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

