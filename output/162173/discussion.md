# [HLSL] Miscompilation of `ResampleNearestNeighbor.*` DML shaders directly cause 141 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162173
**Status:** Closed
**Labels:** miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:40:48Z

## Body

141 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
ResampleNearestNeighborGrad_float16_native_accum32_256
ResampleNearestNeighborGrad_float16_native_accum32_Strided_256
ResampleNearestNeighborGrad_float16_native_accum16_256
ResampleNearestNeighborGrad_float16_native_accum16_Strided_256
```

Two failed tests per shader:
```
OperatorTests::TFResampleGrad#5
OperatorTests::TFResampleGrad#37
OperatorTests::TFResampleGrad#74
OperatorTests::TFResampleGrad#10
OperatorTests::ResampleGrad#36
OperatorTests::ResampleGrad#11
OperatorTests::ResampleGrad#3
OperatorTests::ResampleGrad#4
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::TFResampleGrad#5" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::TFResampleGrad#5
Error: Output Tensor #0:
Error: Tensor Sizes: 3,32,1,6
Error: Tensor Data Type: float16
Error: Index: 0000 @00000000 [0,0,0,0].  Ref: 9.0000000000 (0x4880).  DML: 5.0000000000 (0x4500).  Abs: 4.000000.  Rel: 44.444444%.  Ulp: 896
Error: Index: 0002 @00000002 [0,0,0,2].  Ref: 3.0000000000 (0x4200).  DML: 9.0000000000 (0x4880).  Abs: 6.000000.  Rel: 66.666667%.  Ulp: 1664
Error: Index: 0003 @00000003 [0,0,0,3].  Ref: 6.0000000000 (0x4600).  DML: 14.0000000000 (0x4B00).  Abs: 8.000000.  Rel: 57.142857%.  Ulp: 1280
Error: Index: 0004 @00000004 [0,0,0,4].  Ref: 9.0000000000 (0x4880).  DML: 21.0000000000 (0x4D40).  Abs: 12.000000.  Rel: 57.142857%.  Ulp: 1216
Error: Index: 0005 @00000005 [0,0,0,5].  Ref: 0.0000000000 (0x0000).  DML: 21.0000000000 (0x4D40).  Abs: 21.000000.  Rel: 100.000000%.  Ulp: 19776
Error: Index: 0007 @00000007 [0,1,0,1].  Ref: 15.0000000000 (0x4B80).  DML: 24.0000000000 (0x4E00).  Abs: 9.000000.  Rel: 37.500000%.  Ulp: 640
Error: Index: 0008 @00000008 [0,1,0,2].  Ref: 15.0000000000 (0x4B80).  DML: 39.0000000000 (0x50E0).  Abs: 24.000000.  Rel: 61.538462%.  Ulp: 1376
Error: Index: 0009 @00000009 [0,1,0,3].  Ref: 15.0000000000 (0x4B80).  DML: 54.0000000000 (0x52C0).  Abs: 39.000000.  Rel: 72.222222%.  Ulp: 1856
Error: Index: 0010 @00000010 [0,1,0,4].  Ref: 15.0000000000 (0x4B80).  DML: 69.0000000000 (0x5450).  Abs: 54.000000.  Rel: 78.260870%.  Ulp: 2256
Error: 509 / 576 (88.368056%) of elements were found to be above tolerance.
Error: Max absolute delta: 7060.000000.  Allowed absolute tolerance: 0.001000.
Error: Max relative delta: 100.000000%.  Allowed relative tolerance: 2.000000%.
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::TFResampleGrad#5 [Failed]

Summary of Non-passing Tests:
    OperatorTests::TFResampleGrad#5 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

## Comments

### Comment 1 - Icohedron

A loop in the shader is not executing as many times as it should be. It appears that the loop exit condition was not correctly computed due to #164018 

---

