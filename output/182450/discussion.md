# LLVM ERROR: Cannot select: build_pair when compiling x86_fp80 on x86_64 with x87 disabled

**Author:** Vsevolod-Livinskij
**URL:** https://github.com/llvm/llvm-project/issues/182450
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2026-03-23T09:02:06Z

## Body

Link to CE: https://godbolt.org/z/nb9b44WEc

The bug is a regression from 15.0.0

Summary: `llc` crashes with `LLVM ERROR: Cannot select: t7: i64 = build_pair t2, t4` when compiling a function that returns `x86_fp80` on x86_64 with x87 disabled. Ideally, LLVM should emit a user-friendly error instead of crashing.

Reproducer:
```llvm
define x86_fp80 @test_llrint_f80(x86_fp80 %x) {
entry:
  ret x86_fp80 %x
}
```

Error:
```
$ llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
LLVM ERROR: Cannot select: t7: i64 = build_pair t2, t4
  t2: i32,ch = CopyFromReg t0, Register:i32 %0
  t4: i32,ch = CopyFromReg t0, Register:i32 %1
In function: test_llrint_f80
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@test_llrint_f80'
 #0 0x0000708db02c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x0000708db02c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x0000708db02c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000708db02ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x0000708daf9b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000708dafa0eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000708dafa0eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000708dafa0eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000708daf9b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000708daf9988ff abort ./stdlib/abort.c:81:7
#10 0x0000708db0154f9d llvm::report_fatal_error(llvm::Twine const&, bool) /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x0000708db6162f88 /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4609:3
#12 0x0000708db615fd21 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int, unsigned char const*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4511:9
#13 0x0000708db827b1a0 (anonymous namespace)::X86DAGToDAGISel::SelectCode(llvm::SDNode*) /workspace/oss-llvm/build/lib/Target/X86/X86GenDAGISel.inc:374962:1
#14 0x0000708db826f1d7 (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:6733:1
#15 0x0000708db6150ed9 llvm::SelectionDAGISel::DoInstructionSelection() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1397:5
#16 0x0000708db614ff80 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1166:3
#17 0x0000708db614e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#18 0x0000708db614e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#19 0x0000708db614b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#20 0x0000708db825bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#21 0x0000708db6149c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#22 0x0000708db46a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#23 0x0000708db0d2f51a llvm::FPPassManager::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#24 0x0000708db0d33f02 llvm::FPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#25 0x0000708db0d2fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#26 0x0000708db0d2f94a llvm::legacy::PassManagerImpl::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#27 0x0000708db0d341e1 llvm::legacy::PassManager::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#28 0x00006065649335b3 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#29 0x0000606564931225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#30 0x0000708daf99a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x0000708daf99a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x0000708daf99a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x0000606564930995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CC: @alidavanian @abhilashnv

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Vsevolod Livinskii (Vsevolod-Livinskij)

<details>
Link to CE: https://godbolt.org/z/nb9b44WEc

The bug is a regression from 15.0.0

Summary: `llc` crashes with `LLVM ERROR: Cannot select: t7: i64 = build_pair t2, t4` when compiling a function that returns `x86_fp80` on x86_64 with x87 disabled. Ideally, LLVM should emit a user-friendly error instead of crashing.

Reproducer:
```llvm
define x86_fp80 @<!-- -->test_llrint_f80(x86_fp80 %x) {
entry:
  ret x86_fp80 %x
}
```

Error:
```
$ llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
LLVM ERROR: Cannot select: t7: i64 = build_pair t2, t4
  t2: i32,ch = CopyFromReg t0, Register:i32 %0
  t4: i32,ch = CopyFromReg t0, Register:i32 %1
In function: test_llrint_f80
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->test_llrint_f80'
 #<!-- -->0 0x0000708db02c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x0000708db02c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x0000708db02c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000708db02ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000708daf9b5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000708dafa0eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000708dafa0eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000708dafa0eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000708daf9b527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000708daf9988ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000708db0154f9d llvm::report_fatal_error(llvm::Twine const&amp;, bool) /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x0000708db6162f88 /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4609:3
#<!-- -->12 0x0000708db615fd21 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int, unsigned char const*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4511:9
#<!-- -->13 0x0000708db827b1a0 (anonymous namespace)::X86DAGToDAGISel::SelectCode(llvm::SDNode*) /workspace/oss-llvm/build/lib/Target/X86/X86GenDAGISel.inc:374962:1
#<!-- -->14 0x0000708db826f1d7 (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:6733:1
#<!-- -->15 0x0000708db6150ed9 llvm::SelectionDAGISel::DoInstructionSelection() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1397:5
#<!-- -->16 0x0000708db614ff80 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1166:3
#<!-- -->17 0x0000708db614e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#<!-- -->18 0x0000708db614e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#<!-- -->19 0x0000708db614b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#<!-- -->20 0x0000708db825bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#<!-- -->21 0x0000708db6149c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#<!-- -->22 0x0000708db46a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->23 0x0000708db0d2f51a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->24 0x0000708db0d33f02 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->25 0x0000708db0d2fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->26 0x0000708db0d2f94a llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->27 0x0000708db0d341e1 llvm::legacy::PassManager::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->28 0x00006065649335b3 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#<!-- -->29 0x0000606564931225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#<!-- -->30 0x0000708daf99a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x0000708daf99a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x0000708daf99a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x0000606564930995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CC: @<!-- -->alidavanian @<!-- -->abhilashnv
</details>


---

### Comment 2 - bala-bhargav

@Vsevolod-Livinskij @EugeneZelenko 
can i work on this issue?


---

### Comment 3 - EugeneZelenko

@bala-bhargav: Just prepare draft pull request and mention it here, as well as this issue in pull request.

---

### Comment 4 - Vsevolod-Livinskij

@bala-bhargav 
Yeah, definitely! I really appreciate your help with this issue 

---

### Comment 5 - bala-bhargav

Hi @RKSimon 
can you kindly review the PR #183932 


---

