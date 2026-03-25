# "Assertion `!isa<ConstantExpr>(E)' failed." with compound literal

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/139160
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash
**Closed Date:** 2025-05-15T16:23:46Z

## Body

Testcase:

```
struct A {int x[1]; };
A f();
typedef int *t[];
consteval int* f(int* x) { return x; }
int ** x = (t){f(f().x)};
```

Crashes with:

```
clang: clang/lib/AST/Expr.cpp:339: static clang::ConstantExpr *clang::ConstantExpr::Create(const clang::ASTContext &, clang::Expr *, clang::ConstantResultStorageKind, bool): Assertion `!isa<ConstantExpr>(E)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -x c++ - -o - -S -std=c++20
1.      <stdin>:5:25: current parser token ';'
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eli Friedman (efriedma-quic)

<details>
Testcase:

```
struct A {int x[1]; };
A f();
typedef int *t[];
consteval int* f(int* x) { return x; }
int ** x = (t){f(f().x)};
```

Crashes with:

```
clang: clang/lib/AST/Expr.cpp:339: static clang::ConstantExpr *clang::ConstantExpr::Create(const clang::ASTContext &amp;, clang::Expr *, clang::ConstantResultStorageKind, bool): Assertion `!isa&lt;ConstantExpr&gt;(E)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -x c++ - -o - -S -std=c++20
1.      &lt;stdin&gt;:5:25: current parser token ';'
```
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

Author: Eli Friedman (efriedma-quic)

<details>
Testcase:

```
struct A {int x[1]; };
A f();
typedef int *t[];
consteval int* f(int* x) { return x; }
int ** x = (t){f(f().x)};
```

Crashes with:

```
clang: clang/lib/AST/Expr.cpp:339: static clang::ConstantExpr *clang::ConstantExpr::Create(const clang::ASTContext &amp;, clang::Expr *, clang::ConstantResultStorageKind, bool): Assertion `!isa&lt;ConstantExpr&gt;(E)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -x c++ - -o - -S -std=c++20
1.      &lt;stdin&gt;:5:25: current parser token ';'
```
</details>


---

### Comment 4 - Mr-Anyone

Hey, I'd love to work on this if that's okay. Thanks!

---

### Comment 5 - AaronBallman

> Hey, I'd love to work on this if that's okay. Thanks!

I've assigned to you, thank you for looking into it!

---

