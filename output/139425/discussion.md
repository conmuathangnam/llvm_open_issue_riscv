# [OpenMP] Assertion `NextVal != ArgumentEnd && "Value for integer select modifier was" " larger than the number of options in the diagnostic string!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139425
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, clang:openmp
**Closed Date:** 2025-05-11T01:08:04Z

## Body

Reproducer:
https://godbolt.org/z/oxKMY78Tc
```cpp
void f(){
#pragma omp atomic
++(int){0};
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/oxKMY78Tc
```cpp
void f(){
#pragma omp atomic
++(int){0};
}
```
</details>


---

### Comment 2 - k-arrows

This issue seems to be a duplicate of https://github.com/llvm/llvm-project/issues/107979. I will close this issue.

---

