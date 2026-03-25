# opt -passes=loop-vectorize -pass-remarks=vector causes Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165377
**Status:** Closed
**Labels:** vectorizers, crash, clang:analysis
**Closed Date:** 2026-02-09T15:02:16Z

## Body

Reproducer:
https://godbolt.org/z/sGzfsKGjd

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/IR/Instructions.h:2817: llvm::Value* llvm::PHINode::getIncomingValueForBlock(const llvm::BasicBlock*) const: Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize -pass-remarks=vector <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "test"
 #0 0x0000000005977d28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5977d28)
 #1 0x0000000005974bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007de382242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007de3822969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007de382242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007de3822287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007de38222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007de382239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004dec2c9 llvm::RecurrenceDescriptor::AddReductionVar(llvm::PHINode*, llvm::RecurKind, llvm::Loop*, llvm::FastMathFlags, llvm::RecurrenceDescriptor&, llvm::DemandedBits*, llvm::AssumptionCache*, llvm::DominatorTree*, llvm::ScalarEvolution*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4dec2c9)
 #9 0x0000000004dec6f6 llvm::RecurrenceDescriptor::isReductionPHI(llvm::PHINode*, llvm::Loop*, llvm::RecurrenceDescriptor&, llvm::DemandedBits*, llvm::AssumptionCache*, llvm::DominatorTree*, llvm::ScalarEvolution*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4dec6f6)
#10 0x00000000038cb52c llvm::LoopVectorizationLegality::canVectorizeInstr(llvm::Instruction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38cb52c)
#11 0x00000000038ccf6e llvm::LoopVectorizationLegality::canVectorizeInstrs() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38ccf6e)
#12 0x00000000038cda77 llvm::LoopVectorizationLegality::canVectorize(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38cda77)
#13 0x00000000036b4ce8 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b4ce8)
#14 0x00000000036b9200 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b9200)
#15 0x00000000036b98e5 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b98e5)
#16 0x0000000002fa1c3e llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fa1c3e)
#17 0x0000000005750361 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5750361)
#18 0x0000000000efcb8e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefcb8e)
#19 0x000000000574e9da llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574e9da)
#20 0x000000000097beee llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97beee)
#21 0x000000000574e391 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574e391)
#22 0x000000000098608a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98608a)
#23 0x000000000097a301 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97a301)
#24 0x00007de382229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007de382229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000971715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971715)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

