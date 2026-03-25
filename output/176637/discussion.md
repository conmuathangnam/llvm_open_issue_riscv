# [RISCV] Assertion `castIsValid(getOpcode(), S, Ty) && "Illegal ZExt"` failed for `fptosi fp128 ... to i96`

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/176637
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, release:cherry-pick-failed, crash-on-valid, regression:22
**Closed Date:** 2026-01-29T15:16:16Z

## Body

```llvm
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux5.10.0-musl"

define i96 @__fixtfei(fp128 %0) {
Entry:
  %1 = fptosi fp128 %0 to i96
  ret i96 %1
}
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc1
❯ llc -O0 reduced.ll
llc: /home/alexrp/Sources/llvm/llvm/lib/IR/Instructions.cpp:3421: llvm::ZExtInst::ZExtInst(llvm::Value*, llvm::Type*, const llvm::Twine&, llvm::InsertPosition): Assertion `castIsValid(getOpcode(), S, Ty) && "Illegal ZExt"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -O0 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Expand IR instructions' on function '@__fixtfei'
 #0 0x00007c2048621d2d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:846:3
 #1 0x00007c204861ee64 llvm::sys::RunSignalHandlers() /home/alexrp/Sources/llvm/llvm/lib/Support/Signals.cpp:108:20
 #2 0x00007c204861f67c SignalHandler(int, siginfo_t*, void*) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:429:14
 #3 0x00007c2046e45f60 (/lib/x86_64-linux-gnu/libc.so.6+0x45f60)
 #4 0x00007c2046ea5fed __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007c2046ea5fed __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #6 0x00007c2046ea5fed pthread_kill ./nptl/pthread_kill.c:100:10
 #7 0x00007c2046e45e2e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007c2046e28888 internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #9 0x00007c2046e28888 __abort_lock_wrlock ./stdlib/abort.c:62:3
#10 0x00007c2046e28888 abort ./stdlib/abort.c:85:3
#11 0x00007c2046e287f0 __assert_perror_fail ./assert/assert-perr.c:31:1
#12 0x00007c2047edd9fd (/opt/llvm-22/bin/../lib/libLLVM.so.22.1-rc1+0x8dd9fd)
#13 0x00007c2048666f43 llvm::ZExtInst* llvm::IRBuilderBase::Insert<llvm::ZExtInst>(llvm::ZExtInst*, llvm::Twine const&) const /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:173:5
#14 0x00007c2048666f43 llvm::IRBuilderBase::CreateZExt(llvm::Value*, llvm::Type*, llvm::Twine const&, bool) /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:2091:28
#15 0x00007c2048a3dc5e expandFPToI(llvm::Instruction*) /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:2356:68
#16 0x00007c2048a43949 llvm::SmallVectorBase<unsigned int>::empty() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:83:46
#17 0x00007c2048a43949 runImpl(llvm::Function&, llvm::TargetLowering const&, llvm::LibcallLoweringInfo const&, llvm::AssumptionCache*) (.isra.0) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/ExpandIRInsts.cpp:1091:25
#18 0x00007c20487dd52a llvm::FPPassManager::runOnFunction(llvm::Function&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#19 0x00007c20487dd735 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getNext() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#20 0x00007c20487dd735 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>>::getNext() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node.h:108:66
#21 0x00007c20487dd735 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#22 0x00007c20487dd735 llvm::FPPassManager::runOnModule(llvm::Module&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#23 0x00007c20487ddfcf runOnModule /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#24 0x00007c20487ddfcf llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:531:55
#25 0x0000604b2cb4a9f5 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:874:34
#26 0x0000604b2cb3df5c main /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:462:35
#27 0x00007c2046e2a575 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007c2046e2a628 call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007c2046e2a628 __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x0000604b2cb3ece5 _start (/opt/llvm-22/bin/llc+0x15ce5)
fish: Job 1, 'llc -O0 reduced.ll' terminated by signal SIGABRT (Abort)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Rønne Petersen (alexrp)

<details>
```llvm
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux5.10.0-musl"

define i96 @<!-- -->__fixtfei(fp128 %0) {
Entry:
  %1 = fptosi fp128 %0 to i96
  ret i96 %1
}
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc1
❯ llc -O0 reduced.ll
llc: /home/alexrp/Sources/llvm/llvm/lib/IR/Instructions.cpp:3421: llvm::ZExtInst::ZExtInst(llvm::Value*, llvm::Type*, const llvm::Twine&amp;, llvm::InsertPosition): Assertion `castIsValid(getOpcode(), S, Ty) &amp;&amp; "Illegal ZExt"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc -O0 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Expand IR instructions' on function '@<!-- -->__fixtfei'
 #<!-- -->0 0x00007c2048621d2d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x00007c204861ee64 llvm::sys::RunSignalHandlers() /home/alexrp/Sources/llvm/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x00007c204861f67c SignalHandler(int, siginfo_t*, void*) /home/alexrp/Sources/llvm/llvm/lib/Support/Unix/Signals.inc:429:14
 #<!-- -->3 0x00007c2046e45f60 (/lib/x86_64-linux-gnu/libc.so.6+0x45f60)
 #<!-- -->4 0x00007c2046ea5fed __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007c2046ea5fed __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #<!-- -->6 0x00007c2046ea5fed pthread_kill ./nptl/pthread_kill.c:100:10
 #<!-- -->7 0x00007c2046e45e2e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007c2046e28888 internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #<!-- -->9 0x00007c2046e28888 __abort_lock_wrlock ./stdlib/abort.c:62:3
#<!-- -->10 0x00007c2046e28888 abort ./stdlib/abort.c:85:3
#<!-- -->11 0x00007c2046e287f0 __assert_perror_fail ./assert/assert-perr.c:31:1
#<!-- -->12 0x00007c2047edd9fd (/opt/llvm-22/bin/../lib/libLLVM.so.22.1-rc1+0x8dd9fd)
#<!-- -->13 0x00007c2048666f43 llvm::ZExtInst* llvm::IRBuilderBase::Insert&lt;llvm::ZExtInst&gt;(llvm::ZExtInst*, llvm::Twine const&amp;) const /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:173:5
#<!-- -->14 0x00007c2048666f43 llvm::IRBuilderBase::CreateZExt(llvm::Value*, llvm::Type*, llvm::Twine const&amp;, bool) /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:2091:28
#<!-- -->15 0x00007c2048a3dc5e expandFPToI(llvm::Instruction*) /home/alexrp/Sources/llvm/llvm/include/llvm/IR/IRBuilder.h:2356:68
#<!-- -->16 0x00007c2048a43949 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/SmallVector.h:83:46
#<!-- -->17 0x00007c2048a43949 runImpl(llvm::Function&amp;, llvm::TargetLowering const&amp;, llvm::LibcallLoweringInfo const&amp;, llvm::AssumptionCache*) (.isra.0) /home/alexrp/Sources/llvm/llvm/lib/CodeGen/ExpandIRInsts.cpp:1091:25
#<!-- -->18 0x00007c20487dd52a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#<!-- -->19 0x00007c20487dd735 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getNext() const /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#<!-- -->20 0x00007c20487dd735 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;&gt;::getNext() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_node.h:108:66
#<!-- -->21 0x00007c20487dd735 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;::operator++() /home/alexrp/Sources/llvm/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->22 0x00007c20487dd735 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#<!-- -->23 0x00007c20487ddfcf runOnModule /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#<!-- -->24 0x00007c20487ddfcf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/alexrp/Sources/llvm/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->25 0x0000604b2cb4a9f5 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:874:34
#<!-- -->26 0x0000604b2cb3df5c main /home/alexrp/Sources/llvm/llvm/tools/llc/llc.cpp:462:35
#<!-- -->27 0x00007c2046e2a575 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->28 0x00007c2046e2a628 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x00007c2046e2a628 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->30 0x0000604b2cb3ece5 _start (/opt/llvm-22/bin/llc+0x15ce5)
fish: Job 1, 'llc -O0 reduced.ll' terminated by signal SIGABRT (Abort)
```
</details>


---

### Comment 2 - nikic

Regressed in https://github.com/llvm/llvm-project/pull/175515, cc @BeMg @topperc. Previously it produced `__fixtfti`. Not sure whether that was correct or not -- I think compiler-rt provides that on riscv32, but possibly libgcc doesn't.

The problematic zext in the IR expansion is here: https://github.com/llvm/llvm-project/blob/777c7b85ff218b043f67d5bedebb2338f9c1c149/llvm/lib/CodeGen/ExpandIRInsts.cpp#L569 This assumes that the integer type being cast to is larger than the FP type.

---

### Comment 3 - topperc

> Regressed in [#175515](https://github.com/llvm/llvm-project/pull/175515), cc [@BeMg](https://github.com/BeMg) [@topperc](https://github.com/topperc). Previously it produced `__fixtfti`. Not sure whether that was correct or not -- I think compiler-rt provides that on riscv32, but possibly libgcc doesn't.
> 
> The problematic zext in the IR expansion is here:
> 
> [llvm-project/llvm/lib/CodeGen/ExpandIRInsts.cpp](https://github.com/llvm/llvm-project/blob/777c7b85ff218b043f67d5bedebb2338f9c1c149/llvm/lib/CodeGen/ExpandIRInsts.cpp#L569)
> 
> Line 569 in [777c7b8](/llvm/llvm-project/commit/777c7b85ff218b043f67d5bedebb2338f9c1c149)
> 
>  Value *ARep = Builder.CreateZExt(ARep0, FPToI->getType()); 
> This assumes that the integer type being cast to is larger than the FP type.

libgcc does not provide `__fixtfti` on riscv32.

Should we revert #175515 on LLVM 22?

---

### Comment 4 - alexrp

> Should we revert [#175515](https://github.com/llvm/llvm-project/pull/175515) on LLVM 22?

This would work for Zig at least, since we have `__fixtfti` in our compiler-rt regardless of target, and we do not need to care about libgcc.

What would be the plan for LLVM 23+?

---

### Comment 5 - nikic

I think the proper fix for this is something along these lines: https://github.com/nikic/llvm-project/commit/c7b31801d8eec8a4ef762de112d15aee504db626

---

### Comment 6 - nikic

Unfortunately, it looks like the pass has more issue than this (like not distinguishing fptosi and fptoui...) so I agree that we should revert for now. I'll look into fixing the expansions.

---

### Comment 7 - topperc

/cherry-pick f37bf0ce65b14fd5fb969743c47ab2958a7dad06

---

### Comment 8 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: f37bf0ce65b14fd5fb969743c47ab2958a7dad06

https://github.com/llvm/llvm-project/actions/runs/21483735356

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 9 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: f37bf0ce65b14fd5fb969743c47ab2958a7dad06)

https://github.com/llvm/llvm-project/actions/runs/21483730730

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 10 - nikic

I've put up https://github.com/llvm/llvm-project/pull/178690 to fix the expansion for smaller sizes.


---

