# [MLIR] MLIR crashes in mlir::xegpu::uArch::getUArch while lowering a GPU-dialect program

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/179167
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-03T11:19:40Z

## Body

Hi, I have identified a crash bug in a GPU program. The reproducible code example is provided below.

#### Input Program
```mlir
module attributes {gpu.container_module} {
  gpu.module @memcpy_kernel {
    gpu.func @kernel_memcpy() attributes {gpu.kernel} {
      %thread_id = gpu.thread_id y
      %block_id = gpu.block_id x
      %block_dim = gpu.block_dim y
      %grid_dim = gpu.grid_dim y
      
      scf.for %i = %thread_id to %block_id step %block_dim {
        %cond = arith.cmpi eq, %i, %grid_dim : index
        scf.if %cond {
          %dummy = arith.constant 99 : index
          %dummy2 = arith.constant 99.0 : f64
        }
      }
      
      %trip_count = arith.constant 10 : index
      %init_val = arith.constant 0 : index
      %final = scf.for %idx = %init_val to %trip_count step %block_dim iter_args(%sum = %init_val) -> index {
        %new_sum = arith.addi %sum, %idx : index
        scf.yield %new_sum : index
      }
      
      gpu.return
    }
  }
  
  func.func @memcpy_host(%arg0: memref<f32>, %arg1: memref<*xf64, 3>) -> memref<4x4xf64> {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c4 = arith.constant 4 : index
    %alloc = memref.alloc() : memref<4x4xf64>
    
    %tid = gpu.thread_id x
    %bid = gpu.block_id y
    
    scf.parallel (%i) = (%c0) to (%c4) step (%c1) {
      scf.parallel (%j) = (%c0) to (%c4) step (%c1) {
        %value = arith.constant 3.14 : f64
        memref.store %value, %alloc[%i, %j] : memref<4x4xf64>
      } {attributes = {parallelism = "reduction"}}
    }
    
    gpu.launch_func @memcpy_kernel::@kernel_memcpy
        blocks in (%c1, %c4, %c4)
        threads in (%c4, %c1, %c4)
        args()
    
    return %alloc : memref<4x4xf64>
  }
}
```

#### Stack Trace
```bash
UNREACHABLE executed at /llvm-project-b52591/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:302!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --xegpu-subgroup-distribute
 #0 0x000055af9749487d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x000055af97494dab PrintStackTraceSignalHandler(void*) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x000055af97492c64 llvm::sys::RunSignalHandlers() /llvm-project-b52591/llvm/lib/Support/Signals.cpp:108:5
 #3 0x000055af97495449 SignalHandler(int, siginfo_t*, void*) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007fe991a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fe991a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fe991a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fe991a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fe991a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fe991a288ff abort ./stdlib/abort.c:81:7
#10 0x000055af9746da88 /llvm-project-b52591/llvm/lib/Support/ErrorHandling.cpp:244:3
#11 0x000055af9f5ddb8c mlir::xegpu::uArch::getUArch(llvm::StringRef) /llvm-project-b52591/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:305:1
#12 0x000055af9f606f6a (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&) const /llvm-project-b52591/mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:148:18
#13 0x000055af9f60f11b mlir::detail::OpOrInterfaceRewritePatternBase<mlir::gpu::GPUFuncOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /llvm-project-b52591/mlir/include/mlir/IR/PatternMatch.h:299:12
#14 0x000055afa584d228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /llvm-project-b52591/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#15 0x000055afa584ced5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#16 0x000055af974baa19 llvm::function_ref<void ()>::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#17 0x000055afa584ea2b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#18 0x000055afa584b2ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /llvm-project-b52591/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#19 0x000055afa5803de5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#20 0x000055afa58030f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#21 0x000055afa5803065 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#22 0x000055af974baa19 llvm::function_ref<void ()>::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#23 0x000055afa580275b void mlir::MLIRContext::executeAction<(anonymous namespace)::GreedyPatternRewriteIteration, long&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, long&) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#24 0x000055afa580092e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#25 0x000055afa5800524 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#26 0x000055af9768bb00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /llvm-project-b52591/mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#27 0x000055af9f5e154e (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() /llvm-project-b52591/mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:2068:16
#28 0x000055afa590de74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:0:19
#29 0x000055afa590de15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#30 0x000055af974baa19 llvm::function_ref<void ()>::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#31 0x000055afa591131b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#32 0x000055afa59052be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:619:17
#33 0x000055afa59059da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:688:16
#34 0x000055afa590b2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:1123:10
#35 0x000055afa590a846 mlir::PassManager::run(mlir::Operation*) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:1097:60
#36 0x000055af9759edfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#37 0x000055af9759e4d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#38 0x000055af9759e18c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#39 0x000055af9759e096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#40 0x000055afa5e6ac5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#41 0x000055afa5e6a01a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project-b52591/mlir/lib/Support/ToolUtilities.cpp:30:12
#42 0x000055af97599e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#43 0x000055af9759a215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#44 0x000055af9759a3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#45 0x000055af97469485 main /llvm-project-b52591/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#46 0x00007fe991a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#47 0x00007fe991a2a28b call_init ./csu/../csu/libc-start.c:128:20
#48 0x00007fe991a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#49 0x000055af974692f5 _start (/mnt/raid/llvm-project-b52591/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --xegpu-subgroup-distribute`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: HKSDev (LynnBroe)

<details>
Hi, I have identified a crash bug in a GPU program. The reproducible code example is provided below.

#### Input Program
```mlir
module attributes {gpu.container_module} {
  gpu.module @<!-- -->memcpy_kernel {
    gpu.func @<!-- -->kernel_memcpy() attributes {gpu.kernel} {
      %thread_id = gpu.thread_id y
      %block_id = gpu.block_id x
      %block_dim = gpu.block_dim y
      %grid_dim = gpu.grid_dim y
      
      scf.for %i = %thread_id to %block_id step %block_dim {
        %cond = arith.cmpi eq, %i, %grid_dim : index
        scf.if %cond {
          %dummy = arith.constant 99 : index
          %dummy2 = arith.constant 99.0 : f64
        }
      }
      
      %trip_count = arith.constant 10 : index
      %init_val = arith.constant 0 : index
      %final = scf.for %idx = %init_val to %trip_count step %block_dim iter_args(%sum = %init_val) -&gt; index {
        %new_sum = arith.addi %sum, %idx : index
        scf.yield %new_sum : index
      }
      
      gpu.return
    }
  }
  
  func.func @<!-- -->memcpy_host(%arg0: memref&lt;f32&gt;, %arg1: memref&lt;*xf64, 3&gt;) -&gt; memref&lt;4x4xf64&gt; {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c4 = arith.constant 4 : index
    %alloc = memref.alloc() : memref&lt;4x4xf64&gt;
    
    %tid = gpu.thread_id x
    %bid = gpu.block_id y
    
    scf.parallel (%i) = (%c0) to (%c4) step (%c1) {
      scf.parallel (%j) = (%c0) to (%c4) step (%c1) {
        %value = arith.constant 3.14 : f64
        memref.store %value, %alloc[%i, %j] : memref&lt;4x4xf64&gt;
      } {attributes = {parallelism = "reduction"}}
    }
    
    gpu.launch_func @<!-- -->memcpy_kernel::@<!-- -->kernel_memcpy
        blocks in (%c1, %c4, %c4)
        threads in (%c4, %c1, %c4)
        args()
    
    return %alloc : memref&lt;4x4xf64&gt;
  }
}
```

#### Stack Trace
```bash
UNREACHABLE executed at /llvm-project-b52591/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:302!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --xegpu-subgroup-distribute
 #<!-- -->0 0x000055af9749487d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x000055af97494dab PrintStackTraceSignalHandler(void*) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x000055af97492c64 llvm::sys::RunSignalHandlers() /llvm-project-b52591/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000055af97495449 SignalHandler(int, siginfo_t*, void*) /llvm-project-b52591/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007fe991a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fe991a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fe991a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fe991a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fe991a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fe991a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000055af9746da88 /llvm-project-b52591/llvm/lib/Support/ErrorHandling.cpp:244:3
#<!-- -->11 0x000055af9f5ddb8c mlir::xegpu::uArch::getUArch(llvm::StringRef) /llvm-project-b52591/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:305:1
#<!-- -->12 0x000055af9f606f6a (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&amp;) const /llvm-project-b52591/mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:148:18
#<!-- -->13 0x000055af9f60f11b mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::gpu::GPUFuncOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /llvm-project-b52591/mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->14 0x000055afa584d228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /llvm-project-b52591/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->15 0x000055afa584ced5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->16 0x000055af974baa19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->17 0x000055afa584ea2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->18 0x000055afa584b2ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /llvm-project-b52591/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->19 0x000055afa5803de5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->20 0x000055afa58030f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->21 0x000055afa5803065 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->22 0x000055af974baa19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->23 0x000055afa580275b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->24 0x000055afa580092e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->25 0x000055afa5800524 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /llvm-project-b52591/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->26 0x000055af9768bb00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /llvm-project-b52591/mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->27 0x000055af9f5e154e (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() /llvm-project-b52591/mlir/lib/Dialect/XeGPU/Transforms/XeGPUSubgroupDistribute.cpp:2068:16
#<!-- -->28 0x000055afa590de74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->29 0x000055afa590de15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->30 0x000055af974baa19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->31 0x000055afa591131b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project-b52591/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->32 0x000055afa59052be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->33 0x000055afa59059da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->34 0x000055afa590b2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->35 0x000055afa590a846 mlir::PassManager::run(mlir::Operation*) /llvm-project-b52591/mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->36 0x000055af9759edfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->37 0x000055af9759e4d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->38 0x000055af9759e18c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->39 0x000055af9759e096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->40 0x000055afa5e6ac5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project-b52591/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->41 0x000055afa5e6a01a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project-b52591/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->42 0x000055af97599e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->43 0x000055af9759a215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->44 0x000055af9759a3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project-b52591/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->45 0x000055af97469485 main /llvm-project-b52591/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->46 0x00007fe991a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->47 0x00007fe991a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->48 0x00007fe991a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->49 0x000055af974692f5 _start (/mnt/raid/llvm-project-b52591/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --xegpu-subgroup-distribute`.
</details>


---

### Comment 2 - akroviakov

The stack trace shows
```
0.	Program arguments: mlir-opt input.mlir --xegpu-subgroup-distribute
```
The MLIR infra for Intel GPUs expects operations from the `xegpu` dialect and requires a target attribute for `--xegpu-subgroup-distribute` in order to properly map to architecture-specific features (e.g., subgroup size). 

For example, see a LIT test for 
https://github.com/llvm/llvm-project/blob/f3df4b9292ce6fbd90de53f0124e55db0a9ee714/mlir/test/Dialect/XeGPU/subgroup-distribute.mlir#L1

Also, without layouts, the distribution will fail as the pass has no guide on how to actually distribute.

---

