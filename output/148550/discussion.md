# Clang crash when compiling libc++ for Wasm with `-mexception-handling`

**Author:** ianprime0509
**URL:** https://github.com/llvm/llvm-project/issues/148550

## Body

For context, I ran into this issue when trying to use Zig to compile C++ code for Wasm with exception handling enabled, but I was able to reduce the issue down to something more reasonable using Clang alone so I could submit this bug report. I'm not sure precisely what the issue is, so I just left "libc++" in the title since that's where the reproduction code comes from. I used C-Vise to help find the part causing the crash so that the reproducer isn't too big.

Reproducer files: [repro.tar.gz](https://github.com/user-attachments/files/21206485/repro.tar.gz)
Crash output:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -target wasm32-wasi -mexception-handling -O2 -c reduced.cpp
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(ee-instrument<>,lower-expect,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;no-switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,sroa<modify-cfg>,early-cse<>)" on module "reduced.cpp"
4.	Running pass "early-cse<>" on function "_ZnwmRKSt9nothrow_t"
 #0 0x00007f547882914a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x22914a)
 #1 0x00007f54788260d4 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x2260d4)
 #2 0x00007f547873f8e4 (/lib64/libLLVM.so.20.1+0x13f8e4)
 #3 0x00007f5478027c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #4 0x00007f547a387939 (/lib64/libLLVM.so.20.1+0x1d87939)
 #5 0x00007f547a387ef1 llvm::simplifyInstruction(llvm::Instruction*, llvm::SimplifyQuery const&) (/lib64/libLLVM.so.20.1+0x1d87ef1)
 #6 0x00007f5479b88199 (/lib64/libLLVM.so.20.1+0x1588199)
 #7 0x00007f5479b875d4 llvm::EarlyCSEPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/lib64/libLLVM.so.20.1+0x15875d4)
 #8 0x00007f547b3e9bd1 (/lib64/libLLVM.so.20.1+0x2de9bd1)
 #9 0x00007f5478a2287e llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/lib64/libLLVM.so.20.1+0x42287e)
#10 0x00007f547b3c66c1 (/lib64/libLLVM.so.20.1+0x2dc66c1)
#11 0x00007f5478a271a8 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.20.1+0x4271a8)
#12 0x00007f547b3d3271 (/lib64/libLLVM.so.20.1+0x2dd3271)
#13 0x00007f5478a21344 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.20.1+0x421344)
#14 0x00007f5482596fc1 (/lib64/libclang-cpp.so.20.1+0x1796fc1)
#15 0x00007f548258e8c1 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/lib64/libclang-cpp.so.20.1+0x178e8c1)
#16 0x00007f5482a334c8 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.20.1+0x1c334c8)
#17 0x00007f5481012d39 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212d39)
#18 0x00007f548371907f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291907f)
#19 0x00007f548367b8f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.20.1+0x287b8f4)
#20 0x00007f54837acb8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.20.1+0x29acb8e)
#21 0x0000556d169a6695 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-20+0xa695)
#22 0x0000556d169a2727 (/usr/bin/clang-20+0x6727)
#23 0x00007f548321bfcd (/lib64/libclang-cpp.so.20.1+0x241bfcd)
#24 0x00007f547873f5f0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib64/libLLVM.so.20.1+0x13f5f0)
#25 0x00007f548321b885 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/lib64/libclang-cpp.so.20.1+0x241b885)
#26 0x00007f54831d7b19 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/lib64/libclang-cpp.so.20.1+0x23d7b19)
#27 0x00007f54831d7dd7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/lib64/libclang-cpp.so.20.1+0x23d7dd7)
#28 0x00007f54831fa929 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/lib64/libclang-cpp.so.20.1+0x23fa929)
#29 0x0000556d169a1b54 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-20+0x5b54)
#30 0x0000556d169b19a8 main (/usr/bin/clang-20+0x159a8)
#31 0x00007f54780115f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#32 0x00007f54780116a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#33 0x0000556d1699fe05 _start (/usr/bin/clang-20+0x3e05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.7 (Fedora 20.1.7-1.fc42)
Target: wasm32-unknown-wasi
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/reduced-6c4c86.cpp
clang++: note: diagnostic msg: /tmp/reduced-6c4c86.sh
clang++: note: diagnostic msg: 

********************
```

