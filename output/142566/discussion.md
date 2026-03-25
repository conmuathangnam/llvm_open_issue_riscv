# [Clang] Incorrect "Case value is not a constant expression"

**Author:** kezyr
**URL:** https://github.com/llvm/llvm-project/issues/142566

## Body

**Problem:**
Clang (both `clang++` and Clangd) incorrectly produces an error "Case value is not a constant expression" when attempting to use a static enumerator (e.g., `State::EnumType::A`) accessed through an instance of its containing struct (`state.A`) within a `switch` statement's `case` label. This should be a valid constant expression according to the C++ standard.

**Expected Behavior:**
The code should compile successfully, as `state.A` refers to the constant enumerator `State::EnumType::A` and is suitable for a `case` label. GCC compiles this code without issues.

**Reproducer Code:**
```c++
struct State {
    enum EnumType { A, B };

    EnumType value;

    constexpr operator EnumType() const noexcept { return value; }
};

struct Foo {
    State state;

    void workaround() {
        auto stateCopy = state;

        switch (stateCopy) {
            case stateCopy.A: // This compiles successfully with Clang.
            case stateCopy.B:
                break;
        }
    }

    void shouldCompile() {
        switch (state) {
            case state.A: // Clang error: "Case value is not a constant expression clang(expr_not_cce)"
            case state.B:
                break;
        }
    }
};

## Comments

### Comment 1 - frederick-vs-ja

Slightly reduced example ([Godbolt link](https://godbolt.org/z/cME845MhG)):
```C++
struct State {
  enum EnumType { A };
};

struct Foo {
  State state;

  void by_copy() {
    auto stateCopy = state;
    switch (State::EnumType{}) {
    case stateCopy.A:;
    }
  }

  void by_ref() {
    auto& stateRef = state;
    switch (State::EnumType{}) {
    case stateRef.A:; // Clang accepts since C++23, likely due to P2280R4
    }
  }

  void direct() {
    switch (State::EnumType{}) {
    case state.A:; // Clang rejects this
    }
  }
};
```

It seems that Clang overly imposes some restrictions in a manifestly evaluated expression, including an initializer expression of a `constexpr` variable or a `switch`-`case`.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Miro Rýzek (kezyr)

<details>
**Problem:**
Clang (both `clang++` and Clangd) incorrectly produces an error "Case value is not a constant expression" when attempting to use a static enumerator (e.g., `State::EnumType::A`) accessed through an instance of its containing struct (`state.A`) within a `switch` statement's `case` label. This should be a valid constant expression according to the C++ standard.

**Expected Behavior:**
The code should compile successfully, as `state.A` refers to the constant enumerator `State::EnumType::A` and is suitable for a `case` label. GCC compiles this code without issues.

**Reproducer Code:**
```c++
struct State {
    enum EnumType { A, B };

    EnumType value;

    constexpr operator EnumType() const noexcept { return value; }
};

struct Foo {
    State state;

    void workaround() {
        auto stateCopy = state;

        switch (stateCopy) {
            case stateCopy.A: // This compiles successfully with Clang.
            case stateCopy.B:
                break;
        }
    }

    void shouldCompile() {
        switch (state) {
            case state.A: // Clang error: "Case value is not a constant expression clang(expr_not_cce)"
            case state.B:
                break;
        }
    }
};
</details>


---

### Comment 3 - shafik

This goes back pretty far, at least clang-5: https://godbolt.org/z/rxrqd1Tqd

---

### Comment 4 - AaronBallman

The issue is that we get here: https://github.com/llvm/llvm-project/blob/20d70196c9a4da344d0944f3c78447c3bd7079c7/clang/lib/AST/ExprConstant.cpp#L9195

We get there because `state` is modeled as `this->state` and so we get into https://github.com/llvm/llvm-project/blob/20d70196c9a4da344d0944f3c78447c3bd7079c7/clang/lib/AST/ExprConstant.cpp#L8734 which then causes the constant expression to fail.

This is a bit of a pain because we don't know that the next AST node is also a `MemberExpr` which nominates an enumeration constant.

---

### Comment 5 - zygoloid

> This goes back pretty far, at least clang-5: https://godbolt.org/z/rxrqd1Tqd

Not really. Clang was correct to reject this prior to [P2280R4](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2280r4.html). `state.A` is rewritten to `this->state.A`, violating the old rule that [you can't name `this` unless you're evaluating a call with a known `this`](https://timsong-cpp.github.io/cppwp/n4861/expr.const#5.1).

Do we know why [expr.const]/10.1 was not deleted by P2280? It'd be a lot simpler to implement if the special case for member access expressions wasn't there, and the treatment of `this` compared to other parameters and to explicit object parameters seems a bit inexplicable:

```c++
struct X {
  void f(int *p) {
    // OK, p is constexpr-unknown
    constexpr int n = (p, 0);
    // error, unknown this only usable in a member access
    constexpr int m = (this, 0);
  }
  void g(this X &r) {
    // OK, r is constexpr-unknown
    constexpr int o = (&r, 0);
  }
};
```

Maybe we should just treat `this` like any other parameter and suggest that CWG fix the wording to say the same thing?

---

### Comment 6 - zygoloid

> Maybe we should just treat this like any other parameter and suggest that CWG fix the wording to say the same thing?

I've mailed the core reflector to ask if we can get [expr.const]/10.1 deleted.

---

