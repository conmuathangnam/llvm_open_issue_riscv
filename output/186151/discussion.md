# [HLSL][SPIRV] SPIRVEmitIntrinsics assertion failure for WavePrefixSum tests

**Author:** kcloudy0717
**URL:** https://github.com/llvm/llvm-project/issues/186151
**Status:** Open
**Labels:** crash, HLSL, backend:SPIR-V

## Body

All of clang-vulkan tests for WavePrefixSum test [PR](https://github.com/llvm/offload-test-suite/pull/934) are failing because it's hitting a assertion in the first if statement of `SPIRVEmitIntrinsics::walkLogicalAccessChain`

Code:
```cpp
bool SPIRVEmitIntrinsics::walkLogicalAccessChain(
    GetElementPtrInst &GEP,
    const std::function<void(Type *, uint64_t)> &OnLiteralIndexing,
    const std::function<void(Type *, Value *)> &OnDynamicIndexing) {
  // We only rewrite i8* GEP. Other should be left as-is.
  // Valid i8* GEP must always have a single index.
  assert(GEP.getSourceElementType() ==
         IntegerType::getInt8Ty(CurrF->getContext()));
  assert(GEP.getNumIndices() == 1);

  auto &DL = CurrF->getDataLayout();
  Value *Src = getPointerRoot(GEP.getPointerOperand());
  Type *CurType = deduceElementType(Src, true);

  Value *Operand = *GEP.idx_begin();
  ConstantInt *CI = dyn_cast<ConstantInt>(Operand);
  if (!CI) {
    ArrayType *AT = dyn_cast<ArrayType>(CurType);
    // Operand is not constant. Either we have an array and accept it, or we
    // give up.
    if (AT)
      OnDynamicIndexing(AT->getElementType(), Operand);
    return AT == nullptr;
  }

  assert(CI);
  uint64_t Offset = CI->getZExtValue();

  do {
    if (ArrayType *AT = dyn_cast<ArrayType>(CurType)) {
      uint32_t EltTypeSize = DL.getTypeSizeInBits(AT->getElementType()) / 8;
      assert(Offset < AT->getNumElements() * EltTypeSize);
      uint64_t Index = Offset / EltTypeSize;
      Offset = Offset - (Index * EltTypeSize);
      CurType = AT->getElementType();
      OnLiteralIndexing(CurType, Index);
    } else if (StructType *ST = dyn_cast<StructType>(CurType)) {
      uint32_t StructSize = DL.getTypeSizeInBits(ST) / 8;
      assert(Offset < StructSize);
      (void)StructSize;
      const auto &STL = DL.getStructLayout(ST);
      unsigned Element = STL->getElementContainingOffset(Offset);
      Offset -= STL->getElementOffset(Element);
      CurType = ST->getElementType(Element);
      OnLiteralIndexing(CurType, Element);
    } else if (auto *VT = dyn_cast<FixedVectorType>(CurType)) {
      Type *EltTy = VT->getElementType();
      TypeSize EltSizeBits = DL.getTypeSizeInBits(EltTy);
      assert(EltSizeBits % 8 == 0 &&
             "Element type size in bits must be a multiple of 8.");
      uint32_t EltTypeSize = EltSizeBits / 8;
      assert(Offset < VT->getNumElements() * EltTypeSize);
      uint64_t Index = Offset / EltTypeSize;
      Offset -= Index * EltTypeSize;
      CurType = EltTy;
      OnLiteralIndexing(CurType, Index);

    } else {
      // Unknown composite kind; give up.
      return true;
    }
  } while (Offset > 0);

  return false;
}
```

Output from runners:
```
# | Assertion failed: Offset < AT->getNumElements() * EltTypeSize, file E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Target\SPIRV\SPIRVEmitIntrinsics.cpp, line 760
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
# | Stack dump:
# | 0.	Program arguments: e:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\bin\\clang-dxc.exe -spirv -fspv-target-env=vulkan1.3 -fspv-extension=DXC --dxv-path=E:/actions-runner/_work/offload-test-suite/offload-test-suite/DXC/build/bin -T cs_6_5 -Fo E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\WaveOps\\Output\\WavePrefixSum.fp64.test.tmp.o E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\WaveOps\\Output\\WavePrefixSum.fp64.test.tmp/source.hlsl
# | 1.	<eof> parser at end of file
# | 2.	Code generation
# | 3.	Running pass 'SPIRV emit intrinsics' on module 'E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-vk\WaveOps\Output\WavePrefixSum.fp64.test.tmp/source.hlsl'.
# | Exception Code: 0xC000001D
# |  #0 0x00007ff71aa04246 HandleAbort E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\Windows\Signals.inc:371:0
# |  #1 0x00007ffaeeb31989 (C:\WINDOWS\System32\ucrtbase.dll+0xc1989)
# |  #2 0x00007ffaeeb14ab1 (C:\WINDOWS\System32\ucrtbase.dll+0xa4ab1)
# |  #3 0x00007ffaeeb32986 (C:\WINDOWS\System32\ucrtbase.dll+0xc2986)
# |  #4 0x00007ffaeeb32b61 (C:\WINDOWS\System32\ucrtbase.dll+0xc2b61)
# |  #5 0x00007ff71adf376d `anonymous namespace'::SPIRVEmitIntrinsics::walkLogicalAccessChain E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Target\SPIRV\SPIRVEmitIntrinsics.cpp:761:0
# |  #6 0x00007ff71adf3465 `anonymous namespace'::SPIRVEmitIntrinsics::getGEPTypeLogical E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Target\SPIRV\SPIRVEmitIntrinsics.cpp:832:0
# |  #7 0x00007ff71ade451b `anonymous namespace'::SPIRVEmitIntrinsics::runOnFunction E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Target\SPIRV\SPIRVEmitIntrinsics.cpp:3091:0
# |  #8 0x00007ff71ade200e `anonymous namespace'::SPIRVEmitIntrinsics::runOnModule E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Target\SPIRV\SPIRVEmitIntrinsics.cpp:3293:0
```

