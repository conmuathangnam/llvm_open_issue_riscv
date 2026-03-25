# [HLSL] Assert being thrown when all resources are unused in lib_6_7 shader

**Author:** joaosaffran
**URL:** https://github.com/llvm/llvm-project/issues/178993
**Status:** Closed
**Labels:** crash, backend:DirectX, HLSL
**Closed Date:** 2026-03-17T19:06:10Z

## Body

When compiling a library shader and all resources are unused, we see an assert being hit. 

Repro link: https://hlsl.godbolt.org/z/dKrhMofnK

Crash stack: 
```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = llvm::ConstantArray; From = const llvm::Constant]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -E CSMain -T lib_6_7 -enable-16bit-types -O3 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'DXIL Prepare Module' on module '<source>'.
 #0 0x0000000004325418 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4325418)
 #1 0x0000000004322874 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4322874)
 #2 0x0000000004263f48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000785534442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007855344969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000785534442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007855344287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078553442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000785534439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000005284ece llvm::dxil::PointerTypeAnalysis::run(llvm::Module const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5284ece)
#10 0x0000000002f83464 (anonymous namespace)::DXILPrepareModule::runOnModule(llvm::Module&) DXILPrepare.cpp:0:0
#11 0x0000000003c0252f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c0252f)
#12 0x00000000045e97fc (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#13 0x00000000045ea0a1 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea0a1)
#14 0x0000000004c97347 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c97347)
#15 0x0000000006a9b14c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a9b14c)
#16 0x0000000004c977a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c977a8)
#17 0x0000000004fb6f6d clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4fb6f6d)
#18 0x0000000004f939c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f939c5)
#19 0x0000000004f133ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f133ee)
#20 0x000000000508ec5d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x508ec5d)
#21 0x0000000000de06ae cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde06ae)
#22 0x0000000000dd707a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#23 0x0000000000dd71fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#24 0x0000000004d0b209 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x00000000042643e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42643e4)
#26 0x0000000004d0b81f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x0000000004ccc1f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ccc1f2)
#28 0x0000000004ccd19e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ccd19e)
#29 0x0000000004cd45e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd45e5)
#30 0x0000000000ddca21 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddca21)
#31 0x0000000000c98cf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc98cf4)
#32 0x0000785534429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x0000785534429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000dd6b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd6b15)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

