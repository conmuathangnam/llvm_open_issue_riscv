# clang-trunk fails to compile valid code

**Author:** beached
**URL:** https://github.com/llvm/llvm-project/issues/123498
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, regression:19
**Closed Date:** 2025-01-27T11:43:39Z

## Body

It looks like a recent change has broken the following code and it is instantiating the else branch of an if constexpr 
https://gcc3.godbolt.org/z/YvKjrEKsG

```cpp
#include <cstddef>
#include <functional>
#include <type_traits>

template <typename T>
struct remove_array_ref;

template <typename T, std::size_t N>
struct remove_array_ref<T (&)[N]> {
    using type = T[N];
};
template <typename T>
using remove_array_ref_t = typename remove_array_ref<T>::type;

template <typename T, typename Compare = std::equal_to<>,
          typename U = std::enable_if_t<std::is_array_v<remove_array_ref_t<T>>,
                                        remove_array_ref_t<T>>>
constexpr bool array_cmp(T && lhs, T &&rhs, Compare const &cmp = Compare{}) {
    for (size_t n = 0; n < std::extent_v<U>; ++n) {
        if constexpr (std::rank_v<U> == 1) {
            if (not cmp(lhs[n], rhs[n])) {
                return false;
            }
        } else {
            if (not array_cmp(lhs[n], rhs[n], cmp)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    {
        constexpr int ints1[]{1, 2, 3, 4};
        constexpr int ints2[]{1, 2, 3, 4};
        static_assert(array_cmp(ints1, ints2));
    }
    {
        constexpr int ints3[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        constexpr int ints4[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        static_assert(array_cmp(ints3, ints4));
    }
}
```

```
<source>:25:21: error: no matching function for call to 'array_cmp'
   25 |             if (not array_cmp(lhs[n], rhs[n], cmp)) {
      |                     ^~~~~~~~~
<source>:37:23: note: in instantiation of function template specialization 'array_cmp<const int (&)[4], std::equal_to<void>, const int[4]>' requested here
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^
<source>:18:16: note: candidate template ignored: substitution failure [with T = const int &, Compare = std::equal_to<void>]: implicit instantiation of undefined template 'remove_array_ref<const int &>'
   13 | using remove_array_ref_t = typename remove_array_ref<T>::type;
      | ~~~~~
   14 | 
   15 | template <typename T, typename Compare = std::equal_to<>,
   16 |           typename U = std::enable_if_t<std::is_array_v<remove_array_ref_t<T>>,
   17 |                                         remove_array_ref_t<T>>>
   18 | constexpr bool array_cmp(T && lhs, T &&rhs, Compare const &cmp = Compare{}) {
      |                ^
<source>:37:23: error: static assertion expression is not an integral constant expression
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
<source>:42:23: error: static assertion expression is not an integral constant expression
   42 |         static_assert(array_cmp(ints3, ints4));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
3 errors generated.
Compiler returned: 1
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: beached (beached)

<details>
It looks like a recent change has broken the following code and it is instantiating the else branch of an if constexpr 
https://gcc3.godbolt.org/z/YvKjrEKsG

```cpp
#include &lt;cstddef&gt;
#include &lt;functional&gt;
#include &lt;type_traits&gt;

template &lt;typename T&gt;
struct remove_array_ref;

template &lt;typename T, std::size_t N&gt;
struct remove_array_ref&lt;T (&amp;)[N]&gt; {
    using type = T[N];
};
template &lt;typename T&gt;
using remove_array_ref_t = typename remove_array_ref&lt;T&gt;::type;

template &lt;typename T, typename Compare = std::equal_to&lt;&gt;,
          typename U = std::enable_if_t&lt;std::is_array_v&lt;remove_array_ref_t&lt;T&gt;&gt;,
                                        remove_array_ref_t&lt;T&gt;&gt;&gt;
constexpr bool array_cmp(T &amp;&amp; lhs, T &amp;&amp;rhs, Compare const &amp;cmp = Compare{}) {
    for (size_t n = 0; n &lt; std::extent_v&lt;U&gt;; ++n) {
        if constexpr (std::rank_v&lt;U&gt; == 1) {
            if (not cmp(lhs[n], rhs[n])) {
                return false;
            }
        } else {
            if (not array_cmp(lhs[n], rhs[n], cmp)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    {
        constexpr int ints1[]{1, 2, 3, 4};
        constexpr int ints2[]{1, 2, 3, 4};
        static_assert(array_cmp(ints1, ints2));
    }
    {
        constexpr int ints3[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        constexpr int ints4[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        static_assert(array_cmp(ints3, ints4));
    }
}
```

```
&lt;source&gt;:25:21: error: no matching function for call to 'array_cmp'
   25 |             if (not array_cmp(lhs[n], rhs[n], cmp)) {
      |                     ^~~~~~~~~
&lt;source&gt;:37:23: note: in instantiation of function template specialization 'array_cmp&lt;const int (&amp;)[4], std::equal_to&lt;void&gt;, const int[4]&gt;' requested here
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^
&lt;source&gt;:18:16: note: candidate template ignored: substitution failure [with T = const int &amp;, Compare = std::equal_to&lt;void&gt;]: implicit instantiation of undefined template 'remove_array_ref&lt;const int &amp;&gt;'
   13 | using remove_array_ref_t = typename remove_array_ref&lt;T&gt;::type;
      | ~~~~~
   14 | 
   15 | template &lt;typename T, typename Compare = std::equal_to&lt;&gt;,
   16 |           typename U = std::enable_if_t&lt;std::is_array_v&lt;remove_array_ref_t&lt;T&gt;&gt;,
   17 |                                         remove_array_ref_t&lt;T&gt;&gt;&gt;
   18 | constexpr bool array_cmp(T &amp;&amp; lhs, T &amp;&amp;rhs, Compare const &amp;cmp = Compare{}) {
      |                ^
&lt;source&gt;:37:23: error: static assertion expression is not an integral constant expression
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:42:23: error: static assertion expression is not an integral constant expression
   42 |         static_assert(array_cmp(ints3, ints4));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
3 errors generated.
Compiler returned: 1
```

</details>


---

### Comment 2 - zyn0217

This doesn't reproduce with libstdc++ 14.2 https://gcc3.godbolt.org/z/8rKqheqM8 but only manifests with gcc trunk.

---

### Comment 3 - MagentaTreehouse

libstdc++ started using `__array_rank` since [this commit](https://github.com/gcc-mirror/gcc/commit/6f0dfa6f1acdf78d764d6f5d6f53c2f2a768c047) but clang seems to have some problems with this builtin. See https://gcc3.godbolt.org/z/WjWMozn4e.

libc++ has a workaround: https://github.com/llvm/llvm-project/blob/69d3ba3db922fca8cfc47b5f115b6bea6a737aab/libcxx/include/__type_traits/rank.h#L22-L23

---

### Comment 4 - pinskia

https://github.com/llvm/llvm-project/issues/57133

---

### Comment 5 - pinskia

@jwakely Just an FYI. libstdc++ might need to workaround the clang issue here.

---

### Comment 6 - pinskia

Reduced:
```
template <typename T, int N>
constexpr int array_rank(T (&lhs)[N]) {
    return __array_rank(T[N]);
}
int main() {
    {
        constexpr int ints1[]{1, 2, 3, 4};
        static_assert(array_rank(ints1) == 1);
    }
    {
        constexpr int ints3[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        static_assert(array_rank(ints3) == 2);
    }
}
```

This fails on the clang trunk with:
```
<source>:9:23: error: static assertion expression is not an integral constant expression
    9 |         static_assert(array_rank(ints1) == 1);
      |                       ^~~~~~~~~~~~~~~~~~~~~~
<source>:13:23: error: static assertion expression is not an integral constant expression
   13 |         static_assert(array_rank(ints3) == 2);
      |                       ^~~~~~~~~~~~~~~~~~~~~~
```

With asserts enabled:
```
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:5169: bool EvaluateDependentExpr(const clang::Expr*, {anonymous}::EvalInfo&): Assertion `E->containsErrors() && "valid value-dependent expression should never " "reach invalid code path."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 -O0 -g0 <source>
1.	<source>:9:5: current parser token '}'
2.	<source>:5:12: parsing function body 'main'
3.	<source>:5:12: in compound statement ('{}')
4.	<source>:6:5: in compound statement ('{}')
 #0 0x0000000003cb2408 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb2408)
 #1 0x0000000003cb0114 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb0114)
 #2 0x0000000003bfc298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078073ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078073ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078073ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078073ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078073ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078073ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077e9fff EvaluateDependentExpr(clang::Expr const*, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#10 0x000000000783b759 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#11 0x000000000783a488 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x000000000783faab HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#13 0x0000000007887427 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#14 0x0000000007828268 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x00000000078184f6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#16 0x000000000781feeb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#17 0x0000000007821852 clang::Expr::isCXX11ConstantExpr(clang::ASTContext const&, clang::APValue*, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7821852)
#18 0x0000000007821b8b EvaluateCPlusPlus11IntegralConstantExpr(clang::ASTContext const&, clang::Expr const*, llvm::APSInt*, clang::SourceLocation*) ExprConstant.cpp:0:0
#19 0x0000000007825a43 clang::Expr::getIntegerConstantExpr(clang::ASTContext const&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7825a43)
#20 0x00000000068de38c AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#21 0x00000000068def8d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68def8d)
#22 0x0000000006c7f7dc clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c7f7dc)
#23 0x0000000006aa970e clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa970e)
#24 0x00000000066d832b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d832b)
#25 0x00000000066c6609 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c6609)
#26 0x0000000006774772 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6774772)
#27 0x00000000067754a0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67754a0)
#28 0x0000000006776365 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776365)
#29 0x0000000006776a54 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776a54)
#30 0x0000000006773517 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6773517)
#31 0x00000000067754a0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67754a0)
#32 0x0000000006776365 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776365)
#33 0x0000000006777b6a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6777b6a)
#34 0x00000000066893c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66893c3)
#35 0x00000000066be00d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66be00d)
#36 0x000000000667d12e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d12e)
#37 0x000000000667d8e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d8e9)
#38 0x0000000006685093 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685093)
#39 0x0000000006685f6d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685f6d)
#40 0x00000000066783ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66783ca)
#41 0x000000000463faa8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463faa8)
#42 0x00000000049061e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49061e5)
#43 0x0000000004888b5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4888b5e)
#44 0x00000000049f383e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f383e)
#45 0x0000000000cf4c4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf4c4f)
#46 0x0000000000cec71a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#47 0x0000000004686419 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x0000000003bfc744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bfc744)
#49 0x0000000004686a0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x00000000046497dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46497dd)
#51 0x000000000464a85e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x464a85e)
#52 0x0000000004652365 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4652365)
#53 0x0000000000cf1a83 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf1a83)
#54 0x0000000000bbcc04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbbcc04)
#55 0x000078073ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x000078073ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000cec1c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcec1c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 7 - pinskia

Filed https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118559 to have this worked around in libstdc++ for clang.

---

### Comment 8 - zyn0217

We missed a transform for `__array_rank` where the underlying `ArrayTypeTraitExpr` has its `Dimension` set to null

https://github.com/llvm/llvm-project/blob/4294fe173e9696a4c090857fa0766cd44c1964dc/clang/lib/Sema/TreeTransform.h#L14942-L14955

This results in line 14950 skipping past the expression rebuild during instantiation, hence a dependent expression.

I think the fix should be to convert the `if` on line 14950 to `if (!getDerived().AlwaysRebuild() && E->getDimensionExpression() && SubExpr.get() == E->getDimensionExpression())`


---

### Comment 9 - llvmbot


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

### Comment 10 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: beached (beached)

<details>
It looks like a recent change has broken the following code and it is instantiating the else branch of an if constexpr 
https://gcc3.godbolt.org/z/YvKjrEKsG

```cpp
#include &lt;cstddef&gt;
#include &lt;functional&gt;
#include &lt;type_traits&gt;

template &lt;typename T&gt;
struct remove_array_ref;

template &lt;typename T, std::size_t N&gt;
struct remove_array_ref&lt;T (&amp;)[N]&gt; {
    using type = T[N];
};
template &lt;typename T&gt;
using remove_array_ref_t = typename remove_array_ref&lt;T&gt;::type;

template &lt;typename T, typename Compare = std::equal_to&lt;&gt;,
          typename U = std::enable_if_t&lt;std::is_array_v&lt;remove_array_ref_t&lt;T&gt;&gt;,
                                        remove_array_ref_t&lt;T&gt;&gt;&gt;
constexpr bool array_cmp(T &amp;&amp; lhs, T &amp;&amp;rhs, Compare const &amp;cmp = Compare{}) {
    for (size_t n = 0; n &lt; std::extent_v&lt;U&gt;; ++n) {
        if constexpr (std::rank_v&lt;U&gt; == 1) {
            if (not cmp(lhs[n], rhs[n])) {
                return false;
            }
        } else {
            if (not array_cmp(lhs[n], rhs[n], cmp)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    {
        constexpr int ints1[]{1, 2, 3, 4};
        constexpr int ints2[]{1, 2, 3, 4};
        static_assert(array_cmp(ints1, ints2));
    }
    {
        constexpr int ints3[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        constexpr int ints4[2][4]{{1, 2, 3, 4}, {1, 2, 3, 4}};
        static_assert(array_cmp(ints3, ints4));
    }
}
```

```
&lt;source&gt;:25:21: error: no matching function for call to 'array_cmp'
   25 |             if (not array_cmp(lhs[n], rhs[n], cmp)) {
      |                     ^~~~~~~~~
&lt;source&gt;:37:23: note: in instantiation of function template specialization 'array_cmp&lt;const int (&amp;)[4], std::equal_to&lt;void&gt;, const int[4]&gt;' requested here
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^
&lt;source&gt;:18:16: note: candidate template ignored: substitution failure [with T = const int &amp;, Compare = std::equal_to&lt;void&gt;]: implicit instantiation of undefined template 'remove_array_ref&lt;const int &amp;&gt;'
   13 | using remove_array_ref_t = typename remove_array_ref&lt;T&gt;::type;
      | ~~~~~
   14 | 
   15 | template &lt;typename T, typename Compare = std::equal_to&lt;&gt;,
   16 |           typename U = std::enable_if_t&lt;std::is_array_v&lt;remove_array_ref_t&lt;T&gt;&gt;,
   17 |                                         remove_array_ref_t&lt;T&gt;&gt;&gt;
   18 | constexpr bool array_cmp(T &amp;&amp; lhs, T &amp;&amp;rhs, Compare const &amp;cmp = Compare{}) {
      |                ^
&lt;source&gt;:37:23: error: static assertion expression is not an integral constant expression
   37 |         static_assert(array_cmp(ints1, ints2));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:42:23: error: static assertion expression is not an integral constant expression
   42 |         static_assert(array_cmp(ints3, ints4));
      |                       ^~~~~~~~~~~~~~~~~~~~~~~
3 errors generated.
Compiler returned: 1
```

</details>


---

### Comment 11 - v01dXYZ

Hello,
I'm interested in solving this issue.

---

### Comment 12 - cor3ntin

> Hello,
> I'm interested in solving this issue.

Excellent, thanks! Have fun!

---

### Comment 13 - zyn0217

@v01dXYZ ping. Clang 20 is going to cut on 1/28, and any chance this can make it given that libstdc++ has started relying on the trait?

---

### Comment 14 - zygoloid

> I think the fix should be to convert the `if` on line 14950 to `if (!getDerived().AlwaysRebuild() && E->getDimensionExpression() && SubExpr.get() == E->getDimensionExpression())`

I think instead it should be `if (!getDerived().AlwaysRebuild() && T == E->getQueriedTypeSourceInfo() && SubExpr.get() == E->getDimensionExpression())` -- if `T` changes then we need to rebuild, or the instantiated AST will have the wrong type in it. But we do not reach 14950 in any case where that condition would evaluate to true, due to the prior check on line 14938. So we should just delete the 14950 check and its early exit entirely.

---

### Comment 15 - v01dXYZ

@zyn0217 I'll try to push something within some hours.

Below some facts I remarked:

1. When parsing, ArrayTypeTrait is constructed with a dimension equals to `nullptr`. cf `ParseExprCXX.cpp ParseArrayTypeTrait`.

2. When outputting the TypeSourceInfo, we have the following
```
E.TSI 0x55f9343f5f70
Ty T[N]
TyLoc DependentSizedArrayTypeLoc 'T[N]'  
|-DeclRefExpr 0x55f9343f5f10 'int' NonTypeTemplateParm 0x55f9343f59d8 'N' 'int'
`-TemplateTypeParmTypeLoc 'T' depth 0 index 0
  `-TemplateTypeParm 0x55f9343f5908 'T'

---- versus ----

T 0x55f934415af0
Ty const int[4]
TyLoc ConstantArrayTypeLoc 'const int[4]' 4 
`-SubstTemplateTypeParmTypeLoc 'const int' typename depth 0 index 0 T
  `-Function 0x55f934415878 'array_rank' 'int (const int (&)[4])'
```

3. When using `return __array_rank(...)`, the above assert failure occurs. But if we use a variable and return this variable, clang rejects `static_assert` as the expression is not considered having an integral type.

@zygoloid I think you're right, I'll remove it as returning `E` is wrong since the template type is not the transformed type (b/c the early exit you mentioned).

---

### Comment 16 - jwakely

Which versions of Clang have this bug, is it only 19.x.y?

If I disable the use of `__array_rank` in libstdc++ for `__clang_major__ == 19` is that enough?

---

### Comment 17 - zyn0217

@jwakely  I think it affects all clang versions prior to 20, dating back to when __array_rank was first supported

---

### Comment 18 - jwakely

Yeah, I've just come to the same conclusion testing on compiler explorer.

So `__clang_major__ < 20`

Thanks!

---

