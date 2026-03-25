# crash on __builtin_snprintf with `-Weverything`

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169629
**Status:** Closed
**Labels:** clang:frontend, clang:diagnostics, confirmed, crash, regression:21
**Closed Date:** 2025-12-17T17:56:40Z

## Body

Reproducer:
https://godbolt.org/z/qqYfvdPT7
```cpp
void foo(void) { int n = __builtin_snprintf(0, 0, "%"); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Basic/SourceManager.cpp:886: clang::FileID clang::SourceManager::getFileIDLoaded(clang::SourceLocation::UIntTy) const: Assertion `0 && "Invalid SLocOffset or bad function choice"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Weverything <source>
1.	<eof> parser at end of file
 #0 0x00000000042589c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42589c8)
 #1 0x0000000004255df4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4255df4)
 #2 0x0000000004199b28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000727b3a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000727b3a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000727b3a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000727b3a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000727b3a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000727b3a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000044b6123 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b6123)
#10 0x00000000044b80df (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b80df)
#11 0x00000000044bde0a clang::SourceManager::getDecomposedLoc(clang::SourceLocation) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44bde0a)
#12 0x00000000044c1342 clang::SourceManager::getPresumedLoc(clang::SourceLocation, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44c1342)
#13 0x00000000044b4281 clang::FullSourceLoc::getPresumedLoc(bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b4281)
#14 0x0000000004f74205 clang::DiagnosticRenderer::emitDiagnostic(clang::FullSourceLoc, clang::DiagnosticsEngine::Level, llvm::StringRef, llvm::ArrayRef<clang::CharSourceRange>, llvm::ArrayRef<clang::FixItHint>, llvm::PointerUnion<clang::Diagnostic const*, clang::StoredDiagnostic const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f74205)
#15 0x0000000004efd0c5 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4efd0c5)
#16 0x0000000004495030 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495030)
#17 0x0000000004495745 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495745)
#18 0x0000000004495825 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495825)
#19 0x0000000006ad64e6 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad64e6)
#20 0x0000000006b4bbc2 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4bbc2)
#21 0x0000000006ac2f98 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac2f98)
#22 0x00000000075f0396 (anonymous namespace)::UnsafeBufferUsageReporter::handleUnsafeLibcCall(clang::CallExpr const*, unsigned int, clang::ASTContext&, clang::Expr const*) AnalysisBasedWarnings.cpp:0:0
#23 0x00000000077b6439 applyGadgets(clang::Decl const*, std::vector<std::unique_ptr<(anonymous namespace)::FixableGadget, std::default_delete<(anonymous namespace)::FixableGadget>>, std::allocator<std::unique_ptr<(anonymous namespace)::FixableGadget, std::default_delete<(anonymous namespace)::FixableGadget>>>>, std::vector<std::unique_ptr<(anonymous namespace)::WarningGadget, std::default_delete<(anonymous namespace)::WarningGadget>>, std::allocator<std::unique_ptr<(anonymous namespace)::WarningGadget, std::default_delete<(anonymous namespace)::WarningGadget>>>>, (anonymous namespace)::DeclUseTracker, clang::UnsafeBufferUsageHandler&, bool) UnsafeBufferUsage.cpp:0:0
#24 0x00000000077b9d23 clang::checkUnsafeBufferUsage(clang::Decl const*, clang::UnsafeBufferUsageHandler&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77b9d23)
#25 0x00000000075dac69 CallableVisitor::VisitFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75dac69)
#26 0x0000000007b17bd5 clang::DynamicRecursiveASTVisitorBase<false>::TraverseFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b17bd5)
#27 0x0000000007acd6dc clang::RecursiveASTVisitor<(anonymous namespace)::Impl<false>>::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#28 0x0000000007ae93ec clang::DynamicRecursiveASTVisitorBase<false>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ae93ec)
#29 0x00000000075e125e clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e125e)
#30 0x0000000006ae7ec6 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae7ec6)
#31 0x0000000006951345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6951345)
#32 0x000000000693285a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693285a)
#33 0x0000000004ba7698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba7698)
#34 0x0000000004e94c45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e94c45)
#35 0x0000000004e14cce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e14cce)
#36 0x0000000004f8cf9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8cf9d)
#37 0x0000000000dc6d10 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6d10)
#38 0x0000000000dbd78a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#39 0x0000000000dbd90d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#40 0x0000000004c0fbb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000004199fc4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4199fc4)
#42 0x0000000004c101cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004bd0c92 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd0c92)
#44 0x0000000004bd1c3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd1c3e)
#45 0x0000000004bd9205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd9205)
#46 0x0000000000dc3131 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3131)
#47 0x0000000000c71b04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc71b04)
#48 0x0000727b3a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x0000727b3a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000dbd225 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd225)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/qqYfvdPT7
```cpp
void foo(void) { int n = __builtin_snprintf(0, 0, "%"); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Basic/SourceManager.cpp:886: clang::FileID clang::SourceManager::getFileIDLoaded(clang::SourceLocation::UIntTy) const: Assertion `0 &amp;&amp; "Invalid SLocOffset or bad function choice"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Weverything &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00000000042589c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42589c8)
 #<!-- -->1 0x0000000004255df4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4255df4)
 #<!-- -->2 0x0000000004199b28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000727b3a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000727b3a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000727b3a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000727b3a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000727b3a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000727b3a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000044b6123 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b6123)
#<!-- -->10 0x00000000044b80df (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b80df)
#<!-- -->11 0x00000000044bde0a clang::SourceManager::getDecomposedLoc(clang::SourceLocation) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44bde0a)
#<!-- -->12 0x00000000044c1342 clang::SourceManager::getPresumedLoc(clang::SourceLocation, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44c1342)
#<!-- -->13 0x00000000044b4281 clang::FullSourceLoc::getPresumedLoc(bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b4281)
#<!-- -->14 0x0000000004f74205 clang::DiagnosticRenderer::emitDiagnostic(clang::FullSourceLoc, clang::DiagnosticsEngine::Level, llvm::StringRef, llvm::ArrayRef&lt;clang::CharSourceRange&gt;, llvm::ArrayRef&lt;clang::FixItHint&gt;, llvm::PointerUnion&lt;clang::Diagnostic const*, clang::StoredDiagnostic const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f74205)
#<!-- -->15 0x0000000004efd0c5 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4efd0c5)
#<!-- -->16 0x0000000004495030 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495030)
#<!-- -->17 0x0000000004495745 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495745)
#<!-- -->18 0x0000000004495825 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4495825)
#<!-- -->19 0x0000000006ad64e6 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad64e6)
#<!-- -->20 0x0000000006b4bbc2 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4bbc2)
#<!-- -->21 0x0000000006ac2f98 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac2f98)
#<!-- -->22 0x00000000075f0396 (anonymous namespace)::UnsafeBufferUsageReporter::handleUnsafeLibcCall(clang::CallExpr const*, unsigned int, clang::ASTContext&amp;, clang::Expr const*) AnalysisBasedWarnings.cpp:0:0
#<!-- -->23 0x00000000077b6439 applyGadgets(clang::Decl const*, std::vector&lt;std::unique_ptr&lt;(anonymous namespace)::FixableGadget, std::default_delete&lt;(anonymous namespace)::FixableGadget&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;(anonymous namespace)::FixableGadget, std::default_delete&lt;(anonymous namespace)::FixableGadget&gt;&gt;&gt;&gt;, std::vector&lt;std::unique_ptr&lt;(anonymous namespace)::WarningGadget, std::default_delete&lt;(anonymous namespace)::WarningGadget&gt;&gt;, std::allocator&lt;std::unique_ptr&lt;(anonymous namespace)::WarningGadget, std::default_delete&lt;(anonymous namespace)::WarningGadget&gt;&gt;&gt;&gt;, (anonymous namespace)::DeclUseTracker, clang::UnsafeBufferUsageHandler&amp;, bool) UnsafeBufferUsage.cpp:0:0
#<!-- -->24 0x00000000077b9d23 clang::checkUnsafeBufferUsage(clang::Decl const*, clang::UnsafeBufferUsageHandler&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77b9d23)
#<!-- -->25 0x00000000075dac69 CallableVisitor::VisitFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75dac69)
#<!-- -->26 0x0000000007b17bd5 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b17bd5)
#<!-- -->27 0x0000000007acd6dc clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&lt;false&gt;&gt;::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#<!-- -->28 0x0000000007ae93ec clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ae93ec)
#<!-- -->29 0x00000000075e125e clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e125e)
#<!-- -->30 0x0000000006ae7ec6 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae7ec6)
#<!-- -->31 0x0000000006951345 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6951345)
#<!-- -->32 0x000000000693285a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693285a)
#<!-- -->33 0x0000000004ba7698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba7698)
#<!-- -->34 0x0000000004e94c45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e94c45)
#<!-- -->35 0x0000000004e14cce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e14cce)
#<!-- -->36 0x0000000004f8cf9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8cf9d)
#<!-- -->37 0x0000000000dc6d10 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6d10)
#<!-- -->38 0x0000000000dbd78a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->39 0x0000000000dbd90d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004c0fbb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000004199fc4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4199fc4)
#<!-- -->42 0x0000000004c101cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004bd0c92 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd0c92)
#<!-- -->44 0x0000000004bd1c3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd1c3e)
#<!-- -->45 0x0000000004bd9205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd9205)
#<!-- -->46 0x0000000000dc3131 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3131)
#<!-- -->47 0x0000000000c71b04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc71b04)
#<!-- -->48 0x0000727b3a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x0000727b3a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000dbd225 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd225)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/Yo7W31M7M

---

### Comment 3 - philnik777

Looks like this is specific to `-Wunsafe-buffer-usage-in-libc-call`: https://godbolt.org/z/PMzeTYPs4

---

### Comment 4 - shafik

clang-21 regression: https://godbolt.org/z/TnM3Mav4v

---

### Comment 5 - AaronBallman

I poked at this in the debugger a bit and what's happening is that `hasUnsafeFormatOrSArg()` is returning `true` but not assigning a value into `UnsafeArg`; that in turns ends up doing bad things when we create the `DynTypedNode` from it: https://github.com/llvm/llvm-project/blob/bf81bdec660c29a737f6fcfd8fdeefe6fa1f336c/clang/lib/Analysis/UnsafeBufferUsage.cpp#L1184

As I poked around, it seems there are a lot of ways that call can return true without setting the argument. I made some local changes like this:
```
diff --git a/clang/lib/Analysis/UnsafeBufferUsage.cpp b/clang/lib/Analysis/UnsafeBufferUsage.cpp
index da155d31d4a8..a875fec29b78 100644
--- a/clang/lib/Analysis/UnsafeBufferUsage.cpp
+++ b/clang/lib/Analysis/UnsafeBufferUsage.cpp
@@ -1150,9 +1150,9 @@ static bool hasUnsafePrintfStringArg(const CallExpr &Node, ASTContext &Ctx,
            .isNull() && //`FILE *` must be in the context if it is fprintf
       FirstPteTy.getCanonicalType() == Ctx.getFILEType().getCanonicalType()) {
     // It is a fprintf:
-    const Expr *UnsafeArg;
+    const Expr *UnsafeArg = nullptr;
 
-    if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 1, Ctx, false)) {
+    if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 1, Ctx, false) && UnsafeArg) {
       Result.addNode(Tag, DynTypedNode::create(*UnsafeArg));
       return true;
     }
@@ -1162,11 +1162,12 @@ static bool hasUnsafePrintfStringArg(const CallExpr &Node, ASTContext &Ctx,
   if (FirstPteTy.isConstQualified()) {
     // If the first parameter is a `const char *`, it is a printf/kprintf:
     bool isKprintf = false;
-    const Expr *UnsafeArg;
+    const Expr *UnsafeArg = nullptr;
 
     if (auto *II = FD->getIdentifier())
       isKprintf = II->getName() == "kprintf";
-    if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 0, Ctx, isKprintf)) {
+    if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 0, Ctx, isKprintf) &&
+        UnsafeArg) {
       Result.addNode(Tag, DynTypedNode::create(*UnsafeArg));
       return true;
     }
@@ -1179,9 +1180,9 @@ static bool hasUnsafePrintfStringArg(const CallExpr &Node, ASTContext &Ctx,
     if (!FirstPteTy.isConstQualified() && SecondParmTy->isIntegerType()) {
       // If the first parameter type is non-const qualified `char *` and the
       // second is an integer, it is a snprintf:
-      const Expr *UnsafeArg;
+      const Expr *UnsafeArg = nullptr;
 
-      if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 2, Ctx, false)) {
+      if (hasUnsafeFormatOrSArg(&Node, UnsafeArg, 2, Ctx, false) && UnsafeArg) {
         Result.addNode(Tag, DynTypedNode::create(*UnsafeArg));
         return true;
       }
```
which caused the crash in this reproducer to go away. I did not check what it did to the rest of the lit tests though because I wasn't certain whether this was the correct approach for a fix.

CC @ziqingluo-90 @ivanaivanovska as folks who have been in this area recently for some more opinions.

---

### Comment 6 - ziqingluo-90

The only way `hasUnsafeFormatOrSArg` could return true without setting `UnsafeArg` is when the format parser encounters an invalid specifier.  [This](https://github.com/llvm/llvm-project/pull/172561) should fix the issue.

---

### Comment 7 - ziqingluo-90

ooh, I didn't see an earlier fix PR existed!

---

