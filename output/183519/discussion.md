# [MLIR] A crash bug in vectorizeAffineForOp when processing loop nest

**Author:** karlls12321
**URL:** https://github.com/llvm/llvm-project/issues/183519
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-03-06T21:30:53Z

## Body

```llvm
module {
  func.func @main() -> i32 {
    %c0 = arith.constant 0 : i32
    %sum = affine.for %i = 0 to 10 iter_args(%acc = %c0) -> i32 {
      %inc = arith.addi %acc, %c0 : i32
      affine.yield %inc : i32
    }
    return %sum : i32
  }

  gpu.module @kernels {
    gpu.func @kernel() kernel {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %c1 = arith.constant 1 : index
      %bid_x_plus_1 = arith.addi %bid_x, %c1 : index
      %tid_x_plus_1 = arith.addi %tid_x, %c1 : index
      gpu.return
    }
  }
}
```
Git version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt cudaGraphScaling.mlir --affine-super-vectorizer-test=vectorize-affine-loop-nest --convert-gpu-to-nvvm
 #0 0x0000631c183871fd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x0000631c1838772b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000631c183855c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000631c18387ec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000750f85245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000750f8529eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000750f8529eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000750f8529eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000750f8524527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000750f852288ff abort ./stdlib/abort.c:81:7
#10 0x0000750f8522881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000750f8523b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000631c244cc917 vectorizeAffineForOp(mlir::affine::AffineForOp, (anonymous namespace)::VectorizationState&) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1366:18
#13 0x0000631c244cbae6 vectorizeOneOperation(mlir::Operation*, (anonymous namespace)::VectorizationState&) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1547:5
#14 0x0000631c244cb90d vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0::operator()(mlir::Operation*) const /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1625:16
#15 0x0000631c244cb88d mlir::WalkResult llvm::function_ref<mlir::WalkResult (mlir::Operation*)>::callback_fn<vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0>(long, mlir::Operation*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#16 0x0000631c18538531 llvm::function_ref<mlir::WalkResult (mlir::Operation*)>::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#17 0x0000631c185382cd mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:225:25
#18 0x0000631c244cb82f std::enable_if<llvm::is_one_of<mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, mlir::WalkResult>::type mlir::detail::walk<(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0, mlir::Operation*, mlir::WalkResult>(mlir::Operation*, vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0&&) /mlir/include/mlir/IR/Visitors.h:278:10
#19 0x0000631c244cb7dd std::enable_if<llvm::function_traits<std::decay<vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0>::type>::num_args == 1, mlir::WalkResult>::type mlir::Operation::walk<(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0, mlir::WalkResult>(vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0&&) /mlir/include/mlir/IR/Operation.h:798:12
#20 0x0000631c244cb660 std::enable_if<llvm::function_traits<std::decay<vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0>::type>::num_args == 1, mlir::WalkResult>::type mlir::OpState::walk<(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0, mlir::WalkResult>(vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&)::$_0&&) /mlir/include/mlir/IR/OpDefinition.h:170:19
#21 0x0000631c244c9ef0 vectorizeLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1623:31
#22 0x0000631c244c9b9a mlir::affine::vectorizeAffineLoopNest(std::vector<llvm::SmallVector<mlir::affine::AffineForOp, 2u>, std::allocator<llvm::SmallVector<mlir::affine::AffineForOp, 2u>>>&, mlir::affine::VectorizationStrategy const&) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1931:10
#23 0x0000631c18552846 (anonymous namespace)::VectorizerTestPass::testVecAffineLoopNest(llvm::raw_ostream&) /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:252:9
#24 0x0000631c18551541 (anonymous namespace)::VectorizerTestPass::runOnOperation() /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:286:13
#25 0x0000631c26a01734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#26 0x0000631c26a016d5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#27 0x0000631c183ad5c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#28 0x0000631c26a04bdb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#29 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#30 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#31 0x0000631c26a03548 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const /mlir/lib/Pass/Pass.cpp:1001:36
#32 0x0000631c26a033e0 auto void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)::operator()<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&>(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&) const /mlir/include/mlir/IR/Threading.h:120:56
#33 0x0000631c26a031c8 llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:46:18
#34 0x0000631c26a030e5 void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:119:9
#35 0x0000631c269fcf4b void mlir::parallelForEach<std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:133:1
#36 0x0000631c269fb289 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#37 0x0000631c269fa4a0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#38 0x0000631c26a01722 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#39 0x0000631c26a016d5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#40 0x0000631c183ad5c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#41 0x0000631c26a04bdb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#42 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#43 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#44 0x0000631c269feb7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#45 0x0000631c269fe106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#46 0x0000631c18491b4b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#47 0x0000631c18491221 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#48 0x0000631c18490edc mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#49 0x0000631c18490de6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#50 0x0000631c26f6388a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#51 0x0000631c26f62c4a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#52 0x0000631c1848cb66 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#53 0x0000631c1848cf68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#54 0x0000631c1848d138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#55 0x0000631c1835bde5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#56 0x0000750f8522a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#57 0x0000750f8522a28b call_init ./csu/../csu/libc-start.c:128:20
#58 0x0000750f8522a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#59 0x0000631c1835bc55 _start (/build_all/bin/mlir-opt+0x4742c55)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: bling (karlls12321)

<details>
```llvm
module {
  func.func @<!-- -->main() -&gt; i32 {
    %c0 = arith.constant 0 : i32
    %sum = affine.for %i = 0 to 10 iter_args(%acc = %c0) -&gt; i32 {
      %inc = arith.addi %acc, %c0 : i32
      affine.yield %inc : i32
    }
    return %sum : i32
  }

  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->kernel() kernel {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %c1 = arith.constant 1 : index
      %bid_x_plus_1 = arith.addi %bid_x, %c1 : index
      %tid_x_plus_1 = arith.addi %tid_x, %c1 : index
      gpu.return
    }
  }
}
```
Git version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt cudaGraphScaling.mlir --affine-super-vectorizer-test=vectorize-affine-loop-nest --convert-gpu-to-nvvm
 #<!-- -->0 0x0000631c183871fd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x0000631c1838772b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x0000631c183855c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000631c18387ec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000750f85245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000750f8529eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000750f8529eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000750f8529eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000750f8524527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000750f852288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000750f8522881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000750f8523b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000631c244cc917 vectorizeAffineForOp(mlir::affine::AffineForOp, (anonymous namespace)::VectorizationState&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1366:18
#<!-- -->13 0x0000631c244cbae6 vectorizeOneOperation(mlir::Operation*, (anonymous namespace)::VectorizationState&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1547:5
#<!-- -->14 0x0000631c244cb90d vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0::operator()(mlir::Operation*) const /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1625:16
#<!-- -->15 0x0000631c244cb88d mlir::WalkResult llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;::callback_fn&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;(long, mlir::Operation*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->16 0x0000631c18538531 llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->17 0x0000631c185382cd mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:225:25
#<!-- -->18 0x0000631c244cb82f std::enable_if&lt;llvm::is_one_of&lt;mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, mlir::WalkResult&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::Operation*, mlir::WalkResult&gt;(mlir::Operation*, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/Visitors.h:278:10
#<!-- -->19 0x0000631c244cb7dd std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;::type&gt;::num_args == 1, mlir::WalkResult&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::WalkResult&gt;(vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/Operation.h:798:12
#<!-- -->20 0x0000631c244cb660 std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;::type&gt;::num_args == 1, mlir::WalkResult&gt;::type mlir::OpState::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::WalkResult&gt;(vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/OpDefinition.h:170:19
#<!-- -->21 0x0000631c244c9ef0 vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1623:31
#<!-- -->22 0x0000631c244c9b9a mlir::affine::vectorizeAffineLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1931:10
#<!-- -->23 0x0000631c18552846 (anonymous namespace)::VectorizerTestPass::testVecAffineLoopNest(llvm::raw_ostream&amp;) /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:252:9
#<!-- -->24 0x0000631c18551541 (anonymous namespace)::VectorizerTestPass::runOnOperation() /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:286:13
#<!-- -->25 0x0000631c26a01734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->26 0x0000631c26a016d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->27 0x0000631c183ad5c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->28 0x0000631c26a04bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->29 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->30 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->31 0x0000631c26a03548 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->32 0x0000631c26a033e0 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->33 0x0000631c26a031c8 llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->34 0x0000631c26a030e5 void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->35 0x0000631c269fcf4b void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->36 0x0000631c269fb289 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#<!-- -->37 0x0000631c269fa4a0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#<!-- -->38 0x0000631c26a01722 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#<!-- -->39 0x0000631c26a016d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->40 0x0000631c183ad5c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->41 0x0000631c26a04bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->42 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->43 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->44 0x0000631c269feb7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->45 0x0000631c269fe106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->46 0x0000631c18491b4b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->47 0x0000631c18491221 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->48 0x0000631c18490edc mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->49 0x0000631c18490de6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x0000631c26f6388a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->51 0x0000631c26f62c4a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->52 0x0000631c1848cb66 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->53 0x0000631c1848cf68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->54 0x0000631c1848d138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->55 0x0000631c1835bde5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->56 0x0000750f8522a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x0000750f8522a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x0000750f8522a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x0000631c1835bc55 _start (/build_all/bin/mlir-opt+0x4742c55)
```
</details>


---

### Comment 2 - ayokunle321

Can I be assigned this?


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: bling (karlls12321)

<details>
```llvm
module {
  func.func @<!-- -->main() -&gt; i32 {
    %c0 = arith.constant 0 : i32
    %sum = affine.for %i = 0 to 10 iter_args(%acc = %c0) -&gt; i32 {
      %inc = arith.addi %acc, %c0 : i32
      affine.yield %inc : i32
    }
    return %sum : i32
  }

  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->kernel() kernel {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %c1 = arith.constant 1 : index
      %bid_x_plus_1 = arith.addi %bid_x, %c1 : index
      %tid_x_plus_1 = arith.addi %tid_x, %c1 : index
      gpu.return
    }
  }
}
```
Git version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt cudaGraphScaling.mlir --affine-super-vectorizer-test=vectorize-affine-loop-nest --convert-gpu-to-nvvm
 #<!-- -->0 0x0000631c183871fd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x0000631c1838772b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x0000631c183855c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000631c18387ec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000750f85245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000750f8529eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000750f8529eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000750f8529eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000750f8524527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000750f852288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000750f8522881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000750f8523b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000631c244cc917 vectorizeAffineForOp(mlir::affine::AffineForOp, (anonymous namespace)::VectorizationState&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1366:18
#<!-- -->13 0x0000631c244cbae6 vectorizeOneOperation(mlir::Operation*, (anonymous namespace)::VectorizationState&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1547:5
#<!-- -->14 0x0000631c244cb90d vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0::operator()(mlir::Operation*) const /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1625:16
#<!-- -->15 0x0000631c244cb88d mlir::WalkResult llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;::callback_fn&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;(long, mlir::Operation*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->16 0x0000631c18538531 llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->17 0x0000631c185382cd mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:225:25
#<!-- -->18 0x0000631c244cb82f std::enable_if&lt;llvm::is_one_of&lt;mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, mlir::WalkResult&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::Operation*, mlir::WalkResult&gt;(mlir::Operation*, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/Visitors.h:278:10
#<!-- -->19 0x0000631c244cb7dd std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;::type&gt;::num_args == 1, mlir::WalkResult&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::WalkResult&gt;(vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/Operation.h:798:12
#<!-- -->20 0x0000631c244cb660 std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&gt;::type&gt;::num_args == 1, mlir::WalkResult&gt;::type mlir::OpState::walk&lt;(mlir::WalkOrder)0, mlir::ForwardIterator, vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0, mlir::WalkResult&gt;(vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;)::$_0&amp;&amp;) /mlir/include/mlir/IR/OpDefinition.h:170:19
#<!-- -->21 0x0000631c244c9ef0 vectorizeLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1623:31
#<!-- -->22 0x0000631c244c9b9a mlir::affine::vectorizeAffineLoopNest(std::vector&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;, std::allocator&lt;llvm::SmallVector&lt;mlir::affine::AffineForOp, 2u&gt;&gt;&gt;&amp;, mlir::affine::VectorizationStrategy const&amp;) /mlir/lib/Dialect/Affine/Transforms/SuperVectorize.cpp:1931:10
#<!-- -->23 0x0000631c18552846 (anonymous namespace)::VectorizerTestPass::testVecAffineLoopNest(llvm::raw_ostream&amp;) /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:252:9
#<!-- -->24 0x0000631c18551541 (anonymous namespace)::VectorizerTestPass::runOnOperation() /mlir/test/lib/Dialect/Affine/TestVectorizationUtils.cpp:286:13
#<!-- -->25 0x0000631c26a01734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->26 0x0000631c26a016d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->27 0x0000631c183ad5c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->28 0x0000631c26a04bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->29 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->30 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->31 0x0000631c26a03548 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->32 0x0000631c26a033e0 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->33 0x0000631c26a031c8 llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->34 0x0000631c26a030e5 void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->35 0x0000631c269fcf4b void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->36 0x0000631c269fb289 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#<!-- -->37 0x0000631c269fa4a0 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#<!-- -->38 0x0000631c26a01722 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#<!-- -->39 0x0000631c26a016d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->40 0x0000631c183ad5c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->41 0x0000631c26a04bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->42 0x0000631c269f8b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->43 0x0000631c269f929a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->44 0x0000631c269feb7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->45 0x0000631c269fe106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->46 0x0000631c18491b4b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->47 0x0000631c18491221 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->48 0x0000631c18490edc mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->49 0x0000631c18490de6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x0000631c26f6388a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->51 0x0000631c26f62c4a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->52 0x0000631c1848cb66 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->53 0x0000631c1848cf68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->54 0x0000631c1848d138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->55 0x0000631c1835bde5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->56 0x0000750f8522a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x0000750f8522a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x0000750f8522a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x0000631c1835bc55 _start (/build_all/bin/mlir-opt+0x4742c55)
```
</details>


---

