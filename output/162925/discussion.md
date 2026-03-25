# [RISC-V][SLPVectorizer] Assertion `all_of(Bundles, [](const ScheduleBundle *Bundle) { return Bundle->isScheduled(); }) && "must be scheduled at this point"' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/162925
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-12T17:28:27Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i32 @main(ptr %q, ptr %a) #0 {
entry:
  %.pre = load i8, ptr %q, align 1
  br label %for.body.i

for.body.i:                                       ; preds = %for.body.i, %entry
  %conv34.i = phi i64 [ 0, %entry ], [ %conv.i, %for.body.i ]
  %v.033.i = phi i32 [ 0, %entry ], [ %add.i, %for.body.i ]
  %conv11.i = sext i8 %.pre to i32
  %sub.sink.i = add i32 %v.033.i, %conv11.i
  %shl18.i = shl i32 %conv11.i, %sub.sink.i
  %conv19.i = trunc i32 %shl18.i to i16
  %arrayidx21.i = getelementptr i16, ptr %a, i64 %conv34.i
  store i16 %conv19.i, ptr %arrayidx21.i, align 2
  %add.i = add i32 %v.033.i, 1
  %conv.i = zext i32 %add.i to i64
  %cmp.i = icmp ult i32 %v.033.i, 12
  br i1 %cmp.i, label %for.body.i, label %c.exit

c.exit:                                           ; preds = %for.body.i
  ret i32 0
}

attributes #0 = { "target-features"="+v" }

```

Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt "-passes=lto<O3>" reduced.ll
WARNING: You're attempting to print out a bitcode file.
This is inadvisable as it may cause display problems. If
you REALLY want to taste LLVM bitcode first-hand, you
can force output with the `-f' option.

opt: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:21677: void llvm::slpvectorizer::BoUpSLP::scheduleBlock(const llvm::slpvectorizer::BoUpSLP&, BlockScheduling*): Assertion `all_of(Bundles, [](const ScheduleBundle *Bundle) { return Bundle->isScheduled(); }) && "must be scheduled at this point"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt -passes=lto<O3> reduced.ll
1.      Running pass "function<eager-inv>(loop-mssa(licm<allowspeculation>),gvn<>,memcpyopt,dse,move-auto-init,mldst-motion<no-split-footer-bb>,loop(indvars,loop-deletion,loop-unroll-full),loop-distribute,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,sccp,instcombine<max-iterations=1;no-verify-fixpoint>,bdce,slp-vectorizer,vector-combine,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,jump-threading)" on module "reduced.ll"
2.      Running pass "slp-vectorizer" on function "main"
 #0 0x00005c95dc04ae60 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x35fae60)
 #1 0x00005c95dc047b7a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f5611645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x00007f561169eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #4 0x00007f561169eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #5 0x00007f561169eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #6 0x00007f561164527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007f56116288ff abort ./stdlib/abort.c:81:7
 #8 0x00007f561162881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #9 0x00007f561163b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#10 0x00005c95d9c04287 llvm::slpvectorizer::BoUpSLP::scheduleBlock(llvm::slpvectorizer::BoUpSLP const&, llvm::slpvectorizer::BoUpSLP::BlockScheduling*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11b4287)
#11 0x00005c95d9c174f3 llvm::slpvectorizer::BoUpSLP::vectorizeTree(llvm::SmallDenseSet<llvm::Value*, 4u, llvm::DenseMapInfo<llvm::Value*, void>> const&, llvm::Instruction*, llvm::ArrayRef<std::tuple<llvm::Value*, unsigned int, bool>>) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11c74f3)
#12 0x00005c95d9c1bc72 llvm::slpvectorizer::BoUpSLP::vectorizeTree() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11cbc72)
#13 0x00005c95d9c3a404 llvm::SLPVectorizerPass::vectorizeStoreChain(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, unsigned int, unsigned int, unsigned int&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11ea404)
#14 0x00005c95d9c3d303 llvm::SLPVectorizerPass::vectorizeStores(llvm::ArrayRef<llvm::StoreInst*>, llvm::slpvectorizer::BoUpSLP&, llvm::DenseSet<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, llvm::DenseMapInfo<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, void>>&)::'lambda'(std::map<long, unsigned int, std::less<long>, std::allocator<std::pair<long const, unsigned int>>> const&)::operator()(std::map<long, unsigned int, std::less<long>, std::allocator<std::pair<long const, unsigned int>>> const&) const SLPVectorizer.cpp:0:0
#15 0x00005c95d9c3df00 llvm::SLPVectorizerPass::vectorizeStores(llvm::ArrayRef<llvm::StoreInst*>, llvm::slpvectorizer::BoUpSLP&, llvm::DenseSet<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, llvm::DenseMapInfo<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, void>>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11edf00)
#16 0x00005c95d9c3f4df llvm::SLPVectorizerPass::vectorizeStoreChains(llvm::slpvectorizer::BoUpSLP&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11ef4df)
#17 0x00005c95d9c41343 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11f1343)
#18 0x00005c95d9c42021 llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11f2021)
#19 0x00005c95d931e563 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8ce563)
#20 0x00005c95dbe189e3 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c89e3)
#21 0x00005c95d93203b3 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8d03b3)
#22 0x00005c95dbe174a7 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c74a7)
#23 0x00005c95d8f958d3 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x5458d3)
#24 0x00005c95dbe16da3 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c6da3)
#25 0x00005c95d8fa0af6 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x550af6)
#26 0x00005c95d8f93a7a optMain (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x543a7a)
#27 0x00007f561162a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007f561162a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007f561162a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x00005c95d8f89e15 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x539e15)
Aborted
```

Godbolt: https://godbolt.org/z/76oWGdEzh

Found via fuzzer (C program before reduction)

