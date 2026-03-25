# [MLIR] GPU dialect program crashes when calling get on the APInt

**Author:** kitty-1998
**URL:** https://github.com/llvm/llvm-project/issues/179604
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-07T01:34:23Z

## Body

During the lowering of a GPU dialect program to NVVM via MLIR, a crash occurs.

```llvm
gpu.module @vector_addition {
  gpu.func @kernel(%A: memref<?xf32>, %B: memref<?xf32>, %C: memref<?xf32>, %num_elements: i32) 
      kernel attributes {known_block_size = array<i32: 256, 1, 1>, known_grid_size = array<i32: -1, 1, 1>} {
    %tidx = gpu.thread_id x
    %bidx = gpu.block_id x
    %bdim = gpu.block_dim x
    %base = arith.muli %bidx, %bdim : index
    %linear_index = arith.addi %base, %tidx : index
    
    %i32_index = arith.index_cast %linear_index : index to i32
    %in_bounds = arith.cmpi ult, %i32_index, %num_elements : i32
    scf.if %in_bounds {
      %aval = memref.load %A[%linear_index] : memref<?xf32>
      %bval = memref.load %B[%linear_index] : memref<?xf32>
      %result = arith.addf %aval, %bval : f32
      memref.store %result, %C[%linear_index] : memref<?xf32>
    }
    gpu.return
  }
}
```

Executing `mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm` causes a crash; see the stack trace below.

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm
 #0 0x00006428e368d87d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00006428e368ddab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00006428e368bc64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00006428e368e449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x0000787ca8a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000787ca8a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000787ca8a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000787ca8a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000787ca8a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000787ca8a288ff abort ./stdlib/abort.c:81:7
#10 0x0000787ca8a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000787ca8a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006428e36f70b1 llvm::APInt::APInt(unsigned int, unsigned long, bool, bool) /llvm/include/llvm/ADT/APInt.h:0:11
#13 0x00006428f06128ab mlir::LLVM::ConstantRangeAttr::get(mlir::MLIRContext*, unsigned int, long, long) /build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOpsAttrDefs.cpp.inc:9771:61
#14 0x00006428e5fa8c5c mlir::gpu::index_lowering::OpLowering<mlir::gpu::BlockIdOp, mlir::NVVM::BlockIdXOp, mlir::NVVM::BlockIdYOp, mlir::NVVM::BlockIdZOp>::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpAdaptor, mlir::ConversionPatternRewriter&) const /mlir/lib/Conversion/GPUToNVVM/../GPUCommon/IndexIntrinsicsOpLowering.h:133:31
#15 0x00006428e5fa9223 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::gpu::BlockIdOp, false>, mlir::gpu::BlockIdOp>(mlir::ConvertOpToLLVMPattern<mlir::gpu::BlockIdOp, false> const&, mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#16 0x00006428e5fa8ef7 mlir::ConvertOpToLLVMPattern<mlir::gpu::BlockIdOp, false>::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:265:12
#17 0x00006428e5fa8646 mlir::ConvertOpToLLVMPattern<mlir::gpu::BlockIdOp, false>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:251:12
#18 0x00006428f19aef69 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2409:10
#19 0x00006428f1a46228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#20 0x00006428f1a45ed5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#21 0x00006428e36b3a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#22 0x00006428f1a47a2b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#23 0x00006428f1a442ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#24 0x00006428f19bba58 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2829:21
#25 0x00006428f19afc62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2608:17
#26 0x00006428f19af532 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3306:26
#27 0x00006428f19b05fd llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3407:16
#28 0x00006428f19b01ae mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3453:26
#29 0x00006428f19c0d2c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4176:30
#30 0x00006428f19c0c95 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#31 0x00006428e36b3a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#32 0x00006428f19dcf83 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#33 0x00006428f19b3da8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4180:1
#34 0x00006428f19b3c5c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:10
#35 0x00006428f19b3e45 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4196:10
#36 0x00006428e5f0f9c8 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() /mlir/lib/Conversion/GPUToNVVM/LowerGpuOpsToNVVMOps.cpp:427:13
#37 0x00006428f1b06e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#38 0x00006428f1b06e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#39 0x00006428e36b3a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#40 0x00006428f1b0a31b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#41 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#42 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#43 0x00006428f1b08c88 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const /mlir/lib/Pass/Pass.cpp:1001:36
#44 0x00006428f1b08b20 auto void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)::operator()<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&>(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&) const /mlir/include/mlir/IR/Threading.h:120:56
#45 0x00006428f1b08908 llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:46:18
#46 0x00006428f1b08825 void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:119:9
#47 0x00006428f1b0268b void mlir::parallelForEach<std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:133:1
#48 0x00006428f1b009c9 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#49 0x00006428f1affbe0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#50 0x00006428f1b06e62 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#51 0x00006428f1b06e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#52 0x00006428e36b3a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#53 0x00006428f1b0a31b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#54 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#55 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#56 0x00006428f1b042bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#57 0x00006428f1b03846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#58 0x00006428e3797dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#59 0x00006428e37974d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#60 0x00006428e379718c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#61 0x00006428e3797096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#62 0x00006428f2063c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#63 0x00006428f206301a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#64 0x00006428e3792e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#65 0x00006428e3793215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#66 0x00006428e37933e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#67 0x00006428e3662485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#68 0x0000787ca8a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#69 0x0000787ca8a2a28b call_init ./csu/../csu/libc-start.c:128:20
#70 0x0000787ca8a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#71 0x00006428e36622f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: kitty-apple (kitty-1998)

<details>
During the lowering of a GPU dialect program to NVVM via MLIR, a crash occurs.

```llvm
gpu.module @<!-- -->vector_addition {
  gpu.func @<!-- -->kernel(%A: memref&lt;?xf32&gt;, %B: memref&lt;?xf32&gt;, %C: memref&lt;?xf32&gt;, %num_elements: i32) 
      kernel attributes {known_block_size = array&lt;i32: 256, 1, 1&gt;, known_grid_size = array&lt;i32: -1, 1, 1&gt;} {
    %tidx = gpu.thread_id x
    %bidx = gpu.block_id x
    %bdim = gpu.block_dim x
    %base = arith.muli %bidx, %bdim : index
    %linear_index = arith.addi %base, %tidx : index
    
    %i32_index = arith.index_cast %linear_index : index to i32
    %in_bounds = arith.cmpi ult, %i32_index, %num_elements : i32
    scf.if %in_bounds {
      %aval = memref.load %A[%linear_index] : memref&lt;?xf32&gt;
      %bval = memref.load %B[%linear_index] : memref&lt;?xf32&gt;
      %result = arith.addf %aval, %bval : f32
      memref.store %result, %C[%linear_index] : memref&lt;?xf32&gt;
    }
    gpu.return
  }
}
```

Executing `mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm` causes a crash; see the stack trace below.

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm
 #<!-- -->0 0x00006428e368d87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00006428e368ddab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00006428e368bc64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006428e368e449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000787ca8a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000787ca8a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000787ca8a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000787ca8a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000787ca8a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000787ca8a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000787ca8a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000787ca8a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006428e36f70b1 llvm::APInt::APInt(unsigned int, unsigned long, bool, bool) /llvm/include/llvm/ADT/APInt.h:0:11
#<!-- -->13 0x00006428f06128ab mlir::LLVM::ConstantRangeAttr::get(mlir::MLIRContext*, unsigned int, long, long) /build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOpsAttrDefs.cpp.inc:9771:61
#<!-- -->14 0x00006428e5fa8c5c mlir::gpu::index_lowering::OpLowering&lt;mlir::gpu::BlockIdOp, mlir::NVVM::BlockIdXOp, mlir::NVVM::BlockIdYOp, mlir::NVVM::BlockIdZOp&gt;::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/GPUToNVVM/../GPUCommon/IndexIntrinsicsOpLowering.h:133:31
#<!-- -->15 0x00006428e5fa9223 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;, mlir::gpu::BlockIdOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt; const&amp;, mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->16 0x00006428e5fa8ef7 mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:265:12
#<!-- -->17 0x00006428e5fa8646 mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:251:12
#<!-- -->18 0x00006428f19aef69 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2409:10
#<!-- -->19 0x00006428f1a46228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->20 0x00006428f1a45ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->21 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->22 0x00006428f1a47a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->23 0x00006428f1a442ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->24 0x00006428f19bba58 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2829:21
#<!-- -->25 0x00006428f19afc62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2608:17
#<!-- -->26 0x00006428f19af532 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3306:26
#<!-- -->27 0x00006428f19b05fd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3407:16
#<!-- -->28 0x00006428f19b01ae mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3453:26
#<!-- -->29 0x00006428f19c0d2c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4176:30
#<!-- -->30 0x00006428f19c0c95 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->31 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->32 0x00006428f19dcf83 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->33 0x00006428f19b3da8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4180:1
#<!-- -->34 0x00006428f19b3c5c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:10
#<!-- -->35 0x00006428f19b3e45 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4196:10
#<!-- -->36 0x00006428e5f0f9c8 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() /mlir/lib/Conversion/GPUToNVVM/LowerGpuOpsToNVVMOps.cpp:427:13
#<!-- -->37 0x00006428f1b06e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->38 0x00006428f1b06e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->39 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->40 0x00006428f1b0a31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->41 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->42 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->43 0x00006428f1b08c88 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->44 0x00006428f1b08b20 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->45 0x00006428f1b08908 llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->46 0x00006428f1b08825 void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->47 0x00006428f1b0268b void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->48 0x00006428f1b009c9 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#<!-- -->49 0x00006428f1affbe0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#<!-- -->50 0x00006428f1b06e62 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#<!-- -->51 0x00006428f1b06e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->52 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->53 0x00006428f1b0a31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->54 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->55 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->56 0x00006428f1b042bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->57 0x00006428f1b03846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->58 0x00006428e3797dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->59 0x00006428e37974d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->60 0x00006428e379718c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->61 0x00006428e3797096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->62 0x00006428f2063c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->63 0x00006428f206301a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->64 0x00006428e3792e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->65 0x00006428e3793215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->66 0x00006428e37933e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->67 0x00006428e3662485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->68 0x0000787ca8a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->69 0x0000787ca8a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->70 0x0000787ca8a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->71 0x00006428e36622f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted (core dumped)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: kitty-apple (kitty-1998)

<details>
During the lowering of a GPU dialect program to NVVM via MLIR, a crash occurs.

```llvm
gpu.module @<!-- -->vector_addition {
  gpu.func @<!-- -->kernel(%A: memref&lt;?xf32&gt;, %B: memref&lt;?xf32&gt;, %C: memref&lt;?xf32&gt;, %num_elements: i32) 
      kernel attributes {known_block_size = array&lt;i32: 256, 1, 1&gt;, known_grid_size = array&lt;i32: -1, 1, 1&gt;} {
    %tidx = gpu.thread_id x
    %bidx = gpu.block_id x
    %bdim = gpu.block_dim x
    %base = arith.muli %bidx, %bdim : index
    %linear_index = arith.addi %base, %tidx : index
    
    %i32_index = arith.index_cast %linear_index : index to i32
    %in_bounds = arith.cmpi ult, %i32_index, %num_elements : i32
    scf.if %in_bounds {
      %aval = memref.load %A[%linear_index] : memref&lt;?xf32&gt;
      %bval = memref.load %B[%linear_index] : memref&lt;?xf32&gt;
      %result = arith.addf %aval, %bval : f32
      memref.store %result, %C[%linear_index] : memref&lt;?xf32&gt;
    }
    gpu.return
  }
}
```

Executing `mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm` causes a crash; see the stack trace below.

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-gpu-to-nvvm
 #<!-- -->0 0x00006428e368d87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00006428e368ddab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00006428e368bc64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006428e368e449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000787ca8a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000787ca8a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000787ca8a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000787ca8a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000787ca8a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000787ca8a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000787ca8a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000787ca8a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006428e36f70b1 llvm::APInt::APInt(unsigned int, unsigned long, bool, bool) /llvm/include/llvm/ADT/APInt.h:0:11
#<!-- -->13 0x00006428f06128ab mlir::LLVM::ConstantRangeAttr::get(mlir::MLIRContext*, unsigned int, long, long) /build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOpsAttrDefs.cpp.inc:9771:61
#<!-- -->14 0x00006428e5fa8c5c mlir::gpu::index_lowering::OpLowering&lt;mlir::gpu::BlockIdOp, mlir::NVVM::BlockIdXOp, mlir::NVVM::BlockIdYOp, mlir::NVVM::BlockIdZOp&gt;::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/GPUToNVVM/../GPUCommon/IndexIntrinsicsOpLowering.h:133:31
#<!-- -->15 0x00006428e5fa9223 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;, mlir::gpu::BlockIdOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt; const&amp;, mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->16 0x00006428e5fa8ef7 mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;::matchAndRewrite(mlir::gpu::BlockIdOp, mlir::gpu::BlockIdOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:265:12
#<!-- -->17 0x00006428e5fa8646 mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::BlockIdOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:251:12
#<!-- -->18 0x00006428f19aef69 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2409:10
#<!-- -->19 0x00006428f1a46228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->20 0x00006428f1a45ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->21 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->22 0x00006428f1a47a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->23 0x00006428f1a442ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->24 0x00006428f19bba58 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2829:21
#<!-- -->25 0x00006428f19afc62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2608:17
#<!-- -->26 0x00006428f19af532 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3306:26
#<!-- -->27 0x00006428f19b05fd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3407:16
#<!-- -->28 0x00006428f19b01ae mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3453:26
#<!-- -->29 0x00006428f19c0d2c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4176:30
#<!-- -->30 0x00006428f19c0c95 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->31 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->32 0x00006428f19dcf83 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->33 0x00006428f19b3da8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4180:1
#<!-- -->34 0x00006428f19b3c5c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:10
#<!-- -->35 0x00006428f19b3e45 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4196:10
#<!-- -->36 0x00006428e5f0f9c8 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() /mlir/lib/Conversion/GPUToNVVM/LowerGpuOpsToNVVMOps.cpp:427:13
#<!-- -->37 0x00006428f1b06e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->38 0x00006428f1b06e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->39 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->40 0x00006428f1b0a31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->41 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->42 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->43 0x00006428f1b08c88 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->44 0x00006428f1b08b20 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->45 0x00006428f1b08908 llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->46 0x00006428f1b08825 void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->47 0x00006428f1b0268b void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->48 0x00006428f1b009c9 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#<!-- -->49 0x00006428f1affbe0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#<!-- -->50 0x00006428f1b06e62 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#<!-- -->51 0x00006428f1b06e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->52 0x00006428e36b3a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->53 0x00006428f1b0a31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->54 0x00006428f1afe2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->55 0x00006428f1afe9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->56 0x00006428f1b042bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->57 0x00006428f1b03846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->58 0x00006428e3797dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->59 0x00006428e37974d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->60 0x00006428e379718c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->61 0x00006428e3797096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->62 0x00006428f2063c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->63 0x00006428f206301a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->64 0x00006428e3792e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->65 0x00006428e3793215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->66 0x00006428e37933e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->67 0x00006428e3662485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->68 0x0000787ca8a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->69 0x0000787ca8a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->70 0x0000787ca8a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->71 0x00006428e36622f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted (core dumped)
```
</details>


---

### Comment 3 - woruyu

I would like to fix it!

---

