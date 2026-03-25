# [clang][OpenMP] Assertion `isAllowedClauseForDirective(DKind, CKind, OpenMPVersion) && "Invalid directive with CKind-clause"' failed

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/156232
**Status:** Closed
**Labels:** clang:frontend, regression, crash, clang:openmp
**Closed Date:** 2025-09-02T21:09:57Z

## Body

Reproducer:
https://godbolt.org/z/sv6o9EvTG
```cpp
[[omp::directive(error severity(warning)message("my message")at(compilation))]];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenMP.cpp:15672: clang::OpenMPDirectiveKind getOpenMPCaptureRegionForClause(clang::OpenMPDirectiveKind, clang::OpenMPClauseKind, unsigned int, clang::OpenMPDirectiveKind): Assertion `isAllowedClauseForDirective(DKind, CKind, OpenMPVersion) && "Invalid directive with CKind-clause"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:1:62: current parser token 'at'
 #0 0x000000000404bc38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x404bc38)
 #1 0x0000000004049064 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4049064)
 #2 0x0000000003f8d9f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000754314242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007543142969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000754314242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007543142287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075431422871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000754314239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e2bf40 bool llvm::function_ref<bool (clang::Expr*&, bool)>::callback_fn<clang::SemaOpenMP::checkOpenMPDeclareVariantFunction(clang::OpaquePtr<clang::DeclGroupRef>, clang::Expr*, clang::OMPTraitInfo&, unsigned int, clang::SourceRange)::'lambda0'(clang::Expr*&, bool)>(long, clang::Expr*&, bool) SemaOpenMP.cpp:0:0
#10 0x0000000006e97d5d clang::SemaOpenMP::ActOnOpenMPMessageClause(clang::Expr*, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e97d5d)
#11 0x000000000677ab75 clang::Parser::ParseOpenMPSingleExprClause(llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677ab75)
#12 0x00000000067820df clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67820df)
#13 0x0000000006783728 clang::Parser::ParseOpenMPClauses(llvm::omp::Directive, llvm::SmallVectorImpl<clang::OMPClause*>&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6783728)
#14 0x00000000067926c5 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&, clang::ParsedAttributes&, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#15 0x00000000066bfac3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bfac3)
#16 0x00000000066c0965 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c0965)
#17 0x00000000066c0e20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c0e20)
#18 0x00000000066a0893 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a0893)
#19 0x0000000004975908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975908)
#20 0x0000000004c6e255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6e255)
#21 0x0000000004be988e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be988e)
#22 0x0000000004d62431 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d62431)
#23 0x0000000000db177f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb177f)
#24 0x0000000000da842a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00000000049dda49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003f8de94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8de94)
#27 0x00000000049de05f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x000000000499f9fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499f9fd)
#29 0x00000000049a0a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a0a8e)
#30 0x00000000049a8a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a8a25)
#31 0x0000000000dadc35 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdadc35)
#32 0x0000000000c61bb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61bb4)
#33 0x0000754314229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x0000754314229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000da7ed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7ed5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/sv6o9EvTG
```cpp
[[omp::directive(error severity(warning)message("my message")at(compilation))]];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenMP.cpp:15672: clang::OpenMPDirectiveKind getOpenMPCaptureRegionForClause(clang::OpenMPDirectiveKind, clang::OpenMPClauseKind, unsigned int, clang::OpenMPDirectiveKind): Assertion `isAllowedClauseForDirective(DKind, CKind, OpenMPVersion) &amp;&amp; "Invalid directive with CKind-clause"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;source&gt;:1:62: current parser token 'at'
 #<!-- -->0 0x000000000404bc38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x404bc38)
 #<!-- -->1 0x0000000004049064 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4049064)
 #<!-- -->2 0x0000000003f8d9f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000754314242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007543142969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000754314242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007543142287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075431422871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000754314239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006e2bf40 bool llvm::function_ref&lt;bool (clang::Expr*&amp;, bool)&gt;::callback_fn&lt;clang::SemaOpenMP::checkOpenMPDeclareVariantFunction(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;, clang::Expr*, clang::OMPTraitInfo&amp;, unsigned int, clang::SourceRange)::'lambda0'(clang::Expr*&amp;, bool)&gt;(long, clang::Expr*&amp;, bool) SemaOpenMP.cpp:0:0
#<!-- -->10 0x0000000006e97d5d clang::SemaOpenMP::ActOnOpenMPMessageClause(clang::Expr*, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e97d5d)
#<!-- -->11 0x000000000677ab75 clang::Parser::ParseOpenMPSingleExprClause(llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677ab75)
#<!-- -->12 0x00000000067820df clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67820df)
#<!-- -->13 0x0000000006783728 clang::Parser::ParseOpenMPClauses(llvm::omp::Directive, llvm::SmallVectorImpl&lt;clang::OMPClause*&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6783728)
#<!-- -->14 0x00000000067926c5 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#<!-- -->15 0x00000000066bfac3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bfac3)
#<!-- -->16 0x00000000066c0965 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c0965)
#<!-- -->17 0x00000000066c0e20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c0e20)
#<!-- -->18 0x00000000066a0893 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a0893)
#<!-- -->19 0x0000000004975908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975908)
#<!-- -->20 0x0000000004c6e255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6e255)
#<!-- -->21 0x0000000004be988e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be988e)
#<!-- -->22 0x0000000004d62431 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d62431)
#<!-- -->23 0x0000000000db177f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb177f)
#<!-- -->24 0x0000000000da842a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->25 0x00000000049dda49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003f8de94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8de94)
#<!-- -->27 0x00000000049de05f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x000000000499f9fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499f9fd)
#<!-- -->29 0x00000000049a0a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a0a8e)
#<!-- -->30 0x00000000049a8a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a8a25)
#<!-- -->31 0x0000000000dadc35 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdadc35)
#<!-- -->32 0x0000000000c61bb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc61bb4)
#<!-- -->33 0x0000754314229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x0000754314229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000da7ed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7ed5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

This is trunk only:
https://godbolt.org/z/sKdKEMoeW

---

### Comment 3 - shafik

@ro-i this regression was caused by c94b5f0c0c5edb688ccdbeb0038865daa376d4cc

---

