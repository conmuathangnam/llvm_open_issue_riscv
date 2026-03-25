# opt -passes='function(loop-mssa(licm,lnicm))' crashes

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166309
**Status:** Closed
**Labels:** loopoptim, crash
**Closed Date:** 2025-11-30T02:00:06Z

## Body

Reproducer:
https://godbolt.org/z/M4h5x1z8v

Backtrace:
```console
Instruction does not dominate all uses!
  %.lcssa = phi i64 [ %0, %lor.rhs42 ]
  %tmp45 = add i64 undef, %.lcssa
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/opt -o /app/output.s -S -passes=function(loop-mssa(licm,lnicm)) <source>
1.	Running pass "verify" on module "<source>"
 #0 0x00000000052b5968 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/opt+0x52b5968)
 #1 0x00000000052b2b34 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007e9a42042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007e9a420969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007e9a42042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007e9a420287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000084494c llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000051e3b58 (/opt/compiler-explorer/clang-trunk/bin/opt+0x51e3b58)
 #8 0x00000000050cbbc0 llvm::VerifierPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x50cbbc0)
 #9 0x000000000098fe1e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x98fe1e)
#10 0x0000000005088097 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5088097)
#11 0x000000000099982a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/opt+0x99982a)
#12 0x000000000098e1d7 optMain (/opt/compiler-explorer/clang-trunk/bin/opt+0x98e1d7)
#13 0x00007e9a42029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007e9a42029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000986325 _start (/opt/compiler-explorer/clang-trunk/bin/opt+0x986325)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Comes from llvm/test/Transforms/LoopStrengthReduce/2012-03-15-nopreheader.ll

---

### Comment 2 - k-arrows

It seems that this has been fixed. I will close this issue.

---

