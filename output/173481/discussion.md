# [AArch64] Assertion `PartVT.getFixedSizeInBits() > ValueSize && "lossy conversion of vector to scalar type"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173481
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Reproducer:
https://godbolt.org/z/7M3j7Txre

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:743: void getCopyToPartsVector(llvm::SelectionDAG&, const llvm::SDLoc&, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional<unsigned int>): Assertion `PartVT.getFixedSizeInBits() > ValueSize && "lossy conversion of vector to scalar type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@fpr_to_gpr'
 #0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007dcfc4442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007dcfc44969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007dcfc4442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007dcfc44287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007dcfc442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007dcfc4439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003eec888 getCopyToParts(llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, llvm::Value const*, std::optional<unsigned int>, llvm::ISD::NodeType) SelectionDAGBuilder.cpp:0:0
 #9 0x0000000003eecc39 llvm::RegsForValue::getCopyToRegs(llvm::SDValue, llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue&, llvm::SDValue*, llvm::Value const*, llvm::ISD::NodeType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eecc39)
#10 0x0000000003f13ecd llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f13ecd)
#11 0x0000000003f308af llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f308af)
#12 0x0000000003f44c32 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44c32)
#13 0x0000000003fd774d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd774d)
#14 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#15 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#16 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#17 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#19 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#20 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#21 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#22 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#23 0x00007dcfc4429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x00007dcfc4429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
typedef unsigned char v16qi __attribute__((vector_size(16)));

void
fpr_to_gpr (v16qi q0)
{
  register v16qi x0 asm ("x0");
  x0 = q0;
  asm volatile ("" :: "r" (x0));
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7M3j7Txre

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:743: void getCopyToPartsVector(llvm::SelectionDAG&amp;, const llvm::SDLoc&amp;, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional&lt;unsigned int&gt;): Assertion `PartVT.getFixedSizeInBits() &gt; ValueSize &amp;&amp; "lossy conversion of vector to scalar type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->fpr_to_gpr'
 #<!-- -->0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #<!-- -->1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007dcfc4442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007dcfc44969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007dcfc4442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007dcfc44287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007dcfc442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007dcfc4439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003eec888 getCopyToParts(llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, llvm::Value const*, std::optional&lt;unsigned int&gt;, llvm::ISD::NodeType) SelectionDAGBuilder.cpp:0:0
 #<!-- -->9 0x0000000003eecc39 llvm::RegsForValue::getCopyToRegs(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue&amp;, llvm::SDValue*, llvm::Value const*, llvm::ISD::NodeType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eecc39)
#<!-- -->10 0x0000000003f13ecd llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f13ecd)
#<!-- -->11 0x0000000003f308af llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f308af)
#<!-- -->12 0x0000000003f44c32 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44c32)
#<!-- -->13 0x0000000003fd774d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd774d)
#<!-- -->14 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#<!-- -->15 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#<!-- -->16 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#<!-- -->17 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#<!-- -->19 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#<!-- -->20 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#<!-- -->21 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->22 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#<!-- -->23 0x00007dcfc4429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x00007dcfc4429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
typedef unsigned char v16qi __attribute__((vector_size(16)));

void
fpr_to_gpr (v16qi q0)
{
  register v16qi x0 asm ("x0");
  x0 = q0;
  asm volatile ("" :: "r" (x0));
}
```
</details>


---

