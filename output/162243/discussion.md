# [clang][OpenMP] crash on omp groupprivate

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/162243

## Body

Reproducer:
https://godbolt.org/z/GqhbGjGeo
```cpp
#pragma omp groupprivate(a)
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Basic/Diagnostic.cpp:1258: void clang::Diagnostic::FormatDiagnostic(const char*, const char*, llvm::SmallVectorImpl<char>&) const: Assertion `ModifierLen == 0 && "unknown modifier for string"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<eof> parser at end of file
 #0 0x00000000041d20a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d20a8)
 #1 0x00000000041cf4d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41cf4d4)
 #2 0x0000000004113668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b6f17442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b6f174969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b6f17442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b6f174287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b6f1742871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b6f17439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000440b6a4 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440b6a4)
#10 0x0000000004e6813a clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6813a)
#11 0x0000000004409090 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4409090)
#12 0x00000000044097a5 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44097a5)
#13 0x0000000004409885 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4409885)
#14 0x0000000006a49a84 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a49a84)
#15 0x0000000006ac1c02 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac1c02)
#16 0x0000000006a36478 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a36478)
#17 0x00000000070b187a clang::SemaOpenMP::ActOnOpenMPGroupPrivateDirective(clang::SourceLocation, llvm::ArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b187a)
#18 0x0000000006996439 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&, clang::ParsedAttributes&, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#19 0x00000000068c2938 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c2938)
#20 0x00000000068c37c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c37c5)
#21 0x00000000068c3c80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c3c80)
#22 0x00000000068a4df3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a4df3)
#23 0x0000000004b14728 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14728)
#24 0x0000000004e00195 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e00195)
#25 0x0000000004d81dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81dee)
#26 0x0000000004ef748d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef748d)
#27 0x0000000000dbc110 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc110)
#28 0x0000000000db2c7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000db2dfd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004b7d8d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000004113b04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113b04)
#32 0x0000000004b7deef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004b3f1f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3f1f2)
#34 0x0000000004b4019e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4019e)
#35 0x0000000004b478d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b478d5)
#36 0x0000000000db8639 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8639)
#37 0x0000000000c6a194 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6a194)
#38 0x00007b6f17429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007b6f17429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000db2715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2715)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/GqhbGjGeo
```cpp
#pragma omp groupprivate(a)
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Basic/Diagnostic.cpp:1258: void clang::Diagnostic::FormatDiagnostic(const char*, const char*, llvm::SmallVectorImpl&lt;char&gt;&amp;) const: Assertion `ModifierLen == 0 &amp;&amp; "unknown modifier for string"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00000000041d20a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d20a8)
 #<!-- -->1 0x00000000041cf4d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41cf4d4)
 #<!-- -->2 0x0000000004113668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b6f17442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b6f174969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b6f17442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b6f174287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b6f1742871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b6f17439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000440b6a4 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl&lt;char&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440b6a4)
#<!-- -->10 0x0000000004e6813a clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6813a)
#<!-- -->11 0x0000000004409090 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4409090)
#<!-- -->12 0x00000000044097a5 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44097a5)
#<!-- -->13 0x0000000004409885 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4409885)
#<!-- -->14 0x0000000006a49a84 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a49a84)
#<!-- -->15 0x0000000006ac1c02 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac1c02)
#<!-- -->16 0x0000000006a36478 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a36478)
#<!-- -->17 0x00000000070b187a clang::SemaOpenMP::ActOnOpenMPGroupPrivateDirective(clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b187a)
#<!-- -->18 0x0000000006996439 clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#<!-- -->19 0x00000000068c2938 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c2938)
#<!-- -->20 0x00000000068c37c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c37c5)
#<!-- -->21 0x00000000068c3c80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c3c80)
#<!-- -->22 0x00000000068a4df3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a4df3)
#<!-- -->23 0x0000000004b14728 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14728)
#<!-- -->24 0x0000000004e00195 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e00195)
#<!-- -->25 0x0000000004d81dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81dee)
#<!-- -->26 0x0000000004ef748d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef748d)
#<!-- -->27 0x0000000000dbc110 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc110)
#<!-- -->28 0x0000000000db2c7a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000db2dfd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004b7d8d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000004113b04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113b04)
#<!-- -->32 0x0000000004b7deef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004b3f1f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3f1f2)
#<!-- -->34 0x0000000004b4019e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4019e)
#<!-- -->35 0x0000000004b478d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b478d5)
#<!-- -->36 0x0000000000db8639 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8639)
#<!-- -->37 0x0000000000c6a194 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6a194)
#<!-- -->38 0x00007b6f17429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007b6f17429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000db2715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2715)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

Looks like support for this was brought in via: https://github.com/llvm/llvm-project/pull/158134

CC @Ritanya-B-Bharadwaj @alexey-bataev 

---

### Comment 3 - Ritanya-B-Bharadwaj

Raised a PR to fix this issue - https://github.com/llvm/llvm-project/pull/165562

---

