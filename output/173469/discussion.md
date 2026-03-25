# clang crashes at -O{s,2,3} on x86_64-linux-gnu: Assertion `Picked->isReady() && "must be ready to schedule"' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/173469
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-12-24T22:15:19Z

## Body

Compiler Explorer: https://godbolt.org/z/Won4YPrE7

Note:
- fail: trunk
- work: 21.1.0 and earlier

```
[509] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git c1e72dc469565edea8ecf20700c7cfecef1ae4bf)
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
[510] % 
[510] % clangtk -O3 -c small.c
clangtk: /local/suz-local/software/clangbuild/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:21385: llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, const {anonymous}::InstructionsState&, const llvm::slpvectorizer::BoUpSLP::EdgeInfo&)::<lambda(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&)>: Assertion `Picked->isReady() && "must be ready to schedule"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clangtk -I/usr/local/include -I/local/suz-local/software/local/include -O3 -c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.	Running pass "slp-vectorizer" on function "d"
 #0 0x000055ff7137d7f0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444d7f0)
 #1 0x000055ff7137a4af llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444a4af)
 #2 0x000055ff712ba378 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f476ffb1520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f47700059fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #5 0x00007f47700059fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #6 0x00007f47700059fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #7 0x00007f476ffb1476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f476ff977f3 abort ./stdlib/./stdlib/abort.c:81:7
 #9 0x00007f476ff9771b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#10 0x00007f476ffa8e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055ff72f1e2c0 llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&)::'lambda3'(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&)::operator()(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&) const SLPVectorizer.cpp:0:0
#12 0x000055ff72f201fb llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&) SLPVectorizer.cpp:0:0
#13 0x000055ff72f22124 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff2124)
#14 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#15 0x000055ff72f23b05 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b05)
#16 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#17 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#18 0x000055ff72f249d1 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff49d1)
#19 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#20 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#21 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#22 0x000055ff72f23b05 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b05)
#23 0x000055ff72f23b7a llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef<llvm::Value*>, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b7a)
#24 0x000055ff72f64000 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x6034000)
#25 0x000055ff72f6a8dd bool tryToVectorizeSequence<llvm::Value>(llvm::SmallVectorImpl<llvm::Value*>&, llvm::function_ref<bool (llvm::Value*, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, bool)>, bool, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#26 0x000055ff72f6ac5d llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x603ac5d)
#27 0x000055ff72f72af6 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#28 0x000055ff72f7370b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x604370b)
#29 0x000055ff72442476 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5512476)
#30 0x000055ff70c98b12 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d68b12)
#31 0x000055ff6e2dd806 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13ad806)
#32 0x000055ff70c96b29 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d66b29)
#33 0x000055ff6e2ddea6 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13adea6)
#34 0x000055ff70c97875 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d67875)
#35 0x000055ff7163a7b8 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile> >&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#36 0x000055ff7163e369 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x470e369)
#37 0x000055ff71ce0e22 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4db0e22)
#38 0x000055ff737effec clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x68bffec)
#39 0x000055ff720088d9 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50d88d9)
#40 0x000055ff71f87285 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5057285)
#41 0x000055ff72100833 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x51d0833)
#42 0x000055ff6de20060 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xef0060)
#43 0x000055ff6de1642a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#44 0x000055ff6de165bf int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#45 0x000055ff71d5ca5d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x000055ff712ba820 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x438a820)
#47 0x000055ff71d5d2fe clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (.part.0) Job.cpp:0:0
#48 0x000055ff71d18f67 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4de8f67)
#49 0x000055ff71d1a001 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4dea001)
#50 0x000055ff71d25c24 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4df5c24)
#51 0x000055ff6de1be1b clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xeebe1b)
#52 0x000055ff6dce1f9b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xdb1f9b)
#53 0x00007f476ff98d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#54 0x00007f476ff98e40 call_init ./csu/../csu/libc-start.c:128:20
#55 0x00007f476ff98e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#56 0x000055ff6de15b45 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xee5b45)
clangtk: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git c1e72dc469565edea8ecf20700c7cfecef1ae4bf)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-7667eb.c
clangtk: note: diagnostic msg: /tmp/small-7667eb.sh
clangtk: note: diagnostic msg: 

********************
[511] % 
[511] % cat small.c
extern int c();
int a, b;
void d(int g, int h[], int i) {
  int e, f;
  while (a) {
    e = 1 % b - 3;
    if (c())
      goto j;
    f = 1 % a - e - 4;
    a = 1 % f + h[0];
    i = 1 % g + f - 1;
    g = 1 % h[1] - -2 * a - 1;
  j:
    i = 1 / i - h[2] + 2;
    g = 1 / f + (1 - g);
    if (i)
      c();
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Zhendong Su (zhendongsu)

<details>
Compiler Explorer: https://godbolt.org/z/Won4YPrE7

Note:
- fail: trunk
- work: 21.1.0 and earlier

```
[509] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git c1e72dc469565edea8ecf20700c7cfecef1ae4bf)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@<!-- -->m64
Candidate multilib: 32;@<!-- -->m32
Candidate multilib: x32;@<!-- -->mx32
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /usr/local/cuda, version 12.1
[510] % 
[510] % clangtk -O3 -c small.c
clangtk: /local/suz-local/software/clangbuild/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:21385: llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::slpvectorizer::BoUpSLP*, const {anonymous}::InstructionsState&amp;, const llvm::slpvectorizer::BoUpSLP::EdgeInfo&amp;)::&lt;lambda(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&amp;)&gt;: Assertion `Picked-&gt;isReady() &amp;&amp; "must be ready to schedule"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clangtk -I/usr/local/include -I/local/suz-local/software/local/include -O3 -c small.c
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "small.c"
4.	Running pass "slp-vectorizer" on function "d"
 #<!-- -->0 0x000055ff7137d7f0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444d7f0)
 #<!-- -->1 0x000055ff7137a4af llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x444a4af)
 #<!-- -->2 0x000055ff712ba378 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f476ffb1520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f47700059fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f47700059fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f47700059fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f476ffb1476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f476ff977f3 abort ./stdlib/./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f476ff9771b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f476ffa8e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x000055ff72f1e2c0 llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&amp;, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;)::'lambda3'(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&amp;)::operator()(bool, llvm::slpvectorizer::BoUpSLP::ScheduleBundle&amp;) const SLPVectorizer.cpp:0:0
#<!-- -->12 0x000055ff72f201fb llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::slpvectorizer::BoUpSLP*, (anonymous namespace)::InstructionsState const&amp;, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;) SLPVectorizer.cpp:0:0
#<!-- -->13 0x000055ff72f22124 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff2124)
#<!-- -->14 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->15 0x000055ff72f23b05 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b05)
#<!-- -->16 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->17 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->18 0x000055ff72f249d1 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff49d1)
#<!-- -->19 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->20 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->21 0x000055ff72f246a9 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff46a9)
#<!-- -->22 0x000055ff72f23b05 llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b05)
#<!-- -->23 0x000055ff72f23b7a llvm::slpvectorizer::BoUpSLP::buildTreeRec(llvm::ArrayRef&lt;llvm::Value*&gt;, unsigned int, llvm::slpvectorizer::BoUpSLP::EdgeInfo const&amp;, unsigned int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ff3b7a)
#<!-- -->24 0x000055ff72f64000 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::slpvectorizer::BoUpSLP&amp;, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x6034000)
#<!-- -->25 0x000055ff72f6a8dd bool tryToVectorizeSequence&lt;llvm::Value&gt;(llvm::SmallVectorImpl&lt;llvm::Value*&gt;&amp;, llvm::function_ref&lt;bool (llvm::Value*, llvm::Value*)&gt;, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::Value*)&gt;, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;llvm::Value*&gt;, bool)&gt;, bool, llvm::slpvectorizer::BoUpSLP&amp;) (.constprop.0) SLPVectorizer.cpp:0:0
#<!-- -->26 0x000055ff72f6ac5d llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x603ac5d)
#<!-- -->27 0x000055ff72f72af6 llvm::SLPVectorizerPass::runImpl(llvm::Function&amp;, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#<!-- -->28 0x000055ff72f7370b llvm::SLPVectorizerPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x604370b)
#<!-- -->29 0x000055ff72442476 llvm::detail::PassModel&lt;llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager&lt;llvm::Function&gt; &gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5512476)
#<!-- -->30 0x000055ff70c98b12 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt; &gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d68b12)
#<!-- -->31 0x000055ff6e2dd806 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt; &gt;, llvm::AnalysisManager&lt;llvm::Function&gt; &gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13ad806)
#<!-- -->32 0x000055ff70c96b29 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d66b29)
#<!-- -->33 0x000055ff6e2ddea6 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt; &gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13adea6)
#<!-- -->34 0x000055ff70c97875 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt; &gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3d67875)
#<!-- -->35 0x000055ff7163a7b8 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt; &gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt; &gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->36 0x000055ff7163e369 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt; &gt;, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x470e369)
#<!-- -->37 0x000055ff71ce0e22 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4db0e22)
#<!-- -->38 0x000055ff737effec clang::ParseAST(clang::Sema&amp;, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x68bffec)
#<!-- -->39 0x000055ff720088d9 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50d88d9)
#<!-- -->40 0x000055ff71f87285 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5057285)
#<!-- -->41 0x000055ff72100833 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x51d0833)
#<!-- -->42 0x000055ff6de20060 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xef0060)
#<!-- -->43 0x000055ff6de1642a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->44 0x000055ff6de165bf int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->45 0x000055ff71d5ca5d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x000055ff712ba820 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x438a820)
#<!-- -->47 0x000055ff71d5d2fe clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x000055ff71d18f67 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4de8f67)
#<!-- -->49 0x000055ff71d1a001 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4dea001)
#<!-- -->50 0x000055ff71d25c24 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4df5c24)
#<!-- -->51 0x000055ff6de1be1b clang_main(int, char**, llvm::ToolContext const&amp;) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xeebe1b)
#<!-- -->52 0x000055ff6dce1f9b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xdb1f9b)
#<!-- -->53 0x00007f476ff98d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->54 0x00007f476ff98e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->55 0x00007f476ff98e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->56 0x000055ff6de15b45 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xee5b45)
clangtk: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git c1e72dc469565edea8ecf20700c7cfecef1ae4bf)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-7667eb.c
clangtk: note: diagnostic msg: /tmp/small-7667eb.sh
clangtk: note: diagnostic msg: 

********************
[511] % 
[511] % cat small.c
extern int c();
int a, b;
void d(int g, int h[], int i) {
  int e, f;
  while (a) {
    e = 1 % b - 3;
    if (c())
      goto j;
    f = 1 % a - e - 4;
    a = 1 % f + h[0];
    i = 1 % g + f - 1;
    g = 1 % h[1] - -2 * a - 1;
  j:
    i = 1 / i - h[2] + 2;
    g = 1 / f + (1 - g);
    if (i)
      c();
  }
}
```
</details>


---

### Comment 2 - dtcxzyw

Reduced reproducer:
```
; bin/opt -passes=slp-vectorizer test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @d() {
entry:
  br label %while.body

while.body:                                       ; preds = %j, %entry
  %g.addr.033 = phi i32 [ 0, %entry ], [ %add18, %j ]
  %i.addr.032 = phi i32 [ 0, %entry ], [ %add15, %j ]
  br i1 false, label %if.end, label %j

if.end:                                           ; preds = %while.body
  %sub3 = add i32 0, 0
  %sub4 = or i32 %sub3, 0
  %add7 = or i32 %sub3, 0
  %rem6 = srem i32 0, 0
  %sub8 = or i32 %add7, %rem6
  %rem10 = srem i32 0, 0
  %sub11 = or i32 %rem10, 0
  %add = add i32 0, 0
  %mul.neg = shl i32 %add, 0
  %sub12 = or i32 %sub11, %mul.neg
  br label %j

j:                                                ; preds = %if.end, %while.body
  %i.addr.1 = phi i32 [ %i.addr.032, %while.body ], [ %sub8, %if.end ]
  %g.addr.1 = phi i32 [ %g.addr.033, %while.body ], [ %sub12, %if.end ]
  %f.1 = phi i32 [ 0, %while.body ], [ %sub4, %if.end ]
  %i.addr.1.fr = freeze i32 %i.addr.1
  %div = select i1 false, i32 %i.addr.1.fr, i32 0
  %add15 = or i32 %div, 0
  %f.1.fr = freeze i32 %f.1
  %div16 = select i1 false, i32 %f.1.fr, i32 0
  %add18 = or i32 %div16, 0
  br label %while.body
}

```

---

### Comment 3 - Rajveer100

cc @alexey-bataev 

---

