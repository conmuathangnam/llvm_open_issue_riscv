# crash with Floating point Exception (loop-vectorize pass)

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/172049
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-12-16T06:50:06Z

## Body

To reproduce run opt with the following test (-passes=loop-vectorize):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define ptr addrspace(1) @wombat() gc "statepoint-example" {
bb:
  br label %bb2

bb1:                                              ; preds = %bb6
  ret ptr addrspace(1) null

bb2:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  br i1 false, label %bb3, label %bb6

bb3:                                              ; preds = %bb2
  br i1 false, label %bb4, label %bb5, !prof !0

bb4:                                              ; preds = %bb3
  br label %bb6

bb5:                                              ; preds = %bb3
  br label %bb6

bb6:                                              ; preds = %bb5, %bb4, %bb2
  %add = add i64 %phi, 1
  %icmp = icmp eq i64 %phi, 0
  br i1 %icmp, label %bb2, label %bb1
}

!0 = !{!"branch_weights", i32 1, i32 0}
```
Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "wombat"
 #0 0x0000000005a06d68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a06d68)
 #1 0x0000000005a03c14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000752795e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000003716a77 llvm::LoopVectorizationCostModel::expectedCost(llvm::ElementCount) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3716a77)
 #4 0x0000000003725505 llvm::LoopVectorizationPlanner::computeBestVF() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3725505)
 #5 0x00000000037297d9 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37297d9)
 #6 0x000000000372c3a0 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x372c3a0)
 #7 0x000000000372cb2d llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x372cb2d)
 #8 0x000000000300716e llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x300716e)
 #9 0x00000000057e4471 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57e4471)
#10 0x0000000000f0f07e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0f07e)
#11 0x00000000057e2b3a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57e2b3a)
#12 0x00000000009793be llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9793be)
#13 0x00000000057e24f1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57e24f1)
#14 0x00000000009836da llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9836da)
#15 0x0000000000977578 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977578)
#16 0x0000752795e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#17 0x0000752795e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#18 0x000000000096e025 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96e025)
Program terminated with signal: SIGFPE
Compiler returned: 136
```

Reproducer: https://godbolt.org/z/3h4efaPbf


## Comments

### Comment 1 - dev-priyanshu15

I would like to work on fixing this crash.  
Could you assign the issue to me?  

---

### Comment 2 - lukel97

I can't reproduce this on main currently on an x86 machine, I think this was fixed in 0fbb45e7d6dcccf88248d6be5e4f167ddb3e3fa8.

Do you know what commit you were seeing this crash at? Compiler explorer is saying clang (assertions trunk) is at 3686ff2b15be36b510e8efef2d3f95968f716b68 which is a couple days old now at this point, and before 0fbb45e7d6dcccf88248d6be5e4f167ddb3e3fa8 landed. I couldn't find a way to see the commit for opt (assertions trunk).

---

### Comment 3 - dev-priyanshu15

Hi @lukel97, thanks for the feedback!

I've implemented a fix for this issue. The crash was occurring in the 
`getPredBlockCostDivisor()` function in `LoopVectorize.cpp` when performing 
division by zero when block frequency is 0.

**The Fix:**

In `llvm/lib/Transforms/Vectorize/LoopVectorize.cpp` at line 2907-2914:

```cpp
uint64_t LoopVectorizationCostModel::getPredBlockCostDivisor(
    BasicBlock *BB) {
  uint64_t HeaderFreq = getBFI().getBlockFreq(Header).getFrequency();
  uint64_t BBFreq = getBFI().getBlockFreq(BB).getFrequency();
  assert(HeaderFreq >= BBFreq &&
         "Header has smaller block freq than dominated BB?");
  // Guard against division by zero when BBFreq is 0.
  // In such cases, return 1 to avoid undefined behavior.
  if (BBFreq == 0)
    return 1;
  return std::round((double)HeaderFreq / BBFreq);
}

---

### Comment 4 - lukel97

Closing assuming fixed in 0fbb45e

---

