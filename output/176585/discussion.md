# Function parameter is accessible inside of default argument lambda

**Author:** dgodfrey206
**URL:** https://github.com/llvm/llvm-project/issues/176585
**Status:** Closed
**Labels:** duplicate, clang:frontend, accepts-invalid, regression:19
**Closed Date:** 2026-01-17T19:48:04Z

## Body

This should be ill-formed since the parameter is being odr-used in an evaluated context. Moreover, `x` should not be accessible from within the lambda since it isn't captured.

```cpp
void foo( int x, int (*)() = []{ return x; } );
```

An additional issue is that adding a capture clause to this lambda still allows it to be converted to a function pointer:

```cpp
void foo( int x, int (*)() = [x]{ return x; } );
```

https://godbolt.org/z/118j7fqPa

## Comments

### Comment 1 - keinflue

Probably duplicate of https://github.com/llvm/llvm-project/issues/152467. While this one does not crash with an assertion, it also started being accepted incorrectly with Clang 19: https://godbolt.org/z/KxKo8KEoo

I will verify whether the same commit is responsible.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Godfrey (dgodfrey206)

<details>
This should be ill-formed since the parameter is being odr-used in an evaluated context. Moreover, `x` should not be accessible from within the lambda since it isn't captured.

```cpp
void foo( int x, int (*)() = []{ return x; } );
```

An additional issue is that adding a capture clause to this lambda still allows it to be converted to a function pointer:

```cpp
void foo( int x, int (*)() = [x]{ return x; } );
```

https://godbolt.org/z/118j7fqPa
</details>


---

### Comment 3 - keinflue

The same commit https://github.com/llvm/llvm-project/commit/3d361b225fe89ce1d8c93639f27d689082bd8dad as in https://github.com/llvm/llvm-project/issues/152467 is responsible, so I am closing this as duplicate.

---

