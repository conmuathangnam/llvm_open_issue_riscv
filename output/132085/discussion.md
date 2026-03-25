# [StackColoring] Incorrect slot merging due to stackcoloring-lifetime-start-on-first-use

**Author:** tmiasko
**URL:** https://github.com/llvm/llvm-project/issues/132085
**Status:** Open
**Labels:** llvm:codegen, miscompilation

## Body

In the program below, the allocation `a` and allocation `b` have overlapping
live ranges, so it should be impossible to observe them having the same
address. Nevertheless StackColoring merges stack slots for `a` and `b`.  This
is incorrect since addresses of those allocations might be captured by `g`.

```llvm
define void @f() {
start:
  %a = alloca [1000 x i8], align 1
  %b = alloca [1000 x i8], align 1
  call void @llvm.lifetime.start.p0(i64 1000, ptr %a)
  call void @llvm.lifetime.start.p0(i64 1000, ptr %b)
  call void @g(ptr %a)
  call void @llvm.lifetime.end.p0(i64 1000, ptr %a)
  call void @g(ptr %b)
  call void @llvm.lifetime.end.p0(i64 1000, ptr %b)
  ret void
}
declare void @g(ptr %n)
```

```console
$ llc-21 a.ll -print-before=stack-coloring -print-after=stack-coloring
# Machine code for function f: IsSSA, TracksLiveness
Frame Objects:
  fi#0: size=1000, align=1, at location [SP+8]
  fi#1: size=1000, align=1, at location [SP+8]
<snip>
# *** IR Dump After Merge disjoint stack slots (stack-coloring) ***:
# Machine code for function f: IsSSA, TracksLiveness
Frame Objects:
  fi#0: size=1000, align=1, at location [SP+8]
  fi#1: dead
<snip>
```

This is caused by stackcoloring-lifetime-start-on-first-use (the default) where
stack coloring shrinks live range towards first use.

## Comments

### Comment 1 - nikic

> In the program below, the allocation a and allocation b have overlapping live ranges, so it should be impossible to observe them having the same address.

Where is this requirement specified? I don't believe that overlapping lifetimes are intended to guarantee that the addresses differ. They should be consistently observed as equal or not equal at runtime, but I believe both outcomes are legal. (Similar to how e.g. two different functions may or may not compare equal.)

---

### Comment 2 - tmiasko

LangRef guarantees that pointers returned by alloca are unique, so if any, the
issue is explaining why it is possible to observe distinct allocas having the
same address.

Since the entire purpose of lifetime intrinsics is for them to be sufficient
for merging stack slots, it must be possible to observe distinct allocas having
the same address, and LangRef needs clarification.

Step back for a moment and consider the same function without any lifetime
intrinsics at all. In all possible executions addresses of `a` and `b` must
differ.  Now suppose the function is inlined. This introduces overlapping
lifetimes around the code, but the behavior must remain the same. Therefore in
executions where lifetimes are overlapping their addresses must differ.

Alive2 agrees that with overlapping lifetimes addresses must differ:
https://alive2.llvm.org/ce/z/W_FDbF

---

### Comment 3 - nikic

> LangRef guarantees that pointers returned by alloca are unique, so if any, the issue is explaining why it is possible to observe distinct allocas having the same address.
>
> Since the entire purpose of lifetime intrinsics is for them to be sufficient for merging stack slots, it must be possible to observe distinct allocas having the same address, and LangRef needs clarification.

Could you please point me to where LangRef guarantees that allocas can never have the same address? I couldn't find wording to that effect, and as you say, the presence of lifetime intrinsics would certainly make such a requirement incorrect.

> Step back for a moment and consider the same function without any lifetime intrinsics at all. In all possible executions addresses of `a` and `b` must differ. Now suppose the function is inlined. This introduces overlapping lifetimes around the code, but the behavior must remain the same. Therefore in executions where lifetimes are overlapping their addresses must differ.

I don't follow the example. If you inline the function in the issue description the lifetimes will be placed differently (the %a lifetime will not end early).

But in any case, that still assumes that "equal" and "not equal" cannot be both valid outcomes.

> Alive2 agrees that with overlapping lifetimes addresses must differ: https://alive2.llvm.org/ce/z/W_FDbF

When it comes to address identity, I would not blindly trust alive2 output. I've encountered may bugs it has in this particular area.

---

### Comment 4 - tmiasko

Do I understand correctly, that you are claiming that in the case without
lifetime intrinsics pointers to distinct non-zero sized allocations might
compare equal?

```llvm
define i1 @g() {
  %a = alloca ptr
  %b = alloca ptr
  %c = icmp eq ptr %a, %b
  ret i1 %c
}
```

I take it as a fundamental that allocated objects that are live at the same
time have disjoint storage (unless specified otherwise), so in `g` those
pointers always compare not equal. Furthermore programming languages like C
(6.5.9p7), C++, and Rust prescribe the outcome of such comparisons as not equal
as well.

I don't understand why equal would be a valid outcome, or how would C, C++, or
Rust be lowered to LLVM IR in that case.

Semantics section for `alloca` claims that "Allocating zero bytes is legal, but
the returned pointer may not be unique." What is point of describing the
exception for zero sized allocations unless there is a general rule that those
pointers are indeed unique (notwithstanding interaction with lifetime
intrinsics)?


---

### Comment 5 - nikic

> Do I understand correctly, that you are claiming that in the case without
> lifetime intrinsics pointers to distinct non-zero sized allocations might
> compare equal?

My general assumption here was that lifetime intrinsics are an optimization hint rather than a specification of when exactly allocation/deallocation *must* occur, so that it's possible to shrink the lifetime based on usage (e.g. by "sinking" the allocation point, as happens in the example from the issue description).

For your simplified example from the last comment, I don't think that can return true as both allocas are used in the icmp.

-----

Anyway, my assumption about how this is supposed to work is probably wrong. In any case, we should explicitly document the semantics in LangRef.

cc @efriedma-quic 

---

### Comment 6 - efriedma-quic

The semantics of lifetime intrinsics is a longstanding issue; see #45725.  At first glance, this is basically a duplicate.

---

### Comment 7 - tmiasko

A simpler demonstration of the issue, this time with proper nesting of lifetimes:

```llvm
define i32 @main() {
entry:
  %a = alloca [1000 x i8]
  %b = alloca [1000 x i8]
  call void @llvm.lifetime.start.p0(i64 1000, ptr %a)
  call void @llvm.lifetime.start.p0(i64 1000, ptr %b)
  call void @llvm.lifetime.end.p0(i64 1000, ptr %b)
  %0 = icmp eq ptr %a, %b
  call void @llvm.lifetime.end.p0(i64 1000, ptr %a)
  %1 = zext i1 %0 to i32
  ret i32 %1
}
```

```console
$ lli-21 -stackcoloring-lifetime-start-on-first-use=0 a.ll; echo $?
0
$ lli-21 -stackcoloring-lifetime-start-on-first-use=1 a.ll; echo $?
1
```


---

### Comment 8 - RalfJung

Whether or not two variables can be equal has to be tied to *some* clear property of the program. Maybe it's not lifetime intrinsics, but then what is it? The answer cannot be "look at how the variables are used" because there are optimizations that change how the variables are used, and if the syntactic position of the first and last use was somehow semantically relevant, then those optimizations would be wrong.

---

### Comment 9 - tmiasko

I was wondering whether this bug can also cause a situation where a stores into
one live alloca can clobber another live alloca. It turns out it can, although
reproducer is somewhat esoteric.

```llvm
define i32 @main() {
entry:
  %a = alloca i32
  %b = alloca i32
  %p = alloca ptr
  store ptr null, ptr %p
  br label %bb2
bb2:
  %i = phi i32 [ 2, %entry ], [ %2, %bb4 ]
  call void @llvm.lifetime.start.p0(i64 4, ptr %a)
  %0 = load ptr, ptr %p
  %1 = icmp eq ptr %0, null
  br i1 %1, label %bb4, label %bb3
bb3:
  ; Store 0 to `a` in the second iteration of the loop `bb2`.
  store i32 0, ptr %0
  br label %bb4
bb4:
  call void @llvm.lifetime.start.p0(i64 4, ptr %b)
  ; StackColoring incorrectly shares slot for %a and %b,
  ; so the following store to %b also clobbers %a.
  store volatile i32 42, ptr %b
  call void @llvm.lifetime.end.p0(i64 4, ptr %b)
  ; With stackcoloring-lifetime-start-on-first-use lifetime of a starts
  ; around here, which makes lifetimes %a and %b disjoint.
  store ptr %a,  ptr %p
  %r = load i32, ptr %a
  call void @llvm.lifetime.end.p0(i64 4, ptr %a)
  %2 = add nsw i32 %i, -1
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %bb5, label %bb2
bb5:
  ret i32 %r
}
```

The program always returns 0 (Alive2 demonstration https://alive2.llvm.org/ce/z/YEoPq9 I had to omit volatile). 

```
$ lli-21 --stackcoloring-lifetime-start-on-first-use=0 main.ll; echo $?
0
$ lli-21 --stackcoloring-lifetime-start-on-first-use=1 main.ll; echo $?
42
```


---

