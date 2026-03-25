# [clang] Assertion `isTypeRep((TST) TypeSpecType) && "DeclSpec does not store a type"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/155794
**Status:** Closed
**Labels:** clang:frontend, regression, crash-on-invalid, confirmed
**Closed Date:** 2025-08-28T18:59:18Z

## Body

Reproducer:
https://godbolt.org/z/WTTW5brfx
```c
struct S {
  enum e1 {}
  enum e2 {}
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Sema/DeclSpec.h:518: clang::ParsedType clang::DeclSpec::getRepAsType() const: Assertion `isTypeRep((TST) TypeSpecType) && "DeclSpec does not store a type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:3:3: current parser token 'enum'
2.	<source>:1:1: parsing struct/union body 'S'
 #0 0x00000000040369c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40369c8)
 #1 0x0000000004033df4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4033df4)
 #2 0x0000000003f78278 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072acd2242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072acd22969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072acd2242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072acd22287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072acd222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072acd2239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006a4e49a clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool, clang::RecordDecl*&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e49a)
#10 0x0000000006a4eb37 clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&, clang::ParsedAttributesView const&, clang::RecordDecl*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4eb37)
#11 0x00000000066e6cf0 clang::Parser::ParseStructDeclaration(clang::ParsingDeclSpec&, llvm::function_ref<clang::Decl* (clang::ParsingFieldDeclarator&)>, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e6cf0)
#12 0x00000000066e87c9 clang::Parser::ParseStructUnionBody(clang::SourceLocation, clang::TypeSpecifierType, clang::RecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e87c9)
#13 0x0000000006704964 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6704964)
#14 0x00000000066d856e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d856e)
#15 0x0000000006693018 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693018)
#16 0x0000000006693a4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693a4f)
#17 0x000000000669b88a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669b88a)
#18 0x000000000669c825 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669c825)
#19 0x000000000669cce0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669cce0)
#20 0x000000000667c753 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667c753)
#21 0x0000000004961958 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961958)
#22 0x0000000004c58665 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c58665)
#23 0x0000000004bd3f6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd3f6e)
#24 0x0000000004d4c641 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d4c641)
#25 0x0000000000db59cf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb59cf)
#26 0x0000000000dac67a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000049c8a99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003f78714 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78714)
#29 0x00000000049c90af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000000000498b89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498b89d)
#31 0x000000000498c92e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498c92e)
#32 0x00000000049941b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49941b5)
#33 0x0000000000db1e85 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1e85)
#34 0x0000000000c62674 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc62674)
#35 0x000072acd2229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x000072acd2229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dac125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac125)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/WTTW5brfx
```c
struct S {
  enum e1 {}
  enum e2 {}
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Sema/DeclSpec.h:518: clang::ParsedType clang::DeclSpec::getRepAsType() const: Assertion `isTypeRep((TST) TypeSpecType) &amp;&amp; "DeclSpec does not store a type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:3:3: current parser token 'enum'
2.	&lt;source&gt;:1:1: parsing struct/union body 'S'
 #<!-- -->0 0x00000000040369c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40369c8)
 #<!-- -->1 0x0000000004033df4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4033df4)
 #<!-- -->2 0x0000000003f78278 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072acd2242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072acd22969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072acd2242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072acd22287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072acd222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072acd2239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006a4e49a clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool, clang::RecordDecl*&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e49a)
#<!-- -->10 0x0000000006a4eb37 clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&amp;, clang::ParsedAttributesView const&amp;, clang::RecordDecl*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4eb37)
#<!-- -->11 0x00000000066e6cf0 clang::Parser::ParseStructDeclaration(clang::ParsingDeclSpec&amp;, llvm::function_ref&lt;clang::Decl* (clang::ParsingFieldDeclarator&amp;)&gt;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e6cf0)
#<!-- -->12 0x00000000066e87c9 clang::Parser::ParseStructUnionBody(clang::SourceLocation, clang::TypeSpecifierType, clang::RecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e87c9)
#<!-- -->13 0x0000000006704964 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6704964)
#<!-- -->14 0x00000000066d856e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d856e)
#<!-- -->15 0x0000000006693018 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693018)
#<!-- -->16 0x0000000006693a4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693a4f)
#<!-- -->17 0x000000000669b88a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669b88a)
#<!-- -->18 0x000000000669c825 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669c825)
#<!-- -->19 0x000000000669cce0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669cce0)
#<!-- -->20 0x000000000667c753 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667c753)
#<!-- -->21 0x0000000004961958 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961958)
#<!-- -->22 0x0000000004c58665 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c58665)
#<!-- -->23 0x0000000004bd3f6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd3f6e)
#<!-- -->24 0x0000000004d4c641 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d4c641)
#<!-- -->25 0x0000000000db59cf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb59cf)
#<!-- -->26 0x0000000000dac67a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000049c8a99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003f78714 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78714)
#<!-- -->29 0x00000000049c90af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x000000000498b89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498b89d)
#<!-- -->31 0x000000000498c92e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498c92e)
#<!-- -->32 0x00000000049941b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49941b5)
#<!-- -->33 0x0000000000db1e85 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1e85)
#<!-- -->34 0x0000000000c62674 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc62674)
#<!-- -->35 0x000072acd2229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x000072acd2229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000dac125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac125)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

This is trunk only:
https://godbolt.org/z/9W7M1v1b9

---

### Comment 3 - shafik

@mizvekov git bisect points to dc8596d5485a52abee2967cec72f81ef4872270c

---

