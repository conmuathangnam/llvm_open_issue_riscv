# [Attributor] Assertion `F && "no associated function for return from call"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165708
**Status:** Open
**Labels:** crash, llvm::Attributor

## Body

Reproducer:
https://godbolt.org/z/shM85Y6hb

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/Transforms/IPO/AttributorAttributes.cpp:12850: virtual void {anonymous}::AAInvariantLoadPointerCallSiteReturned::initialize(llvm::Attributor&): Assertion `F && "no associated function for return from call"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=attributor <source>
1.	Running pass "attributor" on module "<source>"
 #0 0x000000000598f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x598f068)
 #1 0x000000000598bf14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007c1b32e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007c1b32e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007c1b32e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007c1b32e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007c1b32e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007c1b32e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000315bc78 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x315bc78)
 #9 0x000000000313a9e8 llvm::AAInvariantLoadPointer const* llvm::Attributor::getOrCreateAAFor<llvm::AAInvariantLoadPointer>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x313a9e8)
#10 0x000000000313b0ac bool llvm::function_ref<bool (llvm::Instruction&)>::callback_fn<llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&)::'lambda0'(llvm::Instruction&)>(long, llvm::Instruction&) Attributor.cpp:0:0
#11 0x0000000003123399 checkForAllInstructionsImpl(llvm::Attributor*, llvm::DenseMap<unsigned int, llvm::SmallVector<llvm::Instruction*, 8u>*, llvm::DenseMapInfo<unsigned int, void>, llvm::detail::DenseMapPair<unsigned int, llvm::SmallVector<llvm::Instruction*, 8u>*>>&, llvm::function_ref<bool (llvm::Instruction&)>, llvm::AbstractAttribute const*, llvm::AAIsDead const*, llvm::ArrayRef<unsigned int>, bool&, bool, bool) (.constprop.0) Attributor.cpp:0:0
#12 0x0000000003138940 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (.part.0) Attributor.cpp:0:0
#13 0x0000000003139711 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3139711)
#14 0x0000000003142d9b runAttributorOnFunctions(llvm::InformationCache&, llvm::SetVector<llvm::Function*, llvm::SmallVector<llvm::Function*, 0u>, llvm::DenseSet<llvm::Function*, llvm::DenseMapInfo<llvm::Function*, void>>, 0u>&, llvm::AnalysisGetter&, llvm::CallGraphUpdater&, bool, bool) (.part.0) Attributor.cpp:0:0
#15 0x000000000314395f llvm::AttributorPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x314395f)
#16 0x0000000002fb687e llvm::detail::PassModel<llvm::Module, llvm::AttributorPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb687e)
#17 0x00000000057656c1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57656c1)
#18 0x0000000000985b5a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985b5a)
#19 0x0000000000979dd1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979dd1)
#20 0x00007c1b32e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007c1b32e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000009711e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9711e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

