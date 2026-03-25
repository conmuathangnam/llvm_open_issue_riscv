# [LoopVectorize] Assertion `OpType == Other.OpType && "OpType must match"' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/160396
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-09-25T08:01:37Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i8 @c() #0 {
entry:
  br label %for.cond1.preheader.us

for.cond1.preheader.us:                           ; preds = %for.cond1.preheader.us, %entry
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.cond1.preheader.us ], [ 0, %entry ]
  %.us-phi4748.us = phi i8 [ %mul.us54, %for.cond1.preheader.us ], [ 0, %entry ]
  %arrayidx12.us = getelementptr [6 x [6 x i16]], ptr null, i64 %indvars.iv
  %0 = load i32, ptr null, align 4
  %tobool9.us52 = icmp ne i32 %0, 0
  %1 = zext i1 %tobool9.us52 to i64
  %conv11.us53 = trunc i64 %1 to i16
  store i16 %conv11.us53, ptr %arrayidx12.us, align 2
  %mul.us54 = mul i8 %.us-phi4748.us, 0
  %indvars.iv.next = add i64 %indvars.iv, 1
  %cmp.us = icmp ult i64 %indvars.iv, 416
  br i1 %cmp.us, label %for.cond1.preheader.us, label %for.cond.cleanup.sink.split.loopexit81

for.cond.cleanup.sink.split.loopexit81:           ; preds = %for.cond1.preheader.us
  ret i8 %mul.us54
}

attributes #0 = { "target-features"="+v" }
```
Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt --passes=loop-vectorize reduced.ll
WARNING: You're attempting to print out a bitcode file.
This is inadvisable as it may cause display problems. If
you REALLY want to taste LLVM bitcode first-hand, you
can force output with the `-f' option.

opt: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:396: void llvm::VPIRFlags::intersectFlags(const llvm::VPIRFlags&): Assertion `OpType == Other.OpType && "OpType must match"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt --passes=loop-vectorize reduced.ll
1.      Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "reduced.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "c"
 #0 0x0000588c53884c12 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33ffc12)
 #1 0x0000588c5388190f llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33fc90f)
 #2 0x0000588c53881a5c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000750611645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000075061169eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000075061169eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000075061169eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000075061164527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007506116288ff abort ./stdlib/abort.c:81:7
 #9 0x000075061162881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000075061163b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000588c51656fcc (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11d1fcc)
#12 0x0000588c516a53e6 llvm::VPlanTransforms::cse(llvm::VPlan&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x12203e6)
#13 0x0000588c5148525a llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x100025a)
#14 0x0000588c514a3d58 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x101ed58)
#15 0x0000588c514a6601 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x1021601)
#16 0x0000588c514a6c16 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x1021c16)
#17 0x0000588c50d36865 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8b1865)
#18 0x0000588c53664155 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31df155)
#19 0x0000588c50d39095 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8b4095)
#20 0x0000588c53662741 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31dd741)
#21 0x0000588c509c3515 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x53e515)
#22 0x0000588c53662f2d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31ddf2d)
#23 0x0000588c509cf599 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x54a599)
#24 0x0000588c509c16ed optMain (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x53c6ed)
#25 0x000075061162a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#26 0x000075061162a28b call_init ./csu/../csu/libc-start.c:128:20
#27 0x000075061162a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#28 0x0000588c509b7615 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x532615)
Aborted
```

Godbolt: https://godbolt.org/z/qn5rr7xn9

Found via fuzzer

