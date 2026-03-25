# [HLSL] Miscompilation of `ResampleLinear.*` DML shaders directly cause 21 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162181
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:41:24Z

## Body

21 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
ResampleLinear_float16_native_accum16_256_SampleX_SampleY_SampleZ_SampleW_0_1
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_0_1
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_SampleW_1_8
ResampleLinear_float16_native_accum32_256_SampleX_SampleZ_SampleW_1_8
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_SampleW_0_8
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_0_8
ResampleLinear_float16_native_accum16_256_SampleX_SampleY_0_8
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_SampleW_2_4
ResampleLinear_float16_native_accum16_256_SampleY_SampleZ_1_4
ResampleLinear_float16_native_accum16_256_SampleX_SampleY_SampleZ_SampleW_0_4
ResampleLinear_float16_native_accum32_256_SampleX_SampleZ_SampleW_0_4
ResampleLinear_float16_native_accum32_256_SampleX_SampleY_SampleZ_SampleW_0_1
ResampleLinear_float16_native_accum16_256_SampleY_SampleZ_SampleW_0_1
ResampleLinear_float16_native_accum32_256_SampleW_0_1
ResampleLinear_float16_native_accum16_256_SampleX_SampleY_SampleZ_0_1
ResampleLinear_float16_native_accum32_256_SampleY_SampleZ_0_1
ResampleLinear_float16_native_accum32_256_SampleX_SampleZ_0_1
ResampleLinear_float16_native_accum16_256_SampleX_SampleY_0_1
```

One failed test per shader:
```
OperatorTests::Resample#34
OperatorTests::Resample#27
OperatorTests::Resample#28
OperatorTests::Resample#288
OperatorTests::Resample#122
OperatorTests::Resample#464
OperatorTests::Resample#147
OperatorTests::Resample#460
OperatorTests::Resample#387
OperatorTests::Resample#433
OperatorTests::Resample#453
OperatorTests::Resample#419
OperatorTests::Resample#104
OperatorTests::Resample#371
OperatorTests::Resample#272
OperatorTests::Resample#396
OperatorTests::Resample#527
OperatorTests::Resample#406
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::Resample#27" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::Resample#27
Error: Output Tensor #0:
Error: Tensor Sizes: 1,2,6,1
Error: Tensor Data Type: float16
Error: Index: 0001 @00000002 [0,0,1,0].  Ref: 24.2500000000 (0x4E10).  DML: 24.0000000000 (0x4E00).  Abs: 0.250000.  Rel: 1.030928%.  Ulp: 16
Error: Index: 0002 @00000004 [0,0,2,0].  Ref: 24.7500000000 (0x4E30).  DML: 24.5000000000 (0x4E20).  Abs: 0.250000.  Rel: 1.010101%.  Ulp: 16
Error: Index: 0003 @00000006 [0,0,3,0].  Ref: 25.2500000000 (0x4E50).  DML: 24.5000000000 (0x4E20).  Abs: 0.750000.  Rel: 2.970297%.  Ulp: 48
Error: Index: 0004 @00000008 [0,0,4,0].  Ref: 25.7500000000 (0x4E70).  DML: 25.5000000000 (0x4E60).  Abs: 0.250000.  Rel: 0.970874%.  Ulp: 16
Error: Index: 0005 @00000010 [0,0,5,0].  Ref: 26.0000000000 (0x4E80).  DML: 25.5000000000 (0x4E60).  Abs: 0.500000.  Rel: 1.923077%.  Ulp: 32
Error: Index: 0007 @00000016 [0,1,1,0].  Ref: 46.7500000000 (0x51D8).  DML: 46.5000000000 (0x51D0).  Abs: 0.250000.  Rel: 0.534759%.  Ulp: 8
Error: Index: 0008 @00000018 [0,1,2,0].  Ref: 47.2500000000 (0x51E8).  DML: 47.0000000000 (0x51E0).  Abs: 0.250000.  Rel: 0.529101%.  Ulp: 8
Error: Index: 0009 @00000020 [0,1,3,0].  Ref: 47.7500000000 (0x51F8).  DML: 47.0000000000 (0x51E0).  Abs: 0.750000.  Rel: 1.570681%.  Ulp: 24
Error: Index: 0010 @00000022 [0,1,4,0].  Ref: 48.2500000000 (0x5208).  DML: 48.0000000000 (0x5200).  Abs: 0.250000.  Rel: 0.518135%.  Ulp: 8
Error: 10 / 12 (83.333333%) of elements were found to be above tolerance.
Error: Max ULP delta: 48.  Allowed tolerance: 1 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::Resample#27 [Failed]

Summary of Non-passing Tests:
    OperatorTests::Resample#27 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

