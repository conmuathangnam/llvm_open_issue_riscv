# Struct with void field crashes llc on apple-m1

**Author:** genericptr
**URL:** https://github.com/llvm/llvm-project/issues/119356
**Status:** Open
**Labels:** llvm:codegen, crash-on-invalid, llvm:crash

## Body

Sorry if this file has other junk but it's what I could get. In the original IR there is a bad type `type { i8, void }` which fails verification but crashes llc if I print the module to bitcode without verification.

Here is the `.bc` file in question and the crash log.

[test.bc.zip](https://github.com/user-attachments/files/18077570/test.bc.zip)


```console
$ /opt/homebrew/opt/llvm/bin/llc --version
Homebrew LLVM version 19.1.3
  Optimized build.
  Default target: arm64-apple-darwin24.0.0
  Host CPU: apple-m1

$ /opt/homebrew/opt/llvm/bin/llc -filetype obj test.bc -o test.o

PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/homebrew/opt/llvm/bin/llc -filetype obj test.bc -o test.o
 #0 0x000000010dd84fec llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x4a10fec)
 #1 0x000000010dd853d8 SignalHandler(int) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x4a113d8)
 #2 0x0000000180cf4184 (/usr/lib/system/libsystem_platform.dylib+0x180484184)
 #3 0x00000001095d1f7c llvm::StructLayout::StructLayout(llvm::StructType*, llvm::DataLayout const&) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x25df7c)
 #4 0x00000001095d1f7c llvm::StructLayout::StructLayout(llvm::StructType*, llvm::DataLayout const&) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x25df7c)
 #5 0x00000001095438dc llvm::DataLayout::getTypeSizeInBits(llvm::Type*) const (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x1cf8dc)
 #6 0x000000010a0f4b34 llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0xd80b34)
 #7 0x000000010a0ff230 llvm::AsmPrinter::doFinalization(llvm::Module&) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0xd8b230)
 #8 0x00000001096c8490 llvm::FPPassManager::doFinalization(llvm::Module&) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x354490)
 #9 0x00000001096a9f2c llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/homebrew/Cellar/llvm/19.1.3/lib/libLLVM.dylib+0x335f2c)
#10 0x000000010008a294 compileModule(char**, llvm::LLVMContext&) (/opt/homebrew/Cellar/llvm/19.1.3/bin/llc+0x100006294)
#11 0x00000001000882b0 main (/opt/homebrew/Cellar/llvm/19.1.3/bin/llc+0x1000042b0)
#12 0x000000018093c274 
```

