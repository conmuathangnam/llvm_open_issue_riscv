# [clang] crash with "invalid retained nodes, retained node does not belong to subprogram"

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/168251
**Status:** Closed
**Labels:** crash
**Closed Date:** 2025-11-21T02:58:10Z

## Body

Reproducer:
https://godbolt.org/z/hT6jaj1EE
```cpp
struct A {
  virtual A *f(int x, ...);
};
struct B {
  virtual B *f(int x, ...);
};
struct C : A, B {
  virtual C *f(int x, ...);
};
C *C::f(int x, ...) { return 0; }
```

Backtrace:
```console
invalid retained nodes, retained node does not belong to subprogram
!41 = distinct !DISubprogram(name: "f", linkageName: "_ZN1C1fEiz", scope: !8, file: !5, line: 10, type: !15, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, declaration: !14, retainedNodes: !32, keyInstructions: true)
!32 = !{!33, !34}
!33 = !DILocalVariable(name: "this", arg: 1, scope: !31, type: !17, flags: DIFlagArtificial | DIFlagObjectPointer)
!31 = distinct !DISubprogram(name: "f", linkageName: "_ZN1C1fEiz", scope: !8, file: !5, line: 10, type: !15, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, declaration: !14, retainedNodes: !32, keyInstructions: true)
fatal error: error in backend: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -g <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "verify" on module "<source>"
 #0 0x0000000004259918 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4259918)
 #1 0x0000000004256d44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4256d44)
 #2 0x000000000419b506 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x419b506)
 #3 0x000000000424e20e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424e20e)
 #4 0x0000000000dcb480 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x00000000041a6793 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41a6793)
 #6 0x00000000041a68f1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41a68f1)
 #7 0x0000000003bdcd18 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bdcd18)
 #8 0x000000000450576e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x450576e)
 #9 0x0000000003ba48c1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ba48c1)
#10 0x0000000004513c2b (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#11 0x00000000045170d3 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45170d3)
#12 0x0000000004ba3547 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba3547)
#13 0x000000000694ac0c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694ac0c)
#14 0x0000000004ba39a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba39a8)
#15 0x0000000004e91db5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e91db5)
#16 0x0000000004e11f7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e11f7e)
#17 0x0000000004f89d8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f89d8d)
#18 0x0000000000dcdce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcdce0)
#19 0x0000000000dc475a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#20 0x0000000000dc48dd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#21 0x0000000004c0d529 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x000000000419b444 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x419b444)
#23 0x0000000004c0db3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x0000000004bce892 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bce892)
#25 0x0000000004bcf83e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcf83e)
#26 0x0000000004bd6f95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd6f95)
#27 0x0000000000dca101 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca101)
#28 0x0000000000c79af4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79af4)
#29 0x0000766ecdc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x0000766ecdc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000dc41f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc41f5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - k-arrows

This is trunk only:
https://godbolt.org/z/j4MWGzvcT

---

### Comment 2 - k-arrows

It seems that this has been fixed. I will close this issue.

---

