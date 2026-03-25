# Clang crashes in InstructionSelect::runOnMachineFunction when compiling PCP

**Author:** andre4ik3
**URL:** https://github.com/llvm/llvm-project/issues/153514
**Status:** Closed
**Labels:** llvm:optimizations, crash, needs-reduction
**Closed Date:** 2025-08-14T01:27:55Z

## Body

Clang crashes when attempting to compile `fetchgroup.c` from PCP:

```
clang -I../../../src/include -I../../../src/include/pcp -DPCP_VERSION=\"6.3.8\" -fPIC -no-cpp-precomp -fno-strict-aliasing -Wshadow -Wno-array-bounds '-DEXEC_SUFFIX=""' '-DDSO_SUFFIX="dylib"' -I/nix/store/kbs1gz7p2j7qd1lixqzkladkjdxs8nj7-openssl-3.4.1-dev/include -I/nix/store/vh6vxik364jciipwmsz4dwgb03inrrhy-cyrus-sasl-2.1.28-dev/include -DPCP_INTERNAL -Ideps -Wall -DPM_FAULT_INJECTION=1 -DPM_MULTI_THREAD_DEBUG=1 -g -O0     -c -o fetchgroup.o fetchgroup.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nix/store/5hgabdyqh22l57cb3qgx10lcc40d5n9p-clang-19.1.7/bin/clang @/tmp/nix-shell.HwoKyv/cc-params.cPwcC3
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'fetchgroup.c'.
4.      Running pass 'InstructionSelect' on function '@__pmExtractValue2'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.dylib            0x000000011469ccb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 72
1  libLLVM.dylib            0x000000011469ac30 llvm::sys::RunSignalHandlers() + 108
2  libLLVM.dylib            0x000000011469c1dc llvm::sys::CleanupOnSignal(unsigned long) + 276
3  libLLVM.dylib            0x00000001145d4ab0 CrashRecoverySignalHandler(int) + 168
4  libsystem_platform.dylib 0x000000018fab16a4 _sigtramp + 56
5  libLLVM.dylib            0x000000011520fb40 llvm::reportGISelFailure(llvm::MachineFunction&, llvm::TargetPassConfig const&, llvm::MachineOptimizationRemarkEmitter&, char const*, llvm::StringRef, llvm::MachineInstr const&) + 88
6  libLLVM.dylib            0x00000001151b3800 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&) + 2428
7  libLLVM.dylib            0x0000000114b2f088 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) + 572
8  libLLVM.dylib            0x00000001148449a8 llvm::FPPassManager::runOnFunction(llvm::Function&) + 488
9  libLLVM.dylib            0x000000011484b7f4 llvm::FPPassManager::runOnModule(llvm::Module&) + 60
10 libLLVM.dylib            0x0000000114845450 llvm::legacy::PassManagerImpl::run(llvm::Module&) + 2140
11 libclang-cpp.dylib       0x0000000109228708 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::__1::unique_ptr<llvm::raw_pwrite_stream, std::__1::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) + 7008
12 libclang-cpp.dylib       0x000000010959e9e0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) + 1852
13 libclang-cpp.dylib       0x0000000107bcbca4 clang::ParseAST(clang::Sema&, bool, bool) + 600
14 libclang-cpp.dylib       0x000000010a1009e4 clang::FrontendAction::Execute() + 120
15 libclang-cpp.dylib       0x000000010a08955c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 940
16 libclang-cpp.dylib       0x000000010a181484 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 512
17 clang-19                 0x00000001029abdac cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 4912
18 clang-19                 0x00000001029a940c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) + 1192
19 libclang-cpp.dylib       0x0000000109d09824 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) + 32
20 libLLVM.dylib            0x00000001145d4788 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 148
21 libclang-cpp.dylib       0x0000000109d08f90 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const + 352
22 libclang-cpp.dylib       0x0000000109cd1780 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 860
23 libclang-cpp.dylib       0x0000000109cd1a00 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const + 132
24 libclang-cpp.dylib       0x0000000109cebd20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) + 348
25 clang-19                 0x00000001029a8724 clang_main(int, char**, llvm::ToolContext const&) + 5484
26 clang-19                 0x00000001029b5240 main + 88
27 dyld                     0x000000018f6d6b98 start + 6076
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: arm64-apple-darwin
Thread model: posix
InstalledDir: /nix/store/5hgabdyqh22l57cb3qgx10lcc40d5n9p-clang-19.1.7/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/nix-shell.HwoKyv/fetchgroup-694910.c
clang: note: diagnostic msg: /tmp/nix-shell.HwoKyv/fetchgroup-694910.sh
clang: note: diagnostic msg: Crash backtrace is located in
clang: note: diagnostic msg: /Users/andre4ik3/Library/Logs/DiagnosticReports/clang-19_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang: note: diagnostic msg:
```

Here are the files it specified to attach:

[fetchgroup-694910.c](https://github.com/user-attachments/files/21764264/fetchgroup-694910.c.txt)
[fetchgroup-694910.sh](https://github.com/user-attachments/files/21764265/fetchgroup-694910.sh.txt)

Original source file:

https://github.com/performancecopilot/pcp/blob/7b0943ade0bcecaf50ed4b4c2cc9084150420eff/src/libpcp/src/fetchgroup.c

Unfortunately I wasn't able to find a clang-19_XXX.crash file in where it said it would be:

```
❯ \ls ~/Library/Logs/DiagnosticReports
nix-2025-08-06-025901.ips       nix-2025-08-11-131909.ips       nix-2025-08-12-020013.ips
nix-2025-08-11-114852.ips       nix-2025-08-11-133628.ips       Retired
nix-2025-08-11-131612.ips       nix-2025-08-12-000859.ips
```

## Comments

### Comment 1 - andre4ik3

This issue appears to be fixed on Clang/LLVM 20, as I wasn't able to reproduce it after updating it to that version.

---

