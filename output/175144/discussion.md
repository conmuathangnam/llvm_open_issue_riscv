# [clang] Crash with PGO: Assertion `validateLoopEntryInstrumentation() && "Loop entries should not be in MST when " "InstrumentLoopEntries is on"' failed.

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/175144
**Status:** Open
**Labels:** PGO, crash

## Body

test.c:

```
int a, b;
void main() {
  for (; b < 256; b++)
    ;
  for (; a;)
    ;
}
```

profile.txt:

```
:ir
:instrument_loop_entries
main
74256657
3
1
256
0
```

This code crashes when compiled with PGO.

Steps to reproduce:

1. Generate profile using `profile.txt`:

```
llvm-profdata merge -o test.profdata profile.txt
```

2. Compile with PGO:

```
clang-trunk -fprofile-instr-use=test.profdata test.c  # or with '-fprofile-use'
```

Crash:

```
clang-22: /home/x27zhou/compilers/llvm-project/llvm/include/llvm/Transforms/Instrumentation/CFGMST.h:335: llvm::CFGMST<(anonymous namespace)::PGOUseEdge, (anonymous namespace)::PGOUseBBInfo>::CFGMST(llvm::Function &, bool, bool, llvm::BranchProbabilityInfo *, llvm::BlockFrequencyInfo *, llvm::LoopInfo *) [Edge = (anonymous namespace)::PGOUseEdge, BBInfo = (anonymous namespace)::PGOUseBBInfo]: Assertion `validateLoopEntryInstrumentation() && "Loop entries should not be in MST when " "InstrumentLoopEntries is on"' failed.
```

Backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/x27zhou/compilers/llvm-trunk-install/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/x27zhou/fuzz-pgo/outcome-2026-01-08-08-12-56/bug-exit-14-id-34564/reduction/reduce-profile -fprofile-instrument-use=llvm -fprofile-instrument-use-path=test.profdata -fcoverage-compilation-dir=/home/x27zhou/fuzz-pgo/outcome-2026-01-08-08-12-56/bug-exit-14-id-34564/reduction/reduce-profile -resource-dir /home/x27zhou/compilers/llvm-trunk-install/lib/clang/22 -internal-isystem /home/x27zhou/compilers/llvm-trunk-install/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=308 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/test-8ad661.o -x c test.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "pgo-instr-use" on module "test.c"
 #0 0x0000558662b5c80b backtrace (/home/x27zhou/compilers/llvm-trunk-install/bin/clang-22+0x4e8c80b)
 #1 0x000055866b719ed8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/x27zhou/compilers/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:8
 #2 0x000055866b711df0 llvm::sys::RunSignalHandlers() /home/x27zhou/compilers/llvm-project/llvm/lib/Support/Signals.cpp:0:5
 #3 0x000055866b71d107 SignalHandler(int, siginfo_t*, void*) /home/x27zhou/compilers/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007f3c25ac2520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f3c25b169fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f3c25b169fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f3c25b169fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f3c25ac2476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f3c25aa87f3 abort ./stdlib/abort.c:81:7
#10 0x00007f3c25aa871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f3c25ab9e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#12 0x000055866ff457ec llvm::DenseMapIterator<llvm::BasicBlock const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseSetPair<llvm::BasicBlock const*>, false>::AdvancePastEmptyBuckets() /home/x27zhou/compilers/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1253:50
#13 0x000055866ff457ec llvm::DenseMapIterator<llvm::BasicBlock const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseSetPair<llvm::BasicBlock const*>, false>::operator++() /home/x27zhou/compilers/llvm-project/llvm/include/llvm/ADT/DenseMap.h:1237:5
#14 0x000055866ff457ec llvm::detail::DenseSetImpl<llvm::BasicBlock const*, llvm::DenseMap<llvm::BasicBlock const*, llvm::detail::DenseSetEmpty, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseSetPair<llvm::BasicBlock const*>>, llvm::DenseMapInfo<llvm::BasicBlock const*, void>>::DenseSetIterator<false>::operator++() /home/x27zhou/compilers/llvm-project/llvm/include/llvm/ADT/DenseSet.h:134:7
#15 0x000055866ff457ec (anonymous namespace)::PGOUseFunc::populateCoverage() /home/x27zhou/compilers/llvm-project/llvm/lib/Transforms/Instrumentation/PGOInstrumentation.cpp:1533:19
#16 0x000055866ff457ec annotateAllFunctions(llvm::Module&, llvm::StringRef, llvm::StringRef, llvm::vfs::FileSystem&, llvm::function_ref<llvm::TargetLibraryInfo& (llvm::Function&)>, llvm::function_ref<llvm::BranchProbabilityInfo* (llvm::Function&)>, llvm::function_ref<llvm::BlockFrequencyInfo* (llvm::Function&)>, llvm::function_ref<llvm::LoopInfo* (llvm::Function&)>, llvm::ProfileSummaryInfo*, bool) /home/x27zhou/compilers/llvm-project/llvm/lib/Transforms/Instrumentation/PGOInstrumentation.cpp:2248:12
#17 0x000055866ff300f9 llvm::PGOInstrumentationUse::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compilers/llvm-project/llvm/lib/Transforms/Instrumentation/PGOInstrumentation.cpp:2390:8
#18 0x000055866e650212 llvm::detail::PassModel<llvm::Module, llvm::PGOInstrumentationUse, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compilers/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#19 0x000055866a426b68 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compilers/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#20 0x000055866bd82173 llvm::SmallPtrSetImplBase::isSmall() const /home/x27zhou/compilers/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:248:33
#21 0x000055866bd82173 llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase() /home/x27zhou/compilers/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:89:10
#22 0x000055866bd82173 llvm::PreservedAnalyses::~PreservedAnalyses() /home/x27zhou/compilers/llvm-project/llvm/include/llvm/IR/Analysis.h:112:7
#23 0x000055866bd82173 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1209:5
#24 0x000055866bd64b44 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:0:0
#25 0x000055866bd64b44 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:3
#26 0x000055866bd64b44 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1473:13
#27 0x000055866ce23639 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#28 0x00005586719f9a76 clang::ParseAST(clang::Sema&, bool, bool) /home/x27zhou/compilers/llvm-project/clang/lib/Parse/ParseAST.cpp:0:13
#29 0x000055866ce338a1 clang::CodeGenAction::ExecuteAction() /home/x27zhou/compilers/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:0:30
#30 0x000055866d6f6055 clang::FrontendAction::Execute() /home/x27zhou/compilers/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:10
#31 0x000055866d4e1c26 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/x27zhou/compilers/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:0:33
#32 0x000055866da3c5b8 std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::get() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1666:16
#33 0x000055866da3c5b8 std::__shared_ptr_access<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1363:66
#34 0x000055866da3c5b8 std::__shared_ptr_access<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2, false, false>::operator->() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1357:9
#35 0x000055866da3c5b8 clang::CompilerInstance::getFrontendOpts() /home/x27zhou/compilers/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:12
#36 0x000055866da3c5b8 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/x27zhou/compilers/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:14
#37 0x0000558662bf2a26 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/x27zhou/compilers/llvm-project/clang/tools/driver/cc1_main.cpp:0:15
#38 0x0000558662be8142 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/x27zhou/compilers/llvm-project/clang/tools/driver/driver.cpp:226:12
#39 0x0000558662be4e48 clang_main(int, char**, llvm::ToolContext const&) /home/x27zhou/compilers/llvm-project/clang/tools/driver/driver.cpp:0:12
#40 0x0000558662c18740 main /home/x27zhou/compilers/llvm-trunk-build/tools/clang/tools/driver/clang-driver.cpp:17:10
#41 0x00007f3c25aa9d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#42 0x00007f3c25aa9e40 call_init ./csu/../csu/libc-start.c:128:20
#43 0x00007f3c25aa9e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#44 0x0000558662b20cd5 _start (/home/x27zhou/compilers/llvm-trunk-install/bin/clang-22+0x4e50cd5)
```


