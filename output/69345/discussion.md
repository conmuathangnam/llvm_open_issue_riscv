# clang: 18: Assertion `isStruct() && "Invalid accessor"' failed.

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/69345
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-11-30T14:29:57Z

## Body

Compiler Explorer: https://godbolt.org/z/Knxhh666z

When compile this program with clang (version 11d07d9ef618497b825badee8b4f06a48575606b), clang crashes:
```
struct T {
  int b;
  int temp1;
} a;

void f() {
  a.b = ({ a.temp2; });
}
```

Interestingly, altering the names of `temp1` and `temp2` can prevent the crash. For instance, the below program does not trigger the crash:
```
struct T {
  int b;
  int c;
} a;

void f() {
  a.b = ({ a.d; });
}
```

The stack dump:
```
<source>:7:14: error: no member named 'temp2' in 'struct T'; did you mean 'temp1'?
    7 |   a.b = ({ a.temp2; });
      |              ^~~~~
      |              temp1
<source>:3:7: note: 'temp1' declared here
    3 |   int temp1;
      |       ^
clang: /root/llvm-project/clang/include/clang/AST/APValue.h:552: clang::APValue& clang::APValue::getStructField(unsigned int): Assertion `isStruct() && "Invalid accessor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O0 <source>
1.	<source>:8:1: current parser token '}'
2.	<source>:6:10: parsing function body 'f'
3.	<source>:6:10: in compound statement ('{}')
 #0 0x000000000372dc98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372dc98)
 #1 0x000000000372b95c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372b95c)
 #2 0x0000000003674298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fda0f9cb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fda0f48e00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fda0f46d859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fda0f46d729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fda0f47efd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000006f27ec1 clang::APValue::getStructField(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f27ec1)
 #9 0x0000000006fcfa37 (anonymous namespace)::ExtractSubobjectHandler::result_type findSubobject<(anonymous namespace)::ExtractSubobjectHandler>((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::CompleteObject const&, (anonymous namespace)::SubobjectDesignator const&, (anonymous namespace)::ExtractSubobjectHandler&) ExprConstant.cpp:0:0
#10 0x0000000006fd907a (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitMemberExpr(clang::MemberExpr const*) ExprConstant.cpp:0:0
#11 0x0000000006fe6fc4 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x0000000006fe72f8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x0000000006fd4f8e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x0000000006fd792e (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) ExprConstant.cpp:0:0
#15 0x0000000007037393 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#16 0x0000000006fe67ad clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#17 0x0000000006fd4f8e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#18 0x0000000006fdf219 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#19 0x0000000006fdfe8f clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fdfe8f)
#20 0x0000000006067ab5 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6067ab5)
#21 0x00000000060bf97d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x60bf97d)
#22 0x0000000006545fb4 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6545fb4)
#23 0x00000000067c08ee clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c08ee)
#24 0x0000000005f88604 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f88604)
#25 0x0000000005f7ec0d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7ec0d)
#26 0x0000000005f7fa48 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7fa48)
#27 0x0000000005f80989 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f80989)
#28 0x0000000005f822aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f822aa)
#29 0x0000000005eaffc1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eaffc1)
#30 0x0000000005ed74d8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ed74d8)
#31 0x0000000005ea442b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea442b)
#32 0x0000000005ea4b5f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#33 0x0000000005eac504 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac504)
#34 0x0000000005eacd2d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eacd2d)
#35 0x0000000005e9fdda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5e9fdda)
#36 0x0000000004999d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4999d08)
#37 0x00000000041fd819 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41fd819)
#38 0x000000000417ecbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x417ecbe)
#39 0x00000000042dc99e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42dc99e)
#40 0x0000000000befa36 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbefa36)
#41 0x0000000000be72fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x0000000003fdaea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x0000000003674744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3674744)
#44 0x0000000003fdb49f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x0000000003fa3665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa3665)
#46 0x0000000003fa40cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa40cd)
#47 0x0000000003fabff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fabff5)
#48 0x0000000000becedc clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbecedc)
#49 0x0000000000ae7071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae7071)
#50 0x00007fda0f46f083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#51 0x0000000000be6dde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe6dde)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
Compiler Explorer: https://godbolt.org/z/Knxhh666z

When compile this program with clang (version 11d07d9ef618497b825badee8b4f06a48575606b), clang crashes:
```
struct T {
  int b;
  int temp1;
} a;

void f() {
  a.b = ({ a.temp2; });
}
```

Interestingly, altering the names of `temp1` and `temp2` can prevent the crash. For instance, the below program does not trigger the crash:
```
struct T {
  int b;
  int c;
} a;

void f() {
  a.b = ({ a.d; });
}
```

The stack dump:
```
&lt;source&gt;:7:14: error: no member named 'temp2' in 'struct T'; did you mean 'temp1'?
    7 |   a.b = ({ a.temp2; });
      |              ^~~~~
      |              temp1
&lt;source&gt;:3:7: note: 'temp1' declared here
    3 |   int temp1;
      |       ^
clang: /root/llvm-project/clang/include/clang/AST/APValue.h:552: clang::APValue&amp; clang::APValue::getStructField(unsigned int): Assertion `isStruct() &amp;&amp; "Invalid accessor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O0 &lt;source&gt;
1.	&lt;source&gt;:8:1: current parser token '}'
2.	&lt;source&gt;:6:10: parsing function body 'f'
3.	&lt;source&gt;:6:10: in compound statement ('{}')
 #<!-- -->0 0x000000000372dc98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372dc98)
 #<!-- -->1 0x000000000372b95c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372b95c)
 #<!-- -->2 0x0000000003674298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fda0f9cb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007fda0f48e00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->5 0x00007fda0f46d859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->6 0x00007fda0f46d729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->7 0x00007fda0f47efd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->8 0x0000000006f27ec1 clang::APValue::getStructField(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f27ec1)
 #<!-- -->9 0x0000000006fcfa37 (anonymous namespace)::ExtractSubobjectHandler::result_type findSubobject&lt;(anonymous namespace)::ExtractSubobjectHandler&gt;((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::CompleteObject const&amp;, (anonymous namespace)::SubobjectDesignator const&amp;, (anonymous namespace)::ExtractSubobjectHandler&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000006fd907a (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitMemberExpr(clang::MemberExpr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000006fe6fc4 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000006fe72f8 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000006fd4f8e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000006fd792e (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007037393 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000006fe67ad clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000006fd4f8e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000006fdf219 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000006fdfe8f clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fdfe8f)
#<!-- -->20 0x0000000006067ab5 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6067ab5)
#<!-- -->21 0x00000000060bf97d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x60bf97d)
#<!-- -->22 0x0000000006545fb4 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6545fb4)
#<!-- -->23 0x00000000067c08ee clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c08ee)
#<!-- -->24 0x0000000005f88604 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f88604)
#<!-- -->25 0x0000000005f7ec0d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7ec0d)
#<!-- -->26 0x0000000005f7fa48 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7fa48)
#<!-- -->27 0x0000000005f80989 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f80989)
#<!-- -->28 0x0000000005f822aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f822aa)
#<!-- -->29 0x0000000005eaffc1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eaffc1)
#<!-- -->30 0x0000000005ed74d8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ed74d8)
#<!-- -->31 0x0000000005ea442b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea442b)
#<!-- -->32 0x0000000005ea4b5f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->33 0x0000000005eac504 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac504)
#<!-- -->34 0x0000000005eacd2d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eacd2d)
#<!-- -->35 0x0000000005e9fdda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5e9fdda)
#<!-- -->36 0x0000000004999d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4999d08)
#<!-- -->37 0x00000000041fd819 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41fd819)
#<!-- -->38 0x000000000417ecbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x417ecbe)
#<!-- -->39 0x00000000042dc99e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42dc99e)
#<!-- -->40 0x0000000000befa36 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbefa36)
#<!-- -->41 0x0000000000be72fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x0000000003fdaea9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x0000000003674744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3674744)
#<!-- -->44 0x0000000003fdb49f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x0000000003fa3665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa3665)
#<!-- -->46 0x0000000003fa40cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa40cd)
#<!-- -->47 0x0000000003fabff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fabff5)
#<!-- -->48 0x0000000000becedc clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbecedc)
#<!-- -->49 0x0000000000ae7071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae7071)
#<!-- -->50 0x00007fda0f46f083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->51 0x0000000000be6dde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe6dde)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - shafik

this crashes differently in C++: https://godbolt.org/z/YYK7MM53h

Assertion:

```console
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:7948:
bool {anonymous}::ExprEvaluatorBase<Derived>::VisitMemberExpr(const clang::MemberExpr*) [with Derived = {anonymous}::IntExprEvaluator]:
Assertion `!Info.Ctx.getLangOpts().CPlusPlus11 && "missing temporary materialization conversion"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:8:1: current parser token '}'
2.	<source>:6:10: parsing function body 'f'
3.	<source>:6:10: in compound statement ('{}')
 #0 0x000000000372dc98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372dc98)
 #1 0x000000000372b95c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x372b95c)
 #2 0x0000000003674298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f05aad38420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f05aa7fb00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f05aa7da859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f05aa7da729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f05aa7ebfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000006fd91cf (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitMemberExpr(clang::MemberExpr const*) ExprConstant.cpp:0:0
 #9 0x0000000006fe6fc4 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#10 0x0000000006fe72f8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x0000000006fd4f8e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#12 0x0000000006fff233 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x000000000700023d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#14 0x0000000006fd4b5e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x0000000006fdf219 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#16 0x0000000006fdfe8f clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fdfe8f)
#17 0x0000000006067ab5 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6067ab5)
#18 0x00000000060bf97d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x60bf97d)
#19 0x0000000006545fb4 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6545fb4)
#20 0x00000000067c08ee clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c08ee)
#21 0x0000000005f88604 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f88604)
#22 0x0000000005f7ec0d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7ec0d)
#23 0x0000000005f7fa48 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f7fa48)
#24 0x0000000005f80989 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f80989)
#25 0x0000000005f822aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f822aa)
#26 0x0000000005eaffc1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eaffc1)
#27 0x0000000005ed74d8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ed74d8)
#28 0x0000000005ea442b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea442b)
#29 0x0000000005ea4b5f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#30 0x0000000005eac504 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac504)
#31 0x0000000005eacd2d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eacd2d)
#32 0x0000000005e9fdda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5e9fdda)
#33 0x0000000004999d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4999d08)
#34 0x00000000041fd819 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41fd819)
#35 0x000000000417ecbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x417ecbe)
#36 0x00000000042dc99e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42dc99e)
#37 0x0000000000befa36 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbefa36)
#38 0x0000000000be72fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x0000000003fdaea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003674744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3674744)
#41 0x0000000003fdb49f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000003fa3665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa3665)
#43 0x0000000003fa40cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fa40cd)
#44 0x0000000003fabff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fabff5)
#45 0x0000000000becedc clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbecedc)
#46 0x0000000000ae7071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae7071)
#47 0x00007f05aa7dc083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#48 0x0000000000be6dde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe6dde)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - jcsxky

Doing an evaluation on a recovery `MemberExpr` which loses conversion causes this crash.

> Interestingly, altering the names of temp1 and temp2 can prevent the crash. For instance, the below program does not trigger the crash:

This prevent clang doing the recovery and won't evaluate on an error expression.


---

### Comment 4 - JOE1994

Clang trunk & Clang 21.1.0 doesn't assert, and returns a helpful error: https://godbolt.org/z/8exMbcore

---

