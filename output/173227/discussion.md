# Clang crashes with incomplete enum declaration using __attribute__((mode(TI))) in C++20 mode

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173227

## Body

### Link for quick verification: https://godbolt.org/z/bPxnzWhTc
```c
typedef enum __attribute__((mode(TI))) MyEnum; 
MyEnum x;
```
The compiler only crashes when compiled with the `-std=c++20`flag (or later versions). 
This issue can be traced back to `Clang 10.0.0`, as the `-std=c++20` flag was introduced in that version. While the error path is shared with issues [#63959](https://github.com/llvm/llvm-project/issues/63959) and [#135801](https://github.com/llvm/llvm-project/issues/135801), the stack dump differs from those cases.

## Stack dump
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:2:9: current parser token ';'
 #0 0x0000000003d30a58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d30a58)
 #1 0x0000000003d2e42c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d2e42c)
 #2 0x0000000003c753b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d3ad8a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006e8d8b0 clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e8d8b0)
 #5 0x0000000006e96702 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e96702)
 #6 0x0000000006e96d0f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e96d0f)
 #7 0x0000000006e97160 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e97160)
 #8 0x0000000006655441 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6655441)
 #9 0x00000000062a5762 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5762)
#10 0x00000000062ba2dc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ba2dc)
#11 0x0000000006267267 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6267267)
#12 0x0000000006267a07 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6267a07)
#13 0x00000000062703cf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62703cf)
#14 0x0000000006271bd7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6271bd7)
#15 0x000000000624aa1a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624aa1a)
#16 0x00000000045fd215 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45fd215)
#17 0x000000000490d2ba clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x490d2ba)
#18 0x000000000488b20b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488b20b)
#19 0x0000000004a036db clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a036db)
#20 0x0000000000de066d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde066d)
#21 0x0000000000dd84eb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dd858d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004677269 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x0000000003c757d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c757d3)
#25 0x0000000004677489 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x000000000463a672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463a672)
#27 0x000000000463b551 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463b551)
#28 0x0000000004643f9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4643f9c)
#29 0x0000000000ddcfb9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddcfb9)
#30 0x0000000000c8ded4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc8ded4)
#31 0x00007d3ad8a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007d3ad8a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dd7f85 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd7f85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/bPxnzWhTc
```c
typedef enum __attribute__((mode(TI))) MyEnum; 
MyEnum x;
```
The compiler only crashes when compiled with the `-std=c++20`flag (or later versions). 
This issue can be traced back to `Clang 10.0.0`, as the `-std=c++20` flag was introduced in that version. While the error path is shared with issues [#<!-- -->63959](https://github.com/llvm/llvm-project/issues/63959) and [#<!-- -->135801](https://github.com/llvm/llvm-project/issues/135801), the stack dump differs from those cases.

## Stack dump
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:2:9: current parser token ';'
 #<!-- -->0 0x0000000003d30a58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d30a58)
 #<!-- -->1 0x0000000003d2e42c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d2e42c)
 #<!-- -->2 0x0000000003c753b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d3ad8a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006e8d8b0 clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e8d8b0)
 #<!-- -->5 0x0000000006e96702 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e96702)
 #<!-- -->6 0x0000000006e96d0f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e96d0f)
 #<!-- -->7 0x0000000006e97160 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e97160)
 #<!-- -->8 0x0000000006655441 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6655441)
 #<!-- -->9 0x00000000062a5762 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5762)
#<!-- -->10 0x00000000062ba2dc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ba2dc)
#<!-- -->11 0x0000000006267267 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6267267)
#<!-- -->12 0x0000000006267a07 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6267a07)
#<!-- -->13 0x00000000062703cf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62703cf)
#<!-- -->14 0x0000000006271bd7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6271bd7)
#<!-- -->15 0x000000000624aa1a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624aa1a)
#<!-- -->16 0x00000000045fd215 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45fd215)
#<!-- -->17 0x000000000490d2ba clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x490d2ba)
#<!-- -->18 0x000000000488b20b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488b20b)
#<!-- -->19 0x0000000004a036db clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a036db)
#<!-- -->20 0x0000000000de066d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde066d)
#<!-- -->21 0x0000000000dd84eb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dd858d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004677269 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x0000000003c757d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c757d3)
#<!-- -->25 0x0000000004677489 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x000000000463a672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463a672)
#<!-- -->27 0x000000000463b551 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463b551)
#<!-- -->28 0x0000000004643f9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4643f9c)
#<!-- -->29 0x0000000000ddcfb9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddcfb9)
#<!-- -->30 0x0000000000c8ded4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc8ded4)
#<!-- -->31 0x00007d3ad8a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x00007d3ad8a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dd7f85 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd7f85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

