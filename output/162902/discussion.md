# [LV] Crash in VPPartialReductionRecipe::computeCost

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/162902
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-12-02T12:39:15Z

## Body

Compiling this:

```
char a, b;
char *c;
void d(bool e) {
  for (;;)
    for (int f; f < 2; f += e) {
      a = c[f];
      b += !f ?: e;
    }
}
```

with `clang -O2 -mcpu=grace` results in a crash with this backtrace:

```
 #4 0x0000000005837d75 llvm::VPPartialReductionRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5837d75)
 #5 0x0000000005841f46 llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5841f46)
 #6 0x0000000005807b57 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5807b57)
 #7 0x000000000580e888 llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x580e888)
 #8 0x000000000580f70d llvm::VPlan::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x580f70d)
 #9 0x00000000056bdbda llvm::LoopVectorizationPlanner::cost(llvm::VPlan&, llvm::ElementCount) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x56bdbda)
#10 0x00000000056c443a llvm::LoopVectorizationPlanner::computeBestVF() (.part.0) LoopVectorize.cpp:0:0
#11 0x00000000056c537a llvm::LoopVectorizationPlanner::computeBestVF() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x56c537a)
#12 0x00000000056ccee7 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x56ccee7)

```

See also: https://godbolt.org/z/njcnr58a4


## Comments

### Comment 1 - sjoerdmeijer

CC: @fhahn , @paulwalker-arm , @sdesmalen-arm , @david-arm 

---

### Comment 2 - sjoerdmeijer

IR Reproducer:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define void @_Z1db(i64 %0) #0 {
  br label %3

2:                                                ; preds = %3
  ret void

3:                                                ; preds = %3, %1
  %4 = phi i64 [ %0, %1 ], [ %8, %3 ]
  %5 = phi i8 [ 0, %1 ], [ %7, %3 ]
  %6 = zext i1 false to i8
  %7 = add i8 %5, %6
  store i8 %7, ptr null, align 1
  %8 = add i64 %4, 1
  %9 = icmp slt i64 %4, 0
  br i1 %9, label %3, label %2
}

attributes #0 = { "target-cpu"="grace" }
```

See also: https://godbolt.org/z/oTfe1KPcz

---

### Comment 3 - fhahn

This should be fixed on current main, due to some of the recent partial reduction changes.

---

