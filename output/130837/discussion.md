# [OpenMP] Crash with `omp declare simd` when associated with a C struct member

**Author:** erichkeane
**URL:** https://github.com/llvm/llvm-project/issues/130837

## Body

https://godbolt.org/z/nTMGdT1MG

```
struct S{
    #pragma omp declare simd 
    int x();

};
```

See assert:
```
clang++: /root/llvm-project/clang/lib/AST/Decl.cpp:2228: bool isDeclExternC(const T&) [with T = clang::FunctionDecl]: Assertion `D.getASTContext().getLangOpts().CPlusPlus' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp -x c <source>
1.	<source>:3:12: current parser token ';'
2.	<source>:1:1: parsing struct/union body 'S'
 #0 0x0000000003e6dcc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6dcc8)
 #1 0x0000000003e6b984 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6b984)
 #2 0x0000000003db8298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c08e3442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c08e34969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c08e3442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c08e34287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c08e342871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007c08e3439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000075333cd (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75333cd)
#10 0x00000000067f8188 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f8188)
#11 0x00000000067fc7c0 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fc7c0)
#12 0x00000000067fd300 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fd300)
#13 0x0000000006464a6e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6464a6e)
#14 0x0000000006474619 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6474619)
#15 0x000000000643411e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643411e)
#16 0x0000000006434583 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6434583)
#17 0x000000000643c0b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643c0b3)
#18 0x000000000650c876 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&, clang::ParsedAttributes&, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#19 0x000000000648693f clang::Parser::ParseStructUnionBody(clang::SourceLocation, clang::TypeSpecifierType, clang::RecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x648693f)
#20 0x00000000064a171b clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a171b)
#21 0x00000000064769e8 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64769e8)
#22 0x0000000006433e74 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6433e74)
#23 0x00000000064348d9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64348d9)
#24 0x000000000643c0b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643c0b3)
#25 0x000000000643cf8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643cf8d)
#26 0x000000000643d430 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643d430)
#27 0x000000000642f3e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642f3e3)
#28 0x0000000004830ea8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4830ea8)
#29 0x0000000004af9d95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af9d95)
#30 0x0000000004a7d56e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7d56e)
#31 0x0000000004be7f2e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be7f2e)
#32 0x0000000000d535a7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd535a7)
#33 0x0000000000d4a36a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x0000000004879539 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000003db8744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db8744)
#36 0x0000000004879b2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x000000000483c07d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483c07d)
#38 0x000000000483d0fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483d0fe)
#39 0x0000000004845015 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4845015)
#40 0x0000000000d4f843 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f843)
#41 0x0000000000c17374 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc17374)
#42 0x00007c08e3429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x00007c08e3429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000d49e15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49e15)
```


This happens for ANY declaration there, so it seems that we're parsing C struct members incorrectly in the associated-decl for the `omp declare simd`.  I would expect that a variable would diagnose as `not a function`, and that a function would just go onto diagnose `not allowed here` (since it is a C struct).

## Comments

### Comment 1 - erichkeane

@alexey-bataev FYI!

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Erich Keane (erichkeane)

<details>
https://godbolt.org/z/nTMGdT1MG

```
struct S{
    #pragma omp declare simd 
    int x();

};
```

See assert:
```
clang++: /root/llvm-project/clang/lib/AST/Decl.cpp:2228: bool isDeclExternC(const T&amp;) [with T = clang::FunctionDecl]: Assertion `D.getASTContext().getLangOpts().CPlusPlus' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp -x c &lt;source&gt;
1.	&lt;source&gt;:3:12: current parser token ';'
2.	&lt;source&gt;:1:1: parsing struct/union body 'S'
 #<!-- -->0 0x0000000003e6dcc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6dcc8)
 #<!-- -->1 0x0000000003e6b984 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6b984)
 #<!-- -->2 0x0000000003db8298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c08e3442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c08e34969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c08e3442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c08e34287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c08e342871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c08e3439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000075333cd (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75333cd)
#<!-- -->10 0x00000000067f8188 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f8188)
#<!-- -->11 0x00000000067fc7c0 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fc7c0)
#<!-- -->12 0x00000000067fd300 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fd300)
#<!-- -->13 0x0000000006464a6e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6464a6e)
#<!-- -->14 0x0000000006474619 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6474619)
#<!-- -->15 0x000000000643411e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643411e)
#<!-- -->16 0x0000000006434583 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6434583)
#<!-- -->17 0x000000000643c0b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643c0b3)
#<!-- -->18 0x000000000650c876 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#<!-- -->19 0x000000000648693f clang::Parser::ParseStructUnionBody(clang::SourceLocation, clang::TypeSpecifierType, clang::RecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x648693f)
#<!-- -->20 0x00000000064a171b clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a171b)
#<!-- -->21 0x00000000064769e8 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64769e8)
#<!-- -->22 0x0000000006433e74 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6433e74)
#<!-- -->23 0x00000000064348d9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64348d9)
#<!-- -->24 0x000000000643c0b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643c0b3)
#<!-- -->25 0x000000000643cf8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643cf8d)
#<!-- -->26 0x000000000643d430 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643d430)
#<!-- -->27 0x000000000642f3e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642f3e3)
#<!-- -->28 0x0000000004830ea8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4830ea8)
#<!-- -->29 0x0000000004af9d95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af9d95)
#<!-- -->30 0x0000000004a7d56e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7d56e)
#<!-- -->31 0x0000000004be7f2e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be7f2e)
#<!-- -->32 0x0000000000d535a7 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd535a7)
#<!-- -->33 0x0000000000d4a36a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x0000000004879539 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->35 0x0000000003db8744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db8744)
#<!-- -->36 0x0000000004879b2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->37 0x000000000483c07d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483c07d)
#<!-- -->38 0x000000000483d0fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483d0fe)
#<!-- -->39 0x0000000004845015 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4845015)
#<!-- -->40 0x0000000000d4f843 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f843)
#<!-- -->41 0x0000000000c17374 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc17374)
#<!-- -->42 0x00007c08e3429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->43 0x00007c08e3429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->44 0x0000000000d49e15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49e15)
```


This happens for ANY declaration there, so it seems that we're parsing C struct members incorrectly in the associated-decl for the `omp declare simd`.  I would expect that a variable would diagnose as `not a function`, and that a function would just go onto diagnose `not allowed here` (since it is a C struct).
</details>


---

