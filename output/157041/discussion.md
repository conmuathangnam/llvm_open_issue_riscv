# [clang] crash on __attribute__((swift_error(nullptr)))

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/157041

## Body

Reproducer:
https://godbolt.org/z/n4dfTT55n
```cpp
__attribute__((swift_error(nullptr))) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&) [with To = clang::IdentifierLoc*; From = llvm::PointerUnion<clang::Expr*, clang::IdentifierLoc*>]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:58: current parser token '{'
 #0 0x0000000004098828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4098828)
 #1 0x0000000004095c54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4095c54)
 #2 0x0000000003fd9f98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070865e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070865e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070865e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070865e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070865e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070865e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000070e472d clang::SemaSwift::handleError(clang::Decl*, clang::ParsedAttr const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e472d)
#10 0x0000000006b59757 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#11 0x0000000006b5c67b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c67b)
#12 0x0000000006b103cc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b103cc)
#13 0x0000000006b16991 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b16991)
#14 0x0000000006b17708 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17708)
#15 0x0000000006753dd8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6753dd8)
#16 0x000000000678a98d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678a98d)
#17 0x00000000067472fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67472fe)
#18 0x0000000006747a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747a9f)
#19 0x000000000674f8da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674f8da)
#20 0x0000000006750875 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750875)
#21 0x0000000006750d30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750d30)
#22 0x00000000067307a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67307a3)
#23 0x00000000049d0e08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d0e08)
#24 0x0000000004cca245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cca245)
#25 0x0000000004c45b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c45b8e)
#26 0x0000000004dbfa81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dbfa81)
#27 0x0000000000db2c8f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2c8f)
#28 0x0000000000da993a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004a39c49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003fda434 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fda434)
#31 0x0000000004a3a25f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x00000000049fad4d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fad4d)
#33 0x00000000049fbdde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fbdde)
#34 0x0000000004a03d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a03d75)
#35 0x0000000000daf145 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf145)
#36 0x0000000000c61ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61ce4)
#37 0x000070865e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000070865e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000da93e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda93e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/n4dfTT55n
```cpp
__attribute__((swift_error(nullptr))) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&amp;) [with To = clang::IdentifierLoc*; From = llvm::PointerUnion&lt;clang::Expr*, clang::IdentifierLoc*&gt;]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:58: current parser token '{'
 #<!-- -->0 0x0000000004098828 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4098828)
 #<!-- -->1 0x0000000004095c54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4095c54)
 #<!-- -->2 0x0000000003fd9f98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070865e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070865e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070865e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070865e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070865e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070865e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000070e472d clang::SemaSwift::handleError(clang::Decl*, clang::ParsedAttr const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e472d)
#<!-- -->10 0x0000000006b59757 ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
#<!-- -->11 0x0000000006b5c67b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c67b)
#<!-- -->12 0x0000000006b103cc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b103cc)
#<!-- -->13 0x0000000006b16991 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b16991)
#<!-- -->14 0x0000000006b17708 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17708)
#<!-- -->15 0x0000000006753dd8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6753dd8)
#<!-- -->16 0x000000000678a98d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678a98d)
#<!-- -->17 0x00000000067472fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67472fe)
#<!-- -->18 0x0000000006747a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747a9f)
#<!-- -->19 0x000000000674f8da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674f8da)
#<!-- -->20 0x0000000006750875 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750875)
#<!-- -->21 0x0000000006750d30 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6750d30)
#<!-- -->22 0x00000000067307a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67307a3)
#<!-- -->23 0x00000000049d0e08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d0e08)
#<!-- -->24 0x0000000004cca245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cca245)
#<!-- -->25 0x0000000004c45b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c45b8e)
#<!-- -->26 0x0000000004dbfa81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dbfa81)
#<!-- -->27 0x0000000000db2c8f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2c8f)
#<!-- -->28 0x0000000000da993a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004a39c49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003fda434 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fda434)
#<!-- -->31 0x0000000004a3a25f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x00000000049fad4d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fad4d)
#<!-- -->33 0x00000000049fbdde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fbdde)
#<!-- -->34 0x0000000004a03d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a03d75)
#<!-- -->35 0x0000000000daf145 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf145)
#<!-- -->36 0x0000000000c61ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61ce4)
#<!-- -->37 0x000070865e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000070865e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000da93e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda93e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-12.0.0:
https://godbolt.org/z/5hnK7aPbj

---

### Comment 3 - shafik

CC @rjmccall 

---

### Comment 4 - shahidiqbal13

Hi @shafik ,
I have assigned it myself and will work once I get the time, thanks

---

