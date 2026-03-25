# [SPIRV] `alloca` generates `OpVariable` instruction for the wrong type

**Author:** simeonschaub
**URL:** https://github.com/llvm/llvm-project/issues/163777
**Status:** Closed
**Labels:** miscompilation, backend:SPIR-V
**Closed Date:** 2026-03-11T14:16:24Z

## Body

Reduced from a more complex example in https://github.com/JuliaGPU/OpenCL.jl/issues/384, I believe this to be the underlying issue.

Reproducer:

```llvm
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-G1"
target triple = "spirv64-unknown-unknown-unknown"

define spir_kernel void @f() local_unnamed_addr {
conversion:
  %y = alloca [1 x { { { ptr addrspace(1), i64, [1 x i64], i64 }, [2 x [1 x i64]] } }], align 8
  %2 = load ptr addrspace(1), ptr %y, align 8
  ret void
}
```

compiled using `llc` yields the following SPIR-V:

```spv
; SPIR-V
; Version: 1.4
; Generator: LLVM LLVM SPIR-V Backend; 20
; Bound: 21
; Schema: 0
               OpCapability Kernel
               OpCapability Addresses
               OpCapability Int8
          %1 = OpExtInstImport "OpenCL.std"
               OpMemoryModel Physical64 OpenCL
               OpEntryPoint Kernel %f "f"
               OpExecutionMode %f ContractionOff
               OpSource OpenCL_CPP 100000
               OpName %f "f"
               OpName %y "y"
               OpName %conversion "conversion"
               OpDecorate %y Alignment 8
       %void = OpTypeVoid
      %uchar = OpTypeInt 8 0
%_ptr_CrossWorkgroup_uchar = OpTypePointer CrossWorkgroup %uchar
%_ptr_Function__ptr_CrossWorkgroup_uchar = OpTypePointer Function %_ptr_CrossWorkgroup_uchar
          %7 = OpTypeFunction %void
          %f = OpFunction %void None %7
 %conversion = OpLabel
          %y = OpVariable %_ptr_Function__ptr_CrossWorkgroup_uchar Function
               OpReturn
               OpFunctionEnd
```

Note the line `%y = OpVariable %_ptr_Function__ptr_CrossWorkgroup_uchar Function`, so it only allocates memory for a single pointer, not for the entire nested array of structs.

For comparison, here is what the Khronos SPIRV-LLVM Translator generates:

```spv
; SPIR-V
; Version: 1.0
; Generator: Khronos LLVM/SPIR-V Translator; 14
; Bound: 25
; Schema: 0
               OpCapability Addresses
               OpCapability Linkage
               OpCapability Kernel
               OpCapability Int64
               OpCapability Int8
          %1 = OpExtInstImport "OpenCL.std"
               OpMemoryModel Physical64 OpenCL
               OpEntryPoint Kernel %22 "f"
               OpSource Unknown 0
               OpName %f "f"
               OpName %conversion "conversion"
               OpName %y "y"
               OpDecorate %f LinkageAttributes "f" Export
               OpDecorate %y Alignment 8
      %ulong = OpTypeInt 64 0
      %uchar = OpTypeInt 8 0
    %ulong_1 = OpConstant %ulong 1
    %ulong_2 = OpConstant %ulong 2
       %void = OpTypeVoid
          %3 = OpTypeFunction %void
%_ptr_CrossWorkgroup_uchar = OpTypePointer CrossWorkgroup %uchar
%_arr_ulong_ulong_1 = OpTypeArray %ulong %ulong_1
 %_struct_10 = OpTypeStruct %_ptr_CrossWorkgroup_uchar %ulong %_arr_ulong_ulong_1 %ulong
%_arr__arr_ulong_ulong_1_ulong_2 = OpTypeArray %_arr_ulong_ulong_1 %ulong_2
  %_struct_9 = OpTypeStruct %_struct_10 %_arr__arr_ulong_ulong_1_ulong_2
  %_struct_8 = OpTypeStruct %_struct_9
%_arr__struct_8_ulong_1 = OpTypeArray %_struct_8 %ulong_1
%_ptr_Function__arr__struct_8_ulong_1 = OpTypePointer Function %_arr__struct_8_ulong_1
%_ptr_Function__ptr_CrossWorkgroup_uchar = OpTypePointer Function %_ptr_CrossWorkgroup_uchar
          %f = OpFunction %void None %3
 %conversion = OpLabel
          %y = OpVariable %_ptr_Function__arr__struct_8_ulong_1 Function
         %20 = OpBitcast %_ptr_Function__ptr_CrossWorkgroup_uchar %y
         %21 = OpLoad %_ptr_CrossWorkgroup_uchar %20 Aligned 8
               OpReturn
               OpFunctionEnd
         %22 = OpFunction %void None %3
         %23 = OpLabel
         %24 = OpFunctionCall %void %f
               OpReturn
               OpFunctionEnd
```

which has the correct type: `%y = OpVariable %_ptr_Function__arr__struct_8_ulong_1 Function`

