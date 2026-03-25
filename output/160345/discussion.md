# [LV] " VPlan cost model and legacy cost model disagreed" regression

**Author:** mcinally
**URL:** https://github.com/llvm/llvm-project/issues/160345
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-09-25T16:21:02Z

## Body

We're seeing a good number of downstream ICEs from a change around 2025-09-22. Here is one reduced issue from Neoverse-V1 (and more from Neoverse-V2 are coming):


```
[scrubbed ~]$ cat new.ll
target triple = "aarch64-unknown-linux-gnu"

define void @foo() local_unnamed_addr #0 {
  br i1 poison, label %.lr.ph, label %._crit_edge

.lr.ph:                                           ; preds = %13, %0
  %1 = phi i64 [ %15, %13 ], [ poison, %0 ]
  %2 = phi i32 [ %14, %13 ], [ 1, %0 ]
  %3 = add nsw i32 %2, -1
  %4 = srem i32 %3, poison
  %5 = add nsw i32 %4, 1
  %6 = icmp eq i32 %5, poison
  br i1 %6, label %7, label %13

7:                                                ; preds = %.lr.ph
  %8 = sdiv i32 %3, poison
  %9 = add i32 %8, 1
  %10 = sext i32 %9 to i64
  %11 = getelementptr i32, ptr poison, i64 %10
  %12 = getelementptr i8, ptr %11, i64 -4
  store i32 %2, ptr %12, align 4
  br label %13

13:                                               ; preds = %7, %.lr.ph
  %14 = add nuw nsw i32 %2, 1
  %15 = add nsw i64 %1, -1
  %16 = icmp sgt i64 %1, 1
  br i1 %16, label %.lr.ph, label %._crit_edge

._crit_edge:                                      ; preds = %13, %0
  ret void
}

attributes #0 = { "target-cpu"="neoverse-v1" }
[scrubbed ~]$ opt --passes=loop-vectorize new.ll -S
opt: /scrubbed/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7090: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || planContainsAdditionalSimplifications(getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: opt --passes=loop-vectorize new.ll -S
1.      Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "new.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "foo"
 #0 0x0000000001bf3d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (opt+0x1bf3d48)
 #1 0x0000000001bf0e70 llvm::sys::RunSignalHandlers() (opt+0x1bf0e70)
 #2 0x0000000001bf0ff4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000040001ee507a0 (linux-vdso.so.1+0x7a0)
 #4 0x000040001f106274 raise (/lib64/libc.so.6+0x36274)
 #5 0x000040001f0f0a2c abort (/lib64/libc.so.6+0x20a2c)
 #6 0x000040001f0ffba0 __assert_fail_base (/lib64/libc.so.6+0x2fba0)
 #7 0x000040001f0ffc18 __assert_perror_fail (/lib64/libc.so.6+0x2fc18) 
 #8 0x0000000003990f70 llvm::LoopVectorizationPlanner::computeBestVF() (opt+0x3990f70)
 #9 0x000000000399c02c llvm::LoopVectorizePass::processLoop(llvm::Loop*) (opt+0x399c02c)
#10 0x000000000399e420 llvm::LoopVectorizePass::runImpl(llvm::Function&) (opt+0x399e420)
#11 0x000000000399e9e0 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (opt+0x399e9e0)
#12 0x00000000034b2d88 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilder.cpp:0:0
#13 0x0000000001e9e81c llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (opt+0x1e9e81c)
#14 0x0000000002dd7418 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) NVPTXTargetMachine.cpp:0:0
#15 0x0000000001e9d008 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (opt+0x1e9d008)
#16 0x0000000002dd73c8 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) NVPTXTargetMachine.cpp:0:0
#17 0x0000000001e9ca74 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (opt+0x1e9ca74)
#18 0x0000000003360278 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (opt+0x3360278)
#19 0x0000000001bd662c optMain (opt+0x1bd662c)
#20 0x000040001f0f4384 __libc_start_main (/lib64/libc.so.6+0x24384)
#21 0x0000000001bcb2c4 _start (opt+0x1bcb2c4)
Aborted (core dumped)
```

## Comments

### Comment 1 - mcinally

@fhahn for visibility.

---

### Comment 2 - fhahn

Should be fixed by https://github.com/llvm/llvm-project/commit/88aab08ae5682a21edef71b814e5ebc05e7a9450 as well

---

### Comment 3 - mcinally

Confirmed. Thanks!

---

