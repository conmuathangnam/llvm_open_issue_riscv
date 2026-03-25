# [LoongArch] Crash when passing integer value to inline asm with `r|m` constraint

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/159200
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid, backend:loongarch
**Closed Date:** 2025-09-18T02:06:34Z

## Body

```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "loongarch64-unknown-linux5.19.0-musl"

define void @foo() {
Entry:
  call void asm sideeffect "", "=*r|m,r|m"(ptr elementtype(i32) null, i64 0)
  ret void
}
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 21.1.0
❯ llc -O3 reduced.ll
LLVM ERROR: unsupported machine operand type
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc -O3 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'LoongArch Merge Base Offset' on function '@foo'
 #0 0x00007c5f1b191af0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:838:3
 #1 0x00007c5f1b18e9b4 llvm::sys::RunSignalHandlers() /home/alexrp/Source/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #2 0x00007c5f1b18f1e9 SignalHandler(int, siginfo_t*, void*) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #3 0x00007c5f19a458d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)
 #4 0x00007c5f19aa49bc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007c5f19aa49bc __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #6 0x00007c5f19aa49bc pthread_kill ./nptl/pthread_kill.c:100:10
 #7 0x00007c5f19a4579e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007c5f19a288cd internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #9 0x00007c5f19a288cd __abort_lock_wrlock ./stdlib/abort.c:58:3
#10 0x00007c5f19a288cd abort ./stdlib/abort.c:81:3
#11 0x00007c5f1aaa17df std::mutex::lock() /usr/include/c++/14/bits/std_mutex.h:117:22
#12 0x00007c5f1aaa17df std::lock_guard<std::mutex>::lock_guard(std::mutex&) /usr/include/c++/14/bits/std_mutex.h:250:23
#13 0x00007c5f1aaa17df llvm::install_bad_alloc_error_handler(void (*)(void*, char const*, bool), void*) (.cold) /home/alexrp/Source/llvm-project/llvm/lib/Support/ErrorHandling.cpp:162:61
#14 0x00007c5f1b0a5215 (/opt/llvm-21/bin/../lib/libLLVM.so.21.1+0xea5215)
#15 0x00007c5f1ed54d11 llvm::SmallVectorBase<unsigned int>::size() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/SmallVector.h:79:32
#16 0x00007c5f1ed54d11 llvm::IndexedMap<std::pair<llvm::PointerUnion<llvm::TargetRegisterClass const*, llvm::RegisterBank const*>, llvm::MachineOperand*>, llvm::VirtReg2IndexFunctor>::operator[](llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IndexedMap.h:53:7
#17 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::getRegUseDefListHead(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:124:33
#18 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::use_instr_begin(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:481:58
#19 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::use_instructions(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:489:22
#20 0x00007c5f1ed54d11 foldIntoMemoryOps /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:742:56
#21 0x00007c5f1ed54d11 runOnMachineFunction /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:831:38
#22 0x00007c5f1ed54d11 (anonymous namespace)::LoongArchMergeBaseOffsetOpt::runOnMachineFunction(llvm::MachineFunction&) /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:800:6
#23 0x00007c5f1b6d947b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#24 0x00007c5f1b34cd34 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1408:7
#25 0x00007c5f1b34cf45 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getNext() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#26 0x00007c5f1b34cf45 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>>::getNext() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#27 0x00007c5f1b34cf45 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>::operator++() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#28 0x00007c5f1b34cf45 llvm::FPPassManager::runOnModule(llvm::Module&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#29 0x00007c5f1b34d75b runOnModule /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1520:7
#30 0x00007c5f1b34d75b llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#31 0x0000641081a3416d compileModule(char**, llvm::LLVMContext&) /home/alexrp/Source/llvm-project/llvm/tools/llc/llc.cpp:761:34
#32 0x0000641081a284cf main /home/alexrp/Source/llvm-project/llvm/tools/llc/llc.cpp:404:35
#33 0x00007c5f19a2a578 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x00007c5f19a2a63b call_init ./csu/../csu/libc-start.c:128:20
#35 0x00007c5f19a2a63b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x0000641081a29aa5 _start (/opt/llvm-21/bin/llc+0x14aa5)
fish: Job 1, 'llc -O3 reduced.ll' terminated by signal SIGABRT (Abort)
```

Changing the `i64 0` argument to `ptr null` makes the repro no longer crash, as does changing the corresponding `r|m` to `r`.

All other backends I've tried handle this gracefully.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: Alex Rønne Petersen (alexrp)

<details>
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "loongarch64-unknown-linux5.19.0-musl"

define void @<!-- -->foo() {
Entry:
  call void asm sideeffect "", "=*r|m,r|m"(ptr elementtype(i32) null, i64 0)
  ret void
}
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 21.1.0
❯ llc -O3 reduced.ll
LLVM ERROR: unsupported machine operand type
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc -O3 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'LoongArch Merge Base Offset' on function '@<!-- -->foo'
 #<!-- -->0 0x00007c5f1b191af0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:838:3
 #<!-- -->1 0x00007c5f1b18e9b4 llvm::sys::RunSignalHandlers() /home/alexrp/Source/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007c5f1b18f1e9 SignalHandler(int, siginfo_t*, void*) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->3 0x00007c5f19a458d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)
 #<!-- -->4 0x00007c5f19aa49bc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007c5f19aa49bc __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #<!-- -->6 0x00007c5f19aa49bc pthread_kill ./nptl/pthread_kill.c:100:10
 #<!-- -->7 0x00007c5f19a4579e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007c5f19a288cd internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #<!-- -->9 0x00007c5f19a288cd __abort_lock_wrlock ./stdlib/abort.c:58:3
#<!-- -->10 0x00007c5f19a288cd abort ./stdlib/abort.c:81:3
#<!-- -->11 0x00007c5f1aaa17df std::mutex::lock() /usr/include/c++/14/bits/std_mutex.h:117:22
#<!-- -->12 0x00007c5f1aaa17df std::lock_guard&lt;std::mutex&gt;::lock_guard(std::mutex&amp;) /usr/include/c++/14/bits/std_mutex.h:250:23
#<!-- -->13 0x00007c5f1aaa17df llvm::install_bad_alloc_error_handler(void (*)(void*, char const*, bool), void*) (.cold) /home/alexrp/Source/llvm-project/llvm/lib/Support/ErrorHandling.cpp:162:61
#<!-- -->14 0x00007c5f1b0a5215 (/opt/llvm-21/bin/../lib/libLLVM.so.21.1+0xea5215)
#<!-- -->15 0x00007c5f1ed54d11 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/SmallVector.h:79:32
#<!-- -->16 0x00007c5f1ed54d11 llvm::IndexedMap&lt;std::pair&lt;llvm::PointerUnion&lt;llvm::TargetRegisterClass const*, llvm::RegisterBank const*&gt;, llvm::MachineOperand*&gt;, llvm::VirtReg2IndexFunctor&gt;::operator[](llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IndexedMap.h:53:7
#<!-- -->17 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::getRegUseDefListHead(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:124:33
#<!-- -->18 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::use_instr_begin(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:481:58
#<!-- -->19 0x00007c5f1ed54d11 llvm::MachineRegisterInfo::use_instructions(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:489:22
#<!-- -->20 0x00007c5f1ed54d11 foldIntoMemoryOps /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:742:56
#<!-- -->21 0x00007c5f1ed54d11 runOnMachineFunction /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:831:38
#<!-- -->22 0x00007c5f1ed54d11 (anonymous namespace)::LoongArchMergeBaseOffsetOpt::runOnMachineFunction(llvm::MachineFunction&amp;) /home/alexrp/Source/llvm-project/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp:800:6
#<!-- -->23 0x00007c5f1b6d947b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->24 0x00007c5f1b34cd34 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1408:7
#<!-- -->25 0x00007c5f1b34cf45 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getNext() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#<!-- -->26 0x00007c5f1b34cf45 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;&gt;::getNext() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#<!-- -->27 0x00007c5f1b34cf45 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;::operator++() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->28 0x00007c5f1b34cf45 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#<!-- -->29 0x00007c5f1b34d75b runOnModule /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1520:7
#<!-- -->30 0x00007c5f1b34d75b llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->31 0x0000641081a3416d compileModule(char**, llvm::LLVMContext&amp;) /home/alexrp/Source/llvm-project/llvm/tools/llc/llc.cpp:761:34
#<!-- -->32 0x0000641081a284cf main /home/alexrp/Source/llvm-project/llvm/tools/llc/llc.cpp:404:35
#<!-- -->33 0x00007c5f19a2a578 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->34 0x00007c5f19a2a63b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->35 0x00007c5f19a2a63b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->36 0x0000641081a29aa5 _start (/opt/llvm-21/bin/llc+0x14aa5)
fish: Job 1, 'llc -O3 reduced.ll' terminated by signal SIGABRT (Abort)
```

Changing the `i64 0` argument to `ptr null` makes the repro no longer crash, as does changing the corresponding `r|m` to `r`.

All other backends I've tried handle this gracefully.
</details>


---

### Comment 2 - zhaoqi5

This can be avoided after applying this:

```
diff --git a/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp b/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp
index f62753356a..4d7fb85da0 100644
--- a/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp
+++ b/llvm/lib/Target/LoongArch/LoongArchMergeBaseOffset.cpp
@@ -759,6 +759,8 @@ bool LoongArchMergeBaseOffsetOpt::foldIntoMemoryOps(MachineInstr &Hi20,
           MO.ChangeToBA(ImmOp.getBlockAddress(), ImmOp.getOffset(),
                         LoongArchII::getDirectFlags(ImmOp));
           break;
+        case MachineOperand::MO_ConstantPoolIndex:
+          return false;
         default:
           report_fatal_error("unsupported machine operand type");
           break;
```

But I wonder if there are any better solutions. cc @heiher 

---

### Comment 3 - heiher

Thanks for looking into this. [My patch](https://github.com/llvm/llvm-project/pull/159336) improves the handling so that memory instructions in inline asm can be merged with constant pool address loads.

---

