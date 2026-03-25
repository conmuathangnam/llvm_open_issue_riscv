# [opt] [ctx-instr-gen] Assertion Error On Anonymous Function

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166304
**Status:** Open
**Labels:** PGO, crash

## Body

`ctx-instr-gen` crashes when its target is an anonymous function (i.e., stripped).

Reproducer:
https://godbolt.org/z/YM6ncz48h

Backtrace:
```
opt: /root/llvm-project/llvm/include/llvm/ADT/StringRef.h:236: char llvm::StringRef::operator[](size_t) const: Assertion `Index < size() && "Invalid index!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=ctx-instr-gen <source>
1.	Running pass "ctx-instr-gen" on module "<source>"
 #0 0x00000000059945f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59945f8)
 #1 0x00000000059914a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f478c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f478c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f478c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f478c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007f478c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007f478c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000050b03ce llvm::getPGOFuncName[abi:cxx11](llvm::StringRef, llvm::GlobalValue::LinkageTypes, llvm::StringRef, unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50b03ce)
 #9 0x00000000050b0bd3 llvm::getPGOFuncName[abi:cxx11](llvm::Function const&, bool, unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50b0bd3)
#10 0x00000000035fe2fe (anonymous namespace)::FuncPGOInstrumentation<(anonymous namespace)::PGOEdge, (anonymous namespace)::PGOBBInfo>::FuncPGOInstrumentation(llvm::Function&, llvm::TargetLibraryInfo&, std::unordered_multimap<llvm::Comdat*, llvm::GlobalValue*, std::hash<llvm::Comdat*>, std::equal_to<llvm::Comdat*>, std::allocator<std::pair<llvm::Comdat* const, llvm::GlobalValue*>>>&, bool, llvm::BranchProbabilityInfo*, llvm::BlockFrequencyInfo*, llvm::LoopInfo*, bool, bool, bool, bool) PGOInstrumentation.cpp:0:0
#11 0x000000000360043e (anonymous namespace)::FunctionInstrumenter::instrument() PGOInstrumentation.cpp:0:0
#12 0x00000000036092d7 llvm::PGOInstrumentationGen::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36092d7)
#13 0x0000000002fb485e llvm::detail::PassModel<llvm::Module, llvm::PGOInstrumentationGen, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb485e)
#14 0x000000000576acc1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x576acc1)
#15 0x0000000000986d8a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x986d8a)
#16 0x000000000097b001 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b001)
#17 0x00007f478c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x00007f478c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x0000000000972415 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972415)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

