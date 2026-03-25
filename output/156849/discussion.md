# [LoopVectorize] Miscompile with masked IV

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/156849
**Status:** Closed
**Labels:** miscompilation, release:backport, release:cherry-pick-failed, llvm:SCEV
**Closed Date:** 2025-09-05T15:13:12Z

## Body

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; out[i] = x[(i * 5) & 16]
define void @test(ptr noalias %x, ptr noalias %out) {
start:
  br label %loop

loop:
  %iv = phi i64 [ 0, %start ], [ %iv.next, %loop ]
  %iv.next = add nuw nsw i64 %iv, 1
  %iv.mul5 = mul nuw nsw i64 %iv, 5
  %iv.mul5.masked = and i64 %iv.mul5, 15
  %gep = getelementptr inbounds nuw i32, ptr %x, i64 %iv.mul5.masked
  %v = load i32, ptr %gep, align 4
  %out.i = getelementptr inbounds nuw i32, ptr %out, i64 %iv
  store i32 %v, ptr %out.i, align 4
  %exitcond.not = icmp eq i64 %iv.next, 16
  br i1 %exitcond.not, label %exit, label %loop

exit:
  ret void
}
```
Produces:
```llvm
define void @test(ptr noalias %x, ptr noalias %out) {
start:
  br i1 false, label %scalar.ph, label %vector.ph

vector.ph:
  br label %vector.body

vector.body:
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %0 = mul nuw nsw i64 %index, 5
  %1 = and i64 %0, 15
  %2 = getelementptr inbounds nuw i32, ptr %x, i64 %1
  %wide.vec = load <20 x i32>, ptr %2, align 4
  %strided.vec = shufflevector <20 x i32> %wide.vec, <20 x i32> poison, <4 x i32> <i32 0, i32 5, i32 10, i32 15>
  %3 = getelementptr inbounds nuw i32, ptr %out, i64 %index
  store <4 x i32> %strided.vec, ptr %3, align 4
  %index.next = add nuw i64 %index, 4
  %4 = icmp eq i64 %index.next, 12
  br i1 %4, label %middle.block, label %vector.body

middle.block:
  br label %scalar.ph

scalar.ph:
  %bc.resume.val = phi i64 [ 12, %middle.block ], [ 0, %start ]
  br label %loop

loop:
  %iv = phi i64 [ %bc.resume.val, %scalar.ph ], [ %iv.next, %loop ]
  %iv.next = add nuw nsw i64 %iv, 1
  %iv.mul5 = mul nuw nsw i64 %iv, 5
  %iv.mul5.masked = and i64 %iv.mul5, 15
  %gep = getelementptr inbounds nuw i32, ptr %x, i64 %iv.mul5.masked
  %v = load i32, ptr %gep, align 4
  %out.i = getelementptr inbounds nuw i32, ptr %out, i64 %iv
  store i32 %v, ptr %out.i, align 4
  %exitcond.not = icmp eq i64 %iv.next, 16
  br i1 %exitcond.not, label %exit, label %loop

exit:
  ret void
}
```

What's happening here with the load and shufflevector doesn't really make sense. It seems like the masking on the IV ended up being ignored?

## Comments

### Comment 1 - nikic

cc @fhahn 

---

### Comment 2 - fhahn

Looks like an SCEVExpander issue, will share a fix soon

---

### Comment 3 - nikic

/cherry-pick a6148071300302b451ae92fbd8f8b955a6974891 ec581e460ae92ef29c1ea4f200b36b79188fdd21 f8972c8280d28660aaff888c093a9e01b9ee71e6

---

### Comment 4 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: f8972c8280d28660aaff888c093a9e01b9ee71e6

https://github.com/llvm/llvm-project/actions/runs/17518622323

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 5 - nikic

Backport PR: https://github.com/llvm/llvm-project/pull/157410

---

