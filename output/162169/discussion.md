# [HLSL] Miscompilation of `GemmLarge.*` DML shaders directly cause 92 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162169
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-11-03T22:23:43Z

## Body

92 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
GemmLargeGemm_Bias_float_b16c32
GemmLargeGemm_Bias_float_16
GemmLargeGemm_float_b16c32
GemmLargeGemm_float_16
```

Two failed tests per shader:
```
OperatorTests::GemmLarge#63
OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#5 
OperatorTests::GemmLarge#2
OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#15
OperatorTests::GemmLarge#436
OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#152
OperatorTests::GemmLarge#834
OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#250 
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#5" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#5
Error: Output Tensor #0:
Error: Tensor Sizes: 3,1,63,64
Error: Tensor Data Type: float16
Error: Index: 0512 @00000512 [0,0,8,0].  Ref: -6.4179687500 (0xC66B).  DML: inf (0x7C00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 46485
Error: Index: 0513 @00000513 [0,0,8,1].  Ref: -8.3125000000 (0xC828).  DML: -inf (0xFC00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 13272
Error: Index: 0514 @00000514 [0,0,8,2].  Ref: -6.2265625000 (0xC63A).  DML: inf (0x7C00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 46534
Error: Index: 0515 @00000515 [0,0,8,3].  Ref: -8.3125000000 (0xC828).  DML: inf (0x7C00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 46040
Error: Index: 0516 @00000516 [0,0,8,4].  Ref: -7.9375000000 (0xC7F0).  DML: 0.1622314453 (0x3131).  Abs: 8.099731.  Rel: 102.043861%.  Ulp: 26945
Error: Index: 0517 @00000517 [0,0,8,5].  Ref: -6.2773437500 (0xC647).  DML: -inf (0xFC00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 13753
Error: Index: 0518 @00000518 [0,0,8,6].  Ref: -8.2656250000 (0xC822).  DML: inf (0x7C00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 46046
Error: Index: 0519 @00000519 [0,0,8,7].  Ref: -6.4218750000 (0xC66C).  DML: inf (0x7C00).  Abs: inf.  Rel: -nan(ind)%.  Ulp: 46484
Error: Index: 0520 @00000520 [0,0,8,8].  Ref: -6.3203125000 (0xC652).  DML: nan (0x7FFF).  Abs: nan.  Rel: nan%.  Ulp: 47533
Error: 10560 / 12096 (87.301587%) of elements were found to be above tolerance.
Error: Max absolute delta: inf.  Allowed absolute tolerance: 0.020000.
Error: Max relative delta: 148.779796%.  Allowed relative tolerance: 1.000000%.
Error: Max ULP delta: 47533.  Allowed tolerance: 6 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#5 [Failed]

Summary of Non-passing Tests:
    OperatorTests::MatrixMultiplyIntegerToFloatLargeGemm#5 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

## Comments

### Comment 1 - Icohedron

These DML operator test failures appear to be due to a bug in the data scalarizer: https://godbolt.org/z/Y7cnG95Wb
There are a total of 557 DML operator tests using a DXC-compiled shader with the name "Gemm" in it.

---

### Comment 2 - Icohedron

These tests are passing with the PR fixing #165473 so I will now close this as a duplicate of that issue.

---

