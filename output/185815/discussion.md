# [MLIR] Crash in SPIRVInstructionSelector::selectIntrinsic during --gpu-lower-to-xevm-pipeline

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/185815
**Status:** Open
**Labels:** mlir, crash, backend:SPIR-V

## Body

Hi, I found a crash bug when lowering porgrams with the XeVM GPU pipeline. See the following code example for reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @gpu_module {
    func.func @kernelAddConstant(%arg0: memref<?xi32>, %arg1: i32) attributes {llvm.emit_c_interface} {
      %0 = gpu.global_id x
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %1 = affine.apply affine_map<(d0) -> (d0)>(%0)
      %2 = affine.apply affine_map<(d0) -> (d0 * 2)>(%0)
      %3 = vector.create_mask %1 : vector<8xi1>
      %splat = vector.broadcast %arg1 : i32 to vector<8xi32>
      %c0_i32 = arith.constant 0 : i32
      %load = vector.transfer_read %arg0[%c0], %c0_i32 {permutation_map = affine_map<(d0) -> (d0)>} : memref<?xi32>, vector<8xi32>
      %add = arith.addi %load, %splat : vector<8xi32>
      vector.transfer_write %add, %arg0[%2] {permutation_map = affine_map<(d0) -> (d0)>} : vector<8xi32>, memref<?xi32>
      return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@kernelAddConstant'
 #0 0x000062f7417f9dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000062f7417fa2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000062f7417f8184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000062f7417faa89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000072c293c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000072c293c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000072c293c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000072c293c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000072c293c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000072c293c288ff abort ./stdlib/abort.c:81:7
#10 0x000062f7417d2a85 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000062f7417d2902 /llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x000062f743cb5d4a (anonymous namespace)::SPIRVInstructionSelector::selectIntrinsic(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3895:29
#13 0x000062f743ca66e9 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:913:5
#14 0x000062f743ca5a2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#15 0x000062f7484494c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#16 0x000062f7484486d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#17 0x000062f7484482a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#18 0x000062f748ce20b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#19 0x000062f74f6ee1cf llvm::FPPassManager::runOnFunction(llvm::Function&) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#20 0x000062f74f6f285b llvm::FPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#21 0x000062f74f6ee9f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#22 0x000062f74f6ee5bf llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#23 0x000062f74f6f2b01 llvm::legacy::PassManager::run(llvm::Module&) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#24 0x000062f743babf48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#25 0x000062f743bac21f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#26 0x000062f743ba5b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#27 0x000062f743ba5324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#28 0x000062f743ba4e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#29 0x000062f743ba4c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#30 0x000062f74e93e324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#31 0x000062f744adcf4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#32 0x000062f744adccbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#33 0x000062f744adde45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#34 0x000062f7501a2fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#35 0x000062f7501a2f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#36 0x000062f741820189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#37 0x000062f7501a644b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#38 0x000062f75019a3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#39 0x000062f75019ab0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#40 0x000062f7501a03ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#41 0x000062f75019f976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#42 0x000062f741904e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#43 0x000062f741904551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#44 0x000062f74190420c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#45 0x000062f741904116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#46 0x000062f75070c6fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#47 0x000062f75070baba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#48 0x000062f7418ffdc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#49 0x000062f7419001c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#50 0x000062f741900398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#51 0x000062f7417ce8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#52 0x000072c293c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#53 0x000072c293c2a28b call_init ./csu/../csu/libc-start.c:128:20
#54 0x000072c293c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#55 0x000062f7417ce745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when lowering porgrams with the XeVM GPU pipeline. See the following code example for reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @<!-- -->gpu_module {
    func.func @<!-- -->kernelAddConstant(%arg0: memref&lt;?xi32&gt;, %arg1: i32) attributes {llvm.emit_c_interface} {
      %0 = gpu.global_id x
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %1 = affine.apply affine_map&lt;(d0) -&gt; (d0)&gt;(%0)
      %2 = affine.apply affine_map&lt;(d0) -&gt; (d0 * 2)&gt;(%0)
      %3 = vector.create_mask %1 : vector&lt;8xi1&gt;
      %splat = vector.broadcast %arg1 : i32 to vector&lt;8xi32&gt;
      %c0_i32 = arith.constant 0 : i32
      %load = vector.transfer_read %arg0[%c0], %c0_i32 {permutation_map = affine_map&lt;(d0) -&gt; (d0)&gt;} : memref&lt;?xi32&gt;, vector&lt;8xi32&gt;
      %add = arith.addi %load, %splat : vector&lt;8xi32&gt;
      vector.transfer_write %add, %arg0[%2] {permutation_map = affine_map&lt;(d0) -&gt; (d0)&gt;} : vector&lt;8xi32&gt;, memref&lt;?xi32&gt;
      return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@<!-- -->kernelAddConstant'
 #<!-- -->0 0x000062f7417f9dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000062f7417fa2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000062f7417f8184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000062f7417faa89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000072c293c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000072c293c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072c293c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072c293c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072c293c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072c293c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000062f7417d2a85 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000062f7417d2902 /llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x000062f743cb5d4a (anonymous namespace)::SPIRVInstructionSelector::selectIntrinsic(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3895:29
#<!-- -->13 0x000062f743ca66e9 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:913:5
#<!-- -->14 0x000062f743ca5a2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&amp;) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#<!-- -->15 0x000062f7484494c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#<!-- -->16 0x000062f7484486d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#<!-- -->17 0x000062f7484482a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#<!-- -->18 0x000062f748ce20b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->19 0x000062f74f6ee1cf llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->20 0x000062f74f6f285b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->21 0x000062f74f6ee9f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->22 0x000062f74f6ee5bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->23 0x000062f74f6f2b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->24 0x000062f743babf48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->25 0x000062f743bac21f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->26 0x000062f743ba5b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->27 0x000062f743ba5324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->28 0x000062f743ba4e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->29 0x000062f743ba4c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->30 0x000062f74e93e324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->31 0x000062f744adcf4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->32 0x000062f744adccbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->33 0x000062f744adde45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->34 0x000062f7501a2fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->35 0x000062f7501a2f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->36 0x000062f741820189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->37 0x000062f7501a644b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->38 0x000062f75019a3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->39 0x000062f75019ab0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->40 0x000062f7501a03ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->41 0x000062f75019f976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->42 0x000062f741904e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->43 0x000062f741904551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->44 0x000062f74190420c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->45 0x000062f741904116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x000062f75070c6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->47 0x000062f75070baba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->48 0x000062f7418ffdc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->49 0x000062f7419001c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->50 0x000062f741900398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->51 0x000062f7417ce8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->52 0x000072c293c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->53 0x000072c293c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->54 0x000072c293c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->55 0x000062f7417ce745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: PureLove77 (jiayiw2008)

<details>
Hi, I found a crash bug when lowering porgrams with the XeVM GPU pipeline. See the following code example for reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @<!-- -->gpu_module {
    func.func @<!-- -->kernelAddConstant(%arg0: memref&lt;?xi32&gt;, %arg1: i32) attributes {llvm.emit_c_interface} {
      %0 = gpu.global_id x
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %1 = affine.apply affine_map&lt;(d0) -&gt; (d0)&gt;(%0)
      %2 = affine.apply affine_map&lt;(d0) -&gt; (d0 * 2)&gt;(%0)
      %3 = vector.create_mask %1 : vector&lt;8xi1&gt;
      %splat = vector.broadcast %arg1 : i32 to vector&lt;8xi32&gt;
      %c0_i32 = arith.constant 0 : i32
      %load = vector.transfer_read %arg0[%c0], %c0_i32 {permutation_map = affine_map&lt;(d0) -&gt; (d0)&gt;} : memref&lt;?xi32&gt;, vector&lt;8xi32&gt;
      %add = arith.addi %load, %splat : vector&lt;8xi32&gt;
      vector.transfer_write %add, %arg0[%2] {permutation_map = affine_map&lt;(d0) -&gt; (d0)&gt;} : vector&lt;8xi32&gt;, memref&lt;?xi32&gt;
      return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
1.	Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.	Running pass 'InstructionSelect' on function '@<!-- -->kernelAddConstant'
 #<!-- -->0 0x000062f7417f9dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000062f7417fa2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000062f7417f8184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000062f7417faa89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000072c293c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000072c293c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072c293c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072c293c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072c293c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072c293c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000062f7417d2a85 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000062f7417d2902 /llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x000062f743cb5d4a (anonymous namespace)::SPIRVInstructionSelector::selectIntrinsic(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:3895:29
#<!-- -->13 0x000062f743ca66e9 (anonymous namespace)::SPIRVInstructionSelector::spvSelect(llvm::Register, llvm::SPIRVTypeInst, llvm::MachineInstr&amp;) const /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:913:5
#<!-- -->14 0x000062f743ca5a2f (anonymous namespace)::SPIRVInstructionSelector::select(llvm::MachineInstr&amp;) /llvm/lib/Target/SPIRV/SPIRVInstructionSelector.cpp:856:7
#<!-- -->15 0x000062f7484494c3 llvm::InstructionSelect::selectInstr(llvm::MachineInstr&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:384:3
#<!-- -->16 0x000062f7484486d7 llvm::InstructionSelect::selectMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:213:13
#<!-- -->17 0x000062f7484482a4 llvm::InstructionSelect::runOnMachineFunction(llvm::MachineFunction&amp;) /llvm/lib/CodeGen/GlobalISel/InstructionSelect.cpp:154:3
#<!-- -->18 0x000062f748ce20b2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /llvm/lib/CodeGen/MachineFunctionPass.cpp:112:8
#<!-- -->19 0x000062f74f6ee1cf llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->20 0x000062f74f6f285b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->21 0x000062f74f6ee9f9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->22 0x000062f74f6ee5bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->23 0x000062f74f6f2b01 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->24 0x000062f743babf48 SPIRVTranslate /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:131:24
#<!-- -->25 0x000062f743bac21f SPIRVTranslateModule /llvm/lib/Target/SPIRV/SPIRVAPI.cpp:158:3
#<!-- -->26 0x000062f743ba5b34 (anonymous namespace)::SPIRVSerializer::moduleToObject(llvm::Module&amp;) /mlir/lib/Target/LLVM/XeVM/Target.cpp:352:7
#<!-- -->27 0x000062f743ba5324 (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:398:10
#<!-- -->28 0x000062f743ba4e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->29 0x000062f743ba4c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->30 0x000062f74e93e324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->31 0x000062f744adcf4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->32 0x000062f744adccbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->33 0x000062f744adde45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->34 0x000062f7501a2fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->35 0x000062f7501a2f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->36 0x000062f741820189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->37 0x000062f7501a644b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->38 0x000062f75019a3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->39 0x000062f75019ab0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->40 0x000062f7501a03ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->41 0x000062f75019f976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->42 0x000062f741904e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->43 0x000062f741904551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->44 0x000062f74190420c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->45 0x000062f741904116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x000062f75070c6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->47 0x000062f75070baba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->48 0x000062f7418ffdc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->49 0x000062f7419001c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->50 0x000062f741900398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->51 0x000062f7417ce8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->52 0x000072c293c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->53 0x000072c293c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->54 0x000072c293c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->55 0x000062f7417ce745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt cudaOpenMP.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

### Comment 3 - woruyu

I would like to fix it!

---

### Comment 4 - jmmartinez

Hello, 
Thanks for the issue!

Please add the `backend:SPIR-V` label when you see a crash is happening on anything under `$LLVM_SRC_DIR/llvm/lib/Target/SPIRV/*`. That would give the issue more visibility for other SPIRV backend devs.

---

