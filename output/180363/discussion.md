# clang crashes on valid code at -O3 on x86_64-linux-gnu: Assertion `!verifyFunction(*F, &dbgs())' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/180363
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-02-08T20:37:15Z

## Body

Compiler Explorer: https://godbolt.org/z/cYs5resov

Note:
- fail: trunk
- work: 21.1.0 and earlier

```
[526] % clangtk -v
clang version 23.0.0git (https://github.com/llvm/llvm-project.git eff21afae01f4a8269a4beb6a7907d2abe6ad82a)
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
[527] % 
[527] % clangtk -O3 small.c
Instruction does not dominate all uses!
  %4 = select i1 %2, <4 x i32> zeroinitializer, <4 x i32> splat (i32 2)
  %3 = extractelement <4 x i32> %4, i32 0
clang-23: /local/suz-local/software/clangbuild/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:9884: bool llvm::LoopVectorizePass::processLoop(llvm::Loop*): Assertion `!verifyFunction(*F, &dbgs())' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/local/suz-local/software/emitesting/bugs/20260207-clangtk-m64-O3-build-072223/delta -fcoverage-compilation-dir=/local/suz-local/software/emitesting/bugs/20260207-clangtk-m64-O3-build-072223/delta -resource-dir /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/23 -I /usr/local/include -I /local/suz-local/software/local/include -internal-isystem /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=130 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/small-f0cadb.o -x c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "k"
 #0 0x000055f9b7427340 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x4527340)
 #1 0x000055f9b742401f llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x452401f)
 #2 0x000055f9b7424172 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f5c913be520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f5c914129fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f5c914129fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f5c914129fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f5c913be476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f5c913a47f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f5c913a471b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f5c913b5e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x000055f9b8f1951b llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x601951b)
#12 0x000055f9b8f1a8d1 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x601a8d1)
#13 0x000055f9b8f1aff5 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x601aff5)
#14 0x000055f9b84fed26 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x55fed26)
#15 0x000055f9b6d25892 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x3e25892)
#16 0x000055f9b42e5506 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x13e5506)
#17 0x000055f9b6d238a9 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x3e238a9)
#18 0x000055f9b42e5ba6 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x13e5ba6)
#19 0x000055f9b6d245f5 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x3e245f5)
#20 0x000055f9b76eb5ce (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#21 0x000055f9b76eeea1 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x47eeea1)
#22 0x000055f9b7d91aa2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x4e91aa2)
#23 0x000055f9b98f3d0c clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x69f3d0c)
#24 0x000055f9b80c03c9 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x51c03c9)
#25 0x000055f9b803dee5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x513dee5)
#26 0x000055f9b81b9073 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0x52b9073)
#27 0x000055f9b3e20de0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0xf20de0)
#28 0x000055f9b3e1719a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x000055f9b3e1b538 clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0xf1b538)
#30 0x000055f9b3ce200b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0xde200b)
#31 0x00007f5c913a5d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007f5c913a5e40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f5c913a5e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000055f9b3e168b5 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-23+0xf168b5)
clangtk: error: unable to execute command: Aborted
clangtk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git eff21afae01f4a8269a4beb6a7907d2abe6ad82a)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-e580ca.c
clangtk: note: diagnostic msg: /tmp/small-e580ca.sh
clangtk: note: diagnostic msg: 

********************
[528] % 
[528] % cat small.c
```
```c
struct a {
  int b;
  int c;
  int d;
  int e;
  int f;
};
int *g, i;
static int h;
extern void s(struct a p);
extern void t(int *q);
void k() {
  int l = 1;
  unsigned n;
  goto m;
  while (1) {
    i = n;
    l = 0;
  m:
    h = 0;
    for (; h < 3; h++) {
      for (n = 2; n; n++)
        if (*g)
          break;
      if (l)
        t(0);
    }
  }
}
int main() {
  struct a o = {0,0,0,0,0};
  if (i)
    s(o);
  if (o.b)
    t(&h);
  return 0;
}
```

## Comments

### Comment 1 - fhahn

thanks, will check

---

