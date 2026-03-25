# Crash in constant evaluation of explicit object member assignment operator

**Author:** cor3ntin
**URL:** https://github.com/llvm/llvm-project/issues/142835
**Status:** Closed
**Labels:** clang:frontend, c++23, crash-on-valid
**Closed Date:** 2025-06-05T17:30:27Z

## Body

```cpp
struct MoveMe {
  MoveMe& operator=(this MoveMe&, const MoveMe&) = default;
  constexpr MoveMe& operator=(this MoveMe& self, MoveMe&& other) {
    self.value = other.value;
    other.value = 0;
    return self;
  }
  int value = 4242;
};

struct S {
  S& operator=(this S&, const S&) = default;
  S& operator=(this S&, S&&) = default;

  MoveMe move_me;
};

constexpr bool f() {
  S s1{};
  S s2{};
  s2 = s1;
  return true;
}

static_assert(f());
```

`
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:6656: bool HandleFunctionCall(clang::SourceLocation, const clang::FunctionDecl*, const {anonymous}::LValue*, const clang::Expr*, llvm::ArrayRef<const clang::Expr*>, {anonymous}::CallRef, const clang::Stmt*, {anonymous}::EvalInfo&, clang::APValue&, const {anonymous}::LValue*): Assertion This && (MD->isCopyAssignmentOperator() || MD->isMoveAssignmentOperator())' failed.
`

https://compiler-explorer.com/z/1nxjPP5Yh

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Corentin Jabot (cor3ntin)

<details>
```cpp
struct MoveMe {
  MoveMe&amp; operator=(this MoveMe&amp;, const MoveMe&amp;) = default;
  constexpr MoveMe&amp; operator=(this MoveMe&amp; self, MoveMe&amp;&amp; other) {
    self.value = other.value;
    other.value = 0;
    return self;
  }
  int value = 4242;
};

struct S {
  S&amp; operator=(this S&amp;, const S&amp;) = default;
  S&amp; operator=(this S&amp;, S&amp;&amp;) = default;

  MoveMe move_me;
};

constexpr bool f() {
  S s1{};
  S s2{};
  s2 = s1;
  return true;
}

static_assert(f());
```

`
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:6656: bool HandleFunctionCall(clang::SourceLocation, const clang::FunctionDecl*, const {anonymous}::LValue*, const clang::Expr*, llvm::ArrayRef&lt;const clang::Expr*&gt;, {anonymous}::CallRef, const clang::Stmt*, {anonymous}::EvalInfo&amp;, clang::APValue&amp;, const {anonymous}::LValue*): Assertion `This &amp;&amp; (MD-&gt;isCopyAssignmentOperator() || MD-&gt;isMoveAssignmentOperator())' failed.
`

https://compiler-explorer.com/z/1nxjPP5Yh
</details>


---

