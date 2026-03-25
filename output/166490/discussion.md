# [opt] -passes=loop-bound-split causes Assertion `Idx >= 0 && "Invalid basic block argument!"'

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166490
**Status:** Closed
**Labels:** duplicate, loopoptim, crash
**Closed Date:** 2025-11-05T02:15:23Z

## Body

Reproducer:
https://godbolt.org/z/TTzhbbboW

Backtrace:
```
opt: /root/llvm-project/llvm/include/llvm/IR/Instructions.h:2817: llvm::Value* llvm::PHINode::getIncomingValueForBlock(const llvm::BasicBlock*) const: Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=loop-bound-split <source>
1.	Running pass "function(loop(loop-bound-split))" on module "<source>"
2.	Running pass "loop(loop-bound-split)" on function "BZ2_hbAssignCodes"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000073f762242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000073f7622969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000073f762242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000073f7622287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000073f76222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000073f762239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000467c6fa findSplitCandidate(llvm::Loop const&, llvm::ScalarEvolution&, (anonymous namespace)::ConditionInfo&, (anonymous namespace)::ConditionInfo&) (.isra.0) LoopBoundSplit.cpp:0:0
 #9 0x000000000467d65b splitLoopBound(llvm::Loop&, llvm::DominatorTree&, llvm::LoopInfo&, llvm::ScalarEvolution&, llvm::LPMUpdater&) LoopBoundSplit.cpp:0:0
#10 0x000000000468008c llvm::LoopBoundSplitPass::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x468008c)
#11 0x0000000002fabe9e llvm::detail::PassModel<llvm::Loop, llvm::LoopBoundSplitPass, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fabe9e)
#12 0x00000000046e5626 std::optional<llvm::PreservedAnalyses> llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::runSinglePass<llvm::Loop, std::unique_ptr<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>>>>(llvm::Loop&, std::unique_ptr<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>>>&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&, llvm::PassInstrumentation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x46e5626)
#13 0x00000000046e580f llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::runWithoutLoopNestPasses(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x46e580f)
#14 0x00000000046e6f74 llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x46e6f74)
#15 0x00000000046e7f4f llvm::FunctionToLoopPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x46e7f4f)
#16 0x0000000000efde0e llvm::detail::PassModel<llvm::Function, llvm::FunctionToLoopPassAdaptor, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefde0e)
#17 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#18 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#19 0x0000000005761d8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761d8a)
#20 0x000000000097972e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97972e)
#21 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#22 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#23 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#24 0x000073f762229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000073f762229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Looks similar to the backtrace in the issue below:
https://github.com/llvm/llvm-project/issues/156793

---

### Comment 2 - fraglantia

Duplicate

---

