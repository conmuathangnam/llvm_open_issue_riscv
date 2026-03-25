# [OpenMP] UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:2094!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139073
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, clang:openmp
**Closed Date:** 2025-05-09T10:52:08Z

## Body

Reproducible on Godbolt:
https://godbolt.org/z/EKc6x1n5x
```cpp
void f(void) {
#pragma omp tile sizes(a)
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
https://godbolt.org/z/EKc6x1n5x
```cpp
void f(void) {
#pragma omp tile sizes(a)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

