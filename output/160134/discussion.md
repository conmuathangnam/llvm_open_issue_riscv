# [AArch64][SVE] LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/160134
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2025-12-02T12:26:16Z

## Body

https://godbolt.org/z/b8bWsfchn
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define <vscale x 4 x float> @blur2x2(<vscale x 1 x float> %0) {
entry:
  %1 = tail call <vscale x 4 x float> @llvm.vector.insert.nxv4f32.nxv1f32(<vscale x 4 x float> zeroinitializer, <vscale x 1 x float> %0, i64 0)
  ret <vscale x 4 x float> %1
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <vscale x 4 x float> @llvm.vector.insert.nxv4f32.nxv1f32(<vscale x 4 x float>, <vscale x 1 x float>, i64 immarg) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

`llc -mtriple=aarch64-none-elf -mattr=+sve2`

```
LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@blur2x2'
 #0 0x00000000040f7978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40f7978)
 #1 0x00000000040f4824 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007abd6aa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007abd6aa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007abd6aa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007abd6aa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007b98f7 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000004040a61 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4040a61)
 #8 0x0000000003f7ce0c llvm::DAGTypeLegalizer::WidenVecOp_INSERT_SUBVECTOR(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f7ce0c)
 #9 0x0000000003f8857d llvm::DAGTypeLegalizer::WidenVectorOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8857d)
#10 0x0000000003f2a4dc llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2a4dc)
#11 0x0000000003f2b269 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2b269)
#12 0x0000000003e901ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e901ab)
#13 0x0000000003e93f0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e93f0d)
#14 0x0000000003e954f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e954f5)
#15 0x0000000003e80cef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e80cef)
#16 0x0000000003007d29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x0000000003643d88 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643d88)
#18 0x0000000003643fc1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643fc1)
#19 0x000000000364482f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x364482f)
#20 0x00000000008f030d compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007c16de main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c16de)
#22 0x00007abd6aa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x00007abd6aa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008e6765 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6765)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
https://godbolt.org/z/b8bWsfchn
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define &lt;vscale x 4 x float&gt; @<!-- -->blur2x2(&lt;vscale x 1 x float&gt; %0) {
entry:
  %1 = tail call &lt;vscale x 4 x float&gt; @<!-- -->llvm.vector.insert.nxv4f32.nxv1f32(&lt;vscale x 4 x float&gt; zeroinitializer, &lt;vscale x 1 x float&gt; %0, i64 0)
  ret &lt;vscale x 4 x float&gt; %1
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare &lt;vscale x 4 x float&gt; @<!-- -->llvm.vector.insert.nxv4f32.nxv1f32(&lt;vscale x 4 x float&gt;, &lt;vscale x 1 x float&gt;, i64 immarg) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

`opt -mtriple=aarch64-none-elf -mattr=+sve2 -O2`

```
LLVM ERROR: Don't know how to widen the operands for INSERT_SUBVECTOR
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->blur2x2'
 #<!-- -->0 0x00000000040f7978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40f7978)
 #<!-- -->1 0x00000000040f4824 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007abd6aa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007abd6aa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007abd6aa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007abd6aa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007b98f7 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000004040a61 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4040a61)
 #<!-- -->8 0x0000000003f7ce0c llvm::DAGTypeLegalizer::WidenVecOp_INSERT_SUBVECTOR(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f7ce0c)
 #<!-- -->9 0x0000000003f8857d llvm::DAGTypeLegalizer::WidenVectorOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8857d)
#<!-- -->10 0x0000000003f2a4dc llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2a4dc)
#<!-- -->11 0x0000000003f2b269 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2b269)
#<!-- -->12 0x0000000003e901ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e901ab)
#<!-- -->13 0x0000000003e93f0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e93f0d)
#<!-- -->14 0x0000000003e954f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e954f5)
#<!-- -->15 0x0000000003e80cef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e80cef)
#<!-- -->16 0x0000000003007d29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x0000000003643d88 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643d88)
#<!-- -->18 0x0000000003643fc1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643fc1)
#<!-- -->19 0x000000000364482f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x364482f)
#<!-- -->20 0x00000000008f030d compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c16de main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c16de)
#<!-- -->22 0x00007abd6aa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x00007abd6aa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008e6765 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6765)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - paulwalker-arm

This should be fixed by https://github.com/llvm/llvm-project/pull/162308.  Not sure why the godbolt link still fails but the test works locally.

---

