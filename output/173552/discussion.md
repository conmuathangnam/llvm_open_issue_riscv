# Clang assertion failure with __attribute__((target_clones("fxsr"))) in StringSwitch.h

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173552
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-valid
**Closed Date:** 2025-12-26T06:23:05Z

## Body

### Link for quick verification: https://godbolt.org/z/vjx6Mo8rq
```c
struct A {
    __attribute__((target_clones("fxsr"))) int m_fn1() { return 0; }
};
```
Go back to `clang 20.1.0 (assertions)`.
The crash occurs during semantic analysis in `SemaX86::checkTargetClonesAttr`, specifically while computing FMV priority via `X86TargetInfo::getFMVPriority`.

## Stack dump:
```console
Fell off the end of a string-switch
UNREACHABLE executed at /root/llvm-project/llvm/include/llvm/ADT/StringSwitch.h:212!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:56: current parser token '{'
2.	<source>:1:1: parsing struct/union/class body 'A'
 #0 0x00000000042ecc08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecc08)
 #1 0x00000000042ea034 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ea034)
 #2 0x000000000422edd8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000781a23e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000781a23e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000781a23e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000781a23e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000004239a0a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239a0a)
 #8 0x00000000085898be getFeature(llvm::StringRef) X86.cpp:0:0
 #9 0x000000000858ab43 clang::targets::X86TargetInfo::getFMVPriority(llvm::ArrayRef<llvm::StringRef>) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x858ab43)
#10 0x00000000076afbed clang::SemaX86::checkTargetClonesAttr(llvm::SmallVectorImpl<llvm::StringRef>&, llvm::SmallVectorImpl<clang::SourceLocation>&, llvm::SmallVectorImpl<llvm::SmallString<64u>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76afbed)
#11 0x0000000006e3cf1a handleTargetClonesAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
#12 0x0000000006e615dd ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#13 0x0000000006e6468b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6468b)
#14 0x0000000006e14bfc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e14bfc)
#15 0x0000000006e1b1e1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1b1e1)
#16 0x0000000006ebbc61 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ebbc61)
#17 0x0000000006a42580 clang::Parser::ParseCXXInlineMethodDef(clang::AccessSpecifier, clang::ParsedAttributesView const&, clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::VirtSpecifiers const&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a42580)
#18 0x0000000006a89f74 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a89f74)
#19 0x0000000006a8bb7f clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8bb7f)
#20 0x0000000006a8d93c clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d93c)
#21 0x0000000006a90520 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90520)
#22 0x0000000006a6334e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6334e)
#23 0x0000000006a1e147 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e147)
#24 0x0000000006a1ebbf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ebbf)
#25 0x0000000006a269ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a269ea)
#26 0x0000000006a27985 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27985)
#27 0x0000000006a27e30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27e30)
#28 0x0000000006a089d3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a089d3)
#29 0x0000000004c4df88 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4df88)
#30 0x0000000004f44885 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f44885)
#31 0x0000000004ec4b1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec4b1e)
#32 0x000000000503ec0d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ec0d)
#33 0x0000000000de5b1c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b1c)
#34 0x0000000000ddc56a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000ddc6ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004cc08d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x000000000422f274 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f274)
#38 0x0000000004cc0eef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004c819d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819d2)
#40 0x0000000004c8297e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8297e)
#41 0x0000000004c89dd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89dd5)
#42 0x0000000000de1f11 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f11)
#43 0x0000000000c8cf64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf64)
#44 0x0000781a23e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x0000781a23e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000ddc005 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc005)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/vjx6Mo8rq
```c
struct A {
    __attribute__((target_clones("fxsr"))) int m_fn1() { return 0; }
};
```
Go back to `clang 20.1.0 (assertions)`.
The crash occurs during semantic analysis in `SemaX86::checkTargetClonesAttr`, specifically while computing FMV priority via `X86TargetInfo::getFMVPriority`.

## Stack dump:
```console
Fell off the end of a string-switch
UNREACHABLE executed at /root/llvm-project/llvm/include/llvm/ADT/StringSwitch.h:212!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:2:56: current parser token '{'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'A'
 #<!-- -->0 0x00000000042ecc08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecc08)
 #<!-- -->1 0x00000000042ea034 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ea034)
 #<!-- -->2 0x000000000422edd8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000781a23e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000781a23e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000781a23e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000781a23e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000004239a0a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239a0a)
 #<!-- -->8 0x00000000085898be getFeature(llvm::StringRef) X86.cpp:0:0
 #<!-- -->9 0x000000000858ab43 clang::targets::X86TargetInfo::getFMVPriority(llvm::ArrayRef&lt;llvm::StringRef&gt;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x858ab43)
#<!-- -->10 0x00000000076afbed clang::SemaX86::checkTargetClonesAttr(llvm::SmallVectorImpl&lt;llvm::StringRef&gt;&amp;, llvm::SmallVectorImpl&lt;clang::SourceLocation&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::SmallString&lt;64u&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76afbed)
#<!-- -->11 0x0000000006e3cf1a handleTargetClonesAttr(clang::Sema&amp;, clang::Decl*, clang::ParsedAttr const&amp;) SemaDeclAttr.cpp:0:0
#<!-- -->12 0x0000000006e615dd ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
#<!-- -->13 0x0000000006e6468b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6468b)
#<!-- -->14 0x0000000006e14bfc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e14bfc)
#<!-- -->15 0x0000000006e1b1e1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1b1e1)
#<!-- -->16 0x0000000006ebbc61 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ebbc61)
#<!-- -->17 0x0000000006a42580 clang::Parser::ParseCXXInlineMethodDef(clang::AccessSpecifier, clang::ParsedAttributesView const&amp;, clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::VirtSpecifiers const&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a42580)
#<!-- -->18 0x0000000006a89f74 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a89f74)
#<!-- -->19 0x0000000006a8bb7f clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8bb7f)
#<!-- -->20 0x0000000006a8d93c clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d93c)
#<!-- -->21 0x0000000006a90520 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90520)
#<!-- -->22 0x0000000006a6334e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6334e)
#<!-- -->23 0x0000000006a1e147 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e147)
#<!-- -->24 0x0000000006a1ebbf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ebbf)
#<!-- -->25 0x0000000006a269ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a269ea)
#<!-- -->26 0x0000000006a27985 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27985)
#<!-- -->27 0x0000000006a27e30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27e30)
#<!-- -->28 0x0000000006a089d3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a089d3)
#<!-- -->29 0x0000000004c4df88 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4df88)
#<!-- -->30 0x0000000004f44885 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f44885)
#<!-- -->31 0x0000000004ec4b1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec4b1e)
#<!-- -->32 0x000000000503ec0d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ec0d)
#<!-- -->33 0x0000000000de5b1c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b1c)
#<!-- -->34 0x0000000000ddc56a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000ddc6ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004cc08d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x000000000422f274 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f274)
#<!-- -->38 0x0000000004cc0eef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004c819d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819d2)
#<!-- -->40 0x0000000004c8297e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8297e)
#<!-- -->41 0x0000000004c89dd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89dd5)
#<!-- -->42 0x0000000000de1f11 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f11)
#<!-- -->43 0x0000000000c8cf64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf64)
#<!-- -->44 0x0000781a23e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x0000781a23e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000ddc005 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc005)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - ecnelises

Confirmed duplicate of #172653 and can be fixed by #172844

---

