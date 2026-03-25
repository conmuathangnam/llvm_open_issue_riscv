# [clang] Assertion `ModifierLen == 0 && "No modifiers for strings yet"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161070
**Status:** Closed
**Labels:** clang:diagnostics, confirmed, crash
**Closed Date:** 2025-09-29T23:16:02Z

## Body

Reproducer:
https://godbolt.org/z/fK5bsr1Yr
```cpp
void foo() { int a = [] [[assume(true)]]; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Basic/Diagnostic.cpp:1332: void clang::Diagnostic::FormatDiagnostic(const char*, const char*, llvm::SmallVectorImpl<char>&) const: Assertion `ModifierLen == 0 && "No modifiers for strings yet"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -pedantic-errors -Weverything <source>
1.	<source>:1:25: current parser token '['
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
4.	<source>:1:22: lambda expression parsing
 #0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ec34f842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ec34f8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ec34f842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ec34f8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ec34f82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ec34f839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000043e7a92 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e7a92)
#10 0x0000000004e4071a clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4071a)
#11 0x00000000043e4a40 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e4a40)
#12 0x00000000043e5155 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e5155)
#13 0x00000000043e5235 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e5235)
#14 0x0000000000db63ad clang::DiagnosticBuilder::~DiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb63ad)
#15 0x0000000006927ec1 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6927ec1)
#16 0x000000000692987b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692987b)
#17 0x0000000006909e35 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6909e35)
#18 0x000000000690a857 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a857)
#19 0x000000000690a8e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a8e9)
#20 0x00000000068c3848 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c3848)
#21 0x00000000068d0bc9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d0bc9)
#22 0x00000000068d986f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d986f)
#23 0x00000000068d9d3c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d9d3c)
#24 0x000000000698ad0a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698ad0a)
#25 0x000000000698ba19 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698ba19)
#26 0x0000000006993f2e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6993f2e)
#27 0x000000000699474a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699474a)
#28 0x000000000689b13b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x689b13b)
#29 0x00000000068d15cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d15cd)
#30 0x000000000688e50e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e50e)
#31 0x000000000688ecaf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ecaf)
#32 0x0000000006896563 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896563)
#33 0x0000000006897475 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897475)
#34 0x0000000006897930 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897930)
#35 0x00000000068783f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68783f3)
#36 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#37 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#38 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#39 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#40 0x0000000000db8ce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#41 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#42 0x0000000000daf9cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#43 0x0000000004b55ea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#45 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#47 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#48 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#49 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#50 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#51 0x00007ec34f829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x00007ec34f829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Note: using `-std=c++23 -pedantic-errors -Weverything`

---

