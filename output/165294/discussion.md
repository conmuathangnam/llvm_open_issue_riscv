# [HLSL] Miscompilation of `Cast_int64_emulated_float64_native_256.*4D` DML shaders directly cause 12 operator test failures on AMD and NVIDIA

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/165294
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-11-05T17:09:15Z

## Body

12 DML operator test failures consistent across devices with AMD and NVIDIA GPUs are confirmed to be caused by the following shaders:
```
Cast_uint64_emulated_float64_native_256_Packed4D
Cast_int64_emulated_float64_native_256_Packed4D
Cast_uint64_emulated_float64_native_256_Strided4D
Cast_int64_emulated_float64_native_256_Strided4D
```

Note that Intel GPUs skip these tests due to unsupported data types.

Failing tests
```
OperatorTests::Cast#55
OperatorTests::Cast#350
OperatorTests::Cast#766
OperatorTests::Cast#790
OperatorTests::Cast#17
OperatorTests::Cast#424
OperatorTests::Cast#359
OperatorTests::Cast#253
OperatorTests::Cast#287
OperatorTests::Cast#543
OperatorTests::Cast#1012
OperatorTests::Cast#1035
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::Cast#55" /p:DisableMetacommands=1 /logOutput:low /p:GpuAdapterIndex=0
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::Cast#55
Error: Output Tensor #0:
Error: Tensor Sizes: 1,3,3,3,3,3,3,3
Error: Tensor Data Type: float64
Error: Index: 0000 @00000000 [0,0,0,0,0,0,0,0].  Ref: 47.0000000000 (0x4047800000000000).  DML: 67108911.0000000000 (0x41900000BC000000).  Abs: 67108864.000000.  Rel: 99.999930%.  Ulp: 92464533003567104
Error: Index: 0001 @00000729 [0,0,0,0,0,0,0,1].  Ref: 101.0000000000 (0x4059400000000000).  DML: 33554533.0000000000 (0x4180000328000000).  Abs: 33554432.000000.  Rel: 99.999699%.  Ulp: 82964762941456384
Error: Index: 0002 @00001458 [0,0,0,0,0,0,0,2].  Ref: 120.0000000000 (0x405E000000000000).  DML: 33554552.0000000000 (0x41800003C0000000).  Abs: 33554432.000000.  Rel: 99.999642%.  Ulp: 81627759352217600
Error: Index: 0003 @00000243 [0,0,0,0,0,0,1,0].  Ref: 23.0000000000 (0x4037000000000000).  DML: 134217751.0000000000 (0x41A000002E000000).  Abs: 134217728.000000.  Rel: 99.999983%.  Ulp: 101612467364298752
Error: Index: 0004 @00000972 [0,0,0,0,0,0,1,1].  Ref: 92.0000000000 (0x4057000000000000).  DML: 33554524.0000000000 (0x41800002E0000000).  Abs: 33554432.000000.  Rel: 99.999726%.  Ulp: 83598080431095808
Error: Index: 0005 @00001701 [0,0,0,0,0,0,1,2].  Ref: 99.0000000000 (0x4058C00000000000).  DML: 33554531.0000000000 (0x4180000318000000).  Abs: 33554432.000000.  Rel: 99.999705%.  Ulp: 83105500161376256
Error: Index: 0006 @00000486 [0,0,0,0,0,0,2,0].  Ref: 76.0000000000 (0x4053000000000000).  DML: 33554508.0000000000 (0x4180000260000000).  Abs: 33554432.000000.  Rel: 99.999774%.  Ulp: 84723978190454784
Error: Index: 0007 @00001215 [0,0,0,0,0,0,2,1].  Ref: 75.0000000000 (0x4052C00000000000).  DML: 33554507.0000000000 (0x4180000258000000).  Abs: 33554432.000000.  Rel: 99.999776%.  Ulp: 84794346800414720
Error: Index: 0008 @00001944 [0,0,0,0,0,0,2,2].  Ref: 19.0000000000 (0x4033000000000000).  DML: 134217747.0000000000 (0x41A0000026000000).  Abs: 134217728.000000.  Rel: 99.999986%.  Ulp: 102738367136923648
Error: 2171 / 2187 (99.268404%) of elements were found to be above tolerance.
Error: Max ULP delta: 139611588450582528.  Allowed tolerance: 0 ULPs (float64).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::Cast#55 [Failed]

Summary of Non-passing Tests:
    OperatorTests::Cast#55 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```




