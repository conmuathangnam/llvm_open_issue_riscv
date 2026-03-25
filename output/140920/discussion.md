# [clang] [OpenACC] Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140920
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, clang:openacc
**Closed Date:** 2025-05-21T17:31:24Z

## Body

Here is a peculiar program that attempts to use template metaprogramming for factorial calculation, but crucially omits the essential base case, leading to infinite template instantiation, and then oddly includes an irrelevant header file at the very end, causing clang to crash when compiled with the `-fopenacc` option.

Reproducible on Godbolt with clang trunk:
https://godbolt.org/z/TGPThPzos

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionDecl; From = clang::DeclContext]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc -fclangir <source>
1.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:798:17: current parser token '='
2.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:68:1: parsing namespace 'std'
3.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:302:5: parsing struct/union/class body 'std::pair'
 #0 0x0000000003fa47b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa47b8)
 #1 0x0000000003fa2444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa2444)
 #2 0x0000000003ee7078 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a6afe642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a6afe6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a6afe642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a6afe6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a6afe62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a6afe639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006dff7f4 clang::SemaOpenACC::ActOnVariableDeclarator(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dff7f4)
#10 0x0000000006a06c18 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06c18)
#11 0x0000000006a1d5ff clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d5ff)
#12 0x0000000006abf0df clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abf0df)
#13 0x00000000066ac8f9 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ac8f9)
#14 0x000000000675e105 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675e105)
#15 0x000000000676b8bf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#16 0x00000000066ab08d clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ab08d)
#17 0x00000000066ad231 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ad231)
#18 0x00000000066af06f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66af06f)
#19 0x00000000066b1fe0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1fe0)
#20 0x0000000006684a90 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6684a90)
#21 0x000000000675dd1c clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675dd1c)
#22 0x000000000676b8bf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#23 0x000000000676bc6a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676bc6a)
#24 0x000000000668b7a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b7a0)
#25 0x00000000066481bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66481bc)
#26 0x00000000066a2574 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a2574)
#27 0x00000000066a31ee clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a31ee)
#28 0x000000000668b7e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b7e0)
#29 0x00000000066481bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66481bc)
#30 0x00000000066490ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66490ad)
#31 0x000000000663b47a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b47a)
#32 0x0000000004951508 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4951508)
#33 0x0000000004c49d35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c49d35)
#34 0x0000000004bc874e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc874e)
#35 0x0000000004d3c9a9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d3c9a9)
#36 0x0000000000dacc0f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdacc0f)
#37 0x0000000000da2dda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x00000000049b8609 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000003ee7514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee7514)
#40 0x00000000049b8c1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x000000000497a9ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497a9ed)
#42 0x000000000497ba7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497ba7e)
#43 0x0000000004983d65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4983d65)
#44 0x0000000000da8b78 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8b78)
#45 0x0000000000c2e554 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e554)
#46 0x00007a6afe629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x00007a6afe629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000da2885 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2885)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Here is a peculiar program that attempts to use template metaprogramming for factorial calculation, but crucially omits the essential base case, leading to infinite template instantiation, and then oddly includes an irrelevant header file at the very end, causing clang to crash when compiled with the `-fopenacc` option.

Reproducible on Godbolt with clang trunk:
https://godbolt.org/z/TGPThPzos

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionDecl; From = clang::DeclContext]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc -fclangir &lt;source&gt;
1.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:798:17: current parser token '='
2.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:68:1: parsing namespace 'std'
3.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/stl_pair.h:302:5: parsing struct/union/class body 'std::pair'
 #<!-- -->0 0x0000000003fa47b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa47b8)
 #<!-- -->1 0x0000000003fa2444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa2444)
 #<!-- -->2 0x0000000003ee7078 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a6afe642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007a6afe6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007a6afe642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007a6afe6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007a6afe62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007a6afe639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006dff7f4 clang::SemaOpenACC::ActOnVariableDeclarator(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dff7f4)
#<!-- -->10 0x0000000006a06c18 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06c18)
#<!-- -->11 0x0000000006a1d5ff clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d5ff)
#<!-- -->12 0x0000000006abf0df clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abf0df)
#<!-- -->13 0x00000000066ac8f9 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ac8f9)
#<!-- -->14 0x000000000675e105 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675e105)
#<!-- -->15 0x000000000676b8bf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->16 0x00000000066ab08d clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ab08d)
#<!-- -->17 0x00000000066ad231 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ad231)
#<!-- -->18 0x00000000066af06f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66af06f)
#<!-- -->19 0x00000000066b1fe0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1fe0)
#<!-- -->20 0x0000000006684a90 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6684a90)
#<!-- -->21 0x000000000675dd1c clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675dd1c)
#<!-- -->22 0x000000000676b8bf clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->23 0x000000000676bc6a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676bc6a)
#<!-- -->24 0x000000000668b7a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b7a0)
#<!-- -->25 0x00000000066481bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66481bc)
#<!-- -->26 0x00000000066a2574 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a2574)
#<!-- -->27 0x00000000066a31ee clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a31ee)
#<!-- -->28 0x000000000668b7e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b7e0)
#<!-- -->29 0x00000000066481bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66481bc)
#<!-- -->30 0x00000000066490ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66490ad)
#<!-- -->31 0x000000000663b47a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b47a)
#<!-- -->32 0x0000000004951508 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4951508)
#<!-- -->33 0x0000000004c49d35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c49d35)
#<!-- -->34 0x0000000004bc874e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc874e)
#<!-- -->35 0x0000000004d3c9a9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d3c9a9)
#<!-- -->36 0x0000000000dacc0f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdacc0f)
#<!-- -->37 0x0000000000da2dda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x00000000049b8609 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000003ee7514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee7514)
#<!-- -->40 0x00000000049b8c1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x000000000497a9ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497a9ed)
#<!-- -->42 0x000000000497ba7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497ba7e)
#<!-- -->43 0x0000000004983d65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4983d65)
#<!-- -->44 0x0000000000da8b78 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8b78)
#<!-- -->45 0x0000000000c2e554 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e554)
#<!-- -->46 0x00007a6afe629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x00007a6afe629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000da2885 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2885)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

CC @erichkeane 

---

### Comment 3 - erichkeane

This was a really strange one!  I was assuming that any sort of "magic static" would be a function local, but we apparently can ALSO make an invalid magic-static-looking declaration in a template head.    Fixed though! Thanks for the report!

---

