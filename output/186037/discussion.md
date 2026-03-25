# [MLIR] Crash during GPU to XeVM lowering: cast<gpu::ReturnOp> fails on non-ReturnOp operation

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/186037
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-18T15:25:15Z

## Body

Hi, when using the MLIR XEVM GPU lowering pipeline, I found a crashing bug. Please see the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```mlir
module attributes {gpu.container_module} {
  gpu.module @kernels {
    gpu.func @add_kernel(%arg0: memref<64xf32, 3>, %arg1: memref<64xf32, 3>, %arg2: memref<64xf32, 3>) kernel {
      %c0 = arith.constant 0 : index
      %c64 = arith.constant 64 : index
      %threadIdx_x = gpu.thread_id  x
      %blockDim_x = gpu.block_dim  x
      %blockIdx_x = gpu.block_id  x
      scf.execute_region -> () {
        %tid = arith.addi %threadIdx_x, %blockIdx_x : index
        %stride = arith.muli %blockDim_x, %c64 : index
        "test.regoin"() : () -> ()
      }
      scf.for %outer = %c0 to %c64 step %c64 {
        scf.for %inner = %c0 to %c64 step %c64 {
          %cond = arith.cmpi slt, %inner, %outer : index
          scf.if %cond {
            %loaded0 = memref.load %arg0[%inner] : memref<64xf32, 3>
            %loaded1 = memref.load %arg1[%inner] : memref<64xf32, 3>
            %sum = arith.mulf %loaded0, %loaded1 : f32
            memref.store %sum, %arg2[%inner] : memref<64xf32, 3>
            scf.yield
          }
        }
      }
      gpu.return
    }
  }
}
```

#### Crash Log
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt postProcessGL.mlir --gpu-lower-to-xevm-pipeline
 #0 0x00005afd7adc6dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00005afd7adc72eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00005afd7adc5184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005afd7adc7a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007f801b245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f801b29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f801b29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f801b29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f801b24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f801b2288ff abort ./stdlib/abort.c:81:7
#10 0x00007f801b22881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f801b23b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005afd7d708ad4 decltype(auto) llvm::cast<mlir::gpu::ReturnOp, mlir::Operation>(mlir::Operation*) /llvm/include/llvm/Support/Casting.h:573:39
#13 0x00005afd832427b9 (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&) const /mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:191:9
#14 0x00005afd83249fab mlir::detail::OpOrInterfaceRewritePatternBase<mlir::gpu::GPUFuncOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#15 0x00005afd896ae338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#16 0x00005afd896adfe5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#17 0x00005afd7aded189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#18 0x00005afd896afb3b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#19 0x00005afd896ac3ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#20 0x00005afd896647b5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#21 0x00005afd89663ac7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#22 0x00005afd89663a35 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#23 0x00005afd7aded189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#24 0x00005afd8966312b void mlir::MLIRContext::executeAction<(anonymous namespace)::GreedyPatternRewriteIteration, long&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, long&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#25 0x00005afd896612fe (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#26 0x00005afd89660ef4 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#27 0x00005afd7afc03a0 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:212:15
#28 0x00005afd8321c51e (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() /mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:2138:16
#29 0x00005afd8976ffa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#30 0x00005afd8976ff45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#31 0x00005afd7aded189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#32 0x00005afd8977344b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#33 0x00005afd897673ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#34 0x00005afd89767b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#35 0x00005afd89771db8 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const /mlir/lib/Pass/Pass.cpp:1001:36
#36 0x00005afd89771c50 auto void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)::operator()<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&>(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&) const /mlir/include/mlir/IR/Threading.h:120:56
#37 0x00005afd89771a38 llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:46:18
#38 0x00005afd89771955 void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:119:9
#39 0x00005afd8976b7bb void mlir::parallelForEach<std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /mlir/include/mlir/IR/Threading.h:133:1
#40 0x00005afd89769af9 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#41 0x00005afd89768d10 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#42 0x00005afd8976ff92 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#43 0x00005afd8976ff45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#44 0x00005afd7aded189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#45 0x00005afd8977344b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#46 0x00005afd897673ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#47 0x00005afd89767b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#48 0x00005afd8976d3ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#49 0x00005afd8976c976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#50 0x00005afd7aed1e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#51 0x00005afd7aed1551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#52 0x00005afd7aed120c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#53 0x00005afd7aed1116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#54 0x00005afd89cd96fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#55 0x00005afd89cd8aba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#56 0x00005afd7aeccdc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#57 0x00005afd7aecd1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#58 0x00005afd7aecd398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#59 0x00005afd7ad9b8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#60 0x00007f801b22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#61 0x00007f801b22a28b call_init ./csu/../csu/libc-start.c:128:20
#62 0x00007f801b22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#63 0x00005afd7ad9b745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt postProcessGL.mlir --gpu-lower-to-xevm-pipeline
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, when using the MLIR XEVM GPU lowering pipeline, I found a crashing bug. Please see the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->add_kernel(%arg0: memref&lt;64xf32, 3&gt;, %arg1: memref&lt;64xf32, 3&gt;, %arg2: memref&lt;64xf32, 3&gt;) kernel {
      %c0 = arith.constant 0 : index
      %c64 = arith.constant 64 : index
      %threadIdx_x = gpu.thread_id  x
      %blockDim_x = gpu.block_dim  x
      %blockIdx_x = gpu.block_id  x
      scf.execute_region -&gt; () {
        %tid = arith.addi %threadIdx_x, %blockIdx_x : index
        %stride = arith.muli %blockDim_x, %c64 : index
        "test.regoin"() : () -&gt; ()
      }
      scf.for %outer = %c0 to %c64 step %c64 {
        scf.for %inner = %c0 to %c64 step %c64 {
          %cond = arith.cmpi slt, %inner, %outer : index
          scf.if %cond {
            %loaded0 = memref.load %arg0[%inner] : memref&lt;64xf32, 3&gt;
            %loaded1 = memref.load %arg1[%inner] : memref&lt;64xf32, 3&gt;
            %sum = arith.mulf %loaded0, %loaded1 : f32
            memref.store %sum, %arg2[%inner] : memref&lt;64xf32, 3&gt;
            scf.yield
          }
        }
      }
      gpu.return
    }
  }
}
```

#### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt postProcessGL.mlir --gpu-lower-to-xevm-pipeline
 #<!-- -->0 0x00005afd7adc6dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00005afd7adc72eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00005afd7adc5184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005afd7adc7a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007f801b245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f801b29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f801b29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f801b29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f801b24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f801b2288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f801b22881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f801b23b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005afd7d708ad4 decltype(auto) llvm::cast&lt;mlir::gpu::ReturnOp, mlir::Operation&gt;(mlir::Operation*) /llvm/include/llvm/Support/Casting.h:573:39
#<!-- -->13 0x00005afd832427b9 (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&amp;) const /mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:191:9
#<!-- -->14 0x00005afd83249fab mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::gpu::GPUFuncOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->15 0x00005afd896ae338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->16 0x00005afd896adfe5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->17 0x00005afd7aded189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->18 0x00005afd896afb3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->19 0x00005afd896ac3ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->20 0x00005afd896647b5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->21 0x00005afd89663ac7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->22 0x00005afd89663a35 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->23 0x00005afd7aded189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->24 0x00005afd8966312b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->25 0x00005afd896612fe (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->26 0x00005afd89660ef4 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->27 0x00005afd7afc03a0 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:212:15
#<!-- -->28 0x00005afd8321c51e (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() /mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:2138:16
#<!-- -->29 0x00005afd8976ffa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->30 0x00005afd8976ff45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->31 0x00005afd7aded189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->32 0x00005afd8977344b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->33 0x00005afd897673ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->34 0x00005afd89767b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->35 0x00005afd89771db8 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->36 0x00005afd89771c50 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->37 0x00005afd89771a38 llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->38 0x00005afd89771955 void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->39 0x00005afd8976b7bb void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->40 0x00005afd89769af9 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /mlir/lib/Pass/Pass.cpp:1012:7
#<!-- -->41 0x00005afd89768d10 mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /mlir/lib/Pass/Pass.cpp:849:5
#<!-- -->42 0x00005afd8976ff92 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:610:13
#<!-- -->43 0x00005afd8976ff45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->44 0x00005afd7aded189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->45 0x00005afd8977344b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->46 0x00005afd897673ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->47 0x00005afd89767b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->48 0x00005afd8976d3ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->49 0x00005afd8976c976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->50 0x00005afd7aed1e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->51 0x00005afd7aed1551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->52 0x00005afd7aed120c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->53 0x00005afd7aed1116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->54 0x00005afd89cd96fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->55 0x00005afd89cd8aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->56 0x00005afd7aeccdc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->57 0x00005afd7aecd1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->58 0x00005afd7aecd398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->59 0x00005afd7ad9b8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->60 0x00007f801b22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->61 0x00007f801b22a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->62 0x00007f801b22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->63 0x00005afd7ad9b745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Reproduction Command
```bash
mlir-opt postProcessGL.mlir --gpu-lower-to-xevm-pipeline
```
</details>


---

### Comment 2 - Men-cotton

I confirmed that #184711 prevents this specific repro from reaching the original crash.

However, the underlying `MoveFuncBodyToWarpOp` `cast<gpu::ReturnOp>` crash is still present at HEAD, and the malformed repro below still reaches the same assertion when run with `mlir-opt repro.mlir --gpu-lower-to-xevm-pipeline --allow-unregistered-dialect`.

```mlir
module attributes {gpu.container_module} {
  gpu.module @kernels {
    "gpu.func"() ({
    ^bb0:
      "test.unknown"() : () -> ()
    }) {function_type = () -> (), sym_name = "bad", kernel} : () -> ()
  }
}
```

---

