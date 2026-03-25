# DSEPass incorrectly eliminates store to captured (and later read) pointer

**Author:** mhjacobson
**URL:** https://github.com/llvm/llvm-project/issues/140997

## Body

llvm v20.1.3
macOS "Big Sur" 11.7.10 (20G1427)

I have this code:

```
declare i32 @printf(i8* noalias readonly, ...)
@fmt = constant [6 x i8] c"%hhd\0a\00"

define ptr @makeValue(ptr %p) noinline optnone memory(none) {
  ret ptr %p
}

define void @useValue(ptr %value) noinline optnone memory(argmem: read) {
  %val = load i8, ptr %value
  call i32 @printf(ptr @fmt, i8 %val)
  ret void
}

define i32 @main() {
entry:
  %1 = alloca i8, align 4
  store i8 42, ptr %1
  %value = call i64 @makeValue(ptr %1)
  call void @useValue(i64 %value)
  ret i32 0
}
```

`opt --passes=dse` transforms `@main()` to (<https://godbolt.org/z/rhx1z9K4W>):

```
define i32 @main() {
entry:
  %0 = alloca i8, align 4
  %value = call i64 @makeValue(ptr %0)
  call void @useValue(i64 %value)
  ret i32 0
}
```

i.e., it eliminates the store to `%1`, even though `%1` is captured by the call to `@makeValue()` and ultimately read in `@useValue()`.

I believe the `memory` annotations on `@makeValue()` and `@useValue()` are legal.  Note that the `noinline optnone` are just to prevent the optimizer from seeing into the contents of the function, equivalent to if the functions were simply `declare`d and implemented in another module.

## Comments

### Comment 1 - mhjacobson

This change fixes the problem for me, but I'm not suggesting it's definitely the proper fix.

The first part of the change is just to get the call to `@makeValue()` to produce a MemorySSA entry; the second part is the real  meat of the change where we avoid DSE.

```diff
diff --git a/llvm/lib/Analysis/MemorySSA.cpp b/llvm/lib/Analysis/MemorySSA.cpp
index 1583e0e31efc..f8a9ce2a0473 100644
--- a/llvm/lib/Analysis/MemorySSA.cpp
+++ b/llvm/lib/Analysis/MemorySSA.cpp
@@ -1777,11 +1777,40 @@ MemoryUseOrDef *MemorySSA::createNewAccess(Instruction *I,
     }
   }
 
+  bool HasCaptureArg = false;
+#if 1
+  // For non-call instructions, if they neither read nor write, skip.
+  // For calls that do not access memory (memory(none)), model captures by pointer args.
+  if (auto *CB = dyn_cast<CallBase>(I)) {
+    // If this call is declared memory(none), only allow it through if it has
+    // a pointer argument without nocapture (i.e., it may capture).
+    if (CB->doesNotAccessMemory()) {
+      for (unsigned i = 0, e = CB->arg_size(); i != e; ++i) {
+        Value *Arg = CB->getArgOperand(i);
+
+        if (Arg->getType()->isPointerTy() &&
+            !CB->paramHasAttr(i, Attribute::NoCapture)) {
+          HasCaptureArg = true;
+          break;
+        }
+      }
+
+      if (!HasCaptureArg)
+        return nullptr;
+      // Otherwise let it through; we'll force a def below if AA indicates none.
+    }
+    // For calls that do access memory (readonly, readwrite, etc.), always model.
+  } else {
+    if (!I->mayReadFromMemory() && !I->mayWriteToMemory())
+      return nullptr;
+  }
+#else
   // Using a nonstandard AA pipelines might leave us with unexpected modref
   // results for I, so add a check to not model instructions that may not read
   // from or write to memory. This is necessary for correctness.
   if (!I->mayReadFromMemory() && !I->mayWriteToMemory())
     return nullptr;
+#endif
 
   bool Def, Use;
   if (Template) {
@@ -1813,7 +1842,7 @@ MemoryUseOrDef *MemorySSA::createNewAccess(Instruction *I,
     // we can precisely represent both "what memory will this read/write/is
     // clobbered by" and "what instructions can I move this past".
     Def = isModSet(ModRef) || isOrdered(I);
-    Use = isRefSet(ModRef);
+    Use = isRefSet(ModRef) || HasCaptureArg;
   }
 
   // It's possible for an instruction to not modify memory at all. During
diff --git a/llvm/lib/Transforms/Scalar/DeadStoreElimination.cpp b/llvm/lib/Transforms/Scalar/DeadStoreElimination.cpp
index 0fdc3354753b..f2facdd6e2c2 100644
--- a/llvm/lib/Transforms/Scalar/DeadStoreElimination.cpp
+++ b/llvm/lib/Transforms/Scalar/DeadStoreElimination.cpp
@@ -1367,6 +1367,19 @@ struct DSEState {
     if (auto SI = dyn_cast<StoreInst>(UseInst))
       return isStrongerThan(SI->getOrdering(), AtomicOrdering::Monotonic);
 
+    // Treat any call that captures a pointer arg as a read clobber.
+    if (auto *CB = dyn_cast<CallBase>(UseInst)) {
+      for (unsigned i = 0, e = CB->arg_size(); i != e; ++i) {
+        Value *Arg = CB->getArgOperand(i);
+
+        // Pointer arg without nocapture may alias the stored location.
+        if (Arg->getType()->isPointerTy() &&
+            !CB->paramHasAttr(i, Attribute::NoCapture)) {
+          return true;
+        }
+      }
+    }
+
     if (!UseInst->mayReadFromMemory())
       return false;
 
```

---

### Comment 2 - dtcxzyw

The signature of the callee is mismatched with the called function.
After fixing this problem, DSE works as expected: https://godbolt.org/z/WKj8ETqes

cc @nikic 


---

### Comment 3 - mhjacobson

Apologies — I was trying out a bunch of variants of this (attempting to bisect it down from where I originally encountered it).  I *think* this case is valid and still hits it: <https://godbolt.org/z/nd6x4naqr>

---

### Comment 4 - dtcxzyw

```
define void @useValue(i64 %value) noinline optnone memory(argmem: read) {
  %pointer = inttoptr i64 %value to ptr
  %val = load i8, ptr %pointer
  call i32 @printf(ptr @fmt, i8 %val)
  ret void
}
```

`memory(argmem: read)` indicates that the optimizer only takes **pointer** arguments into account. You should mark this function as `memory(read)`. Otherwise, it is UB.




---

### Comment 5 - mhjacobson

I see!  Looks like the relevant code is this:

```c++
    // Refine the modref info for argument memory. We only bother to do this
    // if ArgMR is not a subset of OtherMR, otherwise this won't have an impact
    // on the final result.
    ModRefInfo AllArgsMask = ModRefInfo::NoModRef;
    for (const auto &I : llvm::enumerate(Call->args())) {
      const Value *Arg = I.value();
      if (!Arg->getType()->isPointerTy())
        continue;
      unsigned ArgIdx = I.index();
      MemoryLocation ArgLoc = MemoryLocation::getForArgument(Call, ArgIdx, TLI);
      AliasResult ArgAlias = alias(ArgLoc, Loc, AAQI, Call);
      if (ArgAlias != AliasResult::NoAlias)
        AllArgsMask |= getArgModRefInfo(Call, ArgIdx);
    }
    ArgMR &= AllArgsMask;
```

In my frontend, I'm storing pointers in an aggregate type, which I then pass to `@useValue()`.  Marking `@useValue()` as `memory(read)` seems harsh, since it really does only read objects derived from that aggregate.

However, I agree that LangRef does define `argmem` as "This refers to accesses that are based on pointer arguments to the function."

---

### Comment 6 - dtcxzyw

As suggested by https://llvm.org/docs/Frontend/PerformanceTips.html#other-things-to-consider, please avoid using ptrtoint/inttoptr in the IR. It is harmful to pointer aliasing/capturing analysis.


---

### Comment 7 - mhjacobson

I'm not actually using ptrtoint in my frontend — I'm using an aggregate, like this: <https://godbolt.org/z/6PMKoh3dE> (also gets DSEed).

I simplified it to an i64 while reducing the test case, but I gather it would reproduce for any type can capture a pointer into a non-pointer.

---

### Comment 8 - nikic

See https://llvm.org/docs/Frontend/PerformanceTips.html#avoid-creating-values-of-aggregate-type. You almost certainly do not want to pass aggregates as a function argument.

But yes, argmem only refers to plain pointer arguments, not pointers that are part of aggregates.

---

### Comment 9 - mhjacobson

> You almost certainly do not want to pass aggregates as a function argument.

Just for my own edification: Why not?  That document doesn't (explicitly) proscribe it (and it does say that it's fine to *return* them).  In my case I specifically ensured the aggregate fits within two 64-bit values so that it can be passed/returned in registers easily.

Are there more reasons beyond this bug that passing aggregates is perilous?  I would figure that most analyses that would get confounded by the presence of an aggregate would be just as stumped by a pointer-to-aggregate.

In this case, it feels like we'd just be getting lucky that AA can't prove that the pointer-to-aggregate and the pointer-to-alloca don't alias (they don't, in fact), so it preserves the store.

And in fact, if I do prove it (by having `makeValue` simply return a `malloc()`ed pointer that stores the given pointer, then we *do* elide the store: <https://godbolt.org/z/a7voEn498>.

(Thanks to both of you for your help and patience!)

---

### Comment 10 - mhjacobson

> And in fact, if I do prove it (by having `makeValue` simply return a `malloc()`ed pointer that stores the given pointer, then we _do_ elide the store: https://godbolt.org/z/a7voEn498.

You can even pull `@makeValue()` out of the loop here: calling `@malloc()` directly from main also shows the problem: <https://godbolt.org/z/Ea8zKTa8s>.

---

### Comment 11 - nikic

> > You almost certainly do not want to pass aggregates as a function argument.
> 
> Just for my own edification: Why not? That document doesn't (explicitly) proscribe it (and it does say that it's fine to _return_ them). In my case I specifically ensured the aggregate fits within two 64-bit values so that it can be passed/returned in registers easily.
> 
> Are there more reasons beyond this bug that passing aggregates is perilous? I would figure that most analyses that would get confounded by the presence of an aggregate would be just as stumped by a pointer-to-aggregate.

I'm not saying you should pass it by pointer -- you can also pass it in unpacked form, with individual arguments for the aggregate members.

Using aggregates will *work*, but it will generally not optimize well, and we don't want to make them optimize well either. The general direction things are is to remove support for aggregates in most places instead, to remove this unnecessary footgun (but no active work has started on this yet).

> And in fact, if I do prove it (by having `makeValue` simply return a `malloc()`ed pointer that stores the given pointer, then we _do_ elide the store: https://godbolt.org/z/a7voEn498.

That's because you marked makeValue as `memory(inaccessiblemem: readwrite)`, but the function writes into memory it returns.

---

### Comment 12 - mhjacobson

>> And in fact, if I do prove it (by having makeValue simply return a malloc()ed pointer that stores the given pointer, then we do elide the store: https://godbolt.org/z/a7voEn498.
> That's because you marked makeValue as memory(inaccessiblemem: readwrite), but the function writes into memory it returns.

Is that not valid?  That's also how `@malloc()` and `@calloc()` themselves are marked (by `inferattrs` / `llvm::inferNonMandatoryLibFuncAttrs()`), and it seems (to me) to match the documentation of `inaccessiblemem`:

> `inaccessiblemem`: This refers to accesses to memory which is not accessible by the current module (before return from the function – an allocator function may return newly accessible memory while only accessing inaccessible memory itself). 

And as I say, the question can be side-stepped by using malloc directly: <https://godbolt.org/z/Ea8zKTa8s>.

---

### Comment 13 - nikic

> And as I say, the question can be side-stepped by using malloc directly: https://godbolt.org/z/Ea8zKTa8s.

Sorry, I didn't look at your example carefully enough. The actual problem here is that you have `argmem: read` even though it's loading from non-argument memory. `%pointer` is not [*based-on*](https://llvm.org/docs/LangRef.html#pointer-aliasing-rules) `%value`. The notion of argument memory is not recursive through memory.

---

