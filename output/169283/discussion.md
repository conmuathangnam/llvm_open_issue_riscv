# `ppc64-lv2` target is non-functional

**Author:** e820
**URL:** https://github.com/llvm/llvm-project/issues/169283
**Status:** Open
**Labels:** backend:PowerPC, crash

## Body

The `ppc64-lv2` target more or less seems completely dysfunctional, I found a previous related issue that is still open (#52826), and realize there's probably not much interest in fixing it due to it being a fairly niche platform.
That said, I'm still including a reduced IR test case that crashes, in case there's any future interest in addressing it - if not, maybe the target should simply be removed outright.
```llvm
; ModuleID = 'bugpoint-reduced-instructions.bc'
source_filename = "reduced.ll"
target datalayout = "E-m:e-p:32:32-Fi64-i64:64-i128:128-n32:64"
target triple = "ppc64-unknown-lv2-unknown"

@__anon_1820 = external constant [7 x i8]

define { ptr, i64 } @__ubsan_handle_add_overflow() {
Entry:
  ret { ptr, i64 } { ptr @__anon_1820, i64 6 }
}
```

And here's the `bugpoint` output for the test case:
```
Error running tool:
  /usr/bin/llc -o bugpoint-test-program-081c012.bc-ade953a.llc.s bugpoint-test-program-081c012.bc
LLVM ERROR: Cannot select: 0x560754081000: i64 = any_extend 0x560754081310
  0x560754081310: i64,ch = PPCISD::TOC_ENTRY<(load (s64) from got)> TargetGlobalAddress:i32<ptr @__anon_1820> 0, Register:i64 $x2
In function: __ubsan_handle_add_overflow
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/llc -o bugpoint-test-program-081c012.bc-ade953a.llc.s bugpoint-test-program-081c012.bc
1.	Running pass 'Function Pass Manager' on module 'bugpoint-test-program-081c012.bc'.
2.	Running pass 'PowerPC DAG->DAG Pattern Instruction Selection' on function '@__ubsan_handle_add_overflow'
 #0 0x00007f632c70832d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:834:22
 #1 0x00007f632c705407 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Signals.cpp:104:20
 #2 0x00007f632c705407 SignalHandler /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:409:31
 #3 0x00007f632b63e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #4 0x00007f632b69890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #5 0x00007f632b63e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #6 0x00007f632b62557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #7 0x00007f632c33f1be std::mutex::lock() /usr/include/c++/15.2.1/bits/std_mutex.h:119:22
 #8 0x00007f632c33f1be std::lock_guard<std::mutex>::lock_guard(std::mutex&) /usr/include/c++/15.2.1/bits/std_mutex.h:252:23
 #9 0x00007f632c33f1be llvm::install_bad_alloc_error_handler(void (*)(void*, char const*, bool), void*) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/ErrorHandling.cpp:162:61
#10 0x00007f632d47cb0e llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4439:14
#11 0x00007f632d47ed23 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4344:24
#12 0x00007f6330bf9818 llvm::TrackingMDRef::untrack() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:89:9
#13 0x00007f6330bf9818 llvm::TrackingMDRef::~TrackingMDRef() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:55:29
#14 0x00007f6330bf9818 llvm::TypedTrackingMDRef<llvm::MDNode>::~TypedTrackingMDRef() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:106:26
#15 0x00007f6330bf9818 llvm::DebugLoc::~DebugLoc() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/DebugLoc.h:124:9
#16 0x00007f6330bf9818 llvm::SDLoc::~SDLoc() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/CodeGen/SelectionDAGNodes.h:1225:7
#17 0x00007f6330bf9818 Select /usr/src/debug/llvm/llvm-21.1.5.src/lib/Target/PowerPC/PPCISelDAGToDAG.cpp:6378:1
#18 0x00007f632d477499 llvm::SelectionDAGISel::DoInstructionSelection() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1273:28
#19 0x00007f632d484533 llvm::TimeRegion::~TimeRegion() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/Support/Timer.h:162:9
#20 0x00007f632d484533 llvm::NamedRegionTimer::~NamedRegionTimer() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/Support/Timer.h:170:8
#21 0x00007f632d484533 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1112:3
#22 0x00007f632d48795a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1891:33
#23 0x00007f632d4891a4 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:605:22
#24 0x00007f6330c018d1 runOnMachineFunction /usr/src/debug/llvm/llvm-21.1.5.src/lib/Target/PowerPC/PPCISelDAGToDAG.cpp:172:5
#25 0x00007f632d47706b llvm::OptLevelChanger::~OptLevelChanger() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:267:7
#26 0x00007f632d47706b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:373:1
#27 0x00007f632d47706b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:345:6
#28 0x00007f632cd10b75 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/MachineFunctionPass.cpp:108:30
#29 0x00007f632c9050a3 llvm::FPPassManager::runOnFunction(llvm::Function&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1398:40
#30 0x00007f632c905283 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<false, void>, false>::getNext() const /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_node_base.h:28:38
#31 0x00007f632c905283 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, false, false, void, false, void>>::getNext() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_node.h:119:66
#32 0x00007f632c905283 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, false, false, void, false, void>, false, false>::operator++() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_iterator.h:187:25
#33 0x00007f632c905283 llvm::FPPassManager::runOnModule(llvm::Module&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1443:22
#34 0x00007f632c9045a2 runOnModule /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1513:38
#35 0x00007f632c9045a2 llvm::legacy::PassManagerImpl::run(llvm::Module&) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:531:55
#36 0x00005607288ca8f9 (/usr/bin/llc+0x1d8f9)
#37 0x00005607288bf6e1 main (/usr/bin/llc+0x126e1)
#38 0x00007f632b627635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x00007f632b6276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#40 0x00007f632b6276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#41 0x00005607288c0ee5 _start (/usr/bin/llc+0x13ee5)
*** Debugging code generator crash!

*** Attempting to reduce the number of function attributes in the testcase
Checking for crash with changing conditionals to always jump to true: Entry: <llc><crash>
Checking for crash with changing conditionals to always jump to false: Entry: <llc><crash>
Checking for crash with only these blocks: Entry: <llc><crash>
Checking for crash with CFG simplifying: Entry: <llc><crash>

*** Attempting to reduce testcase by deleting instructions: Simplification Level #1

*** Attempting to reduce testcase by deleting instructions: Simplification Level #0
Checking for crash with metadata retained from 1 instruction: <llc><crash>
Emitted bitcode to 'bugpoint-reduced-instructions.bc'

*** Attempting to strip the debug info: <llc><crash>

*** Attempting to strip the debug type info: <llc><crash>

*** Attempting to remove named metadata: Emitted bitcode to 'bugpoint-reduced-named-md.bc'

*** Attempting to perform final cleanups: <llc>
Emitted bitcode to 'bugpoint-reduced-simplified.bc'
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Laurin-Luis Lehning (e820)

<details>
The `ppc64-lv2` target more or less seems completely dysfunctional, I found a previous related issue that is still open (#<!-- -->52826), and realize there's probably not much interest in fixing it due to it being a fairly niche platform.
That said, I'm still including a reduced IR test case that crashes, in case there's any future interest in addressing it - if not, maybe the target should simply be removed outright.
```llvm
; ModuleID = 'bugpoint-reduced-instructions.bc'
source_filename = "reduced.ll"
target datalayout = "E-m:e-p:32:32-Fi64-i64:64-i128:128-n32:64"
target triple = "ppc64-unknown-lv2-unknown"

@<!-- -->__anon_1820 = external constant [7 x i8]

define { ptr, i64 } @<!-- -->__ubsan_handle_add_overflow() {
Entry:
  ret { ptr, i64 } { ptr @<!-- -->__anon_1820, i64 6 }
}
```

And here's the `bugpoint` output for the test case:
```
Error running tool:
  /usr/bin/llc -o bugpoint-test-program-081c012.bc-ade953a.llc.s bugpoint-test-program-081c012.bc
LLVM ERROR: Cannot select: 0x560754081000: i64 = any_extend 0x560754081310
  0x560754081310: i64,ch = PPCISD::TOC_ENTRY&lt;(load (s64) from got)&gt; TargetGlobalAddress:i32&lt;ptr @<!-- -->__anon_1820&gt; 0, Register:i64 $x2
In function: __ubsan_handle_add_overflow
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/llc -o bugpoint-test-program-081c012.bc-ade953a.llc.s bugpoint-test-program-081c012.bc
1.	Running pass 'Function Pass Manager' on module 'bugpoint-test-program-081c012.bc'.
2.	Running pass 'PowerPC DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->__ubsan_handle_add_overflow'
 #<!-- -->0 0x00007f632c70832d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007f632c705407 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007f632c705407 SignalHandler /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:409:31
 #<!-- -->3 0x00007f632b63e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->4 0x00007f632b69890c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f632b63e3a0 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x00007f632b62557a abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #<!-- -->7 0x00007f632c33f1be std::mutex::lock() /usr/include/c++/15.2.1/bits/std_mutex.h:119:22
 #<!-- -->8 0x00007f632c33f1be std::lock_guard&lt;std::mutex&gt;::lock_guard(std::mutex&amp;) /usr/include/c++/15.2.1/bits/std_mutex.h:252:23
 #<!-- -->9 0x00007f632c33f1be llvm::install_bad_alloc_error_handler(void (*)(void*, char const*, bool), void*) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/ErrorHandling.cpp:162:61
#<!-- -->10 0x00007f632d47cb0e llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4439:14
#<!-- -->11 0x00007f632d47ed23 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4344:24
#<!-- -->12 0x00007f6330bf9818 llvm::TrackingMDRef::untrack() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:89:9
#<!-- -->13 0x00007f6330bf9818 llvm::TrackingMDRef::~TrackingMDRef() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:55:29
#<!-- -->14 0x00007f6330bf9818 llvm::TypedTrackingMDRef&lt;llvm::MDNode&gt;::~TypedTrackingMDRef() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/TrackingMDRef.h:106:26
#<!-- -->15 0x00007f6330bf9818 llvm::DebugLoc::~DebugLoc() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/IR/DebugLoc.h:124:9
#<!-- -->16 0x00007f6330bf9818 llvm::SDLoc::~SDLoc() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/CodeGen/SelectionDAGNodes.h:1225:7
#<!-- -->17 0x00007f6330bf9818 Select /usr/src/debug/llvm/llvm-21.1.5.src/lib/Target/PowerPC/PPCISelDAGToDAG.cpp:6378:1
#<!-- -->18 0x00007f632d477499 llvm::SelectionDAGISel::DoInstructionSelection() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1273:28
#<!-- -->19 0x00007f632d484533 llvm::TimeRegion::~TimeRegion() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/Support/Timer.h:162:9
#<!-- -->20 0x00007f632d484533 llvm::NamedRegionTimer::~NamedRegionTimer() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/Support/Timer.h:170:8
#<!-- -->21 0x00007f632d484533 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1112:3
#<!-- -->22 0x00007f632d48795a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1891:33
#<!-- -->23 0x00007f632d4891a4 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:605:22
#<!-- -->24 0x00007f6330c018d1 runOnMachineFunction /usr/src/debug/llvm/llvm-21.1.5.src/lib/Target/PowerPC/PPCISelDAGToDAG.cpp:172:5
#<!-- -->25 0x00007f632d47706b llvm::OptLevelChanger::~OptLevelChanger() /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:267:7
#<!-- -->26 0x00007f632d47706b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:373:1
#<!-- -->27 0x00007f632d47706b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:345:6
#<!-- -->28 0x00007f632cd10b75 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->29 0x00007f632c9050a3 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1398:40
#<!-- -->30 0x00007f632c905283 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;false, void&gt;, false&gt;::getNext() const /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_node_base.h:28:38
#<!-- -->31 0x00007f632c905283 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, false, false, void, false, void&gt;&gt;::getNext() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_node.h:119:66
#<!-- -->32 0x00007f632c905283 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, false, false, void, false, void&gt;, false, false&gt;::operator++() /usr/src/debug/llvm/llvm-21.1.5.src/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->33 0x00007f632c905283 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1443:22
#<!-- -->34 0x00007f632c9045a2 runOnModule /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:1513:38
#<!-- -->35 0x00007f632c9045a2 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->36 0x00005607288ca8f9 (/usr/bin/llc+0x1d8f9)
#<!-- -->37 0x00005607288bf6e1 main (/usr/bin/llc+0x126e1)
#<!-- -->38 0x00007f632b627635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x00007f632b6276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->40 0x00007f632b6276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->41 0x00005607288c0ee5 _start (/usr/bin/llc+0x13ee5)
*** Debugging code generator crash!

*** Attempting to reduce the number of function attributes in the testcase
Checking for crash with changing conditionals to always jump to true: Entry: &lt;llc&gt;&lt;crash&gt;
Checking for crash with changing conditionals to always jump to false: Entry: &lt;llc&gt;&lt;crash&gt;
Checking for crash with only these blocks: Entry: &lt;llc&gt;&lt;crash&gt;
Checking for crash with CFG simplifying: Entry: &lt;llc&gt;&lt;crash&gt;

*** Attempting to reduce testcase by deleting instructions: Simplification Level #<!-- -->1

*** Attempting to reduce testcase by deleting instructions: Simplification Level #<!-- -->0
Checking for crash with metadata retained from 1 instruction: &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-instructions.bc'

*** Attempting to strip the debug info: &lt;llc&gt;&lt;crash&gt;

*** Attempting to strip the debug type info: &lt;llc&gt;&lt;crash&gt;

*** Attempting to remove named metadata: Emitted bitcode to 'bugpoint-reduced-named-md.bc'

*** Attempting to perform final cleanups: &lt;llc&gt;
Emitted bitcode to 'bugpoint-reduced-simplified.bc'
```
</details>


---

### Comment 2 - nemanjai

I agree that this not likely to get any attention from anyone that I know that works on PPC. 32-bit pointers in 64-bit mode are an odd thing and lead to bizarre contradictions such as that TOC_ENTRY node that causes the crash.

---

