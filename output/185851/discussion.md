# [SLP] Assertion `MainOp && "MainOp cannot be nullptr."'

**Author:** arcbbb
**URL:** https://github.com/llvm/llvm-project/issues/185851
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-03-11T17:11:28Z

## Body

Reproducer: opt -p slp-vectorizer reduced.ll -slp-vectorize-non-power-of-2=true
```llvm
define i32 @clause_ImpliesFiniteDomain(ptr %Clause)  {
entry:
  %0 = getelementptr i8, ptr %Clause, i64 64
  %Clause.val = load i32, ptr %0, align 8
  %1 = getelementptr i8, ptr %Clause, i64 68
  %Clause.val51 = load i32, ptr %1, align 4
  %add.i = sub i32 0, %Clause.val
  br label %for.cond.preheader

for.cond.preheader:                               ; preds = %entry
  %2 = getelementptr i8, ptr %Clause, i64 72
  %add.i5667 = add i32 %Clause.val51, %Clause.val
  %Clause.val7.i68 = load i32, ptr %2, align 8
  %add3.i69 = add i32 %add.i5667, %Clause.val7.i68
  %cmp470 = icmp sgt i32 %add3.i69, 0
  ret i32 0
}
```
patch for reproducer
```
diff --git a/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp b/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp
index 0f7322a50865..38f5f3477c99 100644
--- a/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp
+++ b/llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp
@@ -17116,6 +17116,14 @@ InstructionCost BoUpSLP::calculateTreeCostAndTrimNonProfitable(

     InstructionCost C = getEntryCost(&TE, VectorizedVals, CheckedExtracts) *
                         getScaleToLoopIterations(TE);
+
+    if (VectorizableTree.size() == 2) {
+      if (TE.Idx == 0 && TE.Scalars.size() == 3)
+        C = InstructionCost(2);
+      else if (TE.Idx == 1 && TE.Scalars.size() == 2)
+        C = InstructionCost(4);
+    }
+
     Cost += C;
     NodesCosts.try_emplace(&TE, C);
     LLVM_DEBUG(dbgs() << "SLP: Adding cost " << C << " for bundle "
@@ -17328,6 +17336,12 @@ InstructionCost BoUpSLP::calculateTreeCostAndTrimNonProfitable(
                  isConstant(V) || isGathered(V) || getTreeEntries(V).size() > 1;
         }))
       GatherCost *= 2;
+
+    if (VectorizableTree.size() == 2 && TE->Idx == 1) {
+      TotalSubtreeCost = InstructionCost(13);
+      GatherCost = InstructionCost(10);
+    }
+
     // Erase subtree if it is non-profitable.
     if (TotalSubtreeCost > GatherCost) {
       // If the remaining tree is just a buildvector - exit, it will cause
```


## Comments

### Comment 1 - arcbbb

This is introduced after fc648683cd751d66ead63e84cab828b6a92de7d7

---

