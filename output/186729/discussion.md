# [MLIR] Crash in SPIRVModuleAnalysis during XeVM lowering with --gpu-lower-to-xevm-pipeline

**Author:** kitty-1998
**URL:** https://github.com/llvm/llvm-project/issues/186729
**Status:** Open
**Labels:** mlir, crash, backend:SPIR-V

## Body

Hi, a crash bug occurs during the XeVM lowering pipeline when the SPIR-V backend attempts to access module analysis during assembly printing. See the following code example for reproduction.

Git version: 27e4ef689a3aa93173dbd212de8aa5a2b5f65402

```llvm
module attributes {gpu.container_module} {
  gpu.module @kernels {
    func.func private @__hipblaslt_init_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_small_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_sin_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_alternating_sign_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_hpl_alternating_sign_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_cos_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_hpl_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_nan_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_nan_tri_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index,
      %arg6: i1
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_inf_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_zero_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_alt_impl_big_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_init_alt_impl_small_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @__hipblaslt_copy_matrix_kernel(
      %arg0: memref<?xf32, 1>,
      %arg1: memref<?xf32, 1>,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index,
      %arg6: index,
      %arg7: index,
      %arg8: index
    ) attributes {gpu.kernel}
  }
}
```

#### Reproduction Command
```bash
mlir-opt hipblaslt.mlir --gpu-lower-to-xevm-pipeline
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt hipblaslt.mlir --gpu-lower-to-xevm-pipeline
 #0 0x00005f52922a8c3d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00005f52922a916b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00005f52922a6fc8 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005f52922a9909 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000073d870845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000073d87089eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000073d87089eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000073d87089eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000073d87084527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000073d8708288ff abort ./stdlib/abort.c:81:7
#10 0x000073d87082881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000073d87083b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005f5294670b18 llvm::SPIRVModuleAnalysis& llvm::Pass::getAnalysisID<llvm::SPIRVModuleAnalysis>(void const*) const /llvm/include/llvm/PassAnalysisSupport.h:240:27
#13 0x00005f5294670a21 llvm::SPIRVModuleAnalysis& llvm::Pass::getAnalysis<llvm::SPIRVModuleAnalysis>() const /llvm/include/llvm/PassAnalysisSupport.h:226:3
#14 0x00005f529466d1df (anonymous namespace)::SPIRVAsmPrinter::outputModuleSections() /llvm/lib/Target/SPIRV/SPIRVAsmPrinter.cpp:824:10
#15 0x00005f529466cc31 (anonymous namespace)::SPIRVAsmPrinter::emitEndOfAsmFile(llvm::Module&) /llvm/lib/Target/SPIRV/SPIRVAsmPrinter.cpp:122:5
#16 0x00005f5297e87d09 llvm::AsmPrinter::doFinalization(llvm::Module&) /llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:3097:3
#17 0x00005f52a0110faa llvm::FPPassManager::doFinalization(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1462:16
#18 0x00005f52a010d2a5 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1549:16
#19 0x00005f52a010cbff llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#20 0x00005f52a0111141 llvm::legacy::PassManager::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#21 0x00005f529466a888 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#22 0x00005f529466ab5f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#23 0x00005f5294664474 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#24 0x00005f5294663c64 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#25 0x00005f52946637ae (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#26 0x00005f52946635d0 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#27 0x00005f529f355f24 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#28 0x00005f52955a8afc (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#29 0x00005f52955a886d mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#30 0x00005f52955a99f5 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#31 0x00005f52a0bc7c24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#32 0x00005f52a0bc7bc5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#33 0x00005f52922cf109 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#34 0x00005f52a0bcb0cb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#35 0x00005f52a0bbf06e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#36 0x00005f52a0bbf78a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#37 0x00005f52a0bc506d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#38 0x00005f52a0bc45f6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#39 0x00005f52923b40fb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#40 0x00005f52923b37d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#41 0x00005f52923b348c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#42 0x00005f52923b3396 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#43 0x00005f52a1130f4a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#44 0x00005f52a113030a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#45 0x00005f52923af0c6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#46 0x00005f52923af4c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#47 0x00005f52923af698 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#48 0x00005f529227d615 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#49 0x000073d87082a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#50 0x000073d87082a28b call_init ./csu/../csu/libc-start.c:128:20
#51 0x000073d87082a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#52 0x00005f529227d485 _start (./mlir-opt+0x4447485)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: kitty-apple (kitty-1998)

<details>
Hi, a crash bug occurs during the XeVM lowering pipeline when the SPIR-V backend attempts to access module analysis during assembly printing. See the following code example for reproduction.

Git version: 27e4ef689a3aa93173dbd212de8aa5a2b5f65402

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    func.func private @<!-- -->__hipblaslt_init_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_small_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_sin_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_alternating_sign_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_hpl_alternating_sign_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_cos_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_hpl_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_nan_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_nan_tri_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index,
      %arg6: i1
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_inf_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_zero_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_alt_impl_big_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_init_alt_impl_small_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: index,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index
    ) attributes {gpu.kernel}
    
    func.func private @<!-- -->__hipblaslt_copy_matrix_kernel(
      %arg0: memref&lt;?xf32, 1&gt;,
      %arg1: memref&lt;?xf32, 1&gt;,
      %arg2: index,
      %arg3: index,
      %arg4: index,
      %arg5: index,
      %arg6: index,
      %arg7: index,
      %arg8: index
    ) attributes {gpu.kernel}
  }
}
```

#### Reproduction Command
```bash
mlir-opt hipblaslt.mlir --gpu-lower-to-xevm-pipeline
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt hipblaslt.mlir --gpu-lower-to-xevm-pipeline
 #<!-- -->0 0x00005f52922a8c3d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00005f52922a916b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00005f52922a6fc8 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005f52922a9909 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000073d870845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000073d87089eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000073d87089eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000073d87089eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000073d87084527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000073d8708288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000073d87082881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000073d87083b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005f5294670b18 llvm::SPIRVModuleAnalysis&amp; llvm::Pass::getAnalysisID&lt;llvm::SPIRVModuleAnalysis&gt;(void const*) const /llvm/include/llvm/PassAnalysisSupport.h:240:27
#<!-- -->13 0x00005f5294670a21 llvm::SPIRVModuleAnalysis&amp; llvm::Pass::getAnalysis&lt;llvm::SPIRVModuleAnalysis&gt;() const /llvm/include/llvm/PassAnalysisSupport.h:226:3
#<!-- -->14 0x00005f529466d1df (anonymous namespace)::SPIRVAsmPrinter::outputModuleSections() /llvm/lib/Target/SPIRV/SPIRVAsmPrinter.cpp:824:10
#<!-- -->15 0x00005f529466cc31 (anonymous namespace)::SPIRVAsmPrinter::emitEndOfAsmFile(llvm::Module&amp;) /llvm/lib/Target/SPIRV/SPIRVAsmPrinter.cpp:122:5
#<!-- -->16 0x00005f5297e87d09 llvm::AsmPrinter::doFinalization(llvm::Module&amp;) /llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:3097:3
#<!-- -->17 0x00005f52a0110faa llvm::FPPassManager::doFinalization(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1462:16
#<!-- -->18 0x00005f52a010d2a5 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1549:16
#<!-- -->19 0x00005f52a010cbff llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->20 0x00005f52a0111141 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->21 0x00005f529466a888 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->22 0x00005f529466ab5f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->23 0x00005f5294664474 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->24 0x00005f5294663c64 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->25 0x00005f52946637ae (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->26 0x00005f52946635d0 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->27 0x00005f529f355f24 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->28 0x00005f52955a8afc (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->29 0x00005f52955a886d mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->30 0x00005f52955a99f5 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->31 0x00005f52a0bc7c24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->32 0x00005f52a0bc7bc5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->33 0x00005f52922cf109 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->34 0x00005f52a0bcb0cb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->35 0x00005f52a0bbf06e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->36 0x00005f52a0bbf78a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->37 0x00005f52a0bc506d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->38 0x00005f52a0bc45f6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->39 0x00005f52923b40fb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->40 0x00005f52923b37d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->41 0x00005f52923b348c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->42 0x00005f52923b3396 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->43 0x00005f52a1130f4a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->44 0x00005f52a113030a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->45 0x00005f52923af0c6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->46 0x00005f52923af4c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->47 0x00005f52923af698 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->48 0x00005f529227d615 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->49 0x000073d87082a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x000073d87082a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x000073d87082a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x00005f529227d485 _start (./mlir-opt+0x4447485)
Aborted
```
</details>


---

### Comment 2 - aviralgarg05

I would like to solve this issue, please if someone could assign me

---

