# opt -passes=argpromotion crashes on llvm/test/Transforms/DeadArgElim/keepalive.ll

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170032
**Status:** Open
**Labels:** crash

## Body

Here is the reduced reproducer:
https://godbolt.org/z/519cjfexW

Backtrace:
```console
cannot use preallocated intrinsics on a call without preallocated arguments
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/opt -o /app/output.s -S -passes=argpromotion <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005213da8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5213da8)
 #1 0x0000000005210f74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007e9d63842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007e9d638969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007e9d63842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007e9d638287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000000834436 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x000000000514a298 (/opt/compiler-explorer/clang-trunk/bin/opt+0x514a298)
 #8 0x0000000005030960 llvm::VerifierPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5030960)
 #9 0x000000000097fc6e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x97fc6e)
#10 0x0000000004ff3937 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x4ff3937)
#11 0x0000000000989a2a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/opt+0x989a2a)
#12 0x000000000097d700 optMain (/opt/compiler-explorer/clang-trunk/bin/opt+0x97d700)
#13 0x00007e9d63829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007e9d63829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000975c25 _start (/opt/compiler-explorer/clang-trunk/bin/opt+0x975c25)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

