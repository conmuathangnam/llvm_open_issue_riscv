# [RISCV] LLVM ERROR: Cannot scalarize scalable vector stores

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/171141
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-12-10T04:59:35Z

## Body

Reproducer: https://godbolt.org/z/bx99W86Th
Testcase (reduced from a valid C program with `_BitInt`):
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define <vscale x 16 x i4> @backsmith_pure_0() #0 {
entry:
  ret <vscale x 16 x i4> zeroinitializer
}

attributes #0 = { "target-features"="+v" }
```

Dump:
```
LLVM ERROR: Cannot scalarize scalable vector stores
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@backsmith_pure_0'
 #0 0x0000000004213598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4213598)
 #1 0x0000000004210444 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000753a13e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000753a13e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000753a13e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000753a13e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007c10b5 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000041597a1 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41597a1)
 #8 0x0000000003fe08b5 llvm::TargetLowering::scalarizeVectorStore(llvm::StoreSDNode*, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fe08b5)
 #9 0x0000000004056dc3 (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) LegalizeVectorOps.cpp:0:0
#10 0x0000000004059fe9 llvm::SelectionDAG::LegalizeVectors() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4059fe9)
#11 0x0000000003fa8626 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa8626)
#12 0x0000000003fabd88 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fabd88)
#13 0x0000000003fad80c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fad80c)
#14 0x0000000003f9900f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9900f)
#15 0x00000000030f3849 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#16 0x000000000374c486 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374c486)
#17 0x000000000374c831 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374c831)
#18 0x000000000374d09f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374d09f)
#19 0x00000000008fe004 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#20 0x00000000007c7c99 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c7c99)
#21 0x0000753a13e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x0000753a13e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00000000008f3205 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3205)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/bx99W86Th
Testcase (reduced from a valid C program with `_BitInt`):
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define &lt;vscale x 16 x i4&gt; @<!-- -->backsmith_pure_0() #<!-- -->0 {
entry:
  ret &lt;vscale x 16 x i4&gt; zeroinitializer
}

attributes #<!-- -->0 = { "target-features"="+v" }
```

Dump:
```
LLVM ERROR: Cannot scalarize scalable vector stores
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->backsmith_pure_0'
 #<!-- -->0 0x0000000004213598 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4213598)
 #<!-- -->1 0x0000000004210444 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000753a13e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000753a13e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000753a13e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000753a13e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007c10b5 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x00000000041597a1 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41597a1)
 #<!-- -->8 0x0000000003fe08b5 llvm::TargetLowering::scalarizeVectorStore(llvm::StoreSDNode*, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fe08b5)
 #<!-- -->9 0x0000000004056dc3 (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) LegalizeVectorOps.cpp:0:0
#<!-- -->10 0x0000000004059fe9 llvm::SelectionDAG::LegalizeVectors() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4059fe9)
#<!-- -->11 0x0000000003fa8626 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa8626)
#<!-- -->12 0x0000000003fabd88 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fabd88)
#<!-- -->13 0x0000000003fad80c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fad80c)
#<!-- -->14 0x0000000003f9900f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9900f)
#<!-- -->15 0x00000000030f3849 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->16 0x000000000374c486 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374c486)
#<!-- -->17 0x000000000374c831 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374c831)
#<!-- -->18 0x000000000374d09f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x374d09f)
#<!-- -->19 0x00000000008fe004 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->20 0x00000000007c7c99 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c7c99)
#<!-- -->21 0x0000753a13e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->22 0x0000753a13e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->23 0x00000000008f3205 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3205)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - topperc

This doesn't look like a valid reduction of a C test case. I don't know how a C program can create a <vscale x 16 x i4> argument type. Can you share the original C code?

---

### Comment 3 - XChy

> Can you share the original C code?

@topperc, yes, attach it to godbolt now.
A reduced C reproducer: https://godbolt.org/z/h15eKxo8G
Let me know if you need the unreduced C reproducer, thanks!

---

### Comment 4 - topperc

> > Can you share the original C code?
> 
> [@topperc](https://github.com/topperc), yes, attach it to godbolt now. A reduced C reproducer: https://godbolt.org/z/h15eKxo8G Let me know if you need the unreduced C reproducer, thanks!

This is enough. And it confirms that there are 2 differnet issues.

---

### Comment 5 - topperc

> > > Can you share the original C code?
> > 
> > 
> > [@topperc](https://github.com/topperc), yes, attach it to godbolt now. A reduced C reproducer: https://godbolt.org/z/h15eKxo8G Let me know if you need the unreduced C reproducer, thanks!
> 
> This is enough. And it confirms that there are 2 differnet issues.

I was wrong. There is only 1 issue.

---

