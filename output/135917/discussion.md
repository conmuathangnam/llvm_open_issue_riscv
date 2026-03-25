# [CodeGen] Miscompilation at -Os

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/135917
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-04-16T11:34:12Z

## Body

Reproducer:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@j = global i32 0
@r = global ptr @j
@i = global i1 false
@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  %0 = icmp eq i8 0, 0
  br i1 %0, label %for.body.i.i, label %n.exit.i

for.body.i.i:                                     ; preds = %entry
  store i1 true, ptr @i, align 4
  br label %n.exit.i

n.exit.i:                                         ; preds = %for.body.i.i, %entry
  %1 = load ptr, ptr @r, align 8
  %2 = load i1, ptr @i, align 4
  %broadcast.splatinsert2 = insertelement <4 x i1> zeroinitializer, i1 %2, i64 0
  %3 = xor <4 x i1> %broadcast.splatinsert2, splat (i1 true)
  %not.broadcast.splat3 = shufflevector <4 x i1> %3, <4 x i1> zeroinitializer, <4 x i32> zeroinitializer
  %4 = zext <4 x i1> %not.broadcast.splat3 to <4 x i32>
  %5 = extractelement <4 x i32> %4, i64 0
  %6 = extractelement <4 x i32> %4, i64 1
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %n.exit.i
  %spec.select = select i1 false, i32 0, i32 %5
  store i32 %6, ptr %1, align 4
  %7 = icmp eq i32 0, 0
  br i1 %7, label %o.exit, label %vector.body

o.exit:                                           ; preds = %vector.body
  %8 = load i32, ptr @j, align 4
  %call1 = tail call i32 (ptr, ...) @printf(ptr @.str, i32 %8)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
```
> bin/llubi reduced.ll
0
> bin/lli reduced.ll
1
```
llvm version: ece10a64cb180ba931b60cbd939d80412973eada


## Comments

### Comment 1 - dtcxzyw

Further reduced:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@i = global i1 true
@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  %2 = load i1, ptr @i, align 4
  %broadcast.splatinsert2 = insertelement <4 x i1> poison, i1 %2, i64 0
  %3 = xor <4 x i1> %broadcast.splatinsert2, splat (i1 true)
  %not.broadcast.splat3 = shufflevector <4 x i1> %3, <4 x i1> poison, <4 x i32> zeroinitializer
  %4 = zext <4 x i1> %not.broadcast.splat3 to <4 x i32>
  %5 = extractelement <4 x i32> %4, i64 0
  %6 = extractelement <4 x i32> %4, i64 1
  %spec.select = add i32 %5, %6
  %call1 = tail call i32 (ptr, ...) @printf(ptr @.str, i32 %spec.select)
  ret i32 0
}

declare i32 @printf(ptr, ...)

```

---

### Comment 2 - fhahn

Is this a backend issue?

---

### Comment 3 - dtcxzyw

> Is this a backend issue?

Yes. Bisected to https://github.com/llvm/llvm-project/blob/11857bef8a5fdfb8ab65971c3da6593c6076ff62/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp#L26145-L26146. I am working on a fix.


---

### Comment 4 - dtcxzyw

Related patch: https://github.com/llvm/llvm-project/commit/17b4cacbd48b75a6a4c9dfe687dd39f5e88189c3.


---

### Comment 5 - RKSimon

@dtcxzyw We should probably go back to the first more restrictive version of my patch:
```cpp
if (isSplatValue(LHS, DemandedElts, UndefLHS, Depth + 1) &&
    isSplatValue(RHS, DemandedElts, UndefRHS, Depth + 1) &&
    (DemandedElts & UndefLHS) == (DemandedElts & UndefRHS)) {
  UndefElts = UndefLHS | UndefRHS;
  return true;
}
```

---

### Comment 6 - dtcxzyw

```
t34: v4i32 = BUILD_VECTOR t2, undef:i32, undef:i32, undef:i32
t30: v4i32 = BUILD_VECTOR Constant:i32<-1>, Constant:i32<-1>, Constant:i32<-1>, Constant:i32<-1>
t36: v4i32 = xor t34, t30
isSplatValue(t36, allowUndef = false) = true -> incorrect
```
Reduced test:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @test(i1 %2) {
entry:
  %broadcast.splatinsert2 = insertelement <4 x i1> poison, i1 %2, i64 0
  %3 = xor <4 x i1> %broadcast.splatinsert2, splat (i1 true)
  %not.broadcast.splat3 = shufflevector <4 x i1> %3, <4 x i1> poison, <4 x i32> zeroinitializer
  %4 = zext <4 x i1> %not.broadcast.splat3 to <4 x i32>
  %5 = extractelement <4 x i32> %4, i64 0
  %6 = extractelement <4 x i32> %4, i64 1
  %spec.select = add i32 %5, %6
  ret i32 %spec.select
}
```
@RKSimon Are you willing to make a quick fix? Thank you!


---

