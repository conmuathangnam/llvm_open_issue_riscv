# [HLSL] Miscompilation of `CumulativeOperation.*` DML shaders directly cause 48 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162180
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:41:33Z

## Body

48 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
CumulativeOperationSummation_256_int64_native_8
CumulativeOperationSummation_256_uint64_native_8
CumulativeOperationProduct_256_uint64_native_8
CumulativeOperationSummation_256_int64_native_4
CumulativeOperationSummation_256_uint64_native_4
CumulativeOperationSummation_256_float16_native_accum32_8
CumulativeOperationSummation_256_float16_native_accum16_8
CumulativeOperationProduct_256_uint64_native_4
CumulativeOperationProduct_256_float16_native_accum32_8
CumulativeOperationProduct_256_int64_native_4
CumulativeOperationSummation_256_float16_native_accum16_4
```

One failed test per shader:
```
OperatorTests::CumulativeOperation#35
OperatorTests::CumulativeOperation#14
OperatorTests::CumulativeOperation#170
OperatorTests::CumulativeOperation#49
OperatorTests::CumulativeOperation#24
OperatorTests::CumulativeOperation#123
OperatorTests::CumulativeOperation#32
OperatorTests::CumulativeOperation#234
OperatorTests::CumulativeOperation#154
OperatorTests::CumulativeOperation#224
OperatorTests::CumulativeOperation#400
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::CumulativeOperation#35" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::CumulativeOperation#35
Error: Output Tensor #0:
Error: Tensor Sizes: 2,3,1,2,1,1,3
Error: Tensor Data Type: int64
Error: Index: 0000 @00000000 [0,0,0,0,0,0,0].  Ref: -252 (0xFFFFFFFFFFFFFF04)  DML: 0 (0x0000000000000000)  Abs: 252.000000.  Rel: 100.000000%.  Ulp: 9248
Error: Index: 0001 @00000001 [0,0,0,0,0,0,1].  Ref: -49 (0xFFFFFFFFFFFFFFCF)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 48.000000.  Rel: 97.959184%.  Ulp: 5664
Error: Index: 0002 @00000002 [0,0,0,0,0,0,2].  Ref: 0 (0x0000000000000000)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 1.000000.  Rel: 100.000000%.  Ulp: 17408
Error: Index: 0003 @00000016 [0,0,0,1,0,0,0].  Ref: -488 (0xFFFFFFFFFFFFFE18)  DML: 0 (0x0000000000000000)  Abs: 488.000000.  Rel: 100.000000%.  Ulp: 8288
Error: Index: 0004 @00000017 [0,0,0,1,0,0,1].  Ref: -220 (0xFFFFFFFFFFFFFF24)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 219.000000.  Rel: 99.545455%.  Ulp: 7904
Error: Index: 0005 @00000018 [0,0,0,1,0,0,2].  Ref: 0 (0x0000000000000000)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 1.000000.  Rel: 100.000000%.  Ulp: 17408
Error: Index: 0006 @00000096 [0,1,0,0,0,0,0].  Ref: -1247 (0xFFFFFFFFFFFFFB21)  DML: 0 (0x0000000000000000)  Abs: 1247.000000.  Rel: 100.000000%.  Ulp: 6945
Error: Index: 0007 @00000097 [0,1,0,0,0,0,1].  Ref: -844 (0xFFFFFFFFFFFFFCB4)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 843.000000.  Rel: 99.881517%.  Ulp: 9880
Error: Index: 0008 @00000098 [0,1,0,0,0,0,2].  Ref: 0 (0x0000000000000000)  DML: -1 (0xFFFFFFFFFFFFFFFF)  Abs: 1.000000.  Rel: 100.000000%.  Ulp: 17408
Error: 36 / 36 (100.000000%) of elements were found to be above tolerance.
Error: Max ULP delta: 17408.  Allowed tolerance: 2 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::CumulativeOperation#35 [Failed]

Summary of Non-passing Tests:
    OperatorTests::CumulativeOperation#35 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

