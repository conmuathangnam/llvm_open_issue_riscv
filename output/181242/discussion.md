# Assertion `(getNumMemOperands() != 1 || !getMemOperand()->getType().isValid() || TypeSize::isKnownLE(memvt.getStoreSize(), getMemOperand()->getSize().getValue())) && "Size mismatch!"' failed

**Author:** alidavanian
**URL:** https://github.com/llvm/llvm-project/issues/181242
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

Link to CE: https://godbolt.org/z/j19h14Per

The bug is a regression from 15.0.0

Summary: `llc` crashes with an assertion failure `"Size mismatch!"` in `MemSDNode::MemSDNode` when compiling the `llvm.acos.f80` intrinsic for x86_64 with x87 disabled (`-mattr=-x87`).
FP80 uses 16 bytes, but the softened i80 value is only 10 bytes, which causes the size mismatch.

Reproducer:
```llvm
define void @test_f80_acos_expansion(x86_fp80 %val) {
  %result = call x86_fp80 @llvm.acos.f80(x86_fp80 %val)
  store x86_fp80 %result, ptr null, align 16
  ret void
}

declare x86_fp80 @llvm.acos.f80(x86_fp80)
```

Error:
```
>$ llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
llc: /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:13341: llvm::MemSDNode::MemSDNode(unsigned int, unsigned int, const DebugLoc &, SDVTList, EVT, PointerUnion<MachineMemOperand *, MachineMemOperand **>): Assertion `(getNumMemOperands() != 1 || !getMemOperand()->getType().isValid() || TypeSize::isKnownLE(memvt.getStoreSize(), getMemOperand()->getSize().getValue())) && "Size mismatch!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@test_f80_acos_expansion'
 #0 0x00007e80af4c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007e80af4c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007e80af4c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007e80af4ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007e80aebb5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007e80aec0eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007e80aec0eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007e80aec0eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007e80aebb527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007e80aeb988ff abort ./stdlib/abort.c:81:7
#10 0x00007e80aeb9881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007e80aebab517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00007e80b52fda23 llvm::MemSDNode::MemSDNode(unsigned int, unsigned int, llvm::DebugLoc const&, llvm::SDVTList, llvm::EVT, llvm::PointerUnion<llvm::MachineMemOperand*, llvm::MachineMemOperand**>) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:13342:1
#13 0x00007e80b532e38d llvm::LSBaseSDNode::LSBaseSDNode(llvm::ISD::NodeType, unsigned int, llvm::DebugLoc const&, llvm::SDVTList, llvm::ISD::MemIndexedMode, llvm::EVT, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:2602:9
#14 0x00007e80b532e5bc llvm::StoreSDNode::StoreSDNode(unsigned int, llvm::DebugLoc const&, llvm::SDVTList, llvm::ISD::MemIndexedMode, bool, llvm::EVT, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:2664:9
#15 0x00007e80b5311381 unsigned short llvm::SelectionDAG::getSyntheticNodeSubclassData<llvm::StoreSDNode, llvm::SDVTList&, llvm::ISD::MemIndexedMode&, bool&, llvm::EVT&, llvm::MachineMemOperand*&>(unsigned int, llvm::SDVTList&, llvm::ISD::MemIndexedMode&, bool&, llvm::EVT&, llvm::MachineMemOperand*&) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAG.h:436:10
#16 0x00007e80b52e8b71 llvm::SelectionDAG::getStore(llvm::SDValue, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::EVT, llvm::MachineMemOperand*, llvm::ISD::MemIndexedMode, bool) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:10177:17
#17 0x00007e80b52e84d2 llvm::SelectionDAG::getStore(llvm::SDValue, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:10144:10
#18 0x00007e80b50c436f llvm::DAGTypeLegalizer::SoftenFloatOp_STORE(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:1421:14
#19 0x00007e80b50c212f llvm::DAGTypeLegalizer::SoftenFloatOperand(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:1186:32
#20 0x00007e80b51237d3 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:331:26
#21 0x00007e80b5127e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#22 0x00007e80b534ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#23 0x00007e80b534e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#24 0x00007e80b534e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#25 0x00007e80b534b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#26 0x00007e80b745bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#27 0x00007e80b5349c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#28 0x00007e80b38a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#29 0x00007e80aff2f51a llvm::FPPassManager::runOnFunction(llvm::Function&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#30 0x00007e80aff33f02 llvm::FPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#31 0x00007e80aff2fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#32 0x00007e80aff2f94a llvm::legacy::PassManagerImpl::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#33 0x00007e80aff341e1 llvm::legacy::PassManager::run(llvm::Module&) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#34 0x000058b2e274a5b3 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#35 0x000058b2e2748225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#36 0x00007e80aeb9a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#37 0x00007e80aeb9a28b call_init ./csu/../csu/libc-start.c:128:20
#38 0x00007e80aeb9a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#39 0x000058b2e2747995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CCing @abhilashnv and @Vsevolod-Livinskij as collaborators.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (alidavanian)

<details>
Link to CE: https://godbolt.org/z/j19h14Per

The bug is a regression from 15.0.0

Summary: `llc` crashes with an assertion failure `"Size mismatch!"` in `MemSDNode::MemSDNode` when compiling the `llvm.acos.f80` intrinsic for x86_64 with x87 disabled (`-mattr=-x87`).
FP80 uses 16 bytes, but the softened i80 value is only 10 bytes, which causes the size mismatch.

Reproducer:
```llvm
define void @<!-- -->test_f80_acos_expansion(x86_fp80 %val) {
  %result = call x86_fp80 @<!-- -->llvm.acos.f80(x86_fp80 %val)
  store x86_fp80 %result, ptr null, align 16
  ret void
}

declare x86_fp80 @<!-- -->llvm.acos.f80(x86_fp80)
```

Error:
```
&gt;$ llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
llc: /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:13341: llvm::MemSDNode::MemSDNode(unsigned int, unsigned int, const DebugLoc &amp;, SDVTList, EVT, PointerUnion&lt;MachineMemOperand *, MachineMemOperand **&gt;): Assertion `(getNumMemOperands() != 1 || !getMemOperand()-&gt;getType().isValid() || TypeSize::isKnownLE(memvt.getStoreSize(), getMemOperand()-&gt;getSize().getValue())) &amp;&amp; "Size mismatch!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -mtriple=x86_64-unknown-linux-gnu -mattr=-x87 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->test_f80_acos_expansion'
 #<!-- -->0 0x00007e80af4c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007e80af4c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007e80af4c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007e80af4ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007e80aebb5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007e80aec0eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007e80aec0eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007e80aec0eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007e80aebb527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007e80aeb988ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007e80aeb9881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007e80aebab517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00007e80b52fda23 llvm::MemSDNode::MemSDNode(unsigned int, unsigned int, llvm::DebugLoc const&amp;, llvm::SDVTList, llvm::EVT, llvm::PointerUnion&lt;llvm::MachineMemOperand*, llvm::MachineMemOperand**&gt;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:13342:1
#<!-- -->13 0x00007e80b532e38d llvm::LSBaseSDNode::LSBaseSDNode(llvm::ISD::NodeType, unsigned int, llvm::DebugLoc const&amp;, llvm::SDVTList, llvm::ISD::MemIndexedMode, llvm::EVT, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:2602:9
#<!-- -->14 0x00007e80b532e5bc llvm::StoreSDNode::StoreSDNode(unsigned int, llvm::DebugLoc const&amp;, llvm::SDVTList, llvm::ISD::MemIndexedMode, bool, llvm::EVT, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:2664:9
#<!-- -->15 0x00007e80b5311381 unsigned short llvm::SelectionDAG::getSyntheticNodeSubclassData&lt;llvm::StoreSDNode, llvm::SDVTList&amp;, llvm::ISD::MemIndexedMode&amp;, bool&amp;, llvm::EVT&amp;, llvm::MachineMemOperand*&amp;&gt;(unsigned int, llvm::SDVTList&amp;, llvm::ISD::MemIndexedMode&amp;, bool&amp;, llvm::EVT&amp;, llvm::MachineMemOperand*&amp;) /workspace/oss-llvm/llvm_src/llvm/include/llvm/CodeGen/SelectionDAG.h:436:10
#<!-- -->16 0x00007e80b52e8b71 llvm::SelectionDAG::getStore(llvm::SDValue, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::EVT, llvm::MachineMemOperand*, llvm::ISD::MemIndexedMode, bool) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:10177:17
#<!-- -->17 0x00007e80b52e84d2 llvm::SelectionDAG::getStore(llvm::SDValue, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, llvm::MachineMemOperand*) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:10144:10
#<!-- -->18 0x00007e80b50c436f llvm::DAGTypeLegalizer::SoftenFloatOp_STORE(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:1421:14
#<!-- -->19 0x00007e80b50c212f llvm::DAGTypeLegalizer::SoftenFloatOperand(llvm::SDNode*, unsigned int) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp:1186:32
#<!-- -->20 0x00007e80b51237d3 llvm::DAGTypeLegalizer::run() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:331:26
#<!-- -->21 0x00007e80b5127e4b llvm::SelectionDAG::LegalizeTypes() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1046:34
#<!-- -->22 0x00007e80b534ed64 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1023:13
#<!-- -->23 0x00007e80b534e653 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#<!-- -->24 0x00007e80b534e228 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:11
#<!-- -->25 0x00007e80b534b07c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:658:3
#<!-- -->26 0x00007e80b745bb02 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:187:7
#<!-- -->27 0x00007e80b5349c71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:3
#<!-- -->28 0x00007e80b38a639b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->29 0x00007e80aff2f51a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->30 0x00007e80aff33f02 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->31 0x00007e80aff2fdcb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->32 0x00007e80aff2f94a llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->33 0x00007e80aff341e1 llvm::legacy::PassManager::run(llvm::Module&amp;) /workspace/oss-llvm/llvm_src/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->34 0x000058b2e274a5b3 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:870:9
#<!-- -->35 0x000058b2e2748225 main /workspace/oss-llvm/llvm_src/llvm/tools/llc/llc.cpp:458:13
#<!-- -->36 0x00007e80aeb9a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->37 0x00007e80aeb9a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->38 0x00007e80aeb9a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->39 0x000058b2e2747995 _start (/workspace/oss-llvm/bin/bin/llc+0x4f995)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CCing @<!-- -->abhilashnv and @<!-- -->Vsevolod-Livinskij as collaborators.
</details>


---

### Comment 2 - abhishek-kaushik22

I don't think this should be classified as a regression, as `-mattr=-x87 ` was specified but the generated assembly from 15.0.0 uses x87 instructions
```nasm
test_f80_acos_expansion:                # @test_f80_acos_expansion
        sub     rsp, 24
        fld     tbyte ptr [rsp + 32]
        fstp    tbyte ptr [rsp]
        call    llvm.acos.f80@PLT
        fstp    tbyte ptr [0]
        add     rsp, 24
        ret
```

---

