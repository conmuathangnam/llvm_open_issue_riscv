# [clang] Assertion `F.isPresplitCoroutine() && "The frontend uses Switch-Resumed ABI should emit " "\"presplitcoroutine\" attribute for the coroutine."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161078
**Status:** Open
**Labels:** crash, coroutines

## Body

Reproducer:
https://godbolt.org/z/TM94jcnfo
```cpp
void foo() { __builtin_coro_id(32, 0, 0, 0); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Transforms/Coroutines/CoroEarly.cpp:239: void {anonymous}::Lowerer::lowerEarlyIntrinsics(llvm::Function&): Assertion `F.isPresplitCoroutine() && "The frontend uses Switch-Resumed ABI should emit " "\"presplitcoroutine\" attribute for the coroutine."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "coro-cond(coro-early,cgscc(coro-split),coro-cleanup,globaldce)" on module "<source>"
4.	Running pass "coro-early" on module "<source>"
 #0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072089a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072089a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072089a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072089a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072089a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072089a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000059bda54 (anonymous namespace)::Lowerer::lowerEarlyIntrinsics(llvm::Function&) CoroEarly.cpp:0:0
#10 0x00000000059bdc7f llvm::CoroEarlyPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59bdc7f)
#11 0x00000000053837ee llvm::detail::PassModel<llvm::Module, llvm::CoroEarlyPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x53837ee)
#12 0x0000000003b0cd91 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0cd91)
#13 0x00000000059b9d91 llvm::CoroConditionalWrapper::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59b9d91)
#14 0x0000000005382ebe llvm::detail::PassModel<llvm::Module, llvm::CoroConditionalWrapper, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5382ebe)
#15 0x0000000003b0cd91 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0cd91)
#16 0x000000000446417b (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#17 0x0000000004467781 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4467781)
#18 0x0000000004aec915 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aec915)
#19 0x000000000687864c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687864c)
#20 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#21 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#22 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#23 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#24 0x0000000000db8ce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#25 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000daf9cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004b55ea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#29 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#31 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#32 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#33 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#34 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#35 0x000072089a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x000072089a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-coroutines

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/TM94jcnfo
```cpp
void foo() { __builtin_coro_id(32, 0, 0, 0); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Transforms/Coroutines/CoroEarly.cpp:239: void {anonymous}::Lowerer::lowerEarlyIntrinsics(llvm::Function&amp;): Assertion `F.isPresplitCoroutine() &amp;&amp; "The frontend uses Switch-Resumed ABI should emit " "\"presplitcoroutine\" attribute for the coroutine."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "coro-cond(coro-early,cgscc(coro-split),coro-cleanup,globaldce)" on module "&lt;source&gt;"
4.	Running pass "coro-early" on module "&lt;source&gt;"
 #<!-- -->0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #<!-- -->1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #<!-- -->2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072089a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072089a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072089a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072089a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072089a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072089a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000059bda54 (anonymous namespace)::Lowerer::lowerEarlyIntrinsics(llvm::Function&amp;) CoroEarly.cpp:0:0
#<!-- -->10 0x00000000059bdc7f llvm::CoroEarlyPass::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59bdc7f)
#<!-- -->11 0x00000000053837ee llvm::detail::PassModel&lt;llvm::Module, llvm::CoroEarlyPass, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x53837ee)
#<!-- -->12 0x0000000003b0cd91 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0cd91)
#<!-- -->13 0x00000000059b9d91 llvm::CoroConditionalWrapper::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59b9d91)
#<!-- -->14 0x0000000005382ebe llvm::detail::PassModel&lt;llvm::Module, llvm::CoroConditionalWrapper, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5382ebe)
#<!-- -->15 0x0000000003b0cd91 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0cd91)
#<!-- -->16 0x000000000446417b (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->17 0x0000000004467781 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4467781)
#<!-- -->18 0x0000000004aec915 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aec915)
#<!-- -->19 0x000000000687864c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687864c)
#<!-- -->20 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#<!-- -->21 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#<!-- -->22 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#<!-- -->23 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#<!-- -->24 0x0000000000db8ce0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#<!-- -->25 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000daf9cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004b55ea9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#<!-- -->29 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#<!-- -->31 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#<!-- -->32 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#<!-- -->33 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#<!-- -->34 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#<!-- -->35 0x000072089a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x000072089a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-14.0.0:
https://godbolt.org/z/Pxzs7d8hs

---

### Comment 3 - k-arrows

may be related to the following issue:
https://github.com/llvm/llvm-project/issues/156652

---

### Comment 4 - rnk

Seems like expected behavior. Maybe we could emit an error when calling this builtin from a non-coroutine function in the frontend, rather than crashing.

---

### Comment 5 - shafik

> Seems like expected behavior. Maybe we could emit an error when calling this builtin from a non-coroutine function in the frontend, rather than crashing.

Unless this is hard to detect in the frontend we should just emit a diagnostic. I have not looked into at all though.

---

