# [SLPVectorizer] Crash after 57cae2b6a275a8eb3bc8935973263ed84535fb81

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/159406
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-09-18T19:11:40Z

## Body

```
$ cat /tmp/a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-cros-linux-gnu"
                                                                            
%struct.fe = type { [5 x i64] }
                                                                            
define i32 @f(i64 %0, i128 %1, i1 %2) {
  %4 = alloca %struct.fe, align 8
  %5 = getelementptr inbounds nuw i8, ptr %4, i64 16
  %6 = getelementptr inbounds nuw i8, ptr %4, i64 24
  %7 = getelementptr inbounds nuw i8, ptr %4, i64 32
  %8 = getelementptr inbounds nuw i8, ptr %4, i64 8
  br label %9                            

9:                                                ; preds = %9, %3
  %10 = phi i64 [ undef, %3 ], [ %29, %9 ]
  %11 = phi i64 [ undef, %3 ], [ %26, %9 ]
  %12 = phi i64 [ undef, %3 ], [ %23, %9 ]
  %13 = phi i64 [ undef, %3 ], [ %20, %9 ]
  %14 = phi i64 [ undef, %3 ], [ %17, %9 ]
  %.sroa.14.0 = phi i64 [ undef, %3 ], [ %52, %9 ]
  %.sroa.11.0 = phi i64 [ undef, %3 ], [ %50, %9 ]
  %.sroa.8.0 = phi i64 [ undef, %3 ], [ %57, %9 ]
  %.sroa.4.0 = phi i64 [ undef, %3 ], [ %56, %9 ]
  %.sroa.0.0 = phi i64 [ undef, %3 ], [ %54, %9 ]
  %15 = xor i64 %.sroa.0.0, %14
  %16 = and i64 %15, %0
  %17 = xor i64 %16, 1
  store i64 %17, ptr %4, align 8
  %18 = xor i64 %.sroa.4.0, %13
  %19 = and i64 %18, %0
  %20 = xor i64 %19, 1
  store i64 %20, ptr %8, align 8
  %21 = xor i64 %.sroa.8.0, %12
  %22 = and i64 %21, %0
  %23 = xor i64 %22, 1
  store i64 %23, ptr %5, align 8
  %24 = xor i64 %.sroa.11.0, %11
  %25 = and i64 %24, %0
  %26 = xor i64 %25, 1
  store i64 %26, ptr %6, align 8
  %27 = xor i64 %.sroa.14.0, %10
  %28 = and i64 %27, %0
  %29 = xor i64 %28, 1
  store i64 %29, ptr %7, align 8
  %30 = load i64, ptr null, align 4294967296
  %31 = or i64 %19, 1
  %32 = or i64 %16, 1
  %33 = add i64 %17, 1
  %34 = mul i64 %29, 19
  %35 = zext i64 %34 to i128
  %36 = mul i64 %26, 19
  %37 = zext i64 %36 to i128
  %38 = mul i64 %23, 19
  %39 = zext i64 %38 to i128
  %40 = mul nuw nsw i128 %39, 24
  %41 = zext i64 %32 to i128
  %42 = mul nuw i128 %37, %41
  %43 = zext i64 %31 to i128
  %44 = mul nuw i128 %35, %43
  %45 = zext i64 %33 to i128
  %46 = mul i128 %1, %45
  %47 = add i128 %40, %46
  %48 = add i128 %47, %42
  %49 = add i128 %48, %44
  %50 = and i64 %29, 1
  %51 = trunc i128 %49 to i64
  %52 = and i64 %30, 1
  %53 = add i64 %51, 1
  %54 = and i64 %53, 1
  %55 = lshr i64 %53, 1
  %56 = and i64 %19, 1
  %57 = or i64 %55, 1
  br i1 %2, label %58, label %9

58:                                               ; preds = %9
  call void @g(ptr nonnull %4)
  ret i32 0
}

declare void @g(ptr)

$ opt -p slp-vectorizer -disable-output /tmp/a.ll
opt: ../../llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp:20859: auto llvm::slpvectorizer::BoUpSLP::BlockScheduling::tryScheduleBundle(ArrayRef<Value *>, BoUpSLP *, const InstructionsState &, const EdgeInfo &)::(anonymous class)::operator()(bool, ScheduleBundle &) const: Assertion `Picked->isReady() && "m
ust be ready to schedule"' failed.                                                                                                                                                                                                                                                                                 
```

@alexey-bataev 

## Comments

### Comment 1 - nico

linkify 57cae2b6a275a8eb3bc8935973263ed84535fb81

ref #147366

---

