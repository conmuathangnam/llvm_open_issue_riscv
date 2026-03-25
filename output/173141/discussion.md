# [RISCV] Assertion `(TLI.getTypeAction(*DAG.getContext(), Op.getValueType()) == TargetLowering::TypeLegal || Op.getOpcode() == ISD::TargetConstant || Op.getOpcode() == ISD::Register) && "Unexpected illegal type!"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/173141
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-12-21T08:12:35Z

## Body

Reproducer: https://godbolt.org/z/KTv13v8Wx
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define <4 x i16> @backsmith_pure_2(i1 %0, <2 x i16> %conv33) #0 {
entry:
  %1 = tail call i32 @llvm.riscv.orc.b.i32(i32 1)
  %sub.i = add i32 %1, 1
  %2 = and i32 %sub.i, 1
  %and.i = zext i32 %2 to i64
  %3 = select i1 %0, i64 0, i64 %and.i
  %tobool.not128 = icmp sgt i64 %3, -1
  %4 = insertelement <4 x i1> zeroinitializer, i1 %tobool.not128, i64 0
  %5 = shufflevector <4 x i1> %4, <4 x i1> zeroinitializer, <4 x i32> zeroinitializer
  %6 = shufflevector <2 x i16> %conv33, <2 x i16> zeroinitializer, <4 x i32> <i32 1, i32 1, i32 1, i32 1>
  %7 = select <4 x i1> %5, <4 x i16> %6, <4 x i16> zeroinitializer
  ret <4 x i16> %7
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.riscv.orc.b.i32(i32) #1

attributes #0 = { "target-features"="+v" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Dump:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:985: void {anonymous}::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*): Assertion `(TLI.getTypeAction(*DAG.getContext(), Op.getValueType()) == TargetLowering::TypeLegal || Op.getOpcode() == ISD::TargetConstant || Op.getOpcode() == ISD::Register) && "Unexpected illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@backsmith_pure_2'
 #0 0x00000000041effa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41effa8)
 #1 0x00000000041ece54 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000784174842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007841748969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000784174842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007841748287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000078417482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000784174839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e66b57 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
 #9 0x0000000003e68f5e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e68f5e)
#10 0x0000000003f877e2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f877e2)
#11 0x0000000003f8aa18 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8aa18)
#12 0x0000000003f8c54c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8c54c)
#13 0x0000000003f77f3f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f77f3f)
#14 0x00000000030deb49 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x000000000372b5e6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372b5e6)
#16 0x000000000372b991 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372b991)
#17 0x000000000372c1ff llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372c1ff)
#18 0x00000000008f89fd compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#19 0x00000000007c20fc main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c20fc)
#20 0x0000784174829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x0000784174829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000008ed6d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8ed6d5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/KTv13v8Wx
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define &lt;4 x i16&gt; @<!-- -->backsmith_pure_2(i1 %0, &lt;2 x i16&gt; %conv33) #<!-- -->0 {
entry:
  %1 = tail call i32 @<!-- -->llvm.riscv.orc.b.i32(i32 1)
  %sub.i = add i32 %1, 1
  %2 = and i32 %sub.i, 1
  %and.i = zext i32 %2 to i64
  %3 = select i1 %0, i64 0, i64 %and.i
  %tobool.not128 = icmp sgt i64 %3, -1
  %4 = insertelement &lt;4 x i1&gt; zeroinitializer, i1 %tobool.not128, i64 0
  %5 = shufflevector &lt;4 x i1&gt; %4, &lt;4 x i1&gt; zeroinitializer, &lt;4 x i32&gt; zeroinitializer
  %6 = shufflevector &lt;2 x i16&gt; %conv33, &lt;2 x i16&gt; zeroinitializer, &lt;4 x i32&gt; &lt;i32 1, i32 1, i32 1, i32 1&gt;
  %7 = select &lt;4 x i1&gt; %5, &lt;4 x i16&gt; %6, &lt;4 x i16&gt; zeroinitializer
  ret &lt;4 x i16&gt; %7
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.riscv.orc.b.i32(i32) #<!-- -->1

attributes #<!-- -->0 = { "target-features"="+v" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Dump:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:985: void {anonymous}::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*): Assertion `(TLI.getTypeAction(*DAG.getContext(), Op.getValueType()) == TargetLowering::TypeLegal || Op.getOpcode() == ISD::TargetConstant || Op.getOpcode() == ISD::Register) &amp;&amp; "Unexpected illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->backsmith_pure_2'
 #<!-- -->0 0x00000000041effa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41effa8)
 #<!-- -->1 0x00000000041ece54 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000784174842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007841748969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000784174842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007841748287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000078417482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000784174839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e66b57 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
 #<!-- -->9 0x0000000003e68f5e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e68f5e)
#<!-- -->10 0x0000000003f877e2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f877e2)
#<!-- -->11 0x0000000003f8aa18 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8aa18)
#<!-- -->12 0x0000000003f8c54c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f8c54c)
#<!-- -->13 0x0000000003f77f3f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f77f3f)
#<!-- -->14 0x00000000030deb49 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x000000000372b5e6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372b5e6)
#<!-- -->16 0x000000000372b991 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372b991)
#<!-- -->17 0x000000000372c1ff llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x372c1ff)
#<!-- -->18 0x00000000008f89fd compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->19 0x00000000007c20fc main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c20fc)
#<!-- -->20 0x0000784174829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x0000784174829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x00000000008ed6d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8ed6d5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - XChy

Bisected to 6584e47101b0631b44518742efbd4f13020c3dff.

---

