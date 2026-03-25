# [clang] Assertion `CAT && "vla in literal type?"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/151716
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash, regression:18
**Closed Date:** 2025-09-04T14:28:43Z

## Body

Reproducer:
https://godbolt.org/z/89EbxxGfT
```cpp
constexpr int f() {
  int(&&g)[]{0};
  return g[0];
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:4032: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&, const clang::Expr*, const {anonymous}::CompleteObject&, const {anonymous}::SubobjectDesignator&, SubobjectHandler&) [with SubobjectHandler = {anonymous}::ExtractSubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `CAT && "vla in literal type?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:1:19: parsing function body 'f'
 #0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000763a86042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000763a860969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000763a86042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000763a860287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000763a8602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000763a86039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000078c66bf (anonymous namespace)::ExtractSubobjectHandler::result_type findSubobject<(anonymous namespace)::ExtractSubobjectHandler>((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::CompleteObject const&, (anonymous namespace)::SubobjectDesignator const&, (anonymous namespace)::ExtractSubobjectHandler&) ExprConstant.cpp:0:0
#10 0x00000000078c7198 handleLValueToRValueConversion((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::QualType, (anonymous namespace)::LValue const&, clang::APValue&, bool) ExprConstant.cpp:0:0
#11 0x000000000791b91f (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#12 0x00000000078dd09b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x00000000078ce2b6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x00000000078ef8db EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#15 0x00000000078ee7e8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#16 0x00000000078f516b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#17 0x000000000793ef04 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793ef04)
#18 0x0000000006ad2756 clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad2756)
#19 0x0000000006a5b003 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5b003)
#20 0x00000000067a098f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a098f)
#21 0x0000000006698063 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6698063)
#22 0x00000000066cde4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cde4d)
#23 0x000000000668ae3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668ae3e)
#24 0x000000000668b5cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b5cf)
#25 0x00000000066932da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66932da)
#26 0x0000000006694265 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694265)
#27 0x0000000006694720 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694720)
#28 0x0000000006685fa3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fa3)
#29 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#30 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#31 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#32 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#33 0x0000000000daa41f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#34 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x00000000049eeaa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#37 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#39 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#40 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#41 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#42 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#43 0x0000763a86029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x0000763a86029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/89EbxxGfT
```cpp
constexpr int f() {
  int(&amp;&amp;g)[]{0};
  return g[0];
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:4032: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&amp;, const clang::Expr*, const {anonymous}::CompleteObject&amp;, const {anonymous}::SubobjectDesignator&amp;, SubobjectHandler&amp;) [with SubobjectHandler = {anonymous}::ExtractSubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `CAT &amp;&amp; "vla in literal type?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:19: parsing function body 'f'
 #<!-- -->0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #<!-- -->1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #<!-- -->2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000763a86042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000763a860969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000763a86042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000763a860287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000763a8602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000763a86039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000078c66bf (anonymous namespace)::ExtractSubobjectHandler::result_type findSubobject&lt;(anonymous namespace)::ExtractSubobjectHandler&gt;((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::CompleteObject const&amp;, (anonymous namespace)::SubobjectDesignator const&amp;, (anonymous namespace)::ExtractSubobjectHandler&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000078c7198 handleLValueToRValueConversion((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::QualType, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000791b91f (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078dd09b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078ce2b6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000078ef8db EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000078ee7e8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000078f516b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->17 0x000000000793ef04 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793ef04)
#<!-- -->18 0x0000000006ad2756 clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad2756)
#<!-- -->19 0x0000000006a5b003 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5b003)
#<!-- -->20 0x00000000067a098f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a098f)
#<!-- -->21 0x0000000006698063 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6698063)
#<!-- -->22 0x00000000066cde4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cde4d)
#<!-- -->23 0x000000000668ae3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668ae3e)
#<!-- -->24 0x000000000668b5cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b5cf)
#<!-- -->25 0x00000000066932da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66932da)
#<!-- -->26 0x0000000006694265 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694265)
#<!-- -->27 0x0000000006694720 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694720)
#<!-- -->28 0x0000000006685fa3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fa3)
#<!-- -->29 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#<!-- -->30 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#<!-- -->31 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#<!-- -->32 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#<!-- -->33 0x0000000000daa41f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#<!-- -->34 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x00000000049eeaa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#<!-- -->37 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#<!-- -->39 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#<!-- -->40 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#<!-- -->41 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#<!-- -->42 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#<!-- -->43 0x0000763a86029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000763a86029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/89EbxxGfT
```cpp
constexpr int f() {
  int(&amp;&amp;g)[]{0};
  return g[0];
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:4032: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&amp;, const clang::Expr*, const {anonymous}::CompleteObject&amp;, const {anonymous}::SubobjectDesignator&amp;, SubobjectHandler&amp;) [with SubobjectHandler = {anonymous}::ExtractSubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `CAT &amp;&amp; "vla in literal type?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:19: parsing function body 'f'
 #<!-- -->0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #<!-- -->1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #<!-- -->2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000763a86042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000763a860969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000763a86042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000763a860287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000763a8602871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000763a86039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000078c66bf (anonymous namespace)::ExtractSubobjectHandler::result_type findSubobject&lt;(anonymous namespace)::ExtractSubobjectHandler&gt;((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::CompleteObject const&amp;, (anonymous namespace)::SubobjectDesignator const&amp;, (anonymous namespace)::ExtractSubobjectHandler&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000078c7198 handleLValueToRValueConversion((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::QualType, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000791b91f (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078dd09b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078ce2b6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000078ef8db EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000078ee7e8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000078f516b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->17 0x000000000793ef04 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793ef04)
#<!-- -->18 0x0000000006ad2756 clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad2756)
#<!-- -->19 0x0000000006a5b003 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5b003)
#<!-- -->20 0x00000000067a098f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a098f)
#<!-- -->21 0x0000000006698063 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6698063)
#<!-- -->22 0x00000000066cde4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cde4d)
#<!-- -->23 0x000000000668ae3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668ae3e)
#<!-- -->24 0x000000000668b5cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b5cf)
#<!-- -->25 0x00000000066932da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66932da)
#<!-- -->26 0x0000000006694265 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694265)
#<!-- -->27 0x0000000006694720 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694720)
#<!-- -->28 0x0000000006685fa3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fa3)
#<!-- -->29 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#<!-- -->30 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#<!-- -->31 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#<!-- -->32 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#<!-- -->33 0x0000000000daa41f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#<!-- -->34 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x00000000049eeaa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#<!-- -->37 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#<!-- -->39 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#<!-- -->40 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#<!-- -->41 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#<!-- -->42 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#<!-- -->43 0x0000763a86029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000763a86029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - k-arrows

Goes back to clang-18.1.0:
https://godbolt.org/z/ocPMex8Ez

---

### Comment 4 - shafik

@HerrCai0907 git bisect link this to d89d3a6a0eb3e5f9ad35cd8b64a7237ed227f10b

---

