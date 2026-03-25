# [opt] -passes=unify-loop-exits causes UNREACHABLE

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166485
**Status:** Open
**Labels:** loopoptim, crash

## Body

Reproducer:
https://godbolt.org/z/E4qae1z8h

Backtrace:
```
unsupported block terminator
UNREACHABLE executed at /root/llvm-project/llvm/lib/Transforms/Utils/UnifyLoopExits.cpp:206!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=unify-loop-exits <source>
1.	Running pass "function(unify-loop-exits)" on module "<source>"
2.	Running pass "unify-loop-exits" on function "BZ2_bzCompress"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f3d44a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f3d44a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f3d44a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f3d44a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000058b727a (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58b727a)
 #7 0x0000000004c94533 unifyLoopExits(llvm::DominatorTree&, llvm::LoopInfo&, llvm::Loop*) UnifyLoopExits.cpp:0:0
 #8 0x0000000004c96cc2 runImpl(llvm::LoopInfo&, llvm::DominatorTree&) UnifyLoopExits.cpp:0:0
 #9 0x0000000004c971f9 llvm::UnifyLoopExitsPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4c971f9)
#10 0x0000000000efdd4e llvm::detail::PassModel<llvm::Function, llvm::UnifyLoopExitsPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdd4e)
#11 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#12 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#13 0x0000000005761d8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761d8a)
#14 0x000000000097972e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97972e)
#15 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#16 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#17 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#18 0x00007f3d44a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#19 0x00007f3d44a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#20 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - amehsan

the code assumes terminator of each exiting block is a branch. however the failing test case has an exiting block with a switch instruction as terminator.

I just took a quick look, not planning to work on it. 

---

