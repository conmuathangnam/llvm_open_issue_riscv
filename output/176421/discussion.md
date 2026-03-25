# [WinEH] Incorrect block label reference in unwind table

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/176421
**Status:** Closed
**Labels:** llvm:codegen, miscompilation, release:backport, platform:windows
**Closed Date:** 2026-01-20T10:05:16Z

## Body

```llvm
target triple = "aarch64-pc-windows-msvc"

declare void @func()

define void @test(ptr %p) personality ptr @__CxxFrameHandler3 {
  %v0 = load i32, ptr %p
  %v1 = load i32, ptr %p
  %xor = xor i32 %v0, %v1
  %cmp = icmp eq i32 %xor, 0
  br i1 %cmp, label %exit, label %bb

bb:
  invoke void @func()
          to label %exit unwind label %unwind

unwind:
  %cp = cleanuppad within none []
  store volatile i32 0, ptr %p
  cleanupret from %cp unwind to caller

exit:
  ret void
}

declare i32 @__CxxFrameHandler3(...)
```
Produces something like this:
```
$stateUnwindMap$test:
	.word	-1                              // ToState
	.word	"?dtor$-1@?0?test@4HA"@IMGREL   // Action
```
Note the `?dtor$-1` referring to an unnumbered MBB. This ultimately causes linker errors.

Only observed on AArch64 Windows.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Nikita Popov (nikic)

<details>
```llvm
target triple = "aarch64-pc-windows-msvc"

declare void @<!-- -->func()

define void @<!-- -->test(ptr %p) personality ptr @<!-- -->__CxxFrameHandler3 {
  %v0 = load i32, ptr %p
  %v1 = load i32, ptr %p
  %xor = xor i32 %v0, %v1
  %cmp = icmp eq i32 %xor, 0
  br i1 %cmp, label %exit, label %bb

bb:
  invoke void @<!-- -->func()
          to label %unreachable unwind label %unwind

unreachable:
  unreachable

unwind:
  %cp = cleanuppad within none []
  store volatile i32 0, ptr %p
  cleanupret from %cp unwind to caller

exit:
  ret void
}

declare i32 @<!-- -->__CxxFrameHandler3(...)
```
Produces something like this:
```
$stateUnwindMap$test:
	.word	-1                              // ToState
	.word	"?dtor$-1@?0?test@<!-- -->4HA"@<!-- -->IMGREL   // Action
```
Note the `?dtor$-1` referring to an unnumbered MBB. This ultimately causes linker errors.

Only observed on AArch64 Windows.
</details>


---

### Comment 2 - nikic

cc @efriedma-quic @rnk @dpaoliello 

From a cursory look it looks like branch-folder eliminates the unwinding code path, but presumably it's still referenced by some exception handling state.

---

### Comment 3 - efriedma-quic

Yes, this is a use-after-free: CxxUnwindMap holds a pointer to the MachineBasicBlock after it's freed.  Exception-handling targets aren't marked "hasAddressTaken()", so branch folding can eliminate the block.  Don't know how useful it is to eliminate cleanup code this late, but it's what we currently do.

That said, if the code is is dead, we should be able to skip emitting the reference into the exception-handling tables.  Off the top of my head, I'm not sure if we can completely skip the table entries, or we need some sort of stub.  Or maybe we should mark the blocks hasAddressTaken just so we don't have to think about it.

---

### Comment 4 - rnk

Yeah, exception handling is fundamentally a side table full of labels. What I can recall thinking when putting these references in the side table was something like "surely no CodeGen pass will mess with EH constructs, we're done with IPO, this is surely safe", and there's always a counterexample.

I think once these tables get created, we create indices that point into them, so we need to emit the table entries, but we could emit null in place of the labels. That will probably upset some reverse engineering tools, for example, the folks who objected to the -1 byte label bias.

I think some of the SEH code uses MCSymbols to avoid this type of problem, but even if you avoid the direct use-after-free bug, you can very easily end up in a reference to a label that is never defined, so even if we did this, we'd want to force EHPad block emission. Also, the compiler output becomes less readable when you create redundant temporary labels, which made me prefer the MBB reference.

---

### Comment 5 - nikic

/cherry-pick 528bb2bedaa9e51c8078d41b977d266fa7d65ec7

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#176888

---

