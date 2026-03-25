# [clang] constant evaluation of `for` loop does not run destructor for condition variable

**Author:** zygoloid
**URL:** https://github.com/llvm/llvm-project/issues/139818
**Status:** Closed
**Labels:** good first issue, c++20, clang:frontend, accepts-invalid, rejects-valid, c++23, constexpr
**Closed Date:** 2025-06-05T21:50:22Z

## Body

If a condition variable is declared in the second clause of a three-clause `for` loop, clang doesn't destroy it when constant-evaluating the loop. For example:
```c++
struct X {
  constexpr ~X() noexcept(false) { throw "oops"; }
  constexpr operator bool() {
    return b;
  }
  bool b;
};
constexpr bool f() {
  for (bool b = false; X x = {b}; b = true) {}
  return true;
}
static_assert(f());
```
Clang treats this as constant, but the destructor for `x` throws an exception.

It looks like the problem is that we fail to call `IterScope.destroy()` [here](https://github.com/llvm/llvm-project/blob/5f41928689125667cf645835fa9fefee97511d8a/clang/lib/AST/ExprConstant.cpp#L5742), when terminating a loop iteration because the condition evaluated to `false`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Richard Smith (zygoloid)

<details>
If a condition variable is declared in the second clause of a three-clause `for` loop, clang doesn't destroy it when constant-evaluating the loop. For example:
```c++
struct X {
  constexpr ~X() noexcept(false) { throw "oops"; }
  constexpr operator bool() {
    return b;
  }
  bool b;
};
constexpr bool f() {
  for (bool b = false; X x = {b}; b = true) {}
  return true;
}
static_assert(f());
```
Clang treats this as constant, but the destructor for `x` throws an exception.

It looks like the problem is that we fail to call `IterScope.destroy()` [here](https://github.com/llvm/llvm-project/blob/5f41928689125667cf645835fa9fefee97511d8a/clang/lib/AST/ExprConstant.cpp#L5742), when terminating a loop iteration because the condition evaluated to `false`.
</details>


---

### Comment 2 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 3 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Richard Smith (zygoloid)

<details>
If a condition variable is declared in the second clause of a three-clause `for` loop, clang doesn't destroy it when constant-evaluating the loop. For example:
```c++
struct X {
  constexpr ~X() noexcept(false) { throw "oops"; }
  constexpr operator bool() {
    return b;
  }
  bool b;
};
constexpr bool f() {
  for (bool b = false; X x = {b}; b = true) {}
  return true;
}
static_assert(f());
```
Clang treats this as constant, but the destructor for `x` throws an exception.

It looks like the problem is that we fail to call `IterScope.destroy()` [here](https://github.com/llvm/llvm-project/blob/5f41928689125667cf645835fa9fefee97511d8a/clang/lib/AST/ExprConstant.cpp#L5742), when terminating a loop iteration because the condition evaluated to `false`.
</details>


---

### Comment 4 - frederick-vs-ja

A C++20 example ([Godbolt link](https://godbolt.org/z/MsKbGPj4r)):
```C++
struct X {
  constexpr ~X() { ref = false; }
  constexpr operator bool() {
    return b;
  }
  bool b;
  bool& ref;
};
constexpr bool f() {
  bool ret = true;
  for (bool b = false; X x{b, ret}; b = true) {}
  return ret;
}
static_assert(!f());
```

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-c-20

Author: Richard Smith (zygoloid)

<details>
If a condition variable is declared in the second clause of a three-clause `for` loop, clang doesn't destroy it when constant-evaluating the loop. For example:
```c++
struct X {
  constexpr ~X() noexcept(false) { throw "oops"; }
  constexpr operator bool() {
    return b;
  }
  bool b;
};
constexpr bool f() {
  for (bool b = false; X x = {b}; b = true) {}
  return true;
}
static_assert(f());
```
Clang treats this as constant, but the destructor for `x` throws an exception.

It looks like the problem is that we fail to call `IterScope.destroy()` [here](https://github.com/llvm/llvm-project/blob/5f41928689125667cf645835fa9fefee97511d8a/clang/lib/AST/ExprConstant.cpp#L5742), when terminating a loop iteration because the condition evaluated to `false`.
</details>


---

### Comment 6 - Mr-Anyone

Can I work on this, if that’s okay with you?

---

### Comment 7 - frederick-vs-ja

> Can I work on this, if that’s okay with you?

I think you can just do it.

---

