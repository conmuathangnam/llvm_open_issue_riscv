# [Hexagon] Assertion `!MI.isTerminator() && "Cannot insert DBG_VALUE after terminator"` failed

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/176822
**Status:** Closed
**Labels:** llvm:codegen, release:backport, crash-on-valid, regression:22
**Closed Date:** 2026-01-29T15:28:37Z

## Body

```llvm
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux5.10.0-unknown"

declare fastcc i1 @sort.block.block__anon_20404__struct_21687.lessThan()

declare fastcc void @mem.swap__anon_21695()

define fastcc void @sort.block.swap__anon_21725(ptr %0, ptr %1, i32 %2, ptr %3, i32 %4, i32 %5, ptr %6, i1 %7) !dbg !4 {
Entry:
    #dbg_declare(ptr %6, !7, !DIExpression(), !9)
  %8 = call fastcc i1 @sort.block.block__anon_20404__struct_21687.lessThan()
  br i1 %8, label %Block3, label %Else2

Block3:                                           ; preds = %Then4, %Entry
  call fastcc void @mem.swap__anon_21695()
  %9 = icmp ult i32 %4, 8
  br i1 %9, label %Then11, label %Else11

Else2:                                            ; preds = %Entry
  %10 = icmp ult i32 %4, 8
  br i1 %10, label %Then3, label %Else3

Then3:                                            ; preds = %Else2
  br i1 %7, label %Then4, label %Else4

Else3:                                            ; preds = %Else2
  store volatile i32 0, ptr null, align 4
  unreachable

Then4:                                            ; preds = %Then3
  %11 = call fastcc i1 @sort.block.block__anon_20404__struct_21687.lessThan()
  br label %Block3

Else4:                                            ; preds = %Then3
  store volatile i32 0, ptr null, align 4
  unreachable

Then11:                                           ; preds = %Block3
  ret void

Else11:                                           ; preds = %Block3
  store volatile i32 0, ptr null, align 4
  unreachable
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "zig 0.16.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, globals: !2, splitDebugInlining: false)
!1 = !DIFile(filename: "test", directory: "/home/alexrp/Sources/ziglang/zig-llvm22/test")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = distinct !DISubprogram(name: "swap__anon_21725", linkageName: "sort.block.swap__anon_21725", scope: !5, file: !5, line: 1062, type: !6, scopeLine: 1070, flags: DIFlagStaticMember, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !0)
!5 = !DIFile(filename: "block.zig", directory: "/home/alexrp/Sources/ziglang/zig-llvm22/lib/std/sort")
!6 = distinct !DISubroutineType(types: !2)
!7 = !DILocalVariable(name: "context", arg: 5, scope: !4, file: !5, line: 1070, type: !8)
!8 = !DICompositeType(tag: DW_TAG_structure_type, name: "debug.ElfFile.buildSymbolSearchTable__anon_16832.SortContext", scope: !0, size: 96, align: 32, elements: !2)
!9 = !DILocation(line: 1070, column: 8, scope: !4)
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc1
❯ llc reduced.ll
llc: /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1530: {anonymous}::VarLocBasedLDV::insertTransferDebugPair(llvm::MachineInstr&, OpenRangesSet&, TransferMap&, VarLocMap&, {anonymous}::LocIndex, TransferKind, const VarLoc::MachineLoc&, llvm::Register)::<lambda({anonymous}::VarLocBasedLDV::VarLoc&)>: Assertion `!MI.isTerminator() && "Cannot insert DBG_VALUE after terminator"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llc reduced.ll
1.	Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.	Running pass 'Live DEBUG_VALUE analysis' on function '@sort.block.swap__anon_21725'
 #0 0x00007fac98621d2d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:846:3
 #1 0x00007fac9861ee64 llvm::sys::RunSignalHandlers() /home/alexrp/Sources/llvm/llvm/lib/Support/Signals.cpp:108:20
 #2 0x00007fac9861f67c SignalHandler(int, siginfo_t*, void*) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:429:14
 #3 0x00007fac96e45f60 (/lib/x86_64-linux-gnu/libc.so.6+0x45f60)
 #4 0x00007fac96ea5fed __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007fac96ea5fed __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #6 0x00007fac96ea5fed pthread_kill ./nptl/pthread_kill.c:100:10
 #7 0x00007fac96e45e2e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007fac96e28888 internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #9 0x00007fac96e28888 __abort_lock_wrlock ./stdlib/abort.c:62:3
#10 0x00007fac96e28888 abort ./stdlib/abort.c:85:3
#11 0x00007fac96e287f0 __assert_perror_fail ./assert/assert-perr.c:31:1
#12 0x00007fac97f5846a llvm::SmallVectorBase<unsigned int>::set_size(unsigned long) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1530:5
#13 0x00007fac97f5846a push_back /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:567:19
#14 0x00007fac97f5846a (anonymous namespace)::VarLocBasedLDV::insertTransferDebugPair(llvm::MachineInstr&, (anonymous namespace)::VarLocBasedLDV::OpenRangesSet&, llvm::SmallVector<(anonymous namespace)::VarLocBasedLDV::TransferDebugPair, 4u>&, (anonymous namespace)::VarLocBasedLDV::VarLocMap&, (anonymous namespace)::LocIndex, (anonymous namespace)::VarLocBasedLDV::TransferKind, (anonymous namespace)::VarLocBasedLDV::VarLoc::MachineLoc const&, llvm::Register)::'lambda'((anonymous namespace)::VarLocBasedLDV::VarLoc&)::operator()((anonymous namespace)::VarLocBasedLDV::VarLoc&) const (.cold) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1532:24
#15 0x00007fac98f3540a insertTransferDebugPair /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1558:5
#16 0x00007fac98f3540a (anonymous namespace)::VarLocBasedLDV::transferSpillOrRestoreInst(llvm::MachineInstr&, (anonymous namespace)::VarLocBasedLDV::OpenRangesSet&, (anonymous namespace)::VarLocBasedLDV::VarLocMap&, llvm::SmallVector<(anonymous namespace)::VarLocBasedLDV::TransferDebugPair, 4u>&) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1837:28
#17 0x00007fac98f399ba llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineInstr, true, true, void, false, void>, false, false> llvm::MachineInstrBundleIteratorHelper<false>::getBundleFinal<llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineInstr, true, true, void, false, void>, false, false>>(llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineInstr, true, true, void, false, void>, false, false>) /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:62:5
#18 0x00007fac98f399ba void llvm::MachineInstrBundleIteratorHelper<false>::increment<llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineInstr, true, true, void, false, void>, false, false>>(llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineInstr, true, true, void, false, void>, false, false>&) /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:70:18
#19 0x00007fac98f399ba llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:260:20
#20 0x00007fac98f399ba (anonymous namespace)::VarLocBasedLDV::ExtendRanges(llvm::MachineFunction&, llvm::MachineDominatorTree*, bool, unsigned int, unsigned int) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:2350:26
#21 0x00007fac98f202a6 llvm::SmallVectorTemplateCommon<std::unique_ptr<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>, std::default_delete<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>>>, void>::begin() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:273:45
#22 0x00007fac98f202a6 llvm::SmallVectorTemplateCommon<std::unique_ptr<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>, std::default_delete<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>>>, void>::end() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:275:32
#23 0x00007fac98f202a6 llvm::SmallVector<std::unique_ptr<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>, std::default_delete<llvm::DomTreeNodeBase<llvm::MachineBasicBlock>>>, 6u>::~SmallVector() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:1211:24
#24 0x00007fac98f202a6 llvm::DominatorTreeBase<llvm::MachineBasicBlock, false>::~DominatorTreeBase() /home/alexrp/Sources/llvm/llvm/include/llvm/Support/GenericDomTree.h:236:7
#25 0x00007fac98f202a6 llvm::MachineDominatorTree::~MachineDominatorTree() /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineDominators.h:70:7
#26 0x00007fac98f202a6 ~LiveDebugValues /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/LiveDebugValues.cpp:86:3
#27 0x00007fac98f202a6 (anonymous namespace)::LiveDebugValuesLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/LiveDebugValues.cpp:133:10
#28 0x00007fac98baa86a llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#29 0x00007fac987dd52a llvm::FPPassManager::runOnFunction(llvm::Function&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#30 0x00007fac987dd735 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getNext() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#31 0x00007fac987dd735 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>>::getNext() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node.h:108:66
#32 0x00007fac987dd735 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#33 0x00007fac987dd735 llvm::FPPassManager::runOnModule(llvm::Module&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#34 0x00007fac987ddfcf runOnModule /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#35 0x00007fac987ddfcf llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:531:55
#36 0x00005b6722aa09f5 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:874:34
#37 0x00005b6722a93f5c main /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:462:35
#38 0x00007fac96e2a575 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x00007fac96e2a628 call_init ./csu/../csu/libc-start.c:128:20
#40 0x00007fac96e2a628 __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x00005b6722a94ce5 _start (/opt/llvm-22/bin/llc+0x15ce5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Alex Rønne Petersen (alexrp)

<details>
```llvm
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux5.10.0-unknown"

declare fastcc i1 @<!-- -->sort.block.block__anon_20404__struct_21687.lessThan()

declare fastcc void @<!-- -->mem.swap__anon_21695()

define fastcc void @<!-- -->sort.block.swap__anon_21725(ptr %0, ptr %1, i32 %2, ptr %3, i32 %4, i32 %5, ptr %6, i1 %7) !dbg !4 {
Entry:
    #dbg_declare(ptr %6, !7, !DIExpression(), !9)
  %8 = call fastcc i1 @<!-- -->sort.block.block__anon_20404__struct_21687.lessThan()
  br i1 %8, label %Block3, label %Else2

Block3:                                           ; preds = %Then4, %Entry
  call fastcc void @<!-- -->mem.swap__anon_21695()
  %9 = icmp ult i32 %4, 8
  br i1 %9, label %Then11, label %Else11

Else2:                                            ; preds = %Entry
  %10 = icmp ult i32 %4, 8
  br i1 %10, label %Then3, label %Else3

Then3:                                            ; preds = %Else2
  br i1 %7, label %Then4, label %Else4

Else3:                                            ; preds = %Else2
  store volatile i32 0, ptr null, align 4
  unreachable

Then4:                                            ; preds = %Then3
  %11 = call fastcc i1 @<!-- -->sort.block.block__anon_20404__struct_21687.lessThan()
  br label %Block3

Else4:                                            ; preds = %Then3
  store volatile i32 0, ptr null, align 4
  unreachable

Then11:                                           ; preds = %Block3
  ret void

Else11:                                           ; preds = %Block3
  store volatile i32 0, ptr null, align 4
  unreachable
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "zig 0.16.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, globals: !2, splitDebugInlining: false)
!1 = !DIFile(filename: "test", directory: "/home/alexrp/Sources/ziglang/zig-llvm22/test")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = distinct !DISubprogram(name: "swap__anon_21725", linkageName: "sort.block.swap__anon_21725", scope: !5, file: !5, line: 1062, type: !6, scopeLine: 1070, flags: DIFlagStaticMember, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !0)
!5 = !DIFile(filename: "block.zig", directory: "/home/alexrp/Sources/ziglang/zig-llvm22/lib/std/sort")
!6 = distinct !DISubroutineType(types: !2)
!7 = !DILocalVariable(name: "context", arg: 5, scope: !4, file: !5, line: 1070, type: !8)
!8 = !DICompositeType(tag: DW_TAG_structure_type, name: "debug.ElfFile.buildSymbolSearchTable__anon_16832.SortContext", scope: !0, size: 96, align: 32, elements: !2)
!9 = !DILocation(line: 1070, column: 8, scope: !4)
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc1
❯ llc reduced.ll
llc: /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1530: {anonymous}::VarLocBasedLDV::insertTransferDebugPair(llvm::MachineInstr&amp;, OpenRangesSet&amp;, TransferMap&amp;, VarLocMap&amp;, {anonymous}::LocIndex, TransferKind, const VarLoc::MachineLoc&amp;, llvm::Register)::&lt;lambda({anonymous}::VarLocBasedLDV::VarLoc&amp;)&gt;: Assertion `!MI.isTerminator() &amp;&amp; "Cannot insert DBG_VALUE after terminator"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llc reduced.ll
1.	Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.	Running pass 'Live DEBUG_VALUE analysis' on function '@<!-- -->sort.block.swap__anon_21725'
 #<!-- -->0 0x00007fac98621d2d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x00007fac9861ee64 llvm::sys::RunSignalHandlers() /home/alexrp/Sources/llvm/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x00007fac9861f67c SignalHandler(int, siginfo_t*, void*) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:429:14
 #<!-- -->3 0x00007fac96e45f60 (/lib/x86_64-linux-gnu/libc.so.6+0x45f60)
 #<!-- -->4 0x00007fac96ea5fed __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007fac96ea5fed __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #<!-- -->6 0x00007fac96ea5fed pthread_kill ./nptl/pthread_kill.c:100:10
 #<!-- -->7 0x00007fac96e45e2e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007fac96e28888 internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #<!-- -->9 0x00007fac96e28888 __abort_lock_wrlock ./stdlib/abort.c:62:3
#<!-- -->10 0x00007fac96e28888 abort ./stdlib/abort.c:85:3
#<!-- -->11 0x00007fac96e287f0 __assert_perror_fail ./assert/assert-perr.c:31:1
#<!-- -->12 0x00007fac97f5846a llvm::SmallVectorBase&lt;unsigned int&gt;::set_size(unsigned long) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1530:5
#<!-- -->13 0x00007fac97f5846a push_back /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:567:19
#<!-- -->14 0x00007fac97f5846a (anonymous namespace)::VarLocBasedLDV::insertTransferDebugPair(llvm::MachineInstr&amp;, (anonymous namespace)::VarLocBasedLDV::OpenRangesSet&amp;, llvm::SmallVector&lt;(anonymous namespace)::VarLocBasedLDV::TransferDebugPair, 4u&gt;&amp;, (anonymous namespace)::VarLocBasedLDV::VarLocMap&amp;, (anonymous namespace)::LocIndex, (anonymous namespace)::VarLocBasedLDV::TransferKind, (anonymous namespace)::VarLocBasedLDV::VarLoc::MachineLoc const&amp;, llvm::Register)::'lambda'((anonymous namespace)::VarLocBasedLDV::VarLoc&amp;)::operator()((anonymous namespace)::VarLocBasedLDV::VarLoc&amp;) const (.cold) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1532:24
#<!-- -->15 0x00007fac98f3540a insertTransferDebugPair /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1558:5
#<!-- -->16 0x00007fac98f3540a (anonymous namespace)::VarLocBasedLDV::transferSpillOrRestoreInst(llvm::MachineInstr&amp;, (anonymous namespace)::VarLocBasedLDV::OpenRangesSet&amp;, (anonymous namespace)::VarLocBasedLDV::VarLocMap&amp;, llvm::SmallVector&lt;(anonymous namespace)::VarLocBasedLDV::TransferDebugPair, 4u&gt;&amp;) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:1837:28
#<!-- -->17 0x00007fac98f399ba llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineInstr, true, true, void, false, void&gt;, false, false&gt; llvm::MachineInstrBundleIteratorHelper&lt;false&gt;::getBundleFinal&lt;llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineInstr, true, true, void, false, void&gt;, false, false&gt;&gt;(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineInstr, true, true, void, false, void&gt;, false, false&gt;) /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:62:5
#<!-- -->18 0x00007fac98f399ba void llvm::MachineInstrBundleIteratorHelper&lt;false&gt;::increment&lt;llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineInstr, true, true, void, false, void&gt;, false, false&gt;&gt;(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineInstr, true, true, void, false, void&gt;, false, false&gt;&amp;) /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:70:18
#<!-- -->19 0x00007fac98f399ba llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineInstrBundleIterator.h:260:20
#<!-- -->20 0x00007fac98f399ba (anonymous namespace)::VarLocBasedLDV::ExtendRanges(llvm::MachineFunction&amp;, llvm::MachineDominatorTree*, bool, unsigned int, unsigned int) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/VarLocBasedImpl.cpp:2350:26
#<!-- -->21 0x00007fac98f202a6 llvm::SmallVectorTemplateCommon&lt;std::unique_ptr&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;, std::default_delete&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;&gt;&gt;, void&gt;::begin() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:273:45
#<!-- -->22 0x00007fac98f202a6 llvm::SmallVectorTemplateCommon&lt;std::unique_ptr&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;, std::default_delete&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;&gt;&gt;, void&gt;::end() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:275:32
#<!-- -->23 0x00007fac98f202a6 llvm::SmallVector&lt;std::unique_ptr&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;, std::default_delete&lt;llvm::DomTreeNodeBase&lt;llvm::MachineBasicBlock&gt;&gt;&gt;, 6u&gt;::~SmallVector() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:1211:24
#<!-- -->24 0x00007fac98f202a6 llvm::DominatorTreeBase&lt;llvm::MachineBasicBlock, false&gt;::~DominatorTreeBase() /home/alexrp/Sources/llvm/llvm/include/llvm/Support/GenericDomTree.h:236:7
#<!-- -->25 0x00007fac98f202a6 llvm::MachineDominatorTree::~MachineDominatorTree() /home/alexrp/Sources/llvm/llvm/include/llvm/CodeGen/MachineDominators.h:70:7
#<!-- -->26 0x00007fac98f202a6 ~LiveDebugValues /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/LiveDebugValues.cpp:86:3
#<!-- -->27 0x00007fac98f202a6 (anonymous namespace)::LiveDebugValuesLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/LiveDebugValues/LiveDebugValues.cpp:133:10
#<!-- -->28 0x00007fac98baa86a llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->29 0x00007fac987dd52a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#<!-- -->30 0x00007fac987dd735 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getNext() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#<!-- -->31 0x00007fac987dd735 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;&gt;::getNext() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node.h:108:66
#<!-- -->32 0x00007fac987dd735 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->33 0x00007fac987dd735 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#<!-- -->34 0x00007fac987ddfcf runOnModule /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#<!-- -->35 0x00007fac987ddfcf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->36 0x00005b6722aa09f5 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:874:34
#<!-- -->37 0x00005b6722a93f5c main /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:462:35
#<!-- -->38 0x00007fac96e2a575 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x00007fac96e2a628 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x00007fac96e2a628 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x00005b6722a94ce5 _start (/opt/llvm-22/bin/llc+0x15ce5)
```
</details>


---

### Comment 2 - alexrp

Might not actually be specific to Hexagon, but we only see this crash on that target, and in almost all compilations.

---

### Comment 3 - androm3da

@iajbar can you take a look?

---

### Comment 4 - androm3da

Ikhlas, several C and C++ cases in `llvm-test-suite` also evoke this regression.

So - thanks for opening it @alexrp - it's clearly not Zig-specific.

---

### Comment 5 - iajbar

> [@iajbar](https://github.com/iajbar) can you take a look?

Sure, thank you.


---

### Comment 6 - quic-asaravan

https://github.com/llvm/llvm-project/pull/173401/   fixes this issue.

---

### Comment 7 - androm3da

/cherry-pick 02771a3eaff153002ec544c3dc4427d56ccd4456

---

### Comment 8 - llvmbot

/pull-request llvm/llvm-project#178679

---

