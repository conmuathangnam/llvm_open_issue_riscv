# [AArch64] Assertion `ArgVT == MVT::i16' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173483
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2026-02-26T14:54:36Z

## Body

Reproducer:
https://godbolt.org/z/vzs6Gc1or

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:5534: llvm::SDValue llvm::AArch64TargetLowering::LowerBITCAST(llvm::SDValue, llvm::SelectionDAG&) const: Assertion `ArgVT == MVT::i16' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@f'
 #0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000074edba842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000074edba8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000074edba842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000074edba8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000074edba82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000074edba839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000b9a577 llvm::AArch64TargetLowering::LowerBITCAST(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb9a577)
 #9 0x0000000000bf2fd9 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xbf2fd9)
#10 0x0000000003ee4c9f llvm::TargetLowering::LowerOperationWrapper(llvm::SDNode*, llvm::SmallVectorImpl<llvm::SDValue>&, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ee4c9f)
#11 0x000000000406f364 llvm::DAGTypeLegalizer::CustomLowerNode(llvm::SDNode*, llvm::EVT, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x406f364)
#12 0x000000000412d987 llvm::DAGTypeLegalizer::PromoteIntegerOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x412d987)
#13 0x0000000004070204 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4070204)
#14 0x0000000004070d09 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4070d09)
#15 0x0000000003fd490b llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd490b)
#16 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#17 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#18 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#19 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#20 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#21 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#22 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#23 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#24 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#25 0x000074edba829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#26 0x000074edba829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#27 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
void foo();
typedef unsigned char v2qi __attribute__((vector_size(2)));
void f(v2qi *ptr)
{
  v2qi x = *ptr;
  asm volatile ("" :: "w" (x));
  asm volatile ("" ::: "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15");
  foo();
  asm volatile ("" :: "w" (x));
  *ptr = x;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/vzs6Gc1or

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:5534: llvm::SDValue llvm::AArch64TargetLowering::LowerBITCAST(llvm::SDValue, llvm::SelectionDAG&amp;) const: Assertion `ArgVT == MVT::i16' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->f'
 #<!-- -->0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #<!-- -->1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000074edba842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000074edba8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000074edba842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000074edba8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000074edba82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000074edba839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000000b9a577 llvm::AArch64TargetLowering::LowerBITCAST(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb9a577)
 #<!-- -->9 0x0000000000bf2fd9 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xbf2fd9)
#<!-- -->10 0x0000000003ee4c9f llvm::TargetLowering::LowerOperationWrapper(llvm::SDNode*, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ee4c9f)
#<!-- -->11 0x000000000406f364 llvm::DAGTypeLegalizer::CustomLowerNode(llvm::SDNode*, llvm::EVT, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x406f364)
#<!-- -->12 0x000000000412d987 llvm::DAGTypeLegalizer::PromoteIntegerOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x412d987)
#<!-- -->13 0x0000000004070204 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4070204)
#<!-- -->14 0x0000000004070d09 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4070d09)
#<!-- -->15 0x0000000003fd490b llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd490b)
#<!-- -->16 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#<!-- -->17 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#<!-- -->18 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#<!-- -->19 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->20 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#<!-- -->21 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#<!-- -->22 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#<!-- -->23 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->24 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#<!-- -->25 0x000074edba829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->26 0x000074edba829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->27 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
void foo();
typedef unsigned char v2qi __attribute__((vector_size(2)));
void f(v2qi *ptr)
{
  v2qi x = *ptr;
  asm volatile ("" :: "w" (x));
  asm volatile ("" ::: "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15");
  foo();
  asm volatile ("" :: "w" (x));
  *ptr = x;
}
```
</details>


---

