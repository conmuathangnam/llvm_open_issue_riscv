# [MachineSink] llc hang caused by #115111

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/119511
**Status:** Closed
**Labels:** llvm:codegen, llvm:hang
**Closed Date:** 2024-12-13T11:46:06Z

## Body

Reproducer:
```
; timeout 0.5s bin/llc reduced.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  br label %lbl_1680.i.us.i.outer

lbl_1680.i.us.i:                                  ; preds = %lbl_1680.i.us.i.outer, %lbl_1680.i.us.i
  br i1 %tobool415.not.i99.us.i, label %lbl_1680.i.us.i.outer, label %lbl_1680.i.us.i

lbl_1680.i.us.i.outer:                            ; preds = %lbl_1680.i.us.i, %entry
  %tobool415.not.i99.us.i = phi i1 [ true, %entry ], [ false, %lbl_1680.i.us.i ]
  br label %lbl_1680.i.us.i
}
```
Bisected to https://github.com/llvm/llvm-project/pull/115111.
cc @paperchalice @qcolombet @chapuni 


## Comments

### Comment 1 - paperchalice

Reverted. Thanks for the reproducer. Need some time to troubleshoot it...

---

### Comment 2 - paperchalice

Opened #119547.

---

