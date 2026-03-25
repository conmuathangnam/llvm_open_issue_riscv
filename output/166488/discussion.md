# [opt] -passes=print-mustexecute causes Assertion `Idx >= 0 && "Invalid basic block argument!"'

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166488
**Status:** Open
**Labels:** loopoptim, crash

## Body

Reproducer:
https://godbolt.org/z/44djsY1dW

Backtrace:
```
opt: /root/llvm-project/llvm/include/llvm/IR/Instructions.h:2817: llvm::Value* llvm::PHINode::getIncomingValueForBlock(const llvm::BasicBlock*) const: Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=print-mustexecute <source>
1.	Running pass "function(print-mustexecute)" on module "<source>"
2.	Running pass "print-mustexecute" on function "d"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007a67ee642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007a67ee6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007a67ee642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007a67ee6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007a67ee62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007a67ee639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004f50509 llvm::LoopSafetyInfo::allLoopPathsLeadToBlock(llvm::Loop const*, llvm::BasicBlock const*, llvm::DominatorTree const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f50509)
 #9 0x0000000004f543df llvm::MustExecutePrinterPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f543df)
#10 0x0000000002fab31e llvm::detail::PassModel<llvm::Function, llvm::MustExecutePrinterPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fab31e)
#11 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#12 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#13 0x0000000005761d8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761d8a)
#14 0x000000000097972e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97972e)
#15 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#16 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#17 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#18 0x00007a67ee629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#19 0x00007a67ee629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#20 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - ParkHanbum

This issue also causes an assert because Loop's PreHeader is not exist.  #166725 https://github.com/llvm/llvm-project/issues/165377

---

