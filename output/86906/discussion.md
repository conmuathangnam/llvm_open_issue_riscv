# [Clang][Crash] Clang-18 Assertion `iter->getSecond() >= 0 && "Found a negative number of references to a VarDecl"' failed.

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/86906

## Body

Compiler Explorer: https://gcc.godbolt.org/z/bhnczdK3h

The reproducer:
```
void
foo (void)
{
  int a[1024] = {0};
  int *p = 0, *q = 0;
  #pragma omp parallel for collapse(2)
  for (p = &a[0]; p < &a[512]; p += 3)
    for (q = p + 64; q < p + 128; q++)
      ;
}
```

The stack backtrace:
```
clang: /root/llvm-project/clang/lib/Sema/SemaDecl.cpp:2189: void clang::Sema::DiagnoseUnusedButSetDecl(const clang::VarDecl*, clang::Sema::DiagReceiverTy): Assertion `iter->getSecond() >= 0 && "Found a negative number of references to a VarDecl"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd <source>
1.	<eof> parser at end of file
2.	<source>:3:1: parsing function body 'foo'
 #0 0x0000000003920cd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3920cd8)
 #1 0x000000000391e9bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x391e9bc)
 #2 0x0000000003865258 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f4897842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f48978969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f4897842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f48978287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f489782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f4897839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000064ed80a clang::Sema::DiagnoseUnusedButSetDecl(clang::VarDecl const*, llvm::function_ref<void (clang::SourceLocation, clang::PartialDiagnostic)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ed80a)
#10 0x00000000065258a5 clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65258a5)
#11 0x00000000061bbe25 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61bbe25)
#12 0x00000000062aea81 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x62aea81)
#13 0x00000000061c9b41 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c9b41)
#14 0x00000000062011bc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x62011bc)
#15 0x00000000061bdade clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61bdade)
#16 0x00000000061be2d3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61be2d3)
#17 0x00000000061c5746 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c5746)
#18 0x00000000061c65ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c65ed)
#19 0x00000000061c6ac0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c6ac0)
#20 0x00000000061b929c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61b929c)
#21 0x00000000041bd968 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41bd968)
#22 0x000000000443cfe9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x443cfe9)
#23 0x00000000043c66de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x43c66de)
#24 0x000000000452009e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x452009e)
#25 0x0000000000c29b5c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc29b5c)
#26 0x0000000000c22dea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000042008e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003865704 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3865704)
#29 0x0000000004200edf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x00000000041c7275 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c7275)
#31 0x00000000041c7cdd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c7cdd)
#32 0x00000000041cfa45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41cfa45)
#33 0x0000000000c26ff5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc26ff5)
#34 0x0000000000b1d474 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb1d474)
#35 0x00007f4897829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007f4897829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000c228de _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc228de)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
Compiler Explorer: https://gcc.godbolt.org/z/bhnczdK3h

The reproducer:
```
void
foo (void)
{
  int a[1024] = {0};
  int *p = 0, *q = 0;
  #pragma omp parallel for collapse(2)
  for (p = &amp;a[0]; p &lt; &amp;a[512]; p += 3)
    for (q = p + 64; q &lt; p + 128; q++)
      ;
}
```

The stack backtrace:
```
clang: /root/llvm-project/clang/lib/Sema/SemaDecl.cpp:2189: void clang::Sema::DiagnoseUnusedButSetDecl(const clang::VarDecl*, clang::Sema::DiagReceiverTy): Assertion `iter-&gt;getSecond() &gt;= 0 &amp;&amp; "Found a negative number of references to a VarDecl"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:3:1: parsing function body 'foo'
 #<!-- -->0 0x0000000003920cd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3920cd8)
 #<!-- -->1 0x000000000391e9bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x391e9bc)
 #<!-- -->2 0x0000000003865258 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f4897842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f48978969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f4897842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f48978287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f489782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f4897839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000064ed80a clang::Sema::DiagnoseUnusedButSetDecl(clang::VarDecl const*, llvm::function_ref&lt;void (clang::SourceLocation, clang::PartialDiagnostic)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64ed80a)
#<!-- -->10 0x00000000065258a5 clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65258a5)
#<!-- -->11 0x00000000061bbe25 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61bbe25)
#<!-- -->12 0x00000000062aea81 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x62aea81)
#<!-- -->13 0x00000000061c9b41 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c9b41)
#<!-- -->14 0x00000000062011bc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x62011bc)
#<!-- -->15 0x00000000061bdade clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61bdade)
#<!-- -->16 0x00000000061be2d3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61be2d3)
#<!-- -->17 0x00000000061c5746 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c5746)
#<!-- -->18 0x00000000061c65ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c65ed)
#<!-- -->19 0x00000000061c6ac0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61c6ac0)
#<!-- -->20 0x00000000061b929c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x61b929c)
#<!-- -->21 0x00000000041bd968 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41bd968)
#<!-- -->22 0x000000000443cfe9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x443cfe9)
#<!-- -->23 0x00000000043c66de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x43c66de)
#<!-- -->24 0x000000000452009e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x452009e)
#<!-- -->25 0x0000000000c29b5c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc29b5c)
#<!-- -->26 0x0000000000c22dea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000042008e9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003865704 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3865704)
#<!-- -->29 0x0000000004200edf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x00000000041c7275 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c7275)
#<!-- -->31 0x00000000041c7cdd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c7cdd)
#<!-- -->32 0x00000000041cfa45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41cfa45)
#<!-- -->33 0x0000000000c26ff5 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc26ff5)
#<!-- -->34 0x0000000000b1d474 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb1d474)
#<!-- -->35 0x00007f4897829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007f4897829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000c228de _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc228de)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - komalverma04

Can i start working on this?

---

### Comment 3 - ht-cabbagedog

> Can i start working on this?

Have you got any results on this problem? I'm also solving it.

---

