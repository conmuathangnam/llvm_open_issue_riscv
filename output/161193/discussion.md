# [[clang::musttail]] triggers backend crash in llc on MIPS

**Author:** petrovicslavisa
**URL:** https://github.com/llvm/llvm-project/issues/161193
**Status:** Closed
**Labels:** backend:MIPS, clang:codegen, crash
**Closed Date:** 2026-01-27T18:55:24Z

## Body

Using [[clang::musttail]] when targeting mipsel causes llc to crash during tail call elimination.

Minimal example:
https://godbolt.org/z/fPb79sbM8

Steps to reproduce locally:

`clang++ -target mipsel -S -emit-llvm musttail_example.cpp -o musttail_example.bc`
`llc musttail_example.bc`

llc aborts with:

```
LLVM ERROR: failed to perform tail call elimination on a call site marked musttail
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: ./LLVM-19.1.7-Linux-X64/bin/llc musttail_example.bc
1.      Running pass 'Function Pass Manager' on module 'musttail_example.bc'.
2.      Running pass 'MIPS DAG->DAG Pattern Instruction Selection' on function '@_Z13func_musttailv'
 #0 0x000058cd02e8afa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0cfa8)
 #1 0x000058cd02e8843e llvm::sys::RunSignalHandlers() (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0a43e)
 #2 0x000058cd02e8b718 SignalHandler(int) Signals.cpp:0:0
 #3 0x0000758ef5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000758ef58969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000758ef58969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000758ef58969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000758ef5842476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000758ef58287f3 abort ./stdlib/abort.c:81:7
 #9 0x000058cd02de62b8 llvm::report_fatal_error(llvm::Twine const&, bool) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b682b8)
#10 0x000058cd02de60f6 (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b680f6)
#11 0x000058cd00faa659 (./LLVM-19.1.7-Linux-X64/bin/llc+0x3d2c659)
#12 0x000058cd02bb6469 llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&) const (./LLVM-19.1.7-Linux-X64/bin/llc+0x5938469)
#13 0x000058cd02bd8b3f llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&, llvm::BasicBlock const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x595ab3f)
#14 0x000058cd02bbec55 llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5940c55)
#15 0x000058cd02ba7f3c llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5929f3c)
#16 0x000058cd02b9b9bd llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x591d9bd)
#17 0x000058cd02c5e6a5 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, bool&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59e06a5)
#18 0x000058cd02c5d354 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59df354)
#19 0x000058cd02c5b08f llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59dd08f)
#20 0x000058cd010235d6 llvm::MipsSEDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) MipsSEISelDAGToDAG.cpp:0:0
#21 0x000058cd02c59345 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59db345)
#22 0x000058cd01f6d579 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x4cef579)
#23 0x000058cd02418a5f llvm::FPPassManager::runOnFunction(llvm::Function&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519aa5f)
#24 0x000058cd024228a3 llvm::FPPassManager::runOnModule(llvm::Module&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x51a48a3)
#25 0x000058cd02419977 llvm::legacy::PassManagerImpl::run(llvm::Module&) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519b977)
#26 0x000058cd0031c5a5 main (./LLVM-19.1.7-Linux-X64/bin/llc+0x309e5a5)
#27 0x0000758ef5829d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#28 0x0000758ef5829e40 call_init ./csu/../csu/libc-start.c:128:20
#29 0x0000758ef5829e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#30 0x000058cd0030b3a5 _start (./LLVM-19.1.7-Linux-X64/bin/llc+0x308d3a5)
Aborted

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Slavisa Petrovic (petrovicslavisa)

<details>
Using [[clang::musttail]] when targeting mipsel causes llc to crash during tail call elimination.

Minimal example:
https://godbolt.org/z/fPb79sbM8

Steps to reproduce locally:

`clang++ -target mipsel -S -emit-llvm musttail_example.cpp -o musttail_example.bc`
`llc musttail_example.bc`

llc aborts with:

```
LLVM ERROR: failed to perform tail call elimination on a call site marked musttail
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: ./LLVM-19.1.7-Linux-X64/bin/llc musttail_example.bc
1.      Running pass 'Function Pass Manager' on module 'musttail_example.bc'.
2.      Running pass 'MIPS DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z13func_musttailv'
 #<!-- -->0 0x000058cd02e8afa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0cfa8)
 #<!-- -->1 0x000058cd02e8843e llvm::sys::RunSignalHandlers() (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0a43e)
 #<!-- -->2 0x000058cd02e8b718 SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x0000758ef5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000758ef58969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000758ef58969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000758ef58969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000758ef5842476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000758ef58287f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000058cd02de62b8 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b682b8)
#<!-- -->10 0x000058cd02de60f6 (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b680f6)
#<!-- -->11 0x000058cd00faa659 (./LLVM-19.1.7-Linux-X64/bin/llc+0x3d2c659)
#<!-- -->12 0x000058cd02bb6469 llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&amp;) const (./LLVM-19.1.7-Linux-X64/bin/llc+0x5938469)
#<!-- -->13 0x000058cd02bd8b3f llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&amp;, llvm::BasicBlock const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x595ab3f)
#<!-- -->14 0x000058cd02bbec55 llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&amp;, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5940c55)
#<!-- -->15 0x000058cd02ba7f3c llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5929f3c)
#<!-- -->16 0x000058cd02b9b9bd llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x591d9bd)
#<!-- -->17 0x000058cd02c5e6a5 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59e06a5)
#<!-- -->18 0x000058cd02c5d354 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59df354)
#<!-- -->19 0x000058cd02c5b08f llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59dd08f)
#<!-- -->20 0x000058cd010235d6 llvm::MipsSEDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) MipsSEISelDAGToDAG.cpp:0:0
#<!-- -->21 0x000058cd02c59345 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59db345)
#<!-- -->22 0x000058cd01f6d579 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x4cef579)
#<!-- -->23 0x000058cd02418a5f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519aa5f)
#<!-- -->24 0x000058cd024228a3 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x51a48a3)
#<!-- -->25 0x000058cd02419977 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519b977)
#<!-- -->26 0x000058cd0031c5a5 main (./LLVM-19.1.7-Linux-X64/bin/llc+0x309e5a5)
#<!-- -->27 0x0000758ef5829d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->28 0x0000758ef5829e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x0000758ef5829e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->30 0x000058cd0030b3a5 _start (./LLVM-19.1.7-Linux-X64/bin/llc+0x308d3a5)
Aborted
</details>


---

### Comment 2 - alexrp

Related: #107730

---

### Comment 3 - brad0

@petrovicslavisa Please test the PR.  https://github.com/llvm/llvm-project/pull/161860

---

### Comment 4 - petrovicslavisa

With the changes from that PR, everything works properly.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Slavisa Petrovic (petrovicslavisa)

<details>
Using [[clang::musttail]] when targeting mipsel causes llc to crash during tail call elimination.

Minimal example:
https://godbolt.org/z/fPb79sbM8

Steps to reproduce locally:

`clang++ -target mipsel -S -emit-llvm musttail_example.cpp -o musttail_example.bc`
`llc musttail_example.bc`

llc aborts with:

```
LLVM ERROR: failed to perform tail call elimination on a call site marked musttail
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: ./LLVM-19.1.7-Linux-X64/bin/llc musttail_example.bc
1.      Running pass 'Function Pass Manager' on module 'musttail_example.bc'.
2.      Running pass 'MIPS DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z13func_musttailv'
 #<!-- -->0 0x000058cd02e8afa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0cfa8)
 #<!-- -->1 0x000058cd02e8843e llvm::sys::RunSignalHandlers() (./LLVM-19.1.7-Linux-X64/bin/llc+0x5c0a43e)
 #<!-- -->2 0x000058cd02e8b718 SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x0000758ef5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000758ef58969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000758ef58969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000758ef58969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000758ef5842476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000758ef58287f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000058cd02de62b8 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b682b8)
#<!-- -->10 0x000058cd02de60f6 (./LLVM-19.1.7-Linux-X64/bin/llc+0x5b680f6)
#<!-- -->11 0x000058cd00faa659 (./LLVM-19.1.7-Linux-X64/bin/llc+0x3d2c659)
#<!-- -->12 0x000058cd02bb6469 llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&amp;) const (./LLVM-19.1.7-Linux-X64/bin/llc+0x5938469)
#<!-- -->13 0x000058cd02bd8b3f llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&amp;, llvm::BasicBlock const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x595ab3f)
#<!-- -->14 0x000058cd02bbec55 llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&amp;, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5940c55)
#<!-- -->15 0x000058cd02ba7f3c llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x5929f3c)
#<!-- -->16 0x000058cd02b9b9bd llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x591d9bd)
#<!-- -->17 0x000058cd02c5e6a5 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59e06a5)
#<!-- -->18 0x000058cd02c5d354 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59df354)
#<!-- -->19 0x000058cd02c5b08f llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59dd08f)
#<!-- -->20 0x000058cd010235d6 llvm::MipsSEDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) MipsSEISelDAGToDAG.cpp:0:0
#<!-- -->21 0x000058cd02c59345 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x59db345)
#<!-- -->22 0x000058cd01f6d579 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x4cef579)
#<!-- -->23 0x000058cd02418a5f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519aa5f)
#<!-- -->24 0x000058cd024228a3 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x51a48a3)
#<!-- -->25 0x000058cd02419977 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (./LLVM-19.1.7-Linux-X64/bin/llc+0x519b977)
#<!-- -->26 0x000058cd0031c5a5 main (./LLVM-19.1.7-Linux-X64/bin/llc+0x309e5a5)
#<!-- -->27 0x0000758ef5829d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->28 0x0000758ef5829e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x0000758ef5829e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->30 0x000058cd0030b3a5 _start (./LLVM-19.1.7-Linux-X64/bin/llc+0x308d3a5)
Aborted
</details>


---

