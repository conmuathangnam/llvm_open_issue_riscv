# [RISCV] Triggered assertion `Insert subvector VTs must have the same element type!"

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/159294
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-09-24T09:46:53Z

## Body

Reproducer: https://godbolt.org/z/WxbKq6TKj
Testcase:
```llvm
define <16 x i16> @func_1(<2 x i32> %0, <2 x i32> %1, <2 x i32> %2) {
entry:
  %vecext3 = extractelement <2 x i32> %0, i32 0
  %conv4 = trunc i32 %vecext3 to i16
  %vecinit5 = insertelement <16 x i16> <i16 0, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison>, i16 %conv4, i32 1
  %vecext7 = extractelement <2 x i32> %1, i32 0
  %conv8 = trunc i32 %vecext7 to i16
  %vecinit9 = insertelement <16 x i16> %vecinit5, i16 %conv8, i32 2
  %vecext59 = extractelement <2 x i32> %2, i32 0
  %conv60 = trunc i32 %vecext59 to i16
  %vecinit61 = insertelement <16 x i16> %vecinit9, i16 %conv60, i32 0
  ret <16 x i16> %vecinit61
}
```
Output:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8274: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.getVectorElementType() == N2VT.getVectorElementType() && "Insert subvector VTs must have the same element type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -march=riscv64 -mattr=+v <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@func_1'
 #0 0x00000000040e8b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40e8b28)
 #1 0x00000000040e59d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078d135242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078d1352969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078d135242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078d1352287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000078d13522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000078d135239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e4dd77 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e4dd77)
 #9 0x0000000001dd3768 llvm::SelectionDAG::getInsertSubvector(llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1dd3768)
#10 0x0000000001e4a9ee lowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&, llvm::RISCVSubtarget const&) RISCVISelLowering.cpp:0:0
#11 0x0000000001e4cecc llvm::RISCVTargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1e4cecc)
#12 0x0000000003d60fd8 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#13 0x0000000003d6462e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3d6462e)
#14 0x0000000003e8232d llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e8232d)
#15 0x0000000003e85afd llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e85afd)
#16 0x0000000003e870e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e870e5)
#17 0x0000000003e728df llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e728df)
#18 0x0000000002fff4b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x0000000003636ff8 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3636ff8)
#20 0x0000000003637231 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3637231)
#21 0x0000000003637a9f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3637a9f)
#22 0x00000000008eb50d compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#23 0x00000000007bc706 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bc706)
#24 0x000078d135229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000078d135229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000008e1965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e1965)
Program terminated with signal: SIGSEGV
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/WxbKq6TKj
Testcase:
```llvm
define &lt;16 x i16&gt; @<!-- -->func_1(&lt;2 x i32&gt; %0, &lt;2 x i32&gt; %1, &lt;2 x i32&gt; %2) {
entry:
  %vecext3 = extractelement &lt;2 x i32&gt; %0, i32 0
  %conv4 = trunc i32 %vecext3 to i16
  %vecinit5 = insertelement &lt;16 x i16&gt; &lt;i16 0, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison, i16 poison&gt;, i16 %conv4, i32 1
  %vecext7 = extractelement &lt;2 x i32&gt; %1, i32 0
  %conv8 = trunc i32 %vecext7 to i16
  %vecinit9 = insertelement &lt;16 x i16&gt; %vecinit5, i16 %conv8, i32 2
  %vecext59 = extractelement &lt;2 x i32&gt; %2, i32 0
  %conv60 = trunc i32 %vecext59 to i16
  %vecinit61 = insertelement &lt;16 x i16&gt; %vecinit9, i16 %conv60, i32 0
  ret &lt;16 x i16&gt; %vecinit61
}
```
Output:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8274: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.getVectorElementType() == N2VT.getVectorElementType() &amp;&amp; "Insert subvector VTs must have the same element type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -march=riscv64 -mattr=+v &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->func_1'
 #<!-- -->0 0x00000000040e8b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40e8b28)
 #<!-- -->1 0x00000000040e59d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000078d135242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000078d1352969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000078d135242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000078d1352287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000078d13522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000078d135239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e4dd77 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e4dd77)
 #<!-- -->9 0x0000000001dd3768 llvm::SelectionDAG::getInsertSubvector(llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1dd3768)
#<!-- -->10 0x0000000001e4a9ee lowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::RISCVSubtarget const&amp;) RISCVISelLowering.cpp:0:0
#<!-- -->11 0x0000000001e4cecc llvm::RISCVTargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1e4cecc)
#<!-- -->12 0x0000000003d60fd8 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#<!-- -->13 0x0000000003d6462e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3d6462e)
#<!-- -->14 0x0000000003e8232d llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e8232d)
#<!-- -->15 0x0000000003e85afd llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e85afd)
#<!-- -->16 0x0000000003e870e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e870e5)
#<!-- -->17 0x0000000003e728df llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e728df)
#<!-- -->18 0x0000000002fff4b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x0000000003636ff8 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3636ff8)
#<!-- -->20 0x0000000003637231 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3637231)
#<!-- -->21 0x0000000003637a9f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3637a9f)
#<!-- -->22 0x00000000008eb50d compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->23 0x00000000007bc706 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bc706)
#<!-- -->24 0x000078d135229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x000078d135229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000008e1965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e1965)
Program terminated with signal: SIGSEGV
```
</details>


---

