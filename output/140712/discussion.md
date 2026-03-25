# [OpenACC] Assertion `Val && "isa<> used on a null pointer"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140712
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, clang:openacc
**Closed Date:** 2025-05-20T13:31:15Z

## Body

Reproducer:
https://godbolt.org/z/fqGKxTdEh
```cpp
void foo() {
  switch (int x = 0) {
  case 0:
#pragma acc parallel
    break;
  }
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::Expr; From = clang::Stmt]: Assertion `Val && "isa<> used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc -fclangir <source>
1.	<eof> parser at end of file
2.	<source>:1:12: parsing function body 'foo'
 #0 0x0000000003f9b608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b608)
 #1 0x0000000003f99294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99294)
 #2 0x0000000003eddec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007018e9442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007018e94969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007018e9442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007018e94287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007018e942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007018e9439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007331ec9 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&) JumpDiagnostics.cpp:0:0
#10 0x0000000007330f97 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&) JumpDiagnostics.cpp:0:0
#11 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&) JumpDiagnostics.cpp:0:0
#12 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&) JumpDiagnostics.cpp:0:0
#13 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&) JumpDiagnostics.cpp:0:0
#14 0x000000000733b2f6 clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733b2f6)
#15 0x00000000069f2c42 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f2c42)
#16 0x00000000067454ef clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67454ef)
#17 0x000000000663f4f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f4f3)
#18 0x0000000006675e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675e8d)
#19 0x000000000663303e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663303e)
#20 0x00000000066337f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66337f9)
#21 0x000000000663b10a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b10a)
#22 0x000000000663c0ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c0ad)
#23 0x000000000663c560 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c560)
#24 0x000000000662e443 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e443)
#25 0x00000000049463b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49463b8)
#26 0x0000000004c3e1d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e1d5)
#27 0x0000000004bbce1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbce1e)
#28 0x0000000004d30e49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d30e49)
#29 0x0000000000dac8df cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac8df)
#30 0x0000000000da2aaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x00000000049ad4b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x0000000003ede364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede364)
#33 0x00000000049adacf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x000000000496f89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f89d)
#35 0x000000000497092e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497092e)
#36 0x0000000004978c15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978c15)
#37 0x0000000000da8848 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8848)
#38 0x0000000000c2e214 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e214)
#39 0x00007018e9429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x00007018e9429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000da2555 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2555)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/fqGKxTdEh
```cpp
void foo() {
  switch (int x = 0) {
  case 0:
#pragma acc parallel
    break;
  }
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl&lt;To, const From*&gt;::doit(const From*) [with To = clang::Expr; From = clang::Stmt]: Assertion `Val &amp;&amp; "isa&lt;&gt; used on a null pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc -fclangir &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:12: parsing function body 'foo'
 #<!-- -->0 0x0000000003f9b608 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b608)
 #<!-- -->1 0x0000000003f99294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99294)
 #<!-- -->2 0x0000000003eddec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007018e9442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007018e94969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007018e9442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007018e94287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007018e942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007018e9439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007331ec9 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&amp;) JumpDiagnostics.cpp:0:0
#<!-- -->10 0x0000000007330f97 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&amp;) JumpDiagnostics.cpp:0:0
#<!-- -->11 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&amp;) JumpDiagnostics.cpp:0:0
#<!-- -->12 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&amp;) JumpDiagnostics.cpp:0:0
#<!-- -->13 0x0000000007330580 (anonymous namespace)::JumpScopeChecker::BuildScopeInformation(clang::Stmt*, unsigned int&amp;) JumpDiagnostics.cpp:0:0
#<!-- -->14 0x000000000733b2f6 clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733b2f6)
#<!-- -->15 0x00000000069f2c42 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69f2c42)
#<!-- -->16 0x00000000067454ef clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67454ef)
#<!-- -->17 0x000000000663f4f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f4f3)
#<!-- -->18 0x0000000006675e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675e8d)
#<!-- -->19 0x000000000663303e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663303e)
#<!-- -->20 0x00000000066337f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66337f9)
#<!-- -->21 0x000000000663b10a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b10a)
#<!-- -->22 0x000000000663c0ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c0ad)
#<!-- -->23 0x000000000663c560 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c560)
#<!-- -->24 0x000000000662e443 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e443)
#<!-- -->25 0x00000000049463b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49463b8)
#<!-- -->26 0x0000000004c3e1d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e1d5)
#<!-- -->27 0x0000000004bbce1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbce1e)
#<!-- -->28 0x0000000004d30e49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d30e49)
#<!-- -->29 0x0000000000dac8df cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac8df)
#<!-- -->30 0x0000000000da2aaa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x00000000049ad4b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003ede364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede364)
#<!-- -->33 0x00000000049adacf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x000000000496f89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f89d)
#<!-- -->35 0x000000000497092e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497092e)
#<!-- -->36 0x0000000004978c15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978c15)
#<!-- -->37 0x0000000000da8848 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8848)
#<!-- -->38 0x0000000000c2e214 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e214)
#<!-- -->39 0x00007018e9429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x00007018e9429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000da2555 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2555)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

