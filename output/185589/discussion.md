# [MLIR] Crash in SPIRVEmitIntrinsics during the pipeline converts gpu to xevm

**Author:** kitty-1998
**URL:** https://github.com/llvm/llvm-project/issues/185589
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I found a crash bug when lowering a gpu program by the pipeline `--gpu-lower-to-xevm-pipeline`. See the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
gpu.module @parallel_gemv_kernel {
  gpu.func @optimized_gemv(%size: index, %input: memref<?xf64>, %output: memref<?xf64>) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    %f0 = arith.constant 0.0 : f64
    %tid_idx = gpu.thread_id x
    %shared = memref.alloc() : memref<64xcomplex<f64>>
    %cplx_zero = complex.create %f0, %f0 : complex<f64>
    %compute = scf.for %i = %c0 to %size step %c1 iter_args(%cplx_acc = %cplx_zero) -> (complex<f64>) {
      %val = memref.load %input[%i] : memref<?xf64>
      %imag = arith.negf %val : f64
      %cplx_val = complex.create %val, %imag : complex<f64>
      %new_cplx = complex.add %cplx_acc, %cplx_val : complex<f64>
      %shared_idx = arith.remui %i, %c64 : index
      memref.store %new_cplx, %shared[%shared_idx] : memref<64xcomplex<f64>>
      scf.yield %new_cplx : complex<f64>
    }
    %real = complex.re %compute : complex<f64>
    %imag = complex.im %compute : complex<f64>
    %combined = arith.addf %real, %imag : f64
    memref.store %combined, %output[%tid_idx] : memref<?xf64>
    gpu.return
  }
}
```

#### Stack Trace
```bash
UNREACHABLE executed at /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:1489!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt optimized_gemv.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'SPIRV emit intrinsics' on module 'LLVMDialectModule'.
 #0 0x00005bdcdc64adbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00005bdcdc64b2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00005bdcdc649184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005bdcdc64ba89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007dfd02645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007dfd0269eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007dfd0269eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007dfd0269eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007dfd0264527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007dfd026288ff abort ./stdlib/abort.c:81:7
#10 0x00005bdcdc623ed8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#11 0x00005bdcdeaa20e1 (anonymous namespace)::SPIRVEmitIntrinsics::replaceMemInstrUses(llvm::Instruction*, llvm::Instruction*, llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter>&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:0:7
#12 0x00005bdcdeaa5611 (anonymous namespace)::SPIRVEmitIntrinsics::visitInsertValueInst(llvm::InsertValueInst&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:2125:10
#13 0x00005bdcdeaa09cd llvm::InstVisitor<(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*>::visitInsertValue(llvm::InsertValueInst&) /llvm/include/llvm/IR/Instruction.def:219:1
#14 0x00005bdcdea996e1 llvm::InstVisitor<(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*>::visit(llvm::Instruction&) /llvm/include/llvm/IR/Instruction.def:219:1
#15 0x00005bdcdea9304b (anonymous namespace)::SPIRVEmitIntrinsics::runOnFunction(llvm::Function&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3164:7
#16 0x00005bdcdea913d7 (anonymous namespace)::SPIRVEmitIntrinsics::runOnModule(llvm::Module&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3293:16
#17 0x00005bdcea53f9f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#18 0x00005bdcea53f5bf llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#19 0x00005bdcea543b01 llvm::legacy::PassManager::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#20 0x00005bdcde9fcf48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#21 0x00005bdcde9fd21f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#22 0x00005bdcde9f6b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#23 0x00005bdcde9f6324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#24 0x00005bdcde9f5e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#25 0x00005bdcde9f5c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#26 0x00005bdce978f324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#27 0x00005bdcdf92df4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#28 0x00005bdcdf92dcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#29 0x00005bdcdf92ee45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#30 0x00005bdceaff3fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#31 0x00005bdceaff3f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x00005bdcdc671189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x00005bdceaff744b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#34 0x00005bdceafeb3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#35 0x00005bdceafebb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#36 0x00005bdceaff13ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#37 0x00005bdceaff0976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#38 0x00005bdcdc755e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#39 0x00005bdcdc755551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#40 0x00005bdcdc75520c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#41 0x00005bdcdc755116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x00005bdceb55d6fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x00005bdceb55caba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x00005bdcdc750dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#45 0x00005bdcdc7511c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#46 0x00005bdcdc751398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#47 0x00005bdcdc61f8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#48 0x00007dfd0262a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x00007dfd0262a28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x00007dfd0262a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x00005bdcdc61f745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Commands for Reproduction
```bash
mlir-opt optimized_gemv.mlir --gpu-lower-to-xevm-pipeline
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: kitty-apple (kitty-1998)

<details>
Hi, I found a crash bug when lowering a gpu program by the pipeline `--gpu-lower-to-xevm-pipeline`. See the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
gpu.module @<!-- -->parallel_gemv_kernel {
  gpu.func @<!-- -->optimized_gemv(%size: index, %input: memref&lt;?xf64&gt;, %output: memref&lt;?xf64&gt;) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    %f0 = arith.constant 0.0 : f64
    %tid_idx = gpu.thread_id x
    %shared = memref.alloc() : memref&lt;64xcomplex&lt;f64&gt;&gt;
    %cplx_zero = complex.create %f0, %f0 : complex&lt;f64&gt;
    %compute = scf.for %i = %c0 to %size step %c1 iter_args(%cplx_acc = %cplx_zero) -&gt; (complex&lt;f64&gt;) {
      %val = memref.load %input[%i] : memref&lt;?xf64&gt;
      %imag = arith.negf %val : f64
      %cplx_val = complex.create %val, %imag : complex&lt;f64&gt;
      %new_cplx = complex.add %cplx_acc, %cplx_val : complex&lt;f64&gt;
      %shared_idx = arith.remui %i, %c64 : index
      memref.store %new_cplx, %shared[%shared_idx] : memref&lt;64xcomplex&lt;f64&gt;&gt;
      scf.yield %new_cplx : complex&lt;f64&gt;
    }
    %real = complex.re %compute : complex&lt;f64&gt;
    %imag = complex.im %compute : complex&lt;f64&gt;
    %combined = arith.addf %real, %imag : f64
    memref.store %combined, %output[%tid_idx] : memref&lt;?xf64&gt;
    gpu.return
  }
}
```

#### Stack Trace
```bash
UNREACHABLE executed at /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:1489!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt optimized_gemv.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'SPIRV emit intrinsics' on module 'LLVMDialectModule'.
 #<!-- -->0 0x00005bdcdc64adbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00005bdcdc64b2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00005bdcdc649184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005bdcdc64ba89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007dfd02645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007dfd0269eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007dfd0269eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007dfd0269eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007dfd0264527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007dfd026288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005bdcdc623ed8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#<!-- -->11 0x00005bdcdeaa20e1 (anonymous namespace)::SPIRVEmitIntrinsics::replaceMemInstrUses(llvm::Instruction*, llvm::Instruction*, llvm::IRBuilder&lt;llvm::ConstantFolder, llvm::IRBuilderDefaultInserter&gt;&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:0:7
#<!-- -->12 0x00005bdcdeaa5611 (anonymous namespace)::SPIRVEmitIntrinsics::visitInsertValueInst(llvm::InsertValueInst&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:2125:10
#<!-- -->13 0x00005bdcdeaa09cd llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visitInsertValue(llvm::InsertValueInst&amp;) /llvm/include/llvm/IR/Instruction.def:219:1
#<!-- -->14 0x00005bdcdea996e1 llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visit(llvm::Instruction&amp;) /llvm/include/llvm/IR/Instruction.def:219:1
#<!-- -->15 0x00005bdcdea9304b (anonymous namespace)::SPIRVEmitIntrinsics::runOnFunction(llvm::Function&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3164:7
#<!-- -->16 0x00005bdcdea913d7 (anonymous namespace)::SPIRVEmitIntrinsics::runOnModule(llvm::Module&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3293:16
#<!-- -->17 0x00005bdcea53f9f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->18 0x00005bdcea53f5bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->19 0x00005bdcea543b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->20 0x00005bdcde9fcf48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->21 0x00005bdcde9fd21f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->22 0x00005bdcde9f6b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->23 0x00005bdcde9f6324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->24 0x00005bdcde9f5e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->25 0x00005bdcde9f5c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->26 0x00005bdce978f324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->27 0x00005bdcdf92df4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->28 0x00005bdcdf92dcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->29 0x00005bdcdf92ee45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->30 0x00005bdceaff3fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->31 0x00005bdceaff3f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x00005bdcdc671189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x00005bdceaff744b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x00005bdceafeb3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->35 0x00005bdceafebb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->36 0x00005bdceaff13ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->37 0x00005bdceaff0976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->38 0x00005bdcdc755e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->39 0x00005bdcdc755551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x00005bdcdc75520c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x00005bdcdc755116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x00005bdceb55d6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x00005bdceb55caba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x00005bdcdc750dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x00005bdcdc7511c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x00005bdcdc751398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x00005bdcdc61f8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->48 0x00007dfd0262a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x00007dfd0262a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x00007dfd0262a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x00005bdcdc61f745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Commands for Reproduction
```bash
mlir-opt optimized_gemv.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

