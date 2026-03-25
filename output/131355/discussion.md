# Possible miscompile in SLP Vectorizer.

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/131355
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-03-14T19:54:03Z

## Body

For the following IR:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @foo(ptr %arg) #0 gc "statepoint-example" personality ptr null {
bb:
  %call = call noalias ptr null(i32 9, i64 16, i64 816, i64 400, i64 0)
  %icmp = icmp eq ptr %call, null
  %add = add <16 x i16> <i16 0, i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8, i16 9, i16 10, i16 11, i16 12, i16 13, i16 14, i16 15>, splat (i16 16)
  %and = and <16 x i64> zeroinitializer, splat (i64 1)
  %add1 = add nuw nsw i64 0, 48
  %select = select <16 x i1> zeroinitializer, <16 x i16> zeroinitializer, <16 x i16> zeroinitializer
  %add2 = add nsw <16 x i16> %select, splat (i16 -6124)
  %add3 = add <16 x i16> <i16 0, i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8, i16 9, i16 10, i16 11, i16 12, i16 13, i16 14, i16 15>, splat (i16 64)
  %sub = sub nsw <16 x i16> zeroinitializer, %add3
  %select4 = select <16 x i1> zeroinitializer, <16 x i16> %add3, <16 x i16> %sub
  %add5 = add <16 x i64> <i64 0, i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8, i64 9, i64 10, i64 11, i64 12, i64 13, i64 14, i64 15>, splat (i64 80)
  %fadd = fadd float 0.000000e+00, -1.580000e+02
  store float 0.000000e+00, ptr null, align 4
  %icmp6 = icmp eq i32 0, 0
  call void null()
  %mul = mul i32 0, 9
  br label %bb7

bb7:                                              ; preds = %bb27, %bb
  %phi = phi float [ 4.000000e+00, %bb ], [ 0.000000e+00, %bb27 ]
  %fadd8 = fadd float 0.000000e+00, 0.000000e+00
  %fadd9 = fadd float %phi, 1.000000e+00
  %fadd10 = fadd float %fadd9, 1.000000e+00
  %fadd11 = fadd float 0.000000e+00, 0.000000e+00
  %frem = frem float %fadd10, 7.000000e+00
  %call12 = call i32 @llvm.x86.sse.cvttss2si(<4 x float> zeroinitializer)
  switch i32 %call12, label %bb13 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb13:                                             ; preds = %bb7
  %fmul = fmul double 0.000000e+00, 1.220000e+02
  %mul14 = mul i32 0, -3
  %mul15 = mul i32 %mul14, -3
  %call16 = call i32 @llvm.x86.sse.cvttss2si(<4 x float> zeroinitializer)
  switch i32 %call16, label %bb17 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb17:                                             ; preds = %bb13
  %fadd18 = fadd float 0.000000e+00, 1.000000e+00
  %mul19 = mul i32 %mul15, -3
  br label %bb20

bb20:                                             ; preds = %bb17
  %fadd21 = fadd float %fadd18, 1.000000e+00
  switch i32 0, label %bb22 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb22:                                             ; preds = %bb20
  %fadd23 = fadd float %fadd21, 1.000000e+00
  %fadd24 = fadd float 0.000000e+00, 0.000000e+00
  %frem25 = frem float %fadd23, 7.000000e+00
  %fmul26 = fmul float %frem25, 5.000000e+00
  switch i32 0, label %bb27 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb27:                                             ; preds = %bb22
  %fadd28 = fadd float %fadd23, 1.000000e+00
  %fadd29 = fadd float %fadd24, 0.000000e+00
  br label %bb7

bb30:                                             ; preds = %bb22, %bb22, %bb22, %bb22, %bb22, %bb22, %bb22, %bb20, %bb20, %bb20, %bb20, %bb20, %bb20, %bb20, %bb13, %bb13, %bb13, %bb13, %bb13, %bb13, %bb13, %bb7, %bb7, %bb7, %bb7, %bb7, %bb7, %bb7
  %phi31 = phi float [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ %fadd10, %bb7 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd21, %bb20 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ], [ %fadd23, %bb22 ]
  %phi32 = phi float [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ %fadd11, %bb7 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb13 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ 0.000000e+00, %bb20 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ], [ %fadd24, %bb22 ]
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.sse.cvttss2si(<4 x float>) #1

; uselistorder directives
uselistorder ptr @llvm.x86.sse.cvttss2si, { 1, 0 }

attributes #0 = { "target-cpu"="znver2" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(none) }
````
`opt -passes=slp-vectorizer` miscompiled it by introducing load from `ptr poison` (see `bb22`):
```llvm
define void @foo(ptr %arg) #0 gc "statepoint-example" personality ptr null {
bb:
  %call = call noalias ptr null(i32 9, i64 16, i64 816, i64 400, i64 0)
  %icmp = icmp eq ptr %call, null
  %add = add <16 x i16> <i16 0, i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8, i16 9, i16 10, i16 11, i16 12, i16 13, i16 14, i16 15>, splat (i16 16)
  %and = and <16 x i64> zeroinitializer, splat (i64 1)
  %add1 = add nuw nsw i64 0, 48
  %select = select <16 x i1> zeroinitializer, <16 x i16> zeroinitializer, <16 x i16> zeroinitializer
  %add2 = add nsw <16 x i16> %select, splat (i16 -6124)
  %add3 = add <16 x i16> <i16 0, i16 1, i16 2, i16 3, i16 4, i16 5, i16 6, i16 7, i16 8, i16 9, i16 10, i16 11, i16 12, i16 13, i16 14, i16 15>, splat (i16 64)
  %sub = sub nsw <16 x i16> zeroinitializer, %add3
  %select4 = select <16 x i1> zeroinitializer, <16 x i16> %add3, <16 x i16> %sub
  %add5 = add <16 x i64> <i64 0, i64 1, i64 2, i64 3, i64 4, i64 5, i64 6, i64 7, i64 8, i64 9, i64 10, i64 11, i64 12, i64 13, i64 14, i64 15>, splat (i64 80)
  %fadd = fadd float 0.000000e+00, -1.580000e+02
  store float 0.000000e+00, ptr null, align 4
  %icmp6 = icmp eq i32 0, 0
  call void null()
  %mul = mul i32 0, 9
  br label %bb7

bb7:                                              ; preds = %bb27, %bb
  %phi = phi float [ 4.000000e+00, %bb ], [ 0.000000e+00, %bb27 ]
  %fadd8 = fadd float 0.000000e+00, 0.000000e+00
  %fadd9 = fadd float %phi, 1.000000e+00
  %0 = insertelement <2 x float> <float poison, float 0.000000e+00>, float %fadd9, i32 0
  %1 = fadd <2 x float> <float 1.000000e+00, float 0.000000e+00>, %0
  %2 = extractelement <2 x float> %1, i32 0
  %frem = frem float %2, 7.000000e+00
  %call12 = call i32 @llvm.x86.sse.cvttss2si(<4 x float> zeroinitializer)
  switch i32 %call12, label %bb13 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb13:                                             ; preds = %bb7
  %fmul = fmul double 0.000000e+00, 1.220000e+02
  %mul14 = mul i32 0, -3
  %mul15 = mul i32 %mul14, -3
  %call16 = call i32 @llvm.x86.sse.cvttss2si(<4 x float> zeroinitializer)
  switch i32 %call16, label %bb17 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb17:                                             ; preds = %bb13
  %fadd18 = fadd float 0.000000e+00, 1.000000e+00
  %mul19 = mul i32 %mul15, -3
  br label %bb20

bb20:                                             ; preds = %bb17
  %fadd21 = fadd float %fadd18, 1.000000e+00
  %3 = insertelement <2 x float> <float poison, float 0.000000e+00>, float %fadd21, i32 0
  switch i32 0, label %bb22 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb22:                                             ; preds = %bb20
  %4 = load <2 x float>, ptr poison, align 8
  %5 = fadd <2 x float> <float 1.000000e+00, float 0.000000e+00>, %4
  %6 = extractelement <2 x float> %5, i32 0
  %frem25 = frem float %6, 7.000000e+00
  %fmul26 = fmul float %frem25, 5.000000e+00
  switch i32 0, label %bb27 [
    i32 125, label %bb30
    i32 98, label %bb30
    i32 99, label %bb30
    i32 103, label %bb30
    i32 112, label %bb30
    i32 116, label %bb30
    i32 121, label %bb30
  ]

bb27:                                             ; preds = %bb22
  %fadd28 = fadd float %6, 1.000000e+00
  %7 = extractelement <2 x float> %5, i32 1
  %fadd29 = fadd float %7, 0.000000e+00
  br label %bb7

bb30:                                             ; preds = %bb22, %bb22, %bb22, %bb22, %bb22, %bb22, %bb22, %bb20, %bb20, %bb20, %bb20, %bb20, %bb20, %bb20, %bb13, %bb13, %bb13, %bb13, %bb13, %bb13, %bb13, %bb7, %bb7, %bb7, %bb7, %bb7, %bb7, %bb7
  %8 = phi <2 x float> [ %1, %bb7 ], [ %1, %bb7 ], [ %1, %bb7 ], [ %1, %bb7 ], [ %1, %bb7 ], [ %1, %bb7 ], [ %1, %bb7 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ zeroinitializer, %bb13 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %3, %bb20 ], [ %5, %bb22 ], [ %5, %bb22 ], [ %5, %bb22 ], [ %5, %bb22 ], [ %5, %bb22 ], [ %5, %bb22 ], [ %5, %bb22 ]
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.x86.sse.cvttss2si(<4 x float>) #1

attributes #0 = { "target-cpu"="znver2" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(none) }
```
https://godbolt.org/z/eqGe4PcbT

Note, that after https://github.com/llvm/llvm-project/commit/4959025bbc9e64cc58a1dfa0d9fe1c7162b3ade3 I wasn't able to reproduce the issue, but since this commit wasn't supposed to fix anything other than potential non-determinism, I decided to still report the issue.
In my investigations, I was able to reproduce the issue again with the above IR by reversing the order of iteration here: https://github.com/llvm/llvm-project/blob/main/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp#L14085
```cpp
    // Build a list of tree entries where V is used.
    SmallPtrSet<const TreeEntry *, 4> VToTEs;
    for (const TreeEntry *TEPtr : ValueToGatherNodes.lookup(V)) { // replaced with llvm::reverse(tmp)
      if (TEPtr == TE || TEPtr->Idx == 0)
        continue;
```
Which is likely due to this part being order-sensitive:
```cpp
      // The node is reused - exit.
      if (CheckAndUseSameNode(TEPtr))
        break;
```
I'm not sure if it's still possible to trigger this bug after the non-determinism patch since we now always iterate in the order of insertion meaning `TE`s will always have ascending `Idx` but It doesn't seem like it was intended side-effect of that patch.

## Comments

### Comment 1 - danilaml

@alexey-bataev please evaluate whether https://github.com/llvm/llvm-project/commit/4959025bbc9e64cc58a1dfa0d9fe1c7162b3ade3 is a correct fix for this miscompile (and then this can be closed) or if it just hidden it on accident. At the very least I think it needs to be documented that `isGatherShuffledSingleRegisterEntry` expects to iterate though nodes sorted by their `Idx` if the current code is OK.


Here is how I modified the snippet to trigger the miscompile on current main:
```cpp
    // Build a list of tree entries where V is used.
    SmallPtrSet<const TreeEntry *, 4> VToTEs;
    auto tmp = ValueToGatherNodes.lookup(V);
    for (const TreeEntry *TEPtr : llvm::reverse(tmp)) {
      if (TEPtr == TE || TEPtr->Idx == 0)
        continue;
```

---

### Comment 2 - alexey-bataev

Yes, it is correct fix. The non-determinism could trigger incorrect matching of the nodes and result in wrong code sequence.

---

### Comment 3 - danilaml

@alexey-bataev Thanks for confirming! Note, that as far as it appeared it was "deterministic" in a single compilation (and I was able to trigger the miscompile for the above example with 100% rate on several machines). So the actual issue was not the changing iteration order but just iteration order not matching the insertion order. Which was surprising since `isGatherShuffledSingleRegisterEntry` doesn't indicate that it has it as it's invariant. I'm pretty sure if the structure was replaced with some perfectly deterministic `SmallSetReverseVector` that acts just as SetVector but reverses the iteration order, everything will still be deterministic with miscompile still triggering.

---

### Comment 4 - alexey-bataev

> [@alexey-bataev](https://github.com/alexey-bataev) Thanks for confirming! Note, that as far as it appeared it was "deterministic" in a single compilation (and I was able to trigger the miscompile for the above example with 100% rate on several machines). So the actual issue was not the changing iteration order but just iteration order not matching the insertion order. Which was surprising since `isGatherShuffledSingleRegisterEntry` doesn't indicate that it has it as it's invariant. I'm pretty sure if the structure was replaced with some perfectly deterministic `SmallSetReverseVector` that acts just as SetVector but reverses the iteration order, everything will still be deterministic with miscompile still triggering.

The problem was iteration over unordered set, which could change with time, and the order may change. And it may affect the order during the analysis and during the codegen phases and cause matching for different nodes, which, in turn, could cause a miscompile

---

### Comment 5 - danilaml

@alexey-bataev I've modified `ValueToGatherNodes` so that when it's filled in `newTreeEntry` the `Last` is always inserted to the "front" of the vector.
```
for (Value *V : VL)
        if (!isConstant(V)) {
          auto *I = dyn_cast<CastInst>(V);
          AllConstsOrCasts &= I && I->getType()->isIntegerTy();
          if (UserTreeIdx.EdgeIdx != UINT_MAX || !UserTreeIdx.UserTE ||
              !UserTreeIdx.UserTE->isGather())
            ValueToGatherNodes.try_emplace(V).first->getSecond().insert_front(Last);
        }
```
So no non-determinism here, just all iteration on it would happen in reverse order of insertion but the miscompile reproduced. So at least "And it may affect the order during the analysis and during the codegen phases" part seems to not apply here. The order of iteration doesn't change throughout the compilation.

---

### Comment 6 - alexey-bataev

> [@alexey-bataev](https://github.com/alexey-bataev) I've modified `ValueToGatherNodes` so that when it's filled in `newTreeEntry` the `Last` is always inserted to the "front" of the vector.
> 
> ```
> for (Value *V : VL)
>         if (!isConstant(V)) {
>           auto *I = dyn_cast<CastInst>(V);
>           AllConstsOrCasts &= I && I->getType()->isIntegerTy();
>           if (UserTreeIdx.EdgeIdx != UINT_MAX || !UserTreeIdx.UserTE ||
>               !UserTreeIdx.UserTE->isGather())
>             ValueToGatherNodes.try_emplace(V).first->getSecond().insert_front(Last);
>         }
> ```
> 
> So no non-determinism here, just all iteration on it would happen in reverse order of insertion but the miscompile reproduced. So at least "And it may affect the order during the analysis and during the codegen phases" part seems to not apply here. The order of iteration doesn't change throughout the compilation.

Ok, I will doublecheck

---

### Comment 7 - danilaml

@alexey-bataev Thanks! That was a tricky one!

---

