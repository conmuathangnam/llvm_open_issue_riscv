# [Indvars] Miscompile: SSA value incorrectly simplified

**Author:** Nirhar
**URL:** https://github.com/llvm/llvm-project/issues/123550
**Status:** Closed
**Labels:** miscompilation, release:backport, llvm:SCEV
**Closed Date:** 2025-01-29T08:09:16Z

## Body

One can find the incorrect transformation here: https://godbolt.org/z/T9qMhT7rb
And the proof of incorrect transformation here: https://alive2.llvm.org/ce/z/PXjb5J

For the record, I describe the problem here. Given the IR:
```
define i32 @widget() {
bb:
  br label %bb1

bb1:                                              ; preds = %bb4, %bb
  %phi = phi i32 [ -173, %bb ], [ %sub, %bb4 ]
  %phi2 = phi i32 [ 1, %bb ], [ %add, %bb4 ]
  %srem = srem i32 729259140, %phi
  %trunc = trunc i32 %phi2 to i8
  %urem = urem i8 -83, %trunc
  %zext = zext i8 %urem to i32
  %sub = sub nsw i32 0, %zext
  br label %bb4

bb4:                                              ; preds = %bb1
  %add = add nuw nsw i32 %phi2, 1
  %icmp = icmp eq i32 %sub, 0
  br i1 %icmp, label %bb5, label %bb1

bb5:                                              ; preds = %bb4
  %phi6 = phi i32 [ %srem, %bb4 ]
  ret i32 %phi6
}
```
which returns `srem i32 729259140, -173` which is `130`. The above IR under the `indvars` pass transforms to:
```
define i32 @widget() {
  br label %bb1

bb1:                                              ; preds = %bb4, %bb
  br label %bb4

bb4:                                              ; preds = %bb1
  br i1 true, label %bb5, label %bb1

bb5:                                              ; preds = %bb4
  ret i32 729259140
}
```
which returns `729259140` which is the wrong answer. 

## Comments

### Comment 1 - nikic

Slightly cleaned up:
```
define i32 @test() {
bb:
  br label %loop 

loop:                                             
  %phi = phi i32 [ -173, %bb ], [ %sub, %loop ]
  %iv2 = phi i32 [ 1, %bb ], [ %iv2.inc, %loop ]
  %srem = srem i32 729259140, %phi
  %trunc = trunc i32 %iv2 to i8
  %urem = urem i8 -83, %trunc
  %zext = zext i8 %urem to i32
  %sub = sub i32 0, %zext 
  %iv2.inc = add i32 %iv2, 1
  %icmp = icmp eq i32 %zext, 0
  br i1 %icmp, label %exit, label %loop

exit:
  %srem.lcssa = phi i32 [ %srem, %loop ]
  ret i32 %srem.lcssa
}
```

Incorrect exit value is computed by SCEV:
```
  -->  (729259140 + (-1 * (-173 + (-1 * (zext i8 ((-83 /u {0,+,1}<%loop>) * {0,+,-1}<%loop>) to i32))<nuw><nsw>)<nuw><nsw> * (729259140 /u (-173 + (-1 * (zext i8 ((-83 /u {0,+,1}<%loop>) * {0,+,-1}<%loop>) to i32))<nuw><nsw>)<nuw><nsw>)))<nuw><nsw> U: empty-set S: empty-set		Exits: 729259140		LoopDispositions: { %loop: Computable }
```

---

### Comment 2 - nikic

/cherry-pick 07efe2c18a63423943a4f9d9daeada23601f84c8

---

### Comment 3 - llvmbot

/pull-request llvm/llvm-project#124895

---

