# clang Assertion isLinkageValid() failed in Redeclarable::setPreviousDecl when using __attribute__((alias)) 

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173464

## Body

### Link for quick verification: https://godbolt.org/z/WKsWbjasa
```c
#include <immintrin.h>

extern const __m128 kAlias;
const __m128 kAlias __attribute__((alias("kOne")));
```
Go back to `clang 3.7.0 (assertions)`. While the stack trace is similar to [#112737](https://github.com/llvm/llvm-project/issues/112737), the triggering code patterns are distinct and involve different AST node types (`VarDecl` and `FunctionDecl`).

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Decl.h:5354: void clang::Redeclarable<decl_type>::setPreviousDecl(decl_type*) [with decl_type = clang::VarDecl]: Assertion `!isa<NamedDecl>(static_cast<decl_type*>(this)) || cast<NamedDecl>(static_cast<decl_type*>(this))->isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:51: current parser token ';'
 #0 0x00000000042ebc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ebc28)
 #1 0x00000000042e9054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9054)
 #2 0x000000000422de38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000778700442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007787004969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000778700442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007787004287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077870042871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000778700439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006dad960 clang::Redeclarable<clang::VarDecl>::setPreviousDecl(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dad960)
#10 0x0000000006df3e42 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df3e42)
#11 0x0000000006df4eea clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df4eea)
#12 0x0000000006df9c7d clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df9c7d)
#13 0x0000000006e19d1f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e19d1f)
#14 0x0000000006e1a660 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1a660)
#15 0x0000000006a51483 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a51483)
#16 0x0000000006a5f29a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f29a)
#17 0x0000000006a1c94b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c94b)
#18 0x0000000006a1d13f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d13f)
#19 0x0000000006a24f6a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a24f6a)
#20 0x0000000006a25f05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25f05)
#21 0x0000000006a06f8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06f8a)
#22 0x0000000004c4cfb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4cfb8)
#23 0x0000000004f438b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f438b5)
#24 0x0000000004ec3b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec3b4e)
#25 0x000000000503dc3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503dc3d)
#26 0x0000000000de5a6c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5a6c)
#27 0x0000000000ddc4ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#28 0x0000000000ddc63d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#29 0x0000000004cbf909 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x000000000422e2d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422e2d4)
#31 0x0000000004cbff1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004c80a02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c80a02)
#33 0x0000000004c819ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819ae)
#34 0x0000000004c88e05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c88e05)
#35 0x0000000000de1e61 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1e61)
#36 0x0000000000c8ceb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8ceb4)
#37 0x0000778700429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x0000778700429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000ddbf55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbf55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/WKsWbjasa
```c
#include &lt;immintrin.h&gt;

extern const __m128 kAlias;
const __m128 kAlias __attribute__((alias("kOne")));
```
Go back to `clang 3.7.0 (assertions)`. While the stack trace is similar to [#<!-- -->112737](https://github.com/llvm/llvm-project/issues/112737), the triggering code patterns are distinct and involve different AST node types (`VarDecl` and `FunctionDecl`).

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Decl.h:5354: void clang::Redeclarable&lt;decl_type&gt;::setPreviousDecl(decl_type*) [with decl_type = clang::VarDecl]: Assertion `!isa&lt;NamedDecl&gt;(static_cast&lt;decl_type*&gt;(this)) || cast&lt;NamedDecl&gt;(static_cast&lt;decl_type*&gt;(this))-&gt;isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:51: current parser token ';'
 #<!-- -->0 0x00000000042ebc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ebc28)
 #<!-- -->1 0x00000000042e9054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9054)
 #<!-- -->2 0x000000000422de38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000778700442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007787004969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000778700442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007787004287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000077870042871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000778700439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006dad960 clang::Redeclarable&lt;clang::VarDecl&gt;::setPreviousDecl(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dad960)
#<!-- -->10 0x0000000006df3e42 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df3e42)
#<!-- -->11 0x0000000006df4eea clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df4eea)
#<!-- -->12 0x0000000006df9c7d clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df9c7d)
#<!-- -->13 0x0000000006e19d1f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e19d1f)
#<!-- -->14 0x0000000006e1a660 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1a660)
#<!-- -->15 0x0000000006a51483 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a51483)
#<!-- -->16 0x0000000006a5f29a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f29a)
#<!-- -->17 0x0000000006a1c94b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c94b)
#<!-- -->18 0x0000000006a1d13f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d13f)
#<!-- -->19 0x0000000006a24f6a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a24f6a)
#<!-- -->20 0x0000000006a25f05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25f05)
#<!-- -->21 0x0000000006a06f8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06f8a)
#<!-- -->22 0x0000000004c4cfb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4cfb8)
#<!-- -->23 0x0000000004f438b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f438b5)
#<!-- -->24 0x0000000004ec3b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec3b4e)
#<!-- -->25 0x000000000503dc3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503dc3d)
#<!-- -->26 0x0000000000de5a6c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5a6c)
#<!-- -->27 0x0000000000ddc4ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->28 0x0000000000ddc63d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004cbf909 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x000000000422e2d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422e2d4)
#<!-- -->31 0x0000000004cbff1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004c80a02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c80a02)
#<!-- -->33 0x0000000004c819ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819ae)
#<!-- -->34 0x0000000004c88e05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c88e05)
#<!-- -->35 0x0000000000de1e61 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1e61)
#<!-- -->36 0x0000000000c8ceb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8ceb4)
#<!-- -->37 0x0000778700429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x0000778700429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000ddbf55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbf55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

