# [X86][GISel] Assertion `(Size == 32 || Size == 64 || Size == 80) && "Unsupported size for G_FCMP"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166554
**Status:** Open
**Labels:** backend:X86, llvm:globalisel, crash

## Body

Reproducer:
https://godbolt.org/z/z34Mnaz84
```cpp
int foo(_Float16 a, _Float16 b) { return a > b; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Target/X86/GISel/X86RegisterBankInfo.cpp:337: virtual const llvm::RegisterBankInfo::InstructionMapping& llvm::X86RegisterBankInfo::getInstrMapping(const llvm::MachineInstr&) const: Assertion `(Size == 32 || Size == 64 || Size == 80) && "Unsupported size for G_FCMP"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -mllvm -global-isel <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RegBankSelect' on function '@_Z3fooDF16_DF16_'
 #0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dbff3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007dbff3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007dbff3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007dbff3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007dbff3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007dbff3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000002d84e65 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2d84e65)
#10 0x0000000005648f57 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5648f57)
#11 0x00000000056494f5 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56494f5)
#12 0x0000000005649736 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5649736)
#13 0x00000000034ebb59 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x0000000003b350a6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b350a6)
#15 0x0000000003b35451 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35451)
#16 0x0000000003b35cbf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35cbf)
#17 0x00000000044f97f6 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f97f6)
#18 0x0000000004b83fa7 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83fa7)
#19 0x0000000006926d7c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926d7c)
#20 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#21 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#22 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#23 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#24 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#25 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000dc3ead int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004bedcb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#29 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#31 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#32 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#33 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#34 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#35 0x00007dbff3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007dbff3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/z34Mnaz84
```cpp
int foo(_Float16 a, _Float16 b) { return a &gt; b; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Target/X86/GISel/X86RegisterBankInfo.cpp:337: virtual const llvm::RegisterBankInfo::InstructionMapping&amp; llvm::X86RegisterBankInfo::getInstrMapping(const llvm::MachineInstr&amp;) const: Assertion `(Size == 32 || Size == 64 || Size == 80) &amp;&amp; "Unsupported size for G_FCMP"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -mllvm -global-isel &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RegBankSelect' on function '@<!-- -->_Z3fooDF16_DF16_'
 #<!-- -->0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #<!-- -->1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #<!-- -->2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007dbff3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007dbff3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007dbff3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007dbff3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007dbff3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007dbff3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000002d84e65 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2d84e65)
#<!-- -->10 0x0000000005648f57 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5648f57)
#<!-- -->11 0x00000000056494f5 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56494f5)
#<!-- -->12 0x0000000005649736 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5649736)
#<!-- -->13 0x00000000034ebb59 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->14 0x0000000003b350a6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b350a6)
#<!-- -->15 0x0000000003b35451 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35451)
#<!-- -->16 0x0000000003b35cbf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35cbf)
#<!-- -->17 0x00000000044f97f6 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f97f6)
#<!-- -->18 0x0000000004b83fa7 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83fa7)
#<!-- -->19 0x0000000006926d7c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926d7c)
#<!-- -->20 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#<!-- -->21 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#<!-- -->22 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#<!-- -->23 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#<!-- -->24 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#<!-- -->25 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000dc3ead int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004bedcb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#<!-- -->29 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#<!-- -->31 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#<!-- -->32 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#<!-- -->33 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#<!-- -->34 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#<!-- -->35 0x00007dbff3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007dbff3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

