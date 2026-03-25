# [clang] clang: error: clang frontend command failed with exit code 132 (use -v to see invocation)

**Author:** vimalk78
**URL:** https://github.com/llvm/llvm-project/issues/132594
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

```
❯ clang --version
clang version 19.1.7 (Fedora 19.1.7-2.fc41)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang.cfg
```


```
Stack dump:
0.      Program arguments: clang -O2 -mcpu=v1 -O2 -g -Wall -I ./bpf/ -I/usr/include -Wunused-command-line-argument -target bpfel -c /home/vimalkum/src/github/vimalk78/process-tracker/bpf/process_tracker.bpf.c -o /home/vimalkum/src/github/vimalk78/process-tracker/processtracker_bpfel.o -fno-ident -fdebug-prefix-map=/home/vimalkum/src/github/vimalk78/process-tracker/bpf=bpf -fdebug-compilation-dir . -g "-D__BPF_TARGET_MISSING=\"GCC error \\\"The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb\\\"\""
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/home/vimalkum/src/github/vimalk78/process-tracker/bpf/process_tracker.bpf.c'.
4.      Running pass 'CodeGen Prepare' on function '@handle_fork'
 #0 0x00007f808b417b7a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.19.1+0x217b7a)
 #1 0x00007f808b414b24 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.19.1+0x214b24)
 #2 0x00007f808b333714 (/lib64/libLLVM.so.19.1+0x133714)
 #3 0x00007f808ac27050 __restore_rt (/lib64/libc.so.6+0x1a050)
 #4 0x00007f808bb8b574 llvm::MVT::getVT(llvm::Type*, bool) (/lib64/libLLVM.so.19.1+0x98b574)
 #5 0x00007f808bb8aa7f llvm::EVT::getEVT(llvm::Type*, bool) (/lib64/libLLVM.so.19.1+0x98aa7f)
 #6 0x00007f808b6d5d79 (/lib64/libLLVM.so.19.1+0x4d5d79)
 #7 0x00007f808b74ba52 (/lib64/libLLVM.so.19.1+0x54ba52)
 #8 0x00007f808b741166 (/lib64/libLLVM.so.19.1+0x541166)
 #9 0x00007f808b73ea3b (/lib64/libLLVM.so.19.1+0x53ea3b)
#10 0x00007f808b5bf590 llvm::FPPassManager::runOnFunction(llvm::Function&) (/lib64/libLLVM.so.19.1+0x3bf590)
#11 0x00007f808b5c78c3 llvm::FPPassManager::runOnModule(llvm::Module&) (/lib64/libLLVM.so.19.1+0x3c78c3)
#12 0x00007f808b5bfe22 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/lib64/libLLVM.so.19.1+0x3bfe22)
#13 0x00007f809468dc40 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/lib64/libclang-cpp.so.19.1+0x188dc40)
#14 0x00007f8094aff129 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.19.1+0x1cff129)
#15 0x00007f8092ff5579 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.19.1+0x1f5579)
#16 0x00007f809582a36b clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.19.1+0x2a2a36b)
#17 0x00007f8095786814 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.19.1+0x2986814)
#18 0x00007f80958b5abd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.19.1+0x2ab5abd)
#19 0x0000555d46a20516 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-19+0xb516)
#20 0x0000555d46a1c7b7 (/usr/bin/clang-19+0x77b7)
#21 0x00007f809535842d (/lib64/libclang-cpp.so.19.1+0x255842d)
#22 0x00007f808b333400 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib64/libLLVM.so.19.1+0x133400)
#23 0x00007f8095357ca5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/lib64/libclang-cpp.so.19.1+0x2557ca5)
#24 0x00007f809531281d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/lib64/libclang-cpp.so.19.1+0x251281d)
#25 0x00007f8095312b87 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/lib64/libclang-cpp.so.19.1+0x2512b87)
#26 0x00007f8095335c6b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/lib64/libclang-cpp.so.19.1+0x2535c6b)
#27 0x0000555d46a1bb7c clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-19+0x6b7c)
#28 0x0000555d46a2c8a8 main (/usr/bin/clang-19+0x178a8)
#29 0x00007f808ac10248 __libc_start_call_main (/lib64/libc.so.6+0x3248)
#30 0x00007f808ac1030b __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x330b)
#31 0x0000555d46a199f5 _start (/usr/bin/clang-19+0x49f5)
clang: error: clang frontend command failed with exit code 132 (use -v to see invocation)
clang version 19.1.7 (Fedora 19.1.7-2.fc41)
Target: bpfel
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg:
********************
```


## Comments

### Comment 1 - EugeneZelenko

Could you please try 20 or `main` branch? https://godbolt.org should be helpful.

---

