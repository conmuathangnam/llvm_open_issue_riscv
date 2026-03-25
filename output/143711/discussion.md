# Wrong source range for CXXTemporaryObjectExpr

**Author:** tJener
**URL:** https://github.com/llvm/llvm-project/issues/143711
**Status:** Closed
**Labels:** clang:frontend, confirmed
**Closed Date:** 2025-06-26T09:02:15Z

## Body

See https://godbolt.org/z/6cdcj1qqY

```c++
struct S {
    S(int, int);
};

void f() {
    S(S(0, 1));
}
```

When the `CXXTemporaryObjectExpr` is wrapped with a `CXXFunctionalCastExpr`, the source range of the `CXXTemporaryObjectExpr` contains the closing parenthesis of the `CXXFunctionalCastExpr`.

This occurs starting in `-std=c++17`, where guaranteed copy elision is in effect. In C++14, there is an intervening `CXXConstructExpr` and the `CXXTemporaryObjectExpr` has the correct source range (although Compiler Explorer's highlighting of AST nodes includes the closing parenthesis, as if `))` was a single token).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Li (tJener)

<details>
See https://godbolt.org/z/6cdcj1qqY

```c++
struct S {
    S(int, int);
};

void f() {
    S(S(0, 1));
}
```

When the `CXXTemporaryObjectExpr` is wrapped with a `CXXFunctionalCastExpr`, the source range of the `CXXTemporaryObjectExpr` contains the closing parenthesis of the `CXXFunctionalCastExpr`.

This occurs starting in `-std=c++17`, where guaranteed copy elision is in effect. In C++14, there is an intervening `CXXConstructExpr` and the `CXXTemporaryObjectExpr` has the correct source range (although Compiler Explorer's highlighting of AST nodes includes the closing parenthesis, as if `))` was a single token).
</details>


---

