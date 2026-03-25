# [HLSL][SPIRV] `Basic/Matrix/matrix_elementwise_cast.test` failling on Windows Vulkan Intel Clang build

**Author:** joaosaffran
**URL:** https://github.com/llvm/llvm-project/issues/185518
**Status:** Closed
**Labels:** clang:codegen, crash, HLSL
**Closed Date:** 2026-03-13T16:10:29Z

## Body

The test `Basic/Matrix/matrix_elementwise_cast.test` is failing when building `Windows Vulkan Intel Clang`. The issue seems to be introduced after this commit: https://github.com/llvm/llvm-project/commit/ae363d50ad292b4d8c33bf795ad16c79eb640b19.patch on Mar 2nd 2026 12:46:40 PM EST.

Stack trace:

```
# .---command stderr------------
# | fatal error: error in backend: unable to legalize instruction: %68:vid(<6 x s32>) = G_FPTOSI %62:vfid(<6 x s32>) (in function: main)
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
# | Stack dump:
# | 0.	Program arguments: e:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\bin\\clang-dxc.exe -spirv -fspv-target-env=vulkan1.3 -fspv-extension=DXC --dxv-path=E:/actions-runner/_work/offload-test-suite/offload-test-suite/DXC/build/bin -T cs_6_0 -Fo E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Basic\\Matrix\\Output\\matrix_elementwise_cast.test.tmp.o E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Basic\\Matrix\\Output\\matrix_elementwise_cast.test.tmp/source.hlsl
# | 1.	<eof> parser at end of file
# | 2.	Code generation
# | 3.	Running pass 'Function Pass Manager' on module 'E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-vk\Basic\Matrix\Output\matrix_elementwise_cast.test.tmp/source.hlsl'.
# | 4.	Running pass 'Legalizer' on function '@main'
# | Exception Code: 0xE0000001
# |  #0 0x00007ffaef15a80a (C:\WINDOWS\System32\KERNELBASE.dll+0xca80a)
# |  #1 0x00007ff73b9ae65a llvm::CrashRecoveryContext::HandleExit(int) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:457:0
# |  #2 0x00007ff73b9af2e9 llvm::sys::Process::Exit(int, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\Process.cpp:117:0
# |  #3 0x00007ff73b498f11 LLVMErrorHandler E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\cc1_main.cpp:82:0
# |  #4 0x00007ff73b9fe042 std::_String_val<std::_Simple_types<char> >::_Large_mode_engaged C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:453:0
# |  #5 0x00007ff73b9fe042 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy_deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:3083:0
# |  #6 0x00007ff73b9fe042 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::~basic_string C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:1383:0
# |  #7 0x00007ff73b9fe042 llvm::report_fatal_error(class llvm::Twine const &, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\ErrorHandling.cpp:117:0
# |  #8 0x00007ff73b9fe24b llvm::reportFatalUsageError(class llvm::Twine const &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\ErrorHandling.cpp:156:0
# |  #9 0x00007ff73d407ec0 std::_Adjust_manually_vector_aligned C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:219:0
# | #10 0x00007ff73d407ec0 std::_Deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:286:0
# | #11 0x00007ff73d407ec0 std::allocator<char>::deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:985:0
# | #12 0x00007ff73d407ec0 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Deallocate_for_capacity C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:852:0
# | #13 0x00007ff73d407ec0 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::_Tidy_deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:3086:0
# | #14 0x00007ff73d407ec0 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::~basic_string C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:1383:0
# | #15 0x00007ff73d407ec0 reportGISelDiagnostic E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Utils.cpp:244:0
# | #16 0x00007ff73d4081f1 llvm::DiagnosticInfoOptimizationBase::~DiagnosticInfoOptimizationBase E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\DiagnosticInfo.h:521:0
# | #17 0x00007ff73d4081f1 llvm::reportGISelFailure(class llvm::MachineFunction &, class llvm::MachineOptimizationRemarkEmitter &, char const *, class llvm::StringRef, class llvm::MachineInstr const &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Utils.cpp:277:0
# | #18 0x00007ff73be27ad9 llvm::Legalizer::runOnMachineFunction(class llvm::MachineFunction &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Legalizer.cpp:358:0
# | #19 0x00007ff73bd44ec5 llvm::MachineFunctionPass::runOnFunction(class llvm::Function &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\MachineFunctionPass.cpp:112:0
# | #20 0x00007ff73b72a102 llvm::FPPassManager::runOnFunction(class llvm::Function &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1398:0
# | #21 0x00007ff73b7316bd llvm::FPPassManager::runOnModule(class llvm::Module &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1444:0
# | #22 0x00007ff73b72a97f `anonymous namespace'::MPPassManager::runOnModule E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1513:0
# | #23 0x00007ff73b72a58e llvm::legacy::PassManagerImpl::run(class llvm::Module &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:531:0
# | #24 0x00007ff73cb3a4cf `anonymous namespace'::EmitAssemblyHelper::RunCodegenPipeline E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1278:0
# | #25 0x00007ff73cb31258 std::unique_ptr<llvm::ToolOutputFile,std::default_delete<llvm::ToolOutputFile> >::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #26 0x00007ff73cb31258 `anonymous namespace'::EmitAssemblyHelper::emitAssembly E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1303:0
# | #27 0x00007ff73cb2fedc std::unique_ptr<llvm::TargetMachine,std::default_delete<llvm::TargetMachine> >::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #28 0x00007ff73cb2fedc clang::emitBackendOutput(class clang::CompilerInstance &, class clang::CodeGenOptions &, class llvm::StringRef, class llvm::Module *, enum clang::BackendAction, class llvm::IntrusiveRefCntPtr<class llvm::vfs::FileSystem>, class std::unique_ptr<class llvm::raw_pwrite_stream, struct std::default_delete<class llvm::raw_pwrite_stream>>, class clang::BackendConsumer *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1479:0
# | #29 0x00007ff73d13b89e std::unique_ptr<llvm::ToolOutputFile,std::default_delete<llvm::ToolOutputFile> >::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #30 0x00007ff73d13b89e llvm::LLVMRemarkFileHandle::operator bool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\LLVMRemarkStreamer.h:124:0
# | #31 0x00007ff73d13b89e clang::BackendConsumer::HandleTranslationUnit(class clang::ASTContext &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:316:0
# | #32 0x00007ff73f3a6007 clang::ParseAST(class clang::Sema &, bool, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Parse\ParseAST.cpp:183:0
# | #33 0x00007ff73d0f5356 clang::HLSLFrontendAction::ExecuteAction(void) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendActions.cpp:1330:0
# | #34 0x00007ff73d084271 clang::FrontendAction::Execute(void) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1327:0
# | #35 0x00007ff73bb195db llvm::Error::getPtr E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:277:0
# | #36 0x00007ff73bb195db llvm::Error::operator bool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:241:0
# | #37 0x00007ff73bb195db clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1002:0
# | #38 0x00007ff73bbd3959 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:312:0
# | #39 0x00007ff73b497629 cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\cc1_main.cpp:302:0
# | #40 0x00007ff73b4932d0 ExecuteCC1Tool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:229:0
# | #41 0x00007ff73b496d98 clang_main::<lambda_0>::operator() E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:379:0
# | #42 0x00007ff73b496d98 llvm::function_ref<int (llvm::SmallVectorImpl<const char *> &)>::callback_fn<`lambda at E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:377:32'> E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
# | #43 0x00007ff73ce43a8d clang::driver::CC1Command::Execute::<lambda_1>::operator() E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:0
# | #44 0x00007ff73ce43a8d llvm::function_ref<void ()>::callback_fn<`lambda at E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:22'> E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
# | #45 0x00007ff73b9ae505 llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref<(void)>) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:237:0
# | #46 0x00007ff73ce42d7b clang::driver::CC1Command::Execute(class llvm::ArrayRef<class std::optional<class llvm::StringRef>>, class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> *, bool *) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:0
# | #47 0x00007ff73bafd8b2 clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &, class clang::driver::Command const *&, bool) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Compilation.cpp:196:0
# | #48 0x00007ff73bafdb3d clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &, bool) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Compilation.cpp:246:0
# | #49 0x00007ff73bad6bcc llvm::SmallVectorBase<unsigned int>::empty E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\SmallVector.h:83:0
# | #50 0x00007ff73bad6bcc clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Driver.cpp:2330:0
# | #51 0x00007ff73b4926bc clang_main(int, char **, struct llvm::ToolContext const &) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:419:0
# | #52 0x00007ff73b4a802c main E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\clang\tools\driver\clang-driver.cpp:17:0
# | #53 0x00007ff741a6f1f4 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
# | #54 0x00007ff741a6f1f4 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #55 0x00007ffaefdde8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
# | #56 0x00007ffaf18cc53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
# `-----------------------------
```

## Comments

### Comment 1 - Desel72

Hi @joaosaffran I'd love to work on this. Can I pick this up please?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (joaosaffran)

<details>
The test `Basic/Matrix/matrix_elementwise_cast.test` is failing when building `Windows Vulkan Intel Clang`. The issue seems to be introduced after this commit: https://github.com/llvm/llvm-project/commit/ae363d50ad292b4d8c33bf795ad16c79eb640b19.patch on Mar 2nd 2026 12:46:40 PM EST.

Stack trace:

```
# .---command stderr------------
# | fatal error: error in backend: unable to legalize instruction: %68:vid(&lt;6 x s32&gt;) = G_FPTOSI %62:vfid(&lt;6 x s32&gt;) (in function: main)
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
# | Stack dump:
# | 0.	Program arguments: e:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\bin\\clang-dxc.exe -spirv -fspv-target-env=vulkan1.3 -fspv-extension=DXC --dxv-path=E:/actions-runner/_work/offload-test-suite/offload-test-suite/DXC/build/bin -T cs_6_0 -Fo E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Basic\\Matrix\\Output\\matrix_elementwise_cast.test.tmp.o E:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-vk\\Basic\\Matrix\\Output\\matrix_elementwise_cast.test.tmp/source.hlsl
# | 1.	&lt;eof&gt; parser at end of file
# | 2.	Code generation
# | 3.	Running pass 'Function Pass Manager' on module 'E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-vk\Basic\Matrix\Output\matrix_elementwise_cast.test.tmp/source.hlsl'.
# | 4.	Running pass 'Legalizer' on function '@<!-- -->main'
# | Exception Code: 0xE0000001
# |  #<!-- -->0 0x00007ffaef15a80a (C:\WINDOWS\System32\KERNELBASE.dll+0xca80a)
# |  #<!-- -->1 0x00007ff73b9ae65a llvm::CrashRecoveryContext::HandleExit(int) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:457:0
# |  #<!-- -->2 0x00007ff73b9af2e9 llvm::sys::Process::Exit(int, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\Process.cpp:117:0
# |  #<!-- -->3 0x00007ff73b498f11 LLVMErrorHandler E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\cc1_main.cpp:82:0
# |  #<!-- -->4 0x00007ff73b9fe042 std::_String_val&lt;std::_Simple_types&lt;char&gt; &gt;::_Large_mode_engaged C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:453:0
# |  #<!-- -->5 0x00007ff73b9fe042 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::_Tidy_deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:3083:0
# |  #<!-- -->6 0x00007ff73b9fe042 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::~basic_string C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:1383:0
# |  #<!-- -->7 0x00007ff73b9fe042 llvm::report_fatal_error(class llvm::Twine const &amp;, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\ErrorHandling.cpp:117:0
# |  #<!-- -->8 0x00007ff73b9fe24b llvm::reportFatalUsageError(class llvm::Twine const &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\ErrorHandling.cpp:156:0
# |  #<!-- -->9 0x00007ff73d407ec0 std::_Adjust_manually_vector_aligned C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:219:0
# | #<!-- -->10 0x00007ff73d407ec0 std::_Deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:286:0
# | #<!-- -->11 0x00007ff73d407ec0 std::allocator&lt;char&gt;::deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xmemory:985:0
# | #<!-- -->12 0x00007ff73d407ec0 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::_Deallocate_for_capacity C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:852:0
# | #<!-- -->13 0x00007ff73d407ec0 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::_Tidy_deallocate C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:3086:0
# | #<!-- -->14 0x00007ff73d407ec0 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::~basic_string C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\xstring:1383:0
# | #<!-- -->15 0x00007ff73d407ec0 reportGISelDiagnostic E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Utils.cpp:244:0
# | #<!-- -->16 0x00007ff73d4081f1 llvm::DiagnosticInfoOptimizationBase::~DiagnosticInfoOptimizationBase E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\DiagnosticInfo.h:521:0
# | #<!-- -->17 0x00007ff73d4081f1 llvm::reportGISelFailure(class llvm::MachineFunction &amp;, class llvm::MachineOptimizationRemarkEmitter &amp;, char const *, class llvm::StringRef, class llvm::MachineInstr const &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Utils.cpp:277:0
# | #<!-- -->18 0x00007ff73be27ad9 llvm::Legalizer::runOnMachineFunction(class llvm::MachineFunction &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\GlobalISel\Legalizer.cpp:358:0
# | #<!-- -->19 0x00007ff73bd44ec5 llvm::MachineFunctionPass::runOnFunction(class llvm::Function &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\CodeGen\MachineFunctionPass.cpp:112:0
# | #<!-- -->20 0x00007ff73b72a102 llvm::FPPassManager::runOnFunction(class llvm::Function &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1398:0
# | #<!-- -->21 0x00007ff73b7316bd llvm::FPPassManager::runOnModule(class llvm::Module &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1444:0
# | #<!-- -->22 0x00007ff73b72a97f `anonymous namespace'::MPPassManager::runOnModule E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1513:0
# | #<!-- -->23 0x00007ff73b72a58e llvm::legacy::PassManagerImpl::run(class llvm::Module &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:531:0
# | #<!-- -->24 0x00007ff73cb3a4cf `anonymous namespace'::EmitAssemblyHelper::RunCodegenPipeline E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1278:0
# | #<!-- -->25 0x00007ff73cb31258 std::unique_ptr&lt;llvm::ToolOutputFile,std::default_delete&lt;llvm::ToolOutputFile&gt; &gt;::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #<!-- -->26 0x00007ff73cb31258 `anonymous namespace'::EmitAssemblyHelper::emitAssembly E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1303:0
# | #<!-- -->27 0x00007ff73cb2fedc std::unique_ptr&lt;llvm::TargetMachine,std::default_delete&lt;llvm::TargetMachine&gt; &gt;::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #<!-- -->28 0x00007ff73cb2fedc clang::emitBackendOutput(class clang::CompilerInstance &amp;, class clang::CodeGenOptions &amp;, class llvm::StringRef, class llvm::Module *, enum clang::BackendAction, class llvm::IntrusiveRefCntPtr&lt;class llvm::vfs::FileSystem&gt;, class std::unique_ptr&lt;class llvm::raw_pwrite_stream, struct std::default_delete&lt;class llvm::raw_pwrite_stream&gt;&gt;, class clang::BackendConsumer *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1479:0
# | #<!-- -->29 0x00007ff73d13b89e std::unique_ptr&lt;llvm::ToolOutputFile,std::default_delete&lt;llvm::ToolOutputFile&gt; &gt;::operator bool C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | #<!-- -->30 0x00007ff73d13b89e llvm::LLVMRemarkFileHandle::operator bool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\LLVMRemarkStreamer.h:124:0
# | #<!-- -->31 0x00007ff73d13b89e clang::BackendConsumer::HandleTranslationUnit(class clang::ASTContext &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:316:0
# | #<!-- -->32 0x00007ff73f3a6007 clang::ParseAST(class clang::Sema &amp;, bool, bool) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Parse\ParseAST.cpp:183:0
# | #<!-- -->33 0x00007ff73d0f5356 clang::HLSLFrontendAction::ExecuteAction(void) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendActions.cpp:1330:0
# | #<!-- -->34 0x00007ff73d084271 clang::FrontendAction::Execute(void) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1327:0
# | #<!-- -->35 0x00007ff73bb195db llvm::Error::getPtr E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:277:0
# | #<!-- -->36 0x00007ff73bb195db llvm::Error::operator bool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:241:0
# | #<!-- -->37 0x00007ff73bb195db clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1002:0
# | #<!-- -->38 0x00007ff73bbd3959 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:312:0
# | #<!-- -->39 0x00007ff73b497629 cc1_main(class llvm::ArrayRef&lt;char const *&gt;, char const *, void *) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\cc1_main.cpp:302:0
# | #<!-- -->40 0x00007ff73b4932d0 ExecuteCC1Tool E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:229:0
# | #<!-- -->41 0x00007ff73b496d98 clang_main::&lt;lambda_0&gt;::operator() E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:379:0
# | #<!-- -->42 0x00007ff73b496d98 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;const char *&gt; &amp;)&gt;::callback_fn&lt;`lambda at E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:377:32'&gt; E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
# | #<!-- -->43 0x00007ff73ce43a8d clang::driver::CC1Command::Execute::&lt;lambda_1&gt;::operator() E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:0
# | #<!-- -->44 0x00007ff73ce43a8d llvm::function_ref&lt;void ()&gt;::callback_fn&lt;`lambda at E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:22'&gt; E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
# | #<!-- -->45 0x00007ff73b9ae505 llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref&lt;(void)&gt;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:237:0
# | #<!-- -->46 0x00007ff73ce42d7b clang::driver::CC1Command::Execute(class llvm::ArrayRef&lt;class std::optional&lt;class llvm::StringRef&gt;&gt;, class std::basic_string&lt;char, struct std::char_traits&lt;char&gt;, class std::allocator&lt;char&gt;&gt; *, bool *) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Job.cpp:442:0
# | #<!-- -->47 0x00007ff73bafd8b2 clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &amp;, class clang::driver::Command const *&amp;, bool) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Compilation.cpp:196:0
# | #<!-- -->48 0x00007ff73bafdb3d clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;, bool) const E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Compilation.cpp:246:0
# | #<!-- -->49 0x00007ff73bad6bcc llvm::SmallVectorBase&lt;unsigned int&gt;::empty E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\SmallVector.h:83:0
# | #<!-- -->50 0x00007ff73bad6bcc clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Driver\Driver.cpp:2330:0
# | #<!-- -->51 0x00007ff73b4926bc clang_main(int, char **, struct llvm::ToolContext const &amp;) E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:419:0
# | #<!-- -->52 0x00007ff73b4a802c main E:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\clang\tools\driver\clang-driver.cpp:17:0
# | #<!-- -->53 0x00007ff741a6f1f4 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
# | #<!-- -->54 0x00007ff741a6f1f4 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #<!-- -->55 0x00007ffaefdde8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
# | #<!-- -->56 0x00007ffaf18cc53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
# `-----------------------------
```
</details>


---

