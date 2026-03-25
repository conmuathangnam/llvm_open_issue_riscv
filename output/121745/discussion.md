# [clang] Miscompilation at -O2/3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/121745
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-01-06T22:41:55Z

## Body

This code prints 8 at `-O0/1` and prints 1 at `-O2/3`:

```c
int printf(const char *, ...);
static char a;
static char *b = &a;
static int c;
short d;
void e() {
  short f[8];
  char **g[] = {&b, &b};
  c = 0;
  for (; c < 8; c = 81 + 462704684 + *b - 462704765 + c + 1)
    f[c] = 0;
  d = f[5];
}
int main() {
  e();
  printf("%d\n", c);
}
```

Compiler Explorer: https://godbolt.org/z/MEKz8oncP

Bisected to https://github.com/llvm/llvm-project/commit/df4a615c988f3ae56f7e68a7df86acb60f16493a, which was committed by @fhahn 

## Comments

### Comment 1 - dtcxzyw

Reduced testcase: https://alive2.llvm.org/ce/z/6Bg63G
```
; bin/opt -passes=loop-vectorize test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @src() {
entry:
  %f.i = alloca [8 x i16], align 16
  %conv.i = sext i8 0 to i32
  %invariant.op.i = add nsw i32 %conv.i, 1
  br label %for.body.i

for.body.i:                                       ; preds = %for.body.i, %entry
  %0 = phi i32 [ 0, %entry ], [ %add2.reass.i, %for.body.i ]
  %idxprom.i = sext i32 %0 to i64
  %arrayidx.i = getelementptr [8 x i16], ptr %f.i, i64 0, i64 %idxprom.i
  store i16 0, ptr %arrayidx.i, align 2
  %add2.reass.i = add i32 %invariant.op.i, %0
  %cmp.i = icmp slt i32 %add2.reass.i, 8
  br i1 %cmp.i, label %for.body.i, label %e.exit

e.exit:                                           ; preds = %for.body.i
  %add2.reass.i.lcssa = phi i32 [ %add2.reass.i, %for.body.i ]
  ret i32 %add2.reass.i.lcssa
}
```

---

### Comment 2 - fhahn

Looking into it now, thanks

---

### Comment 3 - yaoxinliu

Thank God! clang19 works as expected.

---

