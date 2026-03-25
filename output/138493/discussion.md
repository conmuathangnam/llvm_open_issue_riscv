# [OpenMP] Assertion `(SemaRef.CurContext->isDependentContext() || B.builtAll()) && "loop exprs were not built"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/138493
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, clang:openmp
**Closed Date:** 2025-05-12T12:04:23Z

## Body

I am not confident if this is a duplicate of https://github.com/llvm/llvm-project/issues/138445, because the backtrace looks a bit different.

Reproducer:
https://godbolt.org/z/83rdTfMod
```cpp
void f(void) {
#pragma omp simd collapse(a)
  for (int i = 0; i < 10; i++)
    ;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
I am not confident if this is a duplicate of https://github.com/llvm/llvm-project/issues/138445, because the backtrace looks a bit different.

Reproducer:
https://godbolt.org/z/83rdTfMod
```cpp
void f(void) {
#pragma omp simd collapse(a)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

