# [clang][OpenMP] Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169988

## Body

Note: using `-O3 -fopenmp -mllvm -jump-threading-across-loop-headers -fsanitize=signed-integer-overflow -mllvm -ubsan-guard-checks`

Reproducer:
https://godbolt.org/z/cxqbs7WGM
```cpp
extern void dummy (int);

void foo (void)
{
  [[omp::sequence (directive (tile sizes(5, 6)),
                   directive (tile sizes(1, 2, 3)))]]
  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
    for (int k = 0; k < 100; ++k)
      dummy (i);

}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/IR/Instructions.h:2817: llvm::Value* llvm::PHINode::getIncomingValueForBlock(const llvm::BasicBlock*) const: Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -fopenmp -mllvm -jump-threading-across-loop-headers -fsanitize=signed-integer-overflow -mllvm -ubsan-guard-checks <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "<source>"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "<source>"
5.	Running pass "jump-threading" on function "_Z3foov"
 #0 0x0000000004273358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273358)
 #1 0x0000000004270784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4270784)
 #2 0x00000000041b44b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000738466c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000738466c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000738466c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000738466c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000738466c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000738466c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000084cb8ef addPHINodeEntriesForMappedBlock(llvm::BasicBlock*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::ValueMap<llvm::Value const*, llvm::WeakTrackingVH, llvm::ValueMapConfig<llvm::Value const*, llvm::sys::SmartMutex<false>>>&) JumpThreading.cpp:0:0
#10 0x00000000084d840b llvm::JumpThreadingPass::threadEdge(llvm::BasicBlock*, llvm::SmallVectorImpl<llvm::BasicBlock*> const&, llvm::BasicBlock*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84d840b)
#11 0x00000000084d93c5 llvm::JumpThreadingPass::threadThroughTwoBasicBlocks(llvm::BasicBlock*, llvm::BasicBlock*, llvm::BasicBlock*, llvm::BasicBlock*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84d93c5)
#12 0x00000000084d9e32 llvm::JumpThreadingPass::maybethreadThroughTwoBasicBlocks(llvm::BasicBlock*, llvm::Value*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84d9e32)
#13 0x00000000084da6c2 llvm::JumpThreadingPass::processThreadableEdges(llvm::Value*, llvm::BasicBlock*, llvm::jumpthreading::ConstantPreference, llvm::Instruction*) (.part.0) JumpThreading.cpp:0:0
#14 0x00000000084dbc12 llvm::JumpThreadingPass::processBlock(llvm::BasicBlock*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84dbc12)
#15 0x00000000084dc92a llvm::JumpThreadingPass::runImpl(llvm::Function&, llvm::AnalysisManager<llvm::Function>*, llvm::TargetLibraryInfo*, llvm::TargetTransformInfo*, llvm::LazyValueInfo*, llvm::AAResults*, std::unique_ptr<llvm::DomTreeUpdater, std::default_delete<llvm::DomTreeUpdater>>, llvm::BlockFrequencyInfo*, llvm::BranchProbabilityInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84dc92a)
#16 0x00000000084dd3ad llvm::JumpThreadingPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84dd3ad)
#17 0x000000000451fb3e llvm::detail::PassModel<llvm::Function, llvm::JumpThreadingPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x451fb3e)
#18 0x0000000003bc69c1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bc69c1)
#19 0x00000000012600ce llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x12600ce)
#20 0x0000000003028172 llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3028172)
#21 0x000000000125edee llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::CGSCCToFunctionPassAdaptor, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x125edee)
#22 0x000000000301fe82 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x301fe82)
#23 0x00000000054411de llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x54411de)
#24 0x0000000003024f96 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3024f96)
#25 0x00000000054411fe llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::DevirtSCCRepeatedPass, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x54411fe)
#26 0x0000000003021f11 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3021f11)
#27 0x000000000125edce llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x125edce)
#28 0x0000000003bc4a41 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bc4a41)
#29 0x0000000005b53333 llvm::ModuleInlinerWrapperPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5b53333)
#30 0x00000000052f1dee llvm::detail::PassModel<llvm::Module, llvm::ModuleInlinerWrapperPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52f1dee)
#31 0x0000000003bc4a41 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bc4a41)
#32 0x000000000452dcb3 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#33 0x0000000004531389 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531389)
#34 0x0000000004bc1f37 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc1f37)
#35 0x000000000695755c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695755c)
#36 0x0000000004bc2398 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2398)
#37 0x0000000004eadba5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eadba5)
#38 0x0000000004e2dd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2dd5e)
#39 0x0000000004fa6bdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fa6bdd)
#40 0x0000000000dc7650 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7650)
#41 0x0000000000dbe0ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#42 0x0000000000dbe24d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#43 0x0000000004c2ab89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x00000000041b4954 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b4954)
#45 0x0000000004c2b19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004beb992 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beb992)
#47 0x0000000004bec93e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bec93e)
#48 0x0000000004bf3d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf3d75)
#49 0x0000000000dc3a71 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3a71)
#50 0x0000000000c72444 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72444)
#51 0x0000738466c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x0000738466c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000dbdb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdb65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/erEG8Mjzc

---

