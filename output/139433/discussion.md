# [OpenMP] crash on an invalid sizes argument (stripe directive)

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139433
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, clang:openmp
**Closed Date:** 2025-05-13T16:55:54Z

## Body

Reproducible on Godbolt:
https://godbolt.org/z/7oYo6WaW8
```cpp
void f() {
#pragma omp stripe sizes(a)
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
https://godbolt.org/z/7oYo6WaW8
```cpp
void f() {
#pragma omp stripe sizes(a)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

