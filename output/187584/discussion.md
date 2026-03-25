# [LoopVectorize] Crash due to division by zero

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/187584
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-03-23T17:22:08Z

## Body

@fhahn 

```llvm
$ cat /tmp/a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

define void @wombat() {
bb:
  br label %bb2

bb1:                                              ; preds = %bb6
  ret void

bb2:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  %phi3 = phi i64 [ %phi7, %bb6 ], [ 0, %bb ]
  br i1 true, label %bb4, label %bb6

bb4:                                              ; preds = %bb2
  br i1 false, label %bb6, label %bb5, !prof !0

bb5:                                              ; preds = %bb4
  %load = load i64, ptr null, align 8
  %call = call i64 @llvm.smin.i64(i64 %load, i64 %phi3)
  br label %bb6

bb6:                                              ; preds = %bb5, %bb4, %bb2
  %phi7 = phi i64 [ %phi3, %bb4 ], [ %call, %bb5 ], [ %phi3, %bb2 ]
  %add = add i64 %phi, 1
  %icmp = icmp eq i64 %phi, 1
  br i1 %icmp, label %bb1, label %bb2
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.smin.i64(i64, i64) #0

attributes #0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }

!0 = !{!"branch_weights", i32 -2147483648, i32 0}
```
```
$ opt -p loop-vectorize /tmp/a.ll -disable-output
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.                       
Stack dump:                                                                         
0.      Program arguments: build/rel/bin/opt -p loop-vectorize -disable-output /tmp/l                                                                                   
1.      Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "/tmp/l"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "_ZN8colossus12_GLOBAL__N_123ReplicatedStripeEncoder19UpdateWrite
rOffsetsEN4absl4SpanIKNS_13StripeEncoder10ChunkStateEEE.__uniq.24555151794247875860126112716577434111"                                    
 #0 0x00005618bf5d1d38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/lib/Support/Unix/Sig
nals.inc:880:13                                                                                                                                                         
 #1 0x00005618bf5cecf5 llvm::sys::RunSignalHandlers() /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/lib/Support/Signals.cpp:109:18
 #2 0x00005618bf5d2461 SignalHandler(int, siginfo_t*, void*) /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/lib/Support/Unix/Signals.inc:448:3
8                                                                                                                                                                       
 #3 0x00007fede8249df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)                                                                                                        
 #4 0x00005618c1222c41 operator/= /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/include/llvm/Support/InstructionCost.h:158:11                
 #5 0x00005618c1222c41 operator/= /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/include/llvm/Support/InstructionCost.h:164:11
 #6 0x00005618c1222c41 llvm::VPReplicateRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&) const /usr/local/google/home/aeubanks/repos/llvm-project2/build/re
l/../../llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:3620:12
 #7 0x00005618c1210fea llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) /usr/local/google/home/aeubanks/repos/llvm-project2/build/rel/../../llvm/lib/T
ransforms/Vectorize/VPlanRecipes.cpp:287:16
...
```

looks like division by zero introduced in https://github.com/llvm/llvm-project/pull/179129

## Comments

### Comment 1 - alexfh

Actually, this was introduced in 13a093b2b2508a9a26d47c9b719cffb68756bb99.

---

### Comment 2 - aeubanks

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

%struct.pluto = type { i8, i8, i64, i64 }

define void @eggs(ptr %arg) {
bb:
  br label %bb2

bb1:                                              ; preds = %bb6
  ret void

bb2:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  %phi3 = phi i64 [ %phi7, %bb6 ], [ 0, %bb ]
  br i1 true, label %bb4, label %bb6

bb4:                                              ; preds = %bb2
  br i1 false, label %bb6, label %bb5, !prof !0

bb5:                                              ; preds = %bb4
  %getelementptr = getelementptr %struct.pluto, ptr %arg, i64 %phi
  %load = load i64, ptr %getelementptr, align 8
  %call = call i64 @llvm.smin.i64(i64 %load, i64 %phi3)
  br label %bb6

bb6:                                              ; preds = %bb5, %bb4, %bb2
  %phi7 = phi i64 [ %phi3, %bb4 ], [ %call, %bb5 ], [ %phi3, %bb2 ]
  %add = add i64 %phi, 1
  %icmp = icmp eq i64 %phi, 1
  br i1 %icmp, label %bb1, label %bb2
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.smin.i64(i64, i64) #0

attributes #0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }

!0 = !{!"branch_weights", i32 -2147483648, i32 0}
```

repros for before #181572 with the same crash, the original repro probably just hit a different code path to get to the same crash

---

### Comment 3 - alanzhao1

It's a narrowing conversion bug; I have a fix: https://github.com/llvm/llvm-project/pull/187605

---

### Comment 4 - alexfh

> repros for before [#181572](https://github.com/llvm/llvm-project/pull/181572) with the same crash, the original repro probably just hit a different code path to get to the same crash

I've bisected this to 1dcaf4793a150475d84114cb865d95564ff6d6e5. 

---

