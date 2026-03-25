# clang crash on x86_64-linux-gnu: Assertion `E->isPRValue() && E->getType()->hasPointerRepresentation()' failed.

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/120080

## Body

Compiler Explorer: https://godbolt.org/z/Po5sjedv9
```console
<source>:4:33: error: incompatible integer to pointer conversion passing 'char' to parameter of type 'const void *' [-Wint-conversion]
    4 |   c = __builtin_assume_aligned (c, 16);
      |                                 ^
<source>:4:5: error: incompatible pointer to integer conversion assigning to 'char' from 'void *' [-Wint-conversion]
    4 |   c = __builtin_assume_aligned (c, 16);
      |     ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:9436: bool EvaluatePointer(const clang::Expr*, {anonymous}::LValue&, {anonymous}::EvalInfo&, bool): Assertion `E->isPRValue() && E->getType()->hasPointerRepresentation()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:3: current parser token 'c'
2.	<source>:3:1: parsing function body 'foo'
3.	<source>:3:1: in compound statement ('{}')
 #0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007341f2442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007341f24969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007341f2442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007341f24287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007341f242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007341f2439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077529fa EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#10 0x000000000779d095 (anonymous namespace)::PointerExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#11 0x000000000779e8e2 (anonymous namespace)::PointerExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#12 0x00000000077509f2 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x0000000007752a2b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#14 0x0000000007785360 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#15 0x000000000774c885 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x000000000773bc16 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x000000000773e53f (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) ExprConstant.cpp:0:0
#18 0x00000000077a04b3 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#19 0x000000000774d9c5 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#20 0x000000000773bc16 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#21 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#22 0x0000000007745c1c clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7745c1c)
#23 0x0000000006803644 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6803644)
#24 0x00000000068337ad clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68337ad)
#25 0x0000000006bcda3c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bcda3c)
#26 0x0000000006ede90e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ede90e)
#27 0x00000000066cf2b4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66cf2b4)
#28 0x00000000066c55ae clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c55ae)
#29 0x00000000066c6500 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c6500)
#30 0x00000000066c73c5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c73c5)
#31 0x00000000066c8bca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c8bca)
#32 0x00000000065db123 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65db123)
#33 0x000000000660fadd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660fadd)
#34 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#35 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#36 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#37 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#38 0x00000000065d8170 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d8170)
#39 0x00000000065ca1d3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca1d3)
#40 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#41 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#42 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#43 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#44 0x0000000000ce1c7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#45 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x0000000004624ad9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#48 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#50 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#51 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#52 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#53 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#54 0x00007341f2429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007341f2429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
void
foo (char c)
{
  c = __builtin_assume_aligned (c, 16);
  c[0] = -1;
  c[1] = -1;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/Po5sjedv9
```console
&lt;source&gt;:4:33: error: incompatible integer to pointer conversion passing 'char' to parameter of type 'const void *' [-Wint-conversion]
    4 |   c = __builtin_assume_aligned (c, 16);
      |                                 ^
&lt;source&gt;:4:5: error: incompatible pointer to integer conversion assigning to 'char' from 'void *' [-Wint-conversion]
    4 |   c = __builtin_assume_aligned (c, 16);
      |     ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:9436: bool EvaluatePointer(const clang::Expr*, {anonymous}::LValue&amp;, {anonymous}::EvalInfo&amp;, bool): Assertion `E-&gt;isPRValue() &amp;&amp; E-&gt;getType()-&gt;hasPointerRepresentation()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:3: current parser token 'c'
2.	&lt;source&gt;:3:1: parsing function body 'foo'
3.	&lt;source&gt;:3:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #<!-- -->1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #<!-- -->2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007341f2442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007341f24969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007341f2442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007341f24287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007341f242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007341f2439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000077529fa EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->10 0x000000000779d095 (anonymous namespace)::PointerExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000779e8e2 (anonymous namespace)::PointerExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000077509f2 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007752a2b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007785360 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x000000000774c885 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->16 0x000000000773bc16 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x000000000773e53f (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) ExprConstant.cpp:0:0
#<!-- -->18 0x00000000077a04b3 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->19 0x000000000774d9c5 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->20 0x000000000773bc16 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->21 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->22 0x0000000007745c1c clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7745c1c)
#<!-- -->23 0x0000000006803644 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6803644)
#<!-- -->24 0x00000000068337ad clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68337ad)
#<!-- -->25 0x0000000006bcda3c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bcda3c)
#<!-- -->26 0x0000000006ede90e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ede90e)
#<!-- -->27 0x00000000066cf2b4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66cf2b4)
#<!-- -->28 0x00000000066c55ae clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c55ae)
#<!-- -->29 0x00000000066c6500 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c6500)
#<!-- -->30 0x00000000066c73c5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c73c5)
#<!-- -->31 0x00000000066c8bca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c8bca)
#<!-- -->32 0x00000000065db123 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65db123)
#<!-- -->33 0x000000000660fadd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660fadd)
#<!-- -->34 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#<!-- -->35 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#<!-- -->36 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#<!-- -->37 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#<!-- -->38 0x00000000065d8170 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d8170)
#<!-- -->39 0x00000000065ca1d3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca1d3)
#<!-- -->40 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#<!-- -->41 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#<!-- -->42 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#<!-- -->43 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#<!-- -->44 0x0000000000ce1c7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#<!-- -->45 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x0000000004624ad9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#<!-- -->48 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#<!-- -->50 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#<!-- -->51 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#<!-- -->52 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#<!-- -->53 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#<!-- -->54 0x00007341f2429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x00007341f2429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
void
foo (char c)
{
  c = __builtin_assume_aligned (c, 16);
  c[0] = -1;
  c[1] = -1;
}
```
</details>


---

