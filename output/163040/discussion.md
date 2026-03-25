# [clang] -fsanitize=address causes Assertion `isa<Function>(Callee) || isa<GlobalAlias>(Callee)' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/163040
**Status:** Open
**Labels:** compiler-rt:asan, crash

## Body

Reproducer:
https://godbolt.org/z/rTx6o7dKc
```cpp
float __cxa_begin_catch;

struct S {
  S();
  S(const S &);
  ~S();
};

void foo() {
  try {
    throw S();
  } catch (S s) {
  }
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/Analysis/StackSafetyAnalysis.cpp:536: void {anonymous}::StackSafetyLocalAnalysis::analyzeAllUses(llvm::Value*, {anonymous}::UseInfo<llvm::GlobalValue>&, const llvm::StackLifetime&): Assertion `isa<Function>(Callee) || isa<GlobalAlias>(Callee)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=address <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "asan<use-after-scope>" on module "<source>"
 #0 0x00000000041e9638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9638)
 #1 0x00000000041e6a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e6a64)
 #2 0x000000000412b3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000789dfe042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000789dfe0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000789dfe042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000789dfe0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000789dfe02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000789dfe039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000031c28f1 (anonymous namespace)::StackSafetyLocalAnalysis::analyzeAllUses(llvm::Value*, (anonymous namespace)::UseInfo<llvm::GlobalValue>&, llvm::StackLifetime const&) StackSafetyAnalysis.cpp:0:0
#10 0x00000000031c303d (anonymous namespace)::StackSafetyLocalAnalysis::run() StackSafetyAnalysis.cpp:0:0
#11 0x00000000031c3560 llvm::StackSafetyInfo::getInfo() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x31c3560)
#12 0x00000000031c6e6f llvm::StackSafetyGlobalInfo::getInfo() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x31c6e6f)
#13 0x00000000031c8d79 llvm::StackSafetyGlobalInfo::isSafe(llvm::AllocaInst const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x31c8d79)
#14 0x0000000003e53ddc (anonymous namespace)::AddressSanitizer::isInterestingAlloca(llvm::AllocaInst const&) AddressSanitizer.cpp:0:0
#15 0x0000000003e59ddc (anonymous namespace)::FunctionStackPoisoner::runOnFunction() AddressSanitizer.cpp:0:0
#16 0x0000000003e5ca87 (anonymous namespace)::AddressSanitizer::instrumentFunction(llvm::Function&, llvm::TargetLibraryInfo const*, llvm::TargetTransformInfo const*) AddressSanitizer.cpp:0:0
#17 0x0000000003e5eac0 llvm::AddressSanitizerPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5eac0)
#18 0x000000000449265e llvm::detail::PassModel<llvm::Module, llvm::AddressSanitizerPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x449265e)
#19 0x0000000003b43701 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b43701)
#20 0x00000000044a08ab (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#21 0x00000000044a3dfb clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a3dfb)
#22 0x0000000004b2e247 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e247)
#23 0x00000000068c62bc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c62bc)
#24 0x0000000004b2e6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e6a8)
#25 0x0000000004e1a3a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1a3a5)
#26 0x0000000004d9bade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9bade)
#27 0x0000000004f116bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f116bd)
#28 0x0000000000dbfa50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfa50)
#29 0x0000000000db65ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#30 0x0000000000db673d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#31 0x0000000004b97839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x000000000412b854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412b854)
#33 0x0000000004b97e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x0000000004b59162 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b59162)
#35 0x0000000004b5a10e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5a10e)
#36 0x0000000004b61845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b61845)
#37 0x0000000000dbbf79 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbf79)
#38 0x0000000000c6bf74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bf74)
#39 0x0000789dfe029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x0000789dfe029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000db6055 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6055)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-18.1.0:
https://godbolt.org/z/nT8sY9ejc

---

### Comment 2 - BStott6

It seems like the root issue here is not the crash with ASan but that Clang lets this code through in the first place. `__cxa_begin_catch` is an exception handling routine in the Itanium C++ ABI (https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html) with the signature `void *__cxa_begin_catch(void *)`. The compiler should not allow this name to be redefined as a `float`. GCC rejects this code for this exact reason; I think Clang should too.

---

