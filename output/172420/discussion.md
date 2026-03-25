# [AArch64][SVE] Assertion `(VA.getValVT().isScalableVector() || Subtarget->isWindowsArm64EC()) && "Indirect arguments should be scalable on most subtargets"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172420
**Status:** Closed
**Labels:** backend:AArch64, SVE, crash
**Closed Date:** 2025-12-24T11:01:37Z

## Body

Reproducer:
https://godbolt.org/z/r9f949PE6

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:8746: virtual llvm::SDValue llvm::AArch64TargetLowering::LowerFormalArguments(llvm::SDValue, llvm::CallingConv::ID, bool, const llvm::SmallVectorImpl<llvm::ISD::InputArg>&, const llvm::SDLoc&, llvm::SelectionDAG&, llvm::SmallVectorImpl<llvm::SDValue>&) const: Assertion `(VA.getValVT().isScalableVector() || Subtarget->isWindowsArm64EC()) && "Indirect arguments should be scalable on most subtargets"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@foo'
 #0 0x000000000421fb68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x421fb68)
 #1 0x000000000421ca14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007684f8e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007684f8e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007684f8e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007684f8e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007684f8e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007684f8e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000d77e03 llvm::AArch64TargetLowering::LowerFormalArguments(llvm::SDValue, unsigned int, bool, llvm::SmallVectorImpl<llvm::ISD::InputArg> const&, llvm::SDLoc const&, llvm::SelectionDAG&, llvm::SmallVectorImpl<llvm::SDValue>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xd77e03)
 #9 0x0000000003eea794 llvm::SelectionDAGISel::LowerArguments(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eea794)
#10 0x0000000003fb8ef5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb8ef5)
#11 0x0000000003fbb03c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fbb03c)
#12 0x0000000003fa683f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa683f)
#13 0x00000000030f5fd9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x0000000003750ab6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750ab6)
#15 0x0000000003750e61 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750e61)
#16 0x00000000037516cf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37516cf)
#17 0x00000000008fe384 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#18 0x00000000007c88f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c88f9)
#19 0x00007684f8e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007684f8e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x00000000008f3585 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3585)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
#include <arm_sve.h>

svcount_t foo(svcount_t p0, svcount_t p1, svcount_t p2, svcount_t p3, int x0,
              int x1, int x2, int x3, int x4, int x5, int x6, int x7,
              svcount_t p4) {
  return p0;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/r9f949PE6

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:8746: virtual llvm::SDValue llvm::AArch64TargetLowering::LowerFormalArguments(llvm::SDValue, llvm::CallingConv::ID, bool, const llvm::SmallVectorImpl&lt;llvm::ISD::InputArg&gt;&amp;, const llvm::SDLoc&amp;, llvm::SelectionDAG&amp;, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;) const: Assertion `(VA.getValVT().isScalableVector() || Subtarget-&gt;isWindowsArm64EC()) &amp;&amp; "Indirect arguments should be scalable on most subtargets"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->foo'
 #<!-- -->0 0x000000000421fb68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x421fb68)
 #<!-- -->1 0x000000000421ca14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007684f8e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007684f8e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007684f8e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007684f8e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007684f8e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007684f8e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000000d77e03 llvm::AArch64TargetLowering::LowerFormalArguments(llvm::SDValue, unsigned int, bool, llvm::SmallVectorImpl&lt;llvm::ISD::InputArg&gt; const&amp;, llvm::SDLoc const&amp;, llvm::SelectionDAG&amp;, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xd77e03)
 #<!-- -->9 0x0000000003eea794 llvm::SelectionDAGISel::LowerArguments(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eea794)
#<!-- -->10 0x0000000003fb8ef5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb8ef5)
#<!-- -->11 0x0000000003fbb03c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fbb03c)
#<!-- -->12 0x0000000003fa683f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa683f)
#<!-- -->13 0x00000000030f5fd9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->14 0x0000000003750ab6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750ab6)
#<!-- -->15 0x0000000003750e61 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750e61)
#<!-- -->16 0x00000000037516cf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37516cf)
#<!-- -->17 0x00000000008fe384 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->18 0x00000000007c88f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c88f9)
#<!-- -->19 0x00007684f8e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->20 0x00007684f8e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->21 0x00000000008f3585 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3585)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
#include &lt;arm_sve.h&gt;

svcount_t foo(svcount_t p0, svcount_t p1, svcount_t p2, svcount_t p3, int x0,
              int x1, int x2, int x3, int x4, int x5, int x6, int x7,
              svcount_t p4) {
  return p0;
}
```
</details>


---

