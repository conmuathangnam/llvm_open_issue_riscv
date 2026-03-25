# mlir-opt crash: tensor.from_elements folder fails on nested vector types during --sccp

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180459
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2026-02-27T15:31:30Z

## Body

**Bug Op**: tensor.from_elements

**Bug Pass**: --sccp

**Detailed Description**: The folder for tensor.from_elements crashes when operands are constants of vector type. It attempts to fold the operation into a DenseElementsAttr, but DenseElementsAttr::get does not support direct nesting of non-basic types (like vectors) without specific attribute handling. This triggers the assertion: expected string value for non integer/index/float element.

```mlir
module {
  func.func @main() {
    %1 = vector.constant_mask [1] : vector<1xi1>
    %2 = tensor.from_elements %1 : tensor<1xvector<1xi1>>
    return
  }
}
```
```
mlir-opt --sccp temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:955: static mlir::DenseElementsAttr mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef<mlir::Attribute>): Assertion `llvm::isa<StringAttr>(attr) && "expected string value for non integer/index/float element"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --sccp temp.mlir
 #0 0x000062c046147e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000062c0461445af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000062c0461446fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000784e4ee45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000784e4ee9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000784e4ee9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000784e4ee9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000784e4ee4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000784e4ee288ff abort ./stdlib/abort.c:81:7
 #9 0x0000784e4ee2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000784e4ee3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000062c04dcc24dd mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef<mlir::Attribute>) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0aa4dd)
#12 0x000062c04d58920e mlir::tensor::FromElementsOp::fold(mlir::tensor::FromElementsOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x997120e)
#13 0x000062c04d535ab2 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::tensor::FromElementsOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::RankedTensorType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::OpAsmOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x991dab2)
#14 0x000062c04d542071 mlir::RegisteredOperationName::Model<mlir::tensor::FromElementsOp>::foldHook(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x992a071)
#15 0x000062c04dd5d074 mlir::Operation::fold(llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0xa145074)
#16 0x000062c04db2d62a mlir::dataflow::SparseConstantPropagation::visitOperation(mlir::Operation*, llvm::ArrayRef<mlir::dataflow::Lattice<mlir::dataflow::ConstantValue> const*>, llvm::ArrayRef<mlir::dataflow::Lattice<mlir::dataflow::ConstantValue>*>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f1562a)
#17 0x000062c04db5c450 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitOperation(mlir::Operation*) (.part.0) SparseAnalysis.cpp:0:0
#18 0x000062c04db611bb mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f491bb)
#19 0x000062c04db61833 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f49833)
#20 0x000062c04db61833 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f49833)
#21 0x000062c04dadd0fd mlir::DataFlowSolver::initializeAndRun(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9ec50fd)
#22 0x000062c04d8c2657 (anonymous namespace)::SCCP::runOnOperation() SCCP.cpp:0:0
#23 0x000062c04d9e7756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#24 0x000062c04d9e7a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#25 0x000062c04d9e81e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#26 0x000062c04d9e9400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#27 0x000062c04620e767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#28 0x000062c04620f4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x000062c04ddb7731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#30 0x000062c0462068d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#31 0x000062c04620fb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#32 0x000062c04620fda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#33 0x000062c046041023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#34 0x0000784e4ee2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x0000784e4ee2a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x0000784e4ee2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x000062c046126405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

