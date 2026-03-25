# Assertion `!CI->hasMetadata(LLVMContext::MD_memprof)' failed

**Author:** alidavanian
**URL:** https://github.com/llvm/llvm-project/issues/181237

## Body

Link to CE: https://godbolt.org/z/6ea6e5Ghz

The bug is a regression from 21.1.0

Summary: `opt -O3` crashes with an assertion failure in `MemProfRemoveInfo::run()` when a call instruction has `!memprof` metadata but is missing the required accompanying `!callsite` metadata.
The IR verifier should reject this invalid IR instead of crashing.

Reproducer:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define ptr @library_function() {
entry:
  %call = call ptr null(i64 0), !memprof !0
  ret ptr %call
}

!0 = !{!1, !3}
!1 = !{!2, !"hot"}
!2 = !{i64 444444444, i64 60, i64 0, i64 0}
!3 = !{!4, !"cold"}
!4 = !{i64 555555555, i64 40, i64 0, i64 0}
```

Error:
```
>$ opt -O3 reduced.ll -o /dev/null
opt: /workspace/oss-llvm/llvm_src/llvm/lib/Transforms/IPO/MemProfContextDisambiguation.cpp:6459: PreservedAnalyses llvm::MemProfRemoveInfo::run(Module &, ModuleAnalysisManager &): Assertion `!CI->hasMetadata(LLVMContext::MD_memprof)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: opt -O3 reduced.ll -o /dev/null
1.      Running pass "memprof-remove-attributes" on module "reduced.ll"
 #0 0x000077bfd94c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x000077bfd94c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x000077bfd94c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x000077bfd94ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x000077bfd8bb5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000077bfd8c0eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000077bfd8c0eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000077bfd8c0eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000077bfd8bb527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000077bfd8b988ff abort ./stdlib/abort.c:81:7
#10 0x000077bfd8b9881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000077bfd8bab517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000077bfe00caed9 llvm::MemProfRemoveInfo::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /workspace/oss-llvm/llvm_src/llvm/lib/Transforms/IPO/MemProfContextDisambiguation.cpp:6460:11
#13 0x000077bfe0a02244 llvm::detail::PassModel<llvm::Module, llvm::MemProfRemoveInfo, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /workspace/oss-llvm/llvm_src/llvm/include/llvm/IR/PassManagerInternal.h:91:17
#14 0x000077bfd9fd3e2a llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /workspace/oss-llvm/llvm_src/llvm/include/llvm/IR/PassManagerImpl.h:80:5
#15 0x000077bfe33cbf86 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) /workspace/oss-llvm/llvm_src/llvm/tools/opt/NewPMDriver.cpp:574:3
#16 0x000077bfe340076f optMain /workspace/oss-llvm/llvm_src/llvm/tools/opt/optdriver.cpp:758:9
#17 0x0000603a6a61ad31 main /workspace/oss-llvm/llvm_src/llvm/tools/opt/opt.cpp:27:35
#18 0x000077bfd8b9a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#19 0x000077bfd8b9a28b call_init ./csu/../csu/libc-start.c:128:20
#20 0x000077bfd8b9a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#21 0x0000603a6a61ac15 _start (/workspace/oss-llvm/bin/bin/opt+0x1c15)
Aborted (core dumped)
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360

CCing @abhilashnv and @Vsevolod-Livinskij as collaborators.

