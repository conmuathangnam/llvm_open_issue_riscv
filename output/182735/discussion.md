# [X86][GlobalISel] Assertion `IsVector && "cannot get number of elements on scalar/aggregate"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/182735
**Status:** Open
**Labels:** backend:X86, regression, llvm:globalisel, crash

## Body

This does not crash with clang-21.

Reproducer:
https://godbolt.org/z/5x7bPvndh
```cpp
void foo(char *c) {}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/CodeGenTypes/LowLevelType.h:185: constexpr llvm::ElementCount llvm::LLT::getElementCount() const: Assertion `IsVector && "cannot get number of elements on scalar/aggregate"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -fglobal-isel <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RegBankSelect' on function '@_Z3fooPc'
 #0 0x00000000043fb618 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fb618)
 #1 0x00000000043f8a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43f8a64)
 #2 0x0000000004338bb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e907b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e907b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e907b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e907b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e907b82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e907b839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000002f0f599 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f0f599)
#10 0x0000000002f0f662 llvm::X86RegisterBankInfo::getInstrPartialMappingIdxs(llvm::MachineInstr const&, llvm::MachineRegisterInfo const&, bool, llvm::SmallVectorImpl<llvm::X86GenRegisterBankInfo::PartialMappingIdx>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f0f662)
#11 0x0000000002f106ba llvm::X86RegisterBankInfo::getInstrMapping(llvm::MachineInstr const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f106ba)
#12 0x00000000059acbf7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59acbf7)
#13 0x00000000059ae5f1 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59ae5f1)
#14 0x00000000059ae826 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59ae826)
#15 0x000000000366f809 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#16 0x0000000003cd24b6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd24b6)
#17 0x0000000003cd2861 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd2861)
#18 0x0000000003cd30cf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd30cf)
#19 0x000000000481c83d (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#20 0x000000000481d0d9 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481d0d9)
#21 0x0000000004ed27ae clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed27ae)
#22 0x0000000006bafe2c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bafe2c)
#23 0x0000000004ed2bf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed2bf8)
#24 0x00000000051d2b35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d2b35)
#25 0x000000000515106e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x515106e)
#26 0x00000000052ce84d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52ce84d)
#27 0x0000000000dee9fe cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdee9fe)
#28 0x0000000000de538a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000de550d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004f467a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000004338ff4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4338ff4)
#32 0x0000000004f46dbf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004f07672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f07672)
#34 0x0000000004f0861e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0861e)
#35 0x0000000004f0f995 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0f995)
#36 0x0000000000dead79 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdead79)
#37 0x0000000000ca3a0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca3a0a)
#38 0x00007e907b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007e907b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000de4e25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e25)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
This does not crash with clang-21.

Reproducer:
https://godbolt.org/z/5x7bPvndh
```cpp
void foo(char *c) {}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/CodeGenTypes/LowLevelType.h:185: constexpr llvm::ElementCount llvm::LLT::getElementCount() const: Assertion `IsVector &amp;&amp; "cannot get number of elements on scalar/aggregate"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -fglobal-isel &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RegBankSelect' on function '@<!-- -->_Z3fooPc'
 #<!-- -->0 0x00000000043fb618 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fb618)
 #<!-- -->1 0x00000000043f8a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43f8a64)
 #<!-- -->2 0x0000000004338bb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e907b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e907b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e907b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e907b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e907b82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e907b839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000002f0f599 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f0f599)
#<!-- -->10 0x0000000002f0f662 llvm::X86RegisterBankInfo::getInstrPartialMappingIdxs(llvm::MachineInstr const&amp;, llvm::MachineRegisterInfo const&amp;, bool, llvm::SmallVectorImpl&lt;llvm::X86GenRegisterBankInfo::PartialMappingIdx&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f0f662)
#<!-- -->11 0x0000000002f106ba llvm::X86RegisterBankInfo::getInstrMapping(llvm::MachineInstr const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f106ba)
#<!-- -->12 0x00000000059acbf7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59acbf7)
#<!-- -->13 0x00000000059ae5f1 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59ae5f1)
#<!-- -->14 0x00000000059ae826 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x59ae826)
#<!-- -->15 0x000000000366f809 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->16 0x0000000003cd24b6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd24b6)
#<!-- -->17 0x0000000003cd2861 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd2861)
#<!-- -->18 0x0000000003cd30cf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cd30cf)
#<!-- -->19 0x000000000481c83d (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;) BackendUtil.cpp:0:0
#<!-- -->20 0x000000000481d0d9 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481d0d9)
#<!-- -->21 0x0000000004ed27ae clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed27ae)
#<!-- -->22 0x0000000006bafe2c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bafe2c)
#<!-- -->23 0x0000000004ed2bf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed2bf8)
#<!-- -->24 0x00000000051d2b35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d2b35)
#<!-- -->25 0x000000000515106e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x515106e)
#<!-- -->26 0x00000000052ce84d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52ce84d)
#<!-- -->27 0x0000000000dee9fe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdee9fe)
#<!-- -->28 0x0000000000de538a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000de550d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004f467a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000004338ff4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4338ff4)
#<!-- -->32 0x0000000004f46dbf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004f07672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f07672)
#<!-- -->34 0x0000000004f0861e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0861e)
#<!-- -->35 0x0000000004f0f995 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0f995)
#<!-- -->36 0x0000000000dead79 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdead79)
#<!-- -->37 0x0000000000ca3a0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca3a0a)
#<!-- -->38 0x00007e907b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007e907b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000de4e25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e25)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

