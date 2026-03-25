# [clang] Assertion `!Qualifier' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/167120
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2026-02-13T12:44:07Z

## Body

Reproducer:
https://godbolt.org/z/f9saqErea
```cpp
#include <vector>

template <typename Range> void foo() {
  using T = std::ranges::range_value_t<Range>;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:5174: clang::QualType clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, const clang::TypeDecl*) const: Assertion `!Qualifier' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:26: current parser token 'range_value_t'
2.	<source>:3:38: parsing function body 'foo'
3.	<source>:3:38: in compound statement ('{}')
 #0 0x000000000424d5d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424d5d8)
 #1 0x000000000424aa04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aa04)
 #2 0x000000000418ec68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007736c2842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007736c28969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007736c2842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007736c28287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007736c282871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007736c2839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000078714ad clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, clang::TypeDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78714ad)
#10 0x0000000006b69c00 ExtendNestedNameSpecifier(clang::Sema&, clang::CXXScopeSpec&, clang::NamedDecl const*, clang::SourceLocation, clang::SourceLocation) (.isra.0) SemaCXXScopeSpec.cpp:0:0
#11 0x0000000006b71ae8 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, clang::NamedDecl*, bool, bool*, bool) (.part.0) SemaCXXScopeSpec.cpp:0:0
#12 0x0000000006b736c3 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b736c3)
#13 0x00000000069f318f clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f318f)
#14 0x000000000695e8ac clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695e8ac)
#15 0x00000000069a594f clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a594f)
#16 0x00000000069a6e25 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a6e25)
#17 0x00000000069adc9d clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69adc9d)
#18 0x00000000069bc984 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::Parser::UsingDeclarator&, clang::SourceLocation&, clang::AccessSpecifier, clang::ParsedAttributes&, clang::Decl**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bc984)
#19 0x00000000069c3cb6 clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c3cb6)
#20 0x00000000069c4805 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c4805)
#21 0x00000000069aa823 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69aa823)
#22 0x0000000006a5bfea clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5bfea)
#23 0x0000000006a5c846 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5c846)
#24 0x0000000006a64fce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a64fce)
#25 0x0000000006a6581a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6581a)
#26 0x000000000696ba2b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696ba2b)
#27 0x00000000069a1e2d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a1e2d)
#28 0x0000000006a71768 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a71768)
#29 0x0000000006a7ec19 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#30 0x0000000006a7f02a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f02a)
#31 0x00000000069aa7a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69aa7a0)
#32 0x0000000006967277 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6967277)
#33 0x0000000006968145 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6968145)
#34 0x000000000694973a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694973a)
#35 0x0000000004b99c68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b99c68)
#36 0x0000000004e882f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e882f5)
#37 0x0000000004e090fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e090fe)
#38 0x0000000004f8043d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8043d)
#39 0x0000000000dd2410 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd2410)
#40 0x0000000000dc8f8a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#41 0x0000000000dc910d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#42 0x0000000004c03ae9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x000000000418f104 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x418f104)
#44 0x0000000004c040ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x0000000004bc4b52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc4b52)
#46 0x0000000004bc5afe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc5afe)
#47 0x0000000004bcd255 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcd255)
#48 0x0000000000dce931 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce931)
#49 0x0000000000c7cee4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7cee4)
#50 0x00007736c2829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x00007736c2829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000dc8a25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc8a25)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/f9saqErea
```cpp
#include &lt;vector&gt;

template &lt;typename Range&gt; void foo() {
  using T = std::ranges::range_value_t&lt;Range&gt;;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:5174: clang::QualType clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, const clang::TypeDecl*) const: Assertion `!Qualifier' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:26: current parser token 'range_value_t'
2.	&lt;source&gt;:3:38: parsing function body 'foo'
3.	&lt;source&gt;:3:38: in compound statement ('{}')
 #<!-- -->0 0x000000000424d5d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424d5d8)
 #<!-- -->1 0x000000000424aa04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aa04)
 #<!-- -->2 0x000000000418ec68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007736c2842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007736c28969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007736c2842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007736c28287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007736c282871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007736c2839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000078714ad clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, clang::TypeDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78714ad)
#<!-- -->10 0x0000000006b69c00 ExtendNestedNameSpecifier(clang::Sema&amp;, clang::CXXScopeSpec&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::SourceLocation) (.isra.0) SemaCXXScopeSpec.cpp:0:0
#<!-- -->11 0x0000000006b71ae8 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, clang::NamedDecl*, bool, bool*, bool) (.part.0) SemaCXXScopeSpec.cpp:0:0
#<!-- -->12 0x0000000006b736c3 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b736c3)
#<!-- -->13 0x00000000069f318f clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f318f)
#<!-- -->14 0x000000000695e8ac clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695e8ac)
#<!-- -->15 0x00000000069a594f clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a594f)
#<!-- -->16 0x00000000069a6e25 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a6e25)
#<!-- -->17 0x00000000069adc9d clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69adc9d)
#<!-- -->18 0x00000000069bc984 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::Parser::UsingDeclarator&amp;, clang::SourceLocation&amp;, clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Decl**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bc984)
#<!-- -->19 0x00000000069c3cb6 clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c3cb6)
#<!-- -->20 0x00000000069c4805 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c4805)
#<!-- -->21 0x00000000069aa823 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69aa823)
#<!-- -->22 0x0000000006a5bfea clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5bfea)
#<!-- -->23 0x0000000006a5c846 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5c846)
#<!-- -->24 0x0000000006a64fce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a64fce)
#<!-- -->25 0x0000000006a6581a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6581a)
#<!-- -->26 0x000000000696ba2b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696ba2b)
#<!-- -->27 0x00000000069a1e2d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a1e2d)
#<!-- -->28 0x0000000006a71768 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a71768)
#<!-- -->29 0x0000000006a7ec19 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->30 0x0000000006a7f02a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f02a)
#<!-- -->31 0x00000000069aa7a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69aa7a0)
#<!-- -->32 0x0000000006967277 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6967277)
#<!-- -->33 0x0000000006968145 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6968145)
#<!-- -->34 0x000000000694973a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694973a)
#<!-- -->35 0x0000000004b99c68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b99c68)
#<!-- -->36 0x0000000004e882f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e882f5)
#<!-- -->37 0x0000000004e090fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e090fe)
#<!-- -->38 0x0000000004f8043d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8043d)
#<!-- -->39 0x0000000000dd2410 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd2410)
#<!-- -->40 0x0000000000dc8f8a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->41 0x0000000000dc910d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->42 0x0000000004c03ae9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x000000000418f104 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x418f104)
#<!-- -->44 0x0000000004c040ff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x0000000004bc4b52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc4b52)
#<!-- -->46 0x0000000004bc5afe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc5afe)
#<!-- -->47 0x0000000004bcd255 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcd255)
#<!-- -->48 0x0000000000dce931 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce931)
#<!-- -->49 0x0000000000c7cee4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7cee4)
#<!-- -->50 0x00007736c2829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x00007736c2829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000dc8a25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc8a25)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

This is trunk only:
https://godbolt.org/z/xKzebsxzP

---

### Comment 3 - MagentaTreehouse

This is crash-on-invalid as shown rejected here:
> This is trunk only: https://godbolt.org/z/xKzebsxzP


---

### Comment 4 - k-arrows

Reproducer without any `#include`:
https://godbolt.org/z/fnqxEdhGc
```cpp
namespace foo {}

template <typename qux> void bar() { using T = foo::quux::baz<qux>; }
```

---

### Comment 5 - k-arrows

Another assertion failure with a similar program:
https://godbolt.org/z/P7zx3vrPP
```cpp
namespace foo {}

template <typename qux> void bar() { using T = foo::bar::baz<qux>; }
```

---

### Comment 6 - shafik

CC @mizvekov 

---

### Comment 7 - shafik

@mizvekov ping

---

### Comment 8 - Thibault-Monnier

Large reduction: https://godbolt.org/z/s6PxWfd1d
```cpp
namespace foo {}

template <typename type>
foo::typel::
```

---

