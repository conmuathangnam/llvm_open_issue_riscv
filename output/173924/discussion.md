# x86 DAG isel crash

**Author:** Rexicon226
**URL:** https://github.com/llvm/llvm-project/issues/173924
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2025-12-30T12:28:26Z

## Body

https://llc.godbolt.org/z/MzMnqMjax

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux5.10.0-gnu2.39.0"

define i256 @bar(<8 x i256> %0) #0 {
Entry:
  %1 = and <8 x i256> %0, splat (i256 1)
  %2 = call i256 @llvm.vector.reduce.add.v8i256(<8 x i256> %1)
  ret i256 %2
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i256 @llvm.vector.reduce.add.v8i256(<8 x i256>) #1

attributes #0 = { "target-cpu"="znver5" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc-21 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@bar'
 #0 0x00007e15df4fc91f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
 #1 0x00007e15df4fa1c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
 #2 0x00007e15df4fd6fa (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fd6fa)
 #3 0x00007e15da845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007e15dfbbc26c llvm::EVT::isExtendedFixedLengthVector() const (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4bbc26c)
 #5 0x00007e15dfe3cd91 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e3cd91)
 #6 0x00007e15e2ae83ab (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7ae83ab)
 #7 0x00007e15e2a51a35 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7a51a35)
 #8 0x00007e15e2a4d202 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7a4d202)
 #9 0x00007e15dfc1f9fb (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4c1f9fb)
#10 0x00007e15dfc1e0c9 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4c1e0c9)
#11 0x00007e15dfe6d4cd llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6d4cd)
#12 0x00007e15dfe6cee8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6cee8)
#13 0x00007e15dfe6b06f llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6b06f)
#14 0x00007e15dfe693e1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e693e1)
#15 0x00007e15df942239 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4942239)
#16 0x00007e15df66f01f llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x466f01f)
#17 0x00007e15df674e4d llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4674e4d)
#18 0x00007e15df66f77d llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x466f77d)
#19 0x0000650d48dfce35 (/usr/lib/llvm-21/bin/llc+0x1be35)
#20 0x0000650d48dfa7cf main (/usr/lib/llvm-21/bin/llc+0x197cf)
#21 0x00007e15da82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#22 0x00007e15da82a28b call_init ./csu/../csu/libc-start.c:128:20
#23 0x00007e15da82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#24 0x0000650d48df6f05 _start (/usr/lib/llvm-21/bin/llc+0x15f05)
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: David Rubin (Rexicon226)

<details>
https://llc.godbolt.org/z/MzMnqMjax

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux5.10.0-gnu2.39.0"

define i256 @<!-- -->bar(&lt;8 x i256&gt; %0) #<!-- -->0 {
Entry:
  %1 = and &lt;8 x i256&gt; %0, splat (i256 1)
  %2 = call i256 @<!-- -->llvm.vector.reduce.add.v8i256(&lt;8 x i256&gt; %1)
  ret i256 %2
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i256 @<!-- -->llvm.vector.reduce.add.v8i256(&lt;8 x i256&gt;) #<!-- -->1

attributes #<!-- -->0 = { "target-cpu"="znver5" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc-21 reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->bar'
 #<!-- -->0 0x00007e15df4fc91f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
 #<!-- -->1 0x00007e15df4fa1c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
 #<!-- -->2 0x00007e15df4fd6fa (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fd6fa)
 #<!-- -->3 0x00007e15da845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007e15dfbbc26c llvm::EVT::isExtendedFixedLengthVector() const (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4bbc26c)
 #<!-- -->5 0x00007e15dfe3cd91 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e3cd91)
 #<!-- -->6 0x00007e15e2ae83ab (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7ae83ab)
 #<!-- -->7 0x00007e15e2a51a35 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7a51a35)
 #<!-- -->8 0x00007e15e2a4d202 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x7a4d202)
 #<!-- -->9 0x00007e15dfc1f9fb (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4c1f9fb)
#<!-- -->10 0x00007e15dfc1e0c9 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4c1e0c9)
#<!-- -->11 0x00007e15dfe6d4cd llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6d4cd)
#<!-- -->12 0x00007e15dfe6cee8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6cee8)
#<!-- -->13 0x00007e15dfe6b06f llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e6b06f)
#<!-- -->14 0x00007e15dfe693e1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4e693e1)
#<!-- -->15 0x00007e15df942239 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4942239)
#<!-- -->16 0x00007e15df66f01f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x466f01f)
#<!-- -->17 0x00007e15df674e4d llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4674e4d)
#<!-- -->18 0x00007e15df66f77d llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x466f77d)
#<!-- -->19 0x0000650d48dfce35 (/usr/lib/llvm-21/bin/llc+0x1be35)
#<!-- -->20 0x0000650d48dfa7cf main (/usr/lib/llvm-21/bin/llc+0x197cf)
#<!-- -->21 0x00007e15da82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->22 0x00007e15da82a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->23 0x00007e15da82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->24 0x0000650d48df6f05 _start (/usr/lib/llvm-21/bin/llc+0x15f05)
Segmentation fault (core dumped)
```
</details>


---

