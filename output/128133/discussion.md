# Assertion `SecondFPRegOp == ~0U && "More than two fp operands!"' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/128133
**Status:** Open
**Labels:** llvm:codegen, crash-on-invalid

## Body

Compiler Explorer: https://godbolt.org/z/evMT6aMqo

Console:
```console
<source>:4:55: error: SSE register return with SSE disabled
    4 | V __attribute__((target("no-sse"))) f(const V *ptr) { return *ptr; }
      |                                                       ^
<source>:4:55: error: SSE register return with SSE disabled
clang: /root/llvm-project/llvm/lib/Target/X86/X86FloatingPoint.cpp:1113: void {anonymous}::FPS::handleReturn(llvm::MachineBasicBlock::iterator&): Assertion `SecondFPRegOp == ~0U && "More than two fp operands!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 FP Stackifier' on function '@f'
 #0 0x0000000003e98398 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e98398)
 #1 0x0000000003e96054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e96054)
 #2 0x0000000003de2458 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000744e2d442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000744e2d4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000744e2d442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000744e2d4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000744e2d42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000744e2d439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000028fac30 (anonymous namespace)::FPS::handleSpecialFP(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>&) X86FloatingPoint.cpp:0:0
#10 0x00000000028fb30b (anonymous namespace)::FPS::processBasicBlock(llvm::MachineFunction&, llvm::MachineBasicBlock&) (.isra.0) X86FloatingPoint.cpp:0:0
#11 0x00000000028fd955 (anonymous namespace)::FPS::runOnMachineFunction(llvm::MachineFunction&) (.part.0) X86FloatingPoint.cpp:0:0
#12 0x00000000031d060a llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x00000000037e667f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37e667f)
#14 0x00000000037e6a31 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37e6a31)
#15 0x00000000037e72d1 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37e72d1)
#16 0x0000000004159870 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4159870)
#17 0x0000000004859d00 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4859d00)
#18 0x00000000064aa3dc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64aa3dc)
#19 0x000000000485a108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x485a108)
#20 0x0000000004b211e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b211e5)
#21 0x0000000004aa498e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4aa498e)
#22 0x0000000004c0fc3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c0fc3e)
#23 0x0000000000d5c7ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd5c7ef)
#24 0x0000000000d5426a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00000000048a0719 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003de2904 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3de2904)
#27 0x00000000048a0d0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004863e8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4863e8d)
#29 0x0000000004864f0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4864f0e)
#30 0x000000000486cec5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x486cec5)
#31 0x0000000000d595e3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd595e3)
#32 0x0000000000c2b854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2b854)
#33 0x0000744e2d429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x0000744e2d429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000d53d15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd53d15)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
typedef float V __attribute__((__vector_size__(16), __may_alias__));

V __attribute__((target("no-sse"))) f(const V *ptr) { return *ptr; }

V g(const V *ptr) { return *ptr; }
```

from clang 3.8: https://godbolt.org/z/Waa6r97PT

