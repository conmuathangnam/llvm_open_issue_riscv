# [clang++] Constraints aren't substituted in an unevaluated context

**Author:** katzdm
**URL:** https://github.com/llvm/llvm-project/issues/123472
**Status:** Closed
**Labels:** clang:frontend, concepts
**Closed Date:** 2025-01-22T08:08:21Z

## Body

https://godbolt.org/z/EG4Kf4xvd

The _expression_ of a _simple-requirement_ is an unevaluated operand ([[expr.prim.req.simple]](https://eel.is/c++draft/expr.prim.req#simple-1.sentence-2)), but Clang doesn't currently seem to be pushing a corresponding `ExpressionEvaluationContext` to treat them as such.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Daniel M. Katz (katzdm)

<details>
https://godbolt.org/z/EG4Kf4xvd

The _expression_ of a _simple-requirement_ is an unevaluated operand ([[expr.prim.req.simple]](https://eel.is/c++draft/expr.prim.req#simple-1.sentence-2)), but Clang doesn't currently seem to be pushing a corresponding `ExpressionEvaluationContext` to treat them as such.
</details>


---

### Comment 2 - cor3ntin

@zyn0217 In case you want to look at it

---

### Comment 3 - zyn0217

Interesting that this works

https://godbolt.org/z/qxeMz65db
```cpp
consteval bool fn() { return true; }

struct S {
  template <typename T>
  static consteval void mfn() requires (bool(&fn) || true) {}
};
```

---

