# [DirectX] Direct stores to array allocas cause validation error: Explicit load/store type does not match pointee type of pointer operand

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/147114
**Status:** Closed
**Labels:** miscompilation, backend:DirectX, HLSL
**Closed Date:** 2025-07-15T17:40:54Z

## Body

## The Problem
69 DML shaders are failing to validate due to validation error: `Explicit load/store type does not match pointee type of pointer operand`

## Reproducing the problem
```
dxv workspace/ClangDML/tools/shader_builder_clang/validation/NonzeroCoordinatesSingleLevel_256_16_Strided_int_8.dat
```

Minimal reproduction shader:
```hlsl
uint2 Foo(uint2 a[2], uint i) {
  return a[i];
}
RWStructuredBuffer<uint2> output;
[numthreads(1, 1, 1)]
void CSMain(uint3 Tid : SV_DispatchThreadID) {
  uint2 arr[2] = {Tid.xy, Tid.yx};
  output[0] = Foo(arr, Tid.z);
}
```
https://godbolt.org/z/PdW9d7brE

Using lldb, I have found that the validation error is due to direct scalar stores into array allocas:
```llvm
  %1 = alloca [4 x i32], align 8
  %4 = call i32 @dx.op.threadId.i32(i32 93, i32 0) #3, !dbg !86
  store i32 %4, ptr %1, align 8, !dbg !105
```
returning this error https://github.com/microsoft/DirectXShaderCompiler/blob/4fcf67f78f7d6ffd286316112694a3ae000860e2/lib/Bitcode/Reader/BitcodeReader.cpp#L3693

## Solution

The dxil-legalize pass `DXILLegalizePass.cpp` should insert a GEP for store and load instructions on array allocas.


