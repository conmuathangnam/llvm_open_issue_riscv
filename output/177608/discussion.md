# [Clang][OpenMP] Crash for OpenMP loop referencing auto-typed VMT

**Author:** tstanisl
**URL:** https://github.com/llvm/llvm-project/issues/177608
**Status:** Open
**Labels:** crash, clang:openmp

## Body

The compilation crashes for the following C23 code.

```
void foo(int N, int (*arr_)[N][N]) {
    auto arr = arr_;
    #pragma omp parallel for
    for (int n = 0; n < N; ++n)
            (*arr)[n][n] = 1;
}
```

It looks that there is some issue with OpenMP sections capturing multidimensional VLA-typed objects which type was auto-deduced.  

See [godbolt](https://godbolt.org/z/e9Mf6M4MM).

The similar issue is also observed for
```
    __auto_type arr = arr_;
```
or
```
    auto arr = (int(*)[N][N])arr_;
```

However, there is *no crash* if:

1. The `arr_` is de-referenced directly `(*arr_)[n][n]`
2.  No type-deduction is used
```
int (*arr)[N][N] = arr_;
```
3. A pointer to 1d VLA-typed array is used
```
void bar(int N, int (*arr1d_)[N]) {
    auto arr1d = arr1d_; /* OK */
    #pragma omp parallel for
    for (int n = 0; n < N; ++n)
            (*arr1d)[n] = 1;
}
```

Take a look on [godbolt](https://godbolt.org/z/Pa37Ex9n1) to easily navigate between mentioned cases.


