# Crash when compiling aws-checksums 0.1.13

**Author:** res0nance
**URL:** https://github.com/llvm/llvm-project/issues/156855

## Body

Clang crash when compiling aws-checksum 0.1.13

Crashes on clang15.0.7, 20.1.8 and 21.1.0

[crc32c_sse42_asm-814597.c](https://github.com/user-attachments/files/22137007/crc32c_sse42_asm-814597.c)



```
 clang: llvm/lib/CodeGen/TwoAddressInstructionPass.cpp:1596: void {anonymous}::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, TiedPairList&, unsigned int&): Assertion `i == DstIdx  !MI->getOperand(i).isReg()  MI->getOperand(i).getReg() != RegA' failed.
 PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
 Stack dump:
 0.      Program arguments: /home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang --gcc-toolchain=/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/gcc-runtime --config clang++-raihaan-libstdc++.cfg -DDEBUG_BUILD -DHAVE_SYSCONF -I/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/include -isystem /home/raihaan/.conan2/p/b/aws-ca4847c9a0fab9/p/include -m64 -g -std=gnu99 -fPIC -Wall -Wstrict-prototypes -fno-omit-frame-pointer -Wextra -pedantic -Wno-long-long -fPIC -Wgnu -Wno-gnu-zero-variadic-macro-arguments -MD -MT CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o -MF CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o.d -o CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o -c /home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/source/intel/asm/crc32c_sse42_asm.c
 1.      <eof> parser at end of file
 2.      Code generation
 3.      Running pass 'Function Pass Manager' on module '/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/source/intel/asm/crc32c_sse42_asm.c'.
 4.      Running pass 'Two-Address instruction pass' on function '@aws_checksums_crc32c_hw'
  #0 0x0000000002373198 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2373198)
  #1 0x0000000002370cb4 llvm::sys::CleanupOnSignal(unsigned long) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2370cb4)
  #2 0x00000000022ae190 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007f941f9b6520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x00007f941fa0a9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
  #5 0x00007f941fa0a9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
  #6 0x00007f941fa0a9fc pthread_kill ./nptl/pthread_kill.c:89:10
  #7 0x00007f941f9b6476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
  #8 0x00007f941f99c7f3 abort ./stdlib/abort.c:81:7
  #9 0x00007f941f99c71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #10 0x00007f941f9ade96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #11 0x000000000196753f (anonymous namespace)::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, llvm::SmallVector<std::pair<unsigned int, unsigned int>, 4u>&, unsigned int&) TwoAddressInstructionPass.cpp:0:0
 #12 0x000000000196953d (anonymous namespace)::TwoAddressInstructionImpl::run() TwoAddressInstructionPass.cpp:0:0
 #13 0x000000000196c244 (anonymous namespace)::TwoAddressInstructionLegacyPass::runOnMachineFunction(llvm::MachineFunction&) TwoAddressInstructionPass.cpp:0:0
 #14 0x000000000167e43a llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
 #15 0x0000000001d6c0db llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x1d6c0db)
 #16 0x0000000001d6c4f1 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x1d6c4f1)
 #17 0x0000000001d6cfc8 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x1d6cfc8)
 #18 0x00000000026267b3 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x26267b3)
 #19 0x0000000002d49aab clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2d49aab)
 #20 0x00000000047e27ec clang::ParseAST(clang::Sema&, bool, bool) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x47e27ec)
 #21 0x00000000030ac721 clang::FrontendAction::Execute() (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x30ac721)
 #22 0x0000000003025503 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x3025503)
 #23 0x00000000031a0ca2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x31a0ca2)
 #24 0x0000000000cf172b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0xcf172b)
 #25 0x0000000000ce8ed8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
 #26 0x0000000002dbb329 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
 #27 0x00000000022ae5e1 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x22ae5e1)
 #28 0x0000000002dbc597 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
 #29 0x0000000002d7609b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2d7609b)
 #30 0x0000000002d770cf clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2d770cf)
 #31 0x0000000002d868c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0x2d868c5)
 #32 0x0000000000cee175 clang_main(int, char**, llvm::ToolContext const&) (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0xcee175)
 #33 0x0000000000bcf16f main (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0xbcf16f)
 #34 0x00007f941f99dd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
 #35 0x00007f941f99de40 call_init ./csu/../csu/libc-start.c:128:20
 #36 0x00007f941f99de40 __libc_start_main ./csu/../csu/libc-start.c:379:5
 #37 0x0000000000ce8155 _start (/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang+0xce8155)
 clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
 clang version 21.1.0
 Target: x86_64-pc-linux-gnu
 Thread model: posix
 InstalledDir: /home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin
 Build config: +assertions
 Configuration file: /home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang++-raihaan-libstdc++.cfg
 clang: note: diagnostic msg:
 ****
 
 PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
 Preprocessed source(s) and associated run script(s) are located at:
 clang: note: diagnostic msg: /tmp/crc32c_sse42_asm-814597.c
 clang: note: diagnostic msg: /tmp/crc32c_sse42_asm-814597.sh
 clang: note: diagnostic msg:
 
 ****
```

```
# Crash reproducer for clang version 21.1.0
 # Driver args: "--stdlib=libstdc++" "--rtlib=libgcc" "--unwindlib=libgcc" "--gcc-toolchain=/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/gcc-runtime" "-D" "DEBUG_BUILD" "-D" "HAVE_SYSCONF" "-I" "/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/include" "-isystem" "/home/raihaan/.conan2/p/b/aws-ca4847c9a0fab9/p/include" "-m64" "-g" "-std=gnu99" "-fPIC" "-Wall" "-Wstrict-prototypes" "-fno-omit-frame-pointer" "-Wextra" "-pedantic" "-Wno-long-long" "-fPIC" "-Wgnu" "-Wno-gnu-zero-variadic-macro-arguments" "-MD" "-MT" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o" "-MF" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o.d" "-o" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o" "-c" "/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/source/intel/asm/crc32c_sse42_asm.c"
 # Original command:  "/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang-21" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "crc32c_sse42_asm.c" "-mrelocation-model" "pic" "-pic-level" "2" "-fhalf-no-semantic-interposition" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/build/Debug" "-fcoverage-compilation-dir=/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/build/Debug" "-resource-dir" "/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/lib/clang/21" "-dependency-file" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o.d" "-MT" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o" "-sys-header-deps" "-isystem" "/home/raihaan/.conan2/p/b/aws-ca4847c9a0fab9/p/include" "-D" "DEBUG_BUILD" "-D" "HAVE_SYSCONF" "-I" "/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/include" "-internal-isystem" "/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/lib/clang/21/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/gcc-runtime/lib/gcc/x86_64-pc-linux-gnu/15.2.0/../../../../x86_64-pc-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wall" "-Wstrict-prototypes" "-Wextra" "-Wno-long-long" "-Wgnu" "-Wno-gnu-zero-variadic-macro-arguments" "-pedantic" "-std=gnu99" "-ferror-limit" "19" "-fmessage-length=231" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "CMakeFiles/aws-checksums.dir/source/intel/asm/crc32c_sse42_asm.c.o" "-x" "c" "/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/src/source/intel/asm/crc32c_sse42_asm.c"
  "/home/raihaan/.conan2/p/b/clang6767e9f19be7e/p/bin/clang-21" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-main-file-name" "crc32c_sse42_asm.c" "-mrelocation-model" "pic" "-pic-level" "2" "-fhalf-no-semantic-interposition" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/build/Debug" "-fcoverage-compilation-dir=/home/raihaan/.conan2/p/b/aws-c86ca1a17aa797/b/build/Debug" "-sys-header-deps" "-D" "DEBUG_BUILD" "-D" "HAVE_SYSCONF" "-Wall" "-Wstrict-prototypes" "-Wextra" "-Wno-long-long" "-Wgnu" "-Wno-gnu-zero-variadic-macro-arguments" "-pedantic" "-std=gnu99" "-ferror-limit" "19" "-fmessage-length=231" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "crc32c_sse42_asm-814597.c"

```

## Comments

### Comment 1 - res0nance

Possibly linked to:
https://github.com/llvm/llvm-project/issues/118247
https://github.com/llvm/llvm-project/issues/54957
https://github.com/llvm/llvm-project/issues/40330

---

