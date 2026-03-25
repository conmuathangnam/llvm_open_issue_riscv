# [DebugInfo][RemoveDIs] Crash when building on Windows.

**Author:** tru
**URL:** https://github.com/llvm/llvm-project/issues/76062
**Status:** Closed
**Labels:** debuginfo, llvm:optimizations, crash
**Closed Date:** 2025-09-22T11:28:43Z

## Body

Built as requested in https://discourse.llvm.org/t/call-for-testing-non-instruction-debug-info/75522 on our game engine. It crashes on the first file we try to build.

This is a debug build of LLVM with assertions. It failed in release mode as well. Built from SHA d9803841f2d539f6a740c07ebd0f3cb176812dbf

Here is the stack-trace:

```
1.      <eof> parser at end of file
2.      Optimizer
Exception Code: 0xC0000005
 #0 0x00007ff792254da9 llvm::Instruction::getParent(void) C:\code\llvm\llvm-project\llvm\include\llvm\IR\Instruction.h:140:0
 #1 0x00007ff7935497ba llvm::DPValue::getParent(void) const C:\code\llvm\llvm-project\llvm\lib\IR\DebugProgramInstruction.cpp:229:0
 #2 0x00007ff795f6fa82 llvm::JumpThreadingPass::updateSSA::<lambda_4>::operator() C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1980:0
 #3 0x00007ff795f6f9d1 std::find_if<llvm::DPValue **,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'> C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.38.33130\include\xutility:6304:0
 #4 0x00007ff795f6f8a2 std::remove_if<llvm::DPValue **,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'> C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.38.33130\include\xmemory:2238:0
 #5 0x00007ff795f6f812 llvm::remove_if<llvm::SmallVector<llvm::DPValue *,4> &,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'> C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLExtras.h:1766:0
 #6 0x00007ff795f6b07f llvm::erase_if<llvm::SmallVector<llvm::DPValue *,4>,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'> C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLExtras.h:2021:0
 #7 0x00007ff795f6ad0b llvm::JumpThreadingPass::updateSSA(class llvm::BasicBlock *, class llvm::BasicBlock *, class llvm::DenseMap<class llvm::Instruction *, class llvm::Value *, struct llvm::DenseMapInfo<class llvm::Instruction *, void>, struct llvm::detail::DenseMapPair<class llvm::Instruction *, class llvm::Value *>> &) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1984:0
 #8 0x00007ff795f6d512 llvm::JumpThreadingPass::threadEdge(class llvm::BasicBlock *, class llvm::SmallVectorImpl<class llvm::BasicBlock *> const &, class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:2449:0
 #9 0x00007ff795f69c03 llvm::JumpThreadingPass::tryThreadEdge(class llvm::BasicBlock *, class llvm::SmallVectorImpl<class llvm::BasicBlock *> const &, class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:2372:0
#10 0x00007ff795f653eb llvm::JumpThreadingPass::processThreadableEdges(class llvm::Value *, class llvm::BasicBlock *, enum llvm::jumpthreading::ConstantPreference, class llvm::Instruction *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1724:0
#11 0x00007ff795f610f0 llvm::JumpThreadingPass::processBlock(class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1137:0
#12 0x00007ff795f5fddc llvm::JumpThreadingPass::runImpl(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> *, class llvm::TargetLibraryInfo *, class llvm::TargetTransformInfo *, class llvm::LazyValueInfo *, class llvm::AAResults *, class std::unique_ptr<class llvm::DomTreeUpdater, struct std::default_delete<class llvm::DomTreeUpdater>>, class std::optional<class llvm::BlockFrequencyInfo *>, class std::optional<class llvm::BranchProbabilityInfo *>) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:332:0
#13 0x00007ff795f5f7d7 llvm::JumpThreadingPass::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:249:0
#14 0x00007ff793da50f8 llvm::detail::PassModel<class llvm::Function, class llvm::JumpThreadingPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#15 0x00007ff79342e257 llvm::PassManager<class llvm::Function, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#16 0x00007ff793da7438 llvm::detail::PassModel<class llvm::Function, class llvm::PassManager<class llvm::Function, class llvm::AnalysisManager<class llvm::Function>>, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#17 0x00007ff795315b3d llvm::CGSCCToFunctionPassAdaptor::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:535:0
#18 0x00007ff795d2fdc2 llvm::detail::PassModel<class llvm::LazyCallGraph::SCC, class llvm::CGSCCToFunctionPassAdaptor, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &>, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &>::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#19 0x00007ff795313bb5 llvm::PassManager<class llvm::LazyCallGraph::SCC, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &>, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &>::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:90:0
#20 0x00007ff795d4bea2 llvm::detail::PassModel<class llvm::LazyCallGraph::SCC, class llvm::PassManager<class llvm::LazyCallGraph::SCC, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &>, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &>, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &>, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &>::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#21 0x00007ff795314a59 llvm::DevirtSCCRepeatedPass::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:405:0
#22 0x00007ff795e7a052 llvm::detail::PassModel<class llvm::LazyCallGraph::SCC, class llvm::DevirtSCCRepeatedPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &>, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &>::run(class llvm::LazyCallGraph::SCC &, class llvm::AnalysisManager<class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &> &, class llvm::LazyCallGraph &, struct llvm::CGSCCUpdateResult &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#23 0x00007ff79531453c llvm::ModuleToPostOrderCGSCCPassAdaptor::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:278:0
#24 0x00007ff795d35938 llvm::detail::PassModel<class llvm::Module, class llvm::ModuleToPostOrderCGSCCPassAdaptor, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#25 0x00007ff79342cf77 llvm::PassManager<class llvm::Module, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#26 0x00007ff7988235d9 llvm::ModuleInlinerWrapperPass::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\lib\Transforms\IPO\Inliner.cpp:631:0
#27 0x00007ff795d30448 llvm::detail::PassModel<class llvm::Module, class llvm::ModuleInlinerWrapperPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#28 0x00007ff79342cf77 llvm::PassManager<class llvm::Module, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#29 0x00007ff793d8cb1b `anonymous namespace'::EmitAssemblyHelper::RunOptimizationPipeline C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1100:0
#30 0x00007ff793d870d9 `anonymous namespace'::EmitAssemblyHelper::EmitAssembly C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1165:0
#31 0x00007ff793d8640f clang::EmitBackendOutput(class clang::DiagnosticsEngine &, class clang::HeaderSearchOptions const &, class clang::CodeGenOptions const &, class clang::TargetOptions const &, class clang::LangOptions const &, class llvm::StringRef, class llvm::Module *, enum clang::BackendAction, class llvm::IntrusiveRefCntPtr<class llvm::vfs::FileSystem>, class std::unique_ptr<class llvm::raw_pwrite_stream, struct std::default_delete<class llvm::raw_pwrite_stream>>, class clang::BackendConsumer *) C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1331:0
#32 0x00007ff794638c8d clang::BackendConsumer::HandleTranslationUnit(class clang::ASTContext &) C:\code\llvm\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:383:0
#33 0x00007ff7973f1218 clang::ParseAST(class clang::Sema &, bool, bool) C:\code\llvm\llvm-project\clang\lib\Parse\ParseAST.cpp:183:0
#34 0x00007ff7943d5d67 clang::ASTFrontendAction::ExecuteAction(void) C:\code\llvm\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1185:0
#35 0x00007ff79463d05a clang::CodeGenAction::ExecuteAction(void) C:\code\llvm\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:1155:0
#36 0x00007ff7943d572d clang::FrontendAction::Execute(void) C:\code\llvm\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1073:0
#37 0x00007ff792b363e4 clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) C:\code\llvm\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1057:0
#38 0x00007ff792c3a28b clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) C:\code\llvm\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:272:0
#39 0x00007ff792192efa cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) C:\code\llvm\llvm-project\clang\tools\driver\cc1_main.cpp:294:0
#40 0x00007ff79218281a ExecuteCC1Tool C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:366:0
#41 0x00007ff7921843fd clang_main::<lambda_0>::operator() C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:506:0
#42 0x00007ff7921843cd llvm::function_ref<int (llvm::SmallVectorImpl<const char *> &)>::callback_fn<`lambda at C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:505:25'> C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:45:0
#43 0x00007ff79408fa61 llvm::function_ref<(class llvm::SmallVectorImpl<char const *> &)>::operator()(class llvm::SmallVectorImpl<char const *> &) const C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:68:0
#44 0x00007ff79408c6b7 clang::driver::CC1Command::Execute::<lambda_1>::operator() C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:0
#45 0x00007ff79408c683 llvm::function_ref<void ()>::callback_fn<`lambda at C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:22'> C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:45:0
#46 0x00007ff7928ae5c7 llvm::function_ref<(void)>::operator()(void) const C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:68:0
#47 0x00007ff7928e8902 llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref<(void)>) C:\code\llvm\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:235:0
#48 0x00007ff79408c206 clang::driver::CC1Command::Execute(class llvm::ArrayRef<class std::optional<class llvm::StringRef>>, class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> *, bool *) const C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:0
#49 0x00007ff792b1a98a clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &, class clang::driver::Command const *&, bool) const C:\code\llvm\llvm-project\clang\lib\Driver\Compilation.cpp:199:0
#50 0x00007ff792b1abaf clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &, bool) const C:\code\llvm\llvm-project\clang\lib\Driver\Compilation.cpp:253:0
#51 0x00007ff792ad302d clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &) C:\code\llvm\llvm-project\clang\lib\Driver\Driver.cpp:1891:0
#52 0x00007ff79218227f clang_main(int, char **, struct llvm::ToolContext const &) C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:542:0
#53 0x00007ff7921b4af2 main C:\code\llvm\llvm-project\out\debug\tools\clang\tools\driver\clang-driver.cpp:15:0
#54 0x00007ff79bcaa5a4 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
#55 0x00007ff79bcaa5a4 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#56 0x00007fffc3e5257d (C:\WINDOWS\System32\KERNEL32.DLL+0x1257d)
#57 0x00007fffc5c6aa58 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x5aa58)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 18.0.0git (git@gitlab-ncsa.ubisoft.org:llvm/llvm-project d9803841f2d539f6a740c07ebd0f3cb176812dbf)
Target: x86_64-pc-windows-msvc
Thread model: posix
```

Related to #74735

ping @jmorse @OCHyams @SLTozer 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Tobias Hieta (tru)

<details>
Built as requested in https://discourse.llvm.org/t/call-for-testing-non-instruction-debug-info/75522 on our game engine. It crashes on the first file we try to build.

This is a debug build of LLVM with assertions. It failed in release mode as well. Built from SHA d9803841f2d539f6a740c07ebd0f3cb176812dbf

Here is the stack-trace:

```
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff792254da9 llvm::Instruction::getParent(void) C:\code\llvm\llvm-project\llvm\include\llvm\IR\Instruction.h:140:0
 #<!-- -->1 0x00007ff7935497ba llvm::DPValue::getParent(void) const C:\code\llvm\llvm-project\llvm\lib\IR\DebugProgramInstruction.cpp:229:0
 #<!-- -->2 0x00007ff795f6fa82 llvm::JumpThreadingPass::updateSSA::&lt;lambda_4&gt;::operator() C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1980:0
 #<!-- -->3 0x00007ff795f6f9d1 std::find_if&lt;llvm::DPValue **,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'&gt; C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.38.33130\include\xutility:6304:0
 #<!-- -->4 0x00007ff795f6f8a2 std::remove_if&lt;llvm::DPValue **,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'&gt; C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.38.33130\include\xmemory:2238:0
 #<!-- -->5 0x00007ff795f6f812 llvm::remove_if&lt;llvm::SmallVector&lt;llvm::DPValue *,4&gt; &amp;,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'&gt; C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLExtras.h:1766:0
 #<!-- -->6 0x00007ff795f6b07f llvm::erase_if&lt;llvm::SmallVector&lt;llvm::DPValue *,4&gt;,`lambda at C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1979:30'&gt; C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLExtras.h:2021:0
 #<!-- -->7 0x00007ff795f6ad0b llvm::JumpThreadingPass::updateSSA(class llvm::BasicBlock *, class llvm::BasicBlock *, class llvm::DenseMap&lt;class llvm::Instruction *, class llvm::Value *, struct llvm::DenseMapInfo&lt;class llvm::Instruction *, void&gt;, struct llvm::detail::DenseMapPair&lt;class llvm::Instruction *, class llvm::Value *&gt;&gt; &amp;) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1984:0
 #<!-- -->8 0x00007ff795f6d512 llvm::JumpThreadingPass::threadEdge(class llvm::BasicBlock *, class llvm::SmallVectorImpl&lt;class llvm::BasicBlock *&gt; const &amp;, class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:2449:0
 #<!-- -->9 0x00007ff795f69c03 llvm::JumpThreadingPass::tryThreadEdge(class llvm::BasicBlock *, class llvm::SmallVectorImpl&lt;class llvm::BasicBlock *&gt; const &amp;, class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:2372:0
#<!-- -->10 0x00007ff795f653eb llvm::JumpThreadingPass::processThreadableEdges(class llvm::Value *, class llvm::BasicBlock *, enum llvm::jumpthreading::ConstantPreference, class llvm::Instruction *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1724:0
#<!-- -->11 0x00007ff795f610f0 llvm::JumpThreadingPass::processBlock(class llvm::BasicBlock *) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:1137:0
#<!-- -->12 0x00007ff795f5fddc llvm::JumpThreadingPass::runImpl(class llvm::Function &amp;, class llvm::AnalysisManager&lt;class llvm::Function&gt; *, class llvm::TargetLibraryInfo *, class llvm::TargetTransformInfo *, class llvm::LazyValueInfo *, class llvm::AAResults *, class std::unique_ptr&lt;class llvm::DomTreeUpdater, struct std::default_delete&lt;class llvm::DomTreeUpdater&gt;&gt;, class std::optional&lt;class llvm::BlockFrequencyInfo *&gt;, class std::optional&lt;class llvm::BranchProbabilityInfo *&gt;) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:332:0
#<!-- -->13 0x00007ff795f5f7d7 llvm::JumpThreadingPass::run(class llvm::Function &amp;, class llvm::AnalysisManager&lt;class llvm::Function&gt; &amp;) C:\code\llvm\llvm-project\llvm\lib\Transforms\Scalar\JumpThreading.cpp:249:0
#<!-- -->14 0x00007ff793da50f8 llvm::detail::PassModel&lt;class llvm::Function, class llvm::JumpThreadingPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::Function&gt;&gt;::run(class llvm::Function &amp;, class llvm::AnalysisManager&lt;class llvm::Function&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->15 0x00007ff79342e257 llvm::PassManager&lt;class llvm::Function, class llvm::AnalysisManager&lt;class llvm::Function&gt;&gt;::run(class llvm::Function &amp;, class llvm::AnalysisManager&lt;class llvm::Function&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#<!-- -->16 0x00007ff793da7438 llvm::detail::PassModel&lt;class llvm::Function, class llvm::PassManager&lt;class llvm::Function, class llvm::AnalysisManager&lt;class llvm::Function&gt;&gt;, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::Function&gt;&gt;::run(class llvm::Function &amp;, class llvm::AnalysisManager&lt;class llvm::Function&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->17 0x00007ff795315b3d llvm::CGSCCToFunctionPassAdaptor::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:535:0
#<!-- -->18 0x00007ff795d2fdc2 llvm::detail::PassModel&lt;class llvm::LazyCallGraph::SCC, class llvm::CGSCCToFunctionPassAdaptor, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;&gt;::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->19 0x00007ff795313bb5 llvm::PassManager&lt;class llvm::LazyCallGraph::SCC, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;&gt;::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:90:0
#<!-- -->20 0x00007ff795d4bea2 llvm::detail::PassModel&lt;class llvm::LazyCallGraph::SCC, class llvm::PassManager&lt;class llvm::LazyCallGraph::SCC, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;&gt;, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;&gt;::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->21 0x00007ff795314a59 llvm::DevirtSCCRepeatedPass::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:405:0
#<!-- -->22 0x00007ff795e7a052 llvm::detail::PassModel&lt;class llvm::LazyCallGraph::SCC, class llvm::DevirtSCCRepeatedPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;&gt;::run(class llvm::LazyCallGraph::SCC &amp;, class llvm::AnalysisManager&lt;class llvm::LazyCallGraph::SCC, class llvm::LazyCallGraph &amp;&gt; &amp;, class llvm::LazyCallGraph &amp;, struct llvm::CGSCCUpdateResult &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->23 0x00007ff79531453c llvm::ModuleToPostOrderCGSCCPassAdaptor::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\lib\Analysis\CGSCCPassManager.cpp:278:0
#<!-- -->24 0x00007ff795d35938 llvm::detail::PassModel&lt;class llvm::Module, class llvm::ModuleToPostOrderCGSCCPassAdaptor, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::Module&gt;&gt;::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->25 0x00007ff79342cf77 llvm::PassManager&lt;class llvm::Module, class llvm::AnalysisManager&lt;class llvm::Module&gt;&gt;::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#<!-- -->26 0x00007ff7988235d9 llvm::ModuleInlinerWrapperPass::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\lib\Transforms\IPO\Inliner.cpp:631:0
#<!-- -->27 0x00007ff795d30448 llvm::detail::PassModel&lt;class llvm::Module, class llvm::ModuleInlinerWrapperPass, class llvm::PreservedAnalyses, class llvm::AnalysisManager&lt;class llvm::Module&gt;&gt;::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:89:0
#<!-- -->28 0x00007ff79342cf77 llvm::PassManager&lt;class llvm::Module, class llvm::AnalysisManager&lt;class llvm::Module&gt;&gt;::run(class llvm::Module &amp;, class llvm::AnalysisManager&lt;class llvm::Module&gt; &amp;) C:\code\llvm\llvm-project\llvm\include\llvm\IR\PassManager.h:547:0
#<!-- -->29 0x00007ff793d8cb1b `anonymous namespace'::EmitAssemblyHelper::RunOptimizationPipeline C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1100:0
#<!-- -->30 0x00007ff793d870d9 `anonymous namespace'::EmitAssemblyHelper::EmitAssembly C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1165:0
#<!-- -->31 0x00007ff793d8640f clang::EmitBackendOutput(class clang::DiagnosticsEngine &amp;, class clang::HeaderSearchOptions const &amp;, class clang::CodeGenOptions const &amp;, class clang::TargetOptions const &amp;, class clang::LangOptions const &amp;, class llvm::StringRef, class llvm::Module *, enum clang::BackendAction, class llvm::IntrusiveRefCntPtr&lt;class llvm::vfs::FileSystem&gt;, class std::unique_ptr&lt;class llvm::raw_pwrite_stream, struct std::default_delete&lt;class llvm::raw_pwrite_stream&gt;&gt;, class clang::BackendConsumer *) C:\code\llvm\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1331:0
#<!-- -->32 0x00007ff794638c8d clang::BackendConsumer::HandleTranslationUnit(class clang::ASTContext &amp;) C:\code\llvm\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:383:0
#<!-- -->33 0x00007ff7973f1218 clang::ParseAST(class clang::Sema &amp;, bool, bool) C:\code\llvm\llvm-project\clang\lib\Parse\ParseAST.cpp:183:0
#<!-- -->34 0x00007ff7943d5d67 clang::ASTFrontendAction::ExecuteAction(void) C:\code\llvm\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1185:0
#<!-- -->35 0x00007ff79463d05a clang::CodeGenAction::ExecuteAction(void) C:\code\llvm\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:1155:0
#<!-- -->36 0x00007ff7943d572d clang::FrontendAction::Execute(void) C:\code\llvm\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1073:0
#<!-- -->37 0x00007ff792b363e4 clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &amp;) C:\code\llvm\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1057:0
#<!-- -->38 0x00007ff792c3a28b clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) C:\code\llvm\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:272:0
#<!-- -->39 0x00007ff792192efa cc1_main(class llvm::ArrayRef&lt;char const *&gt;, char const *, void *) C:\code\llvm\llvm-project\clang\tools\driver\cc1_main.cpp:294:0
#<!-- -->40 0x00007ff79218281a ExecuteCC1Tool C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:366:0
#<!-- -->41 0x00007ff7921843fd clang_main::&lt;lambda_0&gt;::operator() C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:506:0
#<!-- -->42 0x00007ff7921843cd llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;const char *&gt; &amp;)&gt;::callback_fn&lt;`lambda at C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:505:25'&gt; C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:45:0
#<!-- -->43 0x00007ff79408fa61 llvm::function_ref&lt;(class llvm::SmallVectorImpl&lt;char const *&gt; &amp;)&gt;::operator()(class llvm::SmallVectorImpl&lt;char const *&gt; &amp;) const C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:68:0
#<!-- -->44 0x00007ff79408c6b7 clang::driver::CC1Command::Execute::&lt;lambda_1&gt;::operator() C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:0
#<!-- -->45 0x00007ff79408c683 llvm::function_ref&lt;void ()&gt;::callback_fn&lt;`lambda at C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:22'&gt; C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:45:0
#<!-- -->46 0x00007ff7928ae5c7 llvm::function_ref&lt;(void)&gt;::operator()(void) const C:\code\llvm\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:68:0
#<!-- -->47 0x00007ff7928e8902 llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref&lt;(void)&gt;) C:\code\llvm\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:235:0
#<!-- -->48 0x00007ff79408c206 clang::driver::CC1Command::Execute(class llvm::ArrayRef&lt;class std::optional&lt;class llvm::StringRef&gt;&gt;, class std::basic_string&lt;char, struct std::char_traits&lt;char&gt;, class std::allocator&lt;char&gt;&gt; *, bool *) const C:\code\llvm\llvm-project\clang\lib\Driver\Job.cpp:440:0
#<!-- -->49 0x00007ff792b1a98a clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &amp;, class clang::driver::Command const *&amp;, bool) const C:\code\llvm\llvm-project\clang\lib\Driver\Compilation.cpp:199:0
#<!-- -->50 0x00007ff792b1abaf clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;, bool) const C:\code\llvm\llvm-project\clang\lib\Driver\Compilation.cpp:253:0
#<!-- -->51 0x00007ff792ad302d clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;) C:\code\llvm\llvm-project\clang\lib\Driver\Driver.cpp:1891:0
#<!-- -->52 0x00007ff79218227f clang_main(int, char **, struct llvm::ToolContext const &amp;) C:\code\llvm\llvm-project\clang\tools\driver\driver.cpp:542:0
#<!-- -->53 0x00007ff7921b4af2 main C:\code\llvm\llvm-project\out\debug\tools\clang\tools\driver\clang-driver.cpp:15:0
#<!-- -->54 0x00007ff79bcaa5a4 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
#<!-- -->55 0x00007ff79bcaa5a4 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#<!-- -->56 0x00007fffc3e5257d (C:\WINDOWS\System32\KERNEL32.DLL+0x1257d)
#<!-- -->57 0x00007fffc5c6aa58 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x5aa58)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 18.0.0git (git@<!-- -->gitlab-ncsa.ubisoft.org:llvm/llvm-project d9803841f2d539f6a740c07ebd0f3cb176812dbf)
Target: x86_64-pc-windows-msvc
Thread model: posix
```

Related to #<!-- -->74735

ping @<!-- -->jmorse @<!-- -->OCHyams @<!-- -->SLTozer 
</details>


---

### Comment 2 - tru

oh forgot to say - that it build correctly without the specific flags requested in the call to action post.

---

### Comment 3 - OCHyams

Hi @tru. Thanks for giving the new debug mode some testing and thanks for the report.

I was able to produce a similar looking crash if I try enabling the new debug iterators (`-mllvm --experimental-debuginfo-iterators=true`) without enabling support for the iterators in LLVM's cmake settings first `LLVM_EXPERIMENTAL_DEBUGINFO_ITERATORS=ON`).

Are you able to confirm that's also the case for your reproducer here?

I haven't been able to reproduce the crash in any other way (e.g. trying with/without the additional patches that @jmorse linked in the discourse post, building a few different codebases).

Even though that's not a support configuration (enabling the new iterators without enabling support for them in LLVM) we probably shouldn't crash! That could be fixed by ignoring `-mllvm --experimental-debuginfo-iterators=true` in that case, by not converting to the new format (and perhaps issuing some kind of compiler-developer-facing warning).

---

### Comment 4 - tru

Hi @OCHyams - thanks for the reply. I will retry on top of latest main next week and report back. I am pretty sure I had it set to include that CMake option, but I might have messed something up.

---

### Comment 5 - jmorse

Came across this ticket while gardening other debug-info tickets-- I believe RemoveDIs has been on for more than a year without incident now, thus it's almost certainly no longer a problem here.

---

