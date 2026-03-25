# Clang crashes in `globalopt` pass when using `target_clones` attribute after inline function definition

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/130515

## Body

Compiler Explorer: [https://godbolt.org/z/PfcEGY1oG](https://godbolt.org/z/PfcEGY1oG)

(Found by fuzzer)

**Source Code:**

```c
inline int foo(void) { return 0; }
extern inline int __attribute__((target_clones("default,avx"))) foo(void);
```

**Crash Traceback:**
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "globalopt" on module "<source>"
 #0 0x0000000003b583f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b583f8)
 #1 0x0000000003b5653c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b5653c)
 #2 0x0000000003aa58c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075186bc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000003489991 llvm::Instruction::mayHaveSideEffects() const (/opt/compiler-explorer/clang-trunk/bin/clang+0x3489991)
 #5 0x000000000534c251 llvm::GlobalOptPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x534c251)
 #6 0x00000000051ac2de llvm::detail::PassModel<llvm::Module, llvm::GlobalOptPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x51ac2de)
 #7 0x000000000351daaf llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x351daaf)
 #8 0x0000000003dfc594 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
 #9 0x0000000003dff74e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3dff74e)
#10 0x00000000044d3775 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x44d3775)
#11 0x00000000060471ec clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x60471ec)
#12 0x00000000044d3e95 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x44d3e95)
#13 0x00000000047ae771 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x47ae771)
#14 0x000000000472f3ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x472f3ab)
#15 0x000000000489bbf3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x489bbf3)
#16 0x0000000000d668a1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xd668a1)
#17 0x0000000000d5f00d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#18 0x0000000004520499 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#19 0x0000000003aa5cf3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3aa5cf3)
#20 0x00000000045206b9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#21 0x00000000044e441d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x44e441d)
#22 0x00000000044e53b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x44e53b1)
#23 0x00000000044ef76c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x44ef76c)
#24 0x0000000000d634c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xd634c1)
#25 0x0000000000c24d54 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc24d54)
#26 0x000075186bc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#27 0x000075186bc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#28 0x0000000000d5eaa5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xd5eaa5)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

