# [HLSL] Miscompilation of `ResampleLinearGrad.*` DML shaders directly cause 59 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162177
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:41:12Z

## Body

59 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
ResampleLinearGrad_float16_native_accum32_Strided_256
ResampleLinearGrad_float16_native_accum16_Strided_256
```

Two failed tests per shader:
```
OperatorTests::TFResampleGrad#61
OperatorTests::ResampleGrad#10
OperatorTests::TFResampleGrad#36
OperatorTests::ResampleGrad#107
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::TFResampleGrad#61" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::TFResampleGrad#61
Error: Output Tensor #0:
Error: Tensor Sizes: 3,16,1,1
Error: Tensor Data Type: float16
Error: Index: 0000 @00000000 [0,0,0,0].  Ref: 37.5000000000 (0x50B0).  DML: 5.0000000000 (0x4500).  Abs: 32.500000.  Rel: 86.666667%.  Ulp: 2992
Error: Index: 0001 @00000004 [0,1,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0002 @00000008 [0,2,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0003 @00000012 [0,3,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0004 @00000016 [0,4,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0005 @00000020 [0,5,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0006 @00000024 [0,6,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0007 @00000028 [0,7,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: Index: 0008 @00000032 [0,8,0,0].  Ref: 60.0000000000 (0x5380).  DML: 8.0000000000 (0x4800).  Abs: 52.000000.  Rel: 86.666667%.  Ulp: 2944
Error: 32 / 48 (66.666667%) of elements were found to be above tolerance.
Error: Max absolute delta: 101.500000.  Allowed absolute tolerance: 0.001000.
Error: Max relative delta: 90.222222%.  Allowed relative tolerance: 2.000000%.
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::TFResampleGrad#61 [Failed]

Summary of Non-passing Tests:
    OperatorTests::TFResampleGrad#61 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

