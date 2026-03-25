# [RISCV] Assertion `Iter != this->end() && "DenseMap::at failed due to a missing key"' failed.

**Author:** BeMg
**URL:** https://github.com/llvm/llvm-project/issues/168665
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-11-20T03:50:01Z

## Body

https://godbolt.org/z/ojcT5x3n3

bisect to 65ad21d730d25789454d18e811f8ff5db79cb5d4

assertion message as below

```
llc: /root/llvm-project/llvm/include/llvm/ADT/DenseMap.h:226: const ValueT& llvm::DenseMapBase<DerivedT, KeyT, ValueT, KeyInfoT, BucketT>::at(llvm::DenseMapBase<DerivedT, KeyT, ValueT, KeyInfoT, BucketT>::const_arg_type_t<KeyT>) const [with DerivedT = llvm::DenseMap<const llvm::MachineInstr*, {anonymous}::DemandedVL>; KeyT = const llvm::MachineInstr*; ValueT = {anonymous}::DemandedVL; KeyInfoT = llvm::DenseMapInfo<const llvm::MachineInstr*, void>; BucketT = llvm::detail::DenseMapPair<const llvm::MachineInstr*, {anonymous}::DemandedVL>; llvm::DenseMapBase<DerivedT, KeyT, ValueT, KeyInfoT, BucketT>::const_arg_type_t<KeyT> = const llvm::MachineInstr*]: Assertion `Iter != this->end() && "DenseMap::at failed due to a missing key"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+v -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V VL Optimizer' on function '@main'
 #0 0x00000000041a29e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41a29e8)
 #1 0x000000000419f894 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000785c70242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000785c702969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000785c70242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000785c702287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000785c7022871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000785c70239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000001d42be3 (anonymous namespace)::RISCVVLOptimizer::runOnMachineFunction(llvm::MachineFunction&) (.part.0) RISCVVLOptimizer.cpp:0:0
 #9 0x0000000003090789 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#10 0x00000000036da9c6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36da9c6)
#11 0x00000000036dad71 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36dad71)
#12 0x00000000036db5df llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36db5df)
#13 0x00000000008f821f compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#14 0x00000000007c5db9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c5db9)
#15 0x0000785c70229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#16 0x0000785c70229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#17 0x00000000008ed4c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8ed4c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Piyou Chen (BeMg)

<details>
https://godbolt.org/z/ojcT5x3n3

bisect to 65ad21d730d25789454d18e811f8ff5db79cb5d4

assertion message as below

```
llc: /root/llvm-project/llvm/include/llvm/ADT/DenseMap.h:226: const ValueT&amp; llvm::DenseMapBase&lt;DerivedT, KeyT, ValueT, KeyInfoT, BucketT&gt;::at(llvm::DenseMapBase&lt;DerivedT, KeyT, ValueT, KeyInfoT, BucketT&gt;::const_arg_type_t&lt;KeyT&gt;) const [with DerivedT = llvm::DenseMap&lt;const llvm::MachineInstr*, {anonymous}::DemandedVL&gt;; KeyT = const llvm::MachineInstr*; ValueT = {anonymous}::DemandedVL; KeyInfoT = llvm::DenseMapInfo&lt;const llvm::MachineInstr*, void&gt;; BucketT = llvm::detail::DenseMapPair&lt;const llvm::MachineInstr*, {anonymous}::DemandedVL&gt;; llvm::DenseMapBase&lt;DerivedT, KeyT, ValueT, KeyInfoT, BucketT&gt;::const_arg_type_t&lt;KeyT&gt; = const llvm::MachineInstr*]: Assertion `Iter != this-&gt;end() &amp;&amp; "DenseMap::at failed due to a missing key"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+v -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V VL Optimizer' on function '@<!-- -->main'
 #<!-- -->0 0x00000000041a29e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41a29e8)
 #<!-- -->1 0x000000000419f894 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000785c70242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000785c702969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000785c70242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000785c702287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000785c7022871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000785c70239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000001d42be3 (anonymous namespace)::RISCVVLOptimizer::runOnMachineFunction(llvm::MachineFunction&amp;) (.part.0) RISCVVLOptimizer.cpp:0:0
 #<!-- -->9 0x0000000003090789 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->10 0x00000000036da9c6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36da9c6)
#<!-- -->11 0x00000000036dad71 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36dad71)
#<!-- -->12 0x00000000036db5df llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36db5df)
#<!-- -->13 0x00000000008f821f compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->14 0x00000000007c5db9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c5db9)
#<!-- -->15 0x0000785c70229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->16 0x0000785c70229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->17 0x00000000008ed4c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8ed4c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - BeMg

cc @lukel97 @topperc 

---

