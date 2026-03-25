# clang crashes at -O1 and above on x86_64-linux-gnu: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/161137
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2025-09-29T12:37:41Z

## Body

Compiler Explorer: https://godbolt.org/z/6hcjfv3dv

```
[508] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git abffc542ff876d9e74d64ee5cb83ec405ec2e70e)
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
[509] % 
[509] % clangtk -O1 small.c
clang-22: /local/suz-local/software/clangbuild/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = llvm::SelectInst; From = llvm::Value]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O1 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/local/suz-local/software/emitesting/bugs/20250929-clangtk-m64-O3-build-064159/delta -fcoverage-compilation-dir=/local/suz-local/software/emitesting/bugs/20250929-clangtk-m64-O3-build-064159/delta -resource-dir /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/22 -I /usr/local/include -I /local/suz-local/software/local/include -internal-isystem /local/home/suz/suz-local/software/local/clang-trunk/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=130 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/small-9bbc20.o -x c small.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(mem2reg,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>)" on module "small.c"
4.	Running pass "simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>" on function "main"
 #0 0x0000561abed1d620 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4324620)
 #1 0x0000561abed1a2df llvm::sys::RunSignalHandlers() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x43212df)
 #2 0x0000561abed1a432 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f1f4810b520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f1f4815f9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f1f4815f9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f1f4815f9fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f1f4810b476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f1f480f17f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f1f480f171b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f1f48102e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x0000561abe80ce34 decltype(auto) llvm::cast<llvm::SelectInst, llvm::Value>(llvm::Value*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3e13e34)
#12 0x0000561abeee538c trySwitchToSelect(llvm::SwitchInst*, llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter>&, llvm::DomTreeUpdater*, llvm::DataLayout const&, llvm::TargetTransformInfo const&) SimplifyCFG.cpp:0:0
#13 0x0000561abeef0009 (anonymous namespace)::SimplifyCFGOpt::simplifySwitch(llvm::SwitchInst*, llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter>&) SimplifyCFG.cpp:0:0
#14 0x0000561abeef82b6 llvm::simplifyCFG(llvm::BasicBlock*, llvm::TargetTransformInfo const&, llvm::DomTreeUpdater*, llvm::SimplifyCFGOptions const&, llvm::ArrayRef<llvm::WeakVH>) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x44ff2b6)
#15 0x0000561abebef121 iterativelySimplifyCFG(llvm::Function&, llvm::TargetTransformInfo const&, llvm::DomTreeUpdater*, llvm::SimplifyCFGOptions const&) SimplifyCFGPass.cpp:0:0
#16 0x0000561abebf0052 simplifyFunctionCFGImpl(llvm::Function&, llvm::TargetTransformInfo const&, llvm::DominatorTree*, llvm::SimplifyCFGOptions const&) SimplifyCFGPass.cpp:0:0
#17 0x0000561abebf1195 simplifyFunctionCFG(llvm::Function&, llvm::TargetTransformInfo const&, llvm::DominatorTree*, llvm::SimplifyCFGOptions const&) SimplifyCFGPass.cpp:0:0
#18 0x0000561abebf12ee llvm::SimplifyCFGPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x41f82ee)
#19 0x0000561abc76d8f6 llvm::detail::PassModel<llvm::Function, llvm::SimplifyCFGPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1d748f6)
#20 0x0000561abe64df12 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c54f12)
#21 0x0000561abbd2cdb6 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x1333db6)
#22 0x0000561abe64bdb9 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c52db9)
#23 0x0000561abbd2d4a6 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x13344a6)
#24 0x0000561abe64cb05 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x3c53b05)
#25 0x0000561abefd0a83 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#26 0x0000561abefd4c93 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x45dbc93)
#27 0x0000561abf65b7da clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4c627da)
#28 0x0000561ac11108cc clang::ParseAST(clang::Sema&, bool, bool) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x67178cc)
#29 0x0000561abf9778b9 clang::FrontendAction::Execute() (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4f7e8b9)
#30 0x0000561abf8f1d85 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x4ef8d85)
#31 0x0000561abfa6b763 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0x5072763)
#32 0x0000561abb89c227 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xea3227)
#33 0x0000561abb8926fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000561abb896aa4 clang_main(int, char**, llvm::ToolContext const&) (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xe9daa4)
#35 0x0000561abb76215b main (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xd6915b)
#36 0x00007f1f480f2d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#37 0x00007f1f480f2e40 call_init ./csu/../csu/libc-start.c:128:20
#38 0x00007f1f480f2e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#39 0x0000561abb891e15 _start (/local/home/suz/suz-local/software/local/clang-trunk/bin/clang-22+0xe98e15)
clangtk: error: unable to execute command: Aborted
clangtk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git abffc542ff876d9e74d64ee5cb83ec405ec2e70e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
clangtk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clangtk: note: diagnostic msg: /tmp/small-bb6bbe.c
clangtk: note: diagnostic msg: /tmp/small-bb6bbe.sh
clangtk: note: diagnostic msg: 

********************
[510] % 
[510] % cat small.c
int a, b;
static int *c;
int main() {
  int e;
  if (!b)
    return a;
  {
    int f;
    if (*c)
      goto g;
  }
g:
  return 0;
}
```

## Comments

### Comment 1 - antoniofrighetto

Related to: https://github.com/llvm/llvm-project/pull/159645.

---

