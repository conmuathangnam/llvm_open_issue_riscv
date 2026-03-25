# Clang crash EvaluateStmt since clang 19 with std=c++23 flag

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/166328
**Status:** Closed
**Labels:** duplicate, clang:frontend, regression, crash-on-invalid, confirmed
**Closed Date:** 2025-11-06T12:44:56Z

## Body

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:19120: llvm::APSInt clang::Expr::EvaluateKnownConstInt(const clang::ASTContext&) const: Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 <source>
1.	<eof> parser at end of file
 #0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007cdf41c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007cdf41c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007cdf41c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007cdf41c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007cdf41c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007cdf41c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007bec916 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bec916)
#10 0x0000000007c0aba3 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#11 0x0000000007c0a158 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x0000000007c119eb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#13 0x0000000007c65285 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#14 0x0000000007bf71f5 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x0000000007be66de Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#16 0x0000000007be6fdb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#17 0x0000000007beb02f clang::Expr::isCXX11ConstantExpr(clang::ASTContext const&, clang::APValue*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7beb02f)
#18 0x0000000007beb4f0 EvaluateCPlusPlus11IntegralConstantExpr(clang::ASTContext const&, clang::Expr const*, llvm::APSInt*) ExprConstant.cpp:0:0
#19 0x0000000007bedbbf clang::Expr::getIntegerConstantExpr(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bedbbf)
#20 0x0000000006bbb997 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#21 0x0000000006bbc5d5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bbc5d5)
#22 0x0000000006f5f227 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f5f227)
#23 0x0000000006dad6d1 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dad6d1)
#24 0x000000000699a2a6 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699a2a6)
#25 0x0000000006987aa6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6987aa6)
#26 0x0000000006944607 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6944607)
#27 0x00000000069454d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69454d5)
#28 0x0000000006926b5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926b5a)
#29 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#30 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#31 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#32 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#33 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#34 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dc3ead int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004bedcb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#38 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#40 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#41 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#42 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#43 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#44 0x00007cdf41c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007cdf41c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
```

## Program

```

template<int n>
constexpr int is_power_of_two_fast(int) {
    return n && !(n & (n - 1));
}
constexpr int is_power_of_two_static_branch(int n) {
    switch (int condition = 0; n) {
        case int(is_power_of_two_fast(n)):
            condition = 10;
            break;
    }
}
static_assert(is_power_of_two_static_branch(1) == 10);

```

## To quickly reproduce

https://godbolt.org/z/rhY6aavz8

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:19120: llvm::APSInt clang::Expr::EvaluateKnownConstInt(const clang::ASTContext&amp;) const: Assertion `!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #<!-- -->1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #<!-- -->2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007cdf41c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007cdf41c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007cdf41c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007cdf41c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007cdf41c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007cdf41c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007bec916 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bec916)
#<!-- -->10 0x0000000007c0aba3 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007c0a158 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007c119eb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007c65285 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007bf71f5 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007be66de Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007be6fdb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007beb02f clang::Expr::isCXX11ConstantExpr(clang::ASTContext const&amp;, clang::APValue*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7beb02f)
#<!-- -->18 0x0000000007beb4f0 EvaluateCPlusPlus11IntegralConstantExpr(clang::ASTContext const&amp;, clang::Expr const*, llvm::APSInt*) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007bedbbf clang::Expr::getIntegerConstantExpr(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bedbbf)
#<!-- -->20 0x0000000006bbb997 AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->21 0x0000000006bbc5d5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bbc5d5)
#<!-- -->22 0x0000000006f5f227 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f5f227)
#<!-- -->23 0x0000000006dad6d1 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dad6d1)
#<!-- -->24 0x000000000699a2a6 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699a2a6)
#<!-- -->25 0x0000000006987aa6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6987aa6)
#<!-- -->26 0x0000000006944607 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6944607)
#<!-- -->27 0x00000000069454d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69454d5)
#<!-- -->28 0x0000000006926b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926b5a)
#<!-- -->29 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#<!-- -->30 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#<!-- -->31 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#<!-- -->32 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#<!-- -->33 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#<!-- -->34 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000dc3ead int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004bedcb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#<!-- -->38 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#<!-- -->40 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#<!-- -->41 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#<!-- -->42 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#<!-- -->43 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#<!-- -->44 0x00007cdf41c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007cdf41c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
```

## Program

```

template&lt;int n&gt;
constexpr int is_power_of_two_fast(int) {
    return n &amp;&amp; !(n &amp; (n - 1));
}
constexpr int is_power_of_two_static_branch(int n) {
    switch (int condition = 0; n) {
        case int(is_power_of_two_fast(n)):
            condition = 10;
            break;
    }
}
static_assert(is_power_of_two_static_branch(1) == 10);

```

## To quickly reproduce

https://godbolt.org/z/rhY6aavz8
</details>


---

### Comment 2 - naveen-seth

Duplicate of #165555.

---

