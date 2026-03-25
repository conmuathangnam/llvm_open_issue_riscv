# opt -passes=pre-isel-intrinsic-lowering crashes

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/156851
**Status:** Closed
**Labels:** llvm:optimizations, crash
**Closed Date:** 2025-10-26T07:08:08Z

## Body

Run opt with -passes=pre-isel-intrinsic-lowering

Reproducer:
https://godbolt.org/z/GPn5ne4cW

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/opt -o /app/output.s -S -passes=pre-isel-intrinsic-lowering <source>
1.	Running pass "pre-isel-intrinsic-lowering" on module "<source>"
 #0 0x00000000052546c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/opt+0x52546c8)
 #1 0x0000000005251894 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007b017ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000003cbd9bd (anonymous namespace)::PreISelIntrinsicLowering::lowerIntrinsics(llvm::Module&) const (.constprop.0) PreISelIntrinsicLowering.cpp:0:0
 #4 0x0000000003cbdfd1 llvm::PreISelIntrinsicLoweringPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x3cbdfd1)
 #5 0x0000000000e5c5ae llvm::detail::PassModel<llvm::Module, llvm::PreISelIntrinsicLoweringPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0xe5c5ae)
 #6 0x000000000503add7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/opt+0x503add7)
 #7 0x000000000098959a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/opt+0x98959a)
 #8 0x000000000097dc53 optMain (/opt/compiler-explorer/clang-trunk/bin/opt+0x97dc53)
 #9 0x00007b017ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#10 0x00007b017ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#11 0x0000000000975c65 _start (/opt/compiler-explorer/clang-trunk/bin/opt+0x975c65)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

It seems that this issue has been fixed. I will close this issue.

---

