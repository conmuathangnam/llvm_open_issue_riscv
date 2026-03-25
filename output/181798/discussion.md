# [SLP][REVEC] Assertion `N + M <= this->size() && "Invalid specifier"' failed with -slp-revec -slp-vectorize-non-power-of-2

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/181798
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-02-17T18:00:28Z

## Body

https://rust.godbolt.org/z/qxf3r78Y4

```
opt: /root/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:378: llvm::MutableArrayRef<T> llvm::MutableArrayRef<T>::slice(size_t, size_t) const [with T = llvm::Value*; size_t = long unsigned int]: Assertion `N + M <= this->size() && "Invalid specifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -slp-revec -slp-vectorize-non-power-of-2 -O3 <source>
1.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "av1_finalize_encoded_frame"
 #0 0x0000000005b8a888 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b8a888)
 #1 0x0000000005b87764 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007833c0042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007833c00969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007833c0042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007833c00287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007833c002871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007833c0039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000038a0ce8 llvm::slpvectorizer::BoUpSLP::tryToGatherExtractElements(llvm::SmallVectorImpl<llvm::Value*>&, llvm::SmallVectorImpl<int>&, unsigned int) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38a0ce8)
 #9 0x00000000038cd7da llvm::slpvectorizer::BoUpSLP::findReusedOrderedScalars(llvm::slpvectorizer::BoUpSLP::TreeEntry const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38cd7da)
#10 0x000000000390a0fb llvm::slpvectorizer::BoUpSLP::getReorderingData(llvm::slpvectorizer::BoUpSLP::TreeEntry const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x390a0fb)
#11 0x000000000390ef52 llvm::slpvectorizer::BoUpSLP::reorderTopToBottom() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x390ef52)
#12 0x000000000391d03c (anonymous namespace)::HorizontalReduction::tryToReduce(llvm::slpvectorizer::BoUpSLP&, llvm::DataLayout const&, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo const&, llvm::AssumptionCache*, llvm::DominatorTree&) SLPVectorizer.cpp:0:0
#13 0x0000000003921808 llvm::SLPVectorizerPass::vectorizeHorReduction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&, llvm::SmallVectorImpl<llvm::WeakTrackingVH>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3921808)
#14 0x0000000003926753 bool llvm::SLPVectorizerPass::vectorizeCmpInsts<std::reverse_iterator<llvm::CmpInst* const*>>(llvm::iterator_range<std::reverse_iterator<llvm::CmpInst* const*>>, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3926753)
#15 0x0000000003929fc0 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3929fc0)
#16 0x0000000003930c16 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#17 0x0000000003931a9b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3931a9b)
#18 0x00000000030ca01e llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30ca01e)
#19 0x00000000058f5961 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58f5961)
#20 0x0000000000f1420e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1420e)
#21 0x00000000058f3f1a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58f3f1a)
#22 0x000000000096bdfe llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bdfe)
#23 0x00000000058f38d1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58f38d1)
#24 0x0000000000975fba llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x975fba)
#25 0x000000000096a068 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96a068)
#26 0x00007833c0029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#27 0x00007833c0029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#28 0x0000000000960ce5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x960ce5)
```

