# Assertion "VPlan cost model and legacy cost model disagreed" when building Clang/LLVM for rv32gcv

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/175909
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, crash-on-valid
**Closed Date:** 2026-01-20T08:00:07Z

## Body

Attempting to cross-compile Clang/LLVM targeting rv32gcv with recent HEAD we get some crashes from the "VPlan cost model and legacy cost model disagreed" assert in LoopVectorize.cpp.

Here is a reduced test case:
```
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

define ptr @_ZSt23__copy_move_backward_a2ILb1EPSt4pairIdyES2_ET1_T0_S4_S3_(ptr %__first, ptr %__last) {
entry:
  %__last.addr1 = alloca ptr, align 4
  %__result.addr = alloca ptr, align 4
  store ptr %__first, ptr %__result.addr, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = phi ptr [ %.pre, %while.body ], [ %__last, %entry ]
  %cmp.not = icmp eq ptr %__first, %0
  br i1 %cmp.not, label %while.end, label %while.body

while.body:                                       ; preds = %while.cond
  %incdec.ptr = getelementptr i8, ptr %0, i32 -16
  store ptr %incdec.ptr, ptr %__last.addr1, align 4
  %1 = load ptr, ptr %__result.addr, align 4
  %incdec.ptr1 = getelementptr i8, ptr %1, i32 -16
  store ptr %incdec.ptr1, ptr %__result.addr, align 4
  %2 = call ptr @_ZSt12__assign_oneILb1EPSt4pairIdyES2_EvRT0_RT1_(ptr %__result.addr, ptr %__last.addr1)
  %.pre = load ptr, ptr %__last.addr1, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret ptr null
}

define ptr @_ZSt12__assign_oneILb1EPSt4pairIdyES2_EvRT0_RT1_(ptr %__out, ptr %__in) {
entry:
  %0 = load ptr, ptr %__in, align 4
  %1 = load double, ptr %0, align 8
  %2 = load ptr, ptr %__out, align 4
  store double %1, ptr %2, align 8
  %second3.i = getelementptr i8, ptr %2, i32 8
  store i64 0, ptr %second3.i, align 8
  ret ptr null
}
```

Reproduce with `./build/rvrel/bin/clang++ --target=riscv32-linux-gnu -c -march=rv32gcv -O3 reduced.ll`

Is this checking of the VPlan vs legacy cost model still needed at this point, or is the VPlan based model mature enough to remove it?

CC @lukel97 

## Comments

### Comment 1 - fhahn

@asb it looks like the march is not encoded in the IR, and passing `-march` to `opt` does not seem to work. What's the best way to run opt with the risc march? 

```
bin/opt -p loop-vectorize crash.ll -march=rv32gcv
bin/opt: WARNING: failed to create target machine for 'riscv32-unknown-linux-gnu': invalid target 'rv32gcv'.
```

---

### Comment 2 - lukel97

> [@asb](https://github.com/asb) it looks like the march is not encoded in the IR, and passing `-march` to `opt` does not seem to work. What's the best way to run opt with the risc march?
> 
> ```
> bin/opt -p loop-vectorize crash.ll -march=rv32gcv
> bin/opt: WARNING: failed to create target machine for 'riscv32-unknown-linux-gnu': invalid target 'rv32gcv'.
> ```

For RISC-V you can use `-mtriple riscv32 -mattr=+v`

---

### Comment 3 - asb

> [@asb](https://github.com/asb) it looks like the march is not encoded in the IR, and passing `-march` to `opt` does not seem to work. What's the best way to run opt with the risc march?
> 
> ```
> bin/opt -p loop-vectorize crash.ll -march=rv32gcv
> bin/opt: WARNING: failed to create target machine for 'riscv32-unknown-linux-gnu': invalid target 'rv32gcv'.
> ```

Using clang to build the .ll with the reproducer line in the report will work. But here's a version of the .ll extracted with `-print-before-all -print-module-scope` that you should be able to pass directly to `opt`:

```
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite)
define noalias noundef ptr @_ZSt23__copy_move_backward_a2ILb1EPSt4pairIdyES2_ET1_T0_S4_S3_(ptr writeonly captures(address) %__first, ptr readonly captures(address) %__last) local_unnamed_addr #0 {
entry:
  %cmp.not2 = icmp eq ptr %__first, %__last
  br i1 %cmp.not2, label %while.end, label %while.body.preheader

while.body.preheader:                             ; preds = %entry
  br label %while.body

while.body:                                       ; preds = %while.body.preheader, %while.body
  %0 = phi ptr [ %incdec.ptr, %while.body ], [ %__last, %while.body.preheader ]
  %__result.addr.03 = phi ptr [ %incdec.ptr1, %while.body ], [ %__first, %while.body.preheader ]
  %incdec.ptr = getelementptr i8, ptr %0, i32 -16
  %incdec.ptr1 = getelementptr i8, ptr %__result.addr.03, i32 -16
  %1 = load double, ptr %incdec.ptr, align 8
  store double %1, ptr %incdec.ptr1, align 8
  %second3.i.i = getelementptr i8, ptr %__result.addr.03, i32 -8
  store i64 0, ptr %second3.i.i, align 8
  %cmp.not = icmp eq ptr %__first, %incdec.ptr
  br i1 %cmp.not, label %while.end.loopexit, label %while.body

while.end.loopexit:                               ; preds = %while.body
  br label %while.end

while.end:                                        ; preds = %while.end.loopexit, %entry
  ret ptr null
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none)
define noalias noundef ptr @_ZSt12__assign_oneILb1EPSt4pairIdyES2_EvRT0_RT1_(ptr readonly captures(none) %__out, ptr readonly captures(none) %__in) local_unnamed_addr #1 {
entry:
  %0 = load ptr, ptr %__in, align 4
  %1 = load double, ptr %0, align 8
  %2 = load ptr, ptr %__out, align 4
  store double %1, ptr %2, align 8
  %second3.i = getelementptr i8, ptr %2, i32 8
  store i64 0, ptr %second3.i, align 8
  ret ptr null
}

attributes #0 = { nofree norecurse nosync nounwind memory(argmem: readwrite) }
attributes #1 = { mustprogress nofree norecurse nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, target_mem0: none, target_mem1: none) }
```

And to run with opt:
```sh
$ ./build/rvrel/bin/opt -passes=loop-vectorize -mtriple=riscv32 -mattr=+v crash2.ll -S
opt: ../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7271: VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./build/rvrel/bin/opt -passes=loop-vectorize -mtriple=riscv32 -mattr=+v crash2.ll -S
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "crash2.ll"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "_ZSt23__copy_move_backward_a2ILb1EPSt4pairIdyES2_ET1_T0_S4_S3_"
 #0 0x00005b8c987e31a6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/rvrel/bin/opt+0x23f61a6)
 #1 0x00005b8c987e05c5 llvm::sys::RunSignalHandlers() (./build/rvrel/bin/opt+0x23f35c5)
 #2 0x00005b8c987e42c4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000071734b63e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #4 0x000071734b69890c (/usr/lib/libc.so.6+0x9890c)
 #5 0x000071734b63e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #6 0x000071734b62557a abort (/usr/lib/libc.so.6+0x2557a)
 #7 0x000071734b6254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #8 0x00005b8c9a247d99 (./build/rvrel/bin/opt+0x3e5ad99)
 #9 0x00005b8c9a2597bd llvm::LoopVectorizePass::processLoop(llvm::Loop*) (./build/rvrel/bin/opt+0x3e6c7bd)
#10 0x00005b8c9a2640cb llvm::LoopVectorizePass::runImpl(llvm::Function&) (./build/rvrel/bin/opt+0x3e770cb)
#11 0x00005b8c9a2649d2 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./build/rvrel/bin/opt+0x3e779d2)
...
```

---

### Comment 4 - EugeneZelenko

@asb: Please also set `Type` to `Bug` in case of crashes. Potentially this task could be delegated to `llvm-bot` :-)

---

### Comment 5 - fhahn

This should probably be back-ported

---

### Comment 6 - lukel97

Making a note #175502 and #175506 made it into the 22 branch

---

### Comment 7 - lukel97

/cherry-pick 0c1257c

---

### Comment 8 - lukel97

I'm not sure if this is a regression to LLVM 21 btw because the release builds won't fire the legacy cost model assertions. But might be good to have LLVM 22 able to build Clang w/ rv32gcv.

---

### Comment 9 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: 0c1257c

https://github.com/llvm/llvm-project/actions/runs/21169374448

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 10 - lukel97

/cherry-pick 3ad6d350c44f54482a86a7eb488732093eaed372 0c1257cd46456513016b106d964dc5ad47c6289b

---

### Comment 11 - llvmbot

/pull-request llvm/llvm-project#176899

---

### Comment 12 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
Attempting to cross-compile Clang/LLVM targeting rv32gcv with recent HEAD we get some crashes from the "VPlan cost model and legacy cost model disagreed" assert in LoopVectorize.cpp.

Here is a reduced test case:
```
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

define ptr @<!-- -->_ZSt23__copy_move_backward_a2ILb1EPSt4pairIdyES2_ET1_T0_S4_S3_(ptr %__first, ptr %__last) {
entry:
  %__last.addr1 = alloca ptr, align 4
  %__result.addr = alloca ptr, align 4
  store ptr %__first, ptr %__result.addr, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = phi ptr [ %.pre, %while.body ], [ %__last, %entry ]
  %cmp.not = icmp eq ptr %__first, %0
  br i1 %cmp.not, label %while.end, label %while.body

while.body:                                       ; preds = %while.cond
  %incdec.ptr = getelementptr i8, ptr %0, i32 -16
  store ptr %incdec.ptr, ptr %__last.addr1, align 4
  %1 = load ptr, ptr %__result.addr, align 4
  %incdec.ptr1 = getelementptr i8, ptr %1, i32 -16
  store ptr %incdec.ptr1, ptr %__result.addr, align 4
  %2 = call ptr @<!-- -->_ZSt12__assign_oneILb1EPSt4pairIdyES2_EvRT0_RT1_(ptr %__result.addr, ptr %__last.addr1)
  %.pre = load ptr, ptr %__last.addr1, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret ptr null
}

define ptr @<!-- -->_ZSt12__assign_oneILb1EPSt4pairIdyES2_EvRT0_RT1_(ptr %__out, ptr %__in) {
entry:
  %0 = load ptr, ptr %__in, align 4
  %1 = load double, ptr %0, align 8
  %2 = load ptr, ptr %__out, align 4
  store double %1, ptr %2, align 8
  %second3.i = getelementptr i8, ptr %2, i32 8
  store i64 0, ptr %second3.i, align 8
  ret ptr null
}
```

Reproduce with `./build/rvrel/bin/clang++ --target=riscv32-linux-gnu -c -march=rv32gcv -O3 reduced.ll`

Is this checking of the VPlan vs legacy cost model still needed at this point, or is the VPlan based model mature enough to remove it?

CC @<!-- -->lukel97 
</details>


---

