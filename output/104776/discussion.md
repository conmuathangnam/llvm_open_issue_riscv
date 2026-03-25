# [SelectionDAG] Incorrect handling of lifetimes with multiple objects

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/104776
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-07-28T08:15:52Z

## Body

The lowering in https://github.com/llvm/llvm-project/blob/0cc6b464f8adb739e6f9bcc9bda8cff9bb1f1c2a/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp#L7554-L7555 treats lifetime.start/end on an argument that has multiple underlying objects by performing a lifetime start/end on *each* of the objects.

So if you have something like this:
```llvm
define i64 @test(i1 %c, ptr %p) {
  %a = alloca i64
  %b = alloca i64
  %sel.b = select i1 %c, ptr %b, ptr %a
  call void @llvm.lifetime.start(i64 8, ptr %a)
  store i64 1, ptr %a
  call void @llvm.lifetime.end(i64 8, ptr %sel.b)
  call void @llvm.lifetime.start(i64 8, ptr %b)
  store i64 2, ptr %b
  store ptr %b, ptr %p ; prevent store from being optimized away
  %v = load i64, ptr %a
  call void @llvm.lifetime.end(i64 8, ptr %b)
  ret i64 %v
}
```
Then the lifetime.end on `%sel.b` will be lowered to a lifetime.end on *both* `%a` and `%b`. However, assuming `%c` is true at runtime, it only ends the lifetime of `%b` and the overall IR is well-defined.

The final assembly is incorrect as a result of stack coloring:
```asm
	movq	$1, -8(%rsp)
	movq	$2, -8(%rsp)
	leaq	-8(%rsp), %rax
	movq	%rax, (%rsi)
	movq	-8(%rsp), %rax
	retq
```


## Comments

### Comment 1 - topperc

@nikic what's the right way to fix this?

---

### Comment 2 - nikic

It's a bit tricky, because not emitting a lifetime marker can also be incorrect. The way things are now we should probably only emit DAG lifetime markers if we know that *all* lifetime markers in the function have a single alloca as underlying object.

The ideal fix would probably be to require that that lifetime.start and lifetime.end can only be used on alloca at the IR level...

---

### Comment 3 - karouzakisp

@nikic I think we can just remove the intrinsics lifetime.start and lifetime.end on instructions that are not alloca, at the IR level. Am I missing something?

---

### Comment 4 - dtcxzyw

> The ideal fix would probably be to require that that lifetime.start and lifetime.end can only be used on alloca at the IR level...

Agree. In my real-world dataset, the pointer operands of lifetime intrinsics can only be allocas
~~and `sret` pointer arguments~~.

---

### Comment 5 - nikic

> @nikic I think we can just remove the intrinsics lifetime.start and lifetime.end on instructions that are not alloca, at the IR level. Am I missing something?

If you drop something like `lifetime.start(c ? a : b)` then the lifetime of the variable may start too late. (It's likely that StackColoring will leave such cases alone, but it's still wrong.)

---

### Comment 6 - nikic

Fixed by https://github.com/llvm/llvm-project/pull/149310 by dint of making this IR ill-formed.

---

