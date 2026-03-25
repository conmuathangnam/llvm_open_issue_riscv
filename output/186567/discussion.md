# [MLIR] Global is referenced by parentless instruction when lowering to XeVM gpu

**Author:** karlls12321
**URL:** https://github.com/llvm/llvm-project/issues/186567
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I found a crash in the XeVM gpu lowering pipeline. The following program reproduces the issue:

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
mlir-opt main.mlir --gpu-lower-to-xevm-pipeline
```

#### Stack Trace
```bash
[xegpu-optimize-peephole]: XeGPUPeepHoleOptimizerPass only supports PVC and BMG targets.
Global is referenced by parentless instruction!
ptr @_Z7barrierj
; ModuleID = 'LLVMDialectModule'
  call spir_func addrspace(0) void @_Z7barrierj(i32 3) #0
While deleting: ptr %_Z7barrierj
Use still stuck around after Def is destroyed:  call spir_func addrspace(0) void @_Z7barrierj(i32 3) #-1
Uses remain when a value is destroyed!
UNREACHABLE executed at /llvm/lib/IR/Value.cpp:102!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt main.mlir --gpu-lower-to-xevm-pipeline
 #0 0x0000652f22b1adbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x0000652f22b1b2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000652f22b19184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000652f22b1ba89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000733d39e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000733d39e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000733d39e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000733d39e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000733d39e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000733d39e288ff abort ./stdlib/abort.c:81:7
#10 0x0000652f22af3ed8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#11 0x0000652f30abc6b7 llvm::Value::~Value() /llvm/lib/IR/Value.cpp:0:5
#12 0x0000652f30abbba8 llvm::User::~User() /llvm/lib/IR/User.cpp:232:1
#13 0x0000652f28dbe905 llvm::Constant::~Constant() /llvm/include/llvm/IR/Constant.h:48:23
#14 0x0000652f28dbe8e2 llvm::GlobalValue::~GlobalValue() /llvm/include/llvm/IR/GlobalValue.h:194:3
#15 0x0000652f30990ee6 llvm::GlobalObject::~GlobalObject() /llvm/lib/IR/Globals.cpp:106:53
#16 0x0000652f3098b652 llvm::Function::~Function() /llvm/lib/IR/Function.cpp:530:1
#17 0x0000652f28beada3 llvm::ilist_alloc_traits<llvm::Function>::deleteNode(llvm::Function*) /llvm/include/llvm/ADT/ilist.h:42:39
#18 0x0000652f28bead25 llvm::iplist_impl<llvm::simple_ilist<llvm::Function>, llvm::SymbolTableListTraits<llvm::Function>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>) /llvm/include/llvm/ADT/ilist.h:206:12
#19 0x0000652f30a67d04 llvm::iplist_impl<llvm::simple_ilist<llvm::Function>, llvm::SymbolTableListTraits<llvm::Function>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>, llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>) /llvm/include/llvm/ADT/ilist.h:242:15
#20 0x0000652f30a6546b llvm::iplist_impl<llvm::simple_ilist<llvm::Function>, llvm::SymbolTableListTraits<llvm::Function>>::clear() /llvm/include/llvm/ADT/ilist.h:246:18
#21 0x0000652f30a6069f llvm::Module::~Module() /llvm/lib/IR/Module.cpp:122:16
#22 0x0000652f24ec9267 std::default_delete<llvm::Module>::operator()(llvm::Module*) const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:99:2
#23 0x0000652f24ec8c8f std::unique_ptr<llvm::Module, std::default_delete<llvm::Module>>::~unique_ptr() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:405:2
#24 0x0000652f2bfa6f30 mlir::LLVM::ModuleTranslation::~ModuleTranslation() /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:779:1
#25 0x0000652f2bfb1e66 mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&, llvm::StringRef, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:2549:1
#26 0x0000652f289d0130 mlir::LLVM::ModuleToObject::translateToLLVMIR(llvm::LLVMContext&) /mlir/lib/Target/LLVM/ModuleToObject.cpp:145:10
#27 0x0000652f24ec60cc (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:373:8
#28 0x0000652f24ec5e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#29 0x0000652f24ec5c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::XeVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#30 0x0000652f2fc5f324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#31 0x0000652f25dfdf4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#32 0x0000652f25dfdcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#33 0x0000652f25dfee45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#34 0x0000652f314c3fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#35 0x0000652f314c3f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#36 0x0000652f22b41189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#37 0x0000652f314c744b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#38 0x0000652f314bb3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#39 0x0000652f314bbb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#40 0x0000652f314c13ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#41 0x0000652f314c0976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#42 0x0000652f22c25e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#43 0x0000652f22c25551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#44 0x0000652f22c2520c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#45 0x0000652f22c25116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#46 0x0000652f31a2d6fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#47 0x0000652f31a2caba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#48 0x0000652f22c20dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#49 0x0000652f22c211c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#50 0x0000652f22c21398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#51 0x0000652f22aef8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#52 0x0000733d39e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#53 0x0000733d39e2a28b call_init ./csu/../csu/libc-start.c:128:20
#54 0x0000733d39e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#55 0x0000652f22aef745 _start (./mlir-opt+0x481e745)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: bling (karlls12321)

<details>
Hi, I found a crash in the XeVM gpu lowering pipeline. The following program reproduces the issue:

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
mlir-opt main.mlir --gpu-lower-to-xevm-pipeline
```

#### Stack Trace
```bash
[xegpu-optimize-peephole]: XeGPUPeepHoleOptimizerPass only supports PVC and BMG targets.
Global is referenced by parentless instruction!
ptr @<!-- -->_Z7barrierj
; ModuleID = 'LLVMDialectModule'
  call spir_func addrspace(0) void @<!-- -->_Z7barrierj(i32 3) #<!-- -->0
While deleting: ptr %_Z7barrierj
Use still stuck around after Def is destroyed:  call spir_func addrspace(0) void @<!-- -->_Z7barrierj(i32 3) #-1
Uses remain when a value is destroyed!
UNREACHABLE executed at /llvm/lib/IR/Value.cpp:102!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt main.mlir --gpu-lower-to-xevm-pipeline
 #<!-- -->0 0x0000652f22b1adbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x0000652f22b1b2eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x0000652f22b19184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000652f22b1ba89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000733d39e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000733d39e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000733d39e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000733d39e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000733d39e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000733d39e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000652f22af3ed8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#<!-- -->11 0x0000652f30abc6b7 llvm::Value::~Value() /llvm/lib/IR/Value.cpp:0:5
#<!-- -->12 0x0000652f30abbba8 llvm::User::~User() /llvm/lib/IR/User.cpp:232:1
#<!-- -->13 0x0000652f28dbe905 llvm::Constant::~Constant() /llvm/include/llvm/IR/Constant.h:48:23
#<!-- -->14 0x0000652f28dbe8e2 llvm::GlobalValue::~GlobalValue() /llvm/include/llvm/IR/GlobalValue.h:194:3
#<!-- -->15 0x0000652f30990ee6 llvm::GlobalObject::~GlobalObject() /llvm/lib/IR/Globals.cpp:106:53
#<!-- -->16 0x0000652f3098b652 llvm::Function::~Function() /llvm/lib/IR/Function.cpp:530:1
#<!-- -->17 0x0000652f28beada3 llvm::ilist_alloc_traits&lt;llvm::Function&gt;::deleteNode(llvm::Function*) /llvm/include/llvm/ADT/ilist.h:42:39
#<!-- -->18 0x0000652f28bead25 llvm::iplist_impl&lt;llvm::simple_ilist&lt;llvm::Function&gt;, llvm::SymbolTableListTraits&lt;llvm::Function&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;) /llvm/include/llvm/ADT/ilist.h:206:12
#<!-- -->19 0x0000652f30a67d04 llvm::iplist_impl&lt;llvm::simple_ilist&lt;llvm::Function&gt;, llvm::SymbolTableListTraits&lt;llvm::Function&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;, llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;) /llvm/include/llvm/ADT/ilist.h:242:15
#<!-- -->20 0x0000652f30a6546b llvm::iplist_impl&lt;llvm::simple_ilist&lt;llvm::Function&gt;, llvm::SymbolTableListTraits&lt;llvm::Function&gt;&gt;::clear() /llvm/include/llvm/ADT/ilist.h:246:18
#<!-- -->21 0x0000652f30a6069f llvm::Module::~Module() /llvm/lib/IR/Module.cpp:122:16
#<!-- -->22 0x0000652f24ec9267 std::default_delete&lt;llvm::Module&gt;::operator()(llvm::Module*) const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:99:2
#<!-- -->23 0x0000652f24ec8c8f std::unique_ptr&lt;llvm::Module, std::default_delete&lt;llvm::Module&gt;&gt;::~unique_ptr() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:405:2
#<!-- -->24 0x0000652f2bfa6f30 mlir::LLVM::ModuleTranslation::~ModuleTranslation() /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:779:1
#<!-- -->25 0x0000652f2bfb1e66 mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&amp;, llvm::StringRef, bool) /mlir/lib/Target/LLVMIR/ModuleTranslation.cpp:2549:1
#<!-- -->26 0x0000652f289d0130 mlir::LLVM::ModuleToObject::translateToLLVMIR(llvm::LLVMContext&amp;) /mlir/lib/Target/LLVM/ModuleToObject.cpp:145:10
#<!-- -->27 0x0000652f24ec60cc (anonymous namespace)::SPIRVSerializer::run() /mlir/lib/Target/LLVM/XeVM/Target.cpp:373:8
#<!-- -->28 0x0000652f24ec5e6e (anonymous namespace)::XeVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /mlir/lib/Target/LLVM/XeVM/Target.cpp:449:10
#<!-- -->29 0x0000652f24ec5c90 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::XeVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:249:51
#<!-- -->30 0x0000652f2fc5f324 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /build_new/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->31 0x0000652f25dfdf4c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:97:10
#<!-- -->32 0x0000652f25dfdcbd mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:130:20
#<!-- -->33 0x0000652f25dfee45 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:74:14
#<!-- -->34 0x0000652f314c3fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->35 0x0000652f314c3f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->36 0x0000652f22b41189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->37 0x0000652f314c744b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->38 0x0000652f314bb3ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->39 0x0000652f314bbb0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->40 0x0000652f314c13ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->41 0x0000652f314c0976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->42 0x0000652f22c25e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->43 0x0000652f22c25551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->44 0x0000652f22c2520c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->45 0x0000652f22c25116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x0000652f31a2d6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->47 0x0000652f31a2caba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->48 0x0000652f22c20dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->49 0x0000652f22c211c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->50 0x0000652f22c21398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->51 0x0000652f22aef8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->52 0x0000733d39e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->53 0x0000733d39e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->54 0x0000733d39e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->55 0x0000652f22aef745 _start (./mlir-opt+0x481e745)
Aborted
```
</details>


---

