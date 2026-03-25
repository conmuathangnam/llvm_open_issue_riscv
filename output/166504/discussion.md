# opt -passes=print<scalar-evolution> crashes: Assertion `!Preds.empty() && "Different predicated BTC, but no predicates"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166504
**Status:** Open
**Labels:** llvm:SCEV, crash

## Body

Reproducer:
https://godbolt.org/z/3da5E81G7

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:13929: void PrintLoopInfo(llvm::raw_ostream&, llvm::ScalarEvolution*, const llvm::Loop*): Assertion `!Preds.empty() && "Different predicated BTC, but no predicates"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=print<scalar-evolution> <source>
1.	Running pass "function(print<scalar-evolution>)" on module "<source>"
2.	Running pass "print<scalar-evolution>" on function "test"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072849f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072849f2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072849f242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072849f2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072849f22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072849f239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004ff2d52 PrintLoopInfo(llvm::raw_ostream&, llvm::ScalarEvolution*, llvm::Loop const*) ScalarEvolution.cpp:0:0
 #9 0x0000000004ff360a llvm::ScalarEvolution::print(llvm::raw_ostream&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ff360a)
#10 0x0000000004ff4382 llvm::ScalarEvolutionPrinterPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ff4382)
#11 0x0000000002fab05e llvm::detail::PassModel<llvm::Function, llvm::ScalarEvolutionPrinterPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fab05e)
#12 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#13 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#14 0x0000000005761d8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761d8a)
#15 0x000000000097972e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97972e)
#16 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#17 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#18 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#19 0x000072849f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x000072849f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Comes from llvm/test/Transforms/LoopStrengthReduce/scev-insertpt-bug.ll

---

### Comment 2 - k-arrows

The reproducer in this issue uses `-passes=print<scalar-evolution>`, and https://github.com/llvm/llvm-project/issues/157235 uses `-passes='loop(loop-rotate),print<scalar-evolution>'`. I'm not sure, but they may be slightly different but essentially the same.

---

