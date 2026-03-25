# LLVM 20 miscompiles/misoptimizes inlined `ldexp` implementation for `x86_fp80`

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/130408
**Status:** Closed
**Labels:** regression, miscompilation, floating-point, llvm:analysis
**Closed Date:** 2025-03-09T13:10:37Z

## Body

Consider this case:

<details>
<summary>LLVM IR</summary>

```llvm
; ModuleID = 'repro.bc'
source_filename = "repro"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux6.8.0-gnu2.39.0"

%Target.Cpu.Feature.Set = type { [5 x i64] }
%Target.Cpu = type { ptr, %Target.Cpu.Feature.Set, i6, [7 x i8] }
%Target.Cpu.Model = type { { ptr, i64 }, { ptr, i64 }, %Target.Cpu.Feature.Set }
%Target.Os = type { %Target.Os.VersionRange, i6, [7 x i8] }
%Target.Os.VersionRange = type { %Target.Os.LinuxVersionRange }
%Target.Os.LinuxVersionRange = type { %SemanticVersion.Range, %SemanticVersion, i32, [4 x i8] }
%SemanticVersion.Range = type { %SemanticVersion, %SemanticVersion }
%SemanticVersion = type { i64, i64, i64, { ptr, i64 }, { ptr, i64 } }
%Target.DynamicLinker = type { [255 x i8], i8 }
%Target = type { %Target.Cpu, %Target.Os, i5, i4, %Target.DynamicLinker, [6 x i8] }
%builtin.CallingConvention.CommonOptions = type { { i64, i8, [7 x i8] } }
%"math.frexp.Frexp(f80)" = type { x86_fp80, i32, [12 x i8] }

@0 = private unnamed_addr constant { i64, i16, [6 x i8] } { i64 undef, i16 1, [6 x i8] undef }, align 8
@builtin.zig_backend = internal unnamed_addr constant i64 2, align 8
@start.simplified_logic = internal unnamed_addr constant i1 false, align 1
@builtin.output_mode = internal unnamed_addr constant i2 -2, align 1
@Target.Cpu.Feature.Set.empty = internal unnamed_addr constant %Target.Cpu.Feature.Set zeroinitializer, align 8
@builtin.cpu = internal unnamed_addr constant %Target.Cpu { ptr @Target.x86.cpu.raptorlake, %Target.Cpu.Feature.Set { [5 x i64] [i64 2116801776035561712, i64 -2445523138194684734, i64 -4456355804684576701, i64 987, i64 0] }, i6 -22, [7 x i8] undef }, align 8
@Target.x86.cpu.raptorlake = internal unnamed_addr constant %Target.Cpu.Model { { ptr, i64 } { ptr @__anon_1971, i64 10 }, { ptr, i64 } { ptr @__anon_1971, i64 10 }, %Target.Cpu.Feature.Set { [5 x i64] [i64 680153494895722672, i64 -7057210256133700414, i64 -4467544435008825277, i64 923, i64 0] } }, align 8
@builtin.os = internal unnamed_addr constant %Target.Os { %Target.Os.VersionRange { %Target.Os.LinuxVersionRange { %SemanticVersion.Range { %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer } }, %SemanticVersion { i64 2, i64 39, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, i32 14, [4 x i8] undef } }, i6 9, [7 x i8] undef }, align 8
@builtin.abi = internal unnamed_addr constant i5 1, align 1
@builtin.object_format = internal unnamed_addr constant i4 2, align 1
@Target.DynamicLinker.none = internal unnamed_addr constant %Target.DynamicLinker { [255 x i8] undef, i8 0 }, align 1
@builtin.target = internal unnamed_addr constant %Target { %Target.Cpu { ptr @Target.x86.cpu.raptorlake, %Target.Cpu.Feature.Set { [5 x i64] [i64 2116801776035561712, i64 -2445523138194684734, i64 -4456355804684576701, i64 987, i64 0] }, i6 -22, [7 x i8] undef }, %Target.Os { %Target.Os.VersionRange { %Target.Os.LinuxVersionRange { %SemanticVersion.Range { %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer } }, %SemanticVersion { i64 2, i64 39, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, i32 14, [4 x i8] undef } }, i6 9, [7 x i8] undef }, i5 1, i4 2, %Target.DynamicLinker { [255 x i8] [i8 47, i8 108, i8 105, i8 98, i8 54, i8 52, i8 47, i8 108, i8 100, i8 45, i8 108, i8 105, i8 110, i8 117, i8 120, i8 45, i8 120, i8 56, i8 54, i8 45, i8 54, i8 52, i8 46, i8 115, i8 111, i8 46, i8 50, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef], i8 27 }, [6 x i8] undef }, align 8
@builtin.CallingConvention.c = internal unnamed_addr constant { <{ %builtin.CallingConvention.CommonOptions, [8 x i8] }>, i8, [7 x i8] } { <{ %builtin.CallingConvention.CommonOptions, [8 x i8] }> <{ %builtin.CallingConvention.CommonOptions { { i64, i8, [7 x i8] } { i64 undef, i8 0, [7 x i8] undef } }, [8 x i8] undef }>, i8 4, [7 x i8] undef }, align 8
@mem.native_endian = internal unnamed_addr constant i1 true, align 1
@__anon_1971 = internal unnamed_addr constant [11 x i8] c"raptorlake\00", align 1

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define dso_local i128 @foo() #0 {
  %1 = alloca [32 x i8], align 16
  call void @llvm.memset.p0.i64(ptr align 16 %1, i8 undef, i64 32, i1 false)
  call void @__fixunsxfei(ptr nonnull align 4 %1, i64 256, x86_fp80 0xK4044D8D726B7177A8000)
  %2 = load i256, ptr %1, align 16
  %3 = trunc i256 %2 to i128
  ret i128 %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: noinline nosanitize_coverage nounwind skipprofile uwtable
define dso_local void @__fixunsxfei(ptr nonnull align 4 %0, i64 %1, x86_fp80 %2) #2 {
  %4 = alloca [16 x i8], align 8
  %5 = alloca [8 x i8], align 8
  store ptr %0, ptr %5, align 8
  call fastcc void @math.divCeil__anon_1677(ptr sret({ i64, i16, [6 x i8] }) %4, i64 %1, i64 32)
  %6 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %4, i32 0, i32 1
  %7 = load i16, ptr %6, align 2
  %8 = icmp eq i16 %7, 0
  br i1 %8, label %17, label %20

9:                                                ; preds = %17
  %10 = phi i64 [ %19, %17 ]
  %11 = load ptr, ptr %5, align 8
  %12 = getelementptr inbounds i32, ptr %11, i64 0
  %13 = insertvalue { ptr, i64 } poison, ptr %12, 0
  %14 = insertvalue { ptr, i64 } %13, i64 %10, 1
  %15 = extractvalue { ptr, i64 } %14, 0
  %16 = extractvalue { ptr, i64 } %14, 1
  call fastcc void @repro.bigIntFromFloat__anon_1692(ptr nonnull align 4 %15, i64 %16, x86_fp80 %2)
  ret void

17:                                               ; preds = %3
  %18 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %4, i32 0, i32 0
  %19 = load i64, ptr %18, align 8
  br label %9

20:                                               ; preds = %3
  unreachable
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @math.divCeil__anon_1677(ptr noalias nonnull sret({ i64, i16, [6 x i8] }) %0, i64 %1, i64 %2) unnamed_addr #0 {
  %4 = icmp eq i64 %2, 0
  br i1 %4, label %7, label %8

5:                                                ; preds = %8
  %6 = icmp ugt i64 %1, 0
  br i1 %6, label %15, label %17

7:                                                ; preds = %3
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %0, ptr align 8 @0, i64 16, i1 false)
  ret void

8:                                                ; preds = %3
  br label %5

9:                                                ; preds = %24
  %10 = udiv i64 %1, %2
  %11 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 1
  store i16 0, ptr %11, align 2
  %12 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 0
  store i64 %10, ptr %12, align 8
  ret void

13:                                               ; preds = %17, %15
  %14 = phi i1 [ %16, %15 ], [ false, %17 ]
  br i1 %14, label %18, label %24

15:                                               ; preds = %5
  %16 = icmp ugt i64 %2, 0
  br label %13

17:                                               ; preds = %5
  br label %13

18:                                               ; preds = %13
  %19 = sub nuw i64 %1, 1
  %20 = udiv i64 %19, %2
  %21 = add nuw i64 %20, 1
  %22 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 1
  store i16 0, ptr %22, align 2
  %23 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 0
  store i64 %21, ptr %23, align 8
  ret void

24:                                               ; preds = %13
  br label %9
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #3

; Function Attrs: noinline nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @repro.bigIntFromFloat__anon_1692(ptr nonnull align 4 %0, i64 %1, x86_fp80 %2) unnamed_addr #2 {
  %4 = alloca [16 x i8], align 8
  %5 = alloca [16 x i8], align 16
  %6 = alloca [32 x i8], align 16
  %7 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %8 = insertvalue { ptr, i64 } %7, i64 %1, 1
  call fastcc void @math.frexp.frexp__anon_1724(ptr sret(%"math.frexp.Frexp(f80)") %6, x86_fp80 %2)
  %9 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %6, i32 0, i32 1
  %10 = load i32, ptr %9, align 4
  %11 = sub nsw i32 %10, 64
  %12 = call i32 @llvm.smax.i32(i32 0, i32 %11)
  %13 = trunc i32 %12 to i31
  switch i31 %13, label %28 [
    i31 0, label %27
  ]

14:                                               ; preds = %28, %27
  %15 = phi x86_fp80 [ %2, %27 ], [ %34, %28 ]
  %16 = fptoui x86_fp80 %15 to i64
  %17 = extractvalue { ptr, i64 } %8, 0
  %18 = extractvalue { ptr, i64 } %8, 1
  %19 = mul nuw i64 %18, 4
  call void @llvm.memset.p0.i64(ptr align 4 %17, i8 0, i64 %19, i1 false)
  %20 = extractvalue { ptr, i64 } %8, 0
  %21 = extractvalue { ptr, i64 } %8, 1
  %22 = call fastcc { ptr, i64 } @mem.sliceAsBytes__anon_1790(ptr nonnull align 4 %20, i64 %21)
  store { ptr, i64 } %22, ptr %4, align 8
  %23 = load { ptr, i64 }, ptr %4, align 8
  %24 = zext i31 %13 to i64
  %25 = extractvalue { ptr, i64 } %23, 0
  %26 = extractvalue { ptr, i64 } %23, 1
  call fastcc void @mem.writePackedIntLittle__anon_1792(ptr nonnull align 1 %25, i64 %26, i64 %24, i64 %16)
  ret void

27:                                               ; preds = %3
  br label %14

28:                                               ; preds = %3
  %29 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %6, i32 0, i32 0
  %30 = load x86_fp80, ptr %29, align 16
  %31 = getelementptr inbounds { x86_fp80 }, ptr %5, i32 0, i32 0
  store x86_fp80 %30, ptr %31, align 16
  %32 = getelementptr inbounds { x86_fp80 }, ptr %5, i32 0, i32 0
  %33 = load x86_fp80, ptr %32, align 16
  %34 = call fastcc x86_fp80 @math.ldexp.ldexp__anon_1741(x86_fp80 %33, i32 64) #5
  br label %14
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @math.frexp.frexp__anon_1724(ptr noalias nonnull sret(%"math.frexp.Frexp(f80)") %0, x86_fp80 %1) unnamed_addr #0 {
  %3 = alloca [16 x i8], align 16
  %4 = alloca [32 x i8], align 16
  call void @llvm.memset.p0.i64(ptr align 16 %4, i8 undef, i64 32, i1 false)
  %5 = bitcast x86_fp80 %1 to i80
  store i80 %5, ptr %3, align 16
  %6 = load i128, ptr %3, align 16
  %7 = trunc i128 %6 to i80
  %8 = trunc i80 %7 to i64
  %9 = load i128, ptr %3, align 16
  %10 = trunc i128 %9 to i80
  %11 = zext i7 -64 to i80
  %12 = lshr i80 %10, %11
  %13 = trunc i80 %12 to i15
  switch i15 %13, label %27 [
    i15 0, label %19
    i15 -1, label %21
  ]

14:                                               ; preds = %58, %37, %27
  %15 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 0
  %16 = load i128, ptr %3, align 16
  %17 = trunc i128 %16 to i80
  %18 = bitcast i80 %17 to x86_fp80
  store x86_fp80 %18, ptr %15, align 16
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %0, ptr align 16 %4, i64 32, i1 false)
  ret void

19:                                               ; preds = %2
  %20 = icmp ne i64 %8, 0
  br i1 %20, label %38, label %56

21:                                               ; preds = %2
  %22 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  call void @llvm.memset.p0.i64(ptr align 4 %22, i8 undef, i64 4, i1 false)
  %23 = load i128, ptr %3, align 16
  %24 = trunc i128 %23 to i80
  %25 = trunc i80 %24 to i63
  %26 = icmp eq i63 %25, 0
  br i1 %26, label %59, label %61

27:                                               ; preds = %2
  %28 = load i128, ptr %3, align 16
  %29 = trunc i128 %28 to i80
  %30 = and i80 %29, -604444463063240877801473
  store i80 %30, ptr %3, align 16
  %31 = load i128, ptr %3, align 16
  %32 = trunc i128 %31 to i80
  %33 = or i80 %32, 302194561415509874573312
  store i80 %33, ptr %3, align 16
  %34 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  %35 = zext i15 %13 to i32
  %36 = sub nsw i32 %35, 16382
  store i32 %36, ptr %34, align 4
  br label %14

37:                                               ; preds = %56, %38
  br label %14

38:                                               ; preds = %19
  %39 = call i64 @llvm.ctlz.i64(i64 %8, i1 false)
  %40 = trunc i64 %39 to i7
  %41 = load i128, ptr %3, align 16
  %42 = trunc i128 %41 to i80
  %43 = and i80 %42, -604462909807314587353088
  store i80 %43, ptr %3, align 16
  %44 = load i128, ptr %3, align 16
  %45 = trunc i128 %44 to i80
  %46 = or i80 %45, 302194561415509874573312
  store i80 %46, ptr %3, align 16
  %47 = load i128, ptr %3, align 16
  %48 = trunc i128 %47 to i80
  %49 = call fastcc i64 @math.shl__anon_1845(i64 %8, i7 %40)
  %50 = zext i64 %49 to i80
  %51 = or i80 %48, %50
  store i80 %51, ptr %3, align 16
  %52 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  %53 = zext i7 %40 to i32
  %54 = sub nsw i32 -16382, %53
  %55 = add nsw i32 %54, 1
  store i32 %55, ptr %52, align 4
  br label %37

56:                                               ; preds = %19
  %57 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  store i32 0, ptr %57, align 4
  br label %37

58:                                               ; preds = %61, %59
  br label %14

59:                                               ; preds = %21
  %60 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  store i32 0, ptr %60, align 4
  br label %58

61:                                               ; preds = %21
  br label %58
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #4

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc x86_fp80 @math.ldexp.ldexp__anon_1741(x86_fp80 %0, i32 %1) unnamed_addr #0 {
  %3 = alloca [16 x i8], align 16
  %4 = alloca [16 x i8], align 16
  %5 = alloca [4 x i8], align 4
  %6 = bitcast x86_fp80 %0 to i80
  %7 = and i80 %6, -604462909807314587353088
  %8 = call fastcc i1 @math.isnan.isNan__anon_1866(x86_fp80 %0)
  br i1 %8, label %19, label %20

9:                                                ; preds = %24
  %10 = zext i7 1 to i80
  %11 = shl i80 %6, %10
  %12 = zext i7 -63 to i80
  %13 = lshr i80 %11, %12
  %14 = trunc i80 %13 to i32
  store i32 %14, ptr %5, align 4
  %15 = load i32, ptr %5, align 4
  %16 = icmp eq i32 %15, 0
  br i1 %16, label %27, label %36

17:                                               ; preds = %20, %19
  %18 = phi i1 [ true, %19 ], [ %22, %20 ]
  br i1 %18, label %23, label %24

19:                                               ; preds = %2
  br label %17

20:                                               ; preds = %2
  %21 = call fastcc i1 @math.isfinite.isFinite__anon_1870(x86_fp80 %0)
  %22 = xor i1 %21, true
  br label %17

23:                                               ; preds = %17
  ret x86_fp80 %0

24:                                               ; preds = %17
  br label %9

25:                                               ; preds = %36, %27
  %26 = icmp sge i32 %1, 0
  br i1 %26, label %37, label %41

27:                                               ; preds = %9
  %28 = load i32, ptr %5, align 4
  %29 = zext i7 1 to i80
  %30 = shl i80 %6, %29
  %31 = call i80 @llvm.ctlz.i80(i80 %30, i1 false)
  %32 = trunc i80 %31 to i7
  %33 = zext i7 %32 to i32
  %34 = sub nsw i32 16, %33
  %35 = add nsw i32 %28, %34
  store i32 %35, ptr %5, align 4
  br label %25

36:                                               ; preds = %9
  br label %25

37:                                               ; preds = %25
  %38 = load i32, ptr %5, align 4
  %39 = sub nsw i32 32766, %38
  %40 = icmp sgt i32 %1, %39
  br i1 %40, label %51, label %54

41:                                               ; preds = %25
  %42 = load i32, ptr %5, align 4
  %43 = sub nsw i32 0, %42
  %44 = icmp sle i32 %1, %43
  br i1 %44, label %96, label %101

45:                                               ; preds = %58
  %46 = sext i32 %1 to i80
  %47 = zext i7 -64 to i80
  %48 = shl i80 %46, %47
  %49 = add nuw i80 %6, %48
  %50 = bitcast i80 %49 to x86_fp80
  ret x86_fp80 %50

51:                                               ; preds = %37
  %52 = or i80 604453686435277732577280, %7
  %53 = bitcast i80 %52 to x86_fp80
  ret x86_fp80 %53

54:                                               ; preds = %37
  %55 = load i32, ptr %5, align 4
  %56 = add nsw i32 %55, %1
  %57 = icmp sle i32 %56, 0
  br i1 %57, label %59, label %65

58:                                               ; preds = %68
  br label %45

59:                                               ; preds = %54
  %60 = trunc i32 %1 to i7
  %61 = zext i7 %60 to i80
  %62 = shl i80 %6, %61
  %63 = or i80 %62, %7
  %64 = bitcast i80 %63 to x86_fp80
  ret x86_fp80 %64

65:                                               ; preds = %54
  %66 = load i32, ptr %5, align 4
  %67 = icmp sle i32 %66, 0
  br i1 %67, label %69, label %88

68:                                               ; preds = %88
  br label %58

69:                                               ; preds = %65
  %70 = load i32, ptr %5, align 4
  %71 = add nsw i32 %1, %70
  %72 = sext i32 %71 to i80
  %73 = zext i7 -64 to i80
  %74 = shl i80 %72, %73
  store i80 %74, ptr %4, align 16
  %75 = load i128, ptr %4, align 16
  %76 = trunc i128 %75 to i80
  %77 = load i32, ptr %5, align 4
  %78 = sub nsw i32 1, %77
  %79 = trunc i32 %78 to i7
  %80 = zext i7 %79 to i80
  %81 = shl i80 %6, %80
  %82 = and i80 %81, 18446744073709551615
  %83 = or i80 %76, %82
  store i80 %83, ptr %4, align 16
  %84 = load i128, ptr %4, align 16
  %85 = trunc i128 %84 to i80
  %86 = or i80 %85, %7
  %87 = bitcast i80 %86 to x86_fp80
  ret x86_fp80 %87

88:                                               ; preds = %65
  br label %68

89:                                               ; preds = %101
  %90 = sub nsw i32 0, %1
  %91 = sext i32 %90 to i80
  %92 = zext i7 -64 to i80
  %93 = shl i80 %91, %92
  %94 = sub nuw i80 %6, %93
  %95 = bitcast i80 %94 to x86_fp80
  ret x86_fp80 %95

96:                                               ; preds = %41
  %97 = load i32, ptr %5, align 4
  %98 = add nsw i32 64, %97
  %99 = sub nsw i32 0, %98
  %100 = icmp slt i32 %1, %99
  br i1 %100, label %132, label %134

101:                                              ; preds = %41
  br label %89

102:                                              ; preds = %134
  %103 = sub nsw i32 0, %1
  %104 = load i32, ptr %5, align 4
  %105 = add nsw i32 %104, %1
  %106 = sub nsw i32 0, %105
  %107 = add nsw i32 %106, 1
  %108 = call i32 @llvm.smin.i32(i32 %103, i32 %107)
  %109 = trunc i32 %108 to i7
  %110 = call i80 @llvm.cttz.i80(i80 %6, i1 false)
  %111 = trunc i80 %110 to i7
  %112 = sub nuw i7 %109, 1
  %113 = icmp eq i7 %111, %112
  %114 = and i80 %6, 18446744073709551615
  store i80 %114, ptr %3, align 16
  %115 = load i128, ptr %3, align 16
  %116 = trunc i128 %115 to i80
  %117 = sub nuw i7 %109, 1
  %118 = zext i7 %117 to i80
  %119 = lshr i80 %116, %118
  store i80 %119, ptr %3, align 16
  %120 = load i128, ptr %3, align 16
  %121 = trunc i128 %120 to i80
  %122 = add nuw i80 %121, 1
  %123 = zext i7 1 to i80
  %124 = lshr i80 %122, %123
  %125 = zext i1 %113 to i80
  %126 = xor i80 %125, -1
  %127 = and i80 %124, %126
  store i80 %127, ptr %3, align 16
  %128 = load i128, ptr %3, align 16
  %129 = trunc i128 %128 to i80
  %130 = or i80 %129, %7
  %131 = bitcast i80 %130 to x86_fp80
  ret x86_fp80 %131

132:                                              ; preds = %96
  %133 = bitcast i80 %7 to x86_fp80
  ret x86_fp80 %133

134:                                              ; preds = %96
  br label %102
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc { ptr, i64 } @mem.sliceAsBytes__anon_1790(ptr nonnull align 4 %0, i64 %1) unnamed_addr #0 {
  %3 = alloca [8 x i8], align 8
  %4 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %5 = insertvalue { ptr, i64 } %4, i64 %1, 1
  %6 = extractvalue { ptr, i64 } %5, 1
  %7 = icmp eq i64 %6, 0
  br i1 %7, label %18, label %19

8:                                                ; preds = %21
  %9 = extractvalue { ptr, i64 } %5, 0
  store ptr %9, ptr %3, align 8
  %10 = extractvalue { ptr, i64 } %5, 1
  %11 = mul nuw i64 %10, 4
  %12 = load ptr, ptr %3, align 8
  %13 = getelementptr inbounds i8, ptr %12, i64 0
  %14 = insertvalue { ptr, i64 } poison, ptr %13, 0
  %15 = insertvalue { ptr, i64 } %14, i64 %11, 1
  ret { ptr, i64 } %15

16:                                               ; preds = %19, %18
  %17 = phi i1 [ true, %18 ], [ false, %19 ]
  br i1 %17, label %20, label %21

18:                                               ; preds = %2
  br label %16

19:                                               ; preds = %2
  br label %16

20:                                               ; preds = %16
  ret { ptr, i64 } { ptr inttoptr (i64 -6148914691236517206 to ptr), i64 0 }

21:                                               ; preds = %16
  br label %8
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @mem.writePackedIntLittle__anon_1792(ptr nonnull align 1 %0, i64 %1, i64 %2, i64 %3) unnamed_addr #0 {
  %5 = alloca [8 x i8], align 8
  %6 = alloca [8 x i8], align 8
  %7 = alloca [16 x i8], align 8
  %8 = alloca [16 x i8], align 8
  %9 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %10 = insertvalue { ptr, i64 } %9, i64 %1, 1
  store { ptr, i64 } %10, ptr %8, align 8
  %11 = urem i64 %2, 8
  %12 = trunc i64 %11 to i3
  %13 = udiv i64 %2, 8
  %14 = load { ptr, i64 }, ptr %8, align 8
  %15 = extractvalue { ptr, i64 } %14, 0
  %16 = getelementptr inbounds i8, ptr %15, i64 %13
  %17 = extractvalue { ptr, i64 } %14, 1
  %18 = sub nuw i64 %17, %13
  %19 = insertvalue { ptr, i64 } poison, ptr %16, 0
  %20 = insertvalue { ptr, i64 } %19, i64 %18, 1
  store { ptr, i64 } %20, ptr %7, align 8
  %21 = extractvalue { ptr, i64 } %20, 0
  %22 = getelementptr inbounds i8, ptr %21, i64 0
  %23 = load i8, ptr %22, align 1
  %24 = zext i3 %12 to i8
  %25 = shl i8 1, %24
  %26 = sub nuw i8 %25, 1
  %27 = and i8 %23, %26
  %28 = zext i3 %12 to i6
  %29 = zext i6 %28 to i64
  %30 = shl i64 %3, %29
  %31 = zext i8 %27 to i64
  %32 = or i64 %30, %31
  store i64 %32, ptr %6, align 8
  %33 = icmp ugt i3 %12, 0
  br i1 %33, label %39, label %60

34:                                               ; preds = %60, %39
  %35 = load { ptr, i64 }, ptr %7, align 8
  %36 = extractvalue { ptr, i64 } %35, 0
  %37 = getelementptr inbounds i8, ptr %36, i64 0
  %38 = load i64, ptr %6, align 8
  store i64 %38, ptr %5, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %37, ptr align 1 %5, i64 8, i1 false)
  ret void

39:                                               ; preds = %4
  %40 = zext i3 %12 to i6
  %41 = load { ptr, i64 }, ptr %7, align 8
  %42 = extractvalue { ptr, i64 } %41, 0
  %43 = getelementptr inbounds i8, ptr %42, i64 8
  %44 = load i8, ptr %43, align 1
  %45 = trunc i6 %40 to i3
  %46 = zext i3 %45 to i8
  %47 = shl i8 1, %46
  %48 = sub nuw i8 %47, 1
  %49 = xor i8 %48, -1
  %50 = and i8 %44, %49
  store i8 %50, ptr %43, align 1
  %51 = load { ptr, i64 }, ptr %7, align 8
  %52 = extractvalue { ptr, i64 } %51, 0
  %53 = getelementptr inbounds i8, ptr %52, i64 8
  %54 = load i8, ptr %53, align 1
  %55 = sub i6 0, %40
  %56 = zext i6 %55 to i64
  %57 = lshr i64 %3, %56
  %58 = trunc i64 %57 to i8
  %59 = or i8 %54, %58
  store i8 %59, ptr %53, align 1
  br label %34

60:                                               ; preds = %4
  br label %34
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.ctlz.i64(i64, i1 immarg) #4

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i64 @math.shl__anon_1845(i64 %0, i7 %1) unnamed_addr #0 {
  %3 = icmp uge i7 %1, -64
  br i1 %3, label %8, label %9

4:                                                ; preds = %9
  %5 = trunc i7 %1 to i6
  %6 = zext i6 %5 to i64
  %7 = shl i64 %0, %6
  ret i64 %7

8:                                                ; preds = %2
  ret i64 0

9:                                                ; preds = %2
  br label %4
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i1 @math.isnan.isNan__anon_1866(x86_fp80 %0) unnamed_addr #0 {
  %2 = fcmp une x86_fp80 %0, %0
  ret i1 %2
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i1 @math.isfinite.isFinite__anon_1870(x86_fp80 %0) unnamed_addr #0 {
  %2 = bitcast x86_fp80 %0 to i80
  %3 = and i80 %2, 604462909807314587353087
  %4 = icmp ult i80 %3, 604453686435277732577280
  ret i1 %4
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i80 @llvm.ctlz.i80(i80, i1 immarg) #4

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #4

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i80 @llvm.cttz.i80(i80, i1 immarg) #4

attributes #0 = { nosanitize_coverage nounwind skipprofile uwtable "frame-pointer"="all" "target-cpu"="raptorlake" "target-features"="+64bit,+adx,+aes,+allow-light-256-bit,+avx,+avx2,+avxvnni,+bmi,+bmi2,+clflushopt,+clwb,+cmov,+crc32,+cx16,+cx8,+f16c,+false-deps-perm,+false-deps-popcnt,+fast-15bytenop,+fast-gather,+fast-scalar-fsqrt,+fast-shld-rotate,+fast-variable-crosslane-shuffle,+fast-variable-perlane-shuffle,+fast-vector-fsqrt,+fma,+fsgsbase,+fxsr,+gfni,+hreset,+idivq-to-divl,+invpcid,+kl,+lzcnt,+macrofusion,+mmx,+movbe,+movdir64b,+movdiri,+no-bypass-delay-blend,+no-bypass-delay-mov,+no-bypass-delay-shuffle,+nopl,+pclmul,+pconfig,+pku,+popcnt,+prefer-movmsk-over-vtest,+prfchw,+ptwrite,+rdpid,+rdrnd,+rdseed,+sahf,+serialize,+sha,+shstk,+slow-3ops-lea,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+tuning-fast-imm-vector-shift,+vaes,+vpclmulqdq,+vzeroupper,+waitpkg,+widekl,+x87,+xsave,+xsavec,+xsaveopt,+xsaves,-16bit-mode,-32bit-mode,-amx-avx512,-amx-bf16,-amx-complex,-amx-fp16,-amx-fp8,-amx-int8,-amx-movrs,-amx-tf32,-amx-tile,-amx-transpose,-avx10.1-256,-avx10.1-512,-avx10.2-256,-avx10.2-512,-avx512bf16,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512f,-avx512fp16,-avx512ifma,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vp2intersect,-avx512vpopcntdq,-avxifma,-avxneconvert,-avxvnniint16,-avxvnniint8,-branch-hint,-branchfusion,-ccmp,-cf,-cldemote,-clzero,-cmpccxadd,-egpr,-enqcmd,-ermsb,-evex512,-false-deps-getmant,-false-deps-lzcnt-tzcnt,-false-deps-mulc,-false-deps-mullq,-false-deps-range,-fast-11bytenop,-fast-7bytenop,-fast-bextr,-fast-dpwssd,-fast-hops,-fast-imm16,-fast-lzcnt,-fast-movbe,-fast-scalar-shift-masks,-fast-vector-shift-masks,-faster-shift-than-shuffle,-fma4,-fsrm,-harden-sls-ijmp,-harden-sls-ret,-idivl-to-divb,-inline-asm-use-gpr32,-lea-sp,-lea-uses-ag,-lvi-cfi,-lvi-load-hardening,-lwp,-movrs,-mwaitx,-ndd,-nf,-no-bypass-delay,-pad-short-functions,-ppx,-prefer-128-bit,-prefer-256-bit,-prefer-mask-registers,-prefer-no-gather,-prefer-no-scatter,-prefetchi,-push2pop2,-raoint,-rdpru,-retpoline,-retpoline-external-thunk,-retpoline-indirect-branches,-retpoline-indirect-calls,-rtm,-sbb-dep-breaking,-seses,-sgx,-sha512,-slow-incdec,-slow-lea,-slow-pmaddwd,-slow-pmulld,-slow-shld,-slow-two-mem-ops,-slow-unaligned-mem-16,-slow-unaligned-mem-32,-sm3,-sm4,-soft-float,-sse4a,-sse-unaligned-mem,-tagged-globals,-tbm,-tsxldtrk,-uintr,-use-glm-div-sqrt-costs,-use-slm-arith-costs,-usermsr,-wbnoinvd,-xop,-zu" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { noinline nosanitize_coverage nounwind skipprofile uwtable "frame-pointer"="all" "target-cpu"="raptorlake" "target-features"="+64bit,+adx,+aes,+allow-light-256-bit,+avx,+avx2,+avxvnni,+bmi,+bmi2,+clflushopt,+clwb,+cmov,+crc32,+cx16,+cx8,+f16c,+false-deps-perm,+false-deps-popcnt,+fast-15bytenop,+fast-gather,+fast-scalar-fsqrt,+fast-shld-rotate,+fast-variable-crosslane-shuffle,+fast-variable-perlane-shuffle,+fast-vector-fsqrt,+fma,+fsgsbase,+fxsr,+gfni,+hreset,+idivq-to-divl,+invpcid,+kl,+lzcnt,+macrofusion,+mmx,+movbe,+movdir64b,+movdiri,+no-bypass-delay-blend,+no-bypass-delay-mov,+no-bypass-delay-shuffle,+nopl,+pclmul,+pconfig,+pku,+popcnt,+prefer-movmsk-over-vtest,+prfchw,+ptwrite,+rdpid,+rdrnd,+rdseed,+sahf,+serialize,+sha,+shstk,+slow-3ops-lea,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+tuning-fast-imm-vector-shift,+vaes,+vpclmulqdq,+vzeroupper,+waitpkg,+widekl,+x87,+xsave,+xsavec,+xsaveopt,+xsaves,-16bit-mode,-32bit-mode,-amx-avx512,-amx-bf16,-amx-complex,-amx-fp16,-amx-fp8,-amx-int8,-amx-movrs,-amx-tf32,-amx-tile,-amx-transpose,-avx10.1-256,-avx10.1-512,-avx10.2-256,-avx10.2-512,-avx512bf16,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512f,-avx512fp16,-avx512ifma,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vp2intersect,-avx512vpopcntdq,-avxifma,-avxneconvert,-avxvnniint16,-avxvnniint8,-branch-hint,-branchfusion,-ccmp,-cf,-cldemote,-clzero,-cmpccxadd,-egpr,-enqcmd,-ermsb,-evex512,-false-deps-getmant,-false-deps-lzcnt-tzcnt,-false-deps-mulc,-false-deps-mullq,-false-deps-range,-fast-11bytenop,-fast-7bytenop,-fast-bextr,-fast-dpwssd,-fast-hops,-fast-imm16,-fast-lzcnt,-fast-movbe,-fast-scalar-shift-masks,-fast-vector-shift-masks,-faster-shift-than-shuffle,-fma4,-fsrm,-harden-sls-ijmp,-harden-sls-ret,-idivl-to-divb,-inline-asm-use-gpr32,-lea-sp,-lea-uses-ag,-lvi-cfi,-lvi-load-hardening,-lwp,-movrs,-mwaitx,-ndd,-nf,-no-bypass-delay,-pad-short-functions,-ppx,-prefer-128-bit,-prefer-256-bit,-prefer-mask-registers,-prefer-no-gather,-prefer-no-scatter,-prefetchi,-push2pop2,-raoint,-rdpru,-retpoline,-retpoline-external-thunk,-retpoline-indirect-branches,-retpoline-indirect-calls,-rtm,-sbb-dep-breaking,-seses,-sgx,-sha512,-slow-incdec,-slow-lea,-slow-pmaddwd,-slow-pmulld,-slow-shld,-slow-two-mem-ops,-slow-unaligned-mem-16,-slow-unaligned-mem-32,-sm3,-sm4,-soft-float,-sse4a,-sse-unaligned-mem,-tagged-globals,-tbm,-tsxldtrk,-uintr,-use-glm-div-sqrt-costs,-use-slm-arith-costs,-usermsr,-wbnoinvd,-xop,-zu" }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #5 = { noinline }

!llvm.module.flags = !{}
```

</details>

Note that the call at `%34 = call fastcc x86_fp80 @math.ldexp.ldexp__anon_1741(x86_fp80 %33, i32 64) #5` is `noinline`. If you remove that attribute and run the program through `opt -O3`, the call is inlined, but the result it produces at runtime is zero instead of the expected `15625000000000000000`.

A self-contained, runnable repro for `x86_64-linux` can be found [here](https://gist.github.com/alexrp/5978cd4aafbbe342424b8d3ebdce44db). Run with `clang -nostdlib -fuse-ld=lld repro.ll && ./a.out` (which will produce the expected result) and then add `-O3` to see the bug. Source code for this repro in Zig form can be found [here](https://gist.github.com/alexrp/12628795e0b497e46598b614f5f76156).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Alex Rønne Petersen (alexrp)

<details>
Consider this case:

```llvm
; ModuleID = 'repro.bc'
source_filename = "repro"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux6.8.0-gnu2.39.0"

%Target.Cpu.Feature.Set = type { [5 x i64] }
%Target.Cpu = type { ptr, %Target.Cpu.Feature.Set, i6, [7 x i8] }
%Target.Cpu.Model = type { { ptr, i64 }, { ptr, i64 }, %Target.Cpu.Feature.Set }
%Target.Os = type { %Target.Os.VersionRange, i6, [7 x i8] }
%Target.Os.VersionRange = type { %Target.Os.LinuxVersionRange }
%Target.Os.LinuxVersionRange = type { %SemanticVersion.Range, %SemanticVersion, i32, [4 x i8] }
%SemanticVersion.Range = type { %SemanticVersion, %SemanticVersion }
%SemanticVersion = type { i64, i64, i64, { ptr, i64 }, { ptr, i64 } }
%Target.DynamicLinker = type { [255 x i8], i8 }
%Target = type { %Target.Cpu, %Target.Os, i5, i4, %Target.DynamicLinker, [6 x i8] }
%builtin.CallingConvention.CommonOptions = type { { i64, i8, [7 x i8] } }
%"math.frexp.Frexp(f80)" = type { x86_fp80, i32, [12 x i8] }

@<!-- -->0 = private unnamed_addr constant { i64, i16, [6 x i8] } { i64 undef, i16 1, [6 x i8] undef }, align 8
@<!-- -->builtin.zig_backend = internal unnamed_addr constant i64 2, align 8
@<!-- -->start.simplified_logic = internal unnamed_addr constant i1 false, align 1
@<!-- -->builtin.output_mode = internal unnamed_addr constant i2 -2, align 1
@<!-- -->Target.Cpu.Feature.Set.empty = internal unnamed_addr constant %Target.Cpu.Feature.Set zeroinitializer, align 8
@<!-- -->builtin.cpu = internal unnamed_addr constant %Target.Cpu { ptr @<!-- -->Target.x86.cpu.raptorlake, %Target.Cpu.Feature.Set { [5 x i64] [i64 2116801776035561712, i64 -2445523138194684734, i64 -4456355804684576701, i64 987, i64 0] }, i6 -22, [7 x i8] undef }, align 8
@<!-- -->Target.x86.cpu.raptorlake = internal unnamed_addr constant %Target.Cpu.Model { { ptr, i64 } { ptr @<!-- -->__anon_1971, i64 10 }, { ptr, i64 } { ptr @<!-- -->__anon_1971, i64 10 }, %Target.Cpu.Feature.Set { [5 x i64] [i64 680153494895722672, i64 -7057210256133700414, i64 -4467544435008825277, i64 923, i64 0] } }, align 8
@<!-- -->builtin.os = internal unnamed_addr constant %Target.Os { %Target.Os.VersionRange { %Target.Os.LinuxVersionRange { %SemanticVersion.Range { %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer } }, %SemanticVersion { i64 2, i64 39, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, i32 14, [4 x i8] undef } }, i6 9, [7 x i8] undef }, align 8
@<!-- -->builtin.abi = internal unnamed_addr constant i5 1, align 1
@<!-- -->builtin.object_format = internal unnamed_addr constant i4 2, align 1
@<!-- -->Target.DynamicLinker.none = internal unnamed_addr constant %Target.DynamicLinker { [255 x i8] undef, i8 0 }, align 1
@<!-- -->builtin.target = internal unnamed_addr constant %Target { %Target.Cpu { ptr @<!-- -->Target.x86.cpu.raptorlake, %Target.Cpu.Feature.Set { [5 x i64] [i64 2116801776035561712, i64 -2445523138194684734, i64 -4456355804684576701, i64 987, i64 0] }, i6 -22, [7 x i8] undef }, %Target.Os { %Target.Os.VersionRange { %Target.Os.LinuxVersionRange { %SemanticVersion.Range { %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, %SemanticVersion { i64 6, i64 8, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer } }, %SemanticVersion { i64 2, i64 39, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, i32 14, [4 x i8] undef } }, i6 9, [7 x i8] undef }, i5 1, i4 2, %Target.DynamicLinker { [255 x i8] [i8 47, i8 108, i8 105, i8 98, i8 54, i8 52, i8 47, i8 108, i8 100, i8 45, i8 108, i8 105, i8 110, i8 117, i8 120, i8 45, i8 120, i8 56, i8 54, i8 45, i8 54, i8 52, i8 46, i8 115, i8 111, i8 46, i8 50, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef, i8 undef], i8 27 }, [6 x i8] undef }, align 8
@<!-- -->builtin.CallingConvention.c = internal unnamed_addr constant { &lt;{ %builtin.CallingConvention.CommonOptions, [8 x i8] }&gt;, i8, [7 x i8] } { &lt;{ %builtin.CallingConvention.CommonOptions, [8 x i8] }&gt; &lt;{ %builtin.CallingConvention.CommonOptions { { i64, i8, [7 x i8] } { i64 undef, i8 0, [7 x i8] undef } }, [8 x i8] undef }&gt;, i8 4, [7 x i8] undef }, align 8
@<!-- -->mem.native_endian = internal unnamed_addr constant i1 true, align 1
@<!-- -->__anon_1971 = internal unnamed_addr constant [11 x i8] c"raptorlake\00", align 1

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define dso_local i128 @<!-- -->foo() #<!-- -->0 {
  %1 = alloca [32 x i8], align 16
  call void @<!-- -->llvm.memset.p0.i64(ptr align 16 %1, i8 undef, i64 32, i1 false)
  call void @<!-- -->__fixunsxfei(ptr nonnull align 4 %1, i64 256, x86_fp80 0xK4044D8D726B7177A8000)
  %2 = load i256, ptr %1, align 16
  %3 = trunc i256 %2 to i128
  ret i128 %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #<!-- -->1

; Function Attrs: noinline nosanitize_coverage nounwind skipprofile uwtable
define dso_local void @<!-- -->__fixunsxfei(ptr nonnull align 4 %0, i64 %1, x86_fp80 %2) #<!-- -->2 {
  %4 = alloca [16 x i8], align 8
  %5 = alloca [8 x i8], align 8
  store ptr %0, ptr %5, align 8
  call fastcc void @<!-- -->math.divCeil__anon_1677(ptr sret({ i64, i16, [6 x i8] }) %4, i64 %1, i64 32)
  %6 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %4, i32 0, i32 1
  %7 = load i16, ptr %6, align 2
  %8 = icmp eq i16 %7, 0
  br i1 %8, label %17, label %20

9:                                                ; preds = %17
  %10 = phi i64 [ %19, %17 ]
  %11 = load ptr, ptr %5, align 8
  %12 = getelementptr inbounds i32, ptr %11, i64 0
  %13 = insertvalue { ptr, i64 } poison, ptr %12, 0
  %14 = insertvalue { ptr, i64 } %13, i64 %10, 1
  %15 = extractvalue { ptr, i64 } %14, 0
  %16 = extractvalue { ptr, i64 } %14, 1
  call fastcc void @<!-- -->repro.bigIntFromFloat__anon_1692(ptr nonnull align 4 %15, i64 %16, x86_fp80 %2)
  ret void

17:                                               ; preds = %3
  %18 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %4, i32 0, i32 0
  %19 = load i64, ptr %18, align 8
  br label %9

20:                                               ; preds = %3
  unreachable
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @<!-- -->math.divCeil__anon_1677(ptr noalias nonnull sret({ i64, i16, [6 x i8] }) %0, i64 %1, i64 %2) unnamed_addr #<!-- -->0 {
  %4 = icmp eq i64 %2, 0
  br i1 %4, label %7, label %8

5:                                                ; preds = %8
  %6 = icmp ugt i64 %1, 0
  br i1 %6, label %15, label %17

7:                                                ; preds = %3
  call void @<!-- -->llvm.memcpy.p0.p0.i64(ptr align 8 %0, ptr align 8 @<!-- -->0, i64 16, i1 false)
  ret void

8:                                                ; preds = %3
  br label %5

9:                                                ; preds = %24
  %10 = udiv i64 %1, %2
  %11 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 1
  store i16 0, ptr %11, align 2
  %12 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 0
  store i64 %10, ptr %12, align 8
  ret void

13:                                               ; preds = %17, %15
  %14 = phi i1 [ %16, %15 ], [ false, %17 ]
  br i1 %14, label %18, label %24

15:                                               ; preds = %5
  %16 = icmp ugt i64 %2, 0
  br label %13

17:                                               ; preds = %5
  br label %13

18:                                               ; preds = %13
  %19 = sub nuw i64 %1, 1
  %20 = udiv i64 %19, %2
  %21 = add nuw i64 %20, 1
  %22 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 1
  store i16 0, ptr %22, align 2
  %23 = getelementptr inbounds { i64, i16, [6 x i8] }, ptr %0, i32 0, i32 0
  store i64 %21, ptr %23, align 8
  ret void

24:                                               ; preds = %13
  br label %9
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #<!-- -->3

; Function Attrs: noinline nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @<!-- -->repro.bigIntFromFloat__anon_1692(ptr nonnull align 4 %0, i64 %1, x86_fp80 %2) unnamed_addr #<!-- -->2 {
  %4 = alloca [16 x i8], align 8
  %5 = alloca [16 x i8], align 16
  %6 = alloca [32 x i8], align 16
  %7 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %8 = insertvalue { ptr, i64 } %7, i64 %1, 1
  call fastcc void @<!-- -->math.frexp.frexp__anon_1724(ptr sret(%"math.frexp.Frexp(f80)") %6, x86_fp80 %2)
  %9 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %6, i32 0, i32 1
  %10 = load i32, ptr %9, align 4
  %11 = sub nsw i32 %10, 64
  %12 = call i32 @<!-- -->llvm.smax.i32(i32 0, i32 %11)
  %13 = trunc i32 %12 to i31
  switch i31 %13, label %28 [
    i31 0, label %27
  ]

14:                                               ; preds = %28, %27
  %15 = phi x86_fp80 [ %2, %27 ], [ %34, %28 ]
  %16 = fptoui x86_fp80 %15 to i64
  %17 = extractvalue { ptr, i64 } %8, 0
  %18 = extractvalue { ptr, i64 } %8, 1
  %19 = mul nuw i64 %18, 4
  call void @<!-- -->llvm.memset.p0.i64(ptr align 4 %17, i8 0, i64 %19, i1 false)
  %20 = extractvalue { ptr, i64 } %8, 0
  %21 = extractvalue { ptr, i64 } %8, 1
  %22 = call fastcc { ptr, i64 } @<!-- -->mem.sliceAsBytes__anon_1790(ptr nonnull align 4 %20, i64 %21)
  store { ptr, i64 } %22, ptr %4, align 8
  %23 = load { ptr, i64 }, ptr %4, align 8
  %24 = zext i31 %13 to i64
  %25 = extractvalue { ptr, i64 } %23, 0
  %26 = extractvalue { ptr, i64 } %23, 1
  call fastcc void @<!-- -->mem.writePackedIntLittle__anon_1792(ptr nonnull align 1 %25, i64 %26, i64 %24, i64 %16)
  ret void

27:                                               ; preds = %3
  br label %14

28:                                               ; preds = %3
  %29 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %6, i32 0, i32 0
  %30 = load x86_fp80, ptr %29, align 16
  %31 = getelementptr inbounds { x86_fp80 }, ptr %5, i32 0, i32 0
  store x86_fp80 %30, ptr %31, align 16
  %32 = getelementptr inbounds { x86_fp80 }, ptr %5, i32 0, i32 0
  %33 = load x86_fp80, ptr %32, align 16
  %34 = call fastcc x86_fp80 @<!-- -->math.ldexp.ldexp__anon_1741(x86_fp80 %33, i32 64) #<!-- -->5
  br label %14
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @<!-- -->math.frexp.frexp__anon_1724(ptr noalias nonnull sret(%"math.frexp.Frexp(f80)") %0, x86_fp80 %1) unnamed_addr #<!-- -->0 {
  %3 = alloca [16 x i8], align 16
  %4 = alloca [32 x i8], align 16
  call void @<!-- -->llvm.memset.p0.i64(ptr align 16 %4, i8 undef, i64 32, i1 false)
  %5 = bitcast x86_fp80 %1 to i80
  store i80 %5, ptr %3, align 16
  %6 = load i128, ptr %3, align 16
  %7 = trunc i128 %6 to i80
  %8 = trunc i80 %7 to i64
  %9 = load i128, ptr %3, align 16
  %10 = trunc i128 %9 to i80
  %11 = zext i7 -64 to i80
  %12 = lshr i80 %10, %11
  %13 = trunc i80 %12 to i15
  switch i15 %13, label %27 [
    i15 0, label %19
    i15 -1, label %21
  ]

14:                                               ; preds = %58, %37, %27
  %15 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 0
  %16 = load i128, ptr %3, align 16
  %17 = trunc i128 %16 to i80
  %18 = bitcast i80 %17 to x86_fp80
  store x86_fp80 %18, ptr %15, align 16
  call void @<!-- -->llvm.memcpy.p0.p0.i64(ptr align 16 %0, ptr align 16 %4, i64 32, i1 false)
  ret void

19:                                               ; preds = %2
  %20 = icmp ne i64 %8, 0
  br i1 %20, label %38, label %56

21:                                               ; preds = %2
  %22 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  call void @<!-- -->llvm.memset.p0.i64(ptr align 4 %22, i8 undef, i64 4, i1 false)
  %23 = load i128, ptr %3, align 16
  %24 = trunc i128 %23 to i80
  %25 = trunc i80 %24 to i63
  %26 = icmp eq i63 %25, 0
  br i1 %26, label %59, label %61

27:                                               ; preds = %2
  %28 = load i128, ptr %3, align 16
  %29 = trunc i128 %28 to i80
  %30 = and i80 %29, -604444463063240877801473
  store i80 %30, ptr %3, align 16
  %31 = load i128, ptr %3, align 16
  %32 = trunc i128 %31 to i80
  %33 = or i80 %32, 302194561415509874573312
  store i80 %33, ptr %3, align 16
  %34 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  %35 = zext i15 %13 to i32
  %36 = sub nsw i32 %35, 16382
  store i32 %36, ptr %34, align 4
  br label %14

37:                                               ; preds = %56, %38
  br label %14

38:                                               ; preds = %19
  %39 = call i64 @<!-- -->llvm.ctlz.i64(i64 %8, i1 false)
  %40 = trunc i64 %39 to i7
  %41 = load i128, ptr %3, align 16
  %42 = trunc i128 %41 to i80
  %43 = and i80 %42, -604462909807314587353088
  store i80 %43, ptr %3, align 16
  %44 = load i128, ptr %3, align 16
  %45 = trunc i128 %44 to i80
  %46 = or i80 %45, 302194561415509874573312
  store i80 %46, ptr %3, align 16
  %47 = load i128, ptr %3, align 16
  %48 = trunc i128 %47 to i80
  %49 = call fastcc i64 @<!-- -->math.shl__anon_1845(i64 %8, i7 %40)
  %50 = zext i64 %49 to i80
  %51 = or i80 %48, %50
  store i80 %51, ptr %3, align 16
  %52 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  %53 = zext i7 %40 to i32
  %54 = sub nsw i32 -16382, %53
  %55 = add nsw i32 %54, 1
  store i32 %55, ptr %52, align 4
  br label %37

56:                                               ; preds = %19
  %57 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  store i32 0, ptr %57, align 4
  br label %37

58:                                               ; preds = %61, %59
  br label %14

59:                                               ; preds = %21
  %60 = getelementptr inbounds %"math.frexp.Frexp(f80)", ptr %4, i32 0, i32 1
  store i32 0, ptr %60, align 4
  br label %58

61:                                               ; preds = %21
  br label %58
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.smax.i32(i32, i32) #<!-- -->4

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc x86_fp80 @<!-- -->math.ldexp.ldexp__anon_1741(x86_fp80 %0, i32 %1) unnamed_addr #<!-- -->0 {
  %3 = alloca [16 x i8], align 16
  %4 = alloca [16 x i8], align 16
  %5 = alloca [4 x i8], align 4
  %6 = bitcast x86_fp80 %0 to i80
  %7 = and i80 %6, -604462909807314587353088
  %8 = call fastcc i1 @<!-- -->math.isnan.isNan__anon_1866(x86_fp80 %0)
  br i1 %8, label %19, label %20

9:                                                ; preds = %24
  %10 = zext i7 1 to i80
  %11 = shl i80 %6, %10
  %12 = zext i7 -63 to i80
  %13 = lshr i80 %11, %12
  %14 = trunc i80 %13 to i32
  store i32 %14, ptr %5, align 4
  %15 = load i32, ptr %5, align 4
  %16 = icmp eq i32 %15, 0
  br i1 %16, label %27, label %36

17:                                               ; preds = %20, %19
  %18 = phi i1 [ true, %19 ], [ %22, %20 ]
  br i1 %18, label %23, label %24

19:                                               ; preds = %2
  br label %17

20:                                               ; preds = %2
  %21 = call fastcc i1 @<!-- -->math.isfinite.isFinite__anon_1870(x86_fp80 %0)
  %22 = xor i1 %21, true
  br label %17

23:                                               ; preds = %17
  ret x86_fp80 %0

24:                                               ; preds = %17
  br label %9

25:                                               ; preds = %36, %27
  %26 = icmp sge i32 %1, 0
  br i1 %26, label %37, label %41

27:                                               ; preds = %9
  %28 = load i32, ptr %5, align 4
  %29 = zext i7 1 to i80
  %30 = shl i80 %6, %29
  %31 = call i80 @<!-- -->llvm.ctlz.i80(i80 %30, i1 false)
  %32 = trunc i80 %31 to i7
  %33 = zext i7 %32 to i32
  %34 = sub nsw i32 16, %33
  %35 = add nsw i32 %28, %34
  store i32 %35, ptr %5, align 4
  br label %25

36:                                               ; preds = %9
  br label %25

37:                                               ; preds = %25
  %38 = load i32, ptr %5, align 4
  %39 = sub nsw i32 32766, %38
  %40 = icmp sgt i32 %1, %39
  br i1 %40, label %51, label %54

41:                                               ; preds = %25
  %42 = load i32, ptr %5, align 4
  %43 = sub nsw i32 0, %42
  %44 = icmp sle i32 %1, %43
  br i1 %44, label %96, label %101

45:                                               ; preds = %58
  %46 = sext i32 %1 to i80
  %47 = zext i7 -64 to i80
  %48 = shl i80 %46, %47
  %49 = add nuw i80 %6, %48
  %50 = bitcast i80 %49 to x86_fp80
  ret x86_fp80 %50

51:                                               ; preds = %37
  %52 = or i80 604453686435277732577280, %7
  %53 = bitcast i80 %52 to x86_fp80
  ret x86_fp80 %53

54:                                               ; preds = %37
  %55 = load i32, ptr %5, align 4
  %56 = add nsw i32 %55, %1
  %57 = icmp sle i32 %56, 0
  br i1 %57, label %59, label %65

58:                                               ; preds = %68
  br label %45

59:                                               ; preds = %54
  %60 = trunc i32 %1 to i7
  %61 = zext i7 %60 to i80
  %62 = shl i80 %6, %61
  %63 = or i80 %62, %7
  %64 = bitcast i80 %63 to x86_fp80
  ret x86_fp80 %64

65:                                               ; preds = %54
  %66 = load i32, ptr %5, align 4
  %67 = icmp sle i32 %66, 0
  br i1 %67, label %69, label %88

68:                                               ; preds = %88
  br label %58

69:                                               ; preds = %65
  %70 = load i32, ptr %5, align 4
  %71 = add nsw i32 %1, %70
  %72 = sext i32 %71 to i80
  %73 = zext i7 -64 to i80
  %74 = shl i80 %72, %73
  store i80 %74, ptr %4, align 16
  %75 = load i128, ptr %4, align 16
  %76 = trunc i128 %75 to i80
  %77 = load i32, ptr %5, align 4
  %78 = sub nsw i32 1, %77
  %79 = trunc i32 %78 to i7
  %80 = zext i7 %79 to i80
  %81 = shl i80 %6, %80
  %82 = and i80 %81, 18446744073709551615
  %83 = or i80 %76, %82
  store i80 %83, ptr %4, align 16
  %84 = load i128, ptr %4, align 16
  %85 = trunc i128 %84 to i80
  %86 = or i80 %85, %7
  %87 = bitcast i80 %86 to x86_fp80
  ret x86_fp80 %87

88:                                               ; preds = %65
  br label %68

89:                                               ; preds = %101
  %90 = sub nsw i32 0, %1
  %91 = sext i32 %90 to i80
  %92 = zext i7 -64 to i80
  %93 = shl i80 %91, %92
  %94 = sub nuw i80 %6, %93
  %95 = bitcast i80 %94 to x86_fp80
  ret x86_fp80 %95

96:                                               ; preds = %41
  %97 = load i32, ptr %5, align 4
  %98 = add nsw i32 64, %97
  %99 = sub nsw i32 0, %98
  %100 = icmp slt i32 %1, %99
  br i1 %100, label %132, label %134

101:                                              ; preds = %41
  br label %89

102:                                              ; preds = %134
  %103 = sub nsw i32 0, %1
  %104 = load i32, ptr %5, align 4
  %105 = add nsw i32 %104, %1
  %106 = sub nsw i32 0, %105
  %107 = add nsw i32 %106, 1
  %108 = call i32 @<!-- -->llvm.smin.i32(i32 %103, i32 %107)
  %109 = trunc i32 %108 to i7
  %110 = call i80 @<!-- -->llvm.cttz.i80(i80 %6, i1 false)
  %111 = trunc i80 %110 to i7
  %112 = sub nuw i7 %109, 1
  %113 = icmp eq i7 %111, %112
  %114 = and i80 %6, 18446744073709551615
  store i80 %114, ptr %3, align 16
  %115 = load i128, ptr %3, align 16
  %116 = trunc i128 %115 to i80
  %117 = sub nuw i7 %109, 1
  %118 = zext i7 %117 to i80
  %119 = lshr i80 %116, %118
  store i80 %119, ptr %3, align 16
  %120 = load i128, ptr %3, align 16
  %121 = trunc i128 %120 to i80
  %122 = add nuw i80 %121, 1
  %123 = zext i7 1 to i80
  %124 = lshr i80 %122, %123
  %125 = zext i1 %113 to i80
  %126 = xor i80 %125, -1
  %127 = and i80 %124, %126
  store i80 %127, ptr %3, align 16
  %128 = load i128, ptr %3, align 16
  %129 = trunc i128 %128 to i80
  %130 = or i80 %129, %7
  %131 = bitcast i80 %130 to x86_fp80
  ret x86_fp80 %131

132:                                              ; preds = %96
  %133 = bitcast i80 %7 to x86_fp80
  ret x86_fp80 %133

134:                                              ; preds = %96
  br label %102
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc { ptr, i64 } @<!-- -->mem.sliceAsBytes__anon_1790(ptr nonnull align 4 %0, i64 %1) unnamed_addr #<!-- -->0 {
  %3 = alloca [8 x i8], align 8
  %4 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %5 = insertvalue { ptr, i64 } %4, i64 %1, 1
  %6 = extractvalue { ptr, i64 } %5, 1
  %7 = icmp eq i64 %6, 0
  br i1 %7, label %18, label %19

8:                                                ; preds = %21
  %9 = extractvalue { ptr, i64 } %5, 0
  store ptr %9, ptr %3, align 8
  %10 = extractvalue { ptr, i64 } %5, 1
  %11 = mul nuw i64 %10, 4
  %12 = load ptr, ptr %3, align 8
  %13 = getelementptr inbounds i8, ptr %12, i64 0
  %14 = insertvalue { ptr, i64 } poison, ptr %13, 0
  %15 = insertvalue { ptr, i64 } %14, i64 %11, 1
  ret { ptr, i64 } %15

16:                                               ; preds = %19, %18
  %17 = phi i1 [ true, %18 ], [ false, %19 ]
  br i1 %17, label %20, label %21

18:                                               ; preds = %2
  br label %16

19:                                               ; preds = %2
  br label %16

20:                                               ; preds = %16
  ret { ptr, i64 } { ptr inttoptr (i64 -6148914691236517206 to ptr), i64 0 }

21:                                               ; preds = %16
  br label %8
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc void @<!-- -->mem.writePackedIntLittle__anon_1792(ptr nonnull align 1 %0, i64 %1, i64 %2, i64 %3) unnamed_addr #<!-- -->0 {
  %5 = alloca [8 x i8], align 8
  %6 = alloca [8 x i8], align 8
  %7 = alloca [16 x i8], align 8
  %8 = alloca [16 x i8], align 8
  %9 = insertvalue { ptr, i64 } poison, ptr %0, 0
  %10 = insertvalue { ptr, i64 } %9, i64 %1, 1
  store { ptr, i64 } %10, ptr %8, align 8
  %11 = urem i64 %2, 8
  %12 = trunc i64 %11 to i3
  %13 = udiv i64 %2, 8
  %14 = load { ptr, i64 }, ptr %8, align 8
  %15 = extractvalue { ptr, i64 } %14, 0
  %16 = getelementptr inbounds i8, ptr %15, i64 %13
  %17 = extractvalue { ptr, i64 } %14, 1
  %18 = sub nuw i64 %17, %13
  %19 = insertvalue { ptr, i64 } poison, ptr %16, 0
  %20 = insertvalue { ptr, i64 } %19, i64 %18, 1
  store { ptr, i64 } %20, ptr %7, align 8
  %21 = extractvalue { ptr, i64 } %20, 0
  %22 = getelementptr inbounds i8, ptr %21, i64 0
  %23 = load i8, ptr %22, align 1
  %24 = zext i3 %12 to i8
  %25 = shl i8 1, %24
  %26 = sub nuw i8 %25, 1
  %27 = and i8 %23, %26
  %28 = zext i3 %12 to i6
  %29 = zext i6 %28 to i64
  %30 = shl i64 %3, %29
  %31 = zext i8 %27 to i64
  %32 = or i64 %30, %31
  store i64 %32, ptr %6, align 8
  %33 = icmp ugt i3 %12, 0
  br i1 %33, label %39, label %60

34:                                               ; preds = %60, %39
  %35 = load { ptr, i64 }, ptr %7, align 8
  %36 = extractvalue { ptr, i64 } %35, 0
  %37 = getelementptr inbounds i8, ptr %36, i64 0
  %38 = load i64, ptr %6, align 8
  store i64 %38, ptr %5, align 8
  call void @<!-- -->llvm.memcpy.p0.p0.i64(ptr align 1 %37, ptr align 1 %5, i64 8, i1 false)
  ret void

39:                                               ; preds = %4
  %40 = zext i3 %12 to i6
  %41 = load { ptr, i64 }, ptr %7, align 8
  %42 = extractvalue { ptr, i64 } %41, 0
  %43 = getelementptr inbounds i8, ptr %42, i64 8
  %44 = load i8, ptr %43, align 1
  %45 = trunc i6 %40 to i3
  %46 = zext i3 %45 to i8
  %47 = shl i8 1, %46
  %48 = sub nuw i8 %47, 1
  %49 = xor i8 %48, -1
  %50 = and i8 %44, %49
  store i8 %50, ptr %43, align 1
  %51 = load { ptr, i64 }, ptr %7, align 8
  %52 = extractvalue { ptr, i64 } %51, 0
  %53 = getelementptr inbounds i8, ptr %52, i64 8
  %54 = load i8, ptr %53, align 1
  %55 = sub i6 0, %40
  %56 = zext i6 %55 to i64
  %57 = lshr i64 %3, %56
  %58 = trunc i64 %57 to i8
  %59 = or i8 %54, %58
  store i8 %59, ptr %53, align 1
  br label %34

60:                                               ; preds = %4
  br label %34
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @<!-- -->llvm.ctlz.i64(i64, i1 immarg) #<!-- -->4

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i64 @<!-- -->math.shl__anon_1845(i64 %0, i7 %1) unnamed_addr #<!-- -->0 {
  %3 = icmp uge i7 %1, -64
  br i1 %3, label %8, label %9

4:                                                ; preds = %9
  %5 = trunc i7 %1 to i6
  %6 = zext i6 %5 to i64
  %7 = shl i64 %0, %6
  ret i64 %7

8:                                                ; preds = %2
  ret i64 0

9:                                                ; preds = %2
  br label %4
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i1 @<!-- -->math.isnan.isNan__anon_1866(x86_fp80 %0) unnamed_addr #<!-- -->0 {
  %2 = fcmp une x86_fp80 %0, %0
  ret i1 %2
}

; Function Attrs: nosanitize_coverage nounwind skipprofile uwtable
define internal fastcc i1 @<!-- -->math.isfinite.isFinite__anon_1870(x86_fp80 %0) unnamed_addr #<!-- -->0 {
  %2 = bitcast x86_fp80 %0 to i80
  %3 = and i80 %2, 604462909807314587353087
  %4 = icmp ult i80 %3, 604453686435277732577280
  ret i1 %4
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i80 @<!-- -->llvm.ctlz.i80(i80, i1 immarg) #<!-- -->4

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.smin.i32(i32, i32) #<!-- -->4

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i80 @<!-- -->llvm.cttz.i80(i80, i1 immarg) #<!-- -->4

attributes #<!-- -->0 = { nosanitize_coverage nounwind skipprofile uwtable "frame-pointer"="all" "target-cpu"="raptorlake" "target-features"="+64bit,+adx,+aes,+allow-light-256-bit,+avx,+avx2,+avxvnni,+bmi,+bmi2,+clflushopt,+clwb,+cmov,+crc32,+cx16,+cx8,+f16c,+false-deps-perm,+false-deps-popcnt,+fast-15bytenop,+fast-gather,+fast-scalar-fsqrt,+fast-shld-rotate,+fast-variable-crosslane-shuffle,+fast-variable-perlane-shuffle,+fast-vector-fsqrt,+fma,+fsgsbase,+fxsr,+gfni,+hreset,+idivq-to-divl,+invpcid,+kl,+lzcnt,+macrofusion,+mmx,+movbe,+movdir64b,+movdiri,+no-bypass-delay-blend,+no-bypass-delay-mov,+no-bypass-delay-shuffle,+nopl,+pclmul,+pconfig,+pku,+popcnt,+prefer-movmsk-over-vtest,+prfchw,+ptwrite,+rdpid,+rdrnd,+rdseed,+sahf,+serialize,+sha,+shstk,+slow-3ops-lea,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+tuning-fast-imm-vector-shift,+vaes,+vpclmulqdq,+vzeroupper,+waitpkg,+widekl,+x87,+xsave,+xsavec,+xsaveopt,+xsaves,-16bit-mode,-32bit-mode,-amx-avx512,-amx-bf16,-amx-complex,-amx-fp16,-amx-fp8,-amx-int8,-amx-movrs,-amx-tf32,-amx-tile,-amx-transpose,-avx10.1-256,-avx10.1-512,-avx10.2-256,-avx10.2-512,-avx512bf16,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512f,-avx512fp16,-avx512ifma,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vp2intersect,-avx512vpopcntdq,-avxifma,-avxneconvert,-avxvnniint16,-avxvnniint8,-branch-hint,-branchfusion,-ccmp,-cf,-cldemote,-clzero,-cmpccxadd,-egpr,-enqcmd,-ermsb,-evex512,-false-deps-getmant,-false-deps-lzcnt-tzcnt,-false-deps-mulc,-false-deps-mullq,-false-deps-range,-fast-11bytenop,-fast-7bytenop,-fast-bextr,-fast-dpwssd,-fast-hops,-fast-imm16,-fast-lzcnt,-fast-movbe,-fast-scalar-shift-masks,-fast-vector-shift-masks,-faster-shift-than-shuffle,-fma4,-fsrm,-harden-sls-ijmp,-harden-sls-ret,-idivl-to-divb,-inline-asm-use-gpr32,-lea-sp,-lea-uses-ag,-lvi-cfi,-lvi-load-hardening,-lwp,-movrs,-mwaitx,-ndd,-nf,-no-bypass-delay,-pad-short-functions,-ppx,-prefer-128-bit,-prefer-256-bit,-prefer-mask-registers,-prefer-no-gather,-prefer-no-scatter,-prefetchi,-push2pop2,-raoint,-rdpru,-retpoline,-retpoline-external-thunk,-retpoline-indirect-branches,-retpoline-indirect-calls,-rtm,-sbb-dep-breaking,-seses,-sgx,-sha512,-slow-incdec,-slow-lea,-slow-pmaddwd,-slow-pmulld,-slow-shld,-slow-two-mem-ops,-slow-unaligned-mem-16,-slow-unaligned-mem-32,-sm3,-sm4,-soft-float,-sse4a,-sse-unaligned-mem,-tagged-globals,-tbm,-tsxldtrk,-uintr,-use-glm-div-sqrt-costs,-use-slm-arith-costs,-usermsr,-wbnoinvd,-xop,-zu" }
attributes #<!-- -->1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #<!-- -->2 = { noinline nosanitize_coverage nounwind skipprofile uwtable "frame-pointer"="all" "target-cpu"="raptorlake" "target-features"="+64bit,+adx,+aes,+allow-light-256-bit,+avx,+avx2,+avxvnni,+bmi,+bmi2,+clflushopt,+clwb,+cmov,+crc32,+cx16,+cx8,+f16c,+false-deps-perm,+false-deps-popcnt,+fast-15bytenop,+fast-gather,+fast-scalar-fsqrt,+fast-shld-rotate,+fast-variable-crosslane-shuffle,+fast-variable-perlane-shuffle,+fast-vector-fsqrt,+fma,+fsgsbase,+fxsr,+gfni,+hreset,+idivq-to-divl,+invpcid,+kl,+lzcnt,+macrofusion,+mmx,+movbe,+movdir64b,+movdiri,+no-bypass-delay-blend,+no-bypass-delay-mov,+no-bypass-delay-shuffle,+nopl,+pclmul,+pconfig,+pku,+popcnt,+prefer-movmsk-over-vtest,+prfchw,+ptwrite,+rdpid,+rdrnd,+rdseed,+sahf,+serialize,+sha,+shstk,+slow-3ops-lea,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+tuning-fast-imm-vector-shift,+vaes,+vpclmulqdq,+vzeroupper,+waitpkg,+widekl,+x87,+xsave,+xsavec,+xsaveopt,+xsaves,-16bit-mode,-32bit-mode,-amx-avx512,-amx-bf16,-amx-complex,-amx-fp16,-amx-fp8,-amx-int8,-amx-movrs,-amx-tf32,-amx-tile,-amx-transpose,-avx10.1-256,-avx10.1-512,-avx10.2-256,-avx10.2-512,-avx512bf16,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512f,-avx512fp16,-avx512ifma,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vp2intersect,-avx512vpopcntdq,-avxifma,-avxneconvert,-avxvnniint16,-avxvnniint8,-branch-hint,-branchfusion,-ccmp,-cf,-cldemote,-clzero,-cmpccxadd,-egpr,-enqcmd,-ermsb,-evex512,-false-deps-getmant,-false-deps-lzcnt-tzcnt,-false-deps-mulc,-false-deps-mullq,-false-deps-range,-fast-11bytenop,-fast-7bytenop,-fast-bextr,-fast-dpwssd,-fast-hops,-fast-imm16,-fast-lzcnt,-fast-movbe,-fast-scalar-shift-masks,-fast-vector-shift-masks,-faster-shift-than-shuffle,-fma4,-fsrm,-harden-sls-ijmp,-harden-sls-ret,-idivl-to-divb,-inline-asm-use-gpr32,-lea-sp,-lea-uses-ag,-lvi-cfi,-lvi-load-hardening,-lwp,-movrs,-mwaitx,-ndd,-nf,-no-bypass-delay,-pad-short-functions,-ppx,-prefer-128-bit,-prefer-256-bit,-prefer-mask-registers,-prefer-no-gather,-prefer-no-scatter,-prefetchi,-push2pop2,-raoint,-rdpru,-retpoline,-retpoline-external-thunk,-retpoline-indirect-branches,-retpoline-indirect-calls,-rtm,-sbb-dep-breaking,-seses,-sgx,-sha512,-slow-incdec,-slow-lea,-slow-pmaddwd,-slow-pmulld,-slow-shld,-slow-two-mem-ops,-slow-unaligned-mem-16,-slow-unaligned-mem-32,-sm3,-sm4,-soft-float,-sse4a,-sse-unaligned-mem,-tagged-globals,-tbm,-tsxldtrk,-uintr,-use-glm-div-sqrt-costs,-use-slm-arith-costs,-usermsr,-wbnoinvd,-xop,-zu" }
attributes #<!-- -->3 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #<!-- -->4 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #<!-- -->5 = { noinline }

!llvm.module.flags = !{}
```

Note that the call at `%34 = call fastcc x86_fp80 @<!-- -->math.ldexp.ldexp__anon_1741(x86_fp80 %33, i32 64) #<!-- -->5` is `noinline`. If you remove that attribute and run the program through `opt -O3`, the call is inlined, but the result it produces at runtime is zero instead of the expected `15625000000000000000`.

A self-contained, runnable repro for `x86_64-linux` can be found [here](https://gist.github.com/alexrp/5978cd4aafbbe342424b8d3ebdce44db). Run with `clang -nostdlib -fuse-ld=lld repro.ll &amp;&amp; ./a.out` (which will produce the expected result) and then add `-O3` to see the bug. Source code for this repro in Zig form can be found [here](https://gist.github.com/alexrp/12628795e0b497e46598b614f5f76156).
</details>


---

### Comment 2 - jacobly0

reduction:
```ll
define i1 @foo(x86_fp80 %0) {
  %2 = bitcast x86_fp80 %0 to i80
  %3 = and i80 %2, -604444463063240877801473
  %4 = or i80 %3, 302194561415509874573312
  %5 = bitcast i80 %4 to x86_fp80
  %6 = fcmp uno x86_fp80 %5, %5
  ret i1 %6
}
```
```
$ opt --version
LLVM (http://llvm.org/):
  LLVM version 20.1.0+libcxx
  Optimized build.
  Default target: x86_64-pc-linux-gnu
  Host CPU: znver4
$ opt -S -O1 repro.ll
; ModuleID = 'repro.ll'
source_filename = "repro.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define noundef i1 @foo(x86_fp80 %0) local_unnamed_addr #0 {
  ret i1 true
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
```
Note that the bitwise ops are changing the exponent to be one less than the bias (as done by `frexp`) which mean that it is impossible for `%5` to be nan, therefore the correct optimization is `ret i1 false`.

---

