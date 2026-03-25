# [AArch64] Assertion `RegisterVT == PartVT && "Part type doesn't match vector breakdown!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/168009
**Status:** Open
**Labels:** backend:AArch64, crash, llvm:SelectionDAG

## Body

Reproducer:
https://godbolt.org/z/o5e3e9ar1

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:772: void getCopyToPartsVector(llvm::SelectionDAG&, const llvm::SDLoc&, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional<unsigned int>): Assertion `RegisterVT == PartVT && "Part type doesn't match vector breakdown!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@_Z3fooi'
 #0 0x00000000041c2638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41c2638)
 #1 0x00000000041bf4e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007e7e9fa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007e7e9fa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007e7e9fa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007e7e9fa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007e7e9fa2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007e7e9fa39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e682a3 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e682a3)
 #9 0x0000000003e68479 llvm::RegsForValue::getCopyToRegs(llvm::SDValue, llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue&, llvm::SDValue*, llvm::Value const*, llvm::ISD::NodeType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e68479)
#10 0x0000000003e8e64d llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e8e64d)
#11 0x0000000003eabbaf llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eabbaf)
#12 0x0000000003ec0282 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec0282)
#13 0x0000000003f53cdd llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f53cdd)
#14 0x0000000003f54c01 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f54c01)
#15 0x0000000003f56225 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f56225)
#16 0x0000000003f4196f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f4196f)
#17 0x00000000030a1219 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x00000000036f0a06 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0a06)
#19 0x00000000036f0db1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0db1)
#20 0x00000000036f161f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f161f)
#21 0x00000000008fca42 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#22 0x00000000007c84f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c84f9)
#23 0x00007e7e9fa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x00007e7e9fa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000008f1275 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f1275)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/o5e3e9ar1

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:772: void getCopyToPartsVector(llvm::SelectionDAG&amp;, const llvm::SDLoc&amp;, llvm::SDValue, llvm::SDValue*, unsigned int, llvm::MVT, const llvm::Value*, std::optional&lt;unsigned int&gt;): Assertion `RegisterVT == PartVT &amp;&amp; "Part type doesn't match vector breakdown!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z3fooi'
 #<!-- -->0 0x00000000041c2638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41c2638)
 #<!-- -->1 0x00000000041bf4e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007e7e9fa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007e7e9fa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007e7e9fa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007e7e9fa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007e7e9fa2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007e7e9fa39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e682a3 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e682a3)
 #<!-- -->9 0x0000000003e68479 llvm::RegsForValue::getCopyToRegs(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue&amp;, llvm::SDValue*, llvm::Value const*, llvm::ISD::NodeType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e68479)
#<!-- -->10 0x0000000003e8e64d llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e8e64d)
#<!-- -->11 0x0000000003eabbaf llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eabbaf)
#<!-- -->12 0x0000000003ec0282 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec0282)
#<!-- -->13 0x0000000003f53cdd llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f53cdd)
#<!-- -->14 0x0000000003f54c01 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f54c01)
#<!-- -->15 0x0000000003f56225 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f56225)
#<!-- -->16 0x0000000003f4196f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f4196f)
#<!-- -->17 0x00000000030a1219 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x00000000036f0a06 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0a06)
#<!-- -->19 0x00000000036f0db1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0db1)
#<!-- -->20 0x00000000036f161f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f161f)
#<!-- -->21 0x00000000008fca42 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->22 0x00000000007c84f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c84f9)
#<!-- -->23 0x00007e7e9fa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x00007e7e9fa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x00000000008f1275 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f1275)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - clingfei

I am working on it.

---

### Comment 3 - clingfei

bisect to d4826cd324d9a10abdc67c973affa62d36dff4ee

---

