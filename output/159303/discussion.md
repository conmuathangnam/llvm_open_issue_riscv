# Risc-V: ICE when trying to use non-offsettable memory operand in inline assembly

**Author:** Hendiadyoin1
**URL:** https://github.com/llvm/llvm-project/issues/159303

## Body

When trying to use a non-offsettable memory constraint (`V`) on the risc-v target the compiler crashes

reproducer: <https://ice.godbolt.org/z/bMvec53cY>

<details>
<summary>Trace Back</summary>

```
fatal error: error in backend: Unexpected asm memory constraint ZT
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_Z10frobnicateRi'
 #0 0x0000000003c60008 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c60008)
 #1 0x0000000003c5d9dc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5d9dc)
 #2 0x0000000003badf43 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3badf43)
 #3 0x0000000003c5522e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5522e)
 #4 0x0000000000dc2b5b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003bb7cb3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bb7cb3)
 #6 0x0000000002250866 llvm::RISCVDAGToDAGISel::SelectInlineAsmMemoryOperand(llvm::SDValue const&, llvm::InlineAsm::ConstraintCode, std::vector<llvm::SDValue, std::allocator<llvm::SDValue>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2250866)
 #7 0x000000000518f07f llvm::SelectionDAGISel::SelectInlineAsmMemoryOperands(std::vector<llvm::SDValue, std::allocator<llvm::SDValue>>&, llvm::SDLoc const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f07f)
 #8 0x000000000518f4fc llvm::SelectionDAGISel::Select_INLINEASM(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f4fc)
 #9 0x000000000518fc55 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518fc55)
#10 0x000000000224a857 llvm::RISCVDAGToDAGISel::Select(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x224a857)
#11 0x000000000518b564 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b564)
#12 0x0000000005197922 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5197922)
#13 0x000000000519a2aa llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519a2aa)
#14 0x000000000519c1fe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c1fe)
#15 0x000000000518b0f1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b0f1)
#16 0x000000000306e5ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x306e5ad)
#17 0x00000000035c6222 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c6222)
#18 0x00000000035c64b1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c64b1)
#19 0x00000000035c7d87 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c7d87)
#20 0x0000000003f010bd clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f010bd)
#21 0x000000000452a65b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452a65b)
#22 0x000000000617b04c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617b04c)
#23 0x000000000452ae5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452ae5d)
#24 0x0000000004826e1a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4826e1a)
#25 0x00000000047a141b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a141b)
#26 0x000000000491a67b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491a67b)
#27 0x0000000000dc4bb5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4bb5)
#28 0x0000000000dbcb6d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004598a59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003bade43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bade43)
#31 0x0000000004598c79 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x000000000455bdd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455bdd2)
#33 0x000000000455ccb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ccb1)
#34 0x000000000456580c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456580c)
#35 0x0000000000dc13d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc13d1)
#36 0x0000000000c6f3c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6f3c4)
#37 0x0000719510629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x0000719510629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000dbc605 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbc605)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70fatal error: error in backend: Unexpected asm memory constraint ZT
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_Z10frobnicateRi'
 #0 0x0000000003c60008 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c60008)
 #1 0x0000000003c5d9dc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5d9dc)
 #2 0x0000000003badf43 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3badf43)
 #3 0x0000000003c5522e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5522e)
 #4 0x0000000000dc2b5b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003bb7cb3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bb7cb3)
 #6 0x0000000002250866 llvm::RISCVDAGToDAGISel::SelectInlineAsmMemoryOperand(llvm::SDValue const&, llvm::InlineAsm::ConstraintCode, std::vector<llvm::SDValue, std::allocator<llvm::SDValue>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2250866)
 #7 0x000000000518f07f llvm::SelectionDAGISel::SelectInlineAsmMemoryOperands(std::vector<llvm::SDValue, std::allocator<llvm::SDValue>>&, llvm::SDLoc const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f07f)
 #8 0x000000000518f4fc llvm::SelectionDAGISel::Select_INLINEASM(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f4fc)
 #9 0x000000000518fc55 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518fc55)
#10 0x000000000224a857 llvm::RISCVDAGToDAGISel::Select(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x224a857)
#11 0x000000000518b564 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b564)
#12 0x0000000005197922 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5197922)
#13 0x000000000519a2aa llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519a2aa)
#14 0x000000000519c1fe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c1fe)
#15 0x000000000518b0f1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b0f1)
#16 0x000000000306e5ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x306e5ad)
#17 0x00000000035c6222 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c6222)
#18 0x00000000035c64b1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c64b1)
#19 0x00000000035c7d87 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c7d87)
#20 0x0000000003f010bd clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f010bd)
#21 0x000000000452a65b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452a65b)
#22 0x000000000617b04c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617b04c)
#23 0x000000000452ae5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452ae5d)
#24 0x0000000004826e1a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4826e1a)
#25 0x00000000047a141b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a141b)
#26 0x000000000491a67b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491a67b)
#27 0x0000000000dc4bb5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4bb5)
#28 0x0000000000dbcb6d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004598a59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003bade43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bade43)
#31 0x0000000004598c79 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x000000000455bdd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455bdd2)
#33 0x000000000455ccb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ccb1)
#34 0x000000000456580c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456580c)
#35 0x0000000000dc13d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc13d1)
#36 0x0000000000c6f3c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6f3c4)
#37 0x0000719510629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x0000719510629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000dbc605 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbc605)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Leon Albrecht (Hendiadyoin1)

<details>
When trying to use a non-offsettable memory constraint (`V`) on the risc-v target the compiler crashes

reproducer: &lt;https://ice.godbolt.org/z/bMvec53cY&gt;

&lt;details&gt;
&lt;summary&gt;Trace Back&lt;/summary&gt;

```
fatal error: error in backend: Unexpected asm memory constraint ZT
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z10frobnicateRi'
 #<!-- -->0 0x0000000003c60008 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c60008)
 #<!-- -->1 0x0000000003c5d9dc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5d9dc)
 #<!-- -->2 0x0000000003badf43 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3badf43)
 #<!-- -->3 0x0000000003c5522e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5522e)
 #<!-- -->4 0x0000000000dc2b5b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003bb7cb3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bb7cb3)
 #<!-- -->6 0x0000000002250866 llvm::RISCVDAGToDAGISel::SelectInlineAsmMemoryOperand(llvm::SDValue const&amp;, llvm::InlineAsm::ConstraintCode, std::vector&lt;llvm::SDValue, std::allocator&lt;llvm::SDValue&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2250866)
 #<!-- -->7 0x000000000518f07f llvm::SelectionDAGISel::SelectInlineAsmMemoryOperands(std::vector&lt;llvm::SDValue, std::allocator&lt;llvm::SDValue&gt;&gt;&amp;, llvm::SDLoc const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f07f)
 #<!-- -->8 0x000000000518f4fc llvm::SelectionDAGISel::Select_INLINEASM(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f4fc)
 #<!-- -->9 0x000000000518fc55 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518fc55)
#<!-- -->10 0x000000000224a857 llvm::RISCVDAGToDAGISel::Select(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x224a857)
#<!-- -->11 0x000000000518b564 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b564)
#<!-- -->12 0x0000000005197922 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5197922)
#<!-- -->13 0x000000000519a2aa llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519a2aa)
#<!-- -->14 0x000000000519c1fe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c1fe)
#<!-- -->15 0x000000000518b0f1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b0f1)
#<!-- -->16 0x000000000306e5ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x306e5ad)
#<!-- -->17 0x00000000035c6222 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c6222)
#<!-- -->18 0x00000000035c64b1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c64b1)
#<!-- -->19 0x00000000035c7d87 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c7d87)
#<!-- -->20 0x0000000003f010bd clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f010bd)
#<!-- -->21 0x000000000452a65b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452a65b)
#<!-- -->22 0x000000000617b04c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617b04c)
#<!-- -->23 0x000000000452ae5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452ae5d)
#<!-- -->24 0x0000000004826e1a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4826e1a)
#<!-- -->25 0x00000000047a141b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a141b)
#<!-- -->26 0x000000000491a67b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491a67b)
#<!-- -->27 0x0000000000dc4bb5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4bb5)
#<!-- -->28 0x0000000000dbcb6d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004598a59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003bade43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bade43)
#<!-- -->31 0x0000000004598c79 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x000000000455bdd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455bdd2)
#<!-- -->33 0x000000000455ccb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ccb1)
#<!-- -->34 0x000000000456580c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456580c)
#<!-- -->35 0x0000000000dc13d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc13d1)
#<!-- -->36 0x0000000000c6f3c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6f3c4)
#<!-- -->37 0x0000719510629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x0000719510629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000dbc605 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbc605)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70fatal error: error in backend: Unexpected asm memory constraint ZT
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z10frobnicateRi'
 #<!-- -->0 0x0000000003c60008 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c60008)
 #<!-- -->1 0x0000000003c5d9dc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5d9dc)
 #<!-- -->2 0x0000000003badf43 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3badf43)
 #<!-- -->3 0x0000000003c5522e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5522e)
 #<!-- -->4 0x0000000000dc2b5b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003bb7cb3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bb7cb3)
 #<!-- -->6 0x0000000002250866 llvm::RISCVDAGToDAGISel::SelectInlineAsmMemoryOperand(llvm::SDValue const&amp;, llvm::InlineAsm::ConstraintCode, std::vector&lt;llvm::SDValue, std::allocator&lt;llvm::SDValue&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2250866)
 #<!-- -->7 0x000000000518f07f llvm::SelectionDAGISel::SelectInlineAsmMemoryOperands(std::vector&lt;llvm::SDValue, std::allocator&lt;llvm::SDValue&gt;&gt;&amp;, llvm::SDLoc const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f07f)
 #<!-- -->8 0x000000000518f4fc llvm::SelectionDAGISel::Select_INLINEASM(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518f4fc)
 #<!-- -->9 0x000000000518fc55 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518fc55)
#<!-- -->10 0x000000000224a857 llvm::RISCVDAGToDAGISel::Select(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x224a857)
#<!-- -->11 0x000000000518b564 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b564)
#<!-- -->12 0x0000000005197922 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5197922)
#<!-- -->13 0x000000000519a2aa llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519a2aa)
#<!-- -->14 0x000000000519c1fe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c1fe)
#<!-- -->15 0x000000000518b0f1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518b0f1)
#<!-- -->16 0x000000000306e5ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x306e5ad)
#<!-- -->17 0x00000000035c6222 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c6222)
#<!-- -->18 0x00000000035c64b1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c64b1)
#<!-- -->19 0x00000000035c7d87 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35c7d87)
#<!-- -->20 0x0000000003f010bd clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f010bd)
#<!-- -->21 0x000000000452a65b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452a65b)
#<!-- -->22 0x000000000617b04c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617b04c)
#<!-- -->23 0x000000000452ae5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x452ae5d)
#<!-- -->24 0x0000000004826e1a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4826e1a)
#<!-- -->25 0x00000000047a141b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a141b)
#<!-- -->26 0x000000000491a67b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491a67b)
#<!-- -->27 0x0000000000dc4bb5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4bb5)
#<!-- -->28 0x0000000000dbcb6d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004598a59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003bade43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bade43)
#<!-- -->31 0x0000000004598c79 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x000000000455bdd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455bdd2)
#<!-- -->33 0x000000000455ccb1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ccb1)
#<!-- -->34 0x000000000456580c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456580c)
#<!-- -->35 0x0000000000dc13d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc13d1)
#<!-- -->36 0x0000000000c6f3c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6f3c4)
#<!-- -->37 0x0000719510629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x0000719510629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000dbc605 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbc605)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

&lt;/details&gt;
</details>


---

### Comment 2 - topperc

On an assertions build, it fails even earlier

```
llc: llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:10209: void llvm::SelectionDAGBuilder::visitInlineAsm(const CallBase &, const BasicBlock *): Assertion `ConstraintID != InlineAsm::ConstraintCode::Unknown && "Failed to convert memory constraint code to constraint id."' failed.
```

---

### Comment 3 - lenary

I don't think we have testing for `V` which are a bit like `m` but with more constraints: https://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html

We do have tests for `m`, `o` and `A`, from what I can see.

---

### Comment 4 - topperc

X86 appears to be the only target that supports `V`.

---

### Comment 5 - Hendiadyoin1

Ah ok, then that explains it though it being listed as a general constraint is a bit odd.
GCC also did not like it, and I did miss the `A` in my search, which sounds like it solves the issues I was hitting

---

### Comment 6 - topperc

> Ah ok, then that explains it though it being listed as a general constraint is a bit odd. GCC also did not like it, and I did miss the `A` in my search, which sounds like it solves the issues I was hitting

To be clear. I meant specifically in clang. I don't know what GCC does on other targets.

---

### Comment 7 - lenary

GCC for RISC-V doesn't seem happy with your example either: https://ice.godbolt.org/z/dvKM11qGK 



---

