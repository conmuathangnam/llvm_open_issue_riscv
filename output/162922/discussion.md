# [RISC-V][LoopVectorize] Assertion `hasUseList()' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/162922
**Status:** Closed
**Labels:** crash, llvm:analysis
**Closed Date:** 2025-10-27T05:01:37Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@l = external global [25 x i16]

define void @init() {
entry:
  store <4 x i16> splat (i16 1), ptr @l, align 8
  ret void
}

define i32 @main(ptr %0, ptr %1, i64 %indvars.iv2) #0 {
entry:
  tail call void @init()
  %2 = load i16, ptr getelementptr inbounds nuw (i8, ptr @l, i64 2), align 2
  %tobool.not.i = icmp eq i16 %2, 0
  %3 = load i16, ptr %0, align 2
  br i1 %tobool.not.i, label %for.cond1.preheader.i.preheader, label %common.ret

common.ret:                                       ; preds = %for.body4.i.1, %entry
  ret i32 0

for.cond1.preheader.i.preheader:                  ; preds = %entry
  %cmp35.i = icmp sgt i16 %3, 1
  %4 = zext i1 %cmp35.i to i64
  br label %for.body4.i

for.cond.cleanup3.i:                              ; preds = %for.body4.i
  %cmp35.i.1 = icmp sgt i16 %3, 1
  %5 = zext i1 %cmp35.i.1 to i64
  br label %for.body4.i.1

for.body4.i.1:                                    ; preds = %for.body4.i.1, %for.cond.cleanup3.i
  %indvars.iv2.1 = phi i64 [ %indvars.iv.next3.1, %for.body4.i.1 ], [ 0, %for.cond.cleanup3.i ]
  %gep57.i.1 = getelementptr [5 x i8], ptr %1, i64 %indvars.iv2.1
  store i8 0, ptr %gep57.i.1, align 1
  %indvars.iv.next3.1 = add nsw i64 %indvars.iv2.1, %5
  %cmp2.i.1 = icmp ult i64 %indvars.iv2.1, 5
  br i1 %cmp2.i.1, label %for.body4.i.1, label %common.ret, !llvm.loop !0

for.body4.i:                                      ; preds = %for.body4.i, %for.cond1.preheader.i.preheader
  %indvars.iv.next3 = or i64 %indvars.iv2, %4
  %cmp2.i = icmp eq i64 %indvars.iv.next3, 0
  br i1 %cmp2.i, label %for.body4.i, label %for.cond.cleanup3.i
}

attributes #0 = { "target-features"="+v" }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.mustprogress"}
```

Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt "-passes=lto<O3>" reduced.ll
WARNING: You're attempting to print out a bitcode file.
This is inadvisable as it may cause display problems. If
you REALLY want to taste LLVM bitcode first-hand, you
can force output with the `-f' option.

opt: /scratch/ewlu/daily-upstream-build/llvm/llvm/include/llvm/IR/Value.h:395: llvm::Value::user_iterator llvm::Value::materialized_user_begin(): Assertion `hasUseList()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt -passes=lto<O3> reduced.ll
1.      Running pass "function<eager-inv>(loop-mssa(licm<allowspeculation>),gvn<>,memcpyopt,dse,move-auto-init,mldst-motion<no-split-footer-bb>,loop(indvars,loop-deletion,loop-unroll-full),loop-distribute,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,sccp,instcombine<max-iterations=1;no-verify-fixpoint>,bdce,slp-vectorizer,vector-combine,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,jump-threading)" on module "reduced.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "main"
 #0 0x00005ec33c4aee60 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x35fae60)
 #1 0x00005ec33c4abb7a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007cce27a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x00007cce27a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #4 0x00007cce27a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #5 0x00007cce27a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #6 0x00007cce27a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007cce27a288ff abort ./stdlib/abort.c:81:7
 #8 0x00007cce27a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #9 0x00007cce27a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#10 0x00005ec33a171ad2 (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x12bdad2)
#11 0x00005ec339f586c5 llvm::LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(std::unique_ptr<llvm::VPlan, std::default_delete<llvm::VPlan>>, llvm::VFRange&, llvm::LoopVersioning*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10a46c5)
#12 0x00005ec339f58e91 llvm::LoopVectorizationPlanner::buildVPlansWithVPRecipes(llvm::ElementCount, llvm::ElementCount) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10a4e91)
#13 0x00005ec339f598b4 llvm::LoopVectorizationPlanner::plan(llvm::ElementCount, unsigned int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10a58b4)
#14 0x00005ec339f60771 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10ac771)
#15 0x00005ec339f63b81 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10afb81)
#16 0x00005ec339f6402a llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x10b002a)
#17 0x00005ec339781b83 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8cdb83)
#18 0x00005ec33c27c9e3 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c89e3)
#19 0x00005ec3397843b3 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x8d03b3)
#20 0x00005ec33c27b4a7 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c74a7)
#21 0x00005ec3393f98d3 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x5458d3)
#22 0x00005ec33c27ada3 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x33c6da3)
#23 0x00005ec339404af6 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x550af6)
#24 0x00005ec3393f7a7a optMain (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x543a7a)
#25 0x00007cce27a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#26 0x00007cce27a2a28b call_init ./csu/../csu/libc-start.c:128:20
#27 0x00007cce27a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#28 0x00005ec3393ede15 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/opt+0x539e15)
Aborted
```
Godbolt: https://godbolt.org/z/rxYEjMd3K

Found via fuzzer (C program before reduction).


