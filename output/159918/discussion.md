# [NewGVN] Assertion `NewClass->size() == 1 || (isa<StoreInst>(I) && NewClass->getStoreCount() == 1)' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159918
**Status:** Closed
**Labels:** crash, llvm:GVN
**Closed Date:** 2025-10-03T19:52:31Z

## Body

Reproducer:
https://godbolt.org/z/xMdK51ceK

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/Transforms/Scalar/NewGVN.cpp:2251: void {anonymous}::NewGVN::moveMemoryToNewCongruenceClass(llvm::Instruction*, llvm::MemoryAccess*, {anonymous}::CongruenceClass*, {anonymous}::CongruenceClass*): Assertion `NewClass->size() == 1 || (isa<StoreInst>(I) && NewClass->getStoreCount() == 1)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=newgvn <source>
1.	Running pass "function(newgvn)" on module "<source>"
2.	Running pass "newgvn" on function "test_catch_with_objc_intrinsic"
 #0 0x00000000058c75d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58c75d8)
 #1 0x00000000058c4484 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007accadc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007accadc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007accadc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007accadc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007accadc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007accadc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000047081ce (anonymous namespace)::NewGVN::performCongruenceFinding(llvm::Instruction*, llvm::GVNExpression::Expression const*) NewGVN.cpp:0:0
 #9 0x0000000004716770 (anonymous namespace)::NewGVN::iterateTouchedInstructions() NewGVN.cpp:0:0
#10 0x00000000047189f9 (anonymous namespace)::NewGVN::runGVN() NewGVN.cpp:0:0
#11 0x000000000471aa7c llvm::NewGVNPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x471aa7c)
#12 0x0000000002f48a6e llvm::detail::PassModel<llvm::Function, llvm::NewGVNPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2f48a6e)
#13 0x00000000056ae891 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56ae891)
#14 0x0000000000eea94e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xeea94e)
#15 0x00000000056acdfa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56acdfa)
#16 0x0000000000971bbe llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971bbe)
#17 0x00000000056ac7b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56ac7b1)
#18 0x000000000097bd78 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bd78)
#19 0x000000000096ff39 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ff39)
#20 0x00007accadc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007accadc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x0000000000967095 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x967095)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

The above reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/llvm/test/CodeGen/X86/win64-funclet-preisel-intrinsics.ll

---

