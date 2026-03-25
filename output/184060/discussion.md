# [clang] crash on named 'continue'

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/184060

## Body

Reproducer:
https://godbolt.org/z/YcchxT8a1
```c
void foo() {
label1:
  for (;;) {
  label2:
    switch (({
      continue label1;
      1;
    })) {}
  }
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2y <source>
1.	<source>:9:3: current parser token '}'
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
4.	<source>:3:12: in compound statement ('{}')
 #0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e5507e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000082c565d clang::LoopControlStmt::getNamedLoopOrSwitch() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x82c565d)
 #5 0x0000000007f345ca EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
 #6 0x0000000007f163d5 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #7 0x0000000007f3cc3e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #8 0x0000000007f3d53b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
 #9 0x0000000007f4611e EvaluateAsInt(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#10 0x0000000007f464a2 clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f464a2)
#11 0x0000000007561658 clang::Sema::ActOnFinishSwitchStmt(clang::SourceLocation, clang::Stmt*, clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7561658)
#12 0x0000000006ce7b4b clang::Parser::ParseSwitchStatement(clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce7b4b)
#13 0x0000000006ce2166 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce2166)
#14 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#15 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#16 0x0000000006cec41c clang::Parser::ParseLabeledStatement(clang::ParsedAttributes&, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec41c)
#17 0x0000000006ce26f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce26f2)
#18 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#19 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#20 0x0000000006cec184 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#21 0x000000000870d1b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x870d1b1)
#22 0x0000000006cdcb75 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdcb75)
#23 0x0000000006ce17c7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce17c7)
#24 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#25 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#26 0x0000000006ce540f clang::Parser::ParseForStatement(clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce540f)
#27 0x0000000006ce21c8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce21c8)
#28 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#29 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#30 0x0000000006cec41c clang::Parser::ParseLabeledStatement(clang::ParsedAttributes&, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec41c)
#31 0x0000000006ce26f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce26f2)
#32 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#33 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#34 0x0000000006cec25a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec25a)
#35 0x0000000006bf18c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf18c3)
#36 0x0000000006c27f5d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27f5d)
#37 0x0000000006be52bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be52bb)
#38 0x0000000006be5abf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5abf)
#39 0x0000000006bed0c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed0c1)
#40 0x0000000006bedff5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bedff5)
#41 0x0000000006bee3e0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee3e0)
#42 0x0000000006bcf6a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6a3)
#43 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#44 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#45 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#46 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#47 0x0000000000df0afe cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#48 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#49 0x0000000000de75ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#50 0x0000000004f58f39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#51 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#52 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#53 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#54 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#55 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#56 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#57 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#58 0x00007e5507e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#59 0x00007e5507e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#60 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/YcchxT8a1
```c
void foo() {
label1:
  for (;;) {
  label2:
    switch (({
      continue label1;
      1;
    })) {}
  }
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2y &lt;source&gt;
1.	&lt;source&gt;:9:3: current parser token '}'
2.	&lt;source&gt;:1:12: parsing function body 'foo'
3.	&lt;source&gt;:1:12: in compound statement ('{}')
4.	&lt;source&gt;:3:12: in compound statement ('{}')
 #<!-- -->0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #<!-- -->1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #<!-- -->2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e5507e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000082c565d clang::LoopControlStmt::getNamedLoopOrSwitch() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x82c565d)
 #<!-- -->5 0x0000000007f345ca EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
 #<!-- -->6 0x0000000007f163d5 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->7 0x0000000007f3cc3e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->8 0x0000000007f3d53b EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
 #<!-- -->9 0x0000000007f4611e EvaluateAsInt(clang::Expr const*, clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::SideEffectsKind, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000007f464a2 clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::SideEffectsKind, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f464a2)
#<!-- -->11 0x0000000007561658 clang::Sema::ActOnFinishSwitchStmt(clang::SourceLocation, clang::Stmt*, clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7561658)
#<!-- -->12 0x0000000006ce7b4b clang::Parser::ParseSwitchStatement(clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce7b4b)
#<!-- -->13 0x0000000006ce2166 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce2166)
#<!-- -->14 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->15 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#<!-- -->16 0x0000000006cec41c clang::Parser::ParseLabeledStatement(clang::ParsedAttributes&amp;, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec41c)
#<!-- -->17 0x0000000006ce26f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce26f2)
#<!-- -->18 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->19 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#<!-- -->20 0x0000000006cec184 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
#<!-- -->21 0x000000000870d1b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x870d1b1)
#<!-- -->22 0x0000000006cdcb75 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdcb75)
#<!-- -->23 0x0000000006ce17c7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce17c7)
#<!-- -->24 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->25 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#<!-- -->26 0x0000000006ce540f clang::Parser::ParseForStatement(clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce540f)
#<!-- -->27 0x0000000006ce21c8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce21c8)
#<!-- -->28 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->29 0x0000000006ce40db clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce40db)
#<!-- -->30 0x0000000006cec41c clang::Parser::ParseLabeledStatement(clang::ParsedAttributes&amp;, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec41c)
#<!-- -->31 0x0000000006ce26f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce26f2)
#<!-- -->32 0x0000000006ce30b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce30b6)
#<!-- -->33 0x0000000006ceba16 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ceba16)
#<!-- -->34 0x0000000006cec25a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cec25a)
#<!-- -->35 0x0000000006bf18c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf18c3)
#<!-- -->36 0x0000000006c27f5d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27f5d)
#<!-- -->37 0x0000000006be52bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be52bb)
#<!-- -->38 0x0000000006be5abf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be5abf)
#<!-- -->39 0x0000000006bed0c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed0c1)
#<!-- -->40 0x0000000006bedff5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bedff5)
#<!-- -->41 0x0000000006bee3e0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee3e0)
#<!-- -->42 0x0000000006bcf6a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6a3)
#<!-- -->43 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#<!-- -->44 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#<!-- -->45 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#<!-- -->46 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#<!-- -->47 0x0000000000df0afe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#<!-- -->48 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->49 0x0000000000de75ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->50 0x0000000004f58f39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->51 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#<!-- -->52 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->53 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#<!-- -->54 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#<!-- -->55 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#<!-- -->56 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#<!-- -->57 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#<!-- -->58 0x00007e5507e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->59 0x00007e5507e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->60 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - AaronBallman

CC @Sirraide 

---

