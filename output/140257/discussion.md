# [OpenMP] Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140257

## Body

Reproducer:
https://godbolt.org/z/zs3xzac18
```c
#include <omp.h>
void foo () {
  int i;
  #pragma omp target device (omp_is_initial_device)
  i++;
}
```

The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/libgomp/testsuite/libgomp.c/target-41.c

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/zs3xzac18
```c
#include &lt;omp.h&gt;
void foo () {
  int i;
  #pragma omp target device (omp_is_initial_device)
  i++;
}
```
</details>


---

### Comment 2 - AaronBallman

Reproducer which doesn't require any headers:
```
extern int omp_is_initial_device (void);
void foo () {
  int i;
  #pragma omp target device (omp_is_initial_device)
  i++;
}
```

---

### Comment 3 - AaronBallman

I think the issue is here: https://github.com/llvm/llvm-project/blob/85fe4ab8a3faa58317c247051df2195c89bed0fa/clang/lib/Sema/SemaOpenMP.cpp#L15832

The comments on that function are:
```
/// Attempt to convert the given expression to a type which is accepted
/// by the given converter.
///
/// This routine will attempt to convert an expression of class type to a
/// type accepted by the specified converter. In C++11 and before, the class
/// must have a single non-explicit conversion function converting to a matching
/// type. In C++1y, there can be multiple such conversion functions, but only
/// one target type.
```
and indeed that function does an early return if the type is not a class type: https://github.com/llvm/llvm-project/blob/85fe4ab8a3faa58317c247051df2195c89bed0fa/clang/lib/Sema/SemaOverload.cpp#L6780 but it also early returns for C in general while not performing any conversion. But notice that it returns `From` instead of `ExprError()`, so the caller has no idea that no conversions happened. Thus, we continue along after emitting the error diagnostic, and later invariants do not hold because no conversion too place.

When I change that into an `ExprError()`, the only failures we get are all OpenMP failures -- OpenMP is relying on returning `From` rather than an error.

I think either OpenMP is using the wrong conversion function and should be using `PerformImplicitConversion()` or the logic in OpenMP needs to be adjusted to not assume you get the source expression on non-class types and in C.

CC @alexey-bataev 

---

### Comment 4 - erichkeane

>I think either OpenMP is using the wrong conversion function and should be using PerformImplicitConversion() or the logic in OpenMP needs to be adjusted to not assume you get the source expression on non-class types and in C.
Based on my look, I strongly suspect its the latter.  But @alexey-bataev should look into this.

---

