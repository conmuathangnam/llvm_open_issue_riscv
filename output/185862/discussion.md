# [MLIR] Crash in SPIRVEmitIntrinsics::visitStoreInst during --gpu-lower-to-xevm-pipeline

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/185862
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I found a crash bug when using the XeVM GPU lowering pipeline, please see the following code example for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @gpu_module {
    gpu.func @execute_gpu(%task_data: memref<?xcomplex<f64>>, %task_vector: memref<?xcomplex<f64>>, %task_result: memref<?xcomplex<f64>>,
                          %size: i32, %id: i32, %tid: i32, %threshold: i32) kernel {
      %c0_i32 = arith.constant 0 : i32
      %c1_i32 = arith.constant 1 : i32
      %c1_complex = complex.constant [1.0, 0.0] : complex<f64>
      %c0_complex = complex.constant [0.0, 0.0] : complex<f64>
      %c0_index = arith.constant 0 : index
      %c1_index = arith.constant 1 : index
      
      %thread_i32 = arith.index_cast %tid : i32 to index
      %size_idx = arith.index_cast %size : i32 to index
      %cmp = arith.cmpi slt, %id, %size : i32
      scf.if %cmp {
        %idx = arith.index_cast %id : i32 to index
        %sum = complex.constant [0.0, 0.0] : complex<f64>
        %final = scf.for %i = %c0_index to %size_idx step %c1_index iter_args(%acc = %sum) -> (complex<f64>) {
          %i_idx = arith.index_cast %i : index to i32
          %i_cast = arith.index_cast %i_idx : i32 to index
          %a = memref.load %task_data[%i_cast] : memref<?xcomplex<f64>>
          %x = memref.load %task_vector[%i_cast] : memref<?xcomplex<f64>>
          %p = complex.mul %a, %x : complex<f64>
          %new = complex.add %acc, %p : complex<f64>
          scf.yield %new : complex<f64>
        }
        memref.store %final, %task_result[%idx] : memref<?xcomplex<f64>>
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'SPIRV emit intrinsics' on module 'LLVMDialectModule'.
 #0 0x000055c36cfdddbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000055c36cfde2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000055c36cfdc184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000055c36cfdea89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000792c74045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000792c7409eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000792c7409eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000792c7409eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000792c7404527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000792c740288ff abort ./stdlib/abort.c:81:7
#10 0x0000792c7402881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000792c7403b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000055c36f4353f8 (anonymous namespace)::SPIRVEmitIntrinsics::visitStoreInst(llvm::StoreInst&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:2179:5
#13 0x000055c36f43339d llvm::InstVisitor<(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*>::visitStore(llvm::StoreInst&) /llvm/include/llvm/IR/Instruction.def:173:1
#14 0x000055c36f42c417 llvm::InstVisitor<(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*>::visit(llvm::Instruction&) /llvm/include/llvm/IR/Instruction.def:173:1
#15 0x000055c36f42604b (anonymous namespace)::SPIRVEmitIntrinsics::runOnFunction(llvm::Function&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3164:7
#16 0x000055c36f4243d7 (anonymous namespace)::SPIRVEmitIntrinsics::runOnModule(llvm::Module&) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3293:16
#17 0x000055c37aed29f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#18 0x000055c37aed25bf llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#19 0x000055c37aed6b01 llvm::legacy::PassManager::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#20 0x000055c36f38ff48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#21 0x000055c36f39021f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#22 0x000055c36f389b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#23 0x000055c36f389324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#24 0x000055c36f388e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#25 0x000055c36f388c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#26 0x000055c37a122324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#27 0x000055c3702c0f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#28 0x000055c3702c0cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#29 0x000055c3702c1e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#30 0x000055c37b986fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#31 0x000055c37b986f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x000055c36d004189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x000055c37b98a44b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#34 0x000055c37b97e3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#35 0x000055c37b97eb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#36 0x000055c37b9843ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#37 0x000055c37b983976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#38 0x000055c36d0e8e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#39 0x000055c36d0e8551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#40 0x000055c36d0e820c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#41 0x000055c36d0e8116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x000055c37bef06fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x000055c37beefaba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x000055c36d0e3dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#45 0x000055c36d0e41c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#46 0x000055c36d0e4398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#47 0x000055c36cfb28d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#48 0x0000792c7402a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x0000792c7402a28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x0000792c7402a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x000055c36cfb2745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when using the XeVM GPU lowering pipeline, please see the following code example for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->gpu_module {
    gpu.func @<!-- -->execute_gpu(%task_data: memref&lt;?xcomplex&lt;f64&gt;&gt;, %task_vector: memref&lt;?xcomplex&lt;f64&gt;&gt;, %task_result: memref&lt;?xcomplex&lt;f64&gt;&gt;,
                          %size: i32, %id: i32, %tid: i32, %threshold: i32) kernel {
      %c0_i32 = arith.constant 0 : i32
      %c1_i32 = arith.constant 1 : i32
      %c1_complex = complex.constant [1.0, 0.0] : complex&lt;f64&gt;
      %c0_complex = complex.constant [0.0, 0.0] : complex&lt;f64&gt;
      %c0_index = arith.constant 0 : index
      %c1_index = arith.constant 1 : index
      
      %thread_i32 = arith.index_cast %tid : i32 to index
      %size_idx = arith.index_cast %size : i32 to index
      %cmp = arith.cmpi slt, %id, %size : i32
      scf.if %cmp {
        %idx = arith.index_cast %id : i32 to index
        %sum = complex.constant [0.0, 0.0] : complex&lt;f64&gt;
        %final = scf.for %i = %c0_index to %size_idx step %c1_index iter_args(%acc = %sum) -&gt; (complex&lt;f64&gt;) {
          %i_idx = arith.index_cast %i : index to i32
          %i_cast = arith.index_cast %i_idx : i32 to index
          %a = memref.load %task_data[%i_cast] : memref&lt;?xcomplex&lt;f64&gt;&gt;
          %x = memref.load %task_vector[%i_cast] : memref&lt;?xcomplex&lt;f64&gt;&gt;
          %p = complex.mul %a, %x : complex&lt;f64&gt;
          %new = complex.add %acc, %p : complex&lt;f64&gt;
          scf.yield %new : complex&lt;f64&gt;
        }
        memref.store %final, %task_result[%idx] : memref&lt;?xcomplex&lt;f64&gt;&gt;
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'SPIRV emit intrinsics' on module 'LLVMDialectModule'.
 #<!-- -->0 0x000055c36cfdddbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000055c36cfde2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000055c36cfdc184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000055c36cfdea89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000792c74045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000792c7409eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000792c7409eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000792c7409eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000792c7404527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000792c740288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000792c7402881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000792c7403b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000055c36f4353f8 (anonymous namespace)::SPIRVEmitIntrinsics::visitStoreInst(llvm::StoreInst&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:2179:5
#<!-- -->13 0x000055c36f43339d llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visitStore(llvm::StoreInst&amp;) /llvm/include/llvm/IR/Instruction.def:173:1
#<!-- -->14 0x000055c36f42c417 llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visit(llvm::Instruction&amp;) /llvm/include/llvm/IR/Instruction.def:173:1
#<!-- -->15 0x000055c36f42604b (anonymous namespace)::SPIRVEmitIntrinsics::runOnFunction(llvm::Function&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3164:7
#<!-- -->16 0x000055c36f4243d7 (anonymous namespace)::SPIRVEmitIntrinsics::runOnModule(llvm::Module&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3293:16
#<!-- -->17 0x000055c37aed29f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->18 0x000055c37aed25bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->19 0x000055c37aed6b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->20 0x000055c36f38ff48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->21 0x000055c36f39021f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->22 0x000055c36f389b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->23 0x000055c36f389324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->24 0x000055c36f388e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->25 0x000055c36f388c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->26 0x000055c37a122324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->27 0x000055c3702c0f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->28 0x000055c3702c0cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->29 0x000055c3702c1e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->30 0x000055c37b986fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->31 0x000055c37b986f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x000055c36d004189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x000055c37b98a44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x000055c37b97e3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->35 0x000055c37b97eb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->36 0x000055c37b9843ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->37 0x000055c37b983976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->38 0x000055c36d0e8e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->39 0x000055c36d0e8551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x000055c36d0e820c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x000055c36d0e8116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000055c37bef06fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000055c37beefaba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x000055c36d0e3dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x000055c36d0e41c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x000055c36d0e4398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x000055c36cfb28d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->48 0x0000792c7402a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x0000792c7402a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x0000792c7402a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x000055c36cfb2745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when using the XeVM GPU lowering pipeline, please see the following code example for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->gpu_module {
    gpu.func @<!-- -->execute_gpu(%task_data: memref&lt;?xcomplex&lt;f64&gt;&gt;, %task_vector: memref&lt;?xcomplex&lt;f64&gt;&gt;, %task_result: memref&lt;?xcomplex&lt;f64&gt;&gt;,
                          %size: i32, %id: i32, %tid: i32, %threshold: i32) kernel {
      %c0_i32 = arith.constant 0 : i32
      %c1_i32 = arith.constant 1 : i32
      %c1_complex = complex.constant [1.0, 0.0] : complex&lt;f64&gt;
      %c0_complex = complex.constant [0.0, 0.0] : complex&lt;f64&gt;
      %c0_index = arith.constant 0 : index
      %c1_index = arith.constant 1 : index
      
      %thread_i32 = arith.index_cast %tid : i32 to index
      %size_idx = arith.index_cast %size : i32 to index
      %cmp = arith.cmpi slt, %id, %size : i32
      scf.if %cmp {
        %idx = arith.index_cast %id : i32 to index
        %sum = complex.constant [0.0, 0.0] : complex&lt;f64&gt;
        %final = scf.for %i = %c0_index to %size_idx step %c1_index iter_args(%acc = %sum) -&gt; (complex&lt;f64&gt;) {
          %i_idx = arith.index_cast %i : index to i32
          %i_cast = arith.index_cast %i_idx : i32 to index
          %a = memref.load %task_data[%i_cast] : memref&lt;?xcomplex&lt;f64&gt;&gt;
          %x = memref.load %task_vector[%i_cast] : memref&lt;?xcomplex&lt;f64&gt;&gt;
          %p = complex.mul %a, %x : complex&lt;f64&gt;
          %new = complex.add %acc, %p : complex&lt;f64&gt;
          scf.yield %new : complex&lt;f64&gt;
        }
        memref.store %final, %task_result[%idx] : memref&lt;?xcomplex&lt;f64&gt;&gt;
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'SPIRV emit intrinsics' on module 'LLVMDialectModule'.
 #<!-- -->0 0x000055c36cfdddbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000055c36cfde2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000055c36cfdc184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000055c36cfdea89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000792c74045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000792c7409eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000792c7409eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000792c7409eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000792c7404527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000792c740288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000792c7402881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000792c7403b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000055c36f4353f8 (anonymous namespace)::SPIRVEmitIntrinsics::visitStoreInst(llvm::StoreInst&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:2179:5
#<!-- -->13 0x000055c36f43339d llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visitStore(llvm::StoreInst&amp;) /llvm/include/llvm/IR/Instruction.def:173:1
#<!-- -->14 0x000055c36f42c417 llvm::InstVisitor&lt;(anonymous namespace)::SPIRVEmitIntrinsics, llvm::Instruction*&gt;::visit(llvm::Instruction&amp;) /llvm/include/llvm/IR/Instruction.def:173:1
#<!-- -->15 0x000055c36f42604b (anonymous namespace)::SPIRVEmitIntrinsics::runOnFunction(llvm::Function&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3164:7
#<!-- -->16 0x000055c36f4243d7 (anonymous namespace)::SPIRVEmitIntrinsics::runOnModule(llvm::Module&amp;) /llvm/lib/Target/SPIRV/SPIRVEmitIntrinsics.cpp:3293:16
#<!-- -->17 0x000055c37aed29f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->18 0x000055c37aed25bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->19 0x000055c37aed6b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->20 0x000055c36f38ff48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->21 0x000055c36f39021f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->22 0x000055c36f389b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->23 0x000055c36f389324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->24 0x000055c36f388e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->25 0x000055c36f388c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->26 0x000055c37a122324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->27 0x000055c3702c0f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->28 0x000055c3702c0cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->29 0x000055c3702c1e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->30 0x000055c37b986fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->31 0x000055c37b986f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x000055c36d004189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x000055c37b98a44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x000055c37b97e3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->35 0x000055c37b97eb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->36 0x000055c37b9843ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->37 0x000055c37b983976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->38 0x000055c36d0e8e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->39 0x000055c36d0e8551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x000055c36d0e820c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x000055c36d0e8116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000055c37bef06fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000055c37beefaba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x000055c36d0e3dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x000055c36d0e41c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x000055c36d0e4398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x000055c36cfb28d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->48 0x0000792c7402a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x0000792c7402a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x0000792c7402a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x000055c36cfb2745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt execute_gpu.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

