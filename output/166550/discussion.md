# [X86][GISel] crashes on a function which uses _Float16

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166550
**Status:** Open
**Labels:** backend:X86, llvm:globalisel, crash

## Body

Reproducer:
https://godbolt.org/z/dsEPfb5Yq
```cpp
_Float16 foo(_Float16 a, _Float16 b) { return a + b; }
```

Backtrace:
```console
fatal error: error in backend: unable to legalize instruction: %9:_(s16) = G_FPTRUNC %8:_(s32) (in function: _Z3fooDF16_DF16_)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -global-isel <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Legalizer' on function '@_Z3fooDF16_DF16_'
 #0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d11b38)
 #1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0f50c)
 #2 0x0000000003c557b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c557b3)
 #3 0x0000000003d06d4e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d06d4e)
 #4 0x0000000000dda51b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003c5fa13 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fa13)
 #6 0x0000000003c5fc5b (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fc5b)
 #7 0x000000000503b4ed llvm::reportGISelFailure(llvm::MachineFunction&, llvm::TargetPassConfig const&, llvm::MachineOptimizationRemarkEmitter&, char const*, llvm::StringRef, llvm::MachineInstr const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x503b4ed)
 #8 0x0000000004fc00fc llvm::Legalizer::runOnMachineFunction(llvm::MachineFunction&) (.part.0) Legalizer.cpp:0:0
 #9 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef0dd)
#10 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663302)
#11 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663591)
#12 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3664e73)
#13 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fae042)
#14 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e5ccb)
#15 0x000000000625ba5c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ba5c)
#16 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e6825)
#17 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48e4c2a)
#18 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x486496b)
#19 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49dcdfb)
#20 0x0000000000ddc575 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddc575)
#21 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dd457d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004654899 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556b3)
#25 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617682)
#27 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4618561)
#28 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462140c)
#29 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd8fa9)
#30 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc83ec4)
#31 0x0000714bd8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x0000714bd8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd3f75)
Compiler returned: 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/dsEPfb5Yq
```cpp
_Float16 foo(_Float16 a, _Float16 b) { return a + b; }
```

Backtrace:
```console
fatal error: error in backend: unable to legalize instruction: %9:_(s16) = G_FPTRUNC %8:_(s32) (in function: _Z3fooDF16_DF16_)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -global-isel &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Legalizer' on function '@<!-- -->_Z3fooDF16_DF16_'
 #<!-- -->0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d11b38)
 #<!-- -->1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0f50c)
 #<!-- -->2 0x0000000003c557b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c557b3)
 #<!-- -->3 0x0000000003d06d4e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d06d4e)
 #<!-- -->4 0x0000000000dda51b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003c5fa13 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fa13)
 #<!-- -->6 0x0000000003c5fc5b (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fc5b)
 #<!-- -->7 0x000000000503b4ed llvm::reportGISelFailure(llvm::MachineFunction&amp;, llvm::TargetPassConfig const&amp;, llvm::MachineOptimizationRemarkEmitter&amp;, char const*, llvm::StringRef, llvm::MachineInstr const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x503b4ed)
 #<!-- -->8 0x0000000004fc00fc llvm::Legalizer::runOnMachineFunction(llvm::MachineFunction&amp;) (.part.0) Legalizer.cpp:0:0
 #<!-- -->9 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef0dd)
#<!-- -->10 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663302)
#<!-- -->11 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663591)
#<!-- -->12 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3664e73)
#<!-- -->13 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fae042)
#<!-- -->14 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e5ccb)
#<!-- -->15 0x000000000625ba5c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ba5c)
#<!-- -->16 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e6825)
#<!-- -->17 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48e4c2a)
#<!-- -->18 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x486496b)
#<!-- -->19 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49dcdfb)
#<!-- -->20 0x0000000000ddc575 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddc575)
#<!-- -->21 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dd457d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004654899 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556b3)
#<!-- -->25 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617682)
#<!-- -->27 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4618561)
#<!-- -->28 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462140c)
#<!-- -->29 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd8fa9)
#<!-- -->30 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc83ec4)
#<!-- -->31 0x0000714bd8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x0000714bd8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd3f75)
Compiler returned: 1
```
</details>


---

### Comment 2 - e-kud

It is not a bug per se, if we compile with `-fglobal-isel` the behavior is correct: GlobalISel falls back to FastISel/SDAG. `-mllvm -global-isel` enforces to use only GlobalISel. Just fp16 support is missing.

---

