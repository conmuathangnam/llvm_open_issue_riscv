# [SDAG] Miscompilation at -O3

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/155828
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2025-08-29T14:51:42Z

## Body

Reproducer:
```
; bin/lli reduced.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  br label %func_188.exit.i.i

func_188.exit.i.i:                                ; preds = %func_188.exit.i.i, %entry
  %or659.i167180.i.i = phi i32 [ 0, %entry ], [ 1, %func_188.exit.i.i ]
  %conv48.i.i = trunc i32 %or659.i167180.i.i to i8
  %and.i.i.i = and i32 %or659.i167180.i.i, 1
  %tobool80.not.i.i.i = icmp eq i32 %and.i.i.i, 0
  br i1 %tobool80.not.i.i.i, label %if.else.i.i.i, label %func_188.exit.i.i

if.else.i.i.i:                                    ; preds = %func_188.exit.i.i
  %cmp183.i.i.i = icmp sgt i8 %conv48.i.i, 0
  %ext = zext i1 %cmp183.i.i.i to i32 
  ret i32 %ext
}
```
The expected return value is 0. But I got 1.
llvm version: 1bb8db5d5e1a3ea99525fd2648164a257955cd6d

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Yingwei Zheng (dtcxzyw)

<details>
Reproducer:
```
; bin/lli reduced.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @<!-- -->main() {
entry:
  br label %func_188.exit.i.i

func_188.exit.i.i:                                ; preds = %func_188.exit.i.i, %entry
  %or659.i167180.i.i = phi i32 [ 0, %entry ], [ 1, %func_188.exit.i.i ]
  %conv48.i.i = trunc i32 %or659.i167180.i.i to i8
  %and.i.i.i = and i32 %or659.i167180.i.i, 1
  %tobool80.not.i.i.i = icmp eq i32 %and.i.i.i, 0
  br i1 %tobool80.not.i.i.i, label %if.else.i.i.i, label %func_188.exit.i.i

if.else.i.i.i:                                    ; preds = %func_188.exit.i.i
  %cmp183.i.i.i = icmp sgt i8 %conv48.i.i, 0
  %ext = zext i1 %cmp183.i.i.i to i32 
  ret i32 %ext
}
```
The expected return value is 0. But I got 1.
llvm version: 1bb8db5d5e1a3ea99525fd2648164a257955cd6d
</details>


---

### Comment 2 - dtcxzyw

@RKSimon Unfortunately I cannot find the bug. Now I am going to bed. Can you please have a look?

---

### Comment 3 - RKSimon

trunk codegen:
```s
main:                                   # @main
        xorl    %eax, %eax
.LBB0_1:                                # %func_188.exit.i.i
        testb   $1, %al
        movl    $1, %eax
        jne     .LBB0_1
        setge   %al
        movzbl  %al, %eax
        retq
```
20.x codegen:
```s
main:                                   # @main
        xorl    %eax, %eax
.LBB0_1:                                # %func_188.exit.i.i
        movl    %eax, %ecx
        movl    $1, %eax
        testb   $1, %cl
        jne     .LBB0_1
        xorl    %eax, %eax
        testb   %cl, %cl
        setg    %al
        retq
```

---

### Comment 4 - RKSimon

CC @phoebewang as I think this is related to #155586 / #156002

---

### Comment 5 - phoebewang

> CC [@phoebewang](https://github.com/phoebewang) as I think this is related to [#155586](https://github.com/llvm/llvm-project/pull/155586) / [#156002](https://github.com/llvm/llvm-project/pull/156002)

Yes, `testb` is back with https://github.com/llvm/llvm-project/pull/156002

---

