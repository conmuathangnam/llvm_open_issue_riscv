# [clang-fuzzer] Assertion failed in `getParamDecl`

**Author:** gal1ium
**URL:** https://github.com/llvm/llvm-project/issues/165995

## Body

Hi, while testing clang by the fuzzing driver `clang-fuzzer` in 531fd45e9238d0485e3268aaf14ae15d01c7740f, it found a crashing case:

PoC:
```c
namespace std{truct type_identity struct{template<T>oid operator delete(s0d::ype_identi
```

Based on https://godbolt.org/z/rfsTT5Y6v it only happens in clang-21.1.0:
```
clang++: /root/llvm-project/clang/include/clang/AST/Decl.h:2775: clang::ParmVarDecl* clang::FunctionDecl::getParamDecl(unsigned int): Assertion `i < getNumParams() && "Illegal param #"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++2a <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	<eof> parser at end of file
2.	<source>:1:1: parsing namespace 'std'
3.	<source>:1:35: parsing struct/union/class body 'std::(unnamed struct at <source>:1:35)'
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079b7af242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079b7af2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079b7af242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079b7af2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079b7af22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079b7af239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006911e12 (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6911e12)
#10 0x0000000006b479f2 clang::Sema::CheckOverloadedOperatorDeclaration(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6b479f2)
#11 0x0000000006aa6677 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6aa6677)
#12 0x0000000006aabef9 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6aabef9)
#13 0x0000000006ab17c1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6ab17c1)
#14 0x0000000006b535ff clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6b535ff)
#15 0x000000000673a6aa clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673a6aa)
#16 0x00000000067f0a25 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67f0a25)
#17 0x00000000067fe2cf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#18 0x0000000006739d5d clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6739d5d)
#19 0x000000000673bee1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673bee1)
#20 0x000000000673dd1f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673dd1f)
#21 0x0000000006740c40 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6740c40)
#22 0x0000000006714220 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6714220)
#23 0x00000000066cf304 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cf304)
#24 0x00000000066cfd39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cfd39)
#25 0x00000000066d7a2a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7a2a)
#26 0x0000000006734ba5 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6734ba5)
#27 0x0000000006735826 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6735826)
#28 0x000000000671af20 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671af20)
#29 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#30 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#31 0x00000000066d8e80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d8e80)
#32 0x00000000066ca703 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca703)
#33 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#34 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#35 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#36 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#37 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#38 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#41 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#43 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#44 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#45 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (gal1ium)

<details>
Hi, while testing clang by the fuzzing driver `clang-fuzzer` in 531fd45e9238d0485e3268aaf14ae15d01c7740f, it found a crashing case:

PoC:
```c
namespace std{truct type_identity struct{template&lt;T&gt;oid operator delete(s0d::ype_identi
```

Based on https://godbolt.org/z/Mj3EaabhT it only happens in clang-21.1.0:
```
clang++: /root/llvm-project/clang/include/clang/AST/Decl.h:2775: clang::ParmVarDecl* clang::FunctionDecl::getParamDecl(unsigned int): Assertion `i &lt; getNumParams() &amp;&amp; "Illegal param #"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++2a &lt;source&gt; -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:1: parsing namespace 'std'
3.	&lt;source&gt;:1:35: parsing struct/union/class body 'std::(unnamed struct at &lt;source&gt;:1:35)'
 #<!-- -->0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #<!-- -->1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #<!-- -->2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079b7af242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079b7af2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079b7af242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079b7af2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000079b7af22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000079b7af239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006911e12 (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6911e12)
#<!-- -->10 0x0000000006b479f2 clang::Sema::CheckOverloadedOperatorDeclaration(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6b479f2)
#<!-- -->11 0x0000000006aa6677 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6aa6677)
#<!-- -->12 0x0000000006aabef9 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6aabef9)
#<!-- -->13 0x0000000006ab17c1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6ab17c1)
#<!-- -->14 0x0000000006b535ff clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6b535ff)
#<!-- -->15 0x000000000673a6aa clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673a6aa)
#<!-- -->16 0x00000000067f0a25 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67f0a25)
#<!-- -->17 0x00000000067fe2cf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->18 0x0000000006739d5d clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6739d5d)
#<!-- -->19 0x000000000673bee1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673bee1)
#<!-- -->20 0x000000000673dd1f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x673dd1f)
#<!-- -->21 0x0000000006740c40 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6740c40)
#<!-- -->22 0x0000000006714220 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6714220)
#<!-- -->23 0x00000000066cf304 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cf304)
#<!-- -->24 0x00000000066cfd39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cfd39)
#<!-- -->25 0x00000000066d7a2a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7a2a)
#<!-- -->26 0x0000000006734ba5 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6734ba5)
#<!-- -->27 0x0000000006735826 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6735826)
#<!-- -->28 0x000000000671af20 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x671af20)
#<!-- -->29 0x00000000066d7adc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7adc)
#<!-- -->30 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#<!-- -->31 0x00000000066d8e80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d8e80)
#<!-- -->32 0x00000000066ca703 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca703)
#<!-- -->33 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#<!-- -->34 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#<!-- -->35 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#<!-- -->36 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#<!-- -->37 0x0000000000d93cff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#<!-- -->38 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004a36b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#<!-- -->41 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#<!-- -->43 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#<!-- -->44 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#<!-- -->45 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)

```
</details>


---

### Comment 2 - EugeneZelenko

Please also include `assertion trunk` into compilers on GodBolt.

---

