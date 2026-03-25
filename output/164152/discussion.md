# Assertion `!IsStrict && "Strict FP doesn't support BF16"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/164152
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

Reproducer:
https://godbolt.org/z/q6z85x4Gj
```cpp
typedef __bf16 __bf1664 __attribute__((vector_size(64)));

__bf1664 test___bf1664_ADD(__bf1664 a, __bf1664 b) {
    return a + b;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Target/X86/X86ISelLowering.cpp:60452: llvm::SDValue combineFP_EXTEND(llvm::SDNode*, llvm::SelectionDAG&, llvm::TargetLowering::DAGCombinerInfo&, const llvm::X86Subtarget&): Assertion `!IsStrict && "Strict FP doesn't support BF16"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ftrapping-math <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z17test___bf1664_ADDDv32_DF16bS_'
 #0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073f31c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073f31c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073f31c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073f31c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073f31c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073f31c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000002ba31f5 combineFP_EXTEND(llvm::SDNode*, llvm::SelectionDAG&, llvm::TargetLowering::DAGCombinerInfo&, llvm::X86Subtarget const&) X86ISelLowering.cpp:0:0
#10 0x0000000002c81786 llvm::X86TargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2c81786)
#11 0x0000000005700671 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#12 0x000000000570206d (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#13 0x0000000005704bd3 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5704bd3)
#14 0x00000000058485f1 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x58485f1)
#15 0x000000000584c45d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584c45d)
#16 0x000000000584da45 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584da45)
#17 0x000000000583924f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x583924f)
#18 0x00000000034bd6b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x0000000003b01568 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b01568)
#20 0x0000000003b017a1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b017a1)
#21 0x0000000003b0200f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0200f)
#22 0x00000000044bf92e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44bf92e)
#23 0x0000000004b4a787 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4a787)
#24 0x00000000068e771c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e771c)
#25 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#26 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#27 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#28 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#29 0x0000000000dc2b50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#30 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000db983d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x0000000004bb3d99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#34 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#36 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#37 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#38 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#39 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#40 0x000073f31c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x000073f31c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Note: using `-ftrapping-math`

---

### Comment 2 - k-arrows

maybe related to: https://github.com/llvm/llvm-project/issues/161139

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/q6z85x4Gj
```cpp
typedef __bf16 __bf1664 __attribute__((vector_size(64)));

__bf1664 test___bf1664_ADD(__bf1664 a, __bf1664 b) {
    return a + b;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Target/X86/X86ISelLowering.cpp:60452: llvm::SDValue combineFP_EXTEND(llvm::SDNode*, llvm::SelectionDAG&amp;, llvm::TargetLowering::DAGCombinerInfo&amp;, const llvm::X86Subtarget&amp;): Assertion `!IsStrict &amp;&amp; "Strict FP doesn't support BF16"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ftrapping-math &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z17test___bf1664_ADDDv32_DF16bS_'
 #<!-- -->0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #<!-- -->1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #<!-- -->2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073f31c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073f31c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073f31c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073f31c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073f31c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073f31c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000002ba31f5 combineFP_EXTEND(llvm::SDNode*, llvm::SelectionDAG&amp;, llvm::TargetLowering::DAGCombinerInfo&amp;, llvm::X86Subtarget const&amp;) X86ISelLowering.cpp:0:0
#<!-- -->10 0x0000000002c81786 llvm::X86TargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2c81786)
#<!-- -->11 0x0000000005700671 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->12 0x000000000570206d (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#<!-- -->13 0x0000000005704bd3 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5704bd3)
#<!-- -->14 0x00000000058485f1 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x58485f1)
#<!-- -->15 0x000000000584c45d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584c45d)
#<!-- -->16 0x000000000584da45 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584da45)
#<!-- -->17 0x000000000583924f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x583924f)
#<!-- -->18 0x00000000034bd6b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x0000000003b01568 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b01568)
#<!-- -->20 0x0000000003b017a1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b017a1)
#<!-- -->21 0x0000000003b0200f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b0200f)
#<!-- -->22 0x00000000044bf92e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44bf92e)
#<!-- -->23 0x0000000004b4a787 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4a787)
#<!-- -->24 0x00000000068e771c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e771c)
#<!-- -->25 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#<!-- -->26 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#<!-- -->27 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#<!-- -->28 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#<!-- -->29 0x0000000000dc2b50 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#<!-- -->30 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000db983d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004bb3d99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#<!-- -->34 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#<!-- -->36 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#<!-- -->37 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#<!-- -->38 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#<!-- -->39 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#<!-- -->40 0x000073f31c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x000073f31c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

