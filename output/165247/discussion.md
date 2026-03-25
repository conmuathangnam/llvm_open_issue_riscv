# StructurizeCFGPass: Assertion `hasOnlySimpleTerminator(*Func) && "Unsupported block terminator."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165247
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2025-11-15T12:44:58Z

## Body

Reproducer:
https://godbolt.org/z/fxYTh5546

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/Transforms/Scalar/StructurizeCFG.cpp:1411: bool {anonymous}::StructurizeCFG::run(llvm::Region*, llvm::DominatorTree*, const llvm::TargetTransformInfo*): Assertion `hasOnlySimpleTerminator(*Func) && "Unsupported block terminator."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=structurizecfg <source>
1.	Running pass "function(StructurizeCFGPass)" on module "<source>"
2.	Running pass "StructurizeCFGPass" on function "foo"
 #0 0x0000000005975828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5975828)
 #1 0x00000000059726d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007d432c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007d432c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007d432c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007d432c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007d432c82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007d432c839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000486acae llvm::StructurizeCFGPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x486acae)
 #9 0x0000000000efc37e llvm::detail::PassModel<llvm::Function, llvm::StructurizeCFGPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc37e)
#10 0x000000000574f081 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574f081)
#11 0x0000000000efc57e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc57e)
#12 0x000000000574d6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d6fa)
#13 0x000000000097bade llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bade)
#14 0x000000000574d0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d0b1)
#15 0x0000000000985c7a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c7a)
#16 0x0000000000979ef1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979ef1)
#17 0x00007d432c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x00007d432c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x0000000000971305 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971305)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

It seems that this has been fixed. I will close this issue.

---

