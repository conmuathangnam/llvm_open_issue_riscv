# [AArch64][SVE] Assertion fail "Request for a fixed element count on a scalable object"

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/160127
**Status:** Closed
**Labels:** backend:AArch64, SVE, crash, llvm:SelectionDAG
**Closed Date:** 2026-01-09T15:25:02Z

## Body

https://godbolt.org/z/v7o1KqY8q
Assertion fail "Request for a fixed element count on a scalable object"

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"


define <vscale x 1 x i32> @test(<vscale x 1 x i64> %0) {
entry:
  %1 = trunc <vscale x 1 x i64> %0 to <vscale x 1 x i32>
  ret <vscale x 1 x i32> %1
}
```
`llc -mtriple=aarch64-none-elf -mattr=+sve2`
```
llc: /root/llvm-project/llvm/include/llvm/Support/TypeSize.h:201: constexpr llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::ScalarTy llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::getFixedValue() const [with LeafTy = llvm::ElementCount; ValueTy = unsigned int; llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::ScalarTy = unsigned int]: Assertion `(!isScalable() || isZero()) && "Request for a fixed element count on a scalable object"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@test'
 #0 0x00000000040f7978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40f7978)
 #1 0x00000000040f4824 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007910f5042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007910f50969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007910f5042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007910f50287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007910f502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007910f5039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003f860c6 llvm::DAGTypeLegalizer::WidenVecRes_Convert(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f860c6)
 #9 0x0000000003f863e8 llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f863e8)
#10 0x0000000003f2a28b llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2a28b)
#11 0x0000000003f2b269 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2b269)
#12 0x0000000003e901ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e901ab)
#13 0x0000000003e93f0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e93f0d)
#14 0x0000000003e954f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e954f5)
#15 0x0000000003e80cef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e80cef)
#16 0x0000000003007d29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x0000000003643d88 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643d88)
#18 0x0000000003643fc1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643fc1)
#19 0x000000000364482f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x364482f)
#20 0x00000000008f030d compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007c16de main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c16de)
#22 0x00007910f5029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x00007910f5029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008e6765 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6765)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
https://godbolt.org/z/v7o1KqY8q
Assertion fail "Request for a fixed element count on a scalable object"

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"


define &lt;vscale x 1 x i32&gt; @<!-- -->test(&lt;vscale x 1 x i64&gt; %0) {
entry:
  %1 = trunc &lt;vscale x 1 x i64&gt; %0 to &lt;vscale x 1 x i32&gt;
  ret &lt;vscale x 1 x i32&gt; %1
}
```
`opt -mtriple=aarch64-none-elf -mattr=+sve2 -O2`
```
llc: /root/llvm-project/llvm/include/llvm/Support/TypeSize.h:201: constexpr llvm::details::FixedOrScalableQuantity&lt;LeafTy, ValueTy&gt;::ScalarTy llvm::details::FixedOrScalableQuantity&lt;LeafTy, ValueTy&gt;::getFixedValue() const [with LeafTy = llvm::ElementCount; ValueTy = unsigned int; llvm::details::FixedOrScalableQuantity&lt;LeafTy, ValueTy&gt;::ScalarTy = unsigned int]: Assertion `(!isScalable() || isZero()) &amp;&amp; "Request for a fixed element count on a scalable object"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->test'
 #<!-- -->0 0x00000000040f7978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40f7978)
 #<!-- -->1 0x00000000040f4824 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007910f5042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007910f50969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007910f5042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007910f50287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007910f502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007910f5039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003f860c6 llvm::DAGTypeLegalizer::WidenVecRes_Convert(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f860c6)
 #<!-- -->9 0x0000000003f863e8 llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f863e8)
#<!-- -->10 0x0000000003f2a28b llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2a28b)
#<!-- -->11 0x0000000003f2b269 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2b269)
#<!-- -->12 0x0000000003e901ab llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e901ab)
#<!-- -->13 0x0000000003e93f0d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e93f0d)
#<!-- -->14 0x0000000003e954f5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e954f5)
#<!-- -->15 0x0000000003e80cef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e80cef)
#<!-- -->16 0x0000000003007d29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x0000000003643d88 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643d88)
#<!-- -->18 0x0000000003643fc1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3643fc1)
#<!-- -->19 0x000000000364482f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x364482f)
#<!-- -->20 0x00000000008f030d compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c16de main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c16de)
#<!-- -->22 0x00007910f5029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x00007910f5029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008e6765 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8e6765)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - davemgreen

This now produces a uzp1

---

