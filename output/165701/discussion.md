# [Attributor] Assertion `llvm::isPowerOf2_64(Value) && "Alignment is not a power of 2"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165701
**Status:** Open
**Labels:** crash, llvm::Attributor

## Body

Reproducer:
https://godbolt.org/z/TddWPEWzG

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/Support/Alignment.h:70: llvm::Align::Align(uint64_t): Assertion `llvm::isPowerOf2_64(Value) && "Alignment is not a power of 2"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=attributor <source>
1.	Running pass "attributor" on module "<source>"
 #0 0x000000000598f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x598f068)
 #1 0x000000000598bf14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007c5201242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007c52012969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007c5201242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007c52012287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007c520122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007c5201239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000c5f3af llvm::Align::Align(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xc5f3af)
 #9 0x00000000031b6148 void (anonymous namespace)::followUsesInContext<(anonymous namespace)::AAAlignImpl, llvm::IncIntegerState<unsigned long, 4294967296ul, 1ul>>((anonymous namespace)::AAAlignImpl&, llvm::Attributor&, llvm::MustBeExecutedContextExplorer&, llvm::Instruction const*, llvm::SetVector<llvm::Use const*, llvm::SmallVector<llvm::Use const*, 0u>, llvm::DenseSet<llvm::Use const*, llvm::DenseMapInfo<llvm::Use const*, void>>, 0u>&, llvm::IncIntegerState<unsigned long, 4294967296ul, 1ul>&) AttributorAttributes.cpp:0:0
#10 0x00000000031b6a88 (anonymous namespace)::AAAlignImpl::initialize(llvm::Attributor&) AttributorAttributes.cpp:0:0
#11 0x0000000003137888 llvm::AAAlign const* llvm::Attributor::getOrCreateAAFor<llvm::AAAlign>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3137888)
#12 0x00000000031389ff llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (.part.0) Attributor.cpp:0:0
#13 0x0000000003139711 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3139711)
#14 0x0000000003142d9b runAttributorOnFunctions(llvm::InformationCache&, llvm::SetVector<llvm::Function*, llvm::SmallVector<llvm::Function*, 0u>, llvm::DenseSet<llvm::Function*, llvm::DenseMapInfo<llvm::Function*, void>>, 0u>&, llvm::AnalysisGetter&, llvm::CallGraphUpdater&, bool, bool) (.part.0) Attributor.cpp:0:0
#15 0x000000000314395f llvm::AttributorPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x314395f)
#16 0x0000000002fb687e llvm::detail::PassModel<llvm::Module, llvm::AttributorPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb687e)
#17 0x00000000057656c1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57656c1)
#18 0x0000000000985b5a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985b5a)
#19 0x0000000000979dd1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979dd1)
#20 0x00007c5201229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007c5201229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000009711e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9711e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

