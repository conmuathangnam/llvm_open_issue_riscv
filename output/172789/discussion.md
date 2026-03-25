# [MLIR] MLIR crash while attempting to select an intrinsic

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/172789
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-01T10:44:59Z

## Body

HI, I found a crash bug when using MLIR to lower the input program. Please see the minimized code example below:

```llvm
gpu.module @multithreading_cluster {
  gpu.func @incKernel_cluster(%data: memref<?xi32>, %N: i32) kernel {
    %tid = gpu.thread_id x
    %bid = gpu.block_id x
    %cid = gpu.cluster_id x
    
    %c8_index = arith.constant 8 : index
    %c512_index = arith.constant 512 : index
    %c1_i32 = arith.constant 1 : i32
    
    %cluster_times_8 = arith.muli %cid, %c8_index : index
    %total_block_id = arith.addi %cluster_times_8, %bid : index
    %block_times_512 = arith.muli %total_block_id, %c512_index : index
    %idx = arith.addi %block_times_512, %tid : index
    
    %idx_as_i32 = arith.index_cast %idx : index to i32
    %is_valid = arith.cmpi slt, %idx_as_i32, %N : i32
    
    scf.if %is_valid {
      %current_value = memref.load %data[%idx] : memref<?xi32>
      %incremented = arith.addi %current_value, %c1_i32 : i32
      memref.store %incremented, %data[%idx] : memref<?xi32>
    }
    
    gpu.return
  }
}
```

### Used Command

I compiled it with the command below, which produced the crash log.

```bash
mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
```

### Crash Log
```bash
LLVM ERROR: Cannot select: intrinsic %llvm.nvvm.read.ptx.sreg.clusterid.x
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/username/projects/llvm-project/build_multi_backends/bin/mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
1.      Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.      Running pass 'NVPTX DAG->DAG Pattern Instruction Selection' on function '@incKernel_cluster'
 #0 0x00005583d5e72cb1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00005583d5e7332d PrintStackTraceSignalHandler(void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005583d5e70b45 llvm::sys::RunSignalHandlers() /home/username/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005583d5e73bf1 SignalHandler(int, siginfo_t*, void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f9ee5c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)

AGISel.cpp:4474:3
#12 0x00005583dd9e1f6c llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4375:9
#13 0x00005583d9d49b9b llvm::NVPTXDAGToDAGISel::SelectCode(llvm::SDNode*) /home/username/projects/llvm-project/build_multi_backends/lib/Target/NVPTX/NVPTXGenDAGISel.inc:70694:1
#14 0x00005583d9d3bf8a llvm::NVPTXDAGToDAGISel::Select(llvm::SDNode*) /home/username/projects/llvm-project/llvm/lib/Target/NVPTX/NVPTXISelDAGToDAG.cpp:196:1
#15 0x00005583dd9cfae4 llvm::SelectionDAGISel::DoInstructionSelection() /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1366:5
#16 0x00005583dd9ce864 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1135:3
#17 0x00005583dd9cca71 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:890:1
#18 0x00005583dd9cc555 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1922:11
#19 0x00005583dd9c89f0 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:627:3
#20 0x00005583d9d3b5c0 llvm::NVPTXDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/username/projects/llvm-project/llvm/lib/Target/NVPTX/NVPTXISelDAGToDAG.cpp:71:3
#21 0x00005583dd9c73c4 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:390:3
#22 0x00005583ddfbd6ea llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /home/username/projects/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:8
#23 0x00005583e5970a37 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#24 0x00005583e597618f llvm::FPPassManager::runOnModule(llvm::Module&) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#25 0x00005583e5971423 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#26 0x00005583e5970f25 llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#27 0x00005583e59764b3 llvm::legacy::PassManager::run(llvm::Module&) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#28 0x00005583dc58888a mlir::LLVM::ModuleToObject::translateToISA[abi:cxx11](llvm::Module&, llvm::TargetMachine&) /home/username/projects/llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:223:3
#29 0x00005583d9c13de4 (anonymous namespace)::NVPTXSerializer::moduleToObject(llvm::Module&) /home/username/projects/llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:702:23
#30 0x00005583dc588e6a mlir::LLVM::Mod
namespace)::NVVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /home/username/projects/llvm-project/build_multi_backends/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:230:51
#33 0x00005583e48ad4e6 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /home/username/projects/llvm-project/build_multi_backends/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#34 0x00005583d9936216 (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:92:10
#35 0x00005583d9935ec5 mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:125:20
#36 0x00005583d99371a7 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:69:14
#37 0x00005583e6660128 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#38 0x00005583e66600a7 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#39 0x00005583d5ea243b llvm::function_ref<void ()>::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#40 0x00005583e6663d2f void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#41 0x00005583e66552c2 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#42 0x00005583e6655bd6 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#43 0x00005583e665cd3d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#44 0x00005583e665be37 mlir::PassManager::run(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#45 0x00005583d5fb06c6 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#46 0x00005583d5fafc25 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#47 0x00005583d5faf850 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#48 0x00005583d5faf738 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#49 0x00005583e6d02ccc llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#50 0x00005583e6d01f0e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /home/username/projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#51 0x00005583d5faacfe mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#52 0x00005583d5fab1f5 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#53 0x00005583d5fab3ea mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#54 0x00005583d5e3cc07 main /home/username/projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#55 0x00007f9ee5c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#56 0x00007f9ee5c2a28b call_init ./csu/../csu/libc-start.c:128:20
#57 0x00007f9ee5c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#58 0x00005583d5e3c7a5 _start (/home/username/projects/llvm-project/build_multi_backends/bin/mlir-opt+0x3fd17a5)
Aborted
```

### Version
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
HI, I found a crash bug when using MLIR to lower the input program. Please see the minimized code example below:

```llvm
gpu.module @<!-- -->multithreading_cluster {
  gpu.func @<!-- -->incKernel_cluster(%data: memref&lt;?xi32&gt;, %N: i32) kernel {
    %tid = gpu.thread_id x
    %bid = gpu.block_id x
    %cid = gpu.cluster_id x
    
    %c8_index = arith.constant 8 : index
    %c512_index = arith.constant 512 : index
    %c1_i32 = arith.constant 1 : i32
    
    %cluster_times_8 = arith.muli %cid, %c8_index : index
    %total_block_id = arith.addi %cluster_times_8, %bid : index
    %block_times_512 = arith.muli %total_block_id, %c512_index : index
    %idx = arith.addi %block_times_512, %tid : index
    
    %idx_as_i32 = arith.index_cast %idx : index to i32
    %is_valid = arith.cmpi slt, %idx_as_i32, %N : i32
    
    scf.if %is_valid {
      %current_value = memref.load %data[%idx] : memref&lt;?xi32&gt;
      %incremented = arith.addi %current_value, %c1_i32 : i32
      memref.store %incremented, %data[%idx] : memref&lt;?xi32&gt;
    }
    
    gpu.return
  }
}
```

### Used Command

I compiled it with the command below, which produced the crash log.

```bash
mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
```

### Crash Log
```bash
LLVM ERROR: Cannot select: intrinsic %llvm.nvvm.read.ptx.sreg.clusterid.x
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/username/projects/llvm-project/build_multi_backends/bin/mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
1.      Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.      Running pass 'NVPTX DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->incKernel_cluster'
 #<!-- -->0 0x00005583d5e72cb1 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x00005583d5e7332d PrintStackTraceSignalHandler(void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x00005583d5e70b45 llvm::sys::RunSignalHandlers() /home/username/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005583d5e73bf1 SignalHandler(int, siginfo_t*, void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f9ee5c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)

AGISel.cpp:4474:3
#<!-- -->12 0x00005583dd9e1f6c llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:4375:9
#<!-- -->13 0x00005583d9d49b9b llvm::NVPTXDAGToDAGISel::SelectCode(llvm::SDNode*) /home/username/projects/llvm-project/build_multi_backends/lib/Target/NVPTX/NVPTXGenDAGISel.inc:70694:1
#<!-- -->14 0x00005583d9d3bf8a llvm::NVPTXDAGToDAGISel::Select(llvm::SDNode*) /home/username/projects/llvm-project/llvm/lib/Target/NVPTX/NVPTXISelDAGToDAG.cpp:196:1
#<!-- -->15 0x00005583dd9cfae4 llvm::SelectionDAGISel::DoInstructionSelection() /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1366:5
#<!-- -->16 0x00005583dd9ce864 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1135:3
#<!-- -->17 0x00005583dd9cca71 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:890:1
#<!-- -->18 0x00005583dd9cc555 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1922:11
#<!-- -->19 0x00005583dd9c89f0 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:627:3
#<!-- -->20 0x00005583d9d3b5c0 llvm::NVPTXDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/username/projects/llvm-project/llvm/lib/Target/NVPTX/NVPTXISelDAGToDAG.cpp:71:3
#<!-- -->21 0x00005583dd9c73c4 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/username/projects/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:390:3
#<!-- -->22 0x00005583ddfbd6ea llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/username/projects/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:8
#<!-- -->23 0x00005583e5970a37 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->24 0x00005583e597618f llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->25 0x00005583e5971423 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->26 0x00005583e5970f25 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->27 0x00005583e59764b3 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/username/projects/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->28 0x00005583dc58888a mlir::LLVM::ModuleToObject::translateToISA[abi:cxx11](llvm::Module&amp;, llvm::TargetMachine&amp;) /home/username/projects/llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:223:3
#<!-- -->29 0x00005583d9c13de4 (anonymous namespace)::NVPTXSerializer::moduleToObject(llvm::Module&amp;) /home/username/projects/llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:702:23
#<!-- -->30 0x00005583dc588e6a mlir::LLVM::Mod
namespace)::NVVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /home/username/projects/llvm-project/build_multi_backends/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:230:51
#<!-- -->33 0x00005583e48ad4e6 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /home/username/projects/llvm-project/build_multi_backends/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->34 0x00005583d9936216 (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:92:10
#<!-- -->35 0x00005583d9935ec5 mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:125:20
#<!-- -->36 0x00005583d99371a7 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /home/username/projects/llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:69:14
#<!-- -->37 0x00005583e6660128 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->38 0x00005583e66600a7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->39 0x00005583d5ea243b llvm::function_ref&lt;void ()&gt;::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->40 0x00005583e6663d2f void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->41 0x00005583e66552c2 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->42 0x00005583e6655bd6 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->43 0x00005583e665cd3d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->44 0x00005583e665be37 mlir::PassManager::run(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->45 0x00005583d5fb06c6 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->46 0x00005583d5fafc25 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->47 0x00005583d5faf850 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->48 0x00005583d5faf738 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->49 0x00005583e6d02ccc llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->50 0x00005583e6d01f0e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /home/username/projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->51 0x00005583d5faacfe mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->52 0x00005583d5fab1f5 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->53 0x00005583d5fab3ea mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->54 0x00005583d5e3cc07 main /home/username/projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->55 0x00007f9ee5c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->56 0x00007f9ee5c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->57 0x00007f9ee5c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->58 0x00005583d5e3c7a5 _start (/home/username/projects/llvm-project/build_multi_backends/bin/mlir-opt+0x3fd17a5)
Aborted
```

### Version
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```
</details>


---

### Comment 2 - actinks

The `'nvvm.read.ptx.sreg.clusterid.x'` operation is not supported on SM_75. It is only available starting from SM_90+ with PTX version 7.8 or higher.

---

### Comment 3 - joker-eph

HEAD is now correctly diagnosing:
```
error: 'nvvm.read.ptx.sreg.clusterid.x' op is not supported on sm_75
```

---

