# assertion failure on __attribute__((swift_name("")))

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/157075
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-09-09T02:03:57Z

## Body

Reproducer:
https://godbolt.org/z/7TvErnPE6
```cpp
__attribute__((swift_name(""))) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/StringRef.h:164: char llvm::StringRef::back() const: Assertion `!empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:52: current parser token '{'
 #0 0x0000000004098828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4098828)
 #1 0x0000000004095c54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4095c54)
 #2 0x0000000003fd9f98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f1120042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f11200969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f1120042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f11200287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f112002871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f1120039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000070e37a6 clang::SemaSwift::DiagnoseName(clang::Decl*, llvm::StringRef, clang::SourceLocation, clang::ParsedAttr const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e37a6)
#10 0x00000000070e3ab3 clang::SemaSwift::handleName(clang::Decl*, clang::ParsedAttr const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e3ab3)
#11 0x0000000006b59712 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#12 0x0000000006b5c67b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c67b)
#13 0x0000000006b103cc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b103cc)
#14 0x0000000006b16991 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b16991)
#15 0x0000000006b17708 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17708)
#16 0x0000000006753dd8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6753dd8)
#17 0x000000000678a98d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678a98d)
#18 0x00000000067472fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67472fe)
#19 0x0000000006747a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747a9f)
#20 0x000000000674f8da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674f8da)
#21 0x0000000006750875 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750875)
#22 0x0000000006750d30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750d30)
#23 0x00000000067307a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67307a3)
#24 0x00000000049d0e08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d0e08)
#25 0x0000000004cca245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cca245)
#26 0x0000000004c45b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c45b8e)
#27 0x0000000004dbfa81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dbfa81)
#28 0x0000000000db2c8f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2c8f)
#29 0x0000000000da993a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004a39c49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003fda434 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fda434)
#32 0x0000000004a3a25f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000049fad4d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fad4d)
#34 0x00000000049fbdde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fbdde)
#35 0x0000000004a03d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a03d75)
#36 0x0000000000daf145 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf145)
#37 0x0000000000c61ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61ce4)
#38 0x00007f1120029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007f1120029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000da93e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda93e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7TvErnPE6
```cpp
__attribute__((swift_name(""))) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/StringRef.h:164: char llvm::StringRef::back() const: Assertion `!empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:52: current parser token '{'
 #<!-- -->0 0x0000000004098828 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4098828)
 #<!-- -->1 0x0000000004095c54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4095c54)
 #<!-- -->2 0x0000000003fd9f98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f1120042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f11200969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f1120042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f11200287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f112002871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f1120039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000070e37a6 clang::SemaSwift::DiagnoseName(clang::Decl*, llvm::StringRef, clang::SourceLocation, clang::ParsedAttr const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e37a6)
#<!-- -->10 0x00000000070e3ab3 clang::SemaSwift::handleName(clang::Decl*, clang::ParsedAttr const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e3ab3)
#<!-- -->11 0x0000000006b59712 ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
#<!-- -->12 0x0000000006b5c67b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c67b)
#<!-- -->13 0x0000000006b103cc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b103cc)
#<!-- -->14 0x0000000006b16991 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b16991)
#<!-- -->15 0x0000000006b17708 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17708)
#<!-- -->16 0x0000000006753dd8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6753dd8)
#<!-- -->17 0x000000000678a98d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678a98d)
#<!-- -->18 0x00000000067472fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67472fe)
#<!-- -->19 0x0000000006747a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747a9f)
#<!-- -->20 0x000000000674f8da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674f8da)
#<!-- -->21 0x0000000006750875 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750875)
#<!-- -->22 0x0000000006750d30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750d30)
#<!-- -->23 0x00000000067307a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67307a3)
#<!-- -->24 0x00000000049d0e08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d0e08)
#<!-- -->25 0x0000000004cca245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cca245)
#<!-- -->26 0x0000000004c45b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c45b8e)
#<!-- -->27 0x0000000004dbfa81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dbfa81)
#<!-- -->28 0x0000000000db2c8f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2c8f)
#<!-- -->29 0x0000000000da993a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004a39c49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003fda434 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fda434)
#<!-- -->32 0x0000000004a3a25f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000049fad4d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fad4d)
#<!-- -->34 0x00000000049fbdde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fbdde)
#<!-- -->35 0x0000000004a03d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a03d75)
#<!-- -->36 0x0000000000daf145 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf145)
#<!-- -->37 0x0000000000c61ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61ce4)
#<!-- -->38 0x00007f1120029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007f1120029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000da93e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda93e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-12.0.0:
https://godbolt.org/z/9hTxvPsGe

---

### Comment 3 - shafik

CC @rjmccall 

---

