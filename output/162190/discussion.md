# [HLSL] Miscompilation of the `RoiAlignImageGradAverage_float_Strided_256_BILINEAR_b16c32` DML shader directly causes an operator test failure

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162190
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:42:23Z

## Body

An DML operator test failure consistent across devices with AMD, NVIDIA, and Intel GPUs is confirmed to be caused by the shader `RoiAlignImageGradAverage_float_Strided_256_BILINEAR_b16c32`

**Failing test**
```
OperatorTests::RoiAlignGrad#313
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::RoiAlignGrad#313" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::RoiAlignGrad#313
Error: Output Tensor #0:
Error: Tensor Sizes: 1,1,1,1
Error: Tensor Data Type: float16
Error: Index: 0000 @00000000 [0,0,0,0].  Ref: 3.0000000000 (0x4200).  DML: 3.5917968750 (0x432F).  Abs: 0.591797.  Rel: 16.476346%.  Ulp: 303
Error: 1 / 1 (100.000000%) of elements were found to be above tolerance.
Error: Max absolute delta: 0.591797.  Allowed absolute tolerance: 0.001000.
Error: Max ULP delta: 303.  Allowed tolerance: 5 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::RoiAlignGrad#313 [Failed]

Summary of Non-passing Tests:
    OperatorTests::RoiAlignGrad#313 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

