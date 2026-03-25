# [GVN] Assertion `(!isScalable() || isZero()) && "Request for a fixed element count on a scalable object"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159368
**Status:** Closed
**Labels:** crash, llvm:GVN
**Closed Date:** 2026-03-04T05:24:17Z

## Body

Reproducer:
https://godbolt.org/z/v41osa8TY

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/Support/TypeSize.h:201: constexpr llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::ScalarTy llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::getFixedValue() const [with LeafTy = llvm::TypeSize; ValueTy = long unsigned int; llvm::details::FixedOrScalableQuantity<LeafTy, ValueTy>::ScalarTy = long unsigned int]: Assertion `(!isScalable() || isZero()) && "Request for a fixed element count on a scalable object"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -aa-pipeline=globals-aa -passes=gvn <source>
1.	Running pass "function(gvn<>)" on module "<source>"
2.	Running pass "gvn<>" on function "test_alloca_store_reload"
 #0 0x00000000058b6c38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58b6c38)
 #1 0x00000000058b3ae4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007d4315442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007d43154969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007d4315442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007d43154287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007d431542871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007d4315439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004bf6fb4 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4bf6fb4)
 #9 0x000000000455c739 llvm::GVNPass::AnalyzeLoadAvailability(llvm::LoadInst*, llvm::MemDepResult, llvm::Value*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x455c739)
#10 0x00000000045690f7 llvm::GVNPass::processLoad(llvm::LoadInst*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x45690f7)
#11 0x000000000456b7b0 llvm::GVNPass::processInstruction(llvm::Instruction*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x456b7b0)
#12 0x000000000456c14b llvm::GVNPass::processBlock(llvm::BasicBlock*) (.part.0) GVN.cpp:0:0
#13 0x000000000456c9e1 llvm::GVNPass::runImpl(llvm::Function&, llvm::AssumptionCache&, llvm::DominatorTree&, llvm::TargetLibraryInfo const&, llvm::AAResults&, llvm::MemoryDependenceResults*, llvm::LoopInfo&, llvm::OptimizationRemarkEmitter*, llvm::MemorySSA*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x456c9e1)
#14 0x000000000456dda0 llvm::GVNPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x456dda0)
#15 0x0000000000eeab9e llvm::detail::PassModel<llvm::Function, llvm::GVNPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xeeab9e)
#16 0x000000000569e1f1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x569e1f1)
#17 0x0000000000eeadbe llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xeeadbe)
#18 0x000000000569c75a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x569c75a)
#19 0x000000000096fb6e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96fb6e)
#20 0x000000000569c111 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x569c111)
#21 0x0000000000979d28 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979d28)
#22 0x000000000096dee9 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96dee9)
#23 0x00007d4315429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x00007d4315429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x0000000000965035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x965035)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

