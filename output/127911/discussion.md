# clang::CastExpr::CastConsistency() const: Assertion `!getType()->isBooleanType() && "unheralded conversion to bool"' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/127911

## Body

Compiler Explorer: https://godbolt.org/z/MT6db5cbq

Console:
```console
<source>:7:15: error: use of undeclared identifier 'nullptr'
    7 |   switch (b = nullptr)
      |               ^
clang: /root/llvm-project/clang/lib/AST/Expr.cpp:1949: bool clang::CastExpr::CastConsistency() const: Assertion `!getType()->isBooleanType() && "unheralded conversion to bool"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:9: current parser token ':'
2.	<source>:5:15: parsing function body 'f7'
3.	<source>:5:15: in compound statement ('{}')
4.	<source>:8:3: in compound statement ('{}')
 #0 0x0000000003e9f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e9f068)
 #1 0x0000000003e9cd24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e9cd24)
 #2 0x0000000003de9128 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000735946a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000735946a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000735946a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000735946a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000735946a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000735946a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076cd2d0 clang::CastExpr::CastConsistency() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76cd2d0)
#10 0x00000000076ce14d clang::ImplicitCastExpr::Create(clang::ASTContext const&, clang::QualType, clang::CastKind, clang::Expr*, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::ExprValueKind, clang::FPOptionsOverride) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76ce14d)
#11 0x0000000006644791 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6644791)
#12 0x0000000006dfd9bf clang::Sema::ActOnCaseExpr(clang::SourceLocation, clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6dfd9bf)
#13 0x00000000065357c2 clang::Parser::ParseCaseExpression(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65357c2)
#14 0x00000000065b0e13 clang::Parser::ParseCaseStatement(clang::Parser::ParsedStmtContext, bool, clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65b0e13)
#15 0x00000000065a84a3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a84a3)
#16 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#17 0x00000000065aa9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aa9f5)
#18 0x00000000065ab0e4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ab0e4)
#19 0x00000000065a7ba7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a7ba7)
#20 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#21 0x00000000065ac388 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ac388)
#22 0x00000000065aecbd clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aecbd)
#23 0x00000000065a86f7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a86f7)
#24 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#25 0x00000000065aa9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aa9f5)
#26 0x00000000065ac1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ac1fa)
#27 0x00000000064bd653 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64bd653)
#28 0x00000000064f237d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64f237d)
#29 0x00000000064b139e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b139e)
#30 0x00000000064b1b59 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b1b59)
#31 0x00000000064b9303 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b9303)
#32 0x00000000064ba1fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ba1fd)
#33 0x00000000064ba6a0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ba6a0)
#34 0x00000000064ac663 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ac663)
#35 0x0000000004860688 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4860688)
#36 0x0000000004b274b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b274b5)
#37 0x0000000004aaae6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4aaae6e)
#38 0x0000000004c15f0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c15f0e)
#39 0x0000000000d6562f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd6562f)
#40 0x0000000000d5d0aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#41 0x00000000048a6c99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000003de95d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3de95d4)
#43 0x00000000048a728f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x000000000486a40d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x486a40d)
#45 0x000000000486b48e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x486b48e)
#46 0x0000000004873445 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4873445)
#47 0x0000000000d62423 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd62423)
#48 0x0000000000c2b604 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2b604)
#49 0x0000735946a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x0000735946a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000d5cb55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd5cb55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
#ifndef __cplusplus
#define bool _Bool
#endif

void f7(void) {
  bool b;
  switch (b = nullptr)
  {
  case 3:
    break;
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/MT6db5cbq

Console:
```console
&lt;source&gt;:7:15: error: use of undeclared identifier 'nullptr'
    7 |   switch (b = nullptr)
      |               ^
clang: /root/llvm-project/clang/lib/AST/Expr.cpp:1949: bool clang::CastExpr::CastConsistency() const: Assertion `!getType()-&gt;isBooleanType() &amp;&amp; "unheralded conversion to bool"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:9: current parser token ':'
2.	&lt;source&gt;:5:15: parsing function body 'f7'
3.	&lt;source&gt;:5:15: in compound statement ('{}')
4.	&lt;source&gt;:8:3: in compound statement ('{}')
 #<!-- -->0 0x0000000003e9f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e9f068)
 #<!-- -->1 0x0000000003e9cd24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e9cd24)
 #<!-- -->2 0x0000000003de9128 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000735946a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000735946a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000735946a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000735946a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000735946a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000735946a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000076cd2d0 clang::CastExpr::CastConsistency() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76cd2d0)
#<!-- -->10 0x00000000076ce14d clang::ImplicitCastExpr::Create(clang::ASTContext const&amp;, clang::QualType, clang::CastKind, clang::Expr*, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::ExprValueKind, clang::FPOptionsOverride) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76ce14d)
#<!-- -->11 0x0000000006644791 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6644791)
#<!-- -->12 0x0000000006dfd9bf clang::Sema::ActOnCaseExpr(clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6dfd9bf)
#<!-- -->13 0x00000000065357c2 clang::Parser::ParseCaseExpression(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65357c2)
#<!-- -->14 0x00000000065b0e13 clang::Parser::ParseCaseStatement(clang::Parser::ParsedStmtContext, bool, clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65b0e13)
#<!-- -->15 0x00000000065a84a3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a84a3)
#<!-- -->16 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#<!-- -->17 0x00000000065aa9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aa9f5)
#<!-- -->18 0x00000000065ab0e4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ab0e4)
#<!-- -->19 0x00000000065a7ba7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a7ba7)
#<!-- -->20 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#<!-- -->21 0x00000000065ac388 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ac388)
#<!-- -->22 0x00000000065aecbd clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aecbd)
#<!-- -->23 0x00000000065a86f7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a86f7)
#<!-- -->24 0x00000000065a9b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a9b30)
#<!-- -->25 0x00000000065aa9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65aa9f5)
#<!-- -->26 0x00000000065ac1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ac1fa)
#<!-- -->27 0x00000000064bd653 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64bd653)
#<!-- -->28 0x00000000064f237d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64f237d)
#<!-- -->29 0x00000000064b139e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b139e)
#<!-- -->30 0x00000000064b1b59 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b1b59)
#<!-- -->31 0x00000000064b9303 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64b9303)
#<!-- -->32 0x00000000064ba1fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ba1fd)
#<!-- -->33 0x00000000064ba6a0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ba6a0)
#<!-- -->34 0x00000000064ac663 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ac663)
#<!-- -->35 0x0000000004860688 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4860688)
#<!-- -->36 0x0000000004b274b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b274b5)
#<!-- -->37 0x0000000004aaae6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4aaae6e)
#<!-- -->38 0x0000000004c15f0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c15f0e)
#<!-- -->39 0x0000000000d6562f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd6562f)
#<!-- -->40 0x0000000000d5d0aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->41 0x00000000048a6c99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x0000000003de95d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3de95d4)
#<!-- -->43 0x00000000048a728f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x000000000486a40d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x486a40d)
#<!-- -->45 0x000000000486b48e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x486b48e)
#<!-- -->46 0x0000000004873445 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4873445)
#<!-- -->47 0x0000000000d62423 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd62423)
#<!-- -->48 0x0000000000c2b604 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2b604)
#<!-- -->49 0x0000735946a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x0000735946a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000d5cb55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd5cb55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
#ifndef __cplusplus
#define bool _Bool
#endif

void f7(void) {
  bool b;
  switch (b = nullptr)
  {
  case 3:
    break;
  }
}
```
</details>


---

