# [clang] Assertion `i < getNumParams() && "invalid parameter index"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159080
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:21
**Closed Date:** 2025-09-17T15:13:46Z

## Body

Reproducer:
https://godbolt.org/z/Wh93Y9Eca
```cpp
void foo (void);
void *bar (void) __attribute__ ((malloc(foo)));
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeBase.h:5545: clang::QualType clang::FunctionProtoType::getParamType(unsigned int) const: Assertion `i < getNumParams() && "invalid parameter index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:47: current parser token ';'
 #0 0x000000000417e408 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417e408)
 #1 0x000000000417b834 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417b834)
 #2 0x00000000040bfec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072189ac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072189ac969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072189ac42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072189ac287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072189ac2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072189ac39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006c27864 clang::getFunctionOrMethodParamType(clang::Decl const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27864)
#10 0x0000000006c4a2c0 handleRestrictAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
#11 0x0000000006c60a7f ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#12 0x0000000006c6366b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6366b)
#13 0x0000000006c16ffc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c16ffc)
#14 0x0000000006c1d5e1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1d5e1)
#15 0x0000000006c1e4e0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e4e0)
#16 0x0000000006883e33 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6883e33)
#17 0x0000000006891c09 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6891c09)
#18 0x000000000684ef6e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684ef6e)
#19 0x000000000684f70f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684f70f)
#20 0x000000000685754a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685754a)
#21 0x00000000068584e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68584e5)
#22 0x0000000006838f5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6838f5a)
#23 0x0000000004ab7fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab7fd8)
#24 0x0000000004db21c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db21c5)
#25 0x0000000004d2d47e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d47e)
#26 0x0000000004ea74fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea74fd)
#27 0x0000000000db5cbf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5cbf)
#28 0x0000000000dac92a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004b20f59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x00000000040c0364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c0364)
#31 0x0000000004b2156f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004ae214d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae214d)
#33 0x0000000004ae31de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae31de)
#34 0x0000000004aeb175 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aeb175)
#35 0x0000000000db2145 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2145)
#36 0x0000000000c63f94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63f94)
#37 0x000072189ac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000072189ac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000dac3d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac3d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Wh93Y9Eca
```cpp
void foo (void);
void *bar (void) __attribute__ ((malloc(foo)));
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeBase.h:5545: clang::QualType clang::FunctionProtoType::getParamType(unsigned int) const: Assertion `i &lt; getNumParams() &amp;&amp; "invalid parameter index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:2:47: current parser token ';'
 #<!-- -->0 0x000000000417e408 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417e408)
 #<!-- -->1 0x000000000417b834 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417b834)
 #<!-- -->2 0x00000000040bfec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072189ac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072189ac969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072189ac42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072189ac287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072189ac2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072189ac39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006c27864 clang::getFunctionOrMethodParamType(clang::Decl const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c27864)
#<!-- -->10 0x0000000006c4a2c0 handleRestrictAttr(clang::Sema&amp;, clang::Decl*, clang::ParsedAttr const&amp;) SemaDeclAttr.cpp:0:0
#<!-- -->11 0x0000000006c60a7f ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
#<!-- -->12 0x0000000006c6366b clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6366b)
#<!-- -->13 0x0000000006c16ffc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c16ffc)
#<!-- -->14 0x0000000006c1d5e1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1d5e1)
#<!-- -->15 0x0000000006c1e4e0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e4e0)
#<!-- -->16 0x0000000006883e33 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6883e33)
#<!-- -->17 0x0000000006891c09 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6891c09)
#<!-- -->18 0x000000000684ef6e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684ef6e)
#<!-- -->19 0x000000000684f70f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684f70f)
#<!-- -->20 0x000000000685754a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685754a)
#<!-- -->21 0x00000000068584e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68584e5)
#<!-- -->22 0x0000000006838f5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6838f5a)
#<!-- -->23 0x0000000004ab7fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab7fd8)
#<!-- -->24 0x0000000004db21c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db21c5)
#<!-- -->25 0x0000000004d2d47e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d47e)
#<!-- -->26 0x0000000004ea74fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea74fd)
#<!-- -->27 0x0000000000db5cbf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5cbf)
#<!-- -->28 0x0000000000dac92a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004b20f59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x00000000040c0364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c0364)
#<!-- -->31 0x0000000004b2156f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004ae214d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae214d)
#<!-- -->33 0x0000000004ae31de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae31de)
#<!-- -->34 0x0000000004aeb175 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aeb175)
#<!-- -->35 0x0000000000db2145 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2145)
#<!-- -->36 0x0000000000c63f94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63f94)
#<!-- -->37 0x000072189ac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000072189ac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000dac3d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac3d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/r1dse86rb

---

### Comment 3 - nicovank

Bisect points to 79a28aa0a48feba34ddc3c1791ea0be88f354542 / https://github.com/llvm/llvm-project/pull/68059.

---

### Comment 4 - shafik

CC @aloisklink 

---

### Comment 5 - erichkeane

See https://github.com/llvm/llvm-project/pull/159371

---

