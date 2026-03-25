# [LV] [VPlan] Assertion in VPlanTransforms.cpp "only recipes with a single defined value expected"

**Author:** scui-ibm
**URL:** https://github.com/llvm/llvm-project/issues/170666
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-12-04T21:09:32Z

## Body

Reduced IR t.ll:

```
target datalayout = "E-m:a-Fi64-i64:64-i128:128-n32:64-S128-v256:256:256-v512:512:512"
target triple = "powerpc64-ibm-aix7.2.0.0"
 
; Function Attrs: mustprogress strictfp
define i32 @_Z14checksumFile_HP4FILEPh(i32 %n) #0 {
entry:
  br label %for.body
 
for.body:                                     ; preds = %for.body, %entry
  %ix2 = phi i8 [ 0, %entry ], [ %xor, %for.body ]
  %ix1 = phi i8 [ 0, %entry ], [ %fshl1, %for.body ]
  %ix = phi i32 [ 0, %entry ], [ %add, %for.body ]
  %0 = sext i32 %ix to i64
  %gep = getelementptr i8, ptr poison, i64 %0
  %fshl = call i8 @llvm.fshl.i8(i8 %ix1, i8 0, i8 0) #2
  %fshl1 = call i8 @llvm.fshl.i8(i8 %ix2, i8 0, i8 0) #2
  %ld = load i8, ptr %gep, align 1
  %xor = xor i8 %ld, %fshl
  %gep1 = getelementptr i8, ptr %gep, i64 1
  %ld1 = load i8, ptr %gep1, align 1
  %add= add i32 %ix, 4
  %cmp = icmp slt i32 %ix, %n
  br i1 %cmp, label %for.body, label %for.end
 
for.end:                             ; preds = %for.body
  ret i32 0
}
 
; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i8 @llvm.fshl.i8(i8, i8, i8) #1
 
; uselistorder directives
uselistorder ptr @llvm.fshl.i8, { 1, 0 }
 
attributes #0 = { mustprogress strictfp }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { strictfp } 
```
 
Assertion:

```
opt: /home/llvm/main/llvm-project/llvm/lib/Transforms/Vectorize/VPlanTransforms.cpp:2104: bool hoistPreviousBeforeFORUsers(VPFirstOrderRecurrencePHIRecipe *, VPRecipeBase *, VPDominatorTree &): Assertion `Current->getNumDefinedValues() == 1 && "only recipes with a single defined value expected"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.     Program arguments: /home/llvm/main/build/bin/opt -passes=loop-vectorize t0.ll
1.     Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "t0.ll"
2.     Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "_Z14checksumFile_HP4FILEPh"
 #0 0x0000040b8191004c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/llvm/main/build/bin/opt+0x5b5004c)
 #1 0x0000040b819107f4 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x0000040b8190ca4c llvm::sys::RunSignalHandlers() (/home/llvm/main/build/bin/opt+0x5b4ca4c)
 #3 0x0000040b8191194c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x0000722b9dc30444 (linux-vdso64.so.1+0x444)
 #5 0x0000722b9d3c55fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000722b9d3c55fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000722b9d3c55fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000722b9d35bd5c raise ./signal/../sysdeps/posix/raise.c:26:13
 #9 0x0000722b9d33645c abort ./stdlib/abort.c:79:7
#10 0x0000722b9d34d824 __assert_fail_base ./assert/assert.c:96:3
#11 0x0000722b9d34d8c4 __assert_fail ./assert/assert.c:105:3
#12 0x0000040b834cae00 llvm::VPlanTransforms::adjustFixedOrderRecurrences(llvm::VPlan&, llvm::VPBuilder&) (/home/llvm/main/build/bin/opt+0x770ae00)
#13 0x0000040b83416364 llvm::LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(std::unique_ptr<llvm::VPlan, std::default_delete<llvm::VPlan>>, llvm::VFRange&, llvm::LoopVersioning*) (/home/llvm/main/build/bin/opt+0x7656364)
#14 0x0000040b83407278 llvm::LoopVectorizationPlanner::buildVPlansWithVPRecipes(llvm::ElementCount, llvm::ElementCount) (/home/llvm/main/build/bin/opt+0x7647278)
#15 0x0000040b834069f0 llvm::LoopVectorizationPlanner::plan(llvm::ElementCount, unsigned int) (/home/llvm/main/build/bin/opt+0x76469f0)
#16 0x0000040b8341dfc4 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/home/llvm/main/build/bin/opt+0x765dfc4)
#17 0x0000040b8342937c llvm::LoopVectorizePass::runImpl(llvm::Function&) (/home/llvm/main/build/bin/opt+0x766937c)
#18 0x0000040b83429dd0 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/llvm/main/build/bin/opt+0x7669dd0)
#19 0x0000040b832bbe9c llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilderPipelines.cpp:0:0
#20 0x0000040b81bd55a0 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/llvm/main/build/bin/opt+0x5e155a0)
#21 0x0000040b832b69fc llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilderPipelines.cpp:0:0
#22 0x0000040b81bdb2c4 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/llvm/main/build/bin/opt+0x5e1b2c4)
#23 0x0000040b8323ff3c llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) NewPMDriver.cpp:0:0
#24 0x0000040b81bd3d60 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/llvm/main/build/bin/opt+0x5e13d60)
#25 0x0000040b83233e9c llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/home/llvm/main/build/bin/opt+0x7473e9c)
#26 0x0000040b81894384 optMain (/home/llvm/main/build/bin/opt+0x5ad4384)
#27 0x0000040b8188c460 main (/home/llvm/main/build/bin/opt+0x5acc460)
#28 0x0000722b9d336ca4 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#29 0x0000722b9d336eec __libc_start_main ./csu/../sysdeps/unix/sysv/linux/powerpc/libc-start.c:77:48

```


