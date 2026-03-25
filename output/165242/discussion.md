# opt -passes=globalopt crashes with "associated metadata must have a global value"

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165242

## Body

Reproducer:
https://godbolt.org/z/rMfh77qeb

Backtrace:
```console
associated metadata must have a global value
ptr @a
!0 = distinct !{null}
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/opt -o /app/output.s -S -passes=globalopt <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005293c88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5293c88)
 #1 0x0000000005290e54 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fed9ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007fed9ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007fed9ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007fed9ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000000843372 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000051c2b48 (/opt/compiler-explorer/clang-trunk/bin/opt+0x51c2b48)
 #8 0x00000000050ab120 llvm::VerifierPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x50ab120)
 #9 0x000000000098e47e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x98e47e)
#10 0x00000000050675f7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x50675f7)
#11 0x0000000000997e8a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/opt+0x997e8a)
#12 0x000000000098c837 optMain (/opt/compiler-explorer/clang-trunk/bin/opt+0x98c837)
#13 0x00007fed9ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007fed9ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000984985 _start (/opt/compiler-explorer/clang-trunk/bin/opt+0x984985)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to 17.0.1:
https://godbolt.org/z/x1fTr3nT5

---

