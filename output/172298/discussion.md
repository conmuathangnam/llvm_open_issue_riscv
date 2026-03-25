# [X86][GISel] Crash with GlobalISel enabled when compiling simple loop with _Complex float (-O2)

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172298
**Status:** Closed
**Labels:** duplicate, backend:X86, llvm:globalisel, crash
**Closed Date:** 2025-12-15T14:50:38Z

## Body

Reproducer:
https://godbolt.org/z/fqrdGfx3W
```cpp
void foo(_Complex float *a, _Complex float *b) {
  for (int i = 0; i != 10; i++)
    a[i] = b[i];
}
```

Backtrace:
```console
Unsupported register size.
UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/GISel/X86RegisterBankInfo.cpp:190!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O2 -mllvm -global-isel -mllvm -global-isel-abort=2 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RegBankSelect' on function '@_Z3fooPCfS0_'
 #0 0x00000000042bbe08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42bbe08)
 #1 0x00000000042b9234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b9234)
 #2 0x00000000041fe018 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bd2d4e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bd2d4e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bd2d4e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bd2d4e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000004208c4a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4208c4a)
 #8 0x0000000002df8689 llvm::X86GenRegisterBankInfo::getPartialMappingIdx(llvm::MachineInstr const&, llvm::LLT const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df8689)
 #9 0x0000000002df8842 llvm::X86RegisterBankInfo::getInstrPartialMappingIdxs(llvm::MachineInstr const&, llvm::MachineRegisterInfo const&, bool, llvm::SmallVectorImpl<llvm::X86GenRegisterBankInfo::PartialMappingIdx>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df8842)
#10 0x0000000002df9dcd llvm::X86RegisterBankInfo::getInstrMapping(llvm::MachineInstr const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df9dcd)
#11 0x00000000056cc0c7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc0c7)
#12 0x00000000056cc665 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc665)
#13 0x00000000056cc896 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc896)
#14 0x0000000003556fe9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x0000000003bad376 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bad376)
#16 0x0000000003bad721 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bad721)
#17 0x0000000003badf8f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3badf8f)
#18 0x000000000457ff7d clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457ff7d)
#19 0x0000000004c0de87 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0de87)
#20 0x00000000069be52c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69be52c)
#21 0x0000000004c0e2e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0e2e8)
#22 0x0000000004f03985 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f03985)
#23 0x0000000004e83bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e83bfe)
#24 0x0000000004ffda3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ffda3d)
#25 0x0000000000dcf4cc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcf4cc)
#26 0x0000000000dc5f2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x0000000000dc60ad int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x0000000004c80f19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x00000000041fe4b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fe4b4)
#30 0x0000000004c8152f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004c41d22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c41d22)
#32 0x0000000004c42cce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c42cce)
#33 0x0000000004c4a105 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4a105)
#34 0x0000000000dcb8d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcb8d1)
#35 0x0000000000c77584 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc77584)
#36 0x00007bd2d4e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x00007bd2d4e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000dc59c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc59c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/fqrdGfx3W
```cpp
void foo(_Complex float *a, _Complex float *b) {
  for (int i = 0; i != 10; i++)
    a[i] = b[i];
}
```

Backtrace:
```console
Unsupported register size.
UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/GISel/X86RegisterBankInfo.cpp:190!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O2 -mllvm -global-isel -mllvm -global-isel-abort=2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RegBankSelect' on function '@<!-- -->_Z3fooPCfS0_'
 #<!-- -->0 0x00000000042bbe08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42bbe08)
 #<!-- -->1 0x00000000042b9234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b9234)
 #<!-- -->2 0x00000000041fe018 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007bd2d4e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007bd2d4e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007bd2d4e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007bd2d4e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000004208c4a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4208c4a)
 #<!-- -->8 0x0000000002df8689 llvm::X86GenRegisterBankInfo::getPartialMappingIdx(llvm::MachineInstr const&amp;, llvm::LLT const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df8689)
 #<!-- -->9 0x0000000002df8842 llvm::X86RegisterBankInfo::getInstrPartialMappingIdxs(llvm::MachineInstr const&amp;, llvm::MachineRegisterInfo const&amp;, bool, llvm::SmallVectorImpl&lt;llvm::X86GenRegisterBankInfo::PartialMappingIdx&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df8842)
#<!-- -->10 0x0000000002df9dcd llvm::X86RegisterBankInfo::getInstrMapping(llvm::MachineInstr const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2df9dcd)
#<!-- -->11 0x00000000056cc0c7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc0c7)
#<!-- -->12 0x00000000056cc665 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc665)
#<!-- -->13 0x00000000056cc896 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56cc896)
#<!-- -->14 0x0000000003556fe9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x0000000003bad376 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bad376)
#<!-- -->16 0x0000000003bad721 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bad721)
#<!-- -->17 0x0000000003badf8f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3badf8f)
#<!-- -->18 0x000000000457ff7d clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457ff7d)
#<!-- -->19 0x0000000004c0de87 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0de87)
#<!-- -->20 0x00000000069be52c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69be52c)
#<!-- -->21 0x0000000004c0e2e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0e2e8)
#<!-- -->22 0x0000000004f03985 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f03985)
#<!-- -->23 0x0000000004e83bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e83bfe)
#<!-- -->24 0x0000000004ffda3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ffda3d)
#<!-- -->25 0x0000000000dcf4cc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcf4cc)
#<!-- -->26 0x0000000000dc5f2a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x0000000000dc60ad int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->28 0x0000000004c80f19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x00000000041fe4b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fe4b4)
#<!-- -->30 0x0000000004c8152f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x0000000004c41d22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c41d22)
#<!-- -->32 0x0000000004c42cce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c42cce)
#<!-- -->33 0x0000000004c4a105 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4a105)
#<!-- -->34 0x0000000000dcb8d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcb8d1)
#<!-- -->35 0x0000000000c77584 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc77584)
#<!-- -->36 0x00007bd2d4e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x00007bd2d4e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000dc59c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc59c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-17.0.1:
https://godbolt.org/z/esn66PvKj

---

### Comment 3 - arsenm

> Goes back to clang-17.0.1: https://godbolt.org/z/esn66PvKj

Globalisel is generally not expected to work for x86, and it only gets more true the further back you go 

---

### Comment 4 - e-kud

```
Assign: %2:_(<2 x s32>) = G_LOAD %1:gpr(p0), debug-location !28 :: (load (<2 x s32>) from %ir.b, align 4); example.cpp:3:12
Unsupported register size.
UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/GISel/X86RegisterBankInfo.cpp:190!
```

---

