# [VPlan][LoopVectorize] Potential issue in how replicate recipe is lowered to IR

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/115169
**Status:** Open
**Labels:** miscompilation, vectorizers

## Body

It's a bit of a tricky one since I can no longer reproduce it on `main` due to changes in 5a4c6f97997f3cdfa9d98f7f0b546e331ee9cc4a , but I believe they didn't actually fix the underlying problem, but just made it (almost?) impossible to trigger the conditions for it.
As a workaround, I've introduced a cl opt for testing that forces replicate recipe over widen recipe so this can be triggered for regular instructions (since it's not possible to trigger it for loads after above changes):
```patch
--- a/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp
+++ b/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp
@@ -357,6 +357,11 @@ static cl::opt<bool> PreferPredicatedReductionSelect(
     cl::desc(
         "Prefer predicating a reduction operation over an after loop select."));
;
 
+static cl::opt<bool> ForceReplicateOverWiden(
+    "force-replicate-over-widen", cl::init(false), cl::Hidden,
+    cl::desc("Force replicate recipies over widen recipes."
+             "This flag should only be used for testing."));
+
 namespace llvm {
 cl::opt<bool> EnableVPlanNativePath(
     "enable-vplan-native-path", cl::Hidden,
@@ -9148,8 +9153,10 @@ LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(VFRange &Range) {
         continue;
       }
 
-      VPRecipeBase *Recipe =
-          RecipeBuilder.tryToCreateWidenRecipe(Instr, Operands, Range, VPBB);
+      VPRecipeBase *Recipe = nullptr;
+      if (!ForceReplicateOverWiden || isa<PHINode>(Instr))
+        Recipe =
+            RecipeBuilder.tryToCreateWidenRecipe(Instr, Operands, Range, VPBB);
       if (!Recipe)
         Recipe = RecipeBuilder.handleReplication(Instr, Range);
```
With this patch and `-passes=loop-vectorize -force-vector-width=4 -force-replicate-over-widen` options passed to `opt` on a command line the following IR:
```llvm
define void @src(i1 %arg, i64 %arg1, ptr %arg2) {
bb:
  br label %bb3

bb3:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  br i1 %arg, label %bb6, label %bb4

bb4:                                              ; preds = %bb3
  %load = load i32, ptr  %arg2, align 8
  %add5 = add i32 %load, 1
  br label %bb6

bb6:                                              ; preds = %bb4, %bb3
  %phi7 = phi i32 [ %add5, %bb4 ], [ 0, %bb3 ]
  %add = add i64 %phi, 1
  %icmp = icmp samesign ult i64 %phi, %arg1
  br i1 %icmp, label %bb3, label %bb8

bb8:                                              ; preds = %bb6
  %phi9 = phi i32 [ %phi7, %bb6 ]
  ret void
}
```
Gets optimized to
```llvm
define void @src(i1 %arg, i64 %arg1, ptr %arg2) {
bb:
  %0 = add i64 %arg1, 1
  %min.iters.check = icmp ult i64 %0, 4
  br i1 %min.iters.check, label %scalar.ph, label %vector.ph

vector.ph:                                        ; preds = %bb
  %n.mod.vf = urem i64 %0, 4
  %n.vec = sub i64 %0, %n.mod.vf
  %broadcast.splatinsert = insertelement <4 x i1> poison, i1 %arg, i64 0
  %broadcast.splat = shufflevector <4 x i1> %broadcast.splatinsert, <4 x i1> poison, <4 x i32> zeroinitializer
  br label %vector.body

vector.body:                                      ; preds = %pred.load.continue6, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %pred.load.continue6 ]
  %1 = xor <4 x i1> %broadcast.splat, <i1 true, i1 true, i1 true, i1 true>
  %2 = extractelement <4 x i1> %1, i32 0
  br i1 %2, label %pred.load.if, label %pred.load.continue

pred.load.if:                                     ; preds = %vector.body
  %3 = load i32, ptr %arg2, align 8
  br label %pred.load.continue

pred.load.continue:                               ; preds = %pred.load.if, %vector.body
  %4 = phi i32 [ poison, %vector.body ], [ %3, %pred.load.if ]
  %5 = extractelement <4 x i1> %1, i32 1
  br i1 %5, label %pred.load.if1, label %pred.load.continue2

pred.load.if1:                                    ; preds = %pred.load.continue
  %6 = load i32, ptr %arg2, align 8
  br label %pred.load.continue2

pred.load.continue2:                              ; preds = %pred.load.if1, %pred.load.continue
  %7 = phi i32 [ poison, %pred.load.continue ], [ %6, %pred.load.if1 ]
  %8 = extractelement <4 x i1> %1, i32 2
  br i1 %8, label %pred.load.if3, label %pred.load.continue4

pred.load.if3:                                    ; preds = %pred.load.continue2
  %9 = load i32, ptr %arg2, align 8
  br label %pred.load.continue4

pred.load.continue4:                              ; preds = %pred.load.if3, %pred.load.continue2
  %10 = phi i32 [ poison, %pred.load.continue2 ], [ %9, %pred.load.if3 ]
  %11 = extractelement <4 x i1> %1, i32 3
  br i1 %11, label %pred.load.if5, label %pred.load.continue6

pred.load.if5:                                    ; preds = %pred.load.continue4
  %12 = load i32, ptr %arg2, align 8
  br label %pred.load.continue6

pred.load.continue6:                              ; preds = %pred.load.if5, %pred.load.continue4
  %13 = phi i32 [ poison, %pred.load.continue4 ], [ %12, %pred.load.if5 ]
  %14 = add i32 %4, 1
  %15 = add i32 %7, 1
  %16 = add i32 %10, 1
  %17 = add i32 %13, 1
  %18 = insertelement <4 x i32> poison, i32 %14, i32 0
  %19 = insertelement <4 x i32> %18, i32 %15, i32 1
  %20 = insertelement <4 x i32> %19, i32 %16, i32 2
  %21 = insertelement <4 x i32> %20, i32 %17, i32 3
  %predphi = select <4 x i1> %broadcast.splat, <4 x i32> zeroinitializer, <4 x i32> %21
  %index.next = add nuw i64 %index, 4
  %22 = icmp eq i64 %index.next, %n.vec
  br i1 %22, label %middle.block, label %vector.body, !llvm.loop !0

middle.block:                                     ; preds = %pred.load.continue6
  %23 = extractelement <4 x i32> %predphi, i32 3
  %cmp.n = icmp eq i64 %0, %n.vec
  br i1 %cmp.n, label %bb8, label %scalar.ph

scalar.ph:                                        ; preds = %middle.block, %bb
  %bc.resume.val = phi i64 [ %n.vec, %middle.block ], [ 0, %bb ]
  br label %bb3

bb3:                                              ; preds = %scalar.ph, %bb6
  %phi = phi i64 [ %add, %bb6 ], [ %bc.resume.val, %scalar.ph ]
  br i1 %arg, label %bb6, label %bb4

bb4:                                              ; preds = %bb3
  %load = load i32, ptr %arg2, align 8
  %add5 = add i32 %load, 1
  br label %bb6

bb6:                                              ; preds = %bb4, %bb3
  %phi7 = phi i32 [ %add5, %bb4 ], [ 0, %bb3 ]
  %add = add i64 %phi, 1
  %icmp = icmp samesign ult i64 %phi, %arg1
  br i1 %icmp, label %bb3, label %bb8, !llvm.loop !3

bb8:                                              ; preds = %middle.block, %bb6
  %phi9 = phi i32 [ %phi7, %bb6 ], [ %23, %middle.block ]
  ret void
}

!0 = distinct !{!0, !1, !2}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = !{!"llvm.loop.unroll.runtime.disable"}
!3 = distinct !{!3, !2, !1}
```
Note the `pred.load.continue6` BB:
```llvm
pred.load.continue6:                              ; preds = %pred.load.if5, %pred.load.continue4
  %13 = phi i32 [ poison, %pred.load.continue4 ], [ %12, %pred.load.if5 ]
  %14 = add i32 %4, 1
  %15 = add i32 %7, 1
  %16 = add i32 %10, 1
  %17 = add i32 %13, 1
  %18 = insertelement <4 x i32> poison, i32 %14, i32 0
  %19 = insertelement <4 x i32> %18, i32 %15, i32 1
  %20 = insertelement <4 x i32> %19, i32 %16, i32 2
  %21 = insertelement <4 x i32> %20, i32 %17, i32 3
  %predphi = select <4 x i1> %broadcast.splat, <4 x i32> zeroinitializer, <4 x i32> %21
  %index.next = add nuw i64 %index, 4
  %22 = icmp eq i64 %index.next, %n.vec
  br i1 %22, label %middle.block, label %vector.body, !llvm.loop !0
```

All those adds are operating on a `poison` coming from phi while the predicate/mask is applied AFTER the operation is done, not before.
Before 5a4c6f97997f3cdfa9d98f7f0b546e331ee9cc4a  this would happen for dereferenceable loads leading to load from poison ptr which is immediate ub. In a downstream project I have loads that are known to be dereferenceable unconditionally in the function body (so no ctx argument is needed) so for them this transformation still applies even after the above change.
To me it seems like the "correct" IR should've been to construct vector from %4, %7, %10, %13, then use it in a `predphi` and then do a vector (or scalar) add on that.

@fhahn , do you see any obvious problems with this? I've tried looking into why VPlan generates this IR but it's rather involved to my untrained eye.

## Comments

### Comment 1 - danilaml

That's how vplan looks for this loop:
```llvm
<x1> vector loop: {
  vector.body:
    EMIT vp<%3> = CANONICAL-INDUCTION ir<0>, vp<%index.next>
    EMIT vp<%4> = not ir<%arg>
  Successor(s): pred.load

  <xVFxUF> pred.load: {
    pred.load.entry:
      BRANCH-ON-MASK vp<%4>
    Successor(s): pred.load.if, pred.load.continue

    pred.load.if:
      REPLICATE ir<%load> = load ir<%arg2>
    Successor(s): pred.load.continue

    pred.load.continue:
      PHI-PREDICATED-INSTRUCTION vp<%5> = ir<%load>
    No successors
  }
  Successor(s): bb4.0

  bb4.0:
    REPLICATE ir<%add5> = add vp<%5>, ir<1>
    BLEND ir<%phi7> = ir<%add5> ir<0>/ir<%arg>
    EMIT vp<%index.next> = add nuw vp<%3>, vp<%0>
    EMIT branch-on-count vp<%index.next>, vp<%1>
  No successors
}
```
ir<%add5> is using phi-predicated instruction as it's operand but when bb4 gets lowered/vectorized it becomes the operand of predphi instead which is strange.

---

### Comment 2 - fhahn

`predphi` is generated for the `BLEND` and ` REPLICATE ir<%add5> = add vp<%5>, ir<1>` is for the add in `bb4`.

I think at least for the case with `add`, things are working as expected unless I am missing something. having adds of `poison` shouldn't be a problem, as those lanes will be masked out by the BLEND (`predphi`).

Now if we would execute a load unconditionally with a pointer that could be poison, that would be incorrect. What did the original test case look like? 

---

### Comment 3 - danilaml

@fhahn og test case had load instead of add there with downstream attribute on `%load = load ptr, ptr %arg2, align 8` (which loaded a ptr this time) saying that it's dereferenceable everywhere in the function body. Before 5a4c6f97997f3cdfa9d98f7f0b546e331ee9cc4a having `!nonnul !0, !align !1, !dereferenceable_or_null !2` was enough to trigger the issue (since the second load would be deemed safe to speculate so not in need of predication).
I think it's "fine" by coincidence, since add poison doesn't trigger immediate UB, but I didn't any checks/guards against the same expansion for any instruction that could (or maybe there are but they don't trigger for loads). Not sure there are any other IR instructions that could be vectorized and not predicated (maybe (s|u)div if isSafeToSpeculativelyExecuteWithOpcode didn't only handle the case of constant divisor and worked with assumes/ranges/other flags).

---

### Comment 4 - fhahn

@danilaml  something like the IR below? I *think* after looking at the code, we only allow widening if the instruction can be executed speculatively (`isSafeToExecuteSpeculatively` or for loads, stores and calls if no mask is required.  There is extra logic to not require masks for loads if they are aligned & dereferenceable in the loop. With https://github.com/llvm/llvm-project/commit/5a4c6f97997f3cdfa9d98f7f0b546e331ee9cc4a, we now correctly require the that load needs to be dereferenceable at entry to the loop header, as this is what is passed as context instruction.

I'll close this as working as expected but please re-open if I missed anything

```
define void @src(i1 %arg, i64 %arg1, ptr %arg2) {
bb:
  br label %bb3

bb3:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  br i1 %arg, label %bb6, label %bb4

bb4:                                              ; preds = %bb3
  %arg3 = load ptr, ptr %arg2, align 8, !dereferenceable_or_null !{i64 8}, !align !{i64 8}
  %load = load i32, ptr %arg3, align 8
  %add5 = add i32 %load, 1
  br label %bb6

bb6:                                              ; preds = %bb4, %bb3
  %phi7 = phi i32 [ %add5, %bb4 ], [ 0, %bb3 ]
  %add = add i64 %phi, 1
  %icmp = icmp samesign ult i64 %phi, 100
  br i1 %icmp, label %bb3, label %bb8

bb8:                                              ; preds = %bb6
  %phi9 = phi i32 [ %phi7, %bb6 ]
  ret void
}
```

---

### Comment 5 - danilaml

@fhahn Yes, like this one. Yes, it no longer shows the problem after https://github.com/llvm/llvm-project/commit/5a4c6f97997f3cdfa9d98f7f0b546e331ee9cc4a just as I wrote in my original post and my replies.

My point being - this fix seems unrelated and just hides the problem by making it's hard/impossible to execute problematic codepath - just like shown in the artificial example I proved with `add poison`.
Suppose we have a knowledge that loads (or at least this specific `%load`) are always dereferenceable (either by having a special metadata on it or the ptr being from a special address space. that's the behavior I essentially see downstream). LoopVectorizer would produce a load form poison, which is immediate UB. I believe it's wrong.

If you have a problem with the way I triggered issue for `add`, what about adding an option to treat loads as as always safe to speculatively execute? Then the IR in your comment would produce UB where original code had none.

To me it seems to me like if 1) there is a predicated load 2) which is used by non-predicated instruction that is replicated it leads to the replicated version using unmasked poison directly and "predication" happening on the result. Which is wrong if the instruction produces immediate UB on poison argument. However, I'm not sure this is still possible to trigger with current llvm because all such instructions will either be always predicated (udiv/sdiv as well as loads after the patch) or won't vectorize (various calls/invokes/ret), but it should be possible to trigger it with some custom modifications (or improving isSafeToSpeculate analysis).

---

### Comment 6 - danilaml

@fhahn to illustrate, with this simple patch (don't see anything conceptually wrong with it):
```diff
--- a/llvm/lib/Analysis/Loads.cpp
+++ b/llvm/lib/Analysis/Loads.cpp
@@ -172,6 +172,9 @@ static bool isDereferenceableAndAlignedPointer(
                                               Size, DL, CtxI, AC, DT, TLI,
                                               Visited, MaxDepth);
 
+  if (V->getType()->getPointerAddressSpace() == 42)
+    return true;
+
   if (CtxI) {
     /// Look through assumes to see if both dereferencability and alignment can
     /// be provent by an assume
```
For the following IR:
```llvm
define void @src(i1 %arg, i64 %arg1, ptr %arg2) {
bb:
  br label %bb3

bb3:                                              ; preds = %bb6, %bb
  %phi = phi i64 [ %add, %bb6 ], [ 0, %bb ]
  br i1 %arg, label %bb6, label %bb4

bb4:                                              ; preds = %bb3
  %arg3 = load ptr addrspace(42), ptr %arg2, align 8
  %load = load i32, ptr addrspace(42) %arg3, align 8
  %add5 = add i32 %load, 1
  br label %bb6

bb6:                                              ; preds = %bb4, %bb3
  %phi7 = phi i32 [ %add5, %bb4 ], [ 0, %bb3 ]
  %add = add i64 %phi, 1
  %icmp = icmp samesign ult i64 %phi, 100
  br i1 %icmp, label %bb3, label %bb8

bb8:                                              ; preds = %bb6
  %phi9 = phi i32 [ %phi7, %bb6 ]
  ret void
}
```
loop vectorizer produces the following result with immediate UB:
```llvm
define void @src(i1 %arg, i64 %arg1, ptr %arg2) {
bb:
  br i1 false, label %scalar.ph, label %vector.ph

vector.ph:                                        ; preds = %bb
  %broadcast.splatinsert = insertelement <4 x i1> poison, i1 %arg, i64 0
  %broadcast.splat = shufflevector <4 x i1> %broadcast.splatinsert, <4 x i1> poison, <4 x i32> zeroinitializer
  br label %vector.body

vector.body:                                      ; preds = %pred.load.continue6, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %pred.load.continue6 ]
  %0 = xor <4 x i1> %broadcast.splat, <i1 true, i1 true, i1 true, i1 true>
  %1 = extractelement <4 x i1> %0, i32 0
  br i1 %1, label %pred.load.if, label %pred.load.continue

pred.load.if:                                     ; preds = %vector.body
  %2 = load ptr addrspace(42), ptr %arg2, align 8
  br label %pred.load.continue

pred.load.continue:                               ; preds = %pred.load.if, %vector.body
  %3 = phi ptr addrspace(42) [ poison, %vector.body ], [ %2, %pred.load.if ]
  %4 = extractelement <4 x i1> %0, i32 1
  br i1 %4, label %pred.load.if1, label %pred.load.continue2

pred.load.if1:                                    ; preds = %pred.load.continue
  %5 = load ptr addrspace(42), ptr %arg2, align 8
  br label %pred.load.continue2

pred.load.continue2:                              ; preds = %pred.load.if1, %pred.load.continue
  %6 = phi ptr addrspace(42) [ poison, %pred.load.continue ], [ %5, %pred.load.if1 ]
  %7 = extractelement <4 x i1> %0, i32 2
  br i1 %7, label %pred.load.if3, label %pred.load.continue4

pred.load.if3:                                    ; preds = %pred.load.continue2
  %8 = load ptr addrspace(42), ptr %arg2, align 8
  br label %pred.load.continue4

pred.load.continue4:                              ; preds = %pred.load.if3, %pred.load.continue2
  %9 = phi ptr addrspace(42) [ poison, %pred.load.continue2 ], [ %8, %pred.load.if3 ]
  %10 = extractelement <4 x i1> %0, i32 3
  br i1 %10, label %pred.load.if5, label %pred.load.continue6

pred.load.if5:                                    ; preds = %pred.load.continue4
  %11 = load ptr addrspace(42), ptr %arg2, align 8
  br label %pred.load.continue6

pred.load.continue6:                              ; preds = %pred.load.if5, %pred.load.continue4
  %12 = phi ptr addrspace(42) [ poison, %pred.load.continue4 ], [ %11, %pred.load.if5 ]
  %13 = load i32, ptr addrspace(42) %3, align 8
  %14 = load i32, ptr addrspace(42) %6, align 8
  %15 = load i32, ptr addrspace(42) %9, align 8
  %16 = load i32, ptr addrspace(42) %12, align 8
  %17 = insertelement <4 x i32> poison, i32 %13, i32 0
  %18 = insertelement <4 x i32> %17, i32 %14, i32 1
  %19 = insertelement <4 x i32> %18, i32 %15, i32 2
  %20 = insertelement <4 x i32> %19, i32 %16, i32 3
  %21 = add <4 x i32> %20, <i32 1, i32 1, i32 1, i32 1>
  %predphi = select <4 x i1> %broadcast.splat, <4 x i32> zeroinitializer, <4 x i32> %21
  %index.next = add nuw i64 %index, 4
  %22 = icmp eq i64 %index.next, 100
  br i1 %22, label %middle.block, label %vector.body, !llvm.loop !0

middle.block:                                     ; preds = %pred.load.continue6
  %23 = extractelement <4 x i32> %predphi, i32 3
  br i1 false, label %bb8, label %scalar.ph

scalar.ph:                                        ; preds = %middle.block, %bb
  %bc.resume.val = phi i64 [ 100, %middle.block ], [ 0, %bb ]
  br label %bb3

bb3:                                              ; preds = %bb6, %scalar.ph
  %phi = phi i64 [ %add, %bb6 ], [ %bc.resume.val, %scalar.ph ]
  br i1 %arg, label %bb6, label %bb4

bb4:                                              ; preds = %bb3
  %arg3 = load ptr addrspace(42), ptr %arg2, align 8
  %load = load i32, ptr addrspace(42) %arg3, align 8
  %add5 = add i32 %load, 1
  br label %bb6

bb6:                                              ; preds = %bb4, %bb3
  %phi7 = phi i32 [ %add5, %bb4 ], [ 0, %bb3 ]
  %add = add i64 %phi, 1
  %icmp = icmp samesign ult i64 %phi, 100
  br i1 %icmp, label %bb3, label %bb8, !llvm.loop !3

bb8:                                              ; preds = %middle.block, %bb6
  %phi9 = phi i32 [ %phi7, %bb6 ], [ %23, %middle.block ]
  ret void
}

!0 = distinct !{!0, !1, !2}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = !{!"llvm.loop.unroll.runtime.disable"}
!3 = distinct !{!3, !2, !1}
```

---

### Comment 7 - danilaml

@fhahn Do you see any problems with the example above (i.e. why LV is correct in generating such IR)?

---

### Comment 8 - danilaml

@fhahn ping

---

### Comment 9 - danilaml

@fhahn ping?

---

### Comment 10 - fhahn


>To me it seems to me like if 1) there is a predicated load 2) which is used by non-predicated instruction that is replicated it leads to the replicated version using unmasked poison directly and "predication" happening on the result. Which is wrong if the instruction produces immediate UB on poison argument. However, I'm not sure this is still possible to trigger with current llvm because all such instructions will either be always predicated (udiv/sdiv as well as loads after the patch) or won't vectorize (various calls/invokes/ret), but it should be possible to trigger it with some custom modifications (or improving isSafeToSpeculate analysis).

@danilaml  yep my previous comment was mostly to the point that with current upstream LLVM IR I don't think the issue would reproduce.

For extensions like you suggested, the problem would exist. Do you have any ideas how we could test this in OSS LLVM? I could come up with a speculative fix, but it would be better to be able to have a test 

---

### Comment 11 - danilaml

@fhahn well, my first patch was an attempt at that, since there are already a couple of "testing only" options in LoopVectorize.cpp, however, since `add` and co do not produce immediate UB it can only be used to test against similar codegen, but not "incorrect" IR.

Maybe add an option to unconditionally speculate loads/all instructions/instructions from a set (not sure how detailed we want it to be) and use it here: https://github.com/llvm/llvm-project/blob/main/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp#L3428 ? Should achieve the same result as my patch Loads.cpp above (might need to adjust assert in `getDivRemSpeculationCost` if it's not limited to loads).

---

### Comment 12 - danilaml

@fhahn created a PoC here: https://github.com/llvm/llvm-project/pull/121154 . It's a bit hacky, but unfortunately haven't come up with a way to do it nicer (like address spaces example above) with changes limited only to LoopVectorize (and introducing changes/options to something like Loads.cpp to help test LoopVectorize seems a bit weird).

---

### Comment 13 - danilaml

@fhahn ping

---

### Comment 14 - danilaml

@fhahn ping

---

### Comment 15 - fhahn

@danilaml I think https://github.com/llvm/llvm-project/issues/142957 may be for a similar issue, but with a reproducer. Could you check if https://github.com/llvm/llvm-project/pull/143204 would fix your case as well?

---

### Comment 16 - danilaml

@fhahn looks really similar. Will be able to check it on Monday.

---

### Comment 17 - danilaml

@fhahn unfortunately it doesn't look like this fix changes anything for this issue. IIUC, it checks if there was any potential poisons feeding the operation in the loop, but in the case above, there was no poison in the original IR at all. poison was introduced by loop vectorizer itself by what I assume incorrect vplan lowering (it shouldn't create `load (phi addr, poison)`, i.e. for some reason phi-predicated load in vplan becomes unconditional in the IR),

---

