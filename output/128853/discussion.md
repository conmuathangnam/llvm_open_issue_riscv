# NVPTX produces incorrect PTX with 8-bit integer vector input which results in wrong answer

**Author:** rashedmyt
**URL:** https://github.com/llvm/llvm-project/issues/128853
**Status:** Closed
**Labels:** miscompilation, backend:NVPTX, llvm:ir
**Closed Date:** 2025-07-31T18:56:51Z

## Body

This issue is created as suggested in https://github.com/llvm/llvm-project/issues/107219.

https://cuda.godbolt.org/z/1ebcMfv87 highlights the differences in the PTX generated for the reproducer IR given below b/w LLVM 16 and current tip of main branch.

<details>
<summary>Original LLVM IR</summary>

```llvm
; LLVM version:16.0.6
; ModuleID = 'module'
source_filename = "module"
target datalayout = "e-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-cuda"

define void @KERNEL(ptr addrspace(1) align 8 %out0, <2 x i8> %in0, i32 %arraySize) addrspace(1) {
check:
  %tid_x = call i32 @llvm.nvvm.read.ptx.sreg.tid.x()
  %ntid_x = call i32 @llvm.nvvm.read.ptx.sreg.ntid.x()
  %ctaid_x = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()
  %ctaid_y = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.y()
  %nctaid_x = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.x()
  %0 = mul i32 %nctaid_x, %ctaid_y
  %1 = add i32 %0, %ctaid_x
  %2 = mul i32 %1, %ntid_x
  %linearTid = add i32 %2, %tid_x
  %3 = icmp ult i32 %linearTid, %arraySize
  %4 = extractelement <2 x i8> %in0, i64 0
  %5 = sitofp i8 %4 to double
  %6 = extractelement <2 x i8> %in0, i64 1
  %7 = sitofp i8 %6 to double
  %8 = insertelement <2 x double> undef, double %5, i64 0
  %9 = insertelement <2 x double> %8, double %7, i64 1
  br i1 %3, label %StraightLineCodeBlock, label %exit

exit:                                             ; preds = %StraightLineCodeBlock, %check
  ret void

StraightLineCodeBlock:                            ; preds = %check
  store <2 x double> %9, ptr addrspace(1) %out0, align 16
  br label %exit
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.nvvm.read.ptx.sreg.tid.x() #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.nvvm.read.ptx.sreg.ntid.x() #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.x() #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.y() #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.nvvm.read.ptx.sreg.nctaid.x() #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!nvvm.annotations = !{!0}

!0 = !{ptr addrspace(1) @KERNEL, !"kernel", i32 1}
```
</details>

<details>
<summary>C++ Driver Code</summary>

```cpp
// Type your code here, or load an example.

#include <array>
#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cuda.h>

inline void checkCudaErrors(CUresult err) {
  if (err != CUDA_SUCCESS)
    std::cout << std::string("CUDA Error: ") + std::to_string(err) << std::endl;
}

#define ALIGN_UP(offset, alignment)                                            \
  (offset) = ((offset) + (alignment) - 1) & ~((alignment) - 1)

#define ADD_TO_PARAM_BUFFER(value, alignment)                                  \
  do {                                                                         \
    ALIGN_UP(paramBufferSize, alignment);                                      \
    memcpy(paramBuffer + paramBufferSize, &(value), sizeof(value));            \
    paramBufferSize += sizeof(value);                                          \
  } while (0)

template <typename T, typename std::enable_if<std::is_pointer<T>::value,
                                              T>::type * = nullptr>
void ProcessInput(CUdeviceptr &devPtr, T input, int const numel,
                  char *paramBuffer, size_t &paramBufferSize) {
  checkCudaErrors(cuMemAlloc(&devPtr, sizeof(*input) * numel));
  checkCudaErrors(cuMemcpyHtoD(devPtr, input, sizeof(*input) * numel));
  ADD_TO_PARAM_BUFFER(devPtr, __alignof(devPtr));
}

template <typename T, typename std::enable_if<!std::is_pointer<T>::value,
                                              T>::type * = nullptr>
void ProcessInput(CUdeviceptr &devPtr, T input, int const numel,
                  char *paramBuffer, size_t &paramBufferSize) {
  ADD_TO_PARAM_BUFFER(input, __alignof(input));
}

template <typename T>
CUdeviceptr ProcessEachInput(T input, int const numel, char *paramBuffer,
                             size_t &paramBufferSize) {
  CUdeviceptr devPtr(0LL);
  ProcessInput<T>(devPtr, input, numel, paramBuffer, paramBufferSize);
  return devPtr;
}

// Forward declare
template <typename OutputType, typename... InputTypes>
int RunPtx(std::string const &ptx, int const numel, int deviceID,
           std::vector<OutputType *> const &outputPtrs, InputTypes... inputs);

/*
 * Overload to convert single outputs to vector of outputs
 */
template <typename OutputType, typename... InputTypes>
inline int RunPtx(std::string const &ptx, int const numel, int deviceID,
                  OutputType *output, InputTypes... inputs) {
  return RunPtx(ptx, numel, deviceID, std::vector<OutputType *>{output},
                inputs...);
}

template <typename OutputType, typename... InputTypes>
inline int RunPtx(std::string const &ptx, int const numel, int deviceID,
                  std::vector<OutputType *> const &outputPtrs,
                  InputTypes... inputs) {
  CUdevice device;
  CUmodule cudaModule;
  CUcontext context;
  CUfunction function;
  int devCount;

  checkCudaErrors(cuInit(0));
  checkCudaErrors(cuDeviceGetCount(&devCount));
  checkCudaErrors(cuDeviceGet(&device, deviceID));

  char name[128];
  checkCudaErrors(cuDeviceGetName(name, 128, device));
//   std::cout << "Using CUDA Device [0]: " << name << "\n";

  int devMajor, devMinor;
  checkCudaErrors(cuDeviceGetAttribute(
      &devMajor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device));
  checkCudaErrors(cuDeviceGetAttribute(
      &devMinor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device));
//   std::cout << "Device Compute Capability: " << devMajor << "." << devMinor
//             << "\n";
  if (devMajor < 2) {
    std::cerr << "ERROR: Device 0 is not SM 2.0 or greater\n";
    return 1;
  }
  checkCudaErrors(cuCtxCreate(&context, 0, device));

  checkCudaErrors(cuModuleLoadDataEx(&cudaModule, ptx.c_str(), 0, 0, 0));

  /* Get kernel function */
  checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "KERNEL"));

  CUdeviceptr numelDevPtr;
  checkCudaErrors(cuMemAlloc(&numelDevPtr, sizeof(numel)));
  checkCudaErrors(cuMemcpyHtoD(numelDevPtr, &numel, sizeof(numel)));

  /* Kernel parameters using EXTRA field */
  char paramBuffer[1024];
  size_t paramBufferSize = 0;

  /*Set up output and array size info*/
  size_t numOutputs = outputPtrs.size();
  std::vector<CUdeviceptr> outputDevPtrs(numOutputs);
  for (size_t i = 0; i < numOutputs; i++) {
    checkCudaErrors(cuMemAlloc(&outputDevPtrs[i], sizeof(OutputType) * numel));
    CUdeviceptr opPtr =
        outputDevPtrs[i]; // Fails to build on windows without this
    ADD_TO_PARAM_BUFFER(opPtr, __alignof(opPtr));
  }

  // Expand parameter pack of inputs
  CUdeviceptr inputDevPtrs[] = {
      ProcessEachInput(inputs, numel, paramBuffer, paramBufferSize)...};

  ADD_TO_PARAM_BUFFER(numel, __alignof(numel));

  unsigned blockSizeX = unsigned(numel);
  unsigned blockSizeY, blockSizeZ, gridSizeX, gridSizeY, gridSizeZ;
  blockSizeY = blockSizeZ = gridSizeX = gridSizeY = gridSizeZ = 1;

  void *extra[] = {CU_LAUNCH_PARAM_BUFFER_POINTER, paramBuffer,
                   CU_LAUNCH_PARAM_BUFFER_SIZE, &paramBufferSize,
                   CU_LAUNCH_PARAM_END};

//   std::cout << "Launching kernel\n";

  /* Kernel launch */
  checkCudaErrors(cuLaunchKernel(function, gridSizeX, gridSizeY, gridSizeZ,
                                 blockSizeX, blockSizeY, blockSizeZ, 0, NULL,
                                 NULL, extra));

  cudaDeviceSynchronize();

  for (size_t i = 0; i < numOutputs; i++) {
    checkCudaErrors(cuMemcpyDtoH(outputPtrs[i], outputDevPtrs[i],
                                 sizeof(OutputType) * numel));
    checkCudaErrors(cuMemFree(outputDevPtrs[i]));
  }

  for (unsigned i = 0; i < sizeof...(InputTypes); i++) {
    checkCudaErrors(cuMemFree(inputDevPtrs[i]));
  }

  checkCudaErrors(cuModuleUnload(cudaModule));
  checkCudaErrors(cuCtxDestroy(context));
  return 0;
}

bool readFileToString(std::string &ptx, std::string const &pathToFile) {
  try {
    std::ifstream t(pathToFile);
    std::stringstream buffer;
    buffer << t.rdbuf();
    ptx = buffer.str();
    return true;
  } catch (...) {
    return false;
  }
}

int main(void) {
  std::string llvm_16_ptx, llvm_19_ptx;
  readFileToString(llvm_16_ptx, "llvm16.ptx");
  readFileToString(llvm_19_ptx, "llvm19.ptx");

  int const numel = 1;
  std::complex<int8_t> input{1, 2};
  std::array<std::complex<double>, numel> output;

  output.fill(std::complex<double>{0.0, 0.0});

  std::cout << "Input:" << std::endl << static_cast<std::complex<int>>(input) << std::endl;

  RunPtx(llvm_16_ptx, numel, /*deviceID=*/0, output.data(), input);

  std::cout << "Output with LLVM 16:" << std::endl;
  for (int i = 0; i < numel; i++) {
    std::cout << output[i] << std::endl;
  }

  RunPtx(llvm_19_ptx, numel, /*deviceID=*/0, output.data(), input);

  std::cout << "Output with LLVM 19:" << std::endl;
  for (int i = 0; i < numel; i++) {
    std::cout << output[i] << std::endl;
  }
}
```
</details>

**Build commands**

```bash
# Generating PTX from the repro LLVM IR using version 16 and 19
<LLVM_16_LOCATION>/bin/llc -mcpu=sm_86 repro.ll -o llvm16.ptx

<LLVM_19_LOCATION>/bin/llc -mcpu=sm_86 repro.ll -o llvm19.ptx

# Generating executable using NVCC
<CUDA_LOCATION>/bin/nvcc -lcuda -std=c++17 repro.cu -o repro.out
```

**Execution**

```bash
$ ./repro.out 
Input:
(1,2)
Output with LLVM 16:
(1,2)
Output with LLVM 19:
(1,0)
```

## Comments

### Comment 1 - Artem-B

The problem is that in the generated code we're loading `<2 x i8>` as if it were `<2 x i16>`:

So, instead of `ld.param.v2.u8  {%rs1, %rs2}, [KERNEL_param_1];` we end up with this PTX:
```
        ld.param.u32    %r1, [KERNEL_param_1];
        mov.b32         {%rs1, %rs2}, %r1;
```



---

### Comment 2 - Artem-B

Minimal reproducer: https://cuda.godbolt.org/z/6bdh85sa5

---

### Comment 3 - Artem-B

It appears that the problem has been introduced in #111189 which was attempting to fix #104864



---

### Comment 4 - rashedmyt

> It appears that the problem has been introduced in https://github.com/llvm/llvm-project/pull/111189 which was attempting to fix https://github.com/llvm/llvm-project/issues/104864

Though it might appear that this is the cause of the issue, I should mention that this functional issue was introduced in LLVM18 - https://cuda.godbolt.org/z/heYdo4rnb

---

### Comment 5 - Artem-B

>  I should mention that this functional issue was introduced in LLVM18 - https://cuda.godbolt.org/z/heYdo4rnb

The code in this example is broken in a different way. It loads the parameter correctly, but messes up storing/loading a local copy, instead. Handling i8 vectors has changed quite a bit in recent versions, so whatever bugs we had in this code path in the older LLVM versions of llvm is somewhat irrelevant. We need to fix the mess we have now.

That said, #111189 may indeed be not the direct cause, but rather an incomplete or broken fix for the older state of the bug. It did address lowering of the return value, but broke handling of the v2i8 arguments (even more?).

---

### Comment 6 - rashedmyt

@Artem-B @AlexMaclean Whenever your schedule allows, could you please look into this issue? I’m not very familiar with the codebase to tackle it on my own, so I’d really appreciate your expertise.

---

### Comment 7 - AlexMaclean

Hi @rashedmyt! With #145585 kernel functions with v2i8 parameters should be working again so the specific reproducer in this issue is fixed https://cuda.godbolt.org/z/EfExYovdd. However, calling device functions with v2i8 parameters or return values is still broken. I'm working on fixing this now https://cuda.godbolt.org/z/hfaovPare

---

### Comment 8 - AlexMaclean

After landing https://github.com/llvm/llvm-project/pull/146930 device function calls with v2i8 should also be fixed. I'm going to go ahead and close this issue. @rashedmyt please re-open and let me know if there are any further issues!

---

