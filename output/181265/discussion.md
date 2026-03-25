# LLVM ERROR: Do not know how to soften this operator's operand!

**Author:** abhilashnv
**URL:** https://github.com/llvm/llvm-project/issues/181265
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2026-02-14T09:03:54Z

## Body

Link to CE: https://godbolt.org/z/EEzEaPYa3
The bug is a regression from 10.0.1
Summary: llc crashes with LLVM ERROR: Do not know how to soften this operator's operand! in DAGTypeLegalizer::SoftenFloatOperand when compiling the llvm.llrint.i64.f64 intrinsic for i386 with both SSE2 and x87 disabled (-mattr=-sse2,-x87). The X86 backend incorrectly attempts to use X86ISD::FIST (an x87 instruction) even when x87 is unavailable, instead of falling back to a libcall.
Reproducer:

```llvm
define i64 @test_llrint_f64_expand() {
entry:
  %result = call i64 @llvm.llrint.i64.f64(double 0.000000e+00)
  ret i64 %result
}

declare i64 @llvm.llrint.i64.f64(double)
```

Error:

```
>$ llc -mtriple=i386-unknown-linux-gnu -mattr=-sse2,-x87 reduced.ll
SoftenFloatOperand Op #1: t15: ch = X86ISD::FIST<(store (s64) into %stack.0, align 4)> t0, ConstantFP:f64<0.000000e+00>, FrameIndex:i32<0>

LLVM ERROR: Do not know how to soften this operator's operand!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=i386-unknown-linux-gnu -mattr=-sse2,-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@test_llrint_f64_expand'
 #0 0x00007062e8ec9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007062e8ec99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007062e8ec770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007062e8eca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007062e85b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007062e860eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007062e860eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007062e860eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007062e85b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007062e85988ff abort ./stdlib/abort.c:81:7
#10 0x00007062e8d54f9d llvm::report_fatal_error(llvm::Twine const&, bool) /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x00007062e8d54e12 /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x00007062eeac1f06 llvm::DAGTypeLegalizer::SoftenFloatOperand(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:0:5
#13 0x00007062eeb237d3 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:331:26
#14 0x00007062eeb27e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#15 0x00007062eed4ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#16 0x00007062eed4e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#17 0x00007062eed4e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#18 0x00007062eed4b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#19 0x00007062f0e5bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#20 0x00007062eed49c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#21 0x00007062ed2a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#22 0x00007062e992f51a llvm::FPPassManager::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#23 0x00007062e9933f02 llvm::FPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#24 0x00007062e992fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#25 0x00007062e992f94a llvm::legacy::PassManagerImpl::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#26 0x00007062e99341e1 llvm::legacy::PassManager::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#27 0x000063a73ad115b3 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#28 0x000063a73ad0f225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#29 0x00007062e859a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#30 0x00007062e859a28b call_init ./csu/../csu/libc-start.c:128:20
#31 0x00007062e859a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#32 0x000063a73ad0e995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360
CC: @alidavanian @Vsevolod-Livinskij as collaborators.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (abhilashnv)

<details>
Link to CE: https://godbolt.org/z/EEzEaPYa3
The bug is a regression from 10.0.1
Summary: llc crashes with LLVM ERROR: Do not know how to soften this operator's operand! in DAGTypeLegalizer::SoftenFloatOperand when compiling the llvm.llrint.i64.f64 intrinsic for i386 with both SSE2 and x87 disabled (-mattr=-sse2,-x87). The X86 backend incorrectly attempts to use X86ISD::FIST (an x87 instruction) even when x87 is unavailable, instead of falling back to a libcall.
Reproducer:

define i64 @<!-- -->test_llrint_f64_expand() {
entry:
  %result = call i64 @<!-- -->llvm.llrint.i64.f64(double 0.000000e+00)
  ret i64 %result
}

declare i64 @<!-- -->llvm.llrint.i64.f64(double)


Error:

&gt;$ llc -mtriple=i386-unknown-linux-gnu -mattr=-sse2,-x87 reduced.ll
SoftenFloatOperand Op #<!-- -->1: t15: ch = X86ISD::FIST&lt;(store (s64) into %stack.0, align 4)&gt; t0, ConstantFP:f64&lt;0.000000e+00&gt;, FrameIndex:i32&lt;0&gt;

LLVM ERROR: Do not know how to soften this operator's operand!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=i386-unknown-linux-gnu -mattr=-sse2,-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->test_llrint_f64_expand'
 #<!-- -->0 0x00007062e8ec9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007062e8ec99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007062e8ec770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007062e8eca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007062e85b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007062e860eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007062e860eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007062e860eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007062e85b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007062e85988ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007062e8d54f9d llvm::report_fatal_error(llvm::Twine const&amp;, bool) /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x00007062e8d54e12 /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x00007062eeac1f06 llvm::DAGTypeLegalizer::SoftenFloatOperand(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:0:5
#<!-- -->13 0x00007062eeb237d3 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:331:26
#<!-- -->14 0x00007062eeb27e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#<!-- -->15 0x00007062eed4ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#<!-- -->16 0x00007062eed4e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#<!-- -->17 0x00007062eed4e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#<!-- -->18 0x00007062eed4b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#<!-- -->19 0x00007062f0e5bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#<!-- -->20 0x00007062eed49c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#<!-- -->21 0x00007062ed2a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->22 0x00007062e992f51a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->23 0x00007062e9933f02 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->24 0x00007062e992fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->25 0x00007062e992f94a llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->26 0x00007062e99341e1 llvm::legacy::PassManager::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->27 0x000063a73ad115b3 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#<!-- -->28 0x000063a73ad0f225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#<!-- -->29 0x00007062e859a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x00007062e859a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x00007062e859a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x000063a73ad0e995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)


LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360
CC: @<!-- -->alidavanian @<!-- -->Vsevolod-Livinskij as collaborators.
</details>


---

