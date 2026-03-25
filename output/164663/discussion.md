# typed load hits assert and `Do not know how to promote this operator!`

**Author:** folkertdev
**URL:** https://github.com/llvm/llvm-project/issues/164663
**Status:** Open
**Labels:** crash, llvm:SelectionDAG

## Body

This issue was reported in https://github.com/rust-lang/rust/issues/124093, but I have reduced the LLVM IR a bit and diagnosed that the issue is a typed load that rust emits. It seems like this is an oversight in a `switch`. The error occurs at least for x86_64 and aarch64, so I think it's not target-specific.

---

Firstly, I reduced the program just looking for any crash. With debug assertions the inputs hit an assertion in a cast. Here is a minimal example:

https://godbolt.org/z/K9d1qTerb

```llvm
define void @a(ptr %b) {
  call { ptr, i1 } @llvm.type.checked.load(ptr %b, i32 4, metadata !"")
  ret void
}
```

```
# | llc: /home/folkertdev/c/llvm-project/llvm/include/llvm/Support/Casting.h:573: decltype(auto) llvm::cast(From*) [with To = MDNode; From = Metadata]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# | Stack dump:
# | 0.	Program arguments: /home/folkertdev/c/llvm-project/build/bin/llc -print-before=isel -print-after=isel
# | 1.	Running pass 'Function Pass Manager' on module '<stdin>'.
```

---

Next I turned that debug assertion off and minimized the two rust examples. The LLVM IR is kind of ill-formed because it uses identifiers that are not defined. Specifically the `call i1 %40()` and similar are apparently needed to inhibit some transformation/optimization.

https://godbolt.org/z/6YYEs3Msc
https://godbolt.org/z/71hd8EYd1

Based on the error location, maybe a typed load needs some special handling here?

https://github.com/llvm/llvm-project/blob/866879f80342b857a8b911c804189c43ac4fc334/llvm/lib/CodeGen/SelectionDAG/LegalizeIntegerTypes.cpp#L42-L58

Here is a local backtrace with some more helpful line numbers.

```
PromoteIntegerResult #1: t9: i64,i1 = llvm.type.checked.load TargetConstant:i64<363>, t5, Constant:i32<0>, MDNode:ch<0x5aaad8feae88>

LLVM ERROR: Do not know how to promote this operator!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/folkertdev/c/llvm-project/build/bin/llc
1.	Running pass 'Function Pass Manager' on module '<stdin>'.
2.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@foo'
 #0 0x00005aaad4fa4af2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x00005aaad4fa4f65 PrintStackTraceSignalHandler(void*) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005aaad4fa20dd llvm::sys::RunSignalHandlers() /home/folkertdev/c/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #3 0x00005aaad4fa432f SignalHandler(int, siginfo_t*, void*) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x000077dbbaa45320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #5 0x000077dbbaa9eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000077dbbaa9eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000077dbbaa9eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000077dbbaa4526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000077dbbaa288ff abort ./stdlib/abort.c:81:7
#10 0x00005aaad4ea995e llvm::report_fatal_error(llvm::Twine const&, bool) /home/folkertdev/c/llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:9
#11 0x00005aaad4ea9735 llvm::report_fatal_error(llvm::StringRef, bool) /home/folkertdev/c/llvm-project/llvm/lib/Support/ErrorHandling.cpp:99:68
#12 0x00005aaad4cf2094 llvm::DAGTypeLegalizer::PromoteIntegerResult(llvm::SDNode*, unsigned int) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeIntegerTypes.cpp:59:58
#13 0x00005aaad4ce6624 llvm::DAGTypeLegalizer::run() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:265:17
#14 0x00005aaad4ceac90 llvm::SelectionDAG::LegalizeTypes() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1054:37
#15 0x00005aaad4cc192c llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:992:36
#16 0x00005aaad4cc0e23 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:890:1
#17 0x00005aaad4cc7152 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1922:33
#18 0x00005aaad4cbf4ac llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:628:7
#19 0x00005aaad2461193 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/folkertdev/c/llvm-project/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:188:0
#20 0x00005aaad4cbe11c llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:390:40
#21 0x00005aaad3585d95 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#22 0x00005aaad3ef1e4c llvm::FPPassManager::runOnFunction(llvm::Function&) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:20
#23 0x00005aaad3ef2122 llvm::FPPassManager::runOnModule(llvm::Module&) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#24 0x00005aaad3ef2581 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:20
#25 0x00005aaad3eed548 llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:13
#26 0x00005aaad3ef2e77 llvm::legacy::PassManager::run(llvm::Module&) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1641:1
#27 0x00005aaad1594d0d compileModule(char**, llvm::LLVMContext&) /home/folkertdev/c/llvm-project/llvm/tools/llc/llc.cpp:833:34
#28 0x00005aaad15921dd main /home/folkertdev/c/llvm-project/llvm/tools/llc/llc.cpp:446:35
#29 0x000077dbbaa2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#30 0x000077dbbaa2a28b call_init ./csu/../csu/libc-start.c:128:20
#31 0x000077dbbaa2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#32 0x00005aaad1590a25 _start (/home/folkertdev/c/llvm-project/build/bin/llc+0x528aa25)
fish: Job 1, '/home/folkertdev/c/llvm-project…' terminated by signal SIGABRT (Abort)
```




## Comments

### Comment 1 - topperc

Intrinsics are not supposed to reach that code. If SelectionDAG is supposed to handle that intrinsic, it must be converted to an `ISD::` node by SelectionDAGBuilder.

LangRef suggests this intrinsic is supposed to be removed by an earlier pass before it can get to SelectionDAG: "The virtual call optimization pass will optimize away llvm.type.checked.load intrinsics associated with devirtualized calls, thereby removing the type check in cases where it is not needed to enforce the control flow integrity constraint."

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Folkert de Vries (folkertdev)

<details>
This issue was reported in https://github.com/rust-lang/rust/issues/124093, but I have reduced the LLVM IR a bit and diagnosed that the issue is a typed load that rust emits. It seems like this is an oversight in a `switch`. The error occurs at least for x86_64 and aarch64, so I think it's not target-specific.

---

Firstly, I reduced the program just looking for any crash. With debug assertions the inputs hit an assertion in a cast. Here is a minimal example:

https://godbolt.org/z/K9d1qTerb

```llvm
define void @<!-- -->a(ptr %b) {
  call { ptr, i1 } @<!-- -->llvm.type.checked.load(ptr %b, i32 4, metadata !"")
  ret void
}
```

```
# | llc: /home/folkertdev/c/llvm-project/llvm/include/llvm/Support/Casting.h:573: decltype(auto) llvm::cast(From*) [with To = MDNode; From = Metadata]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# | Stack dump:
# | 0.	Program arguments: /home/folkertdev/c/llvm-project/build/bin/llc -print-before=isel -print-after=isel
# | 1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
```

---

Next I turned that debug assertion off and minimized the two rust examples. The LLVM IR is kind of ill-formed because it uses identifiers that are not defined. Specifically the `call i1 %40()` and similar are apparently needed to inhibit some transformation/optimization.

https://godbolt.org/z/6YYEs3Msc
https://godbolt.org/z/71hd8EYd1

Based on the error location, maybe a typed load needs some special handling here?

https://github.com/llvm/llvm-project/blob/866879f80342b857a8b911c804189c43ac4fc334/llvm/lib/CodeGen/SelectionDAG/LegalizeIntegerTypes.cpp#L42-L58

Here is a local backtrace with some more helpful line numbers.

```
PromoteIntegerResult #<!-- -->1: t9: i64,i1 = llvm.type.checked.load TargetConstant:i64&lt;363&gt;, t5, Constant:i32&lt;0&gt;, MDNode:ch&lt;0x5aaad8feae88&gt;

LLVM ERROR: Do not know how to promote this operator!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/folkertdev/c/llvm-project/build/bin/llc
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->foo'
 #<!-- -->0 0x00005aaad4fa4af2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00005aaad4fa4f65 PrintStackTraceSignalHandler(void*) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x00005aaad4fa20dd llvm::sys::RunSignalHandlers() /home/folkertdev/c/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->3 0x00005aaad4fa432f SignalHandler(int, siginfo_t*, void*) /home/folkertdev/c/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->4 0x000077dbbaa45320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->5 0x000077dbbaa9eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000077dbbaa9eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000077dbbaa9eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000077dbbaa4526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000077dbbaa288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005aaad4ea995e llvm::report_fatal_error(llvm::Twine const&amp;, bool) /home/folkertdev/c/llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:9
#<!-- -->11 0x00005aaad4ea9735 llvm::report_fatal_error(llvm::StringRef, bool) /home/folkertdev/c/llvm-project/llvm/lib/Support/ErrorHandling.cpp:99:68
#<!-- -->12 0x00005aaad4cf2094 llvm::DAGTypeLegalizer::PromoteIntegerResult(llvm::SDNode*, unsigned int) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeIntegerTypes.cpp:59:58
#<!-- -->13 0x00005aaad4ce6624 llvm::DAGTypeLegalizer::run() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:265:17
#<!-- -->14 0x00005aaad4ceac90 llvm::SelectionDAG::LegalizeTypes() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:1054:37
#<!-- -->15 0x00005aaad4cc192c llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:992:36
#<!-- -->16 0x00005aaad4cc0e23 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:890:1
#<!-- -->17 0x00005aaad4cc7152 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1922:33
#<!-- -->18 0x00005aaad4cbf4ac llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:628:7
#<!-- -->19 0x00005aaad2461193 (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/folkertdev/c/llvm-project/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp:188:0
#<!-- -->20 0x00005aaad4cbe11c llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:390:40
#<!-- -->21 0x00005aaad3585d95 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/folkertdev/c/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->22 0x00005aaad3ef1e4c llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:20
#<!-- -->23 0x00005aaad3ef2122 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#<!-- -->24 0x00005aaad3ef2581 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:20
#<!-- -->25 0x00005aaad3eed548 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:13
#<!-- -->26 0x00005aaad3ef2e77 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/folkertdev/c/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1641:1
#<!-- -->27 0x00005aaad1594d0d compileModule(char**, llvm::LLVMContext&amp;) /home/folkertdev/c/llvm-project/llvm/tools/llc/llc.cpp:833:34
#<!-- -->28 0x00005aaad15921dd main /home/folkertdev/c/llvm-project/llvm/tools/llc/llc.cpp:446:35
#<!-- -->29 0x000077dbbaa2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x000077dbbaa2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x000077dbbaa2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x00005aaad1590a25 _start (/home/folkertdev/c/llvm-project/build/bin/llc+0x528aa25)
fish: Job 1, '/home/folkertdev/c/llvm-project…' terminated by signal SIGABRT (Abort)
```



</details>


---

### Comment 3 - folkertdev

Interesting. So the initial failing assert disappears when I add:

```llvm
!llvm.module.flags = !{!0, !1, !2}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 2, !"RtLibUseGOT", i32 1}
!2 = !{i32 1, !"Virtual Function Elim", i32 1}
```

but the latter 2 examples still fail even with those lines. So I think that indicates some bug in the virtual call optimization pass.

---

