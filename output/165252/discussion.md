# unify-loop-exits pass: Assertion `!empty() && "Cannot call front() on empty SetVector!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165252
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2026-01-26T00:31:30Z

## Body

Reproducer:
https://godbolt.org/z/GET14Pbb8

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/ADT/SetVector.h:132: const value_type& llvm::SetVector<T, Vector, Set, N>::front() const [with T = llvm::BasicBlock*; Vector = llvm::SmallVector<llvm::BasicBlock*, 0>; Set = llvm::DenseSet<llvm::BasicBlock*, llvm::DenseMapInfo<llvm::BasicBlock*, void> >; unsigned int N = 0; llvm::SetVector<T, Vector, Set, N>::value_type = llvm::BasicBlock*]: Assertion `!empty() && "Cannot call front() on empty SetVector!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=unify-loop-exits <source>
1.	Running pass "function(unify-loop-exits)" on module "<source>"
2.	Running pass "unify-loop-exits" on function "test"
 #0 0x0000000005975828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5975828)
 #1 0x00000000059726d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007d1723c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007d1723c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007d1723c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007d1723c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007d1723c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007d1723c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004c99174 llvm::ControlFlowHub::finalize(llvm::DomTreeUpdater*, llvm::SmallVectorImpl<llvm::BasicBlock*>&, llvm::StringRef, std::optional<unsigned int>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4c99174)
 #9 0x0000000004c81693 unifyLoopExits(llvm::DominatorTree&, llvm::LoopInfo&, llvm::Loop*) UnifyLoopExits.cpp:0:0
#10 0x0000000004c83a12 runImpl(llvm::LoopInfo&, llvm::DominatorTree&) UnifyLoopExits.cpp:0:0
#11 0x0000000004c83f79 llvm::UnifyLoopExitsPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4c83f79)
#12 0x0000000000efc39e llvm::detail::PassModel<llvm::Function, llvm::UnifyLoopExitsPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc39e)
#13 0x000000000574f081 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574f081)
#14 0x0000000000efc57e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc57e)
#15 0x000000000574d6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d6fa)
#16 0x000000000097bade llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bade)
#17 0x000000000574d0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d0b1)
#18 0x0000000000985c7a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c7a)
#19 0x0000000000979ef1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979ef1)
#20 0x00007d1723c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007d1723c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x0000000000971305 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971305)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

