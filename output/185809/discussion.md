# [MLIR] Crash in SPIRVGlobalRegistry::getOrCreateConstInt during xevm gpu lowering

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/185809
**Status:** Open
**Labels:** mlir, crash, backend:SPIR-V

## Body

Hi, I found a crash bug when using the lowering pipleine of XeVM GPU, please see the following program for the reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @kernels {
    gpu.func @gemv_kernel(%arg0: memref<?xf64>, %arg1: memref<?xf64>, %arg2: memref<8x8xf64>) kernel {
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %c2 = arith.constant 2 : index
      %c4 = arith.constant 4 : index
      %c0_f64 = arith.constant 0.0 : f64
      %tidx = gpu.thread_id x
      %bidx = gpu.block_id x
      %bdim = gpu.block_dim y
      
      %size = memref.dim %arg0, %c0 : memref<?xf64>
      %reduced_size = arith.remsi %size, %c4 : index
      
      %cmp = arith.cmpi ugt, %tidx, %reduced_size : index
      scf.if %cmp {
        %vec = vector.transfer_read %arg2[%c0, %c0], %c0_f64 {in_bounds = [true, true]} : memref<8x8xf64>, vector<8x8xf64>
        %broadcast = vector.broadcast %c0_f64 : f64 to vector<8x8xf64>
        %vec_mul = arith.mulf %vec, %broadcast : vector<8x8xf64>
        vector.transfer_write %vec_mul, %arg2[%c0, %c0] {in_bounds = [true, true]} : vector<8x8xf64>, memref<8x8xf64>
      } else {
        %val1 = memref.load %arg0[%tidx] : memref<?xf64>
        %val2 = memref.load %arg1[%reduced_size] : memref<?xf64>
        %sum = arith.addf %val1, %val2 : f64
        memref.store %sum, %arg0[%bidx] : memref<?xf64>
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@gemv_kernel'
 #0 0x000057cd63e9fdbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000057cd63ea02eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000057cd63e9e184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000057cd63ea0a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000077e699e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000077e699e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000077e699e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000077e699e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000077e699e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000077e699e288ff abort ./stdlib/abort.c:81:7
#10 0x000077e699e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000077e699e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000057cd65b4baa4 decltype(auto) llvm::cast<llvm::IntegerType, llvm::Type const>(llvm::Type const*) /llvm/include/llvm/Support/Casting.h:573:10
#13 0x000057cd66328fb5 llvm::SPIRVGlobalRegistry::getOrCreateConstInt(llvm::APInt const&, llvm::MachineInstr&, llvm::SPIRVTypeInst, llvm::SPIRVInstrInfo const&, bool) /llvm/lib/Target/SPIRV/SPIRVGlobalRegistry.cpp:401:7
#14 0x000057cd6635a0c7 (anonymous namespace)::SPIRVInstructionSelector::selectConst(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3655:14
#15 0x000057cd6634c623 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:901:5
#16 0x000057cd6634ba2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#17 0x000057cd6aaef4c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#18 0x000057cd6aaee6d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#19 0x000057cd6aaee2a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#20 0x000057cd6b3880b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#21 0x000057cd71d941cf llvm::FPPassManager::runOnFunction(llvm::Function&) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#22 0x000057cd71d9885b llvm::FPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#23 0x000057cd71d949f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#24 0x000057cd71d945bf llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#25 0x000057cd71d98b01 llvm::legacy::PassManager::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#26 0x000057cd66251f48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#27 0x000057cd6625221f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#28 0x000057cd6624bb34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#29 0x000057cd6624b324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#30 0x000057cd6624ae6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#31 0x000057cd6624ac90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#32 0x000057cd70fe4324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#33 0x000057cd67182f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#34 0x000057cd67182cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#35 0x000057cd67183e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#36 0x000057cd72848fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#37 0x000057cd72848f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#38 0x000057cd63ec6189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#39 0x000057cd7284c44b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#40 0x000057cd728403ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#41 0x000057cd72840b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#42 0x000057cd728463ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#43 0x000057cd72845976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#44 0x000057cd63faae7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#45 0x000057cd63faa551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#46 0x000057cd63faa20c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#47 0x000057cd63faa116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#48 0x000057cd72db26fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#49 0x000057cd72db1aba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#50 0x000057cd63fa5dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#51 0x000057cd63fa61c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#52 0x000057cd63fa6398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#53 0x000057cd63e748d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#54 0x000077e699e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#55 0x000077e699e2a28b call_init ./csu/../csu/libc-start.c:128:20
#56 0x000077e699e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#57 0x000057cd63e74745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when using the lowering pipleine of XeVM GPU, please see the following program for the reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->gemv_kernel(%arg0: memref&lt;?xf64&gt;, %arg1: memref&lt;?xf64&gt;, %arg2: memref&lt;8x8xf64&gt;) kernel {
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %c2 = arith.constant 2 : index
      %c4 = arith.constant 4 : index
      %c0_f64 = arith.constant 0.0 : f64
      %tidx = gpu.thread_id x
      %bidx = gpu.block_id x
      %bdim = gpu.block_dim y
      
      %size = memref.dim %arg0, %c0 : memref&lt;?xf64&gt;
      %reduced_size = arith.remsi %size, %c4 : index
      
      %cmp = arith.cmpi ugt, %tidx, %reduced_size : index
      scf.if %cmp {
        %vec = vector.transfer_read %arg2[%c0, %c0], %c0_f64 {in_bounds = [true, true]} : memref&lt;8x8xf64&gt;, vector&lt;8x8xf64&gt;
        %broadcast = vector.broadcast %c0_f64 : f64 to vector&lt;8x8xf64&gt;
        %vec_mul = arith.mulf %vec, %broadcast : vector&lt;8x8xf64&gt;
        vector.transfer_write %vec_mul, %arg2[%c0, %c0] {in_bounds = [true, true]} : vector&lt;8x8xf64&gt;, memref&lt;8x8xf64&gt;
      } else {
        %val1 = memref.load %arg0[%tidx] : memref&lt;?xf64&gt;
        %val2 = memref.load %arg1[%reduced_size] : memref&lt;?xf64&gt;
        %sum = arith.addf %val1, %val2 : f64
        memref.store %sum, %arg0[%bidx] : memref&lt;?xf64&gt;
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
0.	Program arguments: mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@<!-- -->gemv_kernel'
 #<!-- -->0 0x000057cd63e9fdbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000057cd63ea02eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000057cd63e9e184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000057cd63ea0a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000077e699e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000077e699e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000077e699e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000077e699e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000077e699e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000077e699e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000077e699e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000077e699e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000057cd65b4baa4 decltype(auto) llvm::cast&lt;llvm::IntegerType, llvm::Type const&gt;(llvm::Type const*) /llvm/include/llvm/Support/Casting.h:573:10
#<!-- -->13 0x000057cd66328fb5 llvm::SPIRVGlobalRegistry::getOrCreateConstInt(llvm::APInt const&amp;, llvm::MachineInstr&amp;, llvm::SPIRVTypeInst, llvm::SPIRVInstrInfo const&amp;, bool) /llvm/lib/Target/SPIRV/SPIRVGlobalRegistry.cpp:401:7
#<!-- -->14 0x000057cd6635a0c7 (anonymous namespace)::SPIRVInstructionSelector::selectConst(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3655:14
#<!-- -->15 0x000057cd6634c623 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:901:5
#<!-- -->16 0x000057cd6634ba2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&amp;) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#<!-- -->17 0x000057cd6aaef4c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#<!-- -->18 0x000057cd6aaee6d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#<!-- -->19 0x000057cd6aaee2a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#<!-- -->20 0x000057cd6b3880b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->21 0x000057cd71d941cf llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->22 0x000057cd71d9885b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->23 0x000057cd71d949f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->24 0x000057cd71d945bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->25 0x000057cd71d98b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->26 0x000057cd66251f48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->27 0x000057cd6625221f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->28 0x000057cd6624bb34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->29 0x000057cd6624b324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->30 0x000057cd6624ae6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->31 0x000057cd6624ac90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->32 0x000057cd70fe4324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->33 0x000057cd67182f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->34 0x000057cd67182cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->35 0x000057cd67183e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->36 0x000057cd72848fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->37 0x000057cd72848f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->38 0x000057cd63ec6189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->39 0x000057cd7284c44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->40 0x000057cd728403ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->41 0x000057cd72840b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->42 0x000057cd728463ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->43 0x000057cd72845976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->44 0x000057cd63faae7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->45 0x000057cd63faa551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->46 0x000057cd63faa20c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->47 0x000057cd63faa116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->48 0x000057cd72db26fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->49 0x000057cd72db1aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->50 0x000057cd63fa5dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->51 0x000057cd63fa61c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->52 0x000057cd63fa6398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->53 0x000057cd63e748d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->54 0x000077e699e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->55 0x000077e699e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->56 0x000077e699e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->57 0x000057cd63e74745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

### Comment 2 - woruyu

I would like to fix this issue! I seems that legalizeSpvConstComposite function use `buildConstant` instead of `buildFConstant`.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when using the lowering pipleine of XeVM GPU, please see the following program for the reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->gemv_kernel(%arg0: memref&lt;?xf64&gt;, %arg1: memref&lt;?xf64&gt;, %arg2: memref&lt;8x8xf64&gt;) kernel {
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %c2 = arith.constant 2 : index
      %c4 = arith.constant 4 : index
      %c0_f64 = arith.constant 0.0 : f64
      %tidx = gpu.thread_id x
      %bidx = gpu.block_id x
      %bdim = gpu.block_dim y
      
      %size = memref.dim %arg0, %c0 : memref&lt;?xf64&gt;
      %reduced_size = arith.remsi %size, %c4 : index
      
      %cmp = arith.cmpi ugt, %tidx, %reduced_size : index
      scf.if %cmp {
        %vec = vector.transfer_read %arg2[%c0, %c0], %c0_f64 {in_bounds = [true, true]} : memref&lt;8x8xf64&gt;, vector&lt;8x8xf64&gt;
        %broadcast = vector.broadcast %c0_f64 : f64 to vector&lt;8x8xf64&gt;
        %vec_mul = arith.mulf %vec, %broadcast : vector&lt;8x8xf64&gt;
        vector.transfer_write %vec_mul, %arg2[%c0, %c0] {in_bounds = [true, true]} : vector&lt;8x8xf64&gt;, memref&lt;8x8xf64&gt;
      } else {
        %val1 = memref.load %arg0[%tidx] : memref&lt;?xf64&gt;
        %val2 = memref.load %arg1[%reduced_size] : memref&lt;?xf64&gt;
        %sum = arith.addf %val1, %val2 : f64
        memref.store %sum, %arg0[%bidx] : memref&lt;?xf64&gt;
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@<!-- -->gemv_kernel'
 #<!-- -->0 0x000057cd63e9fdbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000057cd63ea02eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000057cd63e9e184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000057cd63ea0a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000077e699e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000077e699e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000077e699e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000077e699e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000077e699e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000077e699e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000077e699e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000077e699e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000057cd65b4baa4 decltype(auto) llvm::cast&lt;llvm::IntegerType, llvm::Type const&gt;(llvm::Type const*) /llvm/include/llvm/Support/Casting.h:573:10
#<!-- -->13 0x000057cd66328fb5 llvm::SPIRVGlobalRegistry::getOrCreateConstInt(llvm::APInt const&amp;, llvm::MachineInstr&amp;, llvm::SPIRVTypeInst, llvm::SPIRVInstrInfo const&amp;, bool) /llvm/lib/Target/SPIRV/SPIRVGlobalRegistry.cpp:401:7
#<!-- -->14 0x000057cd6635a0c7 (anonymous namespace)::SPIRVInstructionSelector::selectConst(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3655:14
#<!-- -->15 0x000057cd6634c623 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:901:5
#<!-- -->16 0x000057cd6634ba2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&amp;) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#<!-- -->17 0x000057cd6aaef4c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#<!-- -->18 0x000057cd6aaee6d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#<!-- -->19 0x000057cd6aaee2a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#<!-- -->20 0x000057cd6b3880b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->21 0x000057cd71d941cf llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->22 0x000057cd71d9885b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->23 0x000057cd71d949f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->24 0x000057cd71d945bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->25 0x000057cd71d98b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->26 0x000057cd66251f48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->27 0x000057cd6625221f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->28 0x000057cd6624bb34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->29 0x000057cd6624b324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->30 0x000057cd6624ae6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->31 0x000057cd6624ac90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->32 0x000057cd70fe4324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->33 0x000057cd67182f4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->34 0x000057cd67182cbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->35 0x000057cd67183e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->36 0x000057cd72848fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->37 0x000057cd72848f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->38 0x000057cd63ec6189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->39 0x000057cd7284c44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->40 0x000057cd728403ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->41 0x000057cd72840b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->42 0x000057cd728463ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->43 0x000057cd72845976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->44 0x000057cd63faae7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->45 0x000057cd63faa551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->46 0x000057cd63faa20c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->47 0x000057cd63faa116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->48 0x000057cd72db26fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->49 0x000057cd72db1aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->50 0x000057cd63fa5dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->51 0x000057cd63fa61c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->52 0x000057cd63fa6398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->53 0x000057cd63e748d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->54 0x000077e699e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->55 0x000077e699e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->56 0x000077e699e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->57 0x000057cd63e74745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt unified_memoryStreams.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

