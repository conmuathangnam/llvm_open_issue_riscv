# [X86][GISel] fatal error: error in backend: Cannot emit physreg copy instruction

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166557
**Status:** Closed
**Labels:** backend:X86, llvm:globalisel, crash
**Closed Date:** 2025-11-25T14:31:29Z

## Body

Reproducer:
https://godbolt.org/z/6PW8fnTP1
```cpp
union foo {
  _Float16 foo;
  short s;
};

_Float16 bar(union foo x) { return x.foo; }
```

Backtrace:
```console
fatal error: error in backend: Cannot emit physreg copy instruction
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -global-isel <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Post-RA pseudo instruction expansion pass' on function '@_Z3bar3foo'
 #0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d11b38)
 #1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0f50c)
 #2 0x0000000003c557b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c557b3)
 #3 0x0000000003d06d4e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d06d4e)
 #4 0x0000000000dda51b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003c5fa13 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fa13)
 #6 0x0000000003c5fb78 (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fb78)
 #7 0x00000000029d9b8c llvm::X86InstrInfo::copyPhysReg(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, llvm::DebugLoc const&, llvm::Register, llvm::Register, bool, bool, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x29d9b8c)
 #8 0x00000000033765c4 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x33765c4)
 #9 0x0000000002fcf8b8 (anonymous namespace)::ExpandPostRA::run(llvm::MachineFunction&) (.constprop.0) ExpandPostRAPseudos.cpp:0:0
#10 0x0000000002fcfad4 (anonymous namespace)::ExpandPostRALegacy::runOnMachineFunction(llvm::MachineFunction&) ExpandPostRAPseudos.cpp:0:0
#11 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef0dd)
#12 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663302)
#13 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663591)
#14 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3664e73)
#15 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fae042)
#16 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e5ccb)
#17 0x000000000625ba5c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ba5c)
#18 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e6825)
#19 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48e4c2a)
#20 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x486496b)
#21 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49dcdfb)
#22 0x0000000000ddc575 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddc575)
#23 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#24 0x0000000000dd457d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#25 0x0000000004654899 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556b3)
#27 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617682)
#29 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4618561)
#30 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462140c)
#31 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd8fa9)
#32 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc83ec4)
#33 0x000071721c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x000071721c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd3f75)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6PW8fnTP1
```cpp
union foo {
  _Float16 foo;
  short s;
};

_Float16 bar(union foo x) { return x.foo; }
```

Backtrace:
```console
fatal error: error in backend: Cannot emit physreg copy instruction
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -global-isel &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Post-RA pseudo instruction expansion pass' on function '@<!-- -->_Z3bar3foo'
 #<!-- -->0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d11b38)
 #<!-- -->1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0f50c)
 #<!-- -->2 0x0000000003c557b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c557b3)
 #<!-- -->3 0x0000000003d06d4e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d06d4e)
 #<!-- -->4 0x0000000000dda51b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003c5fa13 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fa13)
 #<!-- -->6 0x0000000003c5fb78 (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5fb78)
 #<!-- -->7 0x00000000029d9b8c llvm::X86InstrInfo::copyPhysReg(llvm::MachineBasicBlock&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, llvm::DebugLoc const&amp;, llvm::Register, llvm::Register, bool, bool, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x29d9b8c)
 #<!-- -->8 0x00000000033765c4 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x33765c4)
 #<!-- -->9 0x0000000002fcf8b8 (anonymous namespace)::ExpandPostRA::run(llvm::MachineFunction&amp;) (.constprop.0) ExpandPostRAPseudos.cpp:0:0
#<!-- -->10 0x0000000002fcfad4 (anonymous namespace)::ExpandPostRALegacy::runOnMachineFunction(llvm::MachineFunction&amp;) ExpandPostRAPseudos.cpp:0:0
#<!-- -->11 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef0dd)
#<!-- -->12 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663302)
#<!-- -->13 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3663591)
#<!-- -->14 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3664e73)
#<!-- -->15 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fae042)
#<!-- -->16 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e5ccb)
#<!-- -->17 0x000000000625ba5c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ba5c)
#<!-- -->18 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e6825)
#<!-- -->19 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48e4c2a)
#<!-- -->20 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x486496b)
#<!-- -->21 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49dcdfb)
#<!-- -->22 0x0000000000ddc575 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddc575)
#<!-- -->23 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->24 0x0000000000dd457d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004654899 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556b3)
#<!-- -->27 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617682)
#<!-- -->29 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4618561)
#<!-- -->30 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462140c)
#<!-- -->31 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd8fa9)
#<!-- -->32 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc83ec4)
#<!-- -->33 0x000071721c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x000071721c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd3f75)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - GrumpyPigSkin

I have a fix for this if someone could assign me please :)

---

