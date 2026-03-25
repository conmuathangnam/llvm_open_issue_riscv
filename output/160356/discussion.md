# [LV] " VPlan cost model and legacy cost model disagreed" regression on Neoverse-V2

**Author:** mcinally
**URL:** https://github.com/llvm/llvm-project/issues/160356
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-09-23T20:54:43Z

## Body

Similar to #160345, which was for Neoverse-V1...

We're seeing a good number of downstream ICEs from a change around 2025-09-22. Here is one reduced issue from Neoverse-V2:

```
scrubbed:$ cat test.ll
target triple = "aarch64-unknown-linux-gnu"

define void @foo(ptr %0) local_unnamed_addr #0 {
  br i1 poison, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %1
  br label %2

2:                                                ; preds = %11, %.lr.ph
  %3 = phi i64 [ poison, %.lr.ph ], [ %13, %11 ]
  %4 = phi i32 [ poison, %.lr.ph ], [ %12, %11 ]
  %5 = srem i32 %4, poison
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %11

7:                                                ; preds = %2
  %8 = sdiv i32 %4, poison
  %9 = sext i32 %8 to i64
  %10 = getelementptr i32, ptr %0, i64 %9
  store i32 1, ptr %10, align 4
  br label %11

11:                                               ; preds = %7, %2
  %12 = add nsw i32 %4, 1
  %13 = add nsw i64 %3, -1
  %14 = icmp sgt i64 %3, 1
  br i1 %14, label %2, label %._crit_edge

._crit_edge:                                      ; preds = %11, %1
  ret void
}

attributes #0 = { "target-cpu"="neoverse-v2" }
scrubbed:$ opt --passes=loop-vectorize -S test.ll 
opt: scrubbed/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7090: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || planContainsAdditionalSimplifications(getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: opt --passes=loop-vectorize -S test.ll
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "test.ll"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "foo"
 #0 0x0000000001bf3e10 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (scrubbed/llvm-3986/bin/opt+0x1bf3e10)
 #1 0x0000000001bf0f38 llvm::sys::RunSignalHandlers() (scrubbed/llvm-3986/bin/opt+0x1bf0f38)
 #2 0x0000000001bf10bc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00004003685c08dc (linux-vdso.so.1+0x8dc)
 #4 0x000040036887f200 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000040036883a67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x0000400368827130 abort ./stdlib/abort.c:81:7
 #7 0x0000400368833fd0 __assert_fail_base ./assert/assert.c:89:7
 #8 0x0000400368834040 __assert_perror_fail ./assert/assert-perr.c:31:1
 #9 0x0000000003990f98 llvm::LoopVectorizationPlanner::computeBestVF() (scrubbed/llvm-3986/bin/opt+0x3990f98)
#10 0x000000000399c054 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (scrubbed/llvm-3986/bin/opt+0x399c054)
#11 0x000000000399e448 llvm::LoopVectorizePass::runImpl(llvm::Function&) (scrubbed/llvm-3986/bin/opt+0x399e448)
#12 0x000000000399ea08 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (scrubbed/llvm-3986/bin/opt+0x399ea08)
#13 0x00000000034b2db0 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilder.cpp:0:0
#14 0x0000000001e9e8e4 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (scrubbed/llvm-3986/bin/opt+0x1e9e8e4)
#15 0x0000000002dd74e0 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) NVPTXTargetMachine.cpp:0:0
#16 0x0000000001e9d0d0 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (scrubbed/llvm-3986/bin/opt+0x1e9d0d0)
#17 0x0000000002dd7490 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) NVPTXTargetMachine.cpp:0:0
#18 0x0000000001e9cb3c llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (scrubbed/llvm-3986/bin/opt+0x1e9cb3c)
#19 0x00000000033602a0 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (scrubbed/llvm-3986/bin/opt+0x33602a0)
#20 0x0000000001bd66f4 optMain (scrubbed/llvm-3986/bin/opt+0x1bd66f4)
#21 0x00004003688273fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#22 0x00004003688274cc call_init ./csu/../csu/libc-start.c:128:20
#23 0x00004003688274cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#24 0x0000000001bcb38c _start (scrubbed/llvm-3986/bin/opt+0x1bcb38c)
Aborted
``` 

## Comments

### Comment 1 - mcinally

@fhahn for visibility.

---

