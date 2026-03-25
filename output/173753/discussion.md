# [RISC-V] Clang crashes when running pass 'loop-unroll<O3>' on function '@_Z1kv'

**Author:** ams-x9v2b7kq
**URL:** https://github.com/llvm/llvm-project/issues/173753
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-06T13:51:34Z

## Body

## [RISC-V] Clang crashes when running pass 'loop-unroll<O3>' on function '@_Z1kv'

### code link: https://godbolt.org/z/6sx9TPafa
```c++
char a, e;
bool f;
extern int g[];
int h;
extern int i[][4];
extern int j[][1];
void k() {
  for (int b = 4; b < 9; b++)
    for (char c = 0; c < 20; c += 3)
      for (int d = 0; d < 20; d += 2) {
        e ^= i[d][3] ? h : g[d + b];
        j[c][d] = c && (a || f);
      }
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/2025-compiler/gcc-16-20251221/bin/clang++ --gcc-toolchain=/home/2025-compiler/gcc-16-20251221 -O3 -march=rv64gcv_zvfh -Werror=uninitialized -Werror=implicitly-unsigned-literal -Wno-pass-failed -c func.cpp -o func.o
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "func.cpp"
4.	Running pass "loop-unroll<O3>" on function "_Z1kv"
 #0 0x0000557906a1a852 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de9852)
 #1 0x0000557906a182fc llvm::sys::CleanupOnSignal(unsigned long) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de72fc)
 #2 0x0000557906958988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe212349330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00005579069315ea llvm::APInt::compare(llvm::APInt const&) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d005ea)
 #5 0x000055790635eac3 llvm::ConstantRange::contains(llvm::ConstantRange const&) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x172dac3)
 #6 0x0000557905c3c039 StrengthenNoWrapFlags(llvm::ScalarEvolution*, llvm::SCEVTypes, llvm::ArrayRef<llvm::SCEV const*>, llvm::SCEV::NoWrapFlags) ScalarEvolution.cpp:0:0
 #7 0x0000557905c4e697 llvm::ScalarEvolution::getMulExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x101d697)
 #8 0x0000557905c502c0 llvm::ScalarEvolution::getMulExpr(llvm::SCEV const*, llvm::SCEV const*, llvm::SCEV::NoWrapFlags, unsigned int) (.constprop.0) ScalarEvolution.cpp:0:0
 #9 0x0000557905c4f953 llvm::ScalarEvolution::getMulExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x101e953)
#10 0x0000557905c50832 llvm::ScalarEvolution::getNegativeSCEV(llvm::SCEV const*, llvm::SCEV::NoWrapFlags) (.part.0) ScalarEvolution.cpp:0:0
#11 0x0000557905c509cb llvm::ScalarEvolution::getMinusSCEV(llvm::SCEV const*, llvm::SCEV const*, llvm::SCEV::NoWrapFlags, unsigned int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x101f9cb)
#12 0x0000557905c5e84d llvm::ScalarEvolution::createNodeForSelectOrPHIInstWithICmpInstCond(llvm::Type*, llvm::ICmpInst*, llvm::Value*, llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x102d84d)
#13 0x0000557905c5edd9 llvm::ScalarEvolution::createNodeForSelectOrPHI(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x102ddd9)
#14 0x0000557905c376e1 llvm::ScalarEvolution::createSCEVIter(llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x10066e1)
#15 0x000055790aa0fd4f llvm::UnrolledInstAnalyzer::simplifyInstWithSCEV(llvm::Instruction*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5dded4f)
#16 0x000055790686667e analyzeLoopUnrollCost(llvm::Loop const*, unsigned int, llvm::DominatorTree&, llvm::ScalarEvolution&, llvm::SmallPtrSetImpl<llvm::Value const*> const&, llvm::TargetTransformInfo const&, unsigned int, unsigned int) (.isra.0) LoopUnrollPass.cpp:0:0
#17 0x0000557906868012 shouldFullUnroll(llvm::Loop*, llvm::TargetTransformInfo const&, llvm::DominatorTree&, llvm::ScalarEvolution&, llvm::SmallPtrSetImpl<llvm::Value const*> const&, unsigned int, llvm::UnrollCostEstimator, llvm::TargetTransformInfo::UnrollingPreferences const&) (.isra.0) LoopUnrollPass.cpp:0:0
#18 0x00005579068683c1 llvm::computeUnrollCount(llvm::Loop*, llvm::TargetTransformInfo const&, llvm::DominatorTree&, llvm::LoopInfo*, llvm::AssumptionCache*, llvm::ScalarEvolution&, llvm::SmallPtrSetImpl<llvm::Value const*> const&, llvm::OptimizationRemarkEmitter*, unsigned int, unsigned int, bool, unsigned int, llvm::UnrollCostEstimator const&, llvm::TargetTransformInfo::UnrollingPreferences&, llvm::TargetTransformInfo::PeelingPreferences&, bool&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1c373c1)
#19 0x0000557906869929 tryToUnrollLoop(llvm::Loop*, llvm::DominatorTree&, llvm::LoopInfo*, llvm::ScalarEvolution&, llvm::TargetTransformInfo const&, llvm::AssumptionCache&, llvm::OptimizationRemarkEmitter&, llvm::BlockFrequencyInfo*, llvm::ProfileSummaryInfo*, bool, int, bool, bool, bool, std::optional<unsigned int>, std::optional<unsigned int>, std::optional<bool>, std::optional<bool>, std::optional<bool>, std::optional<bool>, std::optional<bool>, std::optional<unsigned int>, llvm::AAResults*) LoopUnrollPass.cpp:0:0
#20 0x000055790686b12a llvm::LoopUnrollPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1c3a12a)
#21 0x0000557907957e85 llvm::detail::PassModel<llvm::Function, llvm::LoopUnrollPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2d26e85)
#22 0x00005579064bb844 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x188a844)
#23 0x0000557906c8ce55 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x205be55)
#24 0x00005579064bc0be llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x188b0be)
#25 0x0000557906c8cbd5 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x205bbd5)
#26 0x00005579064b8c24 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1887c24)
#27 0x0000557906c9ad87 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#28 0x0000557906c9e303 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x206d303)
#29 0x00005579072800a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x264f0a9)
#30 0x0000557908cc608c clang::ParseAST(clang::Sema&, bool, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x409508c)
#31 0x00005579075bbd8e clang::FrontendAction::Execute() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x298ad8e)
#32 0x000055790753d6d3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x290c6d3)
#33 0x000055790769f5af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2a6e5af)
#34 0x00005579058a1a7d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc70a7d)
#35 0x000055790589925a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x00005579058992b5 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x00005579072fc5bd void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000557906958db7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27db7)
#39 0x00005579072fccb1 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x00005579072bfed6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268eed6)
#41 0x00005579072c0f39 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268ff39)
#42 0x00005579072d284c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x26a184c)
#43 0x000055790589e111 clang_main(int, char**, llvm::ToolContext const&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc6d111)
#44 0x00005579057a01da main (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xb6f1da)
#45 0x00007fe21232e1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#46 0x00007fe21232e28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#47 0x0000557905898c85 _start (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc67c85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251221/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/func-38f011.cpp
clang++: note: diagnostic msg: /tmp/func-38f011.sh
clang++: note: diagnostic msg:

********************
```

## Comments

### Comment 1 - topperc

On an assertions build, this fails in the vectorizer https://godbolt.org/z/Y1Kov35cP

Same assertion as #173755 and #173761

---

### Comment 2 - fhahn

IIUC this should be fixed after landing https://github.com/llvm/llvm-project/pull/174416.

Please re-open with an updated reproducer if it still is an issue.

When fuzzing/reducing would probably be good to build with assertions/run verifier

---

