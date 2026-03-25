# [AArch64][SVE] LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/187557
**Status:** Open
**Labels:** backend:AArch64, SVE, crash, llvm:SelectionDAG

## Body

View on Compiler Explorer: https://godbolt.org/z/xEM9roj3c
View on Compiler Explorer: https://godbolt.org/z/qhn4exvKP

### Input LLVM IR

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define <vscale x 14 x i16> @test_func_12(<vscale x 14 x i64> %0) {
entry:
  %1 = trunc <vscale x 14 x i64> %0 to <vscale x 14 x i8>
  %2 = sext <vscale x 14 x i8> %1 to <vscale x 14 x i16>
  ret <vscale x 14 x i16> %2
}
```
Another reproducer:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define <vscale x 12 x i64> @test_func_12(<vscale x 12 x i64> %0) {
entry:
  %1 = shl <vscale x 12 x i64> %0, splat (i64 56)
  %2 = ashr <vscale x 12 x i64> %1, splat (i64 56)
  ret <vscale x 12 x i64> %2
}
```


### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector. Scalable flag may be dropped, use EVT::getVectorElementCount() instead
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@test_func_12'
 #0 0x00000000044d1368 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44d1368)
 #1 0x00000000044ce194 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44ce194)
 #2 0x00000000044ce30a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000761fb8242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000761fb82969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000761fb8242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000761fb82287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000007b7833 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #8 0x000000000440ff70 llvm::reportFatalInternalError(llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x440ff70)
 #9 0x0000000004351b14 llvm::DAGTypeLegalizer::WidenVecRes_InregOp(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4351b14)
#10 0x0000000004356d1a llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4356d1a)
#11 0x00000000042f88e3 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42f88e3)
#12 0x00000000042f99c9 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42f99c9)
#13 0x000000000425a15b llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425a15b)
#14 0x000000000425dca5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425dca5)
#15 0x000000000425f61c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425f61c)
#16 0x0000000004249f6f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4249f6f)
#17 0x000000000322c6d9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x000000000388f2d6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388f2d6)
#19 0x000000000388f681 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388f681)
#20 0x000000000388feef llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388feef)
#21 0x00000000008e7072 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#22 0x00000000007bd9bc main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bd9bc)
#23 0x0000761fb8229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x0000761fb8229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000008dc125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8dc125)
Program terminated with signal: SIGSEGV
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
View on Compiler Explorer: https://godbolt.org/z/xEM9roj3c
View on Compiler Explorer: https://godbolt.org/z/qhn4exvKP

### Input LLVM IR

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define &lt;vscale x 14 x i16&gt; @<!-- -->test_func_12(&lt;vscale x 14 x i64&gt; %0) {
entry:
  %1 = trunc &lt;vscale x 14 x i64&gt; %0 to &lt;vscale x 14 x i8&gt;
  %2 = sext &lt;vscale x 14 x i8&gt; %1 to &lt;vscale x 14 x i16&gt;
  ret &lt;vscale x 14 x i16&gt; %2
}
```
Another reproducer:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define &lt;vscale x 12 x i64&gt; @<!-- -->test_func_12(&lt;vscale x 12 x i64&gt; %0) {
entry:
  %1 = shl &lt;vscale x 12 x i64&gt; %0, splat (i64 56)
  %2 = ashr &lt;vscale x 12 x i64&gt; %1, splat (i64 56)
  ret &lt;vscale x 12 x i64&gt; %2
}
```


### Command

```bash
llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2
```

### Output

```asm
Compilation failed:

[stderr]
LLVM ERROR: Possible incorrect use of EVT::getVectorNumElements() for scalable vector. Scalable flag may be dropped, use EVT::getVectorElementCount() instead
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=aarch64-none-elf -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->test_func_12'
 #<!-- -->0 0x00000000044d1368 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44d1368)
 #<!-- -->1 0x00000000044ce194 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44ce194)
 #<!-- -->2 0x00000000044ce30a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000761fb8242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000761fb82969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000761fb8242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000761fb82287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000007b7833 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->8 0x000000000440ff70 llvm::reportFatalInternalError(llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x440ff70)
 #<!-- -->9 0x0000000004351b14 llvm::DAGTypeLegalizer::WidenVecRes_InregOp(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4351b14)
#<!-- -->10 0x0000000004356d1a llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4356d1a)
#<!-- -->11 0x00000000042f88e3 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42f88e3)
#<!-- -->12 0x00000000042f99c9 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42f99c9)
#<!-- -->13 0x000000000425a15b llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425a15b)
#<!-- -->14 0x000000000425dca5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425dca5)
#<!-- -->15 0x000000000425f61c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x425f61c)
#<!-- -->16 0x0000000004249f6f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4249f6f)
#<!-- -->17 0x000000000322c6d9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x000000000388f2d6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388f2d6)
#<!-- -->19 0x000000000388f681 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388f681)
#<!-- -->20 0x000000000388feef llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x388feef)
#<!-- -->21 0x00000000008e7072 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->22 0x00000000007bd9bc main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bd9bc)
#<!-- -->23 0x0000761fb8229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x0000761fb8229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x00000000008dc125 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8dc125)
Program terminated with signal: SIGSEGV
```
</details>


---

