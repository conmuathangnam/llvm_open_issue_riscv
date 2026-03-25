# [clang] Assertion `Idx < getNumArgs() && "Argument index out of range!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161072
**Status:** Closed
**Labels:** clang:diagnostics, confirmed, crash
**Closed Date:** 2025-10-02T15:20:12Z

## Body

Reproducer:
https://godbolt.org/z/hPe4jh5b7
```cpp
struct S {
  enum E { A };
  using E::A;
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Basic/Diagnostic.h:1579: clang::DiagnosticsEngine::ArgumentKind clang::Diagnostic::getArgKind(unsigned int) const: Assertion `Idx < getNumArgs() && "Argument index out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -pedantic-errors -Weverything <source>
1.	<source>:3:13: current parser token ';'
2.	<source>:1:1: parsing struct/union/class body 'S'
 #0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a1dbb642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a1dbb6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a1dbb642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a1dbb6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a1dbb62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a1dbb639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000043e7971 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e7971)
#10 0x0000000004e4071a clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4071a)
#11 0x00000000043e4a40 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e4a40)
#12 0x00000000043e5155 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e5155)
#13 0x00000000043e5235 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e5235)
#14 0x0000000006a1ce44 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ce44)
#15 0x0000000006a94e72 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a94e72)
#16 0x0000000006a09d48 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a09d48)
#17 0x0000000006ccd582 clang::Sema::CheckUsingDeclQualifier(clang::SourceLocation, bool, clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::SourceLocation, clang::LookupResult const*, clang::UsingDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccd582)
#18 0x0000000006cce7fc clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cce7fc)
#19 0x0000000006ccf6a4 clang::Sema::ActOnUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::UnqualifiedId&, clang::SourceLocation, clang::ParsedAttributesView const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccf6a4)
#20 0x00000000068f3066 clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f3066)
#21 0x00000000068f9c60 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9c60)
#22 0x00000000068fb95f clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68fb95f)
#23 0x00000000068fd7d7 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68fd7d7)
#24 0x0000000006900730 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6900730)
#25 0x00000000068d31fe clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d31fe)
#26 0x000000000688e278 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e278)
#27 0x000000000688ecaf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ecaf)
#28 0x0000000006896563 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896563)
#29 0x0000000006897475 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897475)
#30 0x0000000006897930 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897930)
#31 0x00000000068783f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68783f3)
#32 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#33 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#34 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#35 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#36 0x0000000000db8ce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#37 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000daf9cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004b55ea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#41 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#43 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#44 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#45 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#46 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#47 0x00007a1dbb629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007a1dbb629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Note: using `-std=c++20 -pedantic-errors -Weverything`

---

### Comment 2 - k-arrows

Goes back to clang-13.0.0:
https://godbolt.org/z/G7MGoEn8h

---

