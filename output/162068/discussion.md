# [AArch64][SVE] LLVM ERROR: Cannot select in llvm.vector.interleave3.nxv12i32

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/162068
**Status:** Closed
**Labels:** backend:AArch64, SVE, crash
**Closed Date:** 2025-10-10T12:26:24Z

## Body

View on Compiler Explorer: https://godbolt.org/z/r876G7ePe

Factor of 2 and 4 works, but 3 fails. I expect `st3` instruction is generated in this case.

Similar to [#159801](https://github.com/llvm/llvm-project/issues/159801).

### Input LLVM IR

```llvm
target triple = "aarch64-linux-gnu"

define void @interleave3_test(i32* %ptr, <vscale x 4 x i32> %v0, <vscale x 4 x i32> %v1, <vscale x 4 x i32> %v2) {
  %res = tail call <vscale x 12 x i32> @llvm.vector.interleave3.nxv12i32(<vscale x 4 x i32> %v0, <vscale x 4 x i32> %v1, <vscale x 4 x i32> %v2)
  store <vscale x 12 x i32> %res, ptr %ptr, align 8
  ret void
}
```

### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Cannot select: t9: nxv4i32,nxv4i32,nxv4i32 = vector_interleave t4, t6, t8
  t4: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %1
  t6: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %2
  t8: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %3
In function: interleave3_test
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@interleave3_test'
 #0 0x0000000004126478 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4126478)
 #1 0x0000000004123324 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000759a63e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000759a63e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000759a63e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000759a63e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007b8cf7 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000003eb4601 llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb4601)
 #8 0x0000000003eb8f99 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb8f99)
 #9 0x0000000000ced709 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#10 0x0000000003eb04bb llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb04bb)
#11 0x0000000003ebf6e8 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ebf6e8)
#12 0x0000000003ec2d0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec2d0d)
#13 0x0000000003ec42f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec42f5)
#14 0x0000000003eafcaf llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafcaf)
#15 0x000000000302a9a9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#16 0x000000000366d838 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366d838)
#17 0x000000000366da71 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366da71)
#18 0x000000000366e2df llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366e2df)
#19 0x00000000008efccd compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#20 0x00000000007c0c06 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c0c06)
#21 0x0000759a63e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x0000759a63e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00000000008e6125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6125)
Program terminated with signal: SIGSEGV
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
View on Compiler Explorer: https://godbolt.org/z/r876G7ePe

Factor of 2 and 4 works, but 3 fails. I expect `st3` instruction is generated in this case.

Similar to [#<!-- -->159801](https://github.com/llvm/llvm-project/issues/159801).

### Input LLVM IR

```llvm
target triple = "aarch64-linux-gnu"

define void @<!-- -->interleave3_test(i32* %ptr, &lt;vscale x 4 x i32&gt; %v0, &lt;vscale x 4 x i32&gt; %v1, &lt;vscale x 4 x i32&gt; %v2) {
  %res = tail call &lt;vscale x 12 x i32&gt; @<!-- -->llvm.vector.interleave3.nxv12i32(&lt;vscale x 4 x i32&gt; %v0, &lt;vscale x 4 x i32&gt; %v1, &lt;vscale x 4 x i32&gt; %v2)
  store &lt;vscale x 12 x i32&gt; %res, ptr %ptr, align 8
  ret void
}
```

### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Cannot select: t9: nxv4i32,nxv4i32,nxv4i32 = vector_interleave t4, t6, t8
  t4: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %1
  t6: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %2
  t8: nxv4i32,ch = CopyFromReg t0, Register:nxv4i32 %3
In function: interleave3_test
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->interleave3_test'
 #<!-- -->0 0x0000000004126478 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4126478)
 #<!-- -->1 0x0000000004123324 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000759a63e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000759a63e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000759a63e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000759a63e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007b8cf7 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000003eb4601 llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb4601)
 #<!-- -->8 0x0000000003eb8f99 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb8f99)
 #<!-- -->9 0x0000000000ced709 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#<!-- -->10 0x0000000003eb04bb llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eb04bb)
#<!-- -->11 0x0000000003ebf6e8 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ebf6e8)
#<!-- -->12 0x0000000003ec2d0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec2d0d)
#<!-- -->13 0x0000000003ec42f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ec42f5)
#<!-- -->14 0x0000000003eafcaf llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eafcaf)
#<!-- -->15 0x000000000302a9a9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->16 0x000000000366d838 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366d838)
#<!-- -->17 0x000000000366da71 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366da71)
#<!-- -->18 0x000000000366e2df llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x366e2df)
#<!-- -->19 0x00000000008efccd compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->20 0x00000000007c0c06 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c0c06)
#<!-- -->21 0x0000759a63e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->22 0x0000759a63e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->23 0x00000000008e6125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6125)
Program terminated with signal: SIGSEGV
```

</details>


---

### Comment 2 - paulwalker-arm

Fixed by https://github.com/llvm/llvm-project/issues/162502. When https://github.com/llvm/llvm-project/pull/162475 lands you'll see an improvement to the generated code.

---

