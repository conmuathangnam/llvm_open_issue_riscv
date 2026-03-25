# [HLSL] Miscompilation of `ReverseSubsequences.*` DML shaders directly cause 54 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162174
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:41:02Z

## Body

54 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
ReverseSubsequences_256_uint16_native_8
ReverseSubsequences_256_uint16_native_4
```

One failed test per shader:
```
OperatorTests::ReverseSubsequences#67
OperatorTests::ReverseSubsequences#6
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::ReverseSubsequences#6" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::ReverseSubsequences#6
Error: Output Tensor #0:
Error: Tensor Sizes: 2,3
Error: Tensor Data Type: uint16
Error: Index: 0002 @00000002 [0,2].  Ref: 5 (0x0005)  DML: 4 (0x0004)  Abs: 1.000000.  Rel: 20.000000%.  Ulp: 1
Error: Index: 0004 @00000004 [1,1].  Ref: 1 (0x0001)  DML: 0 (0x0000)  Abs: 1.000000.  Rel: 100.000000%.  Ulp: 1
Error: Index: 0005 @00000005 [1,2].  Ref: 2 (0x0002)  DML: 0 (0x0000)  Abs: 2.000000.  Rel: 100.000000%.  Ulp: 2
Error: 3 / 6 (50.000000%) of elements were found to be above tolerance.
Error: Max ULP delta: 2.  Allowed tolerance: 0 ULPs (uint16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::ReverseSubsequences#6 [Failed]

Summary of Non-passing Tests:
    OperatorTests::ReverseSubsequences#6 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

