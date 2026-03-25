# [LV] Assertion `!R->onlyScalarsGenerated(Plan->hasScalableVF()) && "Recipe should have been replaced"'

**Author:** arcbbb
**URL:** https://github.com/llvm/llvm-project/issues/169780
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-11-27T21:52:16Z

## Body

```
define void @blam(ptr %arg, ptr %arg1) {
bb:
  br label %bb400

bb400:                                            ; preds = %bb437, %bb
  %phi401 = phi ptr [ %getelementptr440, %bb437 ], [ %arg, %bb ]
  %phi402 = phi ptr [ %getelementptr439, %bb437 ], [ %arg1, %bb ]
  %load403 = load i64, ptr %phi402, align 4
  store i64 %load403, ptr %phi401, align 4
  %getelementptr404 = getelementptr i8, ptr %phi402, i64 8
  %getelementptr405 = getelementptr i8, ptr %phi401, i64 8
  %load408 = load i64, ptr %getelementptr404, align 4
  store i64 %load408, ptr %getelementptr405, align 4
  %getelementptr409 = getelementptr i8, ptr %phi402, i64 16
  %getelementptr410 = getelementptr i8, ptr %phi401, i64 16
  %load413 = load i64, ptr %getelementptr409, align 4
  store i64 %load413, ptr %getelementptr410, align 4
  %getelementptr414 = getelementptr i8, ptr %phi402, i64 24
  %getelementptr415 = getelementptr i8, ptr %phi401, i64 24
  %load418 = load i64, ptr %getelementptr414, align 4
  store i64 %load418, ptr %getelementptr415, align 4
  %getelementptr419 = getelementptr i8, ptr %phi402, i64 32
  %getelementptr420 = getelementptr i8, ptr %phi401, i64 32
  %load423 = load i64, ptr %getelementptr419, align 4
  store i64 %load423, ptr %getelementptr420, align 4
  %getelementptr424 = getelementptr i8, ptr %phi402, i64 40
  %getelementptr425 = getelementptr i8, ptr %phi401, i64 40
  %load428 = load i64, ptr %getelementptr424, align 4
  store i64 %load428, ptr %getelementptr425, align 4
  %getelementptr429 = getelementptr i8, ptr %phi402, i64 48
  %getelementptr430 = getelementptr i8, ptr %phi401, i64 48
  %load433 = load i64, ptr %getelementptr429, align 4
  store i64 %load433, ptr %getelementptr430, align 4
  %getelementptr434 = getelementptr i8, ptr %phi402, i64 56
  %getelementptr435 = getelementptr i8, ptr %phi401, i64 56
  %icmp436 = icmp eq ptr %getelementptr434, null
  br i1 %icmp436, label %bb442, label %bb437

bb437:                                            ; preds = %bb400
  %load438 = load i64, ptr %getelementptr434, align 4
  store i64 %load438, ptr %getelementptr435, align 4
  %getelementptr439 = getelementptr i8, ptr %phi402, i64 64
  %getelementptr440 = getelementptr i8, ptr %phi401, i64 64
  br i1 false, label %bb442, label %bb400

bb442:                                            ; preds = %bb437, %bb400
  %phi443 = phi ptr [ %getelementptr440, %bb437 ], [ %getelementptr435, %bb400 ]
  ret void
}
```
This IR triggers the assertion
```
opt: /root/llvm-project/llvm/lib/Transforms/Vectorize/VPlanTransforms.cpp:3513: void expandVPWidenPointerInduction(llvm::VPWidenPointerInductionRecipe*, llvm::VPTypeAnalysis&): Assertion `!R->onlyScalarsGenerated(Plan->hasScalableVF()) && "Recipe should have been replaced"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -mtriple riscv64 -mcpu=sifive-p670 -p loop-vectorize -vectorize-scev-check-threshold=3000 <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "blam"
 #0 0x00000000059a86a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59a86a8)
 #1 0x00000000059a5554 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f0308042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f03080969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f0308042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f03080287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007f030802871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007f0308039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000038d5422 llvm::VPlanTransforms::convertToConcreteRecipes(llvm::VPlan&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38d5422)
 #9 0x00000000036dc59a llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x36dc59a)
```
See also: https://godbolt.org/z/6Yzf35ffG

## Comments

### Comment 1 - arcbbb

My investigation shows that wide-ptr-iv `EMIT ir<%phi401> = WIDEN-POINTER-INDUCTION` is used by `WIDEN-GEP Var[Inv] ir<%getelementptr440>`, so legalizeAndOptimizeInductions skips this ptr-iv.
When removeBranchOnConst deletes the exit block,  %getelementptr440 becomes dead.
After dead recipe removal,  wide-ptr-iv has a remaining scalar use.

We may want to revisit the pass ordering in VPlanTransforms::optimize(). consider moving removeBranchOnConst earlier in the pipeline?


---

### Comment 2 - arcbbb

CC: @fhahn 

---

### Comment 3 - fhahn

This should be fixed by https://github.com/llvm/llvm-project/pull/169338

---

