# [LoopInterchange][DependenceAnalysis] Miscompilation on loops with anti-dependencies

**Author:** mshockwave
**URL:** https://github.com/llvm/llvm-project/issues/116144
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-01-31T09:36:26Z

## Body

Consider this simple function:
``` c
void anti(int *A, int x, unsigned N, unsigned M) {
  for (unsigned m = 0; m < M; ++m)
    for (unsigned i = 0U; i < N - 1; ++i) {
      A[i] = A[i + 1] + x;
    }
}
```
LoopInterchange currently kicks in on this loop and turns it into:
``` c
void anti(int *A, int x, unsigned N, unsigned M) {
  for (unsigned i = 0U; i < N - 1; ++i)
    for (unsigned m = 0; m < M; ++m) {
      A[i] = A[i + 1] + x;
    }
}
```
Which is apparently wrong.

To further illustrate the problem, one can run the function with this `main` function:
``` c
int main(int argc, char **argv) {
  int A[5] = {1, 2, 3, 4, 5};
  anti(A, argc + 2, 5, 3);

  for (unsigned i = 0U; i < 5; ++i) {
    printf(" %d", A[i]);
  }
  printf("\n");

  return 0;
}
```
The `-O0` program (w/ argc = 1) prints out " 13 14 11 8 5" while the incorrectly interchanged program prints " 5 6 7 8 5", hence the miscompile.

-----

The culprit of this problem comes from DependenceAnalysis, which provides a direction vector (between `A[i]` & `A[i + 1]`) of "[S <]" in this case, but I think it really should yield "[S >]" because we have an anti-dependency here. LoopInterchange should be able to stop doing any optimization when it sees "[S >]" as it violates the legality.

CC @sjoerdmeijer @Meinersbur @bmahjour 

## Comments

### Comment 1 - sjoerdmeijer

I haven't looked into details here, but from your description I suspect that this is probably a duplicate of  https://github.com/llvm/llvm-project/issues/54176. This reproducer is smaller and nicer, so thanks for that.

I am planning to look at #54176 and this one very soon, so will assign this to myself but let me know if you want to pick this up.

---

### Comment 2 - sebpop

You may want to check for 'S' scalar dependences in the loop interchange legality.
@sjoerdmeijer, as we spoke earlier today, please add a big FIXME comment in there until we get the code generator to properly handle scalar dependences.
Something like this should fix both bugs for now:
```
--- a/llvm/lib/Transforms/Scalar/LoopInterchange.cpp
+++ b/llvm/lib/Transforms/Scalar/LoopInterchange.cpp
@@ -182,13 +182,23 @@ static void interChangeDependencies(CharMatrix &DepMatrix, unsigned FromIndx,
 // if the direction matrix, after the same permutation is applied to its
 // columns, has no ">" direction as the leftmost non-"=" direction in any row.
 static bool isLexicographicallyPositive(std::vector<char> &DV) {
+  bool FirstDependenceIsFlow = false;
   for (unsigned char Direction : DV) {
-    if (Direction == '<')
-      return true;
-    if (Direction == '>' || Direction == '*')
+      // FIXME: Currently the code generator of loop interchange does not know
+      // how to generate correct code for scalar dependences.  Disable loop
+      // interchange here until the code generator knows how to deal with scalar
+      // dependences.
+    if (Direction == 'S')
       return false;
+
+    if (Direction == '<')
+      FirstDependenceIsFlow = true;
+
+    if (!FirstDependenceIsFlow)
+      if (Direction == '>' || Direction == '*')
+        return false;
   }
-  return true;
+  return FirstDependenceIsFlow;
 }
```

---

### Comment 3 - sebpop

> ```
>  for (unsigned m = 0; m < M; ++m)
>    for (unsigned i = 0U; i < N - 1; ++i) {
>      A[i] = A[i + 1] + x;
>    }
> ```
> [...]
> The culprit of this problem comes from DependenceAnalysis, which provides a direction vector (between A[i] & A[i + 1]) of "[S <]" in this case, but I think it really should yield "[S >]" because we have an anti-dependency here.

No @mshockwave, the dependence on the inner loop is correctly computed as flow '<'.
The dependence is carried by the outer loop and it is marked as a scalar dependence 'S'.
For instance: `A[1]` is written in iteration `(x,1)` and then read in iteration `(x+1,0)`. 
The dependence analysis is correct.


---

### Comment 4 - Meinersbur

Should have been fixed with #119345. Please reopen if persists.

---

