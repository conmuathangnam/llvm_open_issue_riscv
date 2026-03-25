# [SimplifyCFG] LLVM Error due to 'Wrong number of operands'

**Author:** KirillVNaumov
**URL:** https://github.com/llvm/llvm-project/issues/182046
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2026-02-20T06:46:56Z

## Body

Here is a link to the failing test: https://godbolt.org/z/acK6eTsn1.
When it's run with the `-passes=verify`, the test compiles as normal, however when `-passes=simplifycfg` is applied to it, the test crashes with
```
Wrong number of operands
!0 = !{!"branch_weights", i32 1, i32 0, i32 0, i32 0, i32 2}
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/opt -o /app/output.s -S -passes=simplifycfg <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005389b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5389b28)
 #1 0x0000000005386d14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000751047242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007510472969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000751047242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007510472287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000008326d3 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000052bb528 (/opt/compiler-explorer/clang-trunk/bin/opt+0x52bb528)
 #8 0x0000000005137270 llvm::VerifierPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x5137270)
 #9 0x00000000009759de llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x9759de)
#10 0x00000000050fd9d7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x50fd9d7)
#11 0x000000000097f96a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/opt+0x97f96a)
#12 0x0000000000972eff optMain (/opt/compiler-explorer/clang-trunk/bin/opt+0x972eff)
#13 0x0000751047229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x0000751047229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x000000000096b6e5 _start (/opt/compiler-explorer/clang-trunk/bin/opt+0x96b6e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
Git bisect identified https://github.com/llvm/llvm-project/commit/5b2f9b53bdb348393bba221c5f69bfac179092c8 as the guilty commit after which the pass fails. Reverting the commit removes the failure.
CC @kper 

## Comments

### Comment 1 - nikic

Probably same as https://github.com/llvm/llvm-project/issues/181837

---

### Comment 2 - HerrCai0907

fixed in #181837

---

