# crashes with assertion failure when running opt -passes=separate-const-offset-from-gep

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/156449

## Body

Run opt with -passes=separate-const-offset-from-gep

Reproducer:
https://godbolt.org/z/dW6h989xW

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:1565: int64_t llvm::APInt::getSExtValue() const: Assertion `getSignificantBits() <= 64 && "Too many bits for int64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=separate-const-offset-from-gep <source>
1.	Running pass "function(separate-const-offset-from-gep<>)" on module "<source>"
2.	Running pass "separate-const-offset-from-gep<>" on function "test1"
 #0 0x000000000570dcf8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x570dcf8)
 #1 0x000000000570aba4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000074123fc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000074123fc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000074123fc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000074123fc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000074123fc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000074123fc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004617144 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4617144)
 #9 0x0000000004618b0c (anonymous namespace)::SeparateConstOffsetFromGEP::splitGEP(llvm::GetElementPtrInst*) SeparateConstOffsetFromGEP.cpp:0:0
#10 0x000000000461b41f (anonymous namespace)::SeparateConstOffsetFromGEP::run(llvm::Function&) (.part.0) SeparateConstOffsetFromGEP.cpp:0:0
#11 0x000000000461d845 llvm::SeparateConstOffsetFromGEPPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x461d845)
#12 0x0000000000eda48e llvm::detail::PassModel<llvm::Function, llvm::SeparateConstOffsetFromGEPPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xeda48e)
#13 0x00000000054fc221 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x54fc221)
#14 0x0000000000eda70e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xeda70e)
#15 0x00000000054faaca llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x54faaca)
#16 0x000000000096bdee llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bdee)
#17 0x00000000054fa481 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x54fa481)
#18 0x00000000009760e8 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9760e8)
#19 0x000000000096a157 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96a157)
#20 0x000074123fc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x000074123fc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x0000000000961275 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x961275)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

