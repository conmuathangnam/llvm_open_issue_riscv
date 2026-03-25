# loop-vectorize miscompile

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/142957
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-06-20T12:05:20Z

## Body

llvm commit: 6955a7d134e7
Reproduce with:
```
opt -passes="loop-vectorize" bbi-107525_lv.ll -S -o - -mtriple=aarch64-none-linux-gnu --data-layout="p:16:16"
```
The loop-vectorize output contains
```
pred.sdiv.continue:                               ; preds = %pred.sdiv.if, %vector.body
  %3 = phi i16 [ poison, %vector.body ], [ %2, %pred.sdiv.if ]
  [...]

pred.sdiv.continue2:                              ; preds = %pred.sdiv.if1, %pred.sdiv.continue
  [...]
  %7 = shl i16 %3, 14
  %8 = sub i16 %6, %7
  %9 = getelementptr [16 x i64], ptr @g_855, i16 0, i16 %8
  %10 = getelementptr i64, ptr %9, i32 0
  %wide.load = load <2 x i64>, ptr %10, align 1
```

and the problem here is that we will get to the %3 phi from %vector.body and then continue to pred.sdiv.continue2 where the %3 poison value will make %7, %8, %9 and %10 poison, and then we will do a load from %10 which is instant UB.

The input program has no UB.

Before 0d61ffd350
```
[Loads] Support SCEVAddExpr as start for pointer AddRec.
```
the result is ok. Then with 0d61ffd350 it crashes with
```
opt: ../include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<llvm::Instruction, const llvm::Value *>::doit(const From *) [To = llvm::Instruction, From = const llvm::Value *]: Assertion `Val && "isa<> used on a null pointer"' failed.
```
and that crash goes away with fd82b5b287
```
[LV] Support recieps without underlying instr in collectPoisonGenRec.

Support recipes without underlying instruction in
collectPoisonGeneratingRecipes by directly trying to dyn_cast_or_null
the underlying value.

Fixes https://github.com/llvm/llvm-project/issues/70590.
```
but then we get the miscompile instead.

I also tested to cherry-pick the fd82b5b287 to directly after 0d61ffd350 and we get the miscompile then too, so it doesn't seem to be some other patch between them that causes the miscompile.

[bbi-107525_lv.ll.gz](https://github.com/user-attachments/files/20610502/bbi-107525_lv.ll.gz)

## Comments

### Comment 1 - fhahn

Put up a potential fix: https://github.com/llvm/llvm-project/pull/143204

---

