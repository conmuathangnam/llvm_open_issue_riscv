# [opt] [codegenprepare] Assertion Error on loop-interchange after codegenprepare.

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166311
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2026-03-24T14:23:44Z

## Body

Seems that `codegenprepare` does not invalidate `LoopAnalysis` properly and causes the following passes that use it to crash.

Reproducer:
https://godbolt.org/z/oosa5bPo7

Backtrace:
```
opt: /root/llvm-project/llvm/lib/Transforms/Utils/LoopSimplify.cpp:358: llvm::BasicBlock* insertUniqueBackedgeBlock(llvm::Loop*, llvm::BasicBlock*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*): Assertion `L->getNumBackEdges() > 1 && "Must have > 1 backedge!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=module(scc-oz-module-inliner),module(cgscc(function(loop(loop-rotate<prepare-for-lto>)))),module(cgscc(function(codegenprepare))),module(cgscc(function(loop(loop-interchange)))) <source>
1.	Running pass "cgscc(function(loop(loop-interchange)))" on module "<source>"
2.	Running pass "loop(loop-interchange)" on function "c"
3.	Running pass "loop-simplify" on function "c"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007976dfe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007976dfe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007976dfe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007976dfe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007976dfe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007976dfe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004b5c0fc insertUniqueBackedgeBlock(llvm::Loop*, llvm::BasicBlock*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*) LoopSimplify.cpp:0:0
 #9 0x0000000004b5cd07 simplifyOneLoop(llvm::Loop*, llvm::SmallVectorImpl<llvm::Loop*>&, llvm::DominatorTree*, llvm::LoopInfo*, llvm::ScalarEvolution*, llvm::AssumptionCache*, llvm::MemorySSAUpdater*, bool) LoopSimplify.cpp:0:0
#10 0x0000000004b5e43d llvm::simplifyLoop(llvm::Loop*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::ScalarEvolution*, llvm::AssumptionCache*, llvm::MemorySSAUpdater*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4b5e43d)
#11 0x0000000004b5e940 llvm::LoopSimplifyPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4b5e940)
#12 0x0000000000efe18e llvm::detail::PassModel<llvm::Function, llvm::LoopSimplifyPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefe18e)
#13 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#14 0x00000000046e72e4 llvm::FunctionToLoopPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x46e72e4)
#15 0x0000000000efde0e llvm::detail::PassModel<llvm::Function, llvm::FunctionToLoopPassAdaptor, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefde0e)
#16 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#17 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#18 0x0000000004d21332 llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4d21332)
#19 0x0000000000efcc4e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::CGSCCToFunctionPassAdaptor, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefcc4e)
#20 0x0000000004d195c2 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4d195c2)
#21 0x0000000002faa8ce llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2faa8ce)
#22 0x0000000004d1b001 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4d1b001)
#23 0x0000000000efcc2e llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefcc2e)
#24 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#25 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#26 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#27 0x00007976dfe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#28 0x00007976dfe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#29 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - fraglantia

Fixed by https://github.com/llvm/llvm-project/pull/172418

---

