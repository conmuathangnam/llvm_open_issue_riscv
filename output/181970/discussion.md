# [MLIR] Crash in xegpu-propagate-layout pass due to invalid cast during GPU layout propagation

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/181970
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-03T11:14:56Z

## Body

Hi, I found a crash bug related to the lowering pass `--xegpu-propagate-layout`. See the following program.

```llvm
module {
  llvm.func @main() -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.constant(2 : i32) : i32
    %2 = llvm.add %0, %1 : i32
    llvm.return %2 : i32
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt common_error_checking.mlir --xegpu-propagate-layout
 #0 0x000055e5e41ed87d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x000055e5e41eddab PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x000055e5e41ebc64 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:108:5
 #3 0x000055e5e41ee449 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007f5ba3e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f5ba3e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f5ba3e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f5ba3e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f5ba3e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f5ba3e288ff abort ./stdlib/abort.c:81:7
#10 0x00007f5ba3e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f5ba3e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000055e5f1646d14 decltype(auto) llvm::cast<mlir::LLVM::LLVMFunctionType, mlir::Type>(mlir::Type const&) llvm/include/llvm/Support/Casting.h:561:43
#13 0x000055e5f126f01f mlir::LLVM::LLVMFuncOp::getFunctionType() build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:32360:10
#14 0x000055e5e6e742d5 mlir::LLVM::LLVMFuncOp::getArgumentTypes() build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.h.inc:18094:46
#15 0x000055e5f173ae66 mlir::detail::CallableOpInterfaceInterfaceTraits::Model<mlir::LLVM::LLVMFuncOp>::getArgumentTypes(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) build_all/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:529:56
#16 0x000055e5f28757dc mlir::FunctionOpInterface::getArgumentTypes() build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.cpp.inc:185:14
#17 0x000055e5e447df15 mlir::FunctionOpInterface::getNumArguments() build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:249:49
#18 0x000055e5f2877827 mlir::function_interface_impl::setFunctionType(mlir::FunctionOpInterface, mlir::Type) mlir/lib/Interfaces/FunctionInterfaces.cpp:337:12
#19 0x000055e5e50343ec mlir::FunctionOpInterface::setType(mlir::Type) build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:242:5
#20 0x000055e5ec3d27e1 updateFunctionOpInterface(mlir::OpBuilder&, mlir::FunctionOpInterface, llvm::function_ref<mlir::xegpu::DistributeLayoutAttr (mlir::Value)>) mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1707:10
#21 0x000055e5ec3d2471 mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)::operator()(mlir::FunctionOpInterface) const mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1753:17
#22 0x000055e5ec3d23e9 llvm::TypeSwitch<mlir::Operation*, void>& llvm::TypeSwitch<mlir::Operation*, void>::Case<mlir::FunctionOpInterface, mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)>(mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&&) llvm/include/llvm/ADT/TypeSwitch.h:183:7
#23 0x000055e5ec3d1b55 llvm::TypeSwitch<mlir::Operation*, void>& llvm::detail::TypeSwitchBase<llvm::TypeSwitch<mlir::Operation*, void>, mlir::Operation*>::Case<mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)>(mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&&) llvm/include/llvm/ADT/TypeSwitch.h:62:5
#24 0x000055e5ec3d1a2f mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1752:12
#25 0x000055e5ec3d191d mlir::WalkResult llvm::function_ref<mlir::WalkResult (mlir::Block*)>::callback_fn<mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0>(long, mlir::Block*) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#26 0x000055e5e505a271 llvm::function_ref<mlir::WalkResult (mlir::Block*)>::operator()(mlir::Block*) const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#27 0x000055e5e505a1f0 mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Block*)>, mlir::WalkOrder) mlir/include/mlir/IR/Visitors.h:211:13
#28 0x000055e5ec3d18c2 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, mlir::WalkResult>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0, mlir::Block*, mlir::WalkResult>(mlir::Operation*, mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&&) mlir/include/mlir/IR/Visitors.h:278:10
#29 0x000055e5ec3bf26d std::enable_if<llvm::function_traits<std::decay<mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0>::type>::num_args == 1, mlir::WalkResult>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0, mlir::WalkResult>(mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&&) mlir/include/mlir/IR/Operation.h:798:12
#30 0x000055e5ec3bee36 mlir::xegpu::propagateLayouts(mlir::OpBuilder&, mlir::Operation*, mlir::xegpu::LayoutKind, bool) mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1744:25
#31 0x000055e5ec3d02a5 (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation() mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1792:14
#32 0x000055e5f2666e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const mlir/lib/Pass/Pass.cpp:0:19
#33 0x000055e5f2666e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#34 0x000055e5e4213a19 llvm::function_ref<void ()>::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#35 0x000055e5f266a31b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) mlir/include/mlir/IR/MLIRContext.h:291:3
#36 0x000055e5f265e2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) mlir/lib/Pass/Pass.cpp:619:17
#37 0x000055e5f265e9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) mlir/lib/Pass/Pass.cpp:688:16
#38 0x000055e5f26642bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) mlir/lib/Pass/Pass.cpp:1123:10
#39 0x000055e5f2663846 mlir::PassManager::run(mlir::Operation*) mlir/lib/Pass/Pass.cpp:1097:60
#40 0x000055e5e42f7dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#41 0x000055e5e42f74d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#42 0x000055e5e42f718c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#43 0x000055e5e42f7096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#44 0x000055e5f2bc3c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#45 0x000055e5f2bc301a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) mlir/lib/Support/ToolUtilities.cpp:30:12
#46 0x000055e5e42f2e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#47 0x000055e5e42f3215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#48 0x000055e5e42f33e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#49 0x000055e5e41c2485 main mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#50 0x00007f5ba3e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#51 0x00007f5ba3e2a28b call_init ./csu/../csu/libc-start.c:128:20
#52 0x00007f5ba3e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#53 0x000055e5e41c22f5 _start (build_all/bin/mlir-opt+0x46a92f5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
Hi, I found a crash bug related to the lowering pass `--xegpu-propagate-layout`. See the following program.

```llvm
module {
  llvm.func @<!-- -->main() -&gt; i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.constant(2 : i32) : i32
    %2 = llvm.add %0, %1 : i32
    llvm.return %2 : i32
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt common_error_checking.mlir --xegpu-propagate-layout
 #<!-- -->0 0x000055e5e41ed87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x000055e5e41eddab PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x000055e5e41ebc64 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000055e5e41ee449 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007f5ba3e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f5ba3e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f5ba3e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f5ba3e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f5ba3e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f5ba3e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f5ba3e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f5ba3e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000055e5f1646d14 decltype(auto) llvm::cast&lt;mlir::LLVM::LLVMFunctionType, mlir::Type&gt;(mlir::Type const&amp;) llvm/include/llvm/Support/Casting.h:561:43
#<!-- -->13 0x000055e5f126f01f mlir::LLVM::LLVMFuncOp::getFunctionType() build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:32360:10
#<!-- -->14 0x000055e5e6e742d5 mlir::LLVM::LLVMFuncOp::getArgumentTypes() build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.h.inc:18094:46
#<!-- -->15 0x000055e5f173ae66 mlir::detail::CallableOpInterfaceInterfaceTraits::Model&lt;mlir::LLVM::LLVMFuncOp&gt;::getArgumentTypes(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) build_all/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:529:56
#<!-- -->16 0x000055e5f28757dc mlir::FunctionOpInterface::getArgumentTypes() build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.cpp.inc:185:14
#<!-- -->17 0x000055e5e447df15 mlir::FunctionOpInterface::getNumArguments() build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:249:49
#<!-- -->18 0x000055e5f2877827 mlir::function_interface_impl::setFunctionType(mlir::FunctionOpInterface, mlir::Type) mlir/lib/Interfaces/FunctionInterfaces.cpp:337:12
#<!-- -->19 0x000055e5e50343ec mlir::FunctionOpInterface::setType(mlir::Type) build_all/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:242:5
#<!-- -->20 0x000055e5ec3d27e1 updateFunctionOpInterface(mlir::OpBuilder&amp;, mlir::FunctionOpInterface, llvm::function_ref&lt;mlir::xegpu::DistributeLayoutAttr (mlir::Value)&gt;) mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1707:10
#<!-- -->21 0x000055e5ec3d2471 mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)::operator()(mlir::FunctionOpInterface) const mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1753:17
#<!-- -->22 0x000055e5ec3d23e9 llvm::TypeSwitch&lt;mlir::Operation*, void&gt;&amp; llvm::TypeSwitch&lt;mlir::Operation*, void&gt;::Case&lt;mlir::FunctionOpInterface, mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&gt;(mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&amp;&amp;) llvm/include/llvm/ADT/TypeSwitch.h:183:7
#<!-- -->23 0x000055e5ec3d1b55 llvm::TypeSwitch&lt;mlir::Operation*, void&gt;&amp; llvm::detail::TypeSwitchBase&lt;llvm::TypeSwitch&lt;mlir::Operation*, void&gt;, mlir::Operation*&gt;::Case&lt;mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&gt;(mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const::'lambda'(mlir::FunctionOpInterface)&amp;&amp;) llvm/include/llvm/ADT/TypeSwitch.h:62:5
#<!-- -->24 0x000055e5ec3d1a2f mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0::operator()(mlir::Block*) const mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1752:12
#<!-- -->25 0x000055e5ec3d191d mlir::WalkResult llvm::function_ref&lt;mlir::WalkResult (mlir::Block*)&gt;::callback_fn&lt;mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&gt;(long, mlir::Block*) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->26 0x000055e5e505a271 llvm::function_ref&lt;mlir::WalkResult (mlir::Block*)&gt;::operator()(mlir::Block*) const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->27 0x000055e5e505a1f0 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Block*)&gt;, mlir::WalkOrder) mlir/include/mlir/IR/Visitors.h:211:13
#<!-- -->28 0x000055e5ec3d18c2 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, mlir::WalkResult&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0, mlir::Block*, mlir::WalkResult&gt;(mlir::Operation*, mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&amp;&amp;) mlir/include/mlir/IR/Visitors.h:278:10
#<!-- -->29 0x000055e5ec3bf26d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&gt;::type&gt;::num_args == 1, mlir::WalkResult&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0, mlir::WalkResult&gt;(mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool)::$_0&amp;&amp;) mlir/include/mlir/IR/Operation.h:798:12
#<!-- -->30 0x000055e5ec3bee36 mlir::xegpu::propagateLayouts(mlir::OpBuilder&amp;, mlir::Operation*, mlir::xegpu::LayoutKind, bool) mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1744:25
#<!-- -->31 0x000055e5ec3d02a5 (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation() mlir/lib/Dialect/XeGPU/Transforms/XeGPUPropagateLayout.cpp:1792:14
#<!-- -->32 0x000055e5f2666e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->33 0x000055e5f2666e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x000055e5e4213a19 llvm::function_ref&lt;void ()&gt;::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x000055e5f266a31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x000055e5f265e2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->37 0x000055e5f265e9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->38 0x000055e5f26642bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->39 0x000055e5f2663846 mlir::PassManager::run(mlir::Operation*) mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->40 0x000055e5e42f7dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->41 0x000055e5e42f74d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->42 0x000055e5e42f718c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->43 0x000055e5e42f7096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->44 0x000055e5f2bc3c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->45 0x000055e5f2bc301a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->46 0x000055e5e42f2e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->47 0x000055e5e42f3215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->48 0x000055e5e42f33e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->49 0x000055e5e41c2485 main mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->50 0x00007f5ba3e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x00007f5ba3e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->52 0x00007f5ba3e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->53 0x000055e5e41c22f5 _start (build_all/bin/mlir-opt+0x46a92f5)
```
</details>


---

