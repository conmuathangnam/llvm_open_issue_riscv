# clang: TwoAddressInstructionPass assertion failure when inline assembly has multiple output operands constrained to the same register

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170428
**Status:** Closed
**Labels:** duplicate, llvm:regalloc, crash, inline-asm
**Closed Date:** 2025-12-05T02:01:41Z

## Body

### Link for quick verification: https://godbolt.org/z/cnv71je8c
```c
int foo() {
    int a = 5;
    int n = 3;

    // Inline assembly with deliberate overlapping register allocation for two outputs into same register "a"
    asm volatile (
        ""
        : "=a"(a), "=a"(n)   // Output operands both use register "%a", causing overlap
        : "0"(n), "1"(n)   // Input values also assigned via same registers ("%" syntax)
    );

    return a + n;
}
```
When compiling code containing inline assembly with multiple output operands constrained to the same register, the compiler should either:

- Report a proper error about invalid inline assembly constraints, or

- Handle the case gracefully without crashing  

But LLVM's `TwoAddressInstructionPass` hits an assertion failure during the `processTiedPairs` function.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/CodeGen/TwoAddressInstructionPass.cpp:1596: void {anonymous}::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, TiedPairList&, unsigned int&): Assertion `i == DstIdx || !MI->getOperand(i).isReg() || MI->getOperand(i).getReg() != RegA' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 46446.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/46446-747408.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46446.fuzz
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46446.fuzz'.
4.      Running pass 'Two-Address instruction pass' on function '@foo'
 #0 0x000058802d178c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x000058802d179457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x000058802d172f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x000058802d1731c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007d3d8d092330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007d3d8d0ebb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007d3d8d0ebb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007d3d8d0ebb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007d3d8d09227e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007d3d8d0758ff abort ./stdlib/abort.c:81:7
#10 0x00007d3d8d07581b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007d3d8d088517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000058802c1ae9f8 (anonymous namespace)::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, llvm::SmallVector<std::pair<unsigned int, unsigned int>, 4u>&, unsigned int&) TwoAddressInstructionPass.cpp:0:0
#13 0x000058802c1b0ce3 (anonymous namespace)::TwoAddressInstructionImpl::run() TwoAddressInstructionPass.cpp:0:0
#14 0x000058802c1b3c1d (anonymous namespace)::TwoAddressInstructionLegacyPass::runOnMachineFunction(llvm::MachineFunction&) TwoAddressInstructionPass.cpp:0:0
#15 0x000058802bc9b586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#16 0x000058802c6956a9 llvm::FPPassManager::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#17 0x000058802c695e89 llvm::FPPassManager::runOnModule(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#18 0x000058802c694b77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#19 0x000058802c695469 llvm::legacy::PassManager::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#20 0x000058802d5a449e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#21 0x000058802df8d3a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#22 0x0000588030658eb5 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#23 0x000058802e3a092b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#24 0x000058802df8e360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#25 0x000058802e3a89cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#26 0x000058802e2eef46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#27 0x000058802e530f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#28 0x000058802829f523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#29 0x000058802828fc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000588028295b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#31 0x00005880280f0b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#32 0x00007d3d8d0771ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x00007d3d8d07728b call_init ./csu/../csu/libc-start.c:128:20
#34 0x00007d3d8d07728b __libc_start_main ./csu/../csu/libc-start.c:347:5
#35 0x000058802828df55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```

## Clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - frederick-vs-ja

Seemingly crashed in all old versions.

---

