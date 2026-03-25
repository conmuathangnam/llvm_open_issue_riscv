# [AggressiveInstCombine] Invalid trunc creation in store merge optimization

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/181117
**Status:** Closed
**Labels:** release:backport, crash, llvm:instcombine
**Closed Date:** 2026-02-12T13:41:01Z

## Body

```llvm
target triple = "x86_64-unknown-linux-gnu"

define void @test(i56 %arg, ptr %p) {
  %lo = trunc i56 %arg to i32
  store i32 %lo, ptr %p, align 4
  %shr = lshr i56 %arg, 32
  %hi = trunc i56 %shr to i32
  %p.4 = getelementptr i8, ptr %p, i64 4
  store i32 %hi, ptr %p.4, align 4
  ret void
}
```

> opt: /home/npopov/repos/llvm-project/llvm/lib/IR/Instructions.cpp:3043: static CastInst *llvm::CastInst::Create(Instruction::CastOps, Value *, Type *, const Twine &, InsertPosition): Assertion `castIsValid(op, S, Ty) && "Invalid cast!"' failed.


## Comments

### Comment 1 - nikic

/cherry-pick 4167b285e228ee1cfff77f30eee66cbc7b4dd3db

---

### Comment 2 - llvmbot

/pull-request llvm/llvm-project#181140

---

