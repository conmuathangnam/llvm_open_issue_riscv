# [AArch64] llc crash at -O1/O2/O3: Assertion `VT.is128BitVector() && "Unexpected vector MULL size"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/168912
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2025-11-28T12:06:18Z

## Body

Reproducer: https://godbolt.org/z/4Td5GMoes
Testcase:
```llvm
target triple = "aarch64-unknown-linux-musl"

; Function Attrs: vscale_range(1,16)
define <2 x i8> @backsmith_pure_1() #0 {
entry:
  %0 = load <2 x i8>, ptr null, align 2
  %mul = mul <2 x i8> %0, <i8 2, i8 5>
  ret <2 x i8> %mul
}

attributes #0 = { vscale_range(1,16) "target-features"="+sve" }
```

llc dumps:
```
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:5545: llvm::SDValue skipExtensionForVectorMULL(llvm::SDValue, llvm::SelectionDAG&): Assertion `VT.is128BitVector() && "Unexpected vector MULL size"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@backsmith_pure_1'
 #0 0x00000000041a28a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41a28a8)
 #1 0x000000000419f754 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000783b95642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000783b956969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000783b95642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000783b956287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000783b9562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000783b95639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000d2abfc skipExtensionForVectorMULL(llvm::SDValue, llvm::SelectionDAG&) AArch64ISelLowering.cpp:0:0
 #9 0x0000000000d96f78 llvm::AArch64TargetLowering::LowerMUL(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xd96f78)
#10 0x0000000000de0971 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xde0971)
#11 0x0000000003e14b9e (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#12 0x0000000003e186ea llvm::SelectionDAG::LegalizeOp(llvm::SDNode*, llvm::SmallSetVector<llvm::SDNode*, 16u>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e186ea)
#13 0x0000000003dca706 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#14 0x0000000003dcce93 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3dcce93)
#15 0x0000000003f37ad5 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f37ad5)
#16 0x0000000003f3ad88 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3ad88)
#17 0x0000000003f3c805 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3c805)
#18 0x0000000003f27f7f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f27f7f)
#19 0x000000000308fab9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#20 0x00000000036d9f66 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36d9f66)
#21 0x00000000036da311 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36da311)
#22 0x00000000036dab7f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36dab7f)
#23 0x00000000008f8a9f compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#24 0x00000000007c6079 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c6079)
#25 0x0000783b95629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#26 0x0000783b95629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#27 0x00000000008edd45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8edd45)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/4Td5GMoes
Testcase:
```llvm
target triple = "aarch64-unknown-linux-musl"

; Function Attrs: vscale_range(1,16)
define &lt;2 x i8&gt; @<!-- -->backsmith_pure_1() #<!-- -->0 {
entry:
  %0 = load &lt;2 x i8&gt;, ptr null, align 2
  %mul = mul &lt;2 x i8&gt; %0, &lt;i8 2, i8 5&gt;
  ret &lt;2 x i8&gt; %mul
}

attributes #<!-- -->0 = { vscale_range(1,16) "target-features"="+sve" }
```

llc dumps:
```
llc: /root/llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:5545: llvm::SDValue skipExtensionForVectorMULL(llvm::SDValue, llvm::SelectionDAG&amp;): Assertion `VT.is128BitVector() &amp;&amp; "Unexpected vector MULL size"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->backsmith_pure_1'
 #<!-- -->0 0x00000000041a28a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41a28a8)
 #<!-- -->1 0x000000000419f754 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000783b95642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000783b956969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000783b95642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000783b956287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000783b9562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000783b95639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000000d2abfc skipExtensionForVectorMULL(llvm::SDValue, llvm::SelectionDAG&amp;) AArch64ISelLowering.cpp:0:0
 #<!-- -->9 0x0000000000d96f78 llvm::AArch64TargetLowering::LowerMUL(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xd96f78)
#<!-- -->10 0x0000000000de0971 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xde0971)
#<!-- -->11 0x0000000003e14b9e (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#<!-- -->12 0x0000000003e186ea llvm::SelectionDAG::LegalizeOp(llvm::SDNode*, llvm::SmallSetVector&lt;llvm::SDNode*, 16u&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e186ea)
#<!-- -->13 0x0000000003dca706 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#<!-- -->14 0x0000000003dcce93 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3dcce93)
#<!-- -->15 0x0000000003f37ad5 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f37ad5)
#<!-- -->16 0x0000000003f3ad88 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3ad88)
#<!-- -->17 0x0000000003f3c805 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f3c805)
#<!-- -->18 0x0000000003f27f7f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f27f7f)
#<!-- -->19 0x000000000308fab9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->20 0x00000000036d9f66 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36d9f66)
#<!-- -->21 0x00000000036da311 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36da311)
#<!-- -->22 0x00000000036dab7f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36dab7f)
#<!-- -->23 0x00000000008f8a9f compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->24 0x00000000007c6079 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c6079)
#<!-- -->25 0x0000783b95629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->26 0x0000783b95629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->27 0x00000000008edd45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8edd45)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - cofibrant

I can pick this up

---

### Comment 3 - cofibrant

Apologies, I've only just gotten around to this. I did some digging and it looks like `LowerMUL()` falls into the following code path (introduced by @davemgreen):

https://github.com/llvm/llvm-project/blob/58fa7e4ccd533e5547868a9211d4da16bc80ac20/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp#L5796-L5803

But, in this specific reproducer, `N1` ends up with a scalable vector type, which the subsequent code isn't expecting.

I'm definitely not qualified to resolve this so I'll bow out.

---

