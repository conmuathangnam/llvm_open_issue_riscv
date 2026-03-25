# opt -passes=slp-vectorizer crashes: Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/181520
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-02-15T19:02:41Z

## Body

Reproducer:
https://godbolt.org/z/P9WWjGYhY

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -S <source>
1.	Running pass "function(slp-vectorizer)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "add"
 #0 0x0000000005b86608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b86608)
 #1 0x0000000005b834e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072a7ab442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072a7ab4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072a7ab442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072a7ab4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072a7ab42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072a7ab439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000b89439 llvm::APInt::APInt(unsigned int, unsigned long, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xb89439)
 #9 0x000000000576ccd5 llvm::ConstantInt::get(llvm::Type*, unsigned long, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x576ccd5)
#10 0x000000000380dd99 (anonymous namespace)::HorizontalReduction::emitScaleForReusedOps(llvm::Value*, llvm::IRBuilderBase&, unsigned int) (.isra.0) SLPVectorizer.cpp:0:0
#11 0x000000000391df1b (anonymous namespace)::HorizontalReduction::tryToReduce(llvm::slpvectorizer::BoUpSLP&, llvm::DataLayout const&, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo const&, llvm::AssumptionCache*, llvm::DominatorTree&) SLPVectorizer.cpp:0:0
#12 0x0000000003920388 llvm::SLPVectorizerPass::vectorizeHorReduction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&, llvm::SmallVectorImpl<llvm::WeakTrackingVH>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3920388)
#13 0x000000000392508b llvm::SLPVectorizerPass::vectorizeRootInstruction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#14 0x0000000003929620 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3929620)
#15 0x000000000392f796 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#16 0x000000000393061b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x393061b)
#17 0x00000000030c928e llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30c928e)
#18 0x00000000058f15d1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58f15d1)
#19 0x0000000000f129ce llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf129ce)
#20 0x00000000058efb8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58efb8a)
#21 0x000000000096b8ee llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96b8ee)
#22 0x00000000058ef541 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58ef541)
#23 0x0000000000975aaa llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x975aaa)
#24 0x0000000000969b58 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x969b58)
#25 0x000072a7ab429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#26 0x000072a7ab429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#27 0x00000000009607d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9607d5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

The reproducer here comes from llvm/test/Transforms/Reassociate/repeats.ll

---

