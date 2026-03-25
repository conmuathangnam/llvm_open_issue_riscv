# [SLP] Clang crashes at -O2/3/s: Assertion `OpIdx < Operands.size() && "Off bounds"' failed.

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/187904
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-03-22T11:56:06Z

## Body

Compiler Explorer: https://godbolt.org/z/hrWTxeK1q

small.c
```c
int a;
int b() {
  return (a & 1 << 1 ? 0 : 1) | (a & 1 << 7 ? 0 : 1 << 1) |
         (a & 1 << 4 ? 0 : 1 << 2) | (a & 1 << 6 ? 0 : 1 << 3) |
         a - 37 + a - 155 + a & 1 << 4 | a << 5 | a << 6 | a << 7;
}
void main() {}
```

Crash:
```
$ clang-trunk -O2 small.c
clang-23: /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:4207: llvm::slpvectorizer::BoUpSLP::ValueList &llvm::slpvectorizer::BoUpSLP::TreeEntry::getOperand(unsigned int): Assertion `OpIdx < Operands.size() && "Off bounds"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O2 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-21-12-29-38/crash-clang-trunk-18182/perses_result -fcoverage-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-21-12-29-38/crash-clang-trunk-18182/perses_result -resource-dir /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=283 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/small-9cf30a.o -x c small.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto;check-exit-count>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.      Running pass "slp-vectorizer" on function "b"
 #0 0x000055c8d93e3457 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:13
 #1 0x000055c8d93e10fa llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x000055c8d93e42f5 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:38
 #3 0x000055c8d93e4158 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>, std::random_access_iterator_tag) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2067:8
 #4 0x000055c8d93e4158 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2112:14
 #5 0x000055c8d93e4158 int const* std::find<int const*, int>(int const*, int const*, int const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algo.h:3851:14
 #6 0x000055c8d93e4158 bool llvm::is_contained<int const (&) [10], int>(int const (&) [10], int const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:12
 #7 0x000055c8d93e4158 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:7
 #8 0x00007f997edee520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #9 0x00007f997ee429fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
#10 0x00007f997ee429fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
#11 0x00007f997ee429fc pthread_kill ./nptl/pthread_kill.c:89:10
#12 0x00007f997edee476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
#13 0x00007f997edd47f3 abort ./stdlib/abort.c:81:7
#14 0x00007f997edd471b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#15 0x00007f997ede5e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#16 0x000055c8dab78012 (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0x5208012)
#17 0x000055c8daadb85c llvm::slpvectorizer::BoUpSLP::BlockScheduling::buildBundle(llvm::ArrayRef<llvm::Value*>, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:23066:11
#18 0x000055c8daadb85c llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:23370:28
#19 0x000055c8daac9dee std::_Optional_base_impl<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*, std::_Optional_base<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*, true, true>>::_M_is_engaged() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/optional:471:58
#20 0x000055c8daac9dee std::optional<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*>::operator bool() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/optional:985:22
#21 0x000055c8daac9dee llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:12244:8
#22 0x000055c8dab5ae53 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int)::$_61::operator()((anonymous namespace)::InstructionsState const&) const::'lambda'(llvm::ArrayRef<llvm::Value*>, unsigned int)::operator()(llvm::ArrayRef<llvm::Value*>, unsigned int) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:0:9
#23 0x000055c8daad7033 llvm::SmallVectorTemplateCommon<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void>::isSmall() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallVector.h:145:39
#24 0x000055c8daad7033 llvm::SmallVectorImpl<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*>::~SmallVectorImpl() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallVector.h:606:16
#25 0x000055c8daad7033 llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 6u>::~SmallVector() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallVector.h:1212:3
#26 0x000055c8daad7033 llvm::slpvectorizer::BoUpSLP::ScheduleBundle::~ScheduleBundle() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:5111:9
#27 0x000055c8daad7033 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int)::$_61::operator()((anonymous namespace)::InstructionsState const&) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:12127:3
#28 0x000055c8daac951d llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:12167:7
#29 0x000055c8daaca3a2 std::enable_if<std::is_signed_v<long>, long>::type llvm::AddOverflow<long>(long, long, long&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/MathExtras.h:703:10
#30 0x000055c8daaca3a2 llvm::detail::CheckedInt::operator+(long) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/Sequence.h:152:9
#31 0x000055c8daaca3a2 llvm::detail::SafeIntIterator<unsigned int, false>::offset(long) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/Sequence.h:252:42
#32 0x000055c8daaca3a2 llvm::detail::SafeIntIterator<unsigned int, false>::operator++() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/Sequence.h:215:23
#33 0x000055c8daaca3a2 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:12547:23
#34 0x000055c8dab81294 (anonymous namespace)::HorizontalReduction::tryToReduce(llvm::slpvectorizer::BoUpSLP&, llvm::DataLayout const&, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo const&, llvm::AssumptionCache*, llvm::DominatorTree&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:26845:52
#35 0x000055c8dab3e009 llvm::SLPVectorizerPass::vectorizeHorReduction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&, llvm::SmallVectorImpl<llvm::WeakTrackingVH>&)::$_264::operator()(llvm::Instruction*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:28104:19
#36 0x000055c8dab3e009 llvm::SLPVectorizerPass::vectorizeHorReduction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&, llvm::SmallVectorImpl<llvm::WeakTrackingVH>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:28129:30
#37 0x000055c8dab3f5c2 llvm::SLPVectorizerPass::vectorizeRootInstruction(llvm::PHINode*, llvm::Instruction*, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:28269:14
#38 0x000055c8dab34868 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:0:27
#39 0x000055c8dab31456 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:25025:13
#40 0x000055c8dab308ff llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:24953:18
#41 0x000055c8da35042d llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#42 0x000055c8d8e2fad7 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#43 0x000055c8d6cef64d llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#44 0x000055c8d8e336be llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/PassManager.cpp:132:23
#45 0x000055c8d6cf02ed llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#46 0x000055c8d8e2ebb7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#47 0x000055c8d973433e llvm::SmallPtrSetImplBase::isSmall() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:248:33
#48 0x000055c8d973433e llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:89:10
#49 0x000055c8d973433e llvm::PreservedAnalyses::~PreservedAnalyses() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/Analysis.h:112:7
#50 0x000055c8d973433e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1209:5
#51 0x000055c8d972bf42 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1218:7
#52 0x000055c8d972bf42 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:3
#53 0x000055c8d972bf42 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1475:13
#54 0x000055c8d9c99bd1 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unique_ptr.h:395:6
#55 0x000055c8d9c99bd1 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#56 0x000055c8db1dd3c7 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_iterator.h:1073:20
#57 0x000055c8db1dd3c7 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h:869:16
#58 0x000055c8db1dd3c7 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Sema/TemplateInstCallback.h:54:16
#59 0x000055c8db1dd3c7 clang::ParseAST(clang::Sema&, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:190:3
#60 0x000055c8d9f238d6 clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1330:10
#61 0x000055c8d9e9389f llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#62 0x000055c8d9e9389f llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#63 0x000055c8d9e9389f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:989:23
#64 0x000055c8da0195e5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:318:25
#65 0x000055c8d68e2578 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:302:15
#66 0x000055c8d68dea60 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:12
#67 0x000055c8d68ddb5c clang_main(int, char**, llvm::ToolContext const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:12
#68 0x000055c8d68eda2d main /data/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:10
#69 0x00007f997edd5d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#70 0x00007f997edd5e40 call_init ./csu/../csu/libc-start.c:128:20
#71 0x00007f997edd5e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#72 0x000055c8d68dc325 _start (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0xf6c325)
clang-trunk: error: unable to execute command: Aborted (core dumped)
clang-trunk: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 1f9c54a15a87f72ca45fb47ec006d1eae63f4eb0)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-c43c10.c
clang-trunk: note: diagnostic msg: /tmp/small-c43c10.sh
clang-trunk: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - alexey-bataev

The crash was already fixed, need to wait for the compiler update in godbolt

---

