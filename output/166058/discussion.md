# [clang] Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166058
**Status:** Closed
**Labels:** backend:X86, crash, inline-asm
**Closed Date:** 2025-12-09T08:32:23Z

## Body

Reproducer:
https://godbolt.org/z/9z4dr8eM6
```cpp
int out;

void bar(void) { asm("foo %1,%0" : "=m"(out) : "L"(0xffffffff)); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z3barv'
 #0 0x00000000042387e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42387e8)
 #1 0x0000000004235c14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4235c14)
 #2 0x0000000004179e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000744d27042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000744d270969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000744d27042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000744d270287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000744d2702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000744d27039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000584fb99 llvm::SelectionDAG::getConstant(unsigned long, llvm::SDLoc const&, llvm::EVT, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584fb99)
#10 0x0000000002cc66dd llvm::X86TargetLowering::LowerAsmOperandForConstraint(llvm::SDValue, llvm::StringRef, std::vector<llvm::SDValue, std::allocator<llvm::SDValue>>&, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2cc66dd)
#11 0x00000000057c9241 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57c9241)
#12 0x00000000057e5adf llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57e5adf)
#13 0x00000000057fa192 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57fa192)
#14 0x000000000588d09d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588d09d)
#15 0x000000000588dfc1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588dfc1)
#16 0x000000000588f5e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588f5e5)
#17 0x000000000587afef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x587afef)
#18 0x00000000034ec969 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x0000000003b35cb6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35cb6)
#20 0x0000000003b36061 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b36061)
#21 0x0000000003b368cf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b368cf)
#22 0x00000000044f953e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f953e)
#23 0x0000000004b834c7 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b834c7)
#24 0x000000000692f58c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692f58c)
#25 0x0000000004b83928 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83928)
#26 0x0000000004e71515 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e71515)
#27 0x0000000004df231e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df231e)
#28 0x0000000004f69ced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69ced)
#29 0x0000000000dd0670 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#30 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000dc735d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x0000000004bed1c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x000000000417a304 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a304)
#34 0x0000000004bed7df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004bae572 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bae572)
#36 0x0000000004baf51e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf51e)
#37 0x0000000004bb6c55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb6c55)
#38 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#39 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#40 0x0000744d27029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000744d27029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/hMGc1a5sd

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/9z4dr8eM6
```cpp
int out;

void bar(void) { asm("foo %1,%0" : "=m"(out) : "L"(0xffffffff)); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) &amp;&amp; "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z3barv'
 #<!-- -->0 0x00000000042387e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42387e8)
 #<!-- -->1 0x0000000004235c14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4235c14)
 #<!-- -->2 0x0000000004179e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000744d27042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000744d270969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000744d27042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000744d270287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000744d2702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000744d27039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000584fb99 llvm::SelectionDAG::getConstant(unsigned long, llvm::SDLoc const&amp;, llvm::EVT, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x584fb99)
#<!-- -->10 0x0000000002cc66dd llvm::X86TargetLowering::LowerAsmOperandForConstraint(llvm::SDValue, llvm::StringRef, std::vector&lt;llvm::SDValue, std::allocator&lt;llvm::SDValue&gt;&gt;&amp;, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2cc66dd)
#<!-- -->11 0x00000000057c9241 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57c9241)
#<!-- -->12 0x00000000057e5adf llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57e5adf)
#<!-- -->13 0x00000000057fa192 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x57fa192)
#<!-- -->14 0x000000000588d09d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588d09d)
#<!-- -->15 0x000000000588dfc1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588dfc1)
#<!-- -->16 0x000000000588f5e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x588f5e5)
#<!-- -->17 0x000000000587afef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x587afef)
#<!-- -->18 0x00000000034ec969 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x0000000003b35cb6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b35cb6)
#<!-- -->20 0x0000000003b36061 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b36061)
#<!-- -->21 0x0000000003b368cf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b368cf)
#<!-- -->22 0x00000000044f953e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f953e)
#<!-- -->23 0x0000000004b834c7 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b834c7)
#<!-- -->24 0x000000000692f58c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692f58c)
#<!-- -->25 0x0000000004b83928 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83928)
#<!-- -->26 0x0000000004e71515 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e71515)
#<!-- -->27 0x0000000004df231e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df231e)
#<!-- -->28 0x0000000004f69ced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69ced)
#<!-- -->29 0x0000000000dd0670 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#<!-- -->30 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000dc735d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004bed1c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x000000000417a304 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a304)
#<!-- -->34 0x0000000004bed7df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004bae572 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bae572)
#<!-- -->36 0x0000000004baf51e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf51e)
#<!-- -->37 0x0000000004bb6c55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb6c55)
#<!-- -->38 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#<!-- -->39 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#<!-- -->40 0x0000744d27029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x0000744d27029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - ecnelises

```cpp
void X86TargetLowering::LowerAsmOperandForConstraint(
  // ...
  case 'L':
    if (auto *C = dyn_cast<ConstantSDNode>(Op)) {
      if (C->getZExtValue() == 0xff || C->getZExtValue() == 0xffff ||
          (Subtarget.is64Bit() && C->getZExtValue() == 0xffffffff)) {
        Result = DAG.getTargetConstant(C->getSExtValue(), SDLoc(Op),
                                       Op.getValueType());
        break;
      }
    }
    return;
```

Here `C->getZExtValue() == 0xffffffff`, hence `C->getSExtValue() == -1`, but first argument of `getTargetConstant` is `uint64_t`, which makes first parameter in this function become `0xffffffffffffffffull`.

One solution is to modify `getSExtValue` into `getZExtValue`. The code is written 10+ years ago, no idea why it's not triggered before.

---

### Comment 4 - RKSimon

> The code is written 10+ years ago, no idea why it's not triggered before.

The assertion is quite recent 

---

