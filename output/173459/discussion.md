# [LV] Miscompile of FindFirstIV pattern due to truncation

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/173459
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2026-01-12T18:08:50Z

## Body

working with @MitchBriles on this one.

here's a function:
```llvm
define i1 @f(i64 %0, i64 %1) {
  br label %3

3:
  %4 = phi i64 [ %1, %2 ], [ %11, %3 ]
  %5 = phi i64 [ 0, %2 ], [ %10, %3 ]
  %6 = phi i1 [ false, %2 ], [ %9, %3 ]
  %7 = icmp eq i64 %4, 0
  %8 = trunc i64 %5 to i1
  %9 = select i1 %7, i1 %6, i1 %8
  %10 = add i64 %5, 1
  %11 = add i64 %4, 1
  %12 = icmp eq i64 %0, %5
  br i1 %12, label %13, label %3

13:
  ret i1 %9
}
```
call it as `f(3, -3)` using a driver like this:
```c
unsigned long f(unsigned long, unsigned long);

#include <stdio.h>

int main(void) {
  unsigned long res = f(3UL, -3UL);
  printf("%lu\n", res);
}
```
and it returns 0.

now, we optimize the function at `-O2` and we get this:
```llvm
; ModuleID = 'r2.ll'
source_filename = "r2.ll"

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i1 @f(i64 %0, i64 %1) local_unnamed_addr #0 {
  %3 = add i64 %0, 1
  %min.iters.check = icmp ult i64 %3, 4
  br i1 %min.iters.check, label %scalar.ph.preheader, label %vector.ph

vector.ph:                                        ; preds = %2
  %n.vec = and i64 %3, -4
  %4 = add i64 %1, %n.vec
  %broadcast.splatinsert = insertelement <4 x i64> poison, i64 %1, i64 0
  %broadcast.splat = shufflevector <4 x i64> %broadcast.splatinsert, <4 x i64> poison, <4 x i32> zeroinitializer
  %induction = add <4 x i64> %broadcast.splat, <i64 0, i64 1, i64 2, i64 3>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <4 x i64> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i1> [ zeroinitializer, %vector.ph ], [ %6, %vector.body ]
  %5 = icmp eq <4 x i64> %vec.ind, zeroinitializer
  %6 = select <4 x i1> %5, <4 x i1> %vec.phi, <4 x i1> <i1 false, i1 true, i1 false, i1 true>
  %index.next = add nuw i64 %index, 4
  %vec.ind.next = add <4 x i64> %vec.ind, splat (i64 4)
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !0

middle.block:                                     ; preds = %vector.body
  %8 = bitcast <4 x i1> %6 to i4
  %9 = icmp ne i4 %8, 0
  %cmp.n = icmp eq i64 %3, %n.vec
  br i1 %cmp.n, label %.loopexit, label %scalar.ph.preheader

scalar.ph.preheader:                              ; preds = %2, %middle.block
  %.ph = phi i64 [ %1, %2 ], [ %4, %middle.block ]
  %.ph4 = phi i64 [ 0, %2 ], [ %n.vec, %middle.block ]
  %.ph5 = phi i1 [ false, %2 ], [ %9, %middle.block ]
  br label %scalar.ph

scalar.ph:                                        ; preds = %scalar.ph.preheader, %scalar.ph
  %10 = phi i64 [ %17, %scalar.ph ], [ %.ph, %scalar.ph.preheader ]
  %11 = phi i64 [ %16, %scalar.ph ], [ %.ph4, %scalar.ph.preheader ]
  %12 = phi i1 [ %15, %scalar.ph ], [ %.ph5, %scalar.ph.preheader ]
  %13 = icmp eq i64 %10, 0
  %14 = trunc i64 %11 to i1
  %15 = select i1 %13, i1 %12, i1 %14
  %16 = add i64 %11, 1
  %17 = add i64 %10, 1
  %18 = icmp eq i64 %0, %11
  br i1 %18, label %.loopexit, label %scalar.ph, !llvm.loop !3

.loopexit:                                        ; preds = %scalar.ph, %middle.block
  %.lcssa = phi i1 [ %9, %middle.block ], [ %15, %scalar.ph ]
  ret i1 %.lcssa
}

attributes #0 = { nofree norecurse nosync nounwind memory(none) }

!0 = distinct !{!0, !1, !2}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = !{!"llvm.loop.unroll.runtime.disable"}
!3 = distinct !{!3, !2, !1}
```
when we run this function using the same inputs, it returns 1.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: John Regehr (regehr)

<details>
working with @<!-- -->MitchBriles on this one.

here's a function:
```llvm
define i1 @<!-- -->f(i64 %0, i64 %1) {
  br label %3

3:
  %4 = phi i64 [ %1, %2 ], [ %11, %3 ]
  %5 = phi i64 [ 0, %2 ], [ %10, %3 ]
  %6 = phi i1 [ false, %2 ], [ %9, %3 ]
  %7 = icmp eq i64 %4, 0
  %8 = trunc i64 %5 to i1
  %9 = select i1 %7, i1 %6, i1 %8
  %10 = add i64 %5, 1
  %11 = add i64 %4, 1
  %12 = icmp eq i64 %0, %5
  br i1 %12, label %13, label %3

13:
  ret i1 %9
}
```
call it as `f(3, -3)` using a driver like this:
```c
unsigned long f(unsigned long, unsigned long);

#include &lt;stdio.h&gt;

int main(void) {
  unsigned long res = f(3UL, -3UL);
  printf("%lu\n", res);
}
```
and it returns 0.

now, we optimize the function at `-O2` and we get this:
```llvm
; ModuleID = 'r2.ll'
source_filename = "r2.ll"

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define i1 @<!-- -->f(i64 %0, i64 %1) local_unnamed_addr #<!-- -->0 {
  %3 = add i64 %0, 1
  %min.iters.check = icmp ult i64 %3, 4
  br i1 %min.iters.check, label %scalar.ph.preheader, label %vector.ph

vector.ph:                                        ; preds = %2
  %n.vec = and i64 %3, -4
  %4 = add i64 %1, %n.vec
  %broadcast.splatinsert = insertelement &lt;4 x i64&gt; poison, i64 %1, i64 0
  %broadcast.splat = shufflevector &lt;4 x i64&gt; %broadcast.splatinsert, &lt;4 x i64&gt; poison, &lt;4 x i32&gt; zeroinitializer
  %induction = add &lt;4 x i64&gt; %broadcast.splat, &lt;i64 0, i64 1, i64 2, i64 3&gt;
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i64 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi &lt;4 x i64&gt; [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi &lt;4 x i1&gt; [ zeroinitializer, %vector.ph ], [ %6, %vector.body ]
  %5 = icmp eq &lt;4 x i64&gt; %vec.ind, zeroinitializer
  %6 = select &lt;4 x i1&gt; %5, &lt;4 x i1&gt; %vec.phi, &lt;4 x i1&gt; &lt;i1 false, i1 true, i1 false, i1 true&gt;
  %index.next = add nuw i64 %index, 4
  %vec.ind.next = add &lt;4 x i64&gt; %vec.ind, splat (i64 4)
  %7 = icmp eq i64 %index.next, %n.vec
  br i1 %7, label %middle.block, label %vector.body, !llvm.loop !0

middle.block:                                     ; preds = %vector.body
  %8 = bitcast &lt;4 x i1&gt; %6 to i4
  %9 = icmp ne i4 %8, 0
  %cmp.n = icmp eq i64 %3, %n.vec
  br i1 %cmp.n, label %.loopexit, label %scalar.ph.preheader

scalar.ph.preheader:                              ; preds = %2, %middle.block
  %.ph = phi i64 [ %1, %2 ], [ %4, %middle.block ]
  %.ph4 = phi i64 [ 0, %2 ], [ %n.vec, %middle.block ]
  %.ph5 = phi i1 [ false, %2 ], [ %9, %middle.block ]
  br label %scalar.ph

scalar.ph:                                        ; preds = %scalar.ph.preheader, %scalar.ph
  %10 = phi i64 [ %17, %scalar.ph ], [ %.ph, %scalar.ph.preheader ]
  %11 = phi i64 [ %16, %scalar.ph ], [ %.ph4, %scalar.ph.preheader ]
  %12 = phi i1 [ %15, %scalar.ph ], [ %.ph5, %scalar.ph.preheader ]
  %13 = icmp eq i64 %10, 0
  %14 = trunc i64 %11 to i1
  %15 = select i1 %13, i1 %12, i1 %14
  %16 = add i64 %11, 1
  %17 = add i64 %10, 1
  %18 = icmp eq i64 %0, %11
  br i1 %18, label %.loopexit, label %scalar.ph, !llvm.loop !3

.loopexit:                                        ; preds = %scalar.ph, %middle.block
  %.lcssa = phi i1 [ %9, %middle.block ], [ %15, %scalar.ph ]
  ret i1 %.lcssa
}

attributes #<!-- -->0 = { nofree norecurse nosync nounwind memory(none) }

!0 = distinct !{!0, !1, !2}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = !{!"llvm.loop.unroll.runtime.disable"}
!3 = distinct !{!3, !2, !1}
```
when we run this function using the same inputs, it returns 1.


</details>


---

### Comment 2 - dtcxzyw

Bisected to loop-vectorize cc @fhahn @lukel97 @artagnon 

---

### Comment 3 - fhahn

looks like the issue here is that we vectorize with FindFirstIV, but the IV truncated to i1 wraps

---

### Comment 4 - fhahn

Proposed fix https://github.com/llvm/llvm-project/pull/174225

---

