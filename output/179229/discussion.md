# [MemorySSA] Assert "Incorrect incoming access into phi"

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/179229

## Body

This reproducer:

```c
short a, c;
int b;
void d(bool e[], char f[][8][8]) {
  a = 0;
  for (;;)
    for (int g; g; g = c || e[1])
      for (; 1 < (1 >> c + 6) + 1;)
        b = f[g][1][g];
}
```

Compiled with clang -O3, this triggers the following assert for a build with expensive checks enabled:

```
clang++: /path/to/llvm-project/llvm/lib/Analysis/MemorySSA.cpp:1956: void llvm::MemorySSA::verifyPrevDefInPhis(IterT) const [IterT =
llvm::iterator_range<llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::BasicBlock, true, false, void, false, void>, false, false>>]: 
Assertion `LastAcc == IncAcc && "Incorrect incoming access into phi."' failed.
```

Stack trace:

```
#0 0x0000aaaab00ac17c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/home/smeijer/llvm-project/build_expensive_checks/bin/clang+++0x560c17c)
 #1 0x0000aaaab00a9c2c llvm::sys::RunSignalHandlers() (/local/home/smeijer/llvm-project/build_expensive_checks/bin/clang+++0x5609c2c)
 #2 0x0000aaaaafff73dc CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000fffff7fb09d0 (linux-vdso.so.1+0x9d0)
 #4 0x0000fffff7a1f200 __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #5 0x0000fffff79da67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x0000fffff79c7130 abort ./stdlib/./stdlib/abort.c:81:7
 #7 0x0000fffff79d3fd0 __assert_fail_base ./assert/./assert/assert.c:89:7
 #8 0x0000fffff79d4040 __assert_perror_fail ./assert/./assert/assert-perr.c:31:1
 #9 0x0000aaaaaf12e978 void llvm::MemorySSA::verifyPrevDefInPhis<llvm::iterator_range<llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::BasicBlock, true, false, void, false, void>, false, false> > >(llvm::iterator_range<llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::BasicBlock, true, false, void, false, void>, fals
e, false> >) const (/local/home/smeijer/llvm-project/build_expensive_checks/bin/clang+++0x468e978)
#10 0x0000aaaab14c8fb8 unswitchNontrivialInvariants(llvm::Loop&, llvm::Instruction&, llvm::ArrayRef<llvm::Value*>, llvm::IVConditionInfo&, llvm::DominatorTree&, llvm::LoopInfo&, llvm::AssumptionCache&, llvm::ScalarEvolution*, llvm::MemorySSAUpdater*, llvm::LPMUpdater&, bool, bool) SimpleLoopUnswitch.cpp:0:0
#11 0x0000aaaab14c19d8 unswitchBestCondition(llvm::Loop&, llvm::DominatorTree&, llvm::LoopInfo&, llvm::AssumptionCache&, llvm::AAResults&, llvm::TargetTransformInfo&, llvm::ScalarEvolution*, llvm::MemorySSAUpdater*, llvm::LPMUpdater&) SimpleLoopUnswitch.cpp:0:0
#12 0x0000aaaab14b9834 llvm::SimpleLoopUnswitchPass::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/local/home/smeijer/llvm-project/build_expensive_checks/bin/clang+++0x6a19834)
#13 0x0000aaaaafec8780 std::optional<llvm::PreservedAnalyses> llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::runSinglePass<llvm::Loop, std::unique_ptr<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::
LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&> > > >(llvm::Loop&, std::unique_ptr<llvm::detail::PassConcept<llvm::L
oop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&> > >&, llvm::AnalysisMan
ager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&, llvm::PassInstrumentation&) (/local/home/smeijer/llvm-project/build_expensive_checks/bin/clang+++0x5428780)
```

I have been trying to get an IR and `opt` reproducer, but haven't managed to do so. I don't know yet why my IR reproducer isn't triggering this. I will try again, but need to switch tasks now, so am leaving this here. 

