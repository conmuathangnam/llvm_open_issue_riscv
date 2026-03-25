# [HLSL] Miscompilation of the `RoiAlignMax_float16_native_accum16_Strided_256_NN` DML shader directly causes 2 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162189
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:42:11Z

## Body

Two DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the shader `RoiAlignMax_float16_native_accum16_Strided_256_NN`.

**Failing tests**
```
 OperatorTests::RoiAlign#2 
 OperatorTests::RoiAlign#49
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::RoiAlign#2" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::RoiAlign#2
Error: Output Tensor #0:
Error: Tensor Sizes: 1,1,1,4
Error: Tensor Data Type: float16
Error: Index: 0000 @00000000 [0,0,0,0].  Ref: 281.0000000000 (0x5C64).  DML: 169.0000000000 (0x5948).  Abs: 112.000000.  Rel: 39.857651%.  Ulp: 796
Error: Index: 0001 @00000001 [0,0,0,1].  Ref: 274.0000000000 (0x5C48).  DML: 162.0000000000 (0x5910).  Abs: 112.000000.  Rel: 40.875912%.  Ulp: 824
Error: 2 / 4 (50.000000%) of elements were found to be above tolerance.
Error: Max ULP delta: 824.  Allowed tolerance: 0 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::RoiAlign#2 [Failed]

Summary of Non-passing Tests:
    OperatorTests::RoiAlign#2 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0

> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::RoiAlign#49" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::RoiAlign#49
Error: Output Tensor #0:
Error: Tensor Sizes: 4,3,1,3
Error: Tensor Data Type: float16
Error: Index: 0009 @00000009 [1,0,0,0].  Ref: 3972.0000000000 (0x6BC2).  DML: 64.0000000000 (0x5400).  Abs: 3908.000000.  Rel: 98.388721%.  Ulp: 6082
Error: Index: 0010 @00000010 [1,0,0,1].  Ref: 3984.0000000000 (0x6BC8).  DML: 64.0000000000 (0x5400).  Abs: 3920.000000.  Rel: 98.393574%.  Ulp: 6088
Error: Index: 0011 @00000011 [1,0,0,2].  Ref: 3996.0000000000 (0x6BCE).  DML: 64.0000000000 (0x5400).  Abs: 3932.000000.  Rel: 98.398398%.  Ulp: 6094
Error: Index: 0012 @00000012 [1,1,0,0].  Ref: 8068.0000000000 (0x6FE1).  DML: 64.0000000000 (0x5400).  Abs: 8004.000000.  Rel: 99.206743%.  Ulp: 7137
Error: Index: 0013 @00000013 [1,1,0,1].  Ref: 8080.0000000000 (0x6FE4).  DML: 64.0000000000 (0x5400).  Abs: 8016.000000.  Rel: 99.207921%.  Ulp: 7140
Error: Index: 0014 @00000014 [1,1,0,2].  Ref: 8092.0000000000 (0x6FE7).  DML: 64.0000000000 (0x5400).  Abs: 8028.000000.  Rel: 99.209095%.  Ulp: 7143
Error: Index: 0015 @00000015 [1,2,0,0].  Ref: 12160.0000000000 (0x71F0).  DML: 64.0000000000 (0x5400).  Abs: 12096.000000.  Rel: 99.473684%.  Ulp: 7664
Error: Index: 0016 @00000016 [1,2,0,1].  Ref: 12176.0000000000 (0x71F2).  DML: 64.0000000000 (0x5400).  Abs: 12112.000000.  Rel: 99.474376%.  Ulp: 7666
Error: Index: 0017 @00000017 [1,2,0,2].  Ref: 12192.0000000000 (0x71F4).  DML: 64.0000000000 (0x5400).  Abs: 12128.000000.  Rel: 99.475066%.  Ulp: 7668
Error: 21 / 36 (58.333333%) of elements were found to be above tolerance.
Error: Max ULP delta: 7668.  Allowed tolerance: 0 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::RoiAlign#49 [Failed]

Summary of Non-passing Tests:
    OperatorTests::RoiAlign#49 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

