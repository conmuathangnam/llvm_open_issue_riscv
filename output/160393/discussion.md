# [RISC-V] Assertion `VT.getVectorElementType() == N1VT.getVectorElementType() && "Extract subvector VTs must have the same element type!"' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/160393
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-09-24T09:54:25Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define <2 x i16> @main() #0 {
entry:
  %0 = tail call i32 @llvm.vector.reduce.add.v8i32(<8 x i32> zeroinitializer)
  %1 = trunc i32 %0 to i16
  %2 = call i16 @llvm.vector.reduce.add.v8i16(<8 x i16> zeroinitializer)
  %3 = insertelement <2 x i16> zeroinitializer, i16 %2, i64 0
  %4 = insertelement <2 x i16> %3, i16 %1, i64 1
  ret <2 x i16> %4
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v8i32(<8 x i32>) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @llvm.vector.reduce.add.v8i16(<8 x i16>) #1

attributes #0 = { "target-features"="+v" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```
Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8023: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.getVectorElementType() == N1VT.getVectorElementType() && "Extract subvector VTs must have the same element type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@main'
 #0 0x0000599dcc2bb0e2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eef0e2)
 #1 0x0000599dcc2b7ddf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eebddf)
 #2 0x0000599dcc2b7f2c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000071a4c1245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000071a4c129eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000071a4c129eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000071a4c129eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000071a4c124527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000071a4c12288ff abort ./stdlib/abort.c:81:7
 #9 0x000071a4c122881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000071a4c123b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000599dcbfd19ff llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c059ff)
#12 0x0000599dcaaad51d llvm::SelectionDAG::getExtractSubvector(llvm::SDLoc const&, llvm::EVT, llvm::SDValue, unsigned int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x6e151d)
#13 0x0000599dcab2b2de lowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&, llvm::RISCVSubtarget const&) RISCVISelLowering.cpp:0:0
#14 0x0000599dcbeddaba (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#15 0x0000599dcbee0330 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1b14330)
#16 0x0000599dcc01b8d3 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c4f8d3)
#17 0x0000599dcc01f57f llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c5357f)
#18 0x0000599dcc020ad5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c54ad5)
#19 0x0000599dcc00acb7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c3ecb7)
#20 0x0000599dcb0d2c53 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#21 0x0000599dcb77c35e llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b035e)
#22 0x0000599dcb77c7a1 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b07a1)
#23 0x0000599dcb77d0f2 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b10f2)
#24 0x0000599dca90850b compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#25 0x0000599dca8179a7 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x44b9a7)
#26 0x000071a4c122a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x000071a4c122a28b call_init ./csu/../csu/libc-start.c:128:20
#28 0x000071a4c122a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#29 0x0000599dca8fd495 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x531495)
Aborted
```

Godbolt: https://godbolt.org/z/brrE3WjYP

Found via fuzzer

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define &lt;2 x i16&gt; @<!-- -->main() #<!-- -->0 {
entry:
  %0 = tail call i32 @<!-- -->llvm.vector.reduce.add.v8i32(&lt;8 x i32&gt; zeroinitializer)
  %1 = trunc i32 %0 to i16
  %2 = call i16 @<!-- -->llvm.vector.reduce.add.v8i16(&lt;8 x i16&gt; zeroinitializer)
  %3 = insertelement &lt;2 x i16&gt; zeroinitializer, i16 %2, i64 0
  %4 = insertelement &lt;2 x i16&gt; %3, i16 %1, i64 1
  ret &lt;2 x i16&gt; %4
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.vector.reduce.add.v8i32(&lt;8 x i32&gt;) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i16 @<!-- -->llvm.vector.reduce.add.v8i16(&lt;8 x i16&gt;) #<!-- -->1

attributes #<!-- -->0 = { "target-features"="+v" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```
Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8023: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.getVectorElementType() == N1VT.getVectorElementType() &amp;&amp; "Extract subvector VTs must have the same element type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x0000599dcc2bb0e2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eef0e2)
 #<!-- -->1 0x0000599dcc2b7ddf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eebddf)
 #<!-- -->2 0x0000599dcc2b7f2c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000071a4c1245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000071a4c129eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000071a4c129eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000071a4c129eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000071a4c124527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000071a4c12288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000071a4c122881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000071a4c123b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000599dcbfd19ff llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c059ff)
#<!-- -->12 0x0000599dcaaad51d llvm::SelectionDAG::getExtractSubvector(llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, unsigned int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x6e151d)
#<!-- -->13 0x0000599dcab2b2de lowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::RISCVSubtarget const&amp;) RISCVISelLowering.cpp:0:0
#<!-- -->14 0x0000599dcbeddaba (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#<!-- -->15 0x0000599dcbee0330 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1b14330)
#<!-- -->16 0x0000599dcc01b8d3 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c4f8d3)
#<!-- -->17 0x0000599dcc01f57f llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c5357f)
#<!-- -->18 0x0000599dcc020ad5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c54ad5)
#<!-- -->19 0x0000599dcc00acb7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c3ecb7)
#<!-- -->20 0x0000599dcb0d2c53 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->21 0x0000599dcb77c35e llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b035e)
#<!-- -->22 0x0000599dcb77c7a1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b07a1)
#<!-- -->23 0x0000599dcb77d0f2 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x13b10f2)
#<!-- -->24 0x0000599dca90850b compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->25 0x0000599dca8179a7 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x44b9a7)
#<!-- -->26 0x000071a4c122a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x000071a4c122a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x000071a4c122a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->29 0x0000599dca8fd495 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x531495)
Aborted
```

Godbolt: https://godbolt.org/z/brrE3WjYP

Found via fuzzer
</details>


---

### Comment 2 - wangpc-pp

https://github.com/llvm/llvm-project/pull/154450 cc @mshockwave 

---

### Comment 3 - lukel97

This might be a duplicate of https://github.com/llvm/llvm-project/issues/159294

---

### Comment 4 - XChy

Fixed by https://github.com/llvm/llvm-project/pull/159373

---

