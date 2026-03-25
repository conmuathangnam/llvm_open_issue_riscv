# [LoopVectorize] Assertion `OpType == Other.OpType && "OpType must match"' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/162374
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-10-12T13:01:27Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @init(ptr %arrayidx2.i, ptr %arrayidx13.i) #0 {
entry:
  br label %for.body.i

for.body.i:                                       ; preds = %for.body.i, %entry
  %indvars.iv.i1 = phi i64 [ 0, %entry ], [ %indvars.iv.next.i, %for.body.i ]
  %n.023.i = phi i32 [ 0, %entry ], [ %inc.i, %for.body.i ]
  %0 = load i16, ptr %arrayidx2.i, align 2
  %conv3.i = zext i16 %0 to i64
  %conv4.i = trunc i64 %conv3.i to i8
  store i8 %conv4.i, ptr null, align 1
  %conv11.i = trunc i16 %0 to i8
  store i8 %conv11.i, ptr %arrayidx13.i, align 1
  %inc.i = add i32 %n.023.i, 1
  %tobool.not.i = icmp eq i32 %inc.i, 0
  %indvars.iv.next.i = add i64 %indvars.iv.i1, 1
  br i1 %tobool.not.i, label %c.exit.loopexit, label %for.body.i

c.exit.loopexit:                                  ; preds = %for.body.i
  ret void
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

opt: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:404: void llvm::VPIRFlags::intersectFlags(const llvm::VPIRFlags&): Assertion `OpType == Other.OpType && "OpType must match"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt --passes=loop-vectorize reduced.ll
1.      Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "reduced.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "init"
 #0 0x00005c399f7f0072 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x342a072)
 #1 0x00005c399f7ecd6f llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x3426d6f)
 #2 0x00005c399f7ecebc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007e76d1245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007e76d129eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007e76d129eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007e76d129eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007e76d124527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007e76d12288ff abort ./stdlib/abort.c:81:7
 #9 0x00007e76d122881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007e76d123b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005c399d5a1cbc (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x11dbcbc)
#12 0x00005c399d5f4f66 llvm::VPlanTransforms::cse(llvm::VPlan&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x122ef66)
#13 0x00005c399d3d02ef llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x100a2ef)
#14 0x00005c399d3edb79 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x1027b79)
#15 0x00005c399d3f00e1 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x102a0e1)
#16 0x00005c399d3f06f6 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x102a6f6)
#17 0x00005c399cc79fd5 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8b3fd5)
#18 0x00005c399f5c5cf5 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31ffcf5)
#19 0x00005c399cc7c805 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8b6805)
#20 0x00005c399f5c43f1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31fe3f1)
#21 0x00005c399c905fb5 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x53ffb5)
#22 0x00005c399f5c4bdd llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x31febdd)
#23 0x00005c399c911eeb llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x54beeb)
#24 0x00005c399c90418d optMain (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x53e18d)
#25 0x00007e76d122a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#26 0x00007e76d122a28b call_init ./csu/../csu/libc-start.c:128:20
#27 0x00007e76d122a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#28 0x00005c399c8fa0b5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x5340b5)
Aborted
```

Godbolt: https://godbolt.org/z/ns6hTYcbK

Found via fuzzer (C program before reduction).

