# [clang] Miscompilation at -O2/3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/122496
**Status:** Closed
**Labels:** regression, miscompilation, vectorizers
**Closed Date:** 2025-01-12T22:03:43Z

## Body

This code prints 0 at `-O0/1` and triggers SIGKILL at `-O2/3`:

```c
int printf(const char *, ...);
int a;
short b, c;
long e;
int f[8][1];
unsigned g;
int h(int i) {
  long d = 0;
  for (; (a -= i) >= 0; d += 6)
    ;
  return d;
}
void j() {
  g = 0;
  for (; h(90) + g <= 0; g++) {
    int k = -1;
    b = 0;
    for (; k + g - -1 + b <= 3; b++)
      f[b + 3][0] = c;
    for (; b + g - 3 + e <= 8; e++)
      ;
    for (; e <= 3;)
      ;
  }
}
int main() {
  j();
  printf("%d\n", f[0][0]);
}
```

Compiler Explorer: https://godbolt.org/z/3x8Yc3fnW

It seems to be a recent regression. 

## Comments

### Comment 1 - fhahn

Bisecting now

---

### Comment 2 - dtcxzyw

Reproducer: https://godbolt.org/z/6bzT9rs3K
```
; bin/opt -passes=loop-vectorize test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@f = global [8 x [1 x i32]] zeroinitializer

define i32 @main() {
entry:
  br label %for.body6.i

for.body6.i:                                      ; preds = %for.body6.i, %entry
  %conv34.i = phi i32 [ %conv.i, %for.body6.i ], [ 0, %entry ]
  %storemerge3133.i = phi i16 [ %inc.i, %for.body6.i ], [ 0, %entry ]
  %idxprom.i = zext i32 %conv34.i to i64
  %arrayidx.i = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %idxprom.i
  store i32 0, ptr %arrayidx.i, align 4
  %inc.i = add i16 %storemerge3133.i, 1
  %conv.i = zext i16 %inc.i to i32
  %cmp4.i = icmp ult i16 %storemerge3133.i, 3
  br i1 %cmp4.i, label %for.body6.i, label %for.cond11thread-pre-split.i

for.cond11thread-pre-split.i:                     ; preds = %for.body6.i
  %0 = add i32 %conv.i, -9
  %cmp24.i = icmp ult i32 %0, -6
  br i1 %cmp24.i, label %for.cond23.i, label %for.end27.i

for.cond23.i:                                     ; preds = %for.cond23.i, %for.cond11thread-pre-split.i
  br label %for.cond23.i

for.end27.i:                                      ; preds = %for.cond11thread-pre-split.i
  ret i32 0
}
```
[llubi](https://github.com/dtcxzyw/llvm-ub-aware-interpreter) output:

Before:
```
Entering function main
  br label %for.body6.i jump to %for.body6.i
    phi i32 %conv34.i -> i32 0
    phi i16 %storemerge3133.i -> i16 0
  %idxprom.i = zext i32 %conv34.i to i64 -> i64 0
  %arrayidx.i = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %idxprom.i -> Ptr 16[@f]
  store i32 0, ptr %arrayidx.i, align 4
  %inc.i = add i16 %storemerge3133.i, 1 -> i16 1
  %conv.i = zext i16 %inc.i to i32 -> i32 1
  %cmp4.i = icmp ult i16 %storemerge3133.i, 3 -> T
  br i1 %cmp4.i, label %for.body6.i, label %for.cond11thread-pre-split.i jump to %for.body6.i
    phi i32 %conv34.i -> i32 1
    phi i16 %storemerge3133.i -> i16 1
  %idxprom.i = zext i32 %conv34.i to i64 -> i64 1
  %arrayidx.i = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %idxprom.i -> Ptr 20[@f + 4]
  store i32 0, ptr %arrayidx.i, align 4
  %inc.i = add i16 %storemerge3133.i, 1 -> i16 2
  %conv.i = zext i16 %inc.i to i32 -> i32 2
  %cmp4.i = icmp ult i16 %storemerge3133.i, 3 -> T
  br i1 %cmp4.i, label %for.body6.i, label %for.cond11thread-pre-split.i jump to %for.body6.i
    phi i32 %conv34.i -> i32 2
    phi i16 %storemerge3133.i -> i16 2
  %idxprom.i = zext i32 %conv34.i to i64 -> i64 2
  %arrayidx.i = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %idxprom.i -> Ptr 24[@f + 8]
  store i32 0, ptr %arrayidx.i, align 4
  %inc.i = add i16 %storemerge3133.i, 1 -> i16 3
  %conv.i = zext i16 %inc.i to i32 -> i32 3
  %cmp4.i = icmp ult i16 %storemerge3133.i, 3 -> T
  br i1 %cmp4.i, label %for.body6.i, label %for.cond11thread-pre-split.i jump to %for.body6.i
    phi i32 %conv34.i -> i32 3
    phi i16 %storemerge3133.i -> i16 3
  %idxprom.i = zext i32 %conv34.i to i64 -> i64 3
  %arrayidx.i = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %idxprom.i -> Ptr 28[@f + 12]
  store i32 0, ptr %arrayidx.i, align 4
  %inc.i = add i16 %storemerge3133.i, 1 -> i16 4
  %conv.i = zext i16 %inc.i to i32 -> i32 4
  %cmp4.i = icmp ult i16 %storemerge3133.i, 3 -> F
  br i1 %cmp4.i, label %for.body6.i, label %for.cond11thread-pre-split.i jump to %for.cond11thread-pre-split.i
  %0 = add i32 %conv.i, -9 -> i32 -5
  %cmp24.i = icmp ult i32 %0, -6 -> F
  br i1 %cmp24.i, label %for.cond23.i, label %for.end27.i jump to %for.end27.i
  ret i32 0
Exiting function main
```
After:
```
Entering function main
  br i1 false, label %scalar.ph, label %vector.ph jump to %vector.ph
  br label %vector.body jump to %vector.body
  %0 = zext i32 0 to i64 -> i64 0
  %1 = getelementptr [8 x [1 x i32]], ptr @f, i64 0, i64 %0 -> Ptr 16[@f]
  %2 = getelementptr i32, ptr %1, i32 0 -> Ptr 16[@f]
  store <4 x i32> zeroinitializer, ptr %2, align 4
  %3 = add i16 0, 1 -> i16 1
  %4 = zext i16 %3 to i32 -> i32 1
  %5 = zext i16 %3 to i32 -> i32 1
  %6 = zext i16 %3 to i32 -> i32 1
  %7 = zext i16 %3 to i32 -> i32 1
  br label %middle.block jump to %middle.block
  br i1 true, label %for.cond11thread-pre-split.i, label %scalar.ph jump to %for.cond11thread-pre-split.i
    phi i32 %conv.i.lcssa -> i32 1
  %8 = add i32 %conv.i.lcssa, -9 -> i32 -8
  %cmp24.i = icmp ult i32 %8, -6 -> T
  br i1 %cmp24.i, label %for.cond23.i.preheader, label %for.end27.i jump to %for.cond23.i.preheader
  br label %for.cond23.i jump to %for.cond23.i
  br label %for.cond23.i jump to %for.cond23.i
  br label %for.cond23.i jump to %for.cond23.i
  ...
```


---

