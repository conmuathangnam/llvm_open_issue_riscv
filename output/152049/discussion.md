# [SPIR-V] OpSpecConstantOp Bitcast error for `reversebits` vec2/vec3 integer cases

**Author:** kmpeng
**URL:** https://github.com/llvm/llvm-project/issues/152049
**Status:** Closed
**Labels:** miscompilation, HLSL, backend:SPIR-V
**Closed Date:** 2025-12-01T03:01:03Z

## Body

https://godbolt.org/z/s9Pbjh8E8

Discovered while running the offload-test-suite. This constant folding case:
``` hlsl
// RUN: %clang-dxc -spirv -fspv-target-env=vulkan1.3 -T cs_6_5 -enable-16bit-types %s

StructuredBuffer<uint16_t4> In : register(t0);
RWStructuredBuffer<uint16_t4> Out : register(u1);

[numthreads(1,1,1)]
void main() {
    Out[0].xy = reversebits(uint16_t2(0, 256));
}
```
generates a OpSpecConstantOp Bitcast operation, which is not supported by Vulkan:
``` 
# | Validation Error: [ VUID-VkShaderModuleCreateInfo-pCode-08737 ] | MessageID = 0xa5625282
# | vkCreateShaderModule(): pCreateInfo->pCode (spirv-val produced an error):
# | Specialization constant operation Bitcast requires Kernel capability
# |   %38 = OpSpecConstantOp %ushort Bitcast %33
```
The same error occurs for vec3 constant folding `Out[0].xyz = reversebits(uint16_t3(0, 1, 256))`. 
These errors also occur for `uint32_t` and `uint64_t`.

<br>

Crash stack:
```
# | Stack dump:
# | 0.	Program arguments: e:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\bin\\offloader.exe E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Feature\\HLSLLib\\Output\\reversebits.16.test.tmp/pipeline.yaml E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Feature\\HLSLLib\\Output\\reversebits.16.test.tmp.o
# | Exception Code: 0xC0000005
# |  #0 0x00007ffdbb86a37a (C:\WINDOWS\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_f42d50ef4c9376ba\igvk64.dll+0x57a37a)
# |  #1 0x00007ffdbb85b25a (C:\WINDOWS\System32\DriverStore\FileRepository\iigd_dch.inf_amd64_f42d50ef4c9376ba\igvk64.dll+0x56b25a)
# |  #2 0x00007ffdb9fe859a std::vector<std::vector<vvl::base::Device *,std::allocator<vvl::base::Device *> >,std::allocator<std::vector<vvl::base::Device *,std::allocator<vvl::base::Device *> > > >::operator[] D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\include\vector:1556:0
# |  #3 0x00007ffdb9fe859a vulkan_layer_chassis::CmdDispatch(struct VkCommandBuffer_T *, unsigned int, unsigned int, unsigned int) C:\SDKBuild\build-X64-1.4.313.1\Vulkan-ValidationLayers\layers\vulkan\generated\chassis.cpp:4480:0
# |  #4 0x00007ffdc976073d (C:\WINDOWS\SYSTEM32\vulkan-1.dll+0x6073d)
# |  #5 0x00007ff7873bb71a `anonymous namespace'::VKDevice::createComputeCommands E:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\lib\API\VK\Device.cpp:1083:0
# |  #6 0x00007ff787396377 llvm::Error::getPtr E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:277:0
# |  #7 0x00007ff787396377 llvm::Error::operator bool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:241:0
# |  #8 0x00007ff787396377 `anonymous namespace'::VKDevice::executeProgram E:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\lib\API\VK\Device.cpp:1188:0
# |  #9 0x00007ff7872b2b51 llvm::Error::getPtr E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:277:0
# | #10 0x00007ff7872b2b51 llvm::Error::operator= E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:218:0
# | #11 0x00007ff7872b2b51 llvm::Error::Error E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:198:0
# | #12 0x00007ff7872b2b51 llvm::ExitOnError::operator() E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:1460:0
# | #13 0x00007ff7872b2b51 run E:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\tools\offloader\offloader.cpp:136:0
# | #14 0x00007ff7872b22b8 main E:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\tools\offloader\offloader.cpp:75:0
# | #15 0x00007ff787b6b060 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
# | #16 0x00007ff787b6b060 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #17 0x00007ffdffd9e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
# | #18 0x00007ffe0155c34c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x3c34c)
# `-----------------------------
# error: command failed with exit status: 0xc0000005
```

## Comments

### Comment 1 - kmpeng

Closing as this seems to have been fixed by https://github.com/llvm/llvm-project/pull/169076.

---

