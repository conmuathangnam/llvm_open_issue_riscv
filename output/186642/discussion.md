# [MLIR] Segmentation fault in NVVM to LLVM IR translation when creating intrinsic

**Author:** karlls12321
**URL:** https://github.com/llvm/llvm-project/issues/186642
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I found a segmentation fault occurs during NVVM to LLVM IR translation when creating an intrinsic call. See the following code example for reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @kernels {
    gpu.barrier
  }

  func.func @main() -> i32 {
    %mem = memref.alloca() : memref<10xf32>
    %c0_index = arith.constant 0 : index
    %cst = arith.constant 3.140000e+00 : f32
    memref.store %cst, %mem[%c0_index] : memref<10xf32>
    %loaded = memref.load %mem[%c0_index] : memref<10xf32>
    %cast = arith.fptosi %loaded : f32 to i32
    memref.dealloc %mem : memref<10xf32>
    return %cast : i32
  }
}
```

#### Reproduction Command
```bash
mlir-opt main.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt main.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
 #0 0x00006166ee76cdbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00006166ee76d2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00006166ee76b184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00006166ee76da89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000768622a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00006166f041c94c llvm::BasicBlock::getParent() const /llvm/include/llvm/IR/BasicBlock.h:213:46
 #6 0x00006166fc498695 llvm::BasicBlock::getModule() const /llvm/lib/IR/BasicBlock.cpp:249:10
 #7 0x00006166f0c5ed65 llvm::BasicBlock::getModule() /llvm/include/llvm/IR/BasicBlock.h:222:5
 #8 0x00006166fc5fdcf2 llvm::IRBuilderBase::CreateIntrinsic(unsigned int, llvm::ArrayRef<llvm::Type*>, llvm::ArrayRef<llvm::Value*>, llvm::FMFSource, llvm::Twine const&) /llvm/lib/IR/IRBuilder.cpp:934:11
 #9 0x00006166f7bf9e62 mlir::LLVM::detail::createIntrinsicCall(llvm::IRBuilderBase&, unsigned int, llvm::ArrayRef<llvm::Value*>, llvm::ArrayRef<llvm::Type*>) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:902:3
#10 0x00006166f1d4c735 (anonymous namespace)::NVVMDialectLLVMIRTranslationInterface::convertOperation(mlir::Operation*, llvm::IRBuilderBase&, mlir::LLVM::ModuleTranslation&) const /build_new/tools/mlir/include/mlir/Dialect/LLVMIR/NVVMConversions.inc:21:10
#11 0x00006166f7bfac8c mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&, llvm::IRBuilderBase&, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:1005:23
#12 0x00006166f7c03cb5 mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&, llvm::StringRef, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:2520:27
#13 0x00006166f4622130 mlir::LLVM::ModuleToObject::translateToLLVMIR(llvm::LLVMContext&) /mlir/lib/Target/LLVM/ModuleToObject.cpp:145:10
#14 0x00006166f46229cc mlir::LLVM::ModuleToObject::run() /mlir/lib/Target/LLVM/ModuleToObject.cpp:249:8
#15 0x00006166f1d47816 (anonymous namespace)::NVVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/NVVM/Target.cpp:766:8
#16 0x00006166f1d47660 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::NVVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#17 0x00006166fb8b1324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#18 0x00006166f1a4ff4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#19 0x00006166f1a4fcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#20 0x00006166f1a50e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#21 0x00006166fd115fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#22 0x00006166fd115f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#23 0x00006166ee793189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#24 0x00006166fd11944b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#25 0x00006166fd10d3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#26 0x00006166fd10db0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#27 0x00006166fd1133ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#28 0x00006166fd112976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#29 0x00006166ee877e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#30 0x00006166ee877551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#31 0x00006166ee87720c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#32 0x00006166ee877116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#33 0x00006166fd67f6fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#34 0x00006166fd67eaba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#35 0x00006166ee872dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#36 0x00006166ee8731c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#37 0x00006166ee873398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#38 0x00006166ee7418d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#39 0x0000768622a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#40 0x0000768622a2a28b call_init ./csu/../csu/libc-start.c:128:20
#41 0x0000768622a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#42 0x00006166ee741745 _start (./mlir-opt+0x481e745)
Segmentation fault
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: bling (karlls12321)

<details>
Hi, I found a segmentation fault occurs during NVVM to LLVM IR translation when creating an intrinsic call. See the following code example for reproduction.

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    gpu.barrier
  }

  func.func @<!-- -->main() -&gt; i32 {
    %mem = memref.alloca() : memref&lt;10xf32&gt;
    %c0_index = arith.constant 0 : index
    %cst = arith.constant 3.140000e+00 : f32
    memref.store %cst, %mem[%c0_index] : memref&lt;10xf32&gt;
    %loaded = memref.load %mem[%c0_index] : memref&lt;10xf32&gt;
    %cast = arith.fptosi %loaded : f32 to i32
    memref.dealloc %mem : memref&lt;10xf32&gt;
    return %cast : i32
  }
}
```

#### Reproduction Command
```bash
mlir-opt main.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt main.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
 #<!-- -->0 0x00006166ee76cdbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00006166ee76d2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00006166ee76b184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006166ee76da89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000768622a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00006166f041c94c llvm::BasicBlock::getParent() const /llvm/include/llvm/IR/BasicBlock.h:213:46
 #<!-- -->6 0x00006166fc498695 llvm::BasicBlock::getModule() const /llvm/lib/IR/BasicBlock.cpp:249:10
 #<!-- -->7 0x00006166f0c5ed65 llvm::BasicBlock::getModule() /llvm/include/llvm/IR/BasicBlock.h:222:5
 #<!-- -->8 0x00006166fc5fdcf2 llvm::IRBuilderBase::CreateIntrinsic(unsigned int, llvm::ArrayRef&lt;llvm::Type*&gt;, llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::FMFSource, llvm::Twine const&amp;) /llvm/lib/IR/IRBuilder.cpp:934:11
 #<!-- -->9 0x00006166f7bf9e62 mlir::LLVM::detail::createIntrinsicCall(llvm::IRBuilderBase&amp;, unsigned int, llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::ArrayRef&lt;llvm::Type*&gt;) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:902:3
#<!-- -->10 0x00006166f1d4c735 (anonymous namespace)::NVVMDialectLLVMIRTranslationInterface::convertOperation(mlir::Operation*, llvm::IRBuilderBase&amp;, mlir::LLVM::ModuleTranslation&amp;) const /build_new/tools/mlir/include/mlir/Dialect/LLVMIR/NVVMConversions.inc:21:10
#<!-- -->11 0x00006166f7bfac8c mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:1005:23
#<!-- -->12 0x00006166f7c03cb5 mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&amp;, llvm::StringRef, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:2520:27
#<!-- -->13 0x00006166f4622130 mlir::LLVM::ModuleToObject::translateToLLVMIR(llvm::LLVMContext&amp;) /mlir/lib/Target/LLVM/ModuleToObject.cpp:145:10
#<!-- -->14 0x00006166f46229cc mlir::LLVM::ModuleToObject::run() /mlir/lib/Target/LLVM/ModuleToObject.cpp:249:8
#<!-- -->15 0x00006166f1d47816 (anonymous namespace)::NVVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/NVVM/Target.cpp:766:8
#<!-- -->16 0x00006166f1d47660 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::NVVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->17 0x00006166fb8b1324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->18 0x00006166f1a4ff4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->19 0x00006166f1a4fcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->20 0x00006166f1a50e45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->21 0x00006166fd115fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->22 0x00006166fd115f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->23 0x00006166ee793189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->24 0x00006166fd11944b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->25 0x00006166fd10d3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->26 0x00006166fd10db0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->27 0x00006166fd1133ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->28 0x00006166fd112976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->29 0x00006166ee877e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->30 0x00006166ee877551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->31 0x00006166ee87720c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->32 0x00006166ee877116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->33 0x00006166fd67f6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->34 0x00006166fd67eaba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->35 0x00006166ee872dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->36 0x00006166ee8731c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->37 0x00006166ee873398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->38 0x00006166ee7418d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->39 0x0000768622a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->40 0x0000768622a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->41 0x0000768622a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->42 0x00006166ee741745 _start (./mlir-opt+0x481e745)
Segmentation fault
```

</details>


---

### Comment 2 - dwrank

I believe the problem is that with --gpu-module-to-binary, mlir::translateModuleToLLVMIR gets called on the gpu.module op, and reaches the for loop where it attempts to convert other top level operations.  When gpu.barrier is a top level operation, it passes the if condition and translator.convertOperation gets called with the child op (gpu.barrier) and llvmBuilder, whose BasicBlock pointer is null.  It eventually ends up in IRBuilderBase::CreateIntrinsic and crashes on the BasicBlock nullptr access.

When gpu.barrier is inside a gpu.func then the above is avoided by failing the if condition in the loop.

I don't see in the documentation on gpu.barrier for any ops that gpu.barrier needs to reside in.  According to mlir test files in mlir/tests, gpu.barrier resides in:
gpu.module > gpu.func > * > gpu.barrier
gpu.launch > * > gpu.barrier
func.func > * > gpu.barrier

So perhaps a simple verifier for gpu.barrier would not be acceptable.  gpu.barrier gets lowered to nvvm.barrier, so maybe a verifier can be added to nvvm.barrier.  I would assume that it needs to reside in an llvm.func, but I also do not see any requirement in the documentation of what it needs to reside in.

---

