# [LoopVectorize] opt crashed at -O2/O3: Assertion "VPlan cost model and legacy cost model disagreed" failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/168709
**Status:** Closed
**Labels:** llvm:SCEV, crash
**Closed Date:** 2025-12-02T14:09:55Z

## Body

Reproducer: https://godbolt.org/z/E3jq9MPsc
Testcase:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define <8 x i64> @backsmith_pure_0(ptr %0) {
entry:
  %BS_VAR_2 = alloca [6 x i16], align 2
  %BS_VAR_3 = alloca [7 x i16], align 2
  %BS_ARG_01 = load <64 x i8>, ptr %0, align 64
  store i8 0, ptr %BS_VAR_2, align 2
  %conv = extractelement <64 x i8> %BS_ARG_01, i64 44
  %cmp = icmp ult i8 %conv, 60
  %narrow = select i1 %cmp, i8 %conv, i8 0
  %cond = zext i8 %narrow to i32
  br label %for.cond

for.cond:                                         ; preds = %for.body, %entry
  %BS_INC_0.0 = phi i32 [ 0, %entry ], [ %add, %for.body ]
  %cmp7 = icmp ult i32 %BS_INC_0.0, %cond
  br i1 %cmp7, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  ret <8 x i64> zeroinitializer

for.body:                                         ; preds = %for.cond
  %div2 = lshr i32 %BS_INC_0.0, 2
  %idxprom = zext i32 %div2 to i64
  %arrayidx = getelementptr i16, ptr %BS_VAR_2, i64 %idxprom
  %1 = load i16, ptr %arrayidx, align 2
  %mul = shl i32 %BS_INC_0.0, 1
  %idxprom12 = zext i32 %mul to i64
  %arrayidx13 = getelementptr nusw i16, ptr %BS_VAR_3, i64 %idxprom12
  %2 = load i16, ptr %arrayidx13, align 2
  %sub = or i16 %2, %1
  store i16 %sub, ptr %arrayidx13, align 2
  %add = add i32 %BS_INC_0.0, 1
  br label %for.cond
}

define <8 x i64> @backsmith_pure_6(i32 %vecext, ptr %byval-temp11) #0 {
entry:
  %vecinit9 = insertelement <16 x i32> zeroinitializer, i32 %vecext, i64 11
  store <16 x i32> %vecinit9, ptr %byval-temp11, align 64
  %call13 = call <8 x i64> @backsmith_pure_0(ptr %byval-temp11)
  ret <8 x i64> %call13
}

attributes #0 = { "target-features"="+avx2" "tune-cpu"="alderlake" }
```

opt output:
```
opt: /root/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7181: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || planContainsAdditionalSimplifications(getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -O2 <source>
1.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "backsmith_pure_6"
 #0 0x0000000005970698 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5970698)
 #1 0x000000000596d544 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000772001e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000772001e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000772001e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000772001e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000772001e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000772001e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000036b434a (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b434a)
 #9 0x00000000036b664e llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b664e)
#10 0x00000000036b9470 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b9470)
#11 0x00000000036b9b9b llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b9b9b)
#12 0x0000000002fa0f6e llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fa0f6e)
#13 0x000000000574fbd1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574fbd1)
#14 0x0000000000efd90e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefd90e)
#15 0x000000000574e29a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574e29a)
#16 0x0000000000973bfe llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x973bfe)
#17 0x000000000574dc51 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574dc51)
#18 0x000000000097de2a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97de2a)
#19 0x0000000000971ea8 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971ea8)
#20 0x0000772001e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x0000772001e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000009690b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9690b5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

