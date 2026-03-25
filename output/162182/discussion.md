# [HLSL] Miscompilation of `Upsample5D.*` DML shaders directly cause 4 operator test failures

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162182
**Status:** Closed
**Labels:** duplicate, miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-10-27T18:41:44Z

## Body

4 DML operator test failures consistent across devices with AMD, NVIDIA, and Intel GPUs are confirmed to be caused by the following shaders:
```
Upsample5D_float_256_BILINEAR_b16c32
Upsample5D_float_256_BILINEAR_16
```

**Failing tests**
```
OperatorTests::Upsample2d#87
OperatorTests::Upsample2d#95
OperatorTests::Upsample2d#101
OperatorTests::Upsample2d#90
```

## Sample Reproduction
```
> ./TE.exe DirectML.Test.OperatorTests.dll /name:"OperatorTests::Upsample2d#87" /p:DisableMetacommands=1 /logOutput:low
Test Authoring and Execution Framework v10.72 for x64

StartGroup: OperatorTests::Upsample2d#87
Error: Output Tensor #0:
Error: Tensor Sizes: 1,3,3,12,24
Error: Tensor Data Type: float16
Error: Index: 0002 @00000002 [0,0,0,0,2].  Ref: 0.8549804688 (0x3AD7).  DML: 0.9506835938 (0x3B9B).  Abs: 0.095703.  Rel: 10.066769%.  Ulp: 196
Error: Index: 0003 @00000003 [0,0,0,0,3].  Ref: 0.6630859375 (0x394E).  DML: 0.9506835938 (0x3B9B).  Abs: 0.287598.  Rel: 30.251669%.  Ulp: 589
Error: Index: 0006 @00000006 [0,0,0,0,6].  Ref: 0.2519531250 (0x3408).  DML: 0.0875854492 (0x2D9B).  Abs: 0.164368.  Rel: 65.237403%.  Ulp: 1645
Error: Index: 0007 @00000007 [0,0,0,0,7].  Ref: 0.3891601562 (0x363A).  DML: -0.1042480469 (0xAEAC).  Abs: 0.493408.  Rel: 126.787955%.  Ulp: 34702
Error: Index: 0010 @00000010 [0,0,0,0,10].  Ref: 0.7377929688 (0x39E7).  DML: 0.8002929688 (0x3A67).  Abs: 0.062500.  Rel: 7.809640%.  Ulp: 128
Error: Index: 0011 @00000011 [0,0,0,0,11].  Ref: 0.7500000000 (0x3A00).  DML: 0.9375000000 (0x3B80).  Abs: 0.187500.  Rel: 20.000000%.  Ulp: 384
Error: Index: 0014 @00000014 [0,0,0,0,14].  Ref: 0.7573242188 (0x3A0F).  DML: 0.7856445312 (0x3A49).  Abs: 0.028320.  Rel: 3.604723%.  Ulp: 58
Error: Index: 0015 @00000015 [0,0,0,0,15].  Ref: 0.7119140625 (0x39B2).  DML: 0.7978515625 (0x3A62).  Abs: 0.085938.  Rel: 10.771114%.  Ulp: 176
Error: Index: 0018 @00000018 [0,0,0,0,18].  Ref: 0.5986328125 (0x38CA).  DML: 0.5761718750 (0x389C).  Abs: 0.022461.  Rel: 3.752039%.  Ulp: 46
Error: 1289 / 2592 (49.729938%) of elements were found to be above tolerance.
Error: Max ULP delta: 41391.  Allowed tolerance: 1 ULPs (float16).
Error: Verify: Fail [File: C:\workspace\DirectML\SharedToolingLib\External\Test\TaefHelper\TaefHelper.cpp, Function: TaefHelper::Fail, Line: 133]
EndGroup: OperatorTests::Upsample2d#87 [Failed]

Summary of Non-passing Tests:
    OperatorTests::Upsample2d#87 [Failed]

Summary: Total=1, Passed=0, Failed=1, Blocked=0, Not Run=0, Skipped=0
```

