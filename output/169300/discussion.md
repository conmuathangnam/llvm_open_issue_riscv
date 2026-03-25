# [AArch64][SVE] LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR <vscale x 10 x i8>

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/169300
**Status:** Closed
**Labels:** duplicate, backend:AArch64, SVE, crash, llvm:SelectionDAG
**Closed Date:** 2025-11-27T13:15:03Z

## Body

View on Compiler Explorer: https://godbolt.org/z/6r17T3jbP

### Input LLVM IR

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define <vscale x 16 x i8> @blurry_8(<vscale x 10 x i8> %0, <vscale x 10 x i8> %1) {
entry:
  %2 = tail call <vscale x 16 x i8> @llvm.vector.insert.nxv16i8.nxv10i8(<vscale x 16 x i8> zeroinitializer, <vscale x 10 x i8> %0, i64 0)
  ret <vscale x 16 x i8> %2
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <vscale x 16 x i8> @llvm.vector.insert.nxv16i8.nxv10i8(<vscale x 16 x i8>, <vscale x 10 x i8>, i64 immarg) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@blurry_8'
 #0 0x0000000004167ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4167ff8)
 #1 0x0000000004164ea4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072a23ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072a23ce969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072a23ce42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072a23ce287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007bbb41 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000040a87b1 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40a87b1)
 #8 0x0000000003fe48fc llvm::DAGTypeLegalizer::WidenVecOp_INSERT_SUBVECTOR(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fe48fc)
 #9 0x0000000003ff01ed llvm::DAGTypeLegalizer::WidenVectorOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ff01ed)
#10 0x0000000003f9223c llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9223c)
#11 0x0000000003f92fa9 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f92fa9)
#12 0x0000000003ef81ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ef81ab)
#13 0x0000000003efbf5d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3efbf5d)
#14 0x0000000003efd545 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3efd545)
#15 0x0000000003ee8d4f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ee8d4f)
#16 0x0000000003053de9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x00000000036a2058 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2058)
#18 0x00000000036a2291 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2291)
#19 0x00000000036a2aff llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2aff)
#20 0x00000000008f5a69 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007c3cd6 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c3cd6)
#22 0x000072a23ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000072a23ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008eb0a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8eb0a5)
Program terminated with signal: SIGSEGV
```

## Comments

### Comment 1 - stevesuzuki-arm

Similar crash : https://godbolt.org/z/h4PnqTddW
```
tail call <vscale x 60 x i8> @llvm.vector.insert.nxv60i8.nxv12i8(<vscale x 60 x i8> zeroinitializer, <vscale x 12 x i8> %0, i64 0)
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
View on Compiler Explorer: https://godbolt.org/z/6r17T3jbP

### Input LLVM IR

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define &lt;vscale x 16 x i8&gt; @<!-- -->blurry_8(&lt;vscale x 10 x i8&gt; %0, &lt;vscale x 10 x i8&gt; %1) {
entry:
  %2 = tail call &lt;vscale x 16 x i8&gt; @<!-- -->llvm.vector.insert.nxv16i8.nxv10i8(&lt;vscale x 16 x i8&gt; zeroinitializer, &lt;vscale x 10 x i8&gt; %0, i64 0)
  ret &lt;vscale x 16 x i8&gt; %2
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare &lt;vscale x 16 x i8&gt; @<!-- -->llvm.vector.insert.nxv16i8.nxv10i8(&lt;vscale x 16 x i8&gt;, &lt;vscale x 10 x i8&gt;, i64 immarg) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->blurry_8'
 #<!-- -->0 0x0000000004167ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4167ff8)
 #<!-- -->1 0x0000000004164ea4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000072a23ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000072a23ce969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000072a23ce42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000072a23ce287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007bbb41 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x00000000040a87b1 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40a87b1)
 #<!-- -->8 0x0000000003fe48fc llvm::DAGTypeLegalizer::WidenVecOp_INSERT_SUBVECTOR(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fe48fc)
 #<!-- -->9 0x0000000003ff01ed llvm::DAGTypeLegalizer::WidenVectorOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ff01ed)
#<!-- -->10 0x0000000003f9223c llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9223c)
#<!-- -->11 0x0000000003f92fa9 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f92fa9)
#<!-- -->12 0x0000000003ef81ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ef81ab)
#<!-- -->13 0x0000000003efbf5d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3efbf5d)
#<!-- -->14 0x0000000003efd545 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3efd545)
#<!-- -->15 0x0000000003ee8d4f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ee8d4f)
#<!-- -->16 0x0000000003053de9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x00000000036a2058 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2058)
#<!-- -->18 0x00000000036a2291 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2291)
#<!-- -->19 0x00000000036a2aff llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36a2aff)
#<!-- -->20 0x00000000008f5a69 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c3cd6 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c3cd6)
#<!-- -->22 0x000072a23ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x000072a23ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008eb0a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8eb0a5)
Program terminated with signal: SIGSEGV
```
</details>


---

