# [AArch64][SVE] LLVM ERROR: Don't know how to widen the result of EXTRACT_SUBVECTOR for scalable vectors

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/160532
**Status:** Open
**Labels:** backend:AArch64, SVE, crash, llvm:SelectionDAG

## Body

https://godbolt.org/z/jsPd5j8bq
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define { <vscale x 1 x i64>, <vscale x 1 x i64> } @test(<vscale x 2 x i64> %t148) {
entry:
  %0 = tail call { <vscale x 1 x i64>, <vscale x 1 x i64> } @llvm.vector.deinterleave2.nxv2i64(<vscale x 2 x i64> %t148)
  ret { <vscale x 1 x i64>, <vscale x 1 x i64> } %0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare { <vscale x 1 x i64>, <vscale x 1 x i64> } @llvm.vector.deinterleave2.nxv2i64(<vscale x 2 x i64>) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(none) }

```
`llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2`

```
LLVM ERROR: Don't know how to widen the result of EXTRACT_SUBVECTOR for scalable vectors
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -mtriple=aarch64-none-elf -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@test'
 #0 0x0000000003b9aaa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b9aaa8)
 #1 0x0000000003b97c74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000075d48b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000075d48b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000075d48b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000075d48b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007b7136 llvm::UniqueStringSaver::save(llvm::Twine const&) (.cold) StringSaver.cpp:0:0
 #7 0x0000000003aee228 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3aee228)
 #8 0x0000000003a286c2 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a286c2)
 #9 0x0000000003a3cf27 llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a3cf27)
#10 0x00000000039e44d2 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39e44d2)
#11 0x00000000039e4b81 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39e4b81)
#12 0x0000000003949b32 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3949b32)
#13 0x000000000394c64a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394c64a)
#14 0x000000000394e59e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394e59e)
#15 0x000000000393d491 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x393d491)
#16 0x0000000002c1bb9d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c1bb9d)
#17 0x000000000318a8a2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318a8a2)
#18 0x000000000318ab31 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318ab31)
#19 0x000000000318c407 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318c407)
#20 0x00000000008fece9 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007cbfee main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cbfee)
#22 0x000075d48b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000075d48b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008f6535 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f6535)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
https://godbolt.org/z/jsPd5j8bq
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64--linux-gnueabihf"

define { &lt;vscale x 1 x i64&gt;, &lt;vscale x 1 x i64&gt; } @<!-- -->test(&lt;vscale x 2 x i64&gt; %t148) {
entry:
  %0 = tail call { &lt;vscale x 1 x i64&gt;, &lt;vscale x 1 x i64&gt; } @<!-- -->llvm.vector.deinterleave2.nxv2i64(&lt;vscale x 2 x i64&gt; %t148)
  ret { &lt;vscale x 1 x i64&gt;, &lt;vscale x 1 x i64&gt; } %0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare { &lt;vscale x 1 x i64&gt;, &lt;vscale x 1 x i64&gt; } @<!-- -->llvm.vector.deinterleave2.nxv2i64(&lt;vscale x 2 x i64&gt;) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind willreturn memory(none) }

```
`llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2`

```
LLVM ERROR: Don't know how to widen the result of EXTRACT_SUBVECTOR for scalable vectors
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -mtriple=aarch64-none-elf -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->test'
 #<!-- -->0 0x0000000003b9aaa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b9aaa8)
 #<!-- -->1 0x0000000003b97c74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000075d48b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000075d48b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000075d48b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000075d48b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007b7136 llvm::UniqueStringSaver::save(llvm::Twine const&amp;) (.cold) StringSaver.cpp:0:0
 #<!-- -->7 0x0000000003aee228 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3aee228)
 #<!-- -->8 0x0000000003a286c2 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a286c2)
 #<!-- -->9 0x0000000003a3cf27 llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a3cf27)
#<!-- -->10 0x00000000039e44d2 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39e44d2)
#<!-- -->11 0x00000000039e4b81 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39e4b81)
#<!-- -->12 0x0000000003949b32 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3949b32)
#<!-- -->13 0x000000000394c64a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394c64a)
#<!-- -->14 0x000000000394e59e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394e59e)
#<!-- -->15 0x000000000393d491 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x393d491)
#<!-- -->16 0x0000000002c1bb9d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c1bb9d)
#<!-- -->17 0x000000000318a8a2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318a8a2)
#<!-- -->18 0x000000000318ab31 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318ab31)
#<!-- -->19 0x000000000318c407 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318c407)
#<!-- -->20 0x00000000008fece9 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007cbfee main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cbfee)
#<!-- -->22 0x000075d48b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x000075d48b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008f6535 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f6535)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

