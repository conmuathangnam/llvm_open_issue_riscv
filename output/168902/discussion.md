# [LoopVectorize] Miscompile after recent VPlan change

**Author:** danilaml
**URL:** https://github.com/llvm/llvm-project/issues/168902
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-11-21T08:14:47Z

## Body

After ef023cae388d7becd18df602cb2d77bdb3d59e55 the following snippet started miscompiling when optimized with `-passes=loop-vectorize,instcombine -mcpu=znver2`:

```llvm
target triple = "x86_64-unknown-linux-gnu"

define i32 @foo() {
entry:
  br label %loop

loop:                                          ; preds = %loop, %entry
  %indvars.iv248 = phi i64 [ 326, %entry ], [ %indvars.iv.next249, %loop ]
  %local = phi i64 [ -72, %entry ], [ %2, %loop ]
  %0 = trunc i64 %local to i8
  %1 = getelementptr i8, ptr null, i64 %indvars.iv248
  store i8 %0, ptr %1, align 1
  %2 = add nsw i64 %local, 4
  %indvars.iv.next249 = add i64 %indvars.iv248, -1
  %3 = icmp eq i64 %indvars.iv248, 0
  br i1 %3, label %exit, label %loop

exit:                             ; preds = %loop
  ret i32 0
}
```

You can see that with patch LLVM generates the following epilog vector bb:
```llvm
vec.epilog.vector.body:
  %index4 = phi i64 [ 256, %vec.epilog.ph ], [ %index.next7, %vec.epilog.vector.body ]
  %vec.ind5 = phi <32 x i8> [ <i8 52, i8 48, i8 44, i8 40, i8 36, i8 32, i8 28, i8 24, i8 20, i8 16, i8 12, i8 8, i8 4, i8 0, i8 -4, i8 -8, i8 -12, i8 -16, i8 -20, i8 -24, i8 -28, i8 -32, i8 -36, i8 -40, i8 -44, i8 -48, i8 -52, i8 -56, i8 -60, i8 -64, i8 -68, i8 -72>, %vec.epilog.ph ], [ <i8 -76, i8 -80, i8 -84, i8 -88, i8 -92, i8 -96, i8 -100, i8 -104, i8 -108, i8 -112, i8 -116, i8 -120, i8 -124, i8 -128, i8 -4, i8 -8, i8 -12, i8 -16, i8 -20, i8 -24, i8 -28, i8 -32, i8 -36, i8 -40, i8 -44, i8 -48, i8 -52, i8 -56, i8 -60, i8 -64, i8 -68, i8 -72>, %vec.epilog.vector.body ]
  %offset.idx = sub i64 326, %index4
  %7 = getelementptr i8, ptr null, i64 %offset.idx
  %8 = getelementptr i8, ptr %7, i64 -31
  store <32 x i8> %vec.ind5, ptr %8, align 1
  %index.next7 = add nuw i64 %index4, 32
  %9 = icmp eq i64 %index.next7, 320
  br i1 %9, label %vec.epilog.middle.block, label %vec.epilog.vector.body
```
While without it's
```llvm
vec.epilog.vector.body:
  %index4 = phi i64 [ 256, %vec.epilog.ph ], [ %index.next7, %vec.epilog.vector.body ]
  %vec.ind5 = phi <32 x i8> [ <i8 -72, i8 -68, i8 -64, i8 -60, i8 -56, i8 -52, i8 -48, i8 -44, i8 -40, i8 -36, i8 -32, i8 -28, i8 -24, i8 -20, i8 -16, i8 -12, i8 -8, i8 -4, i8 0, i8 4, i8 8, i8 12, i8 16, i8 20, i8 24, i8 28, i8 32, i8 36, i8 40, i8 44, i8 48, i8 52>, %vec.epilog.ph ], [ %vec.ind.next8, %vec.epilog.vector.body ]
  %offset.idx = sub i64 326, %index4
  %7 = getelementptr i8, ptr null, i64 %offset.idx
  %8 = getelementptr i8, ptr %7, i64 -31
  %reverse6 = shufflevector <32 x i8> %vec.ind5, <32 x i8> poison, <32 x i32> <i32 31, i32 30, i32 29, i32 28, i32 27, i32 26, i32 25, i32 24, i32 23, i32 22, i32 21, i32 20, i32 19, i32 18, i32 17, i32 16, i32 15, i32 14, i32 13, i32 12, i32 11, i32 10, i32 9, i32 8, i32 7, i32 6, i32 5, i32 4, i32 3, i32 2, i32 1, i32 0>
  store <32 x i8> %reverse6, ptr %8, align 1
  %index.next7 = add nuw i64 %index4, 32
  %vec.ind.next8 = xor <32 x i8> %vec.ind5, splat (i8 -128)
  %9 = icmp eq i64 %index.next7, 320
  br i1 %9, label %vec.epilog.middle.block, label %vec.epilog.vector.body
```
They are not equivalent after the first iteration. Alive2 link: https://alive2.llvm.org/ce/z/rrabgg

Godbolt link: https://godbolt.org/z/aoW3PM81G

## Comments

### Comment 1 - artagnon

Thanks, it looks like it's only safe to copy over nuw and not nsw -- will fix soon.

---

