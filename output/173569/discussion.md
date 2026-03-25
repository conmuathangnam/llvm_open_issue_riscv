# clang crashes at -O{2,3} on x86_64-linux-gnu: Assertion `hasUseList()' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/173569
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-12-25T18:11:06Z

## Body

Compiler Explorer: https://godbolt.org/z/vMo7rzrnd

Note:
- fail: trunk
- work: 21.1.0 and earlier

```
[517] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 6d1e7d4982fabc9e245897056a5425496df6a7a3)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.1
[518] % 
[518] % clangtk -O3 -c small.c
clangtk: /local/suz-local/software/clangbuild/llvm-project/llvm/include/llvm/IR/Value.h:357: llvm::Value::use_iterator llvm::Value::materialized_use_begin(): Assertion `hasUseList()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clangtk -I/usr/local/include -I/local/suz-local/software/local/include -O3 -c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.	Running pass "slp-vectorizer" on function "o"
 #0 0x000055aa8ebc64e0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444f4e0)
 #1 0x000055aa8ebc319f llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444c19f)
 #2 0x000055aa8eb03018 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc73d5df520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fc73d6339fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #5 0x00007fc73d6339fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #6 0x00007fc73d6339fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #7 0x00007fc73d5df476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007fc73d5c57f3 abort ./stdlib/./stdlib/abort.c:81:7
 #9 0x00007fc73d5c571b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#10 0x00007fc73d5d6e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055aa8c666bb9 (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1eefbb9)
#12 0x000055aa906acf82 isCommutative(llvm::Instruction*, llvm::Value*, bool) SLPVectorizer.cpp:0:0
#13 0x000055aa9070bd69 llvm::slpvectorizer::BoUpSLP::VLOperands::VLOperands(llvm::ArrayRef<llvm::Value*>, llvm::ArrayRef<llvm::SmallVector<llvm::Value*, 8u> >, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP const&) SLPVectorizer.cpp:0:0
#14 0x000055aa9076f9ca llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff89ca)
#15 0x000055aa9076fc9f llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff8c9f)
#16 0x000055aa907af180 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x6038180)
#17 0x000055aa907b37aa bool llvm::SLPVectorizerPass::vectorizeCmpInsts<std::reverse_iterator<llvm::CmpInst* const*> >(llvm::iterator_range<std::reverse_iterator<llvm::CmpInst* const*> >, llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x603c7aa)
#18 0x000055aa907b4213 llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&)::'lambda3'(bool)::operator()(bool) const SLPVectorizer.cpp:0:0
#19 0x000055aa907b738a llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x604038a)
#20 0x000055aa907bdc76 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#21 0x000055aa907be88b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x604788b)
#22 0x000055aa8fc8bbf6 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5514bf6)
#23 0x000055aa8e4e17a2 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d6a7a2)
#24 0x000055aa8bb25466 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13ae466)
#25 0x000055aa8e4df7b9 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d687b9)
#26 0x000055aa8bb25b06 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13aeb06)
#27 0x000055aa8e4e0505 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d69505)
#28 0x000055aa8ee83868 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile> >&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#29 0x000055aa8ee87419 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4710419)
#30 0x000055aa8f52a382 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4db3382)
#31 0x000055aa9103b49c clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x68c449c)
#32 0x000055aa8f851f09 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50daf09)
#33 0x000055aa8f7d08d5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50598d5)
#34 0x000055aa8f94a1f3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x51d31f3)
#35 0x000055aa8b668070 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xef1070)
#36 0x000055aa8b65e43a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x000055aa8b65e5cf int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x000055aa8f5a5fbd void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x000055aa8eb034c0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x438c4c0)
#40 0x000055aa8f5a685e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (.part.0) Job.cpp:0:0
#41 0x000055aa8f5624c7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4deb4c7)
#42 0x000055aa8f563561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4dec561)
#43 0x000055aa8f56f184 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4df8184)
#44 0x000055aa8b663e2b clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xeece2b)
#45 0x000055aa8b529fab main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xdb2fab)
#46 0x00007fc73d5c6d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#47 0x00007fc73d5c6e40 call_init ./csu/../csu/libc-start.c:128:20
#48 0x00007fc73d5c6e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#49 0x000055aa8b65db55 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xee6b55)
clangtk: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 6d1e7d4982fabc9e245897056a5425496df6a7a3)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-3570d4.c
clangtk: note: diagnostic msg: /tmp/small-3570d4.sh
clangtk: note: diagnostic msg: 

********************
[519] % 
[519] % cat small.c
int a, b, f, g;
static int h = 1;
int j(int k[]) {
  int c[] = {-1, -2, 0, 4};
  while (1) {
    int d = 1;
    for (int e = 0; e < 4; e++)
      if (k[e] != c[e])
        d = 0;
    if (d)
      return 0;
  }
}
void l(int k, int n) {
  if (1 / k)
    goto m;
  b = 0;
  if (n)
    f = 0;
  do {
    n = -f;
  m:
    ;
  } while (a > 0);
  int i[] = {a, k, n, b};
  j(i);
}
void o() {
  for (int p = 0; p < 2; p++)
    l(h - 3, h);
}
void q() {
  o();
  h = g;
}
```

