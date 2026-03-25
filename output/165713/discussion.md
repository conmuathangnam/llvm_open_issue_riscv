# [WebAssembly] llc (assertion trunk) triggered assertion "Unexpected out of bounds negative value" at -O1/O2/O3 with feature simd128

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/165713
**Status:** Closed
**Labels:** backend:WebAssembly, crash
**Closed Date:** 2025-11-17T10:39:19Z

## Body

Reproducer: https://godbolt.org/z/bjrr5e9jG
Testcase (reduced from a valid C program):
```llvm
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-i128:128-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-wasi"

define <2 x i8> @func_8() #0 {
entry:
  %shuffle3.i = shufflevector <32 x i32> zeroinitializer, <32 x i32> <i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 -62880201, i32 poison, i32 poison, i32 poison>, <2 x i32> <i32 17, i32 60>
  %conv.i = trunc <2 x i32> %shuffle3.i to <2 x i8>
  ret <2 x i8> %conv.i
}

attributes #0 = { "target-features"="+simd128" }
```

llc (assertion trunk) output:
```
llc: /root/llvm-project/llvm/lib/Target/WebAssembly/WebAssemblyISelLowering.cpp:2628: llvm::SDValue llvm::WebAssemblyTargetLowering::LowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&) const: Assertion `(LaneBits == 64 || Val >= -(1ll << (LaneBits - 1))) && "Unexpected out of bounds negative value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'WebAssembly Instruction Selection' on function '@func_8'
 #0 0x000000000419a438 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x419a438)
 #1 0x00000000041972e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007437eb042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007437eb0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007437eb042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007437eb0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007437eb02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007437eb039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000218e407 llvm::WebAssemblyTargetLowering::LowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x218e407)
 #9 0x000000000219628b llvm::WebAssemblyTargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x219628b)
#10 0x0000000003e060c3 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#11 0x0000000003e0976e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e0976e)
#12 0x0000000003f28e82 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f28e82)
#13 0x0000000003f2c158 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2c158)
#14 0x0000000003f2dbd5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2dbd5)
#15 0x0000000003f195df llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f195df)
#16 0x000000000307f7e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x00000000036cda16 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cda16)
#18 0x00000000036cddc1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cddc1)
#19 0x00000000036ce62f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36ce62f)
#20 0x00000000008f95d3 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007c6b56 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c6b56)
#22 0x00007437eb029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x00007437eb029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008eea15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8eea15)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

llc output:
```
LLVM ERROR: Cannot select: 0x39a25540: v16i8 = BUILD_VECTOR Constant:i32<0>, Constant:i32<-62880201>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>, Constant:i32<0>
In function: func_8
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'WebAssembly Instruction Selection' on function '@func_8'
 #0 0x0000000003c2f2b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c2f2b8)
 #1 0x0000000003c2c484 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007eff50242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007eff502969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007eff50242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007eff502287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007beb88 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000039c14ba llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39c14ba)
 #8 0x00000000039c56f9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39c56f9)
 #9 0x0000000001eb3c1f (anonymous namespace)::WebAssemblyDAGToDAGISel::Select(llvm::SDNode*) WebAssemblyISelDAGToDAG.cpp:0:0
#10 0x00000000039be624 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39be624)
#11 0x00000000039ca9f2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39ca9f2)
#12 0x00000000039cd321 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39cd321)
#13 0x00000000039cf2ce llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39cf2ce)
#14 0x00000000039be1c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39be1c1)
#15 0x0000000002c891fd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c891fd)
#16 0x00000000032007d2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32007d2)
#17 0x0000000003200a61 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3200a61)
#18 0x0000000003202343 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3202343)
#19 0x00000000007d65aa main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7d65aa)
#20 0x00007eff50229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007eff50229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000009040b5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x9040b5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/bjrr5e9jG
Testcase (reduced from a valid C program):
```llvm
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-i128:128-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-wasi"

define &lt;2 x i8&gt; @<!-- -->func_8() #<!-- -->0 {
entry:
  %shuffle3.i = shufflevector &lt;32 x i32&gt; zeroinitializer, &lt;32 x i32&gt; &lt;i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 -62880201, i32 poison, i32 poison, i32 poison&gt;, &lt;2 x i32&gt; &lt;i32 17, i32 60&gt;
  %conv.i = trunc &lt;2 x i32&gt; %shuffle3.i to &lt;2 x i8&gt;
  ret &lt;2 x i8&gt; %conv.i
}

attributes #<!-- -->0 = { "target-features"="+simd128" }
```

llc (assertion trunk) output:
```
llc: /root/llvm-project/llvm/lib/Target/WebAssembly/WebAssemblyISelLowering.cpp:2628: llvm::SDValue llvm::WebAssemblyTargetLowering::LowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&amp;) const: Assertion `(LaneBits == 64 || Val &gt;= -(1ll &lt;&lt; (LaneBits - 1))) &amp;&amp; "Unexpected out of bounds negative value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'WebAssembly Instruction Selection' on function '@<!-- -->func_8'
 #<!-- -->0 0x000000000419a438 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x419a438)
 #<!-- -->1 0x00000000041972e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007437eb042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007437eb0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007437eb042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007437eb0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007437eb02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007437eb039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000218e407 llvm::WebAssemblyTargetLowering::LowerBUILD_VECTOR(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x218e407)
 #<!-- -->9 0x000000000219628b llvm::WebAssemblyTargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x219628b)
#<!-- -->10 0x0000000003e060c3 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
#<!-- -->11 0x0000000003e0976e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e0976e)
#<!-- -->12 0x0000000003f28e82 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f28e82)
#<!-- -->13 0x0000000003f2c158 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2c158)
#<!-- -->14 0x0000000003f2dbd5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f2dbd5)
#<!-- -->15 0x0000000003f195df llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f195df)
#<!-- -->16 0x000000000307f7e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x00000000036cda16 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cda16)
#<!-- -->18 0x00000000036cddc1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cddc1)
#<!-- -->19 0x00000000036ce62f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36ce62f)
#<!-- -->20 0x00000000008f95d3 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c6b56 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c6b56)
#<!-- -->22 0x00007437eb029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x00007437eb029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008eea15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8eea15)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

llc output:
```
LLVM ERROR: Cannot select: 0x39a25540: v16i8 = BUILD_VECTOR Constant:i32&lt;0&gt;, Constant:i32&lt;-62880201&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;
In function: func_8
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O1 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'WebAssembly Instruction Selection' on function '@<!-- -->func_8'
 #<!-- -->0 0x0000000003c2f2b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c2f2b8)
 #<!-- -->1 0x0000000003c2c484 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007eff50242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007eff502969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007eff50242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007eff502287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007beb88 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x00000000039c14ba llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39c14ba)
 #<!-- -->8 0x00000000039c56f9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39c56f9)
 #<!-- -->9 0x0000000001eb3c1f (anonymous namespace)::WebAssemblyDAGToDAGISel::Select(llvm::SDNode*) WebAssemblyISelDAGToDAG.cpp:0:0
#<!-- -->10 0x00000000039be624 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39be624)
#<!-- -->11 0x00000000039ca9f2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39ca9f2)
#<!-- -->12 0x00000000039cd321 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39cd321)
#<!-- -->13 0x00000000039cf2ce llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39cf2ce)
#<!-- -->14 0x00000000039be1c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39be1c1)
#<!-- -->15 0x0000000002c891fd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c891fd)
#<!-- -->16 0x00000000032007d2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x32007d2)
#<!-- -->17 0x0000000003200a61 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3200a61)
#<!-- -->18 0x0000000003202343 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3202343)
#<!-- -->19 0x00000000007d65aa main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7d65aa)
#<!-- -->20 0x00007eff50229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x00007eff50229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x00000000009040b5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x9040b5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

