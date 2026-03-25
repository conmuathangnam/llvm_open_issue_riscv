# [LLVM] SLP Vectorizer crashes with APInt setBit assertion on out-of-bounds extractelement

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/176780
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-01-28T11:08:05Z

## Body

**Fuzzer Generated Test** 
**Reproducer**
https://godbolt.org/z/rxrEYrj1v

**Test Commit**
[0b2f3cfb72a76fa90f3ec2a234caabe0d0712590](https://github.com/llvm/llvm-project/commit/0b2f3cfb72a76fa90f3ec2a234caabe0d0712590)

**Description**
The SLP Vectorizer crashes when processing IR containing an out-of-bounds `extractelement` index. While `extractelement` with an out-of-bounds index is valid IR (it produces poison), the SLP Vectorizer's cost calculation logic uses an APInt bitmask to track vector lanes and attempts to set a bit beyond the APInt's width, triggering an assertion failure.

Related:  https://github.com/llvm/llvm-project/issues/58054

**Steps to reproduce**
- Minimized test case, `input.ll`
```
define <4 x i32> @test(<4 x i32> %A) {
entry:
  %e0 = extractelement <4 x i32> %A, i64 0
  %e1 = extractelement <4 x i32> %A, i64 1
  %e2 = extractelement <4 x i32> %A, i64 2
  %e3 = extractelement <4 x i32> %A, i64 3
  %oob = extractelement <4 x i32> %A, i64 4
  %d0 = sdiv i32 %e0, %e0
  %d1 = sdiv i32 %oob, %e1
  %d2 = sdiv i32 %e2, %e3
  %d3 = sdiv i32 %oob, %e1
  %v0 = insertelement <4 x i32> poison, i32 %d0, i64 0
  %v1 = insertelement <4 x i32> %v0, i32 %d1, i64 1
  %v2 = insertelement <4 x i32> %v1, i32 %d2, i64 2
  %v3 = insertelement <4 x i32> %v2, i32 %d3, i64 3
  ret <4 x i32> %v3
}
```

**Command**
```
opt -passes=slp-vectorizer -S input.ll
```

**Output**
```
opt: /workdir/llvm-project/llvm/include/llvm/ADT/APInt.h:1332: void llvm::APInt::setBit(unsigned int): Assertion `BitPosition < BitWidth && "BitPosition out of range"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: opt -passes=slp-vectorizer -S /input.ll
1.	Running pass "function(slp-vectorizer)" on module "input.ll"
2.	Running pass "slp-vectorizer" on function "test"
 #0 0x00007f726e5ac728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMSupport.so.22.0git+0x1d3728)
 #1 0x00007f726e5a99d5 llvm::sys::RunSignalHandlers() (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMSupport.so.22.0git+0x1d09d5)
 #2 0x00007f726e5ad3c6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f72786d5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f726e0c000b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f726e09f859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f726e09f729 get_sysdep_segment_value /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f726e09f729 _nl_load_domain /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f726e0b0fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x00007f7270fce24d llvm::InstructionCost llvm::function_ref<llvm::InstructionCost (unsigned int)>::callback_fn<llvm::slpvectorizer::BoUpSLP::getEntryCost(llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::ArrayRef<llvm::Value*>, llvm::SmallPtrSetImpl<llvm::Value*>&)::$_6>(long, unsigned int) SLPVectorizer.cpp:0:0
#10 0x00007f7270f12f56 llvm::slpvectorizer::BoUpSLP::getEntryCost(llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::ArrayRef<llvm::Value*>, llvm::SmallPtrSetImpl<llvm::Value*>&)::$_2::operator()(llvm::function_ref<llvm::InstructionCost (unsigned int)>, llvm::function_ref<llvm::InstructionCost (llvm::InstructionCost)>) const SLPVectorizer.cpp:0:0
#11 0x00007f7270f0d5fc llvm::slpvectorizer::BoUpSLP::getEntryCost(llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::ArrayRef<llvm::Value*>, llvm::SmallPtrSetImpl<llvm::Value*>&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1665fc)
#12 0x00007f7270f19404 llvm::slpvectorizer::BoUpSLP::calculateTreeCostAndTrimNonProfitable(llvm::ArrayRef<llvm::Value*>) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x172404)
#13 0x00007f7270f540e4 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1ad0e4)
#14 0x00007f7270f5a075 llvm::SLPVectorizerPass::vectorizeInsertElementInst(llvm::InsertElementInst*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&, bool) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1b3075)
#15 0x00007f7270f5a279 llvm::SLPVectorizerPass::vectorizeInserts(llvm::SmallSetVector<llvm::Instruction*, 8u>&, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1b3279)
#16 0x00007f7270f5b359 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&)::$_4::operator()(bool) const SLPVectorizer.cpp:0:0
#17 0x00007f7270f4e4a4 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1a74a4)
#18 0x00007f7270f4ac94 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1a3c94)
#19 0x00007f7270f49fe5 llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMVectorize.so.22.0git+0x1a2fe5)
#20 0x00007f726ea41f6b llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMCore.so.22.0git+0x2e3f6b)
#21 0x00007f726ea46f9f llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMCore.so.22.0git+0x2e8f9f)
#22 0x00007f726ea40d4b llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMCore.so.22.0git+0x2e2d4b)
#23 0x00007f727868a9fd llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::__1::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMOptDriver.so.22.0git+0x269fd)
#24 0x00007f727869bae4 optMain (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMOptDriver.so.22.0git+0x37ae4)
#25 0x00007f726e0a1083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#26 0x000055c43c38e07e _start (/workdir/llvm-project/build-libcxx/bin/opt+0x207e)
```


