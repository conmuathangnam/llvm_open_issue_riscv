# clang: 18: hangs on a simple syntax error

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/69194
**Status:** Closed
**Labels:** clang:frontend, hang, clang:openmp
**Closed Date:** 2025-02-07T19:44:22Z

## Body

Compiler Explorer: https://gcc.godbolt.org/z/7c9dKMGcW

When compile this program with option `-O0 -fopenmp-simd`, Clang hangs:
```cpp
static int a;
#pragma omp declare target
b = b ^ 3 + a;
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-openmp

Author: None (wierton)

<details>
Compiler Explorer: https://gcc.godbolt.org/z/7c9dKMGcW

When compile this program with option `-O0 -fopenmp-simd`, Clang hangs:
```cpp
static int a;
#pragma omp declare target
b = b ^ 3 + a;
```
</details>


---

### Comment 2 - shiltian

Is it still the case for trunk?

---

### Comment 3 - jhuber6

It does, seems to get stuck forever in `ActOnDeclareTargetInitializer` no matter what flavor of OpenMP you use, most likely because we have a self-referential variable declaration.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wierton)

<details>
Compiler Explorer: https://gcc.godbolt.org/z/7c9dKMGcW

When compile this program with option `-O0 -fopenmp-simd`, Clang hangs:
```cpp
static int a;
#pragma omp declare target
b = b ^ 3 + a;
```
</details>


---

