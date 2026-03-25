# clang crashes at -O{s,2,3} on x86_64-linux-gnu: Assertion `isa< To>(Val) && "cast<Ty>() argument of incompatible type!"' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/162663
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-10T23:59:28Z

## Body

This appears to be a recent regression as the latest Compiler Explorer build doesn't crash.

```
[530] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 45757b9284cf491072c8c477cd606df8a19061df)
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
[531] % 
[531] % clangtk -O3 -w small.c
clang-22: /local/suz-local/software/clangbuild/llvm-project/llvm/include/llvm/Support/Casting.h:573: decltype(auto) llvm::cast(From*) [with To = llvm::Instruction; From = llvm::Value]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/local/suz-local/software/emitesting/bugs/20251009-clangtk-m64-O3-build-142441/delta -fcoverage-compilation-dir=/local/suz-local/software/emitesting/bugs/20251009-clangtk-m64-O3-build-142441/delta -resource-dir /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/22 -I /usr/local/include -I /local/suz-local/software/local/include -internal-isystem /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -w -ferror-limit 19 -fmessage-length=100 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/small-561535.o -x c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.	Running pass "slp-vectorizer" on function "main"
 #0 0x000055b926cf3610 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4357610)
 #1 0x000055b926cf02cf llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x43542cf)
 #2 0x000055b926cf0422 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f79d73a9520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f79d73fd9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f79d73fd9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f79d73fd9fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f79d73a9476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f79d738f7f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f79d738f71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f79d73a0e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055b928780771 (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5de4771)
#12 0x000055b928780860 bool llvm::function_ref<bool (unsigned int, unsigned int)>::callback_fn<llvm::slpvectorizer::BoUpSLP::collectValuesToDemote(llvm::slpvectorizer::BoUpSLP::TreeEntry const&, bool, unsigned int&, llvm::SmallVectorImpl<unsigned int>&, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, void>>&, llvm::SmallDenseSet<unsigned int, 8u, llvm::DenseMapInfo<unsigned int, void>> const&, unsigned int&, bool&, bool) const::'lambda10'(unsigned int, unsigned int)>(long, unsigned int, unsigned int) SLPVectorizer.cpp:0:0
#13 0x000055b9287f3845 llvm::slpvectorizer::BoUpSLP::collectValuesToDemote(llvm::slpvectorizer::BoUpSLP::TreeEntry const&, bool, unsigned int&, llvm::SmallVectorImpl<unsigned int>&, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, void>>&, llvm::SmallDenseSet<unsigned int, 8u, llvm::DenseMapInfo<unsigned int, void>> const&, unsigned int&, bool&, bool) const::'lambda6'(unsigned int&, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::TreeEntry const*>, llvm::function_ref<bool (unsigned int, unsigned int)>)::operator()(unsigned int&, llvm::ArrayRef<llvm::slpvectorizer::BoUpSLP::TreeEntry const*>, llvm::function_ref<bool (unsigned int, unsigned int)>) const SLPVectorizer.cpp:0:0
#14 0x000055b9287f2bbc llvm::slpvectorizer::BoUpSLP::collectValuesToDemote(llvm::slpvectorizer::BoUpSLP::TreeEntry const&, bool, unsigned int&, llvm::SmallVectorImpl<unsigned int>&, llvm::DenseSet<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, llvm::DenseMapInfo<llvm::slpvectorizer::BoUpSLP::TreeEntry const*, void>>&, llvm::SmallDenseSet<unsigned int, 8u, llvm::DenseMapInfo<unsigned int, void>> const&, unsigned int&, bool&, bool) const (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5e56bbc)
#15 0x000055b928872926 llvm::slpvectorizer::BoUpSLP::computeMinimumValueSizes() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ed6926)
#16 0x000055b9288840bb llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ee80bb)
#17 0x000055b928889412 bool tryToVectorizeSequence<llvm::Value>(llvm::SmallVectorImpl<llvm::Value*>&, llvm::function_ref<bool (llvm::Value*, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, bool)>, bool, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#18 0x000055b92888a62f llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5eee62f)
#19 0x000055b928892d76 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#20 0x000055b92889398b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5ef798b)
#21 0x000055b927d7bc26 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x53dfc26)
#22 0x000055b926621ed2 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c85ed2)
#23 0x000055b923cde446 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1342446)
#24 0x000055b92661fd79 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c83d79)
#25 0x000055b923cdeb36 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1342b36)
#26 0x000055b926620ac5 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c84ac5)
#27 0x000055b926fa823b (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#28 0x000055b926fac0c3 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x46100c3)
#29 0x000055b92763635a clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4c9a35a)
#30 0x000055b9290f427c clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x675827c)
#31 0x000055b927951659 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4fb5659)
#32 0x000055b9278d1cc5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4f35cc5)
#33 0x000055b927a46563 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x50aa563)
#34 0x000055b92384d5d7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xeb15d7)
#35 0x000055b923843aaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x000055b923847e54 clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xeabe54)
#37 0x000055b92371200b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xd7600b)
#38 0x00007f79d7390d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#39 0x00007f79d7390e40 call_init ./csu/../csu/libc-start.c:128:20
#40 0x00007f79d7390e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#41 0x000055b9238431c5 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xea71c5)
clangtk: error: unable to execute command: Aborted
clangtk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 45757b9284cf491072c8c477cd606df8a19061df)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-436cbe.c
clangtk: note: diagnostic msg: /tmp/small-436cbe.sh
clangtk: note: diagnostic msg: 

********************
[532] % 
[532] % cat small.c
int a, b, d, e, g, h, k;
short c;
volatile int f, i;
char j;
int main() {
l:
  if (g)
    goto ae;
  while (a) {
    unsigned long m = k;
    if (j)
      goto q;
    c = m;
    char n = a;
  ab:
    h = c / n;
    if (m)
      goto ac;
    if (d)
      goto l;
    int l = 0 % 0;
  q:
    i;
    if (!l)
      goto ae;
    f;
  ae:
    if (d) {
      int o = -(m / e), q = ~m;
      short p = k;
      if (k)
      ac:
        if (f)
          q = o / p;
      k = q;
      j = 0;
    }
    if (b)
      goto ab;
  }
}
```

