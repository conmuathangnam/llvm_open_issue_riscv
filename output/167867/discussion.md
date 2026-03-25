# Assertion `ScaleFactor == vputils::getVFScaleFactor(Accumulator->getDefiningRecipe()) && "all accumulators in chain must have same scale factor"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/167867
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-11-20T15:54:59Z

## Body

Most likely this issue duplicates https://github.com/llvm/llvm-project/issues/167243

To reproduce run opt with -S -passes=loop-vectorize and the test below:
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32-ni:1-p2:32:8:8:32-ni:2"
target triple = "aarch64-none-linux-gnu"

define i64 @wombat(ptr addrspace(1) %arg) #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb1, %bb
  %phi = phi i64 [ 0, %bb ], [ %add3, %bb1 ]
  %phi2 = phi i32 [ 729, %bb ], [ %add4, %bb1 ]
  %load = load i32, ptr addrspace(1) null, align 4
  %sext = sext i32 %load to i64
  %mul = mul i64 %sext, 0
  %add = add i64 %sext, %phi
  %add3 = add i64 %add, %mul
  store i64 %add3, ptr addrspace(1) %arg, align 8
  %add4 = add i32 %phi2, 1
  %icmp = icmp ult i32 %phi2, 1
  br i1 %icmp, label %bb5, label %bb1

bb5:                                              ; preds = %bb1
  ret i64 0
}

attributes #0 = { "target-cpu"="neoverse-n1" }
```
Reproducer: https://godbolt.org/z/d5jGn6P6f

Stack dump:
```
opt: /root/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:8234: llvm::VPRecipeBase* llvm::VPRecipeBuilder::tryToCreatePartialReduction(llvm::VPInstruction*, unsigned int): Assertion `ScaleFactor == vputils::getVFScaleFactor(Accumulator->getDefiningRecipe()) && "all accumulators in chain must have same scale factor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "wombat"
 #0 0x00000000059c2018 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59c2018)
 #1 0x00000000059beec4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072eee7e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072eee7e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072eee7e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072eee7e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072eee7e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072eee7e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000036c4de0 llvm::VPRecipeBuilder::tryToCreatePartialReduction(llvm::VPInstruction*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36c4de0)
 #9 0x00000000036c779f llvm::VPRecipeBuilder::tryToCreateWidenRecipe(llvm::VPSingleDefRecipe*, llvm::VFRange&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36c779f)
#10 0x00000000036e69b7 llvm::LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(std::unique_ptr<llvm::VPlan, std::default_delete<llvm::VPlan>>, llvm::VFRange&, llvm::LoopVersioning*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36e69b7)
#11 0x00000000036e7fb8 llvm::LoopVectorizationPlanner::buildVPlansWithVPRecipes(llvm::ElementCount, llvm::ElementCount) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36e7fb8)
#12 0x00000000036e8935 llvm::LoopVectorizationPlanner::plan(llvm::ElementCount, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36e8935)
#13 0x00000000036ec5b6 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36ec5b6)
#14 0x00000000036ef4d0 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36ef4d0)
#15 0x00000000036efbb5 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36efbb5)
#16 0x0000000002fd72be llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fd72be)
#17 0x0000000005799e01 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5799e01)
#18 0x0000000000f035fe llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf035fe)
#19 0x000000000579847a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x579847a)
#20 0x000000000097defe llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97defe)
#21 0x0000000005797e31 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5797e31)
#22 0x000000000098809a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98809a)
#23 0x000000000097c311 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97c311)
#24 0x000072eee7e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000072eee7e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000973725 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x973725)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


