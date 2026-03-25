# LoopVectorizePass crash: assertion `!NodePtr->isKnownSentinel()' failed

**Author:** DaniilSuchkov
**URL:** https://github.com/llvm/llvm-project/issues/182152
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-03-04T11:28:58Z

## Body

Running `opt -passes=loop-vectorize reduced.ll` on this IR:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @hoge.24(i32 %load2) {
bb:
  %icmp120 = icmp eq i32 0, 0
  br label %bb121

bb121:                                            ; preds = %bb121, %bb
  %phi122 = phi i32 [ %add124, %bb121 ], [ %load2, %bb ]
  %phi123 = phi i32 [ %select, %bb121 ], [ 0, %bb ]
  %select = select i1 %icmp120, i32 %phi123, i32 %phi122
  %add124 = add i32 %phi122, 1
  %icmp125 = icmp sgt i32 %phi122, 0
  br i1 %icmp125, label %bb127, label %bb121

bb127:                                            ; preds = %bb121
  ret i32 %select
}
```
results in the following crash:
```
opt: /root/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:168: llvm::ilist_iterator<OptionsT, IsReverse, IsConst>::reference llvm::ilist_iterator<OptionsT, IsReverse, IsConst>::operator*() const [with OptionsT = llvm::ilist_detail::node_options<llvm::VPRecipeBase, true, false, void, false, void>; bool IsReverse = false; bool IsConst = false; llvm::ilist_iterator<OptionsT, IsReverse, IsConst>::reference = llvm::VPRecipeBase&]: Assertion `!NodePtr->isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "hoge.24"
 #0 0x0000000005b93398 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b93398)
 #1 0x0000000005b90274 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007086d7a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007086d7a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007086d7a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007086d7a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007086d7a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007086d7a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003997e03 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3997e03)
 #9 0x00000000039a241c llvm::VPlanTransforms::handleFindLastReductions(llvm::VPlan&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x39a241c)
#10 0x00000000037de84f llvm::LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(std::unique_ptr<llvm::VPlan, std::default_delete<llvm::VPlan>>, llvm::VFRange&, llvm::LoopVersioning*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37de84f)
#11 0x00000000037dff45 llvm::LoopVectorizationPlanner::buildVPlansWithVPRecipes(llvm::ElementCount, llvm::ElementCount) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37dff45)
#12 0x00000000037ef095 llvm::LoopVectorizationPlanner::plan(llvm::ElementCount, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37ef095)
#13 0x00000000037ff8a6 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37ff8a6)
#14 0x0000000003802610 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3802610)
#15 0x0000000003802d9d llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3802d9d)
#16 0x00000000030cefae llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30cefae)
#17 0x00000000058fe441 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58fe441)
#18 0x0000000000f1484e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1484e)
#19 0x00000000058fc9fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58fc9fa)
#20 0x000000000096c50e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96c50e)
#21 0x00000000058fc3b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58fc3b1)
#22 0x00000000009766ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9766ca)
#23 0x000000000096a778 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96a778)
#24 0x00007086d7a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007086d7a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000009613f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9613f5)
```
It's reproducible on LLVM trunk: https://godbolt.org/z/3PhjKfroe

## Comments

### Comment 1 - DaniilSuchkov

According to our QA, this crash was introduced/triggered by this commit: https://github.com/llvm/llvm-project/commit/2abd6d6d7ac54d40302dfc946d01572a12963ee5

```
commit 2abd6d6d7ac54d40302dfc946d01572a12963ee5
Author: Graham Hunter <[graham.hunter@arm.com](mailto:graham.hunter@arm.com)>
Date:   Wed Jan 14 14:59:18 2026 +0000

[LV] Vectorize conditional scalar assignments (https://github.com/llvm/llvm-project/pull/158088)
Based on Michael Maitland's previous work:
https://github.com/llvm/llvm-project/pull/121222

This PR uses the existing recurrences code instead of introducing a
new pass just for CSA autovec. I've also made recipes that are more
generic.
```

---

### Comment 2 - DaniilSuchkov

@huntergr-arm

---

