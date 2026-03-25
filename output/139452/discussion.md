# Clang crash after "Implement P2280R4 Using unknown pointers and references in constant expressions"

**Author:** thevar1able
**URL:** https://github.com/llvm/llvm-project/issues/139452
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash-on-valid, regression:20
**Closed Date:** 2025-05-15T23:04:39Z

## Body

```
 #0 0x00005fa8dbe95219 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Unix/Signals.inc:800:3
 #1 0x00005fa8dbe92d34 llvm::sys::RunSignalHandlers() /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #2 0x00005fa8dbdf1a08 HandleCrash /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00005fa8dbdf1a08 CrashRecoverySignalHandler(int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:62
 #4 0x00007aa479728ef0 (/usr/lib/libc.so.6+0x3def0)
 #5 0x00005fa8df2e63da HandleLValueBase((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue&, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3308:3
 #6 0x00005fa8df2e6851 HandleLValueBasePath((anonymous namespace)::EvalInfo&, clang::CastExpr const*, clang::QualType, (anonymous namespace)::LValue&) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3320:5
 #7 0x00005fa8df3001f1 (anonymous namespace)::LValueExprEvaluatorBase<(anonymous namespace)::LValueExprEvaluator>::VisitMemberExpr(clang::MemberExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8654:27
 #8 0x00005fa8df34f5ff EvaluateLValue /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8872:64
 #9 0x00005fa8df34f5ff VisitCastExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8499:26
#10 0x00005fa8df34f5ff (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:15045:46
#11 0x00005fa8df3080e0 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#12 0x00005fa8df31be28 EvaluateExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14010:21
#13 0x00005fa8df31be28 process /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14192:19
#14 0x00005fa8df31be28 Traverse /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:13976:14
#15 0x00005fa8df31be28 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14643:66
#16 0x00005fa8df3080e0 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#17 0x00005fa8df309565 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:2763:3
#18 0x00005fa8df32596f EvaluateCond /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5247:3
#19 0x00005fa8df32596f EvaluateCond((anonymous namespace)::EvalInfo&, clang::VarDecl const*, clang::Expr const*, bool&) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5240:13
#20 0x00005fa8df32348c EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5620:12
#21 0x00005fa8df322ae8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5586:40
#22 0x00005fa8df32a3be HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) (.isra.0) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:6600:36
#23 0x00005fa8df333df2 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::RecordExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8390:78
```

Full log: https://pastila.nl/?000c2db4/648c000755344e7c43e24c25dd06a571#dyq1EG+tE1OVC+RrCFJoNw==
Artifacts: [gtest_coordination_storage.tar.gz](https://github.com/user-attachments/files/20149772/gtest_coordination_storage.tar.gz)

[Our change](https://github.com/ClickHouse/ClickHouse/commit/c844c4b613795e036650b153825b6a25f9efd352#diff-a7cac25a87b64c38dc7740f952bae5c08602866a7128cada1750de08504b7506) to get it building again.


Bisecting pointed to [this PR](https://github.com/llvm/llvm-project/pull/95474).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Konstantin Bogdanov (thevar1able)

<details>
```
 #<!-- -->0 0x00005fa8dbe95219 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Unix/Signals.inc:800:3
 #<!-- -->1 0x00005fa8dbe92d34 llvm::sys::RunSignalHandlers() /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x00005fa8dbdf1a08 HandleCrash /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00005fa8dbdf1a08 CrashRecoverySignalHandler(int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->4 0x00007aa479728ef0 (/usr/lib/libc.so.6+0x3def0)
 #<!-- -->5 0x00005fa8df2e63da HandleLValueBase((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue&amp;, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3308:3
 #<!-- -->6 0x00005fa8df2e6851 HandleLValueBasePath((anonymous namespace)::EvalInfo&amp;, clang::CastExpr const*, clang::QualType, (anonymous namespace)::LValue&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3320:5
 #<!-- -->7 0x00005fa8df3001f1 (anonymous namespace)::LValueExprEvaluatorBase&lt;(anonymous namespace)::LValueExprEvaluator&gt;::VisitMemberExpr(clang::MemberExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8654:27
 #<!-- -->8 0x00005fa8df34f5ff EvaluateLValue /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8872:64
 #<!-- -->9 0x00005fa8df34f5ff VisitCastExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8499:26
#<!-- -->10 0x00005fa8df34f5ff (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:15045:46
#<!-- -->11 0x00005fa8df3080e0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#<!-- -->12 0x00005fa8df31be28 EvaluateExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14010:21
#<!-- -->13 0x00005fa8df31be28 process /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14192:19
#<!-- -->14 0x00005fa8df31be28 Traverse /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:13976:14
#<!-- -->15 0x00005fa8df31be28 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14643:66
#<!-- -->16 0x00005fa8df3080e0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#<!-- -->17 0x00005fa8df309565 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:2763:3
#<!-- -->18 0x00005fa8df32596f EvaluateCond /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5247:3
#<!-- -->19 0x00005fa8df32596f EvaluateCond((anonymous namespace)::EvalInfo&amp;, clang::VarDecl const*, clang::Expr const*, bool&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5240:13
#<!-- -->20 0x00005fa8df32348c EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5620:12
#<!-- -->21 0x00005fa8df322ae8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5586:40
#<!-- -->22 0x00005fa8df32a3be HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:6600:36
#<!-- -->23 0x00005fa8df333df2 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8390:78
```

Full log: https://pastila.nl/?000c2db4/648c000755344e7c43e24c25dd06a571#dyq1EG+tE1OVC+RrCFJoNw==
Artifacts: [gtest_coordination_storage.tar.gz](https://github.com/user-attachments/files/20149772/gtest_coordination_storage.tar.gz)

[Our change](https://github.com/ClickHouse/ClickHouse/commit/c844c4b613795e036650b153825b6a25f9efd352#diff-a7cac25a87b64c38dc7740f952bae5c08602866a7128cada1750de08504b7506) to get it building again.


Bisecting pointed to [this PR](https://github.com/llvm/llvm-project/pull/95474).
</details>


---

### Comment 2 - frederick-vs-ja

There're some following-up fixes. Have you tested this with trunk or the latest release of Clang 20?

---

### Comment 3 - thevar1able

@frederick-vs-ja Yes, this reproduces on 20.1.4.
https://pastila.nl/?000db417/984f30d20ea00fbbd49e74d94262f00e#RvQ6rooKozBvzidpY48Rmw==

---

### Comment 4 - thevar1able

Adding a nullptr check fixes the issue for me https://github.com/llvm/llvm-project/commit/4f9d3674e2706eb28cb5e27d5053fee1afef0413

---

### Comment 5 - thevar1able

@cor3ntin Reduction: https://godbolt.org/z/e8xhjYdd9

---

### Comment 6 - frederick-vs-ja

A library-free reduced example ([Godbolt link](https://godbolt.org/z/Po8oW5j6e)):
```C++
template<class T>
struct Wrapper {
    T value{};
    constexpr explicit operator bool() const noexcept { return true; }
};

struct Base {
    virtual ~Base() = default;
    int error = 0;
};
struct Derived : virtual Base {
    int data{};
};

int main() {
    auto get_wrapper = []() -> Wrapper<int> {
        const auto& derived_val = Derived{};
        if (derived_val.error != 0)
           return Wrapper<int>{};
        return Wrapper<int>{derived_val.data};
    };

    (void)(!get_wrapper());
}
```

----
Edit:

A lambda-free reduced example ([Godbolt link](https://godbolt.org/z/E4b6Erb4Y)):
```C++
struct Dummy {
    explicit operator bool() const noexcept { return true; }
};

struct Base { int error; }; // a data member in a virtual base class seems related
struct Derived : virtual Base { };

template <class R>
constexpr // constexpr (or implicit constexpr in lambda) is critical
R get_value() {
    const auto& derived_val = Derived{};
    if (derived_val.error != 0)
        /* nothing */;
    return R{};
}

int main() {
    return !get_value<Dummy>(); // contextually convert the function call result to bool
}
```

---

### Comment 7 - cor3ntin

@shafik 

---

### Comment 8 - shafik

I think what is happening here is that we are not expecting to get a Reference Type in `HandleLValueBase` and so `getAsCXXRecordDecl()` fails but debugging I see `expr D.MostDerivedType->getPointeeType()->isRecordType()` is valid.

So the question is, is doing a null check the right thing or should `MostDerivedType` actually be the pointee type?

CC @cor3ntin 

---

### Comment 9 - shafik

So in `VisitCastExpr` w/ unknown values we now pass the `if (!this->Visit(E->getSubExpr()))` check. 

---

### Comment 10 - cor3ntin

> So the question is, is doing a null check the right thing, or should `MostDerivedType` actually be the pointee type?

I look at it, and yes, I think the correct fix is
`DerivedDecl = D.MostDerivedType.getNonReferenceType()->getAsCXXRecordDecl();`

Here
https://github.com/llvm/llvm-project/blob/ab119add3573c834185810a15a8a3648b1819959/clang/lib/AST/ExprConstant.cpp#L3317

---

### Comment 11 - llvmbot


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

### Comment 12 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Konstantin Bogdanov (thevar1able)

<details>
```
 #<!-- -->0 0x00005fa8dbe95219 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Unix/Signals.inc:800:3
 #<!-- -->1 0x00005fa8dbe92d34 llvm::sys::RunSignalHandlers() /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x00005fa8dbdf1a08 HandleCrash /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00005fa8dbdf1a08 CrashRecoverySignalHandler(int) /home/thevar1able/nvmemount/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->4 0x00007aa479728ef0 (/usr/lib/libc.so.6+0x3def0)
 #<!-- -->5 0x00005fa8df2e63da HandleLValueBase((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue&amp;, clang::CXXRecordDecl const*, clang::CXXBaseSpecifier const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3308:3
 #<!-- -->6 0x00005fa8df2e6851 HandleLValueBasePath((anonymous namespace)::EvalInfo&amp;, clang::CastExpr const*, clang::QualType, (anonymous namespace)::LValue&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:3320:5
 #<!-- -->7 0x00005fa8df3001f1 (anonymous namespace)::LValueExprEvaluatorBase&lt;(anonymous namespace)::LValueExprEvaluator&gt;::VisitMemberExpr(clang::MemberExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8654:27
 #<!-- -->8 0x00005fa8df34f5ff EvaluateLValue /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8872:64
 #<!-- -->9 0x00005fa8df34f5ff VisitCastExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8499:26
#<!-- -->10 0x00005fa8df34f5ff (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:15045:46
#<!-- -->11 0x00005fa8df3080e0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#<!-- -->12 0x00005fa8df31be28 EvaluateExpr /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14010:21
#<!-- -->13 0x00005fa8df31be28 process /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14192:19
#<!-- -->14 0x00005fa8df31be28 Traverse /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:13976:14
#<!-- -->15 0x00005fa8df31be28 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:14643:66
#<!-- -->16 0x00005fa8df3080e0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:16517:46
#<!-- -->17 0x00005fa8df309565 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:2763:3
#<!-- -->18 0x00005fa8df32596f EvaluateCond /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5247:3
#<!-- -->19 0x00005fa8df32596f EvaluateCond((anonymous namespace)::EvalInfo&amp;, clang::VarDecl const*, clang::Expr const*, bool&amp;) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5240:13
#<!-- -->20 0x00005fa8df32348c EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5620:12
#<!-- -->21 0x00005fa8df322ae8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:5586:40
#<!-- -->22 0x00005fa8df32a3be HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:6600:36
#<!-- -->23 0x00005fa8df333df2 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/thevar1able/nvmemount/llvm-project/clang/lib/AST/ExprConstant.cpp:8390:78
```

Full log: https://pastila.nl/?000c2db4/648c000755344e7c43e24c25dd06a571#dyq1EG+tE1OVC+RrCFJoNw==
Artifacts: [gtest_coordination_storage.tar.gz](https://github.com/user-attachments/files/20149772/gtest_coordination_storage.tar.gz)

[Our change](https://github.com/ClickHouse/ClickHouse/commit/c844c4b613795e036650b153825b6a25f9efd352#diff-a7cac25a87b64c38dc7740f952bae5c08602866a7128cada1750de08504b7506) to get it building again.


Bisecting pointed to [this PR](https://github.com/llvm/llvm-project/pull/95474).
</details>


---

### Comment 13 - shafik

The only concern I have with fixing this locally is that we do `MostDerivedType->getAsCXXRecordDecl()` in several other locations. 

So it could be that applying the fix in `set(APValue::LValueBase B, bool BInvalid = false)` makes more sense but it is not obvious that is the case. 

It may make sense to do a local fix and leave a `FIXME` in case we see this kind of crash come up via a different path and then we should consider a fix at a different point.

---

