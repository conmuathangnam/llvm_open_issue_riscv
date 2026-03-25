# Clang crashes when compiling with PGO

**Author:** Slice0w0
**URL:** https://github.com/llvm/llvm-project/issues/166035
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-11-03T16:00:29Z

## Body

The crash happens on the last commit `8331c732b4ce523e0731981f`

```
$ clang-trunk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 8331c732b4ce523e0731981ffd42f4e3f4064d2d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

Clang compiles successfully under plain `-O3` but it crashes when using PGO `-O3 -fprofile-instr-generate`

```
$ clang-trunk -O3  abc.c
$ clang-trunk -O3  -fprofile-instr-generate abc.c
clang-22: /data/soft/trunk/llvm-project/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:21837: void llvm::slpvectorizer::BoUpSLP::scheduleBlock(const llvm::slpvectorizer::BoUpSLP&, BlockScheduling*): Assertion `all_of(Bundles, [](const ScheduleBundle *Bundle) { return Bundle->isScheduled(); }) && "must be scheduled at this point"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /data/installed/root-clang/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name abc.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/absozero/projects/reduce -fprofile-instrument=clang -fcoverage-compilation-dir=/home/absozero/projects/reduce -resource-dir /data/installed/root-clang/lib/clang/22 -I/data/installed/root-bin/include/ -internal-isystem /data/installed/root-clang/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=176 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/abc-11c2d3.o -x c abc.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "abc.c"
4.	Running pass "slp-vectorizer" on function "main"
 #0 0x000059f4656e6dc2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/installed/root-clang/bin/clang-22+0x43e8dc2)
 #1 0x000059f4656e3abf llvm::sys::RunSignalHandlers() (/data/installed/root-clang/bin/clang-22+0x43e5abf)
 #2 0x000059f4656e3c0c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000075f681e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000075f681e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000075f681e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000075f681e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000075f681e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000075f681e288ff abort ./stdlib/abort.c:81:7
 #9 0x000075f681e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000075f681e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000059f46728a217 llvm::slpvectorizer::BoUpSLP::scheduleBlock(llvm::slpvectorizer::BoUpSLP const&, llvm::slpvectorizer::BoUpSLP::BlockScheduling*) (/data/installed/root-clang/bin/clang-22+0x5f8c217)
#12 0x000059f46729c30b llvm::slpvectorizer::BoUpSLP::vectorizeTree(llvm::SmallDenseSet<llvm::Value*, 4u, llvm::DenseMapInfo<llvm::Value*, void>> const&, llvm::Instruction*, llvm::ArrayRef<std::tuple<llvm::Value*, unsigned int, bool>>) (/data/installed/root-clang/bin/clang-22+0x5f9e30b)
#13 0x000059f4672a017a llvm::slpvectorizer::BoUpSLP::vectorizeTree() (/data/installed/root-clang/bin/clang-22+0x5fa217a)
#14 0x000059f4672bba0e llvm::SLPVectorizerPass::vectorizeStoreChain(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, unsigned int, unsigned int, unsigned int&) (/data/installed/root-clang/bin/clang-22+0x5fbda0e)
#15 0x000059f4672bd1fe llvm::SLPVectorizerPass::vectorizeStores(llvm::ArrayRef<llvm::StoreInst*>, llvm::slpvectorizer::BoUpSLP&, llvm::DenseSet<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, llvm::DenseMapInfo<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, void>>&)::'lambda'(std::map<long, unsigned int, std::less<long>, std::allocator<std::pair<long const, unsigned int>>> const&)::operator()(std::map<long, unsigned int, std::less<long>, std::allocator<std::pair<long const, unsigned int>>> const&) const SLPVectorizer.cpp:0:0
#16 0x000059f4672bfc98 llvm::SLPVectorizerPass::vectorizeStores(llvm::ArrayRef<llvm::StoreInst*>, llvm::slpvectorizer::BoUpSLP&, llvm::DenseSet<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, llvm::DenseMapInfo<std::tuple<llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*, unsigned int>, void>>&) (/data/installed/root-clang/bin/clang-22+0x5fc1c98)
#17 0x000059f4672c0c65 llvm::SLPVectorizerPass::vectorizeStoreChains(llvm::slpvectorizer::BoUpSLP&) (/data/installed/root-clang/bin/clang-22+0x5fc2c65)
#18 0x000059f4672c2b7a llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (/data/installed/root-clang/bin/clang-22+0x5fc4b7a)
#19 0x000059f4672c3829 llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x5fc5829)
#20 0x000059f4667895e5 llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x548b5e5)
#21 0x000059f464ffc5c5 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x3cfe5c5)
#22 0x000059f4626a8935 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x13aa935)
#23 0x000059f464ffacc1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x3cfccc1)
#24 0x000059f4626a9075 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x13ab075)
#25 0x000059f464ffb4ad llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x3cfd4ad)
#26 0x000059f4659a1932 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#27 0x000059f4659a590d clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/installed/root-clang/bin/clang-22+0x46a790d)
#28 0x000059f46602d060 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/installed/root-clang/bin/clang-22+0x4d2f060)
#29 0x000059f467b27dec clang::ParseAST(clang::Sema&, bool, bool) (/data/installed/root-clang/bin/clang-22+0x6829dec)
#30 0x000059f466358107 clang::FrontendAction::Execute() (/data/installed/root-clang/bin/clang-22+0x505a107)
#31 0x000059f4662ddf0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/installed/root-clang/bin/clang-22+0x4fdff0e)
#32 0x000059f46645141e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/installed/root-clang/bin/clang-22+0x515341e)
#33 0x000059f46221b09b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/installed/root-clang/bin/clang-22+0xf1d09b)
#34 0x000059f4622105e7 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x000059f46221535e clang_main(int, char**, llvm::ToolContext const&) (/data/installed/root-clang/bin/clang-22+0xf1735e)
#36 0x000059f4620ab74a main (/data/installed/root-clang/bin/clang-22+0xdad74a)
#37 0x000075f681e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#38 0x000075f681e2a28b call_init ./csu/../csu/libc-start.c:128:20
#39 0x000075f681e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#40 0x000059f46220fb55 _start (/data/installed/root-clang/bin/clang-22+0xf11b55)
clang-trunk: error: unable to execute command: Aborted
clang-trunk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 8331c732b4ce523e0731981ffd42f4e3f4064d2d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/installed/root-clang/bin
Build config: +assertions
clang-trunk: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/abc-afc680.c
clang-trunk: note: diagnostic msg: /tmp/abc-afc680.sh
clang-trunk: note: diagnostic msg:

********************
```

The test file is 

```c
char a, b;
int c, d;
struct e {
  unsigned f;
} g;
short h;
volatile int i;
static int j[] = {3, 3};
char(k)(char l) { return l; }
char(m)(char l) { return l < 0 ? 0 : l; }
void(n)() { a || c ? 0 : a; }
void(o)(int l, int p) { (l && p && l > p) || 0 ? 0 : 0; }
void(q)(char l) { l == 0 ? 0 : l; }
static int r() {
  short s;
  struct e t;
  for (; h; h++) {
    g.f = 0;
    for (; g.f <= 2; g.f++) {
      q(0);
      s = s == j;
      q(d);
      o((0 != t.f) != j[1], 5);
      t = g;
    }
    j[1] = 9;
    n();
    b = k(i || 0 || (o(m(b), t.f), t.f)) && 0;
  }
}
void main() { r(); }
```


## Comments

### Comment 1 - aviralgarg05

I would like to solve this issue

---

### Comment 2 - alexey-bataev

> I would like to solve this issue

This one is very tricky, deals with the multiple deps for copyable elements and parent nodes, which do not require scheduling.

---

