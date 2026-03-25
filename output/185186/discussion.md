# [SLP] `mul/or/and` with LHS constant can be misvectorized into wrong `shl/sub` operand order

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/185186
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2026-03-09T20:17:41Z

## Body

Here is a miscompilation case when reviewing https://github.com/llvm/llvm-project/pull/180457:

```llvm
define i16 @src_sub_and() {
entry:
  %p = alloca [4 x i16], align 2
  %s = alloca [4 x i16], align 2
  store i16 3, ptr %p, align 2
  %p1 = getelementptr i16, ptr %p, i64 1
  store i16 2, ptr %p1, align 2
  %p2 = getelementptr i16, ptr %p, i64 2
  store i16 3, ptr %p2, align 2
  %p3 = getelementptr i16, ptr %p, i64 3
  store i16 4, ptr %p3, align 2

  %l0 = load i16, ptr %p, align 2
  %l1 = load i16, ptr %p1, align 2
  %l2 = load i16, ptr %p2, align 2
  %l3 = load i16, ptr %p3, align 2

  %mul0 = or i16 0, %l0
  %mul1 = shl i16 %l1, 5
  %mul2 = shl i16 %l2, 2
  %mul3 = shl i16 %l3, 3

  %s1 = getelementptr i16, ptr %s, i64 1
  %s2 = getelementptr i16, ptr %s, i64 2
  %s3 = getelementptr i16, ptr %s, i64 3

  store i16 %mul0, ptr %s, align 2
  store i16 %mul1, ptr %s1, align 2
  store i16 %mul2, ptr %s2, align 2
  store i16 %mul3, ptr %s3, align 2

  %res = load i16, ptr %s, align 2
  ret i16 %res
}

define i16 @main(i32 %argc, ptr %argv) {
entry:
  %r = call i16 @sub_and()
  ret i16 %r
}
```

With opt built on this patch, it's transformed into:

```llvm
define i16 @sub_and() {
entry:
  %p = alloca [4 x i16], align 2
  %s = alloca [4 x i16], align 2
  store <4 x i16> <i16 3, i16 2, i16 3, i16 4>, ptr %p, align 2
  %0 = load <4 x i16>, ptr %p, align 2
  %1 = shufflevector <4 x i16> %0, <4 x i16> <i16 0, i16 poison, i16 poison, i16 poison>, <4 x i32> <i32 4, i32 1, i32 2, i32 3>
  %2 = shufflevector <4 x i16> %0, <4 x i16> <i16 poison, i16 5, i16 2, i16 3>, <4 x i32> <i32 0, i32 5, i32 6, i32 7>
  %3 = shl <4 x i16> %1, %2
  store <4 x i16> %3, ptr %s, align 2
  %res = load i16, ptr %s, align 2
  ret i16 %res
}

define i16 @main(i32 %argc, ptr %argv) {
entry:
  %r = call i16 @sub_and()
  ret i16 %r
}
```

Ran lli on them, we got different results:

```sh
$ lli src.ll
$ echo $?
3
$ lli tgt.ll
$ echo $?
0
```

Alive2 proof: https://alive2.llvm.org/ce/z/QQxz8D

> Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**

The provided test case triggers the bug because it includes a scalar lane of the form `or i16 0, %l0`, which is an identity operation and should evaluate to `%l0`. In the Alive2 counterexample, `%l0 = 3`, so the source function returns `3`.

After SLP vectorization, this lane is rewritten into a `shl` lane, but the operands are constructed in the wrong order when the constant is originally on the left-hand side (`Pos == 0`). As a result, the transformation produces `shl i16 0, %l0` instead of the intended `shl i16 %l0, 0`.

This changes the semantics of the program: `or i16 0, 3 = 3`, but `shl i16 0, 3 = 0`. Alive2 confirms this mismatch directly, showing that the source returns `3` while the target returns `0`.

**Fix Weakness Analysis:**

The weakness in the fix is that it computes the replacement identity constant correctly, but does not preserve the correct operand order for non-commutative target opcodes such as `shl` and `sub`.

In particular, when the original constant is in operand 0 (`Pos == 0`), the helper returns `{RHS, LHS}` for most cases. This is incorrect for transformations like:

- `or 0, X` -> should become `shl X, 0`
- `and -1, X` -> should become `shl X, 0`
- `mul 2, X` -> should become `shl X, 1`

but the patched logic instead produces:

- `shl 0, X`
- `shl 0, X`
- `shl 1, X`

These are not equivalent to the original expressions. The Alive2 counterexample exposes exactly this flaw: the patch preserves the identity constant, but places it in the wrong operand position, making the transformation unsound.

cc @bababuck 

