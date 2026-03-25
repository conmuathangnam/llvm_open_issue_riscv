# [clang] Assertion `CallRange.isValid()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/185241

## Body

Reproducer:
https://godbolt.org/z/6Y1P414Me
```cpp
struct S {
  constexpr int func() {}
};

static_assert([]() {
  S s;
  if (__builtin_invoke(&S::func, s))
    return false;
  return true;
}());
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/State.cpp:128: void clang::interp::State::addCallStack(unsigned int): Assertion `CallRange.isValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<eof> parser at end of file
 #0 0x0000000004435848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4435848)
 #1 0x0000000004432c94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4432c94)
 #2 0x0000000004372a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078afade42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078afade969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078afade42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078afade287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078afade2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078afade39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000821a0b9 clang::interp::State::addCallStack(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a0b9)
#10 0x000000000821b2dc clang::interp::State::diag(clang::SourceLocation, unsigned int, unsigned int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821b2dc)
#11 0x0000000007f7d31e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#12 0x0000000007fe5cc0 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#13 0x0000000007f5788d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x0000000007f7f6a6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x0000000007f8a5f8 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#16 0x0000000007fd8e53 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#17 0x0000000007f57ac2 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x0000000007f7f6a6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#19 0x0000000007f8a5f8 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#20 0x0000000007f7608e EvaluateCond((anonymous namespace)::EvalInfo&, clang::VarDecl const*, clang::Expr const*, bool&) ExprConstant.cpp:0:0
#21 0x0000000007f782c6 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#22 0x0000000007f77838 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#23 0x0000000007f7d0fb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#24 0x0000000007fe5cc0 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#25 0x0000000007f5789d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#26 0x0000000007f590e6 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#27 0x0000000007f7f6a6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#28 0x0000000007f8010b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#29 0x0000000007f870ee clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f870ee)
#30 0x0000000007181f81 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7181f81)
#31 0x0000000007182ac2 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7182ac2)
#32 0x0000000007091d46 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7091d46)
#33 0x0000000006c73e16 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c73e16)
#34 0x0000000006c61976 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c61976)
#35 0x0000000006c1e1a6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e1a6)
#36 0x0000000006c1f0b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1f0b5)
#37 0x0000000006c0077a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0077a)
#38 0x0000000004f09ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f09ef8)
#39 0x0000000005212375 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5212375)
#40 0x0000000005190d0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5190d0e)
#41 0x000000000530ecdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x530ecdd)
#42 0x0000000000df2364 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf2364)
#43 0x0000000000de8cda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#44 0x0000000000de8e5d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#45 0x0000000004f7f019 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x0000000004372e84 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4372e84)
#47 0x0000000004f7f62f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004f3f2e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3f2e2)
#49 0x0000000004f4028e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4028e)
#50 0x0000000004f47fd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f47fd5)
#51 0x0000000000dee6ea clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdee6ea)
#52 0x0000000000ca5e6a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca5e6a)
#53 0x000078afade29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x000078afade29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000de8775 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8775)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6Y1P414Me
```cpp
struct S {
  constexpr int func() {}
};

static_assert([]() {
  S s;
  if (__builtin_invoke(&amp;S::func, s))
    return false;
  return true;
}());
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/State.cpp:128: void clang::interp::State::addCallStack(unsigned int): Assertion `CallRange.isValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004435848 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4435848)
 #<!-- -->1 0x0000000004432c94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4432c94)
 #<!-- -->2 0x0000000004372a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078afade42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078afade969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078afade42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078afade287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078afade2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000078afade39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000821a0b9 clang::interp::State::addCallStack(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a0b9)
#<!-- -->10 0x000000000821b2dc clang::interp::State::diag(clang::SourceLocation, unsigned int, unsigned int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821b2dc)
#<!-- -->11 0x0000000007f7d31e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007fe5cc0 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007f5788d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007f7f6a6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007f8a5f8 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007fd8e53 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007f57ac2 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000007f7f6a6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007f8a5f8 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007f7608e EvaluateCond((anonymous namespace)::EvalInfo&amp;, clang::VarDecl const*, clang::Expr const*, bool&amp;) ExprConstant.cpp:0:0
#<!-- -->21 0x0000000007f782c6 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->22 0x0000000007f77838 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->23 0x0000000007f7d0fb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->24 0x0000000007fe5cc0 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->25 0x0000000007f5789d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->26 0x0000000007f590e6 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->27 0x0000000007f7f6a6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->28 0x0000000007f8010b EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->29 0x0000000007f870ee clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f870ee)
#<!-- -->30 0x0000000007181f81 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7181f81)
#<!-- -->31 0x0000000007182ac2 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7182ac2)
#<!-- -->32 0x0000000007091d46 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7091d46)
#<!-- -->33 0x0000000006c73e16 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c73e16)
#<!-- -->34 0x0000000006c61976 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c61976)
#<!-- -->35 0x0000000006c1e1a6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e1a6)
#<!-- -->36 0x0000000006c1f0b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1f0b5)
#<!-- -->37 0x0000000006c0077a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0077a)
#<!-- -->38 0x0000000004f09ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f09ef8)
#<!-- -->39 0x0000000005212375 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5212375)
#<!-- -->40 0x0000000005190d0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5190d0e)
#<!-- -->41 0x000000000530ecdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x530ecdd)
#<!-- -->42 0x0000000000df2364 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf2364)
#<!-- -->43 0x0000000000de8cda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->44 0x0000000000de8e5d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->45 0x0000000004f7f019 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x0000000004372e84 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4372e84)
#<!-- -->47 0x0000000004f7f62f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x0000000004f3f2e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3f2e2)
#<!-- -->49 0x0000000004f4028e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4028e)
#<!-- -->50 0x0000000004f47fd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f47fd5)
#<!-- -->51 0x0000000000dee6ea clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdee6ea)
#<!-- -->52 0x0000000000ca5e6a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca5e6a)
#<!-- -->53 0x000078afade29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x000078afade29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000de8775 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8775)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Note: `-fexperimental-new-constant-interpreter` is not specified here

---

### Comment 3 - k-arrows

Goes back to clang-22:
https://godbolt.org/z/jbobfGd56

---

### Comment 4 - shafik

I am assuming, like your other reports this is fuzzer generated?

---

### Comment 5 - k-arrows

> I am assuming, like your other reports this is fuzzer generated?

Yes. This was probably fuzzed from the following program:
https://github.com/llvm/llvm-project/blob/main/clang/test/SemaCXX/builtin-invoke.cpp

---

