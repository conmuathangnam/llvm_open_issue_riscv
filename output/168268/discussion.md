# clang crashes on valid code at -O3 on x86_64-linux-gnu: Assertion `UnscheduledDeps >= 0 && "Expected valid number of unscheduled deps"' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/168268
**Status:** Closed
**Labels:** regression, llvm:SLPVectorizer, crash
**Closed Date:** 2025-11-16T21:49:21Z

## Body

Compiler Explorer: https://godbolt.org/z/vn7WbvGb3

Note
* fail: trunk
* work: 21.1.0 and earlier

```
[513] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 5ed26ade0c585d2a081f23546902f64a0366de19)
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
[514] % 
[514] % clangtk -O3 -c small.c
clangtk: /local/suz-local/software/clangbuild/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:4875: int llvm::slpvectorizer::BoUpSLP::ScheduleData::incrementUnscheduledDeps(int): Assertion `UnscheduledDeps >= 0 && "Expected valid number of unscheduled deps"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clangtk -I/usr/local/include -I/local/suz-local/software/local/include -O3 -c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.	Running pass "slp-vectorizer" on function "main"
 #0 0x0000556ecaa7f6c0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x43d36c0)
 #1 0x0000556ecaa7c37f llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x43d037f)
 #2 0x0000556eca9baeb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f87cde69520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f87cdebd9fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #5 0x00007f87cdebd9fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #6 0x00007f87cdebd9fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #7 0x00007f87cde69476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f87cde4f7f3 abort ./stdlib/./stdlib/abort.c:81:7
 #9 0x00007f87cde4f71b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#10 0x00007f87cde60e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x0000556ecc5fdaba auto void llvm::slpvectorizer::BoUpSLP::BlockScheduling::schedule<llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u> >(llvm::slpvectorizer::BoUpSLP const&, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u>&)::'lambda'(llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*>)::operator()(llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*>) const::'lambda'(llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u>*, bool)::operator()<llvm::slpvectorizer::BoUpSLP::ScheduleData>(llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u>*, bool) const SLPVectorizer.cpp:0:0
#12 0x0000556ecc5fe7eb void llvm::slpvectorizer::BoUpSLP::BlockScheduling::schedule<llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u> >(llvm::slpvectorizer::BoUpSLP const&, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SetVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::SmallVector<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, 0u>, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, void> >, 0u>&)::'lambda'(llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*>)::operator()(llvm::slpvectorizer::BoUpSLP::ScheduleEntity*, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::ScheduleBundle*>) const SLPVectorizer.cpp:0:0
#13 0x0000556ecc5ffd19 llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&)::'lambda3'(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&)::operator()(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&) const SLPVectorizer.cpp:0:0
#14 0x0000556ecc601dc3 llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&) SLPVectorizer.cpp:0:0
#15 0x0000556ecc603c24 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f57c24)
#16 0x0000556ecc605f71 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f59f71)
#17 0x0000556ecc606d2a llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int)::'lambda'((anonymous namespace)::InstructionsState const&)::operator()((anonymous namespace)::InstructionsState const&) const::'lambda'(llvm::ArrayRef<llvm::Value*>, unsigned int)::operator()(llvm::ArrayRef<llvm::Value*>, unsigned int) const SLPVectorizer.cpp:0:0
#18 0x0000556ecc607b36 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int)::'lambda'((anonymous namespace)::InstructionsState const&)::operator()((anonymous namespace)::InstructionsState const&) const SLPVectorizer.cpp:0:0
#19 0x0000556ecc603b4f llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f57b4f)
#20 0x0000556ecc605444 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f59444)
#21 0x0000556ecc6054b7 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f594b7)
#22 0x0000556ecc625938 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f79938)
#23 0x0000556ecc62ad80 bool tryToVectorizeSequence<llvm::Value>(llvm::SmallVectorImpl<llvm::Value*>&, llvm::function_ref<bool (llvm::Value*, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, bool)>, bool, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#24 0x0000556ecc62bf8b llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f7ff8b)
#25 0x0000556ecc634896 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#26 0x0000556ecc6354ab llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5f894ab)
#27 0x0000556ecbb12756 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5466756)
#28 0x0000556eca39f272 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3cf3272)
#29 0x0000556ec7a20706 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1374706)
#30 0x0000556eca39d119 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3cf1119)
#31 0x0000556ec7a20e46 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1374e46)
#32 0x0000556eca39de65 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3cf1e65)
#33 0x0000556ecad36f3a (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile> >&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#34 0x0000556ecad3ae53 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x468ee53)
#35 0x0000556ecb3c8562 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4d1c562)
#36 0x0000556ecce9f2ac clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x67f32ac)
#37 0x0000556ecb6e39f9 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50379f9)
#38 0x0000556ecb662a25 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4fb6a25)
#39 0x0000556ecb7d8b63 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x512cb63)
#40 0x0000556ec757d2c7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xed12c7)
#41 0x0000556ec75736aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#42 0x0000556ec757383f int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#43 0x0000556ecb438e5d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000556eca9bb360 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x430f360)
#45 0x0000556ecb4396fe clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000556ecb3f6057 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4d4a057)
#47 0x0000556ecb3f70f1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4d4b0f1)
#48 0x0000556ecb403d84 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4d57d84)
#49 0x0000556ec757909b clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xecd09b)
#50 0x0000556ec744116b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xd9516b)
#51 0x00007f87cde50d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#52 0x00007f87cde50e40 call_init ./csu/../csu/libc-start.c:128:20
#53 0x00007f87cde50e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#54 0x0000556ec7572dc5 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xec6dc5)
clangtk: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 5ed26ade0c585d2a081f23546902f64a0366de19)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-cd8076.c
clangtk: note: diagnostic msg: /tmp/small-cd8076.sh
clangtk: note: diagnostic msg: 

********************
[515] % 
[515] % cat small.c
extern int s(int u);
extern void t(int v);
int c, f, g = 1, h, i, j, k;
short d, e;
int main() {
  int m, n = 1, o = 1, b = 0, a = 0;
p:
  t(m = c < (j && (n = 0)));
  if ((i - 1) && o < 1)
    goto p;
  if (k)
    goto q;
  if (!i)
    goto r;
  a = 1 / d;
  b = e;
r:
  if (b)
    f = a;
  int l = ~(n & e), p = ~(1 & a), q = ~(~p / g ^ ~c), r = ~(d & n);
ah:
  g = p | r | (l + q);
q:
  if (h)
    goto ah;
  while (c)
    if (s(m))
      goto p;
    else
      o = s(0);
  return 0;
}
```


## Comments

### Comment 1 - shafik

Looks like a regression on trunk

---

