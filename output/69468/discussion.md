# clang: 18: Assertion `getType(Base)->isPointerType() || getType(Base)->isArrayType()' failed.

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/69468

## Body

Compiler Explorer: https://gcc.godbolt.org/z/9Yz4fTevr

When compile this program with clang (version 5a56f0074cf3229983a15496c3caa41d27c5606f), clang crashes:
```
a[i] = b[i];
extern char b[];
extern char a[];

void foo(int j) {
  a[j] = b[j];
}
```

Stack dump:
```
<source>:1:3: error: use of undeclared identifier 'i'
    1 | a[i] = b[i];
      |   ^
<source>:1:1: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    1 | a[i] = b[i];
      | ^
      | int
<source>:1:8: error: use of undeclared identifier 'b'
    1 | a[i] = b[i];
      |        ^
<source>:1:10: error: use of undeclared identifier 'i'
    1 | a[i] = b[i];
      |          ^
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:1704: void {anonymous}::LValue::addUnsizedArray({anonymous}::EvalInfo&, const clang::Expr*, clang::QualType): Assertion `getType(Base)->isPointerType() || getType(Base)->isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O0 <source>
1.	<source>:7:1: current parser token '}'
2.	<source>:5:17: parsing function body 'foo'
3.	<source>:5:17: in compound statement ('{}')
 #0 0x00000000037346b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37346b8)
 #1 0x000000000373237c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x373237c)
 #2 0x000000000367ac28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f29190b5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f2918b7800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f2918b57859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f2918b57729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f2918b68fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000006fbc65f (anonymous namespace)::LValue::addUnsizedArray((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
 #9 0x000000000703f7a2 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#10 0x0000000006ff69c1 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x0000000006ff7c8b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#12 0x0000000006ff93ad (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#13 0x000000000700792e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x00000000070094dd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#15 0x0000000006fdddfe Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#16 0x0000000006fe0bce (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) ExprConstant.cpp:0:0
#17 0x0000000007040633 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#18 0x0000000006fefa4d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#19 0x0000000006fde22e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#20 0x0000000006fe84b9 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#21 0x0000000006fe912f clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fe912f)
#22 0x000000000606fa85 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x606fa85)
#23 0x00000000060c8a0d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x60c8a0d)
#24 0x000000000654f4c4 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x654f4c4)
#25 0x00000000067c9eae clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c9eae)
#26 0x0000000005f904e4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f904e4)
#27 0x0000000005f86aed clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f86aed)
#28 0x0000000005f87928 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f87928)
#29 0x0000000005f88869 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f88869)
#30 0x0000000005f8a18a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f8a18a)
#31 0x0000000005eb7ea1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb7ea1)
#32 0x0000000005edf3b8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5edf3b8)
#33 0x0000000005eac30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac30b)
#34 0x0000000005eaca3f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#35 0x0000000005eb43e4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb43e4)
#36 0x0000000005eb4c0d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb4c0d)
#37 0x0000000005ea7cba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea7cba)
#38 0x00000000049a0c78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0c78)
#39 0x00000000042046a9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42046a9)
#40 0x0000000004185b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4185b4e)
#41 0x00000000042e382e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42e382e)
#42 0x0000000000bf0b06 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf0b06)
#43 0x0000000000be83ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#44 0x0000000003fe1d39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x000000000367b0d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x367b0d4)
#46 0x0000000003fe232f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000003faa4f5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faa4f5)
#48 0x0000000003faaf5d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faaf5d)
#49 0x0000000003fb2e85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb2e85)
#50 0x0000000000bedfac clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbedfac)
#51 0x0000000000ae8141 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8141)
#52 0x00007f2918b59083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#53 0x0000000000be7eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe7eae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
Compiler Explorer: https://gcc.godbolt.org/z/9Yz4fTevr

When compile this program with clang (version 5a56f0074cf3229983a15496c3caa41d27c5606f), clang crashes:
```
a[i] = b[i];
extern char b[];
extern char a[];

void foo(int j) {
  a[j] = b[j];
}
```

Stack dump:
```
&lt;source&gt;:1:3: error: use of undeclared identifier 'i'
    1 | a[i] = b[i];
      |   ^
&lt;source&gt;:1:1: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    1 | a[i] = b[i];
      | ^
      | int
&lt;source&gt;:1:8: error: use of undeclared identifier 'b'
    1 | a[i] = b[i];
      |        ^
&lt;source&gt;:1:10: error: use of undeclared identifier 'i'
    1 | a[i] = b[i];
      |          ^
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:1704: void {anonymous}::LValue::addUnsizedArray({anonymous}::EvalInfo&amp;, const clang::Expr*, clang::QualType): Assertion `getType(Base)-&gt;isPointerType() || getType(Base)-&gt;isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O0 &lt;source&gt;
1.	&lt;source&gt;:7:1: current parser token '}'
2.	&lt;source&gt;:5:17: parsing function body 'foo'
3.	&lt;source&gt;:5:17: in compound statement ('{}')
 #<!-- -->0 0x00000000037346b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37346b8)
 #<!-- -->1 0x000000000373237c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x373237c)
 #<!-- -->2 0x000000000367ac28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f29190b5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007f2918b7800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->5 0x00007f2918b57859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->6 0x00007f2918b57729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->7 0x00007f2918b68fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->8 0x0000000006fbc65f (anonymous namespace)::LValue::addUnsizedArray((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
 #<!-- -->9 0x000000000703f7a2 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000006ff69c1 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000006ff7c8b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000006ff93ad (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x000000000700792e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000070094dd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000006fdddfe Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000006fe0bce (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007040633 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000006fefa4d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000006fde22e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000006fe84b9 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->21 0x0000000006fe912f clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fe912f)
#<!-- -->22 0x000000000606fa85 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x606fa85)
#<!-- -->23 0x00000000060c8a0d clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x60c8a0d)
#<!-- -->24 0x000000000654f4c4 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x654f4c4)
#<!-- -->25 0x00000000067c9eae clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c9eae)
#<!-- -->26 0x0000000005f904e4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f904e4)
#<!-- -->27 0x0000000005f86aed clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f86aed)
#<!-- -->28 0x0000000005f87928 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f87928)
#<!-- -->29 0x0000000005f88869 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f88869)
#<!-- -->30 0x0000000005f8a18a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f8a18a)
#<!-- -->31 0x0000000005eb7ea1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb7ea1)
#<!-- -->32 0x0000000005edf3b8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5edf3b8)
#<!-- -->33 0x0000000005eac30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac30b)
#<!-- -->34 0x0000000005eaca3f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->35 0x0000000005eb43e4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb43e4)
#<!-- -->36 0x0000000005eb4c0d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb4c0d)
#<!-- -->37 0x0000000005ea7cba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea7cba)
#<!-- -->38 0x00000000049a0c78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0c78)
#<!-- -->39 0x00000000042046a9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42046a9)
#<!-- -->40 0x0000000004185b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4185b4e)
#<!-- -->41 0x00000000042e382e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42e382e)
#<!-- -->42 0x0000000000bf0b06 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf0b06)
#<!-- -->43 0x0000000000be83ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000003fe1d39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x000000000367b0d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x367b0d4)
#<!-- -->46 0x0000000003fe232f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000003faa4f5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faa4f5)
#<!-- -->48 0x0000000003faaf5d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faaf5d)
#<!-- -->49 0x0000000003fb2e85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb2e85)
#<!-- -->50 0x0000000000bedfac clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbedfac)
#<!-- -->51 0x0000000000ae8141 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8141)
#<!-- -->52 0x00007f2918b59083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->53 0x0000000000be7eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe7eae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - cygao90

69cf59ed26c1a382960dd206ecffe104b45c755f is the first bad commit

---

### Comment 3 - cygao90

In the first line, `a` will be marked implicitly as int.
```
<source>:1:1: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    1 | a[i] = b[i];
      | ^
      | int
```
`extern char a[];` declares `a` as an imcomplete type. `getType(Base)` will find the fist complete type so it will return `int` which is neither a pointer type nor an array type. Consequently, the assertion fails.

---

### Comment 4 - shafik

This is not particular to C: https://gcc.godbolt.org/z/v6TbP3WGs

---

### Comment 5 - shafik

If we look before it asserts in the call to `APValue::LValueBase::getType()` in the assert, we see that before the loop here:

https://github.com/llvm/llvm-project/blob/f2afd10776bda7e0dc23176afd1a99d4ccbf967c/clang/lib/AST/APValue.cpp#L73-L80

That `D` is an invalid decl:

```console
expr D->isInvalidDecl()

(bool) $1 = true
```

the first iteration through the loop `Redecl` is valid and look like what we want:

```console
IncompleteArrayType 0x14a8e28e0 'char[]' 
`-BuiltinType 0x14a831eb0 'char'
```

The next iteration gets us back to `int` and:

```console
expr Redecl->isInvalidDecl())

(bool) $2 = true
```

I think the logic needs to be adjusted slightly so that `D` gets reset to `Redecl` each iteration and we modify the stop condition for the loop to also check `!Redecl->isInvalidDecl()`.

I verified this fixes the problem and does not break `check-clang` but I am not sure it covers all possible cases.

CC @zygoloid @erichkeane 

---

### Comment 6 - zygoloid

Yeah, bailing out if we see any invalid declaration seems reasonable to me.

---

