# SLP vectorizer crashes with assertions on

**Author:** gbaraldi
**URL:** https://github.com/llvm/llvm-project/issues/175768
**Status:** Closed
**Labels:** release:backport, llvm:SLPVectorizer, crash
**Closed Date:** 2026-01-24T15:32:07Z

## Body

This is the repro in godbold
https://godbolt.org/z/c3nzMcrGs

```llvm
target triple = "x86_64-unknown-linux-gnu"

define void @test(i64 %arg0, i64 %arg1) {
entry:
  %init = add i64 %arg0, 1
  br label %loop

loop:
  %iv = phi i64 [ %init, %entry ], [ %iv.next, %reduce ]
  %counter = phi i64 [ 1, %entry ], [ %counter.next, %reduce ]
  %off0 = add i64 %iv, -4
  %off1 = add i64 %iv, -3
  %off2 = add i64 %iv, -2
  %off3 = add i64 %iv, -1
  %ptr = call ptr null(ptr null, ptr null)
  %idx0 = add i64 %arg1, %off0
  %idx0.scaled = shl i64 %idx0, 3
  %gep0 = getelementptr i8, ptr %ptr, i64 %idx0.scaled
  %gep0.off = getelementptr i8, ptr %gep0, i64 -8
  %load0 = load double, ptr %gep0.off, align 8
  %idx1 = add i64 %arg1, %off1
  %idx1.scaled = shl i64 %idx1, 3
  %gep1 = getelementptr i8, ptr %ptr, i64 %idx1.scaled
  %gep1.off = getelementptr i8, ptr %gep1, i64 -8
  %load1 = load double, ptr %gep1.off, align 8
  %idx2 = add i64 %arg1, %off2
  %idx2.scaled = shl i64 %idx2, 3
  %gep2 = getelementptr i8, ptr %ptr, i64 %idx2.scaled
  %gep2.off = getelementptr i8, ptr %gep2, i64 -8
  %load2 = load double, ptr %gep2.off, align 8
  %idx3 = add i64 %arg1, %off3
  %idx3.scaled = shl i64 %idx3, 3
  %gep3 = getelementptr i8, ptr %ptr, i64 %idx3.scaled
  %gep3.off = getelementptr i8, ptr %gep3, i64 -8
  %load3 = load double, ptr %gep3.off, align 8
  %idx4 = add i64 %arg1, %iv
  %idx4.scaled = shl i64 %idx4, 3
  %gep4 = getelementptr i8, ptr %ptr, i64 %idx4.scaled
  %gep4.off = getelementptr i8, ptr %gep4, i64 -8
  %load4 = load double, ptr %gep4.off, align 8
  %load5 = load double, ptr %gep4, align 8
  br label %reduce

dead:
  br label %reduce

reduce:
  %phi0 = phi double [ %load0, %loop ], [ 0.000000e+00, %dead ]
  %phi1 = phi double [ %load1, %loop ], [ 0.000000e+00, %dead ]
  %phi2 = phi double [ %load2, %loop ], [ 0.000000e+00, %dead ]
  %phi3 = phi double [ %load3, %loop ], [ 0.000000e+00, %dead ]
  %phi4 = phi double [ %load4, %loop ], [ 0.000000e+00, %dead ]
  %phi5 = phi double [ %load5, %loop ], [ 0.000000e+00, %dead ]
  %min0 = call double @llvm.minimum.f64(double 0.000000e+00, double %phi0)
  %min1 = call double @llvm.minimum.f64(double %min0, double %phi1)
  %min2 = call double @llvm.minimum.f64(double %min1, double %phi2)
  %min3 = call double @llvm.minimum.f64(double %min2, double %phi3)
  %min4 = call double @llvm.minimum.f64(double %min3, double %phi4)
  %min5 = call double @llvm.minimum.f64(double %min4, double %phi5)
  %min6 = call double @llvm.minimum.f64(double %min5, double 0.000000e+00)
  %counter.next = add i64 %counter, 1
  %iv.next = add i64 %counter, %init
  br label %loop
}

declare double @llvm.minimum.f64(double, double)

```

## Comments

### Comment 1 - RKSimon

22.x regression - tentatively tagging this for cherry picking 

---

### Comment 2 - gbaraldi

This broke between 18 and 20 (this was found in Julia when we did that bump) 

---

### Comment 3 - RKSimon

> This broke between 18 and 20 (this was found in Julia when we did that bump)

@alexey-bataev if it is this old, should we get it fixed in 22.x? is there a higher chance of it occurring with more recent slp features?

---

### Comment 4 - alexey-bataev

Potentially yes, so it would be good to port it to 22

---

### Comment 5 - RKSimon

/cherry-pick 72a20b8e29876106aa918d03ef95f27a6bf0fa0d

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#177774

---

