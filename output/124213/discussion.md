# Miscompile after #122723

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/124213
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-01-29T09:16:16Z

## Body

#122723 introduced a miscompilation. I've reduced it to this:
```
$ cat q.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.wombat = type { %struct.snork }
%struct.snork = type { [1 x i32] }
%struct.wobble = type { %struct.snork }

declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #0

define void @wobble() {
  %alloca = alloca [0 x [0 x [0 x [0 x %struct.wombat]]]], i32 0, align 4
  call void @pluto(ptr %alloca)
  %call = call i1 @wibble(ptr %alloca, double 5.000000e-01)
  br i1 %call, label %bb3, label %bb1

bb1:                                              ; preds = %bb
  %call2 = call i1 @hoge(ptr %alloca, double -5.000000e-01)
  br i1 %call2, label %bb3, label %bb4

bb3:                                              ; preds = %bb1, %bb
  call void @llvm.lifetime.start.p0(i64 0, ptr null)
  br label %bb4

bb4:                                              ; preds = %bb3, %bb1
  ret void
}

define void @pluto(ptr %arg) {
  %load = load volatile i1, ptr %arg, align 1
  call void @ham(ptr %arg)
  ret void
}

define i1 @wibble(ptr %arg, double %arg1) {
  %call = call i1 @hoge(ptr %arg, double %arg1)
  %xor = xor i1 %call, true
  ret i1 %xor
}

define i1 @hoge(ptr %arg, double %arg1) {
  %alloca = alloca [0 x [0 x [0 x [0 x %struct.wobble]]]], i32 0, align 4
  %call = call i1 @blam.1(ptr %arg)
  %fcmp = fcmp uge double %arg1, 0.000000e+00
  %xor = xor i1 %call, %fcmp
  br i1 %xor, label %bb4, label %bb2

bb2:                                              ; preds = %bb
  %call3 = call i1 @blam.1(ptr %arg)
  br label %bb7

bb4:                                              ; preds = %bb
  %call5 = call double @quux(double %arg1)
  call void @barney(ptr %alloca, double %call5)
  %call6 = call i1 @pluto.2(ptr %arg, ptr %alloca)
  br label %bb7

bb7:                                              ; preds = %bb4, %bb2
  %phi = phi i1 [ %call3, %bb2 ], [ %call6, %bb4 ]
  ret i1 %phi
}

define i1 @wombat(ptr %arg, ptr %arg1) {
  %call = call i1 @foo(ptr %arg, ptr %arg1)
  ret i1 %call
}

define i1 @foo(ptr %arg, ptr %arg1) {
  %load = load i32, ptr %arg, align 4
  %load2 = load i32, ptr %arg1, align 4
  %icmp = icmp slt i32 %load, %load2
  ret i1 %icmp
}

define void @barney(ptr %arg, double %arg1) {
  call void @blam(double %arg1, ptr %arg)
  ret void
}

define void @blam(double %arg, ptr %arg1) {
  %fcmp = fcmp olt double %arg, 0.000000e+00
  %fmul = fmul double %arg, 0x41F0000000000000
  %select = select i1 %fcmp, double 0.000000e+00, double %fmul
  %fptoui = fptoui double %select to i32
  %sext = sext i1 %fcmp to i32
  %xor = xor i32 %fptoui, %sext
  store i32 %xor, ptr %arg1, align 4
  ret void
}

define void @ham(ptr %arg) {
  %load = load i32, ptr %arg, align 4
  %ashr = ashr i32 %load, 1
  store i32 %ashr, ptr %arg, align 4
  ret void
}

define i1 @blam.1(ptr %arg) {
  %load = load i32, ptr %arg, align 4
  %icmp = icmp slt i32 %load, 0
  ret i1 %icmp
}

define double @quux(double %arg) {
  %fmul = fmul double %arg, 0x3F10000000000000
  ret double %fmul
}

define i1 @pluto.2(ptr %arg, ptr %arg1) {
  %call = call i1 @wombat(ptr %arg, ptr %arg1)
  ret i1 %call
}

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
$ diff -u <(./clang-before -O3 q.ll -emit-llvm -S -o -) <(./clang-after -O3 q.ll -emit-llvm -S -o -)
--- /dev/fd/63  2025-01-24 01:18:49.168689114 +0100
+++ /dev/fd/62  2025-01-24 01:18:49.168689114 +0100
@@ -15,10 +15,8 @@
   %alloca1 = alloca [0 x [0 x [0 x [0 x [0 x %struct.wombat]]]]], align 4
   %load.i = load volatile i1, ptr %alloca1, align 4
   %load.i.i = load i32, ptr %alloca1, align 4
-  %ashr.i.i = ashr i32 %load.i.i, 1
-  %0 = add nsw i32 %ashr.i.i, -131072
-  %or.cond5 = icmp ult i32 %0, -131073
-  br i1 %or.cond5, label %bb3, label %bb4
+  %spec.select6 = icmp ugt i32 %load.i.i, 262143
+  br i1 %spec.select6, label %bb3, label %bb4

 bb3:                                              ; preds = %wibble.exit
   tail call void @llvm.lifetime.start.p0(i64 0, ptr null)
```

Further analysis and smaller reproducers here: https://github.com/llvm/llvm-project/pull/122723#issuecomment-2611083638

## Comments

### Comment 1 - artagnon

Re-land should close this: #124320.

---

