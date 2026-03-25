# [clang] crashes when parsing enable_if attribute on a function declaration with parameter names but no types

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173826
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2026-01-06T19:38:43Z

## Body

Reproducer:
https://godbolt.org/z/1bW3v3ead
```c
void foo(bar) __attribute__((enable_if(1, "")));
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::ParmVarDecl; From = clang::Decl]: Assertion `Val && "isa<> used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:1:39: current parser token '('
 #0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072f30a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072f30a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072f30a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072f30a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072f30a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072f30a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006a5f2fc (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f2fc)
#10 0x0000000006a5fa53 clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&, clang::SourceLocation&, clang::Parser::LateParsedAttrList*, clang::Declarator*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fa53)
#11 0x0000000006a6028d clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&, clang::Parser::LateParsedAttrList*, clang::Declarator*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6028d)
#12 0x0000000006a62549 clang::Parser::MaybeParseGNUAttributes(clang::Declarator&, clang::Parser::LateParsedAttrList*) (.part.0) ParseDecl.cpp:0:0
#13 0x0000000006a649c0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a649c0)
#14 0x0000000006a21cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21cdb)
#15 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#16 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#17 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#18 0x0000000006a2b740 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b740)
#19 0x0000000006a0c2e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c2e3)
#20 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#21 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#22 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#23 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#24 0x0000000000de5c4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#25 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000ddc81d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004cc3049 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#29 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#31 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#32 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#33 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#34 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#35 0x000072f30a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x000072f30a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-3.5:
https://godbolt.org/z/ErYa85W5M

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/1bW3v3ead
```c
void foo(bar) __attribute__((enable_if(1, "")));
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl&lt;To, const From*&gt;::doit(const From*) [with To = clang::ParmVarDecl; From = clang::Decl]: Assertion `Val &amp;&amp; "isa&lt;&gt; used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:1:39: current parser token '('
 #<!-- -->0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #<!-- -->1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #<!-- -->2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072f30a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072f30a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072f30a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072f30a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072f30a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072f30a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006a5f2fc (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f2fc)
#<!-- -->10 0x0000000006a5fa53 clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&amp;, clang::SourceLocation&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fa53)
#<!-- -->11 0x0000000006a6028d clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6028d)
#<!-- -->12 0x0000000006a62549 clang::Parser::MaybeParseGNUAttributes(clang::Declarator&amp;, clang::Parser::LateParsedAttrList*) (.part.0) ParseDecl.cpp:0:0
#<!-- -->13 0x0000000006a649c0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a649c0)
#<!-- -->14 0x0000000006a21cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21cdb)
#<!-- -->15 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#<!-- -->16 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#<!-- -->17 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#<!-- -->18 0x0000000006a2b740 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b740)
#<!-- -->19 0x0000000006a0c2e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c2e3)
#<!-- -->20 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#<!-- -->21 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#<!-- -->22 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#<!-- -->23 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#<!-- -->24 0x0000000000de5c4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#<!-- -->25 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000ddc81d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004cc3049 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#<!-- -->29 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#<!-- -->31 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#<!-- -->32 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#<!-- -->33 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#<!-- -->34 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#<!-- -->35 0x000072f30a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x000072f30a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-3.5:
https://godbolt.org/z/ErYa85W5M
</details>


---

