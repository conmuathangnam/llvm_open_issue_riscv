# [AArch64] Assertion `DAG.getTargetLoweringInfo().isTypeLegal(PartVT) && "Copying to an illegal type!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/167462
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2025-11-13T16:33:19Z

## Body

Reproducer:
https://godbolt.org/z/EeTvsdW5h

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:520: void getCopyToParts(llvm::SelectionDAG&, const llvm::SDLoc&, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional<unsigned int>, llvm::ISD::NodeType): Assertion `DAG.getTargetLoweringInfo().isTypeLegal(PartVT) && "Copying to an illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@_Z3foov'
 #0 0x00000000041b1ab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41b1ab8)
 #1 0x00000000041ae964 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000079ba03e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000079ba03e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000079ba03e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000079ba03e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000079ba03e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000079ba03e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e56147 getCopyToParts(llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, llvm::Value const*, std::optional<unsigned int>, llvm::ISD::NodeType) SelectionDAGBuilder.cpp:0:0
 #9 0x0000000003e5d6eb llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e5d6eb)
#10 0x0000000003e67dd4 llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e67dd4)
#11 0x0000000003e84fc5 llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e84fc5)
#12 0x0000000003e9b2cc llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e9b2cc)
#13 0x0000000003eafa02 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafa02)
#14 0x0000000003f4290d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f4290d)
#15 0x0000000003f433d8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f433d8)
#16 0x0000000003f44e55 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44e55)
#17 0x0000000003f3059f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3059f)
#18 0x0000000003090fc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x00000000036e16a6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e16a6)
#20 0x00000000036e1a51 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e1a51)
#21 0x00000000036e22bf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e22bf)
#22 0x00000000008fe402 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#23 0x00000000007c9cb9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c9cb9)
#24 0x000079ba03e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000079ba03e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000008f2c35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f2c35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Comes from the following program:
```cpp
void f(__SVCount_t, __SVCount_t);

void foo() { f(__SVCount_t(), __SVCount_t()); }
```

This crashes with `clang++ -c -mcpu=neoverse-v1 bug.cpp`.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/EeTvsdW5h

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:520: void getCopyToParts(llvm::SelectionDAG&amp;, const llvm::SDLoc&amp;, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional&lt;unsigned int&gt;, llvm::ISD::NodeType): Assertion `DAG.getTargetLoweringInfo().isTypeLegal(PartVT) &amp;&amp; "Copying to an illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z3foov'
 #<!-- -->0 0x00000000041b1ab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41b1ab8)
 #<!-- -->1 0x00000000041ae964 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000079ba03e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000079ba03e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000079ba03e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000079ba03e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000079ba03e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000079ba03e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e56147 getCopyToParts(llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, llvm::Value const*, std::optional&lt;unsigned int&gt;, llvm::ISD::NodeType) SelectionDAGBuilder.cpp:0:0
 #<!-- -->9 0x0000000003e5d6eb llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e5d6eb)
#<!-- -->10 0x0000000003e67dd4 llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&amp;, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e67dd4)
#<!-- -->11 0x0000000003e84fc5 llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&amp;, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e84fc5)
#<!-- -->12 0x0000000003e9b2cc llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e9b2cc)
#<!-- -->13 0x0000000003eafa02 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafa02)
#<!-- -->14 0x0000000003f4290d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f4290d)
#<!-- -->15 0x0000000003f433d8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f433d8)
#<!-- -->16 0x0000000003f44e55 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44e55)
#<!-- -->17 0x0000000003f3059f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3059f)
#<!-- -->18 0x0000000003090fc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x00000000036e16a6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e16a6)
#<!-- -->20 0x00000000036e1a51 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e1a51)
#<!-- -->21 0x00000000036e22bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36e22bf)
#<!-- -->22 0x00000000008fe402 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->23 0x00000000007c9cb9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c9cb9)
#<!-- -->24 0x000079ba03e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x000079ba03e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000008f2c35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f2c35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Comes from the following program:
```cpp
void f(__SVCount_t, __SVCount_t);

void foo() { f(__SVCount_t(), __SVCount_t()); }
```

This crashes with `clang++ -c -mcpu=neoverse-v1 bug.cpp`.

</details>


---

