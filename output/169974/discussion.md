# Wrong end location for `VarDecl` that is move constructed from a prvalue

**Author:** tJener
**URL:** https://github.com/llvm/llvm-project/issues/169974

## Body

See https://godbolt.org/z/jPcdfeW6G.

```c++
// Minimal repro.
struct S {};
S MakeS();
void f() {
  S s(MakeS());
}
```

The source range for the `VarDecl` `s` ends on the first `)`, which is part of the call to `MakeS()`.

This occurs starting in C++17. In [C++14](https://godbolt.org/z/ov9vzjG7K), there is a `ExprWithCleanups`, `CXXConstructExpr`, and `MaterializeTemporaryExpr` between the `VarDecl` and the `CallExpr` to `MakeS()`. This seems similar to #143711, except this time we lose a paren, instead of gaining one.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Li (tJener)

<details>
See https://godbolt.org/z/jPcdfeW6G.

```c++
// Minimal repro.
struct S {};
S MakeS();
void f() {
  S s(MakeS());
}
```

The source range for the `VarDecl` `s` ends on the first `)`, which is part of the call to `MakeS()`.

This occurs starting in C++17. In [C++14](https://godbolt.org/z/ov9vzjG7K), there is a `ExprWithCleanups`, `CXXConstructExpr`, and `MaterializeTemporaryExpr` between the `VarDecl` and the `CallExpr` to `MakeS()`. This seems similar to #<!-- -->143711, except this time we lose a paren, instead of gaining one.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Eric Li (tJener)

<details>
See https://godbolt.org/z/jPcdfeW6G.

```c++
// Minimal repro.
struct S {};
S MakeS();
void f() {
  S s(MakeS());
}
```

The source range for the `VarDecl` `s` ends on the first `)`, which is part of the call to `MakeS()`.

This occurs starting in C++17. In [C++14](https://godbolt.org/z/ov9vzjG7K), there is a `ExprWithCleanups`, `CXXConstructExpr`, and `MaterializeTemporaryExpr` between the `VarDecl` and the `CallExpr` to `MakeS()`. This seems similar to #<!-- -->143711, except this time we lose a paren, instead of gaining one.
</details>


---

### Comment 3 - efriedma-quic

Looking at the AST, I don't think we have anywhere to actually store the location of the closing parenthesis.  VarDecl itself doesn't have an end location: it just uses the initializer's end location.  But the initializer doesn't include the parentheses.

Probably if there isn't a constructor call, we should construct a ParenExpr to represent the parentheses.

Related testcase:

```
void f() {
    int i(0         );
}
```

---

