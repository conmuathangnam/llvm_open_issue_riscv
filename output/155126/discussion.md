# [clang][bytecode] Assertion `isPtrType(*ToT)' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/155126
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-08-28T04:20:00Z

## Body

Reproducer:
https://godbolt.org/z/1ofjxqYGc
```c
enum e : bool { b = true };

void foo ()
{
  enum e e1;
  e1 = (bool) nullptr;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Compiler.cpp:504: bool clang::interp::Compiler<Emitter>::VisitCastExpr(const clang::CastExpr*) [with Emitter = clang::interp::EvalEmitter]: Assertion `isPtrType(*ToT)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 -fexperimental-new-constant-interpreter <source>
1.	<source>:7:1: current parser token '}'
2.	<source>:4:1: parsing function body 'foo'
3.	<source>:4:1: in compound statement ('{}')
 #0 0x000000000403c9d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403c9d8)
 #1 0x0000000004039e04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4039e04)
 #2 0x0000000003f7e518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007eb975242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007eb9752969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007eb975242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007eb9752287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007eb97522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007eb975239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007e5e277 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5e277)
#10 0x0000000007e5bb4b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5bb4b)
#11 0x0000000007e5b66c clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5b66c)
#12 0x0000000007e4fe82 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e4fe82)
#13 0x00000000079e9521 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79e9521)
#14 0x00000000079c15f2 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c15f2)
#15 0x00000000079396a3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#16 0x000000000793e9f8 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793e9f8)
#17 0x0000000006923cbb TryGetExprRange(clang::ASTContext&, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#18 0x0000000006953e8e clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6953e8e)
#19 0x0000000006957c89 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#20 0x0000000006959172 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#21 0x00000000069599a5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69599a5)
#22 0x0000000006cdea77 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdea77)
#23 0x0000000006ff064e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff064e)
#24 0x00000000067e6ea4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e6ea4)
#25 0x00000000067de584 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67de584)
#26 0x00000000067df3c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67df3c9)
#27 0x00000000067e751b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e751b)
#28 0x00000000067e7d2a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e7d2a)
#29 0x00000000066dd91b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd91b)
#30 0x0000000006713d7d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713d7d)
#31 0x00000000066d06ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d06ee)
#32 0x00000000066d0e8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d0e8f)
#33 0x00000000066d8cca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d8cca)
#34 0x00000000066d9c65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d9c65)
#35 0x00000000066cb7ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cb7ea)
#36 0x00000000049b06f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b06f8)
#37 0x0000000004ca6b95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca6b95)
#38 0x0000000004c22d3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c22d3e)
#39 0x0000000004d98bc1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d98bc1)
#40 0x0000000000db3c5f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3c5f)
#41 0x0000000000daa90a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x0000000004a186d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x0000000003f7e9b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7e9b4)
#44 0x0000000004a18cef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x00000000049da9ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49da9ad)
#46 0x00000000049dba3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49dba3e)
#47 0x00000000049e3675 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e3675)
#48 0x0000000000db0115 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0115)
#49 0x0000000000c63a44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63a44)
#50 0x00007eb975229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x00007eb975229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000daa3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa3b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Crash goes back to clang-15.0.0:
https://godbolt.org/z/f6c373se1

---

### Comment 2 - tbaederr

This also fails in the current interpreter, although in codegen: https://godbolt.org/z/6Ye35xYP5

In
```
CStyleCastExpr 0x7d97b6856250 '_Bool' <NoOp>
`-ImplicitCastExpr 0x7d97b6856230 '_Bool' <BitCast> part_of_explicit_cast
  `-CXXNullPtrLiteralExpr 0x7d97b6856200 'nullptr_t'
```

Why are we using a `BitCast` here to cast from `nullptr_t` to `bool`?

With a slightly modified example, we get this in c++:
```
BinaryOperator 0x7d9730e8f7b8 'enum e' lvalue '='
|-DeclRefExpr 0x7d9730e57758 'enum e' lvalue Var 0x7d9730e576c8 'e1' 'enum e'
`-CStyleCastExpr 0x7d9730e57868 'e' <IntegralToBoolean>
  `-CStyleCastExpr 0x7d9730e57818 '_Bool' <NoOp>
    `-ImplicitCastExpr 0x7d9730e577f8 '_Bool' <PointerToBoolean> part_of_explicit_cast
      `-CXXNullPtrLiteralExpr 0x7d9730e577c8 'nullptr_t'
```

which is what I expected, a `PointerToBoolean` cast. @AaronBallman?

---

### Comment 3 - tbaederr

Easier C23 reproducer:
```c++
constexpr bool b1 = nullptr;
constexpr bool b2 = (bool)nullptr;
```
https://godbolt.org/z/bv7Waac4b

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/1ofjxqYGc
```c
enum e : bool { b = true };

void foo ()
{
  enum e e1;
  e1 = (bool) nullptr;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Compiler.cpp:504: bool clang::interp::Compiler&lt;Emitter&gt;::VisitCastExpr(const clang::CastExpr*) [with Emitter = clang::interp::EvalEmitter]: Assertion `isPtrType(*ToT)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 -fexperimental-new-constant-interpreter &lt;source&gt;
1.	&lt;source&gt;:7:1: current parser token '}'
2.	&lt;source&gt;:4:1: parsing function body 'foo'
3.	&lt;source&gt;:4:1: in compound statement ('{}')
 #<!-- -->0 0x000000000403c9d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403c9d8)
 #<!-- -->1 0x0000000004039e04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4039e04)
 #<!-- -->2 0x0000000003f7e518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007eb975242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007eb9752969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007eb975242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007eb9752287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007eb97522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007eb975239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007e5e277 clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5e277)
#<!-- -->10 0x0000000007e5bb4b clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5bb4b)
#<!-- -->11 0x0000000007e5b66c clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e5b66c)
#<!-- -->12 0x0000000007e4fe82 clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e4fe82)
#<!-- -->13 0x00000000079e9521 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79e9521)
#<!-- -->14 0x00000000079c15f2 clang::interp::Context::evaluateAsRValue(clang::interp::State&amp;, clang::Expr const*, clang::APValue&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c15f2)
#<!-- -->15 0x00000000079396a3 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->16 0x000000000793e9f8 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793e9f8)
#<!-- -->17 0x0000000006923cbb TryGetExprRange(clang::ASTContext&amp;, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#<!-- -->18 0x0000000006953e8e clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6953e8e)
#<!-- -->19 0x0000000006957c89 AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->20 0x0000000006959172 AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->21 0x00000000069599a5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69599a5)
#<!-- -->22 0x0000000006cdea77 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdea77)
#<!-- -->23 0x0000000006ff064e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff064e)
#<!-- -->24 0x00000000067e6ea4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e6ea4)
#<!-- -->25 0x00000000067de584 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67de584)
#<!-- -->26 0x00000000067df3c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67df3c9)
#<!-- -->27 0x00000000067e751b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e751b)
#<!-- -->28 0x00000000067e7d2a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e7d2a)
#<!-- -->29 0x00000000066dd91b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd91b)
#<!-- -->30 0x0000000006713d7d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713d7d)
#<!-- -->31 0x00000000066d06ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d06ee)
#<!-- -->32 0x00000000066d0e8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d0e8f)
#<!-- -->33 0x00000000066d8cca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d8cca)
#<!-- -->34 0x00000000066d9c65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d9c65)
#<!-- -->35 0x00000000066cb7ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cb7ea)
#<!-- -->36 0x00000000049b06f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b06f8)
#<!-- -->37 0x0000000004ca6b95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca6b95)
#<!-- -->38 0x0000000004c22d3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c22d3e)
#<!-- -->39 0x0000000004d98bc1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d98bc1)
#<!-- -->40 0x0000000000db3c5f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3c5f)
#<!-- -->41 0x0000000000daa90a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x0000000004a186d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x0000000003f7e9b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7e9b4)
#<!-- -->44 0x0000000004a18cef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x00000000049da9ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49da9ad)
#<!-- -->46 0x00000000049dba3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49dba3e)
#<!-- -->47 0x00000000049e3675 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e3675)
#<!-- -->48 0x0000000000db0115 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0115)
#<!-- -->49 0x0000000000c63a44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63a44)
#<!-- -->50 0x00007eb975229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x00007eb975229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000daa3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa3b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

