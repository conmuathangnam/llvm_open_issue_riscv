# [clang][bytecode] Assertion `E->getNumSubExprs() > 2' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/158471
**Status:** Closed
**Labels:** clang:frontend, crash, clang:bytecode
**Closed Date:** 2025-09-16T05:58:16Z

## Body

Reproducer:
https://godbolt.org/z/Kvvr9j167
```cpp
typedef int vi6 __attribute__((ext_vector_type(6)));

void foo() {
  vi6 a;
  vi6 b;
  vi6 r = __builtin_shufflevector(a, b);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Compiler.cpp:3990: bool clang::interp::Compiler<Emitter>::VisitShuffleVectorExpr(const clang::ShuffleVectorExpr*) [with Emitter = clang::interp::EvalEmitter]: Assertion `E->getNumSubExprs() > 2' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter --analyze <source>
1.	<eof> parser at end of file
 #0 0x0000000004177998 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4177998)
 #1 0x0000000004174dc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174dc4)
 #2 0x00000000040b9368 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d9f1a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d9f1a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d9f1a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d9f1a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d9f1a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d9f1a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007f5e971 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitShuffleVectorExpr(clang::ShuffleVectorExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f5e971)
#10 0x0000000007f7ca32 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f7ca32)
#11 0x0000000007f7dea5 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f7dea5)
#12 0x0000000007b1edfd clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b1edfd)
#13 0x0000000007aed243 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7aed243)
#14 0x0000000007a63e23 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x0000000007a66ef0 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a66ef0)
#16 0x0000000007a670c1 clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a670c1)
#17 0x0000000006321c4d (anonymous namespace)::DeadStoreObs::observeStmt(clang::Stmt const*, clang::CFGBlock const*, clang::LiveVariables::LivenessValues const&) DeadStoresChecker.cpp:0:0
#18 0x000000000755724a (anonymous namespace)::TransferFunctions::Visit(clang::Stmt*) LiveVariables.cpp:0:0
#19 0x0000000007557b34 (anonymous namespace)::LiveVariablesImpl::runOnBlock(clang::CFGBlock const*, clang::LiveVariables::LivenessValues, clang::LiveVariables::Observer*) LiveVariables.cpp:0:0
#20 0x0000000007557cc2 clang::LiveVariables::runOnAllBlocks(clang::LiveVariables::Observer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7557cc2)
#21 0x000000000631de48 (anonymous namespace)::DeadStoresChecker::checkASTCodeBody(clang::Decl const*, clang::ento::AnalysisManager&, clang::ento::BugReporter&) const DeadStoresChecker.cpp:0:0
#22 0x000000000669a8df clang::ento::CheckerManager::runCheckersOnASTBody(clang::Decl const*, clang::ento::AnalysisManager&, clang::ento::BugReporter&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669a8df)
#23 0x000000000627a0c6 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) AnalysisConsumer.cpp:0:0
#24 0x000000000627aeec non-virtual thunk to (anonymous namespace)::AnalysisConsumer::VisitFunctionDecl(clang::FunctionDecl*) AnalysisConsumer.cpp:0:0
#25 0x0000000007985945 clang::DynamicRecursiveASTVisitorBase<false>::TraverseFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7985945)
#26 0x000000000627b429 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#27 0x000000000683354c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x683354c)
#28 0x0000000004dab495 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dab495)
#29 0x0000000004d2682e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2682e)
#30 0x0000000004ea0281 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea0281)
#31 0x0000000000db5abf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5abf)
#32 0x0000000000dac72a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000004b1a3c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000040b9804 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40b9804)
#35 0x0000000004b1a9df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004adb5bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adb5bd)
#37 0x0000000004adc64e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adc64e)
#38 0x0000000004ae45e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae45e5)
#39 0x0000000000db1f45 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1f45)
#40 0x0000000000c63e84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63e84)
#41 0x00007d9f1a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007d9f1a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dac1d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac1d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

To reproduce this crash, the `--analyze` option is required in addition to the `-fexperimental-new-constant-interpreter` option.

---

### Comment 2 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/az7T85e9G

---

### Comment 3 - tbaederr

This also crashes in the current interpreter: https://godbolt.org/z/Pnz8a4815

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Kvvr9j167
```cpp
typedef int vi6 __attribute__((ext_vector_type(6)));

void foo() {
  vi6 a;
  vi6 b;
  vi6 r = __builtin_shufflevector(a, b);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Compiler.cpp:3990: bool clang::interp::Compiler&lt;Emitter&gt;::VisitShuffleVectorExpr(const clang::ShuffleVectorExpr*) [with Emitter = clang::interp::EvalEmitter]: Assertion `E-&gt;getNumSubExprs() &gt; 2' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter --analyze &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004177998 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4177998)
 #<!-- -->1 0x0000000004174dc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174dc4)
 #<!-- -->2 0x00000000040b9368 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d9f1a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d9f1a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d9f1a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d9f1a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007d9f1a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007d9f1a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007f5e971 clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::VisitShuffleVectorExpr(clang::ShuffleVectorExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f5e971)
#<!-- -->10 0x0000000007f7ca32 clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f7ca32)
#<!-- -->11 0x0000000007f7dea5 clang::interp::Compiler&lt;clang::interp::EvalEmitter&gt;::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f7dea5)
#<!-- -->12 0x0000000007b1edfd clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b1edfd)
#<!-- -->13 0x0000000007aed243 clang::interp::Context::evaluateAsRValue(clang::interp::State&amp;, clang::Expr const*, clang::APValue&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7aed243)
#<!-- -->14 0x0000000007a63e23 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007a66ef0 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a66ef0)
#<!-- -->16 0x0000000007a670c1 clang::Expr::isEvaluatable(clang::ASTContext const&amp;, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a670c1)
#<!-- -->17 0x0000000006321c4d (anonymous namespace)::DeadStoreObs::observeStmt(clang::Stmt const*, clang::CFGBlock const*, clang::LiveVariables::LivenessValues const&amp;) DeadStoresChecker.cpp:0:0
#<!-- -->18 0x000000000755724a (anonymous namespace)::TransferFunctions::Visit(clang::Stmt*) LiveVariables.cpp:0:0
#<!-- -->19 0x0000000007557b34 (anonymous namespace)::LiveVariablesImpl::runOnBlock(clang::CFGBlock const*, clang::LiveVariables::LivenessValues, clang::LiveVariables::Observer*) LiveVariables.cpp:0:0
#<!-- -->20 0x0000000007557cc2 clang::LiveVariables::runOnAllBlocks(clang::LiveVariables::Observer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7557cc2)
#<!-- -->21 0x000000000631de48 (anonymous namespace)::DeadStoresChecker::checkASTCodeBody(clang::Decl const*, clang::ento::AnalysisManager&amp;, clang::ento::BugReporter&amp;) const DeadStoresChecker.cpp:0:0
#<!-- -->22 0x000000000669a8df clang::ento::CheckerManager::runCheckersOnASTBody(clang::Decl const*, clang::ento::AnalysisManager&amp;, clang::ento::BugReporter&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669a8df)
#<!-- -->23 0x000000000627a0c6 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) AnalysisConsumer.cpp:0:0
#<!-- -->24 0x000000000627aeec non-virtual thunk to (anonymous namespace)::AnalysisConsumer::VisitFunctionDecl(clang::FunctionDecl*) AnalysisConsumer.cpp:0:0
#<!-- -->25 0x0000000007985945 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7985945)
#<!-- -->26 0x000000000627b429 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->27 0x000000000683354c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x683354c)
#<!-- -->28 0x0000000004dab495 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dab495)
#<!-- -->29 0x0000000004d2682e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2682e)
#<!-- -->30 0x0000000004ea0281 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea0281)
#<!-- -->31 0x0000000000db5abf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5abf)
#<!-- -->32 0x0000000000dac72a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004b1a3c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000040b9804 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40b9804)
#<!-- -->35 0x0000000004b1a9df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004adb5bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adb5bd)
#<!-- -->37 0x0000000004adc64e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adc64e)
#<!-- -->38 0x0000000004ae45e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae45e5)
#<!-- -->39 0x0000000000db1f45 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1f45)
#<!-- -->40 0x0000000000c63e84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63e84)
#<!-- -->41 0x00007d9f1a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007d9f1a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dac1d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac1d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

