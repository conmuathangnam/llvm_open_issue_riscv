# [SPIR-V] Unexpected opcode when compiling program with `select` in HLSL

**Author:** kmpeng
**URL:** https://github.com/llvm/llvm-project/issues/166642
**Status:** Closed
**Labels:** crash, HLSL, backend:SPIR-V
**Closed Date:** 2025-12-05T19:56:39Z

## Body

https://godbolt.org/z/fePfa5Ybx

Code that reproduces the error:
```hlsl
// RUN: %clang-dxc -spirv -fspv-target-env=vulkan1.3 -T cs_6_5 %s
StructuredBuffer<bool4> Cond : register(t0);
StructuredBuffer<float4> TrueVal : register(t2);
StructuredBuffer<float4> FalseVal : register(t3);

RWStructuredBuffer<float4> Out : register(u4);

[numthreads(1,1,1)]
void main() {
  Out[0] = select(Cond[0], TrueVal[0].x, FalseVal[0].x);
}
```
The crash is happening in SPIRVPreLegalizer::generateAssignInstrs()
https://github.com/llvm/llvm-project/blob/9e2f73fe9052a4fbf382a06e30b2441c6d99fb7e/llvm/lib/Target/SPIRV/SPIRVPreLegalizer.cpp#L647

The same error occurs for all types (half, int16_t, uint16_t, float, int, uint, bool, double, int64_t, uint64_t).

Crash stack:
```
Unexpected opcode
UNREACHABLE executed at /home/kaitlinpeng/llvm-project/llvm/lib/Target/SPIRV/SPIRVPreLegalizer.cpp:647!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ../llvm-build-debug/bin/clang-dxc -spirv -fspv-target-env=vulkan1.3 -T cs_6_5 clang/test/CodeGenHLSL/builtins/TestFile.hlsl
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'clang/test/CodeGenHLSL/builtins/TestFile.hlsl'.
4.      Running pass 'SPIRV pre legalizer' on function '@main'
 #0 0x00005a1960835321 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/kaitlinpeng/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00005a196083589b PrintStackTraceSignalHandler(void*) /home/kaitlinpeng/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005a19608335b6 llvm::sys::RunSignalHandlers() /home/kaitlinpeng/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005a1960834b9e llvm::sys::CleanupOnSignal(unsigned long) /home/kaitlinpeng/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
 #4 0x00005a1960754087 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/kaitlinpeng/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #5 0x00005a1960754475 CrashRecoverySignalHandler(int) /home/kaitlinpeng/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #6 0x0000753147645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #7 0x000075314769eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #8 0x000075314764527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #9 0x00007531476288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
#10 0x00005a1960761e30 llvm::install_out_of_memory_new_handler() /home/kaitlinpeng/llvm-project/llvm/lib/Support/ErrorHandling.cpp:225:0
#11 0x00005a195e52fb3b generateAssignInstrs(llvm::MachineFunction&, llvm::SPIRVGlobalRegistry*, llvm::MachineIRBuilder, llvm::DenseMap<llvm::MachineInstr*, llvm::Type*, llvm::DenseMapInfo<llvm::MachineInstr*, void>, llvm::detail::DenseMapPair<llvm::MachineInstr*, llvm::Type*>>&) /home/kaitlinpeng/llvm-project/llvm/lib/Target/SPIRV/SPIRVPreLegalizer.cpp:648:13
#12 0x00005a195e52d61c (anonymous namespace)::SPIRVPreLegalizer::runOnMachineFunction(llvm::MachineFunction&) /home/kaitlinpeng/llvm-project/llvm/lib/Target/SPIRV/SPIRVPreLegalizer.cpp:1089:3
#13 0x00005a195f5fbcdb llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /home/kaitlinpeng/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:8
#14 0x00005a195ff7949a llvm::FPPassManager::runOnFunction(llvm::Function&) /home/kaitlinpeng/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#15 0x00005a195ff7de82 llvm::FPPassManager::runOnModule(llvm::Module&) /home/kaitlinpeng/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#16 0x00005a195ff79d4b (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/kaitlinpeng/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#17 0x00005a195ff798ca llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/kaitlinpeng/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#18 0x00005a195ff7e161 llvm::legacy::PassManager::run(llvm::Module&) /home/kaitlinpeng/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#19 0x00005a1961710aa4 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /home/kaitlinpeng/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1288:19
#20 0x00005a196170a597 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/kaitlinpeng/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1314:7
#21 0x00005a1961709a18 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/kaitlinpeng/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1482:3
#22 0x00005a1961732096 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/kaitlinpeng/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#23 0x00005a19645ce828 clang::ParseAST(clang::Sema&, bool, bool) /home/kaitlinpeng/llvm-project/clang/lib/Parse/ParseAST.cpp:190:12
#24 0x00005a1961f2c653 clang::ASTFrontendAction::ExecuteAction() /home/kaitlinpeng/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1434:1
#25 0x00005a1961736000 clang::CodeGenAction::ExecuteAction() /home/kaitlinpeng/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1110:5
#26 0x00005a1961f2c8a2 clang::WrapperFrontendAction::ExecuteAction() /home/kaitlinpeng/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1465:1
#27 0x00005a1961f8771e clang::HLSLFrontendAction::ExecuteAction() /home/kaitlinpeng/llvm-project/clang/lib/Frontend/FrontendActions.cpp:1336:35
#28 0x00005a1961f2bfc6 clang::FrontendAction::Execute() /home/kaitlinpeng/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:7
#29 0x00005a1961e3206e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/kaitlinpeng/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1003:23
#30 0x00005a196213e1bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/kaitlinpeng/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:8
#31 0x00005a195e4427ab cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/kaitlinpeng/llvm-project/clang/tools/driver/cc1_main.cpp:300:13
#32 0x00005a195e4358fc ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/kaitlinpeng/llvm-project/clang/tools/driver/driver.cpp:225:5
#33 0x00005a195e436451 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /home/kaitlinpeng/llvm-project/clang/tools/driver/driver.cpp:369:12
#34 0x00005a195e4363fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /home/kaitlinpeng/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#35 0x00005a1961c87ec1 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /home/kaitlinpeng/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#36 0x00005a1961c84e28 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /home/kaitlinpeng/llvm-project/clang/lib/Driver/Job.cpp:437:34
#37 0x00005a1961c84df5 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /home/kaitlinpeng/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#38 0x00005a195f2ff079 llvm::function_ref<void ()>::operator()() const /home/kaitlinpeng/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#39 0x00005a1960753e7a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/kaitlinpeng/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#40 0x00005a1961c8475b clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/kaitlinpeng/llvm-project/clang/lib/Driver/Job.cpp:437:7
#41 0x00005a1961c16403 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/kaitlinpeng/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#42 0x00005a1961c16614 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/kaitlinpeng/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#43 0x00005a1961c32282 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/kaitlinpeng/llvm-project/clang/lib/Driver/Driver.cpp:2253:7
#44 0x00005a195e4353cd clang_main(int, char**, llvm::ToolContext const&) /home/kaitlinpeng/llvm-project/clang/tools/driver/driver.cpp:407:9
#45 0x00005a195e4694c5 main /home/kaitlinpeng/llvm-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#46 0x000075314762a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#47 0x000075314762a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#48 0x00005a195e4340a5 _start (../llvm-build-debug/bin/clang-dxc+0x5d680a5)
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 5f1b9023a8093fd8beb931a74d28753fbda88fdf)
Target: spirv1.6-unknown-vulkan1.3-compute
Thread model: posix
InstalledDir: /home/kaitlinpeng/llvm-build-debug/bin
Build config: +unoptimized, +assertions
```

## Comments

### Comment 1 - bogner

This is fixed by #169696

---

