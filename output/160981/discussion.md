# Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/160981
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2025-09-28T02:14:31Z

## Body

The assertion is hit when trying the following IR:
https://godbolt.org/z/6cbhrrbME
```txt
declare { float, i32 } @llvm.frexp.f32.i32(float)

define { float, i32 } @frexp_pos_denorm() {
  %ret = call { float, i32 } @llvm.frexp.f32.i32(float bitcast (i32 8388607 to float))
  ret { float, i32 } %ret
}
```

Backtrace:
```console
llc: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@frexp_pos_denorm'
 #0 0x0000000004114118 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4114118)
 #1 0x0000000004110fc4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000073b835c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000073b835c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000073b835c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000073b835c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000073b835c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000073b835c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e72129 llvm::SelectionDAG::getConstant(unsigned long, llvm::SDLoc const&, llvm::EVT, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e72129)
 #9 0x0000000003e7f1dc llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::SDVTList, llvm::ArrayRef<llvm::SDValue>, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e7f1dc)
#10 0x0000000003e02cb5 llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e02cb5)
#11 0x0000000003e0a5b3 llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e0a5b3)
#12 0x0000000003e1db12 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e1db12)
#13 0x0000000003eafb5d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafb5d)
#14 0x0000000003eb0b0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb0b0d)
#15 0x0000000003eb20f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb20f5)
#16 0x0000000003e9d8ef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e9d8ef)
#17 0x00000000030129c9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x000000000365b4e8 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365b4e8)
#19 0x000000000365b721 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365b721)
#20 0x000000000365bf8f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365bf8f)
#21 0x00000000008eee3d compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#22 0x00000000007bfba6 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bfba6)
#23 0x000073b835c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x000073b835c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000008e5295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e5295)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
The assertion is hit when trying the following IR:
https://godbolt.org/z/6cbhrrbME
```txt
declare { float, i32 } @<!-- -->llvm.frexp.f32.i32(float)

define { float, i32 } @<!-- -->frexp_pos_denorm() {
  %ret = call { float, i32 } @<!-- -->llvm.frexp.f32.i32(float bitcast (i32 8388607 to float))
  ret { float, i32 } %ret
}
```

Backtrace:
```console
llc: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) &amp;&amp; "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->frexp_pos_denorm'
 #<!-- -->0 0x0000000004114118 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4114118)
 #<!-- -->1 0x0000000004110fc4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000073b835c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000073b835c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000073b835c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000073b835c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000073b835c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000073b835c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e72129 llvm::SelectionDAG::getConstant(unsigned long, llvm::SDLoc const&amp;, llvm::EVT, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e72129)
 #<!-- -->9 0x0000000003e7f1dc llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::SDVTList, llvm::ArrayRef&lt;llvm::SDValue&gt;, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e7f1dc)
#<!-- -->10 0x0000000003e02cb5 llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&amp;, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e02cb5)
#<!-- -->11 0x0000000003e0a5b3 llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e0a5b3)
#<!-- -->12 0x0000000003e1db12 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e1db12)
#<!-- -->13 0x0000000003eafb5d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafb5d)
#<!-- -->14 0x0000000003eb0b0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb0b0d)
#<!-- -->15 0x0000000003eb20f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb20f5)
#<!-- -->16 0x0000000003e9d8ef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e9d8ef)
#<!-- -->17 0x00000000030129c9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x000000000365b4e8 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365b4e8)
#<!-- -->19 0x000000000365b721 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365b721)
#<!-- -->20 0x000000000365bf8f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x365bf8f)
#<!-- -->21 0x00000000008eee3d compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->22 0x00000000007bfba6 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bfba6)
#<!-- -->23 0x000073b835c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x000073b835c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x00000000008e5295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e5295)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - RKSimon

Unlikely to be x86 specific - probably should be getSignedConstant instead of getConstant?

---

### Comment 3 - k-arrows

This `frexp_pos_denorm` test comes from the following:
https://github.com/llvm/llvm-project/blob/main/llvm/test/Transforms/InstCombine/frexp.ll

---

### Comment 4 - XChy

@RKSimon, do you work on this? If not, I will post a patch for it, thanks.

---

