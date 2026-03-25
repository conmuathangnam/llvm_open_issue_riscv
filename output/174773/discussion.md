# Assertion `I->use_empty() && "trying to erase instruction with users."' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/174773
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-01-08T15:23:16Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer -slp-threshold=-99999)

[reduced-stripped.ll.zip](https://github.com/user-attachments/files/24473473/reduced-stripped.ll.zip)

Reproducer: https://godbolt.org/z/c8rcjzPdr

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "function(slp-vectorizer)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "wombat"
 #0 0x0000000005a7f2e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a7f2e8)
 #1 0x0000000005a7c194 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072441c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072441c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072441c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072441c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072441c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072441c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000379becf llvm::slpvectorizer::BoUpSLP::~BoUpSLP() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x379becf)
 #9 0x0000000003896946 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#10 0x00000000038973cb llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38973cb)
#11 0x000000000304364e llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x304364e)
#12 0x0000000005859c61 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5859c61)
#13 0x0000000000f1ba3e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1ba3e)
#14 0x000000000585821a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585821a)
#15 0x000000000097ee5e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97ee5e)
#16 0x0000000005857bd1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5857bd1)
#17 0x0000000000989110 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x989110)
#18 0x000000000097d038 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d038)
#19 0x000072441c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x000072441c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x0000000000973a35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x973a35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - EugeneZelenko

@TatyanaDoubts: If you plan to file more issues in future, it'll be good idea to request triage role ([examples](https://github.com/llvm/llvm-project/issues/?q=is%3Aissue%20label%3Ainfra%3Acommit-access-request%20Triage)), so you'll be able to label issues yourself, preferably at creation.


---

