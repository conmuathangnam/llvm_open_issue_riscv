# [clang] crash triggered by the use of a struct inside sizeof with -std=c89

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/152884

## Body

Reproducer:
https://godbolt.org/z/PejE8GW9Y
```c
void foo(int a)
{
  struct foo { };

  switch(a = sizeof (struct foo { }))
    default:
      a = sizeof (struct foo { });
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::RecordDecl; From = clang::Decl]: Assertion `Val && "isa<> used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c89 <source>
1.	<source>:7:30: current parser token '{'
2.	<source>:2:1: parsing function body 'foo'
3.	<source>:2:1: in compound statement ('{}')
 #0 0x0000000003ff1df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ff1df8)
 #1 0x0000000003fef224 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fef224)
 #2 0x0000000003f33b68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000788fbcc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000788fbcc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000788fbcc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000788fbcc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000788fbcc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000788fbcc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006718ee5 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6718ee5)
#10 0x00000000066ec07e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ec07e)
#11 0x00000000066ef365 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ef365)
#12 0x000000000672b296 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b296)
#13 0x000000000672c20f clang::Parser::ParseExprAfterUnaryExprOrTypeTrait(clang::Token const&, bool&, clang::OpaquePtr<clang::QualType>&, clang::SourceRange&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672c20f)
#14 0x000000000672c66c clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672c66c)
#15 0x0000000006720912 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6720912)
#16 0x00000000067227e7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67227e7)
#17 0x0000000006723986 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6723986)
#18 0x0000000006726f39 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6726f39)
#19 0x00000000067bbf29 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bbf29)
#20 0x00000000067b36ac clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b36ac)
#21 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#22 0x00000000067b5458 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b5458)
#23 0x00000000067b901e clang::Parser::ParseDefaultStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b901e)
#24 0x00000000067b30e9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b30e9)
#25 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#26 0x00000000067b5458 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b5458)
#27 0x00000000067b8c45 clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b8c45)
#28 0x00000000067b3322 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b3322)
#29 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#30 0x00000000067bc61b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bc61b)
#31 0x00000000067bce6a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bce6a)
#32 0x00000000066b4643 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b4643)
#33 0x00000000066ea42d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ea42d)
#34 0x00000000066a741e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a741e)
#35 0x00000000066a7baf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a7baf)
#36 0x00000000066af8ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66af8ba)
#37 0x00000000066b0845 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b0845)
#38 0x00000000066b0d00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b0d00)
#39 0x00000000066a2563 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a2563)
#40 0x00000000049986e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49986e8)
#41 0x0000000004c8caf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c8caf5)
#42 0x0000000004c08e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c08e2e)
#43 0x0000000004d81611 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d81611)
#44 0x0000000000daeb8f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdaeb8f)
#45 0x0000000000da583a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x00000000049ffff9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000003f34004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f34004)
#48 0x0000000004a0060f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x00000000049c28ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c28ad)
#50 0x00000000049c393e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c393e)
#51 0x00000000049cb385 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49cb385)
#52 0x0000000000dab045 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdab045)
#53 0x0000000000c5f114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc5f114)
#54 0x0000788fbcc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x0000788fbcc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000da52e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda52e5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/PejE8GW9Y
```c
void foo(int a)
{
  struct foo { };

  switch(a = sizeof (struct foo { }))
    default:
      a = sizeof (struct foo { });
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl&lt;To, const From*&gt;::doit(const From*) [with To = clang::RecordDecl; From = clang::Decl]: Assertion `Val &amp;&amp; "isa&lt;&gt; used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c89 &lt;source&gt;
1.	&lt;source&gt;:7:30: current parser token '{'
2.	&lt;source&gt;:2:1: parsing function body 'foo'
3.	&lt;source&gt;:2:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003ff1df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ff1df8)
 #<!-- -->1 0x0000000003fef224 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fef224)
 #<!-- -->2 0x0000000003f33b68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000788fbcc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000788fbcc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000788fbcc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000788fbcc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000788fbcc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000788fbcc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006718ee5 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6718ee5)
#<!-- -->10 0x00000000066ec07e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ec07e)
#<!-- -->11 0x00000000066ef365 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ef365)
#<!-- -->12 0x000000000672b296 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b296)
#<!-- -->13 0x000000000672c20f clang::Parser::ParseExprAfterUnaryExprOrTypeTrait(clang::Token const&amp;, bool&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceRange&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672c20f)
#<!-- -->14 0x000000000672c66c clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672c66c)
#<!-- -->15 0x0000000006720912 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6720912)
#<!-- -->16 0x00000000067227e7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67227e7)
#<!-- -->17 0x0000000006723986 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6723986)
#<!-- -->18 0x0000000006726f39 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6726f39)
#<!-- -->19 0x00000000067bbf29 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bbf29)
#<!-- -->20 0x00000000067b36ac clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b36ac)
#<!-- -->21 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#<!-- -->22 0x00000000067b5458 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b5458)
#<!-- -->23 0x00000000067b901e clang::Parser::ParseDefaultStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b901e)
#<!-- -->24 0x00000000067b30e9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b30e9)
#<!-- -->25 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#<!-- -->26 0x00000000067b5458 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b5458)
#<!-- -->27 0x00000000067b8c45 clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b8c45)
#<!-- -->28 0x00000000067b3322 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b3322)
#<!-- -->29 0x00000000067b44e9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b44e9)
#<!-- -->30 0x00000000067bc61b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bc61b)
#<!-- -->31 0x00000000067bce6a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bce6a)
#<!-- -->32 0x00000000066b4643 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b4643)
#<!-- -->33 0x00000000066ea42d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ea42d)
#<!-- -->34 0x00000000066a741e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a741e)
#<!-- -->35 0x00000000066a7baf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a7baf)
#<!-- -->36 0x00000000066af8ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66af8ba)
#<!-- -->37 0x00000000066b0845 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b0845)
#<!-- -->38 0x00000000066b0d00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b0d00)
#<!-- -->39 0x00000000066a2563 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66a2563)
#<!-- -->40 0x00000000049986e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49986e8)
#<!-- -->41 0x0000000004c8caf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c8caf5)
#<!-- -->42 0x0000000004c08e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c08e2e)
#<!-- -->43 0x0000000004d81611 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d81611)
#<!-- -->44 0x0000000000daeb8f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdaeb8f)
#<!-- -->45 0x0000000000da583a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x00000000049ffff9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000000003f34004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f34004)
#<!-- -->48 0x0000000004a0060f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x00000000049c28ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c28ad)
#<!-- -->50 0x00000000049c393e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c393e)
#<!-- -->51 0x00000000049cb385 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49cb385)
#<!-- -->52 0x0000000000dab045 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdab045)
#<!-- -->53 0x0000000000c5f114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc5f114)
#<!-- -->54 0x0000788fbcc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x0000788fbcc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000da52e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda52e5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-3.8:
https://godbolt.org/z/ob44Tf5hj

---

