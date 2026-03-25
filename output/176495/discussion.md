# [HLSL] Offload test `Feature/HLSLLib/adduint64.test` fails to compile due to assertion error

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/176495
**Status:** Closed
**Labels:** duplicate, crash, llvm:instcombine
**Closed Date:** 2026-01-17T17:20:07Z

## Body

The test [`Feature/HLSLLib/adduint64.test` from the LLVM offload-test-suite](https://github.com/llvm/offload-test-suite/blob/main/test/Feature/HLSLLib/adduint64.test) is failing to compile with Clang due to hitting an assertion: `Assertion failed: isValidElementType(ElementType) && "Element type of a VectorType must " "be an integer, floating point, " "pointer type, or a valid target " "extension type."`

```hlsl
// compile cmd: clang-dxc -T cs_6_8 test.hlsl
StructuredBuffer<uint4> In : register(t0);
RWStructuredBuffer<uint4> Out : register(u1);

[numthreads(1,1,1)]
void main() {
  // vec4
  Out[0] = AddUint64(In[0], In[0]);

  // vec2
  uint4 Tmp = {AddUint64(In[1].xy, In[1].xy), AddUint64(In[1].zw, In[1].zw)};
  Out[1] = Tmp;
  uint4 Tmp2 = {AddUint64(In[2].xy, In[2].xy), AddUint64(In[2].zw, In[2].zw)};
  Out[2] = Tmp2;
  uint4 Tmp3 = {AddUint64(uint2(0xffffffff, 0xffffffff), uint2(1, 0)), AddUint64(uint2(1, 256), uint2(1, 256))};
  Out[3] = Tmp3;
}
```

This is a relatively recent new failure. Running a git bisect on the llvm-project repo reveals that the merge of PR https://github.com/llvm/llvm-project/pull/174835 is the first bad commit. 
The cause of the test failure issue is likely due to the change to `isTriviallyScalarizable` in `VectorUtils.cpp` in the LLVM backend.
https://github.com/llvm/llvm-project/pull/174835/changes#diff-2f62880098fcadde9f98ed4cec11530191826532c0f019fcca771502a1b2f3a3L125-L143

Logs: https://github.com/llvm/offload-test-suite/actions/runs/21076045825/job/60618042926#step:12:203
```
FAIL: OffloadTest-clang-warp-d3d12 :: Feature/HLSLLib/adduint64.test (57 of 318)
******************** TEST 'OffloadTest-clang-warp-d3d12 :: Feature/HLSLLib/adduint64.test' FAILED ********************
Exit Code: 1

Command Output (stdout):
--
# RUN: at line 74
c:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\bin\split-file.exe C:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\test\Feature\HLSLLib\adduint64.test C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp
# executed command: 'c:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\bin\split-file.exe' 'C:\actions-runner\_work\offload-test-suite\offload-test-suite\OffloadTest\test\Feature\HLSLLib\adduint64.test' 'C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp'
# RUN: at line 75
c:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\bin\clang-dxc.exe --dxv-path=C:/actions-runner/_work/offload-test-suite/offload-test-suite/DXC/build/bin -T cs_6_5 -Fo C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp.o C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp/source.hlsl
# executed command: 'c:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\bin\clang-dxc.exe' --dxv-path=C:/actions-runner/_work/offload-test-suite/offload-test-suite/DXC/build/bin -T cs_6_5 -Fo 'C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp.o' 'C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp/source.hlsl'
# .---command stderr------------
# | Assertion failed: isValidElementType(ElementType) && "Element type of a VectorType must " "be an integer, floating point, " "pointer type, or a valid target " "extension type.", file C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\Type.cpp, line 807
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
# | Stack dump:
# | 0.	Program arguments: C:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\bin\\clang-dxc.exe -cc1 -triple dxilv1.5-unknown-shadermodel6.5-compute -O3 -Wconversion -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name source.hlsl -mrelocation-model static -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -debugger-tuning=gdb -fdebug-compilation-dir=C:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-warp-d3d12\\Feature\\HLSLLib -fcoverage-compilation-dir=C:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-warp-d3d12\\Feature\\HLSLLib -resource-dir C:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\lib\\clang\\23 -ferror-limit 19 -O3 -finclude-default-header -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -vectorize-loops -vectorize-slp -o C:\\WINDOWS\\SERVIC~1\\NETWOR~1\\AppData\\Local\\Temp\\lit-tmp-oo_o2o_7\\source-94426e.obj -x hlsl C:\\actions-runner\\_work\\offload-test-suite\\offload-test-suite\\llvm-project\\build\\tools\\OffloadTest\\test\\clang-warp-d3d12\\Feature\\HLSLLib\\Output\\adduint64.test.tmp/source.hlsl
# | 1.	<eof> parser at end of file
# | 2.	Optimizer
# | 3.	Running pass "function<eager-inv>(mem2reg,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>)" on module "C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\OffloadTest\test\clang-warp-d3d12\Feature\HLSLLib\Output\adduint64.test.tmp/source.hlsl"
# | 4.	Running pass "instcombine<max-iterations=1;no-verify-fixpoint>" on function "_Z4mainv"
# | Exception Code: 0xC000001D
# |  #0 0x00007ff6e741d5b6 HandleAbort C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Support\Windows\Signals.inc:371:0
# |  llvm/offload-test-suite#1 0x00007ffe7d051989 (C:\WINDOWS\System32\ucrtbase.dll+0xc1989)
# |  llvm/offload-test-suite#2 0x00007ffe7d034ab1 (C:\WINDOWS\System32\ucrtbase.dll+0xa4ab1)
# |  llvm/offload-test-suite#3 0x00007ffe7d052986 (C:\WINDOWS\System32\ucrtbase.dll+0xc2986)
# |  llvm/offload-test-suite#4 0x00007ffe7d052b61 (C:\WINDOWS\System32\ucrtbase.dll+0xc2b61)
# |  llvm/offload-test-suite#5 0x00007ff6e778550d llvm::Type::getContext C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\Type.h:128:0
# |  llvm/offload-test-suite#6 0x00007ff6e778550d llvm::FixedVectorType::get(class llvm::Type *, unsigned int) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\Type.cpp:811:0
# |  llvm/offload-test-suite#7 0x00007ff6e81f8906 llvm::VectorType::get C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\DerivedTypes.h:475:0
# |  llvm/offload-test-suite#8 0x00007ff6e81f8906 llvm::InstCombinerImpl::foldShuffledIntrinsicOperands(class llvm::IntrinsicInst *) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Transforms\InstCombine\InstCombineCalls.cpp:1536:0
# |  llvm/offload-test-suite#9 0x00007ff6e820248f llvm::InstCombinerImpl::visitCallInst(class llvm::CallInst &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Transforms\InstCombine\InstCombineCalls.cpp:4246:0
# | llvm/offload-test-suite#10 0x00007ff6e71fc6f6 llvm::InstCombinerImpl::run(void) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Transforms\InstCombine\InstructionCombining.cpp:5806:0
# | llvm/offload-test-suite#11 0x00007ff6e7200cf7 combineInstructionsOverFunction C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Transforms\InstCombine\InstructionCombining.cpp:6119:0
# | llvm/offload-test-suite#12 0x00007ff6e7200637 llvm::InstCombinePass::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\Transforms\InstCombine\InstructionCombining.cpp:6181:0
# | llvm/offload-test-suite#13 0x00007ff6e8573881 llvm::detail::PassModel<class llvm::Function, class llvm::InstCombinePass, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:91:0
# | llvm/offload-test-suite#14 0x00007ff6e7be45fb llvm::PassManager<class llvm::Function, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\PassManagerImpl.h:80:0
# | llvm/offload-test-suite#15 0x00007ff6e7e13431 llvm::detail::PassModel<class llvm::Function, class llvm::PassManager<class llvm::Function, class llvm::AnalysisManager<class llvm::Function>>, class llvm::AnalysisManager<class llvm::Function>>::run(class llvm::Function &, class llvm::AnalysisManager<class llvm::Function> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:91:0
# | llvm/offload-test-suite#16 0x00007ff6e7be9c96 llvm::ModuleToFunctionPassAdaptor::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\lib\IR\PassManager.cpp:132:0
# | llvm/offload-test-suite#17 0x00007ff6e7e135b1 llvm::detail::PassModel<class llvm::Module, class llvm::ModuleToFunctionPassAdaptor, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\PassManagerInternal.h:91:0
# | llvm/offload-test-suite#18 0x00007ff6e7be31ab llvm::PassManager<class llvm::Module, class llvm::AnalysisManager<class llvm::Module>>::run(class llvm::Module &, class llvm::AnalysisManager<class llvm::Module> &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\PassManagerImpl.h:80:0
# | llvm/offload-test-suite#19 0x00007ff6e856cda8 llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\ADT\SmallPtrSet.h:89:0
# | llvm/offload-test-suite#20 0x00007ff6e856cda8 llvm::PreservedAnalyses::~PreservedAnalyses C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\IR\Analysis.h:112:0
# | llvm/offload-test-suite#21 0x00007ff6e856cda8 `anonymous namespace'::EmitAssemblyHelper::RunOptimizationPipeline C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1209:0
# | llvm/offload-test-suite#22 0x00007ff6e8564696 `anonymous namespace'::EmitAssemblyHelper::emitAssembly C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1300:0
# | llvm/offload-test-suite#23 0x00007ff6e856342c std::unique_ptr<llvm::TargetMachine,std::default_delete<llvm::TargetMachine> >::operator bool C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include\memory:3461:0
# | llvm/offload-test-suite#24 0x00007ff6e856342c clang::emitBackendOutput(class clang::CompilerInstance &, class clang::CodeGenOptions &, class llvm::StringRef, class llvm::Module *, enum clang::BackendAction, class llvm::IntrusiveRefCntPtr<class llvm::vfs::FileSystem>, class std::unique_ptr<class llvm::raw_pwrite_stream, struct std::default_delete<class llvm::raw_pwrite_stream>>, class clang::BackendConsumer *) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\BackendUtil.cpp:1477:0
# | llvm/offload-test-suite#25 0x00007ff6e8b5500e clang::BackendConsumer::HandleTranslationUnit(class clang::ASTContext &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:310:0
# | llvm/offload-test-suite#26 0x00007ff6ead11617 clang::ParseAST(class clang::Sema &, bool, bool) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Parse\ParseAST.cpp:183:0
# | llvm/offload-test-suite#27 0x00007ff6e8b01396 clang::HLSLFrontendAction::ExecuteAction(void) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendActions.cpp:1326:0
# | llvm/offload-test-suite#28 0x00007ff6e8a95441 clang::FrontendAction::Execute(void) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1317:0
# | llvm/offload-test-suite#29 0x00007ff6e75807eb llvm::Error::getPtr C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:278:0
# | llvm/offload-test-suite#30 0x00007ff6e75807eb llvm::Error::operator bool C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\llvm\include\llvm\Support\Error.h:242:0
# | llvm/offload-test-suite#31 0x00007ff6e75807eb clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1007:0
# | llvm/offload-test-suite#32 0x00007ff6e7638609 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:310:0
# | llvm/offload-test-suite#33 0x00007ff6e6f07649 cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\cc1_main.cpp:304:0
# | llvm/offload-test-suite#34 0x00007ff6e6f032e0 ExecuteCC1Tool C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:226:0
# | llvm/offload-test-suite#35 0x00007ff6e6f0244f clang_main(int, char **, struct llvm::ToolContext const &) C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\clang\tools\driver\driver.cpp:274:0
# | llvm/offload-test-suite#36 0x00007ff6e6f17f67 main C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\tools\clang\tools\driver\clang-driver.cpp:17:0
# | llvm/offload-test-suite#37 0x00007ff6ed359314 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
# | llvm/offload-test-suite#38 0x00007ff6ed359314 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | llvm/offload-test-suite#39 0x00007ffe7e55e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
# | llvm/offload-test-suite#40 0x00007ffe7f72c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
# | clang-dxc: error: clang frontend command failed due to signal (use -v to see invocation)
# | clang version 23.0.0git (https://github.com/llvm/llvm-project 584a5899653d90a218fb7ba4218ce039cd00d54e)
# | Target: dxilv1.5-unknown-shadermodel6.5-compute
# | Thread model: posix
# | InstalledDir: C:\actions-runner\_work\offload-test-suite\offload-test-suite\llvm-project\build\bin
# | Build config: +assertions
# | clang-dxc: note: diagnostic msg: Error generating preprocessed source(s).
# `-----------------------------
# error: command failed with exit status: 1

--
```


