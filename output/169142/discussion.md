# `llvm-lipo -info` segfaults when given a (particular?) non-fat file

**Author:** mdaniels5757
**URL:** https://github.com/llvm/llvm-project/issues/169142
**Status:** Closed
**Labels:** llvm-tools, crash
**Closed Date:** 2025-12-06T22:41:52Z

## Body

Input file: [attached in tarball form](https://github.com/user-attachments/files/23686387/libLLVMAArch64AsmParser.a.tar.gz)

Command and output (on aarch64-darwin, LLVM from nixpkgs):
```
% llvm-lipo -info libLLVMAArch64AsmParser.a
Non-fat file: libLLVMAArch64AsmParser.a is architecture: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-lipo -info libLLVMAArch64AsmParser.a
 #0 0x000000010bf70ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/ml5z7dif4v7n53giamrh1xynail92g9d-llvm-21.1.2-lib/lib/libLLVM.dylib+0x184ca8)
 #1 0x000000010bf6e6b0 llvm::sys::RunSignalHandlers() (/nix/store/ml5z7dif4v7n53giamrh1xynail92g9d-llvm-21.1.2-lib/lib/libLLVM.dylib+0x1826b0)
 #2 0x000000010bf71870 SignalHandler(int, __siginfo*, void*) (/nix/store/ml5z7dif4v7n53giamrh1xynail92g9d-llvm-21.1.2-lib/lib/libLLVM.dylib+0x185870)
 #3 0x00000001810456a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #4 0x000000010dd2d260 llvm::object::Slice::create(llvm::object::IRObjectFile const&, unsigned int) (/nix/store/ml5z7dif4v7n53giamrh1xynail92g9d-llvm-21.1.2-lib/lib/libLLVM.dylib+0x1f41260)
 #5 0x000000010dd2d260 llvm::object::Slice::create(llvm::object::IRObjectFile const&, unsigned int) (/nix/store/ml5z7dif4v7n53giamrh1xynail92g9d-llvm-21.1.2-lib/lib/libLLVM.dylib+0x1f41260)
 #6 0x0000000102bb5310 (anonymous namespace)::createSliceFromIR(llvm::object::IRObjectFile const&, unsigned int) (/nix/store/7niib6al50jnbnxkmx36jvjijrqc5hxm-llvm-21.1.2/bin/llvm-lipo+0x100009310)
 #7 0x0000000102bb59c8 printBinaryArchs(llvm::LLVMContext&, llvm::object::Binary const*, llvm::raw_ostream&) (/nix/store/7niib6al50jnbnxkmx36jvjijrqc5hxm-llvm-21.1.2/bin/llvm-lipo+0x1000099c8)
 #8 0x0000000102bb3074 printInfo(llvm::LLVMContext&, llvm::ArrayRef<llvm::object::OwningBinary<llvm::object::Binary>>) (/nix/store/7niib6al50jnbnxkmx36jvjijrqc5hxm-llvm-21.1.2/bin/llvm-lipo+0x100007074)
 #9 0x0000000102bb215c llvm_lipo_main(int, char**, llvm::ToolContext const&) (/nix/store/7niib6al50jnbnxkmx36jvjijrqc5hxm-llvm-21.1.2/bin/llvm-lipo+0x10000615c)
#10 0x0000000102bbabf4 main (/nix/store/7niib6al50jnbnxkmx36jvjijrqc5hxm-llvm-21.1.2/bin/llvm-lipo+0x10000ebf4)
#11 0x0000000180c6ab98
zsh: segmentation fault  llvm-lipo -info libLLVMAArch64AsmParser.a
```

Command and output (on x86_64-linux, LLVM from Arch Linux):
```
% llvm-lipo -info libLLVMAArch64AsmParser.a
Non-fat file: libLLVMAArch64AsmParser.a is architecture: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-lipo -info libLLVMAArch64AsmParser.a
 #0 0x000079d22a10832d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.21.1+0x90832d)
 #1 0x000079d22a105407 (/usr/lib/libLLVM.so.21.1+0x905407)
 #2 0x000079d22903e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #3 0x000079d22c964665 llvm::object::IRObjectFile::getTargetTriple() const (/usr/lib/libLLVM.so.21.1+0x3164665)
 #4 0x000079d22c9a51d3 llvm::object::Slice::create(llvm::object::IRObjectFile const&, unsigned int) (/usr/lib/libLLVM.so.21.1+0x31a51d3)
 #5 0x000056eb2123a7aa (/usr/bin/llvm-lipo+0xa7aa)
 #6 0x000056eb2123bfb2 (/usr/bin/llvm-lipo+0xbfb2)
 #7 0x000056eb21240fd2 (/usr/bin/llvm-lipo+0x10fd2)
 #8 0x000056eb212342d0 (/usr/bin/llvm-lipo+0x42d0)
 #9 0x000079d229027635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#10 0x000079d2290276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#11 0x000079d2290276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#12 0x000056eb21234335 (/usr/bin/llvm-lipo+0x4335)
zsh: segmentation fault (core dumped)  llvm-lipo -info libLLVMAArch64AsmParser.a
```

LLVM version: 21.1.2 (on aarch64-darwin/Nix); 21.1.5 (on x86_64-linux/Arch)


## Comments

### Comment 1 - rjmansfield

This is fixed on `main` with: 

```
commit 30002f22b95ca084e3e2cc23734ccda24ca5502c
Author: Keith Smiley <keithbsmiley@gmail.com>
Date:   Thu Aug 28 10:02:29 2025 -0700

    [llvm-lipo] Add support for -info with archive files (#155309)
    
    Previously trying to print the info of an archive caused a crash. Now we
    correctly report the architecture of the contained object files.
    
    Fixes https://github.com/llvm/llvm-project/issues/41655
```

---

### Comment 2 - mdaniels5757

Indeed it is! Thanks for letting me know!

Closing.

---

