# Assertion `(!AccumulatorRecipe || isa<VPReductionPHIRecipe>(AccumulatorRecipe) || isa<VPPartialReductionRecipe>(AccumulatorRecipe)) && "Unexpected operand order for partial reduction recipe"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/165222
**Status:** Closed
**Labels:** backend:AArch64, obsolete, vectorizers, crash
**Closed Date:** 2025-12-16T12:09:05Z

## Body

To reproduce run opt with the test below (-passes=loop-vectorize):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32-ni:1-p2:32:8:8:32-ni:2"
target triple = "aarch64-none-linux-gnu"

define void @wombat(i32 %arg) #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb1, %bb
  %phi = phi i64 [ 266, %bb ], [ %add4, %bb1 ]
  %phi2 = phi i64 [ 0, %bb ], [ %add3, %bb1 ]
  %sext = sext i32 %arg to i64
  %mul = mul i64 %sext, 2
  %add = add i64 %sext, %phi2
  %add3 = add i64 %add, %mul
  store i32 0, ptr addrspace(1) null, align 4
  %add4 = add i64 %phi, 1
  %icmp = icmp ugt i64 %phi, 0
  br i1 %icmp, label %bb1, label %bb5

bb5:                                              ; preds = %bb1
  %phi6 = phi i64 [ %add3, %bb1 ]
  ret void
}

attributes #0 = { "target-cpu"="neoverse-n1" }
```
Reproducer: https://godbolt.org/z/8dh8xxaTf

Stack dump:
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "wombat"
 #0 0x0000000005975828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5975828)
 #1 0x00000000059726d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007a7348c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007a7348c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007a7348c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007a7348c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007a7348c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007a7348c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000385d8cc llvm::VPPartialReductionRecipe::clone() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x385d8cc)
 #9 0x00000000038addeb (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#10 0x00000000038ad0dc (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#11 0x00000000038aec18 llvm::VPlanTransforms::unrollByUF(llvm::VPlan&, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38aec18)
#12 0x0000000003697ef2 llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3697ef2)
#13 0x00000000036b625c llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b625c)
#14 0x00000000036b8150 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8150)
#15 0x00000000036b8835 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8835)
#16 0x0000000002fa072e llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fa072e)
#17 0x000000000574f081 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574f081)
#18 0x0000000000efc57e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc57e)
#19 0x000000000574d6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d6fa)
#20 0x000000000097bade llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bade)
#21 0x000000000574d0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d0b1)
#22 0x0000000000985c7a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c7a)
#23 0x0000000000979ef1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979ef1)
#24 0x00007a7348c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007a7348c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000971305 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971305)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (TatyanaDoubts)

<details>
To reproduce run opt with the test below (-passes=loop-vectorize):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32-ni:1-p2:32:8:8:32-ni:2"
target triple = "aarch64-none-linux-gnu"

define void @<!-- -->wombat(i32 %arg) #<!-- -->0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb1, %bb
  %phi = phi i64 [ 266, %bb ], [ %add4, %bb1 ]
  %phi2 = phi i64 [ 0, %bb ], [ %add3, %bb1 ]
  %sext = sext i32 %arg to i64
  %mul = mul i64 %sext, 2
  %add = add i64 %sext, %phi2
  %add3 = add i64 %add, %mul
  store i32 0, ptr addrspace(1) null, align 4
  %add4 = add i64 %phi, 1
  %icmp = icmp ugt i64 %phi, 0
  br i1 %icmp, label %bb1, label %bb5

bb5:                                              ; preds = %bb1
  %phi6 = phi i64 [ %add3, %bb1 ]
  ret void
}

attributes #<!-- -->0 = { "target-cpu"="neoverse-n1" }
```
Reproducer: https://godbolt.org/z/8dh8xxaTf

Stack dump:
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize &lt;source&gt;
1.	Running pass "function(loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;)" on module "&lt;source&gt;"
2.	Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "wombat"
 #<!-- -->0 0x0000000005975828 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5975828)
 #<!-- -->1 0x00000000059726d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007a7348c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007a7348c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007a7348c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007a7348c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007a7348c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007a7348c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000385d8cc llvm::VPPartialReductionRecipe::clone() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x385d8cc)
 #<!-- -->9 0x00000000038addeb (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#<!-- -->10 0x00000000038ad0dc (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#<!-- -->11 0x00000000038aec18 llvm::VPlanTransforms::unrollByUF(llvm::VPlan&amp;, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38aec18)
#<!-- -->12 0x0000000003697ef2 llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&amp;, llvm::InnerLoopVectorizer&amp;, llvm::DominatorTree*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3697ef2)
#<!-- -->13 0x00000000036b625c llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b625c)
#<!-- -->14 0x00000000036b8150 llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8150)
#<!-- -->15 0x00000000036b8835 llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8835)
#<!-- -->16 0x0000000002fa072e llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fa072e)
#<!-- -->17 0x000000000574f081 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574f081)
#<!-- -->18 0x0000000000efc57e llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc57e)
#<!-- -->19 0x000000000574d6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d6fa)
#<!-- -->20 0x000000000097bade llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bade)
#<!-- -->21 0x000000000574d0b1 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d0b1)
#<!-- -->22 0x0000000000985c7a llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c7a)
#<!-- -->23 0x0000000000979ef1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979ef1)
#<!-- -->24 0x00007a7348c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x00007a7348c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000971305 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971305)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (TatyanaDoubts)

<details>
To reproduce run opt with the test below (-passes=loop-vectorize):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32-ni:1-p2:32:8:8:32-ni:2"
target triple = "aarch64-none-linux-gnu"

define void @<!-- -->wombat(i32 %arg) #<!-- -->0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb1, %bb
  %phi = phi i64 [ 266, %bb ], [ %add4, %bb1 ]
  %phi2 = phi i64 [ 0, %bb ], [ %add3, %bb1 ]
  %sext = sext i32 %arg to i64
  %mul = mul i64 %sext, 2
  %add = add i64 %sext, %phi2
  %add3 = add i64 %add, %mul
  store i32 0, ptr addrspace(1) null, align 4
  %add4 = add i64 %phi, 1
  %icmp = icmp ugt i64 %phi, 0
  br i1 %icmp, label %bb1, label %bb5

bb5:                                              ; preds = %bb1
  %phi6 = phi i64 [ %add3, %bb1 ]
  ret void
}

attributes #<!-- -->0 = { "target-cpu"="neoverse-n1" }
```
Reproducer: https://godbolt.org/z/8dh8xxaTf

Stack dump:
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize &lt;source&gt;
1.	Running pass "function(loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;)" on module "&lt;source&gt;"
2.	Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "wombat"
 #<!-- -->0 0x0000000005975828 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5975828)
 #<!-- -->1 0x00000000059726d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007a7348c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007a7348c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007a7348c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007a7348c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007a7348c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007a7348c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000385d8cc llvm::VPPartialReductionRecipe::clone() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x385d8cc)
 #<!-- -->9 0x00000000038addeb (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#<!-- -->10 0x00000000038ad0dc (anonymous namespace)::UnrollState::unrollBlock(llvm::VPBlockBase*) VPlanUnroll.cpp:0:0
#<!-- -->11 0x00000000038aec18 llvm::VPlanTransforms::unrollByUF(llvm::VPlan&amp;, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38aec18)
#<!-- -->12 0x0000000003697ef2 llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&amp;, llvm::InnerLoopVectorizer&amp;, llvm::DominatorTree*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3697ef2)
#<!-- -->13 0x00000000036b625c llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b625c)
#<!-- -->14 0x00000000036b8150 llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8150)
#<!-- -->15 0x00000000036b8835 llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36b8835)
#<!-- -->16 0x0000000002fa072e llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fa072e)
#<!-- -->17 0x000000000574f081 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574f081)
#<!-- -->18 0x0000000000efc57e llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefc57e)
#<!-- -->19 0x000000000574d6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d6fa)
#<!-- -->20 0x000000000097bade llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bade)
#<!-- -->21 0x000000000574d0b1 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574d0b1)
#<!-- -->22 0x0000000000985c7a llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c7a)
#<!-- -->23 0x0000000000979ef1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979ef1)
#<!-- -->24 0x00007a7348c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x00007a7348c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000971305 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971305)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

### Comment 3 - sdesmalen-arm

Hi @TatyanaDoubts I can't reproduce the failure with latest HEAD. Should I use any other flags to run the test other than `-passes=loop-vectorize` ?

---

### Comment 4 - TatyanaDoubts

Hi @sdesmalen-arm, I cannot reproduce it either now.. I don't think you should use other flags.

---

### Comment 5 - fhahn

Looks like this does not reproduce on current main, closing. Please re-open if it still reproduces on your end.

---

