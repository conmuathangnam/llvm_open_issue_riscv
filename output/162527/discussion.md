# Segmentation Fault when building AflPlusPlus

**Author:** marvi3
**URL:** https://github.com/llvm/llvm-project/issues/162527
**Status:** Open
**Labels:** crash, LTO

## Body

Hello,

I am running in a Problem when trying to build AFL and would like to know if there is something you can help me with.

A similar problem has happened to others like [llvm-issue 77404](https://github.com/llvm/llvm-project/issues/77404)

<details>
  <summary>Output of make and error</summary>

```
    PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
    Stack dump:
    0.      Program arguments: /usr/bin/clang-19 -cc1 -triple x86_64-suse-linux -emit-obj -dumpdir test-instr- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test-instr.c -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/vol/fob-vol7/mi19/dielfeli/fuzzing/fuzzer/AFLplusplus -fcoverage-compilation-dir=/vol/fob-vol7/mi19/dielfeli/fuzzing/fuzzer/AFLplusplus -resource-dir /usr/lib64/clang/19 -D __AFL_COMPILER=1 -D FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION=1 -D "__AFL_COVERAGE()=int __afl_selective_coverage __attribute__ ((weak)) = 1;void __afl_coverage_discard();void __afl_coverage_skip();void __afl_coverage_on();void __afl_coverage_off();" -D "__AFL_COVERAGE_START_OFF()=int __afl_selective_coverage_start_off = 1;" -D __AFL_COVERAGE_ON()=__afl_coverage_on() -D __AFL_COVERAGE_OFF()=__afl_coverage_off() -D __AFL_COVERAGE_DISCARD()=__afl_coverage_discard() -D __AFL_COVERAGE_SKIP()=__afl_coverage_skip() -D __AFL_HAVE_MANUAL_CONTROL=1 -D "__AFL_FUZZ_INIT()=int __afl_sharedmem_fuzzing = 1;extern __attribute__((visibility(\"default\"))) unsigned int *__afl_fuzz_len;extern __attribute__((visibility(\"default\"))) unsigned char *__afl_fuzz_ptr;unsigned char __afl_fuzz_alt[1048576];unsigned char *__afl_fuzz_alt_ptr = __afl_fuzz_alt;" -D "__AFL_FUZZ_TESTCASE_BUF=(__afl_fuzz_ptr ? __afl_fuzz_ptr : __afl_fuzz_alt_ptr)" -D "__AFL_FUZZ_TESTCASE_LEN=(__afl_fuzz_ptr ? *__afl_fuzz_len : (*__afl_fuzz_len = read(0, __afl_fuzz_alt_ptr, 1048576)) == 0xffffffff ? 0 : *__afl_fuzz_len)" -D "__AFL_LOOP(_A)=({ static volatile const char *_B __attribute__((used,unused));  _B = (const char*)\"##SIG_AFL_PERSISTENT##\"; extern __attribute__((visibility(\"default\"))) int __afl_connected;__attribute__((visibility(\"default\"))) int _L(unsigned int) __asm__(\"__afl_persistent_loop\"); _L(__afl_connected ? _A : 1); })" -D "__AFL_INIT()=do { static volatile const char *_A __attribute__((used,unused));  _A = (const char*)\"##SIG_AFL_DEFER_FORKSRV##\"; __attribute__((visibility(\"default\"))) void _I(void) __asm__(\"__afl_manual_init\"); _I(); } while (0)" -internal-isystem /usr/lib64/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-suse-linux/14/../../../../x86_64-suse-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -O3 -Wno-unused-command-line-argument -ferror-limit 19 -funroll-loops -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fpass-plugin=./SanitizerCoveragePCGUARD.so -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-instr-fc0aa0.o -x c test-instr.c
    1.      <eof> parser at end of file
    2.      Optimizer
    3.      Running pass "(anonymous namespace)::ModuleSanitizerCoverageAFL" on module "test-instr.c"
     #0 0x00007fad8f8475b9 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib64/libLLVM.so.19.1+0xa475b9)
     #1 0x00007fad8f844bd0 llvm::sys::RunSignalHandlers() (/usr/lib64/libLLVM.so.19.1+0xa44bd0)
     #2 0x00007fad8f847d5b SignalHandler(int) (/usr/lib64/libLLVM.so.19.1+0xa47d5b)
     #3 0x00007fad8e657900 __restore_rt (/lib64/libc.so.6+0x57900)
     #4 0x00007fad8f98aac5 llvm::GlobalVariable::GlobalVariable(llvm::Module&, llvm::Type*, bool, llvm::GlobalValue::LinkageTypes, llvm::Constant*, llvm::Twine const&, llvm::GlobalVariable*, llvm::GlobalValue::ThreadLocalMode, std::optional<unsigned int>, bool) (/usr/lib64/libLLVM.so.19.1+0xb8aac5)
     #5 0x00007fad8e87cf44 (anonymous namespace)::ModuleSanitizerCoverageAFL::instrumentModule(llvm::Module&, llvm::function_ref<llvm::DominatorTree const* (llvm::Function&)>, llvm::function_ref<llvm::PostDominatorTree const* (llvm::Function&)>) /vol/fob-vol7/mi19/dielfeli/fuzzing/fuzzer/AFLplusplus/instrumentation/SanitizerCoveragePCGUARD.so.cc:527:19
     #6 0x00007fad8e87c5a9 llvm::PreservedAnalyses::none() /usr/include/llvm/IR/Analysis.h:114:44
     #7 0x00007fad8e87c5a9 (anonymous namespace)::ModuleSanitizerCoverageAFL::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /vol/fob-vol7/mi19/dielfeli/fuzzing/fuzzer/AFLplusplus/instrumentation/SanitizerCoveragePCGUARD.so.cc:290:14
     #8 0x00007fad8e87c5a9 llvm::detail::PassModel<llvm::Module, (anonymous namespace)::ModuleSanitizerCoverageAFL, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /usr/include/llvm/IR/PassManagerInternal.h:90:17
     #9 0x00007fad8fa07826 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib64/libLLVM.so.19.1+0xc07826)
    #10 0x00007fad9866b34e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) (/usr/lib64/libclang-cpp.so.19.1+0x206b34e)
    #11 0x00007fad98664f1b clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib64/libclang-cpp.so.19.1+0x2064f1b)
    #12 0x00007fad98a6d294 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib64/libclang-cpp.so.19.1+0x246d294)
    #13 0x00007fad9710feb9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib64/libclang-cpp.so.19.1+0xb0feb9)
    #14 0x00007fad996cbd16 clang::FrontendAction::Execute() (/usr/lib64/libclang-cpp.so.19.1+0x30cbd16)
    #15 0x00007fad99635894 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib64/libclang-cpp.so.19.1+0x3035894)
    #16 0x00007fad9974c6af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib64/libclang-cpp.so.19.1+0x314c6af)
    #17 0x00005641f5d13bee cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-19+0x10bee)
    #18 0x00005641f5d10398 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/usr/bin/clang-19+0xd398)
    #19 0x00005641f5d0f3af clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-19+0xc3af)
    #20 0x00005641f5d1d019 main (/usr/bin/clang-19+0x1a019)
    #21 0x00007fad8e640e6c __libc_start_call_main (/lib64/libc.so.6+0x40e6c)
    #22 0x00007fad8e640f35 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x40f35)
    #23 0x00005641f5d0dd91 _start (/usr/bin/clang-19+0xad91)
    clang: error: unable to execute command: Segmentation fault (core dumped)
    clang: error: clang frontend command failed due to signal (use -v to see invocation)
    clang version 19.1.7
    Target: x86_64-suse-linux
    Thread model: posix
    InstalledDir: /usr/bin
    clang: note: diagnostic msg:
    ********************
    
    PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
    Preprocessed source(s) and associated run script(s) are located at:
    clang: note: diagnostic msg: /tmp/test-instr-364018.c
    clang: note: diagnostic msg: /tmp/test-instr-364018.sh
    clang: note: diagnostic msg:
    
    ********************
    Oops, afl-cc failed
    make: *** [GNUmakefile:622: test_build] Error 1
```
</details>

The interesting part is also, that the build itself seem to work just fine since afterwards all the afl-binaries are created:

<img width="1772" height="215" alt="Image" src="https://github.com/user-attachments/assets/eebae8f4-4b4c-42de-ba63-a85421fbb704" />

I have reported the problem already to AFL where I was told it seems to be an llvm-problem ([afl-issue 2371](https://github.com/AFLplusplus/AFLplusplus/issues/2371))

The two files for reproducing the crash are the following. If you need anything else from my side to help with this bug I will try my very best to help.
[test-instr-364018.sh](https://github.com/user-attachments/files/22783845/test-instr-364018.sh)
[test-instr-364018.c](https://github.com/user-attachments/files/22783844/test-instr-364018.c)

Thank you already for any possible help and best wishes,
marvi3

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch?

---

### Comment 2 - marvi3

Hey, I have tried several different versions like 17, 19 and 20 and none worked. I will try 21 tomorrow and report if anything changed. Thanks for the quick reply!

---

### Comment 3 - nikic

Based on your backtrace, the crash is not "when building AflPlusPlus" but "when *using* AflPlusPlus". The crash is inside ModuleSanitizerCoverageLTO, which is a pass defined by AFL, not LLVM. While it's not impossible that this is an LLVM bug exposed by the AFL pass, it's much more likely to be an issue with AFL's instrumentation pass.

---

