# Assertion `LC != RTLIB::UNKNOWN_LIBCALL && "Unsupported FP_EXTEND!"' failed for fpext double to x86_fp80 on AArch64

**Author:** Vsevolod-Livinskij
**URL:** https://github.com/llvm/llvm-project/issues/182449
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2026-02-21T23:22:41Z

## Body

Link to CE: https://godbolt.org/z/b5xefWq5T

Summary: `llc` crashes with an assertion failure `"Unsupported FP_EXTEND!"` in `DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND` when compiling `fpext double to x86_fp80` for AArch64. Ideally, LLVM should emit a user-friendly error instead of crashing.

Reproducer:
```llvm
define x86_fp80 @test_fadd_fma_fpext_long_double(double %u) {
entry:
  %ext = fpext double %u to x86_fp80
  ret x86_fp80 %ext
}
```

Error:
```
$ llc -mtriple=aarch64-unknown-linux-gnu reduced.ll
llc: /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:658: SDValue llvm::DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND(SDNode *): Assertion `LC != RTLIB::UNKNOWN_LIBCALL && "Unsupported FP_EXTEND!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=aarch64-unknown-linux-gnu reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'AArch64 Instruction Selection' on function '@test_fadd_fma_fpext_long_double'
 #0 0x00007ef0eacc9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007ef0eacc99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007ef0eacc770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007ef0eacca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007ef0ea3b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007ef0ea40eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007ef0ea40eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007ef0ea40eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007ef0ea3b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007ef0ea3988ff abort ./stdlib/abort.c:81:7
#10 0x00007ef0ea39881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007ef0ea3ab517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00007ef0f08bc62b llvm::DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND(llvm::SDNode*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:659:38
#13 0x00007ef0f08b8b9c llvm::DAGTypeLegalizer::SoftenFloatResult(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:124:32
#14 0x00007ef0f09234d8 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:273:17
#15 0x00007ef0f0927e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#16 0x00007ef0f0b4ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#17 0x00007ef0f0b4e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#18 0x00007ef0f0b4e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#19 0x00007ef0f0b4b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#20 0x00007ef0f394f925 (anonymous namespace)::AArch64DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/Target/AArch64/AArch64ISelDAGToDAG.cpp:60:5
#21 0x00007ef0f0b49c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#22 0x00007ef0ef0a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#23 0x00007ef0eb72f51a llvm::FPPassManager::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#24 0x00007ef0eb733f02 llvm::FPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#25 0x00007ef0eb72fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#26 0x00007ef0eb72f94a llvm::legacy::PassManagerImpl::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#27 0x00007ef0eb7341e1 llvm::legacy::PassManager::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#28 0x00005b861825b5b3 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#29 0x00005b8618259225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#30 0x00007ef0ea39a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x00007ef0ea39a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x00007ef0ea39a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00005b8618258995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CC: @alidavanian @abhilashnv

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Vsevolod Livinskii (Vsevolod-Livinskij)

<details>
Link to CE: https://godbolt.org/z/b5xefWq5T

Summary: `llc` crashes with an assertion failure `"Unsupported FP_EXTEND!"` in `DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND` when compiling `fpext double to x86_fp80` for AArch64. Ideally, LLVM should emit a user-friendly error instead of crashing.

Reproducer:
```llvm
define x86_fp80 @<!-- -->test_fadd_fma_fpext_long_double(double %u) {
entry:
  %ext = fpext double %u to x86_fp80
  ret x86_fp80 %ext
}
```

Error:
```
$ llc -mtriple=aarch64-unknown-linux-gnu reduced.ll
llc: /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:658: SDValue llvm::DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND(SDNode *): Assertion `LC != RTLIB::UNKNOWN_LIBCALL &amp;&amp; "Unsupported FP_EXTEND!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=aarch64-unknown-linux-gnu reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'AArch64 Instruction Selection' on function '@<!-- -->test_fadd_fma_fpext_long_double'
 #<!-- -->0 0x00007ef0eacc9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007ef0eacc99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007ef0eacc770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007ef0eacca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007ef0ea3b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007ef0ea40eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007ef0ea40eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007ef0ea40eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007ef0ea3b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007ef0ea3988ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007ef0ea39881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007ef0ea3ab517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00007ef0f08bc62b llvm::DAGTypeLegalizer::SoftenFloatRes_FP_EXTEND(llvm::SDNode*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:659:38
#<!-- -->13 0x00007ef0f08b8b9c llvm::DAGTypeLegalizer::SoftenFloatResult(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:124:32
#<!-- -->14 0x00007ef0f09234d8 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:273:17
#<!-- -->15 0x00007ef0f0927e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#<!-- -->16 0x00007ef0f0b4ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#<!-- -->17 0x00007ef0f0b4e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#<!-- -->18 0x00007ef0f0b4e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#<!-- -->19 0x00007ef0f0b4b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#<!-- -->20 0x00007ef0f394f925 (anonymous namespace)::AArch64DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/Target/AArch64/AArch64ISelDAGToDAG.cpp:60:5
#<!-- -->21 0x00007ef0f0b49c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#<!-- -->22 0x00007ef0ef0a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->23 0x00007ef0eb72f51a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->24 0x00007ef0eb733f02 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->25 0x00007ef0eb72fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->26 0x00007ef0eb72f94a llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->27 0x00007ef0eb7341e1 llvm::legacy::PassManager::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->28 0x00005b861825b5b3 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#<!-- -->29 0x00005b8618259225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#<!-- -->30 0x00007ef0ea39a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x00007ef0ea39a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x00007ef0ea39a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00005b8618258995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CC: @<!-- -->alidavanian @<!-- -->abhilashnv
</details>


---

### Comment 2 - bala-bhargav

@Vsevolod-Livinskij 
can i work on this issue ?


---

### Comment 3 - Vsevolod-Livinskij

@bala-bhargav 
Thank you so much for the fix! 

---

