# Clang CodeGenPrepare asserts building llvmlibc with LLVM_ENABLE_EXPENSIVE_CHECKS=O

**Author:** vrukesh
**URL:** https://github.com/llvm/llvm-project/issues/188332
**Status:** Open
**Labels:** llvm:codegen, crash, needs-reduction

## Body

**What code / commands /steps will reproduce the problem?**
1. Download LLVM based Arm Toolchain for Embedded: https://github.com/arm/arm-toolchain.git 
2. Compile it as 2-stage pipeline.
3. Compile with LLVM_ENABLE_EXPENSIVE_CHECKS=ON ; LLVM_ENABLE_ASSERTIONS=ON ; CMAKE_BUILD_TYPE=Debug


**What is the expected result?**
Compilation should be successful.

**What happens instead?**
Build aborts with `Pass modifies its input and doesn't report it: CodeGen Prepare` while compiling `libc/src/math/generic/sincos.cpp` and `cos.cpp`.

Stack traces point to `llvm/lib/IR/LegacyPassManager.cpp:1404`

Detailed logs:
```
[10/40] Performing build step for 'llvmlibc'
[775/1015] Building CXX object libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj
FAILED: libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj 
/home/ubuntu/arm-toolchain/build_assertions/llvm/bin/clang++ --target=aarch64-none-elf -DLIBC_NAMESPACE=__llvm_libc_23_0_0_git -I/home/ubuntu/arm-toolchain/libc -isystem /home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/llvmlibc/aarch64a_exn_rtti_unaligned/build/libc/include --target=aarch64-none-elf -march=armv8-a -fPIC -ffixed-x18 --sysroot /home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/runtimes/aarch64a_exn_rtti_unaligned/build/tmp_install -I/home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/runtimes/aarch64a_exn_rtti_unaligned/build/tmp_install/include -ffunction-sections -fdata-sections -fno-ident -Wno-error=atomic-alignment -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -ffunction-sections -fdata-sections -Xclang -fno-pch-timestamp -O3 -DNDEBUG --target=aarch64-none-elf -D__LIBC_USE_BUILTIN_CEIL_FLOOR_RINT_TRUNC -D__LIBC_USE_BUILTIN_ROUND -D__LIBC_USE_BUILTIN_ROUNDEVEN -DLIBC_COPT_STRTOFLOAT_DISABLE_EISEL_LEMIRE -DLIBC_COPT_STRTOFLOAT_DISABLE_CLINGER_FAST_PATH -DLIBC_QSORT_IMPL=LIBC_QSORT_HEAP_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=element -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=element "-DLIBC_MATH=(LIBC_MATH_SKIP_ACCURATE_PASS | LIBC_MATH_SMALL_TABLES | LIBC_MATH_NO_ERRNO | LIBC_MATH_NO_EXCEPT | LIBC_MATH_INTERMEDIATE_COMP_IN_FLOAT)" -fno-math-errno -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_SHARED -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_SINGLE -DLIBC_CONF_WCTYPE_MODE=LIBC_WCTYPE_MODE_ASCII -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_CTYPE_SMALLER_ASCII -DLIBC_COPT_PRINTF_DISABLE_WIDE -DLIBC_COPT_PRINTF_DISABLE_BITINT -DLIBC_TARGET_OS_IS_BAREMETAL -fpie -ffreestanding -DLIBC_FULL_BUILD -nostdlibinc -ffixed-point -fno-builtin -fno-exceptions -fno-lax-vector-conversions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ftrivial-auto-var-init=pattern -fno-omit-frame-pointer -Wall -Wextra -Werror -Wconversion -Wno-sign-conversion -Wdeprecated -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wimplicit-fallthrough -Wwrite-strings -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wstrict-prototypes -Wthread-safety -Wglobal-constructors -DLIBC_COPT_PUBLIC_PACKAGING -MD -MT libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj -MF libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj.d -o libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj -c /home/ubuntu/arm-toolchain/libc/src/math/generic/sincos.cpp
Pass modifies its input and doesn't report it: CodeGen Prepare
Pass modifies its input and doesn't report it
UNREACHABLE executed at /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:1404!
PLEASE submit a bug report to https://github.com/arm/arm-toolchain/issues and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/ubuntu/arm-toolchain/build_assertions/llvm/bin/clang++ --target=aarch64-none-elf -DLIBC_NAMESPACE=__llvm_libc_23_0_0_git -I/home/ubuntu/arm-toolchain/libc -isystem /home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/llvmlibc/aarch64a_exn_rtti_unaligned/build/libc/include --target=aarch64-none-elf -march=armv8-a -fPIC -ffixed-x18 --sysroot /home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/runtimes/aarch64a_exn_rtti_unaligned/build/tmp_install -I/home/ubuntu/arm-toolchain/build_assertions/multilib-llvmlibc-builds/runtimes/aarch64a_exn_rtti_unaligned/build/tmp_install/include -ffunction-sections -fdata-sections -fno-ident -Wno-error=atomic-alignment -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -ffunction-sections -fdata-sections -Xclang -fno-pch-timestamp -O3 -DNDEBUG --target=aarch64-none-elf -D__LIBC_USE_BUILTIN_CEIL_FLOOR_RINT_TRUNC -D__LIBC_USE_BUILTIN_ROUND -D__LIBC_USE_BUILTIN_ROUNDEVEN -DLIBC_COPT_STRTOFLOAT_DISABLE_EISEL_LEMIRE -DLIBC_COPT_STRTOFLOAT_DISABLE_CLINGER_FAST_PATH -DLIBC_QSORT_IMPL=LIBC_QSORT_HEAP_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=element -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=element "-DLIBC_MATH=(LIBC_MATH_SKIP_ACCURATE_PASS | LIBC_MATH_SMALL_TABLES | LIBC_MATH_NO_ERRNO | LIBC_MATH_NO_EXCEPT | LIBC_MATH_INTERMEDIATE_COMP_IN_FLOAT)" -fno-math-errno -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_SHARED -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_SINGLE -DLIBC_CONF_WCTYPE_MODE=LIBC_WCTYPE_MODE_ASCII -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_CTYPE_SMALLER_ASCII -DLIBC_COPT_PRINTF_DISABLE_WIDE -DLIBC_COPT_PRINTF_DISABLE_BITINT -DLIBC_TARGET_OS_IS_BAREMETAL -fpie -ffreestanding -DLIBC_FULL_BUILD -nostdlibinc -ffixed-point -fno-builtin -fno-exceptions -fno-lax-vector-conversions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ftrivial-auto-var-init=pattern -fno-omit-frame-pointer -Wall -Wextra -Werror -Wconversion -Wno-sign-conversion -Wdeprecated -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wimplicit-fallthrough -Wwrite-strings -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wstrict-prototypes -Wthread-safety -Wglobal-constructors -DLIBC_COPT_PUBLIC_PACKAGING -MD -MT libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj -MF libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj.d -o libc/src/math/generic/CMakeFiles/libc.src.math.generic.sincos.dir/sincos.cpp.obj -c /home/ubuntu/arm-toolchain/libc/src/math/generic/sincos.cpp
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/home/ubuntu/arm-toolchain/libc/src/math/generic/sincos.cpp'.
4.	Running pass 'CodeGen Prepare' on function '@_ZN22__llvm_libc_23_0_0_git4math6sincosEdPdS1_'
 #0 0x0000ab544db9ab68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/ubuntu/arm-toolchain/llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x0000ab544db9b084 PrintStackTraceSignalHandler(void*) /home/ubuntu/arm-toolchain/llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000ab544db992ac llvm::sys::RunSignalHandlers() /home/ubuntu/arm-toolchain/llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000ab544db9a41c llvm::sys::CleanupOnSignal(unsigned long) /home/ubuntu/arm-toolchain/llvm/lib/Support/Unix/Signals.inc:396:1
 #4 0x0000ab544daad750 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/ubuntu/arm-toolchain/llvm/lib/Support/CrashRecoveryContext.cpp:71:7
 #5 0x0000ab544daadb8c CrashRecoverySignalHandler(int) /home/ubuntu/arm-toolchain/llvm/lib/Support/CrashRecoveryContext.cpp:393:5
 #6 0x0000f34a031648f8 (linux-vdso.so.1+0x8f8)
 #7 0x0000f34a02ce2008 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #8 0x0000f34a02c9a83c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000f34a02c87134 abort ./stdlib/abort.c:81:7
#10 0x0000ab544dabad08 llvm::install_out_of_memory_new_handler() /home/ubuntu/arm-toolchain/llvm/lib/Support/ErrorHandling.cpp:225:0
#11 0x0000ab544d28b134 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:1408:11
#12 0x0000ab544d28f3b0 llvm::FPPassManager::runOnModule(llvm::Module&) /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#13 0x0000ab544d28b940 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#14 0x0000ab544d28b4f0 llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:531:16
#15 0x0000ab544d28f6bc llvm::legacy::PassManager::run(llvm::Module&) /home/ubuntu/arm-toolchain/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#16 0x0000ab544ec8c408 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>&, std::__1::unique_ptr<llvm::ToolOutputFile, std::__1::default_delete<llvm::ToolOutputFile>>&) /home/ubuntu/arm-toolchain/clang/lib/CodeGen/BackendUtil.cpp:1277:19
#17 0x0000ab544ec86c54 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/ubuntu/arm-toolchain/clang/lib/CodeGen/BackendUtil.cpp:1301:3
#18 0x0000ab544ec86150 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/ubuntu/arm-toolchain/clang/lib/CodeGen/BackendUtil.cpp:1475:13
#19 0x0000ab544ecaeffc clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/ubuntu/arm-toolchain/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#20 0x0000ab5451a06848 clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu/arm-toolchain/clang/lib/Parse/ParseAST.cpp:190:12
#21 0x0000ab544f5347c4 clang::ASTFrontendAction::ExecuteAction() /home/ubuntu/arm-toolchain/clang/lib/Frontend/FrontendAction.cpp:1448:1
#22 0x0000ab544ecb296c clang::CodeGenAction::ExecuteAction() /home/ubuntu/arm-
.
.
.
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git
Target: aarch64-unknown-none-elf
Thread model: posix
InstalledDir: /home/ubuntu/arm-toolchain/build_assertions/llvm/bin
Build config: +unoptimized, +assertions, +expensive-checks
Arm Toolchain ID: E0000-pre (f167bad1)
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/sincos-eff6a8.cpp
clang++: note: diagnostic msg: /tmp/sincos-eff6a8.sh
clang++: note: diagnostic msg: 
```

[atfe_assertions.sh](https://github.com/user-attachments/files/26225033/atfe_assertions.sh)
[console_log.log](https://github.com/user-attachments/files/26225036/console_log.log)
[sincos-eff6a8.cpp](https://github.com/user-attachments/files/26225035/sincos-eff6a8.cpp)
[sincos-eff6a8.sh](https://github.com/user-attachments/files/26225034/sincos-eff6a8.sh)

