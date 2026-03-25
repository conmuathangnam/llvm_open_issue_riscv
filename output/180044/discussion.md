# [clang] crash on vector_size+constexpr

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/180044
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:20
**Closed Date:** 2026-02-13T15:38:07Z

## Body

`$ cat /tmp/a.cc`

```c++
template <typename T>
constexpr T All(char c) {
    T v;
    for (int i = 0; i < sizeof(T); ++i) v[i] = c;
    return v;
}

using V = char __attribute__((vector_size(16)));
V ones = All<V>(~1);
```

```
$ clang -std=c++20 /tmp/a.cc -fsyntax-only
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source> -isystem/app/abseil/include
1.	<source>:9:20: current parser token ';'
 #0 0x0000000003dacba8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3dacba8)
 #1 0x0000000003daa59c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3daa59c)
 #2 0x0000000003cedee8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070b782c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007582ec9 (anonymous namespace)::LValueExprEvaluator::VisitVarDecl(clang::Expr const*, clang::VarDecl const*) ExprConstant.cpp:0:0
 #5 0x00000000075b5d85 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #6 0x00000000075ba4fb (anonymous namespace)::LValueExprEvaluator::VisitUnaryPreIncDec(clang::UnaryOperator const*) ExprConstant.cpp:0:0
 #7 0x00000000075b53b3 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #8 0x00000000075a1782 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #9 0x00000000075aa1ea EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#10 0x00000000075d3307 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#11 0x00000000075d2ce0 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x00000000075dae6e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#13 0x00000000075fc9a5 (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#14 0x00000000075f303f clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VectorExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x00000000075a18ca Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#16 0x00000000075c74e6 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#17 0x00000000075d86c2 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x75d86c2)
#18 0x0000000007422f76 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7422f76)
#19 0x000000000742339e clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x742339e)
#20 0x000000000670d416 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x670d416)
#21 0x00000000067128d0 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67128d0)
#22 0x000000000635aa36 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x635aa36)
#23 0x000000000636f3ac clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636f3ac)
#24 0x000000000631c3e7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x631c3e7)
#25 0x000000000631cb87 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x631cb87)
#26 0x00000000063253cf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63253cf)
#27 0x0000000006326d07 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6326d07)
#28 0x00000000062ffb6a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ffb6a)
#29 0x0000000004676415 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4676415)
#30 0x000000000498b7ca clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498b7ca)
#31 0x00000000049092cb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49092cb)
#32 0x0000000004a81fab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a81fab)
#33 0x0000000000de995d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde995d)
#34 0x0000000000de178b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000de182d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x00000000046f1769 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003cee303 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cee303)
#38 0x00000000046f1989 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000046b4ad2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b4ad2)
#40 0x00000000046b59b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b59b1)
#41 0x00000000046be4ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46be4ac)
#42 0x0000000000de6259 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde6259)
#43 0x0000000000c99b44 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc99b44)
```

https://gcc.godbolt.org/z/o8an45d9v

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Arthur Eubanks (aeubanks)

<details>
```
$ cat /tmp/a.cc
template &lt;typename T&gt;
constexpr T All(char c) {
    T v;
    for (int i = 0; i &lt; sizeof(T); ++i) v[i] = c;
    return v;
}

using V = char __attribute__((vector_size(16)));
V ones = All&lt;V&gt;(~1);
$ clang -std=c++20 /tmp/a.cc -fsyntax-only
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt; -isystem/app/abseil/include
1.	&lt;source&gt;:9:20: current parser token ';'
 #<!-- -->0 0x0000000003dacba8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3dacba8)
 #<!-- -->1 0x0000000003daa59c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3daa59c)
 #<!-- -->2 0x0000000003cedee8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070b782c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007582ec9 (anonymous namespace)::LValueExprEvaluator::VisitVarDecl(clang::Expr const*, clang::VarDecl const*) ExprConstant.cpp:0:0
 #<!-- -->5 0x00000000075b5d85 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->6 0x00000000075ba4fb (anonymous namespace)::LValueExprEvaluator::VisitUnaryPreIncDec(clang::UnaryOperator const*) ExprConstant.cpp:0:0
 #<!-- -->7 0x00000000075b53b3 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->8 0x00000000075a1782 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->9 0x00000000075aa1ea EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000075d3307 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->11 0x00000000075d2ce0 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000075dae6e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000075fc9a5 (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000075f303f clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VectorExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000075a18ca Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000075c74e6 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#<!-- -->17 0x00000000075d86c2 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x75d86c2)
#<!-- -->18 0x0000000007422f76 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7422f76)
#<!-- -->19 0x000000000742339e clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x742339e)
#<!-- -->20 0x000000000670d416 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x670d416)
#<!-- -->21 0x00000000067128d0 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67128d0)
#<!-- -->22 0x000000000635aa36 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x635aa36)
#<!-- -->23 0x000000000636f3ac clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636f3ac)
#<!-- -->24 0x000000000631c3e7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x631c3e7)
#<!-- -->25 0x000000000631cb87 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x631cb87)
#<!-- -->26 0x00000000063253cf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63253cf)
#<!-- -->27 0x0000000006326d07 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6326d07)
#<!-- -->28 0x00000000062ffb6a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ffb6a)
#<!-- -->29 0x0000000004676415 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4676415)
#<!-- -->30 0x000000000498b7ca clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498b7ca)
#<!-- -->31 0x00000000049092cb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49092cb)
#<!-- -->32 0x0000000004a81fab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a81fab)
#<!-- -->33 0x0000000000de995d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde995d)
#<!-- -->34 0x0000000000de178b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000de182d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x00000000046f1769 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003cee303 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cee303)
#<!-- -->38 0x00000000046f1989 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x00000000046b4ad2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b4ad2)
#<!-- -->40 0x00000000046b59b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b59b1)
#<!-- -->41 0x00000000046be4ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46be4ac)
#<!-- -->42 0x0000000000de6259 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde6259)
#<!-- -->43 0x0000000000c99b44 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc99b44)
```

https://gcc.godbolt.org/z/o8an45d9v
</details>


---

### Comment 2 - shafik

Goes back to clang-20: https://godbolt.org/z/vK51Pr1cq

Note assertions build are usually better b/c they usually indicate the problem better. Including a godbolt link is super helpful as well as checking how fat back it goes.

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/APValue.h:564:
clang::APValue& clang::APValue::getVectorElt(unsigned int):
Assertion `isVector() && "Invalid accessor"' failed.
```

Backtrace:

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -pedantic <source>
1.	<source>:9:20: current parser token ';'
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007764ba042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007764ba0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007764ba042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007764ba0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007764ba02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007764ba039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007a13af7 clang::APValue::getVectorElt(unsigned int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7a13af7)
#10 0x0000000007a3108b handleAssignment((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue const&, clang::QualType, clang::APValue&) ExprConstant.cpp:0:0
#11 0x0000000007a6487d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x0000000007a6523d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#13 0x0000000007a3a7b6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x0000000007a3b164 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x0000000007a5cee7 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#16 0x0000000007a62c51 EvaluateLoopBody((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) ExprConstant.cpp:0:0
#17 0x0000000007a5ddd9 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#18 0x0000000007a5d2a8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#19 0x0000000007a628cb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#20 0x0000000007a70c8d (anonymous namespace)::VectorExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#21 0x0000000007a7872f clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VectorExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#22 0x0000000007a79dad EvaluateVector(clang::Expr const*, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#23 0x0000000007a3a8e6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#24 0x0000000007a570b6 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#25 0x0000000007a9a310 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7a9a310)
#26 0x00000000079046d2 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x79046d2)
#27 0x0000000007904a51 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7904a51)
#28 0x0000000006bd829e clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6bd829e)
#29 0x0000000006bdebd5 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6bdebd5)
#30 0x000000000687589a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x687589a)
#31 0x0000000006884cc9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6884cc9)
#32 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#33 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#34 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#35 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#36 0x000000000683fa5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#37 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#38 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#39 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#40 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#41 0x0000000000cfe40f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#42 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#43 0x000000000483e749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#45 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#47 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#48 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#49 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#50 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#51 0x00007764ba029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x00007764ba029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
```

---

### Comment 3 - a-tarasyuk

Bisect points to https://github.com/llvm/llvm-project/commit/77534291fcbd2c784c54e39a60895e4f60f19742

---

### Comment 4 - shafik

CC @vikramRH

---

