# [RISC-V] Assertion `isSimple() && "Expected a SimpleValueType!"' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/157177
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-09-06T05:00:39Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define <vscale x 8 x i1> @main(<120 x i1> %0) #0 {
entry:
  %1 = extractelement <120 x i1> %0, i64 0
  %2 = insertelement <vscale x 8 x i1> zeroinitializer, i1 %1, i64 0
  %3 = shufflevector <vscale x 8 x i1> %2, <vscale x 8 x i1> zeroinitializer, <vscale x 8 x i32> zeroinitializer
  ret <vscale x 8 x i1> %3
}

attributes #0 = { "target-features"="+v" }
```

Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/include/llvm/CodeGen/ValueTypes.h:312: llvm::MVT llvm::EVT::getSimpleVT() const: Assertion `isSimple() && "Expected a SimpleValueType!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@main'
 #0 0x00006213b93c2fc2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec6fc2)
 #1 0x00006213b93bfcbf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec3cbf)
 #2 0x00006213b93bfe0c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000727ebec45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000727ebec9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000727ebec9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000727ebec9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000727ebec4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000727ebec288ff abort ./stdlib/abort.c:81:7
 #9 0x0000727ebec2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000727ebec3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00006213b7bdaa11 (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x6dea11)
#12 0x00006213b7c3f57a llvm::RISCVTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x74357a)
#13 0x00006213b8fb9ed1 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#14 0x00006213b8fbb925 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#15 0x00006213b8fbe4ed llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ac24ed)
#16 0x00006213b9120076 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c24076)
#17 0x00006213b91243af llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c283af)
#18 0x00006213b91258ba llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c298ba)
#19 0x00006213b910fce7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c13ce7)
#20 0x00006213b81f8023 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#21 0x00006213b88875ee llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138b5ee)
#22 0x00006213b8887a31 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138ba31)
#23 0x00006213b8888382 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138c382)
#24 0x00006213b7a3490b compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#25 0x00006213b7943887 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x447887)
#26 0x0000727ebec2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x0000727ebec2a28b call_init ./csu/../csu/libc-start.c:128:20
#28 0x0000727ebec2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#29 0x00006213b7a29895 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x52d895)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/z8v5bd3W7

Found via fuzzer (C program before reduction)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define &lt;vscale x 8 x i1&gt; @<!-- -->main(&lt;120 x i1&gt; %0) #<!-- -->0 {
entry:
  %1 = extractelement &lt;120 x i1&gt; %0, i64 0
  %2 = insertelement &lt;vscale x 8 x i1&gt; zeroinitializer, i1 %1, i64 0
  %3 = shufflevector &lt;vscale x 8 x i1&gt; %2, &lt;vscale x 8 x i1&gt; zeroinitializer, &lt;vscale x 8 x i32&gt; zeroinitializer
  ret &lt;vscale x 8 x i1&gt; %3
}

attributes #<!-- -->0 = { "target-features"="+v" }
```

Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/include/llvm/CodeGen/ValueTypes.h:312: llvm::MVT llvm::EVT::getSimpleVT() const: Assertion `isSimple() &amp;&amp; "Expected a SimpleValueType!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x00006213b93c2fc2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec6fc2)
 #<!-- -->1 0x00006213b93bfcbf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec3cbf)
 #<!-- -->2 0x00006213b93bfe0c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000727ebec45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000727ebec9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000727ebec9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000727ebec9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000727ebec4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000727ebec288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000727ebec2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000727ebec3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00006213b7bdaa11 (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x6dea11)
#<!-- -->12 0x00006213b7c3f57a llvm::RISCVTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x74357a)
#<!-- -->13 0x00006213b8fb9ed1 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->14 0x00006213b8fbb925 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#<!-- -->15 0x00006213b8fbe4ed llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ac24ed)
#<!-- -->16 0x00006213b9120076 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c24076)
#<!-- -->17 0x00006213b91243af llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c283af)
#<!-- -->18 0x00006213b91258ba llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c298ba)
#<!-- -->19 0x00006213b910fce7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1c13ce7)
#<!-- -->20 0x00006213b81f8023 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->21 0x00006213b88875ee llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138b5ee)
#<!-- -->22 0x00006213b8887a31 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138ba31)
#<!-- -->23 0x00006213b8888382 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138c382)
#<!-- -->24 0x00006213b7a3490b compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->25 0x00006213b7943887 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x447887)
#<!-- -->26 0x0000727ebec2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x0000727ebec2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x0000727ebec2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->29 0x00006213b7a29895 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x52d895)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/z8v5bd3W7

Found via fuzzer (C program before reduction)
</details>


---

