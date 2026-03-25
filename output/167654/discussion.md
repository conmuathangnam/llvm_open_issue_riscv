# [MLIR] `convert-parallel-loops-to-gpu` crashes in processParallelLoop creating `AffineApplyOp`

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/167654
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-11-20T14:20:35Z

## Body

Hi, I used MLIR to lower a linalg program to gpu dialect, and it crashed unexpectedly. Please see the following example, repro with `mlir-opt  --convert-parallel-loops-to-gpu `.

```mlir
module {
  func.func @main() {
    %c5 = arith.constant 5 : index
    %c1 = arith.constant 1 : index
    %c16 = arith.constant 16 : index
    %c32 = arith.constant 32 : index
    %c4 = arith.constant 4 : index
    %c124 = arith.constant 124 : index
    %c2 = arith.constant 2 : index
    %c8 = arith.constant 8 : index
    %c0 = arith.constant 0 : index
    %alloc = memref.alloc() : memref<8x128x128x4xf32>
    %alloc_0 = memref.alloc() : memref<32x5x5x4xf32>
    %alloc_1 = memref.alloc() : memref<8x124x124x32xf32>
    scf.parallel (%arg0, %arg1, %arg2, %arg3) = (%c0, %c0, %c0, %c0) to (%c8, %c124, %c124, %c32) step (%c2, %c4, %c8, %c16) {
      %0 = arith.addi %arg0, %c2 : index
      scf.parallel (%arg4) = (%arg0) to (%0) step (%c1) {
        %1 = arith.addi %arg1, %c4 : index
        scf.parallel (%arg5) = (%arg1) to (%1) step (%c1) {
          %2 = arith.addi %arg2, %c8 : index
          %3 = arith.minsi %2, %c124 : index
          scf.parallel (%arg6) = (%arg2) to (%3) step (%c1) {
            %4 = arith.addi %arg3, %c16 : index
            scf.parallel (%arg7) = (%arg3) to (%4) step (%c1) {
              scf.for %arg8 = %c0 to %c5 step %c1 {
                scf.for %arg9 = %c0 to %c5 step %c1 {
                  scf.for %arg10 = %c0 to %c4 step %c1 {
                    %5 = arith.addi %arg5, %arg8 : index
                    %6 = arith.addi %arg6, %arg9 : index
                    %7 = memref.load %alloc[%arg4, %5, %6, %arg10] : memref<8x128x128x4xf32>
                    %8 = memref.load %alloc_0[%arg7, %arg8, %arg9, %arg10] : memref<32x5x5x4xf32>
                    %9 = memref.load %alloc_1[%arg4, %arg5, %arg6, %arg7] : memref<8x124x124x32xf32>
                    %10 = arith.mulf %7, %8 : f32
                    %11 = arith.addf %9, %10 : f32
                    memref.store %11, %alloc_1[%arg4, %arg5, %arg6, %arg7] : memref<8x124x124x32xf32>
                  }
                }
              }
              scf.reduce 
            } {mapping = [#gpu.loop_dim_map<processor = sequential, map = (d0) -> (d0), bound = (d0) -> (d0)>]}
            scf.reduce 
          } {mapping = [#gpu.loop_dim_map<processor = sequential, map = (d0) -> (d0), bound = (d0) -> (d0)>]}
          scf.reduce 
        } {mapping = [#gpu.loop_dim_map<processor = sequential, map = (d0) -> (d0), bound = (d0) -> (d0)>]}
        scf.reduce 
      } {mapping = [#gpu.loop_dim_map<processor = thread_x, map = (d0) -> (d0), bound = (d0) -> (d0)>]}
      scf.reduce 
    } {mapping = [#gpu.loop_dim_map<processor = block_x, map = (d0) -> (d0), bound = (d0) -> (d0)>, #gpu.loop_dim_map<processor = block_y, map = (d0) -> (d0), bound = (d0) -> (d0)>, #gpu.loop_dim_map<processor = block_z, map = (d0) -> (d0), bound = (d0) -> (d0)>, #gpu.loop_dim_map<processor = sequential, map = (d0) -> (d0), bound = (d0) -> (d0)>]}
    return
  }
}

```



Original test case starts from linalg:

```mlir
func.func @main() {
  %in_buf = memref.alloc() : memref<8x128x128x4xf32>
  %filter_buf = memref.alloc() : memref<32x5x5x4xf32>
  %out_buf = memref.alloc() : memref<8x124x124x32xf32>

  linalg.generic
    { indexing_maps = [
        affine_map<(n, oh, ow, oc, kh, kw, ic) -> (n, oh + kh, ow + kw, ic)>,
        affine_map<(n, oh, ow, oc, kh, kw, ic) -> (oc, kh, kw, ic)>,
        affine_map<(n, oh, ow, oc, kh, kw, ic) -> (n, oh, ow, oc)>
      ],
      iterator_types = ["parallel", "parallel", "parallel", "parallel",
                        "reduction", "reduction", "reduction"]
    }
    ins(%in_buf, %filter_buf : memref<8x128x128x4xf32>, memref<32x5x5x4xf32>)
    outs(%out_buf : memref<8x124x124x32xf32>) {
  ^bb0(%in: f32, %filt: f32, %out: f32):
    %prod = arith.mulf %in, %filt : f32
    %sum = arith.addf %out, %prod : f32
    linalg.yield %sum : f32
  }
  return
}
```

### Crash Backtrace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --convert-linalg-to-affine-loops --affine-loop-tile=tile-sizes=2,4,8,16,32,64 --affine-parallelize --lower-affine --canonicalize --gpu-map-parallel-loops --convert-parallel-loops-to-gpu main.mlir
 #0 0x0000564e5b293508 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b3f508)
 #1 0x0000564e5b290c35 llvm::sys::RunSignalHandlers() (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b3cc35)
 #2 0x0000564e5b294611 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f5475e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000564e6280d8f3 mlir::detail::OperandStorage::OperandStorage(mlir::Operation*, mlir::OpOperand*, mlir::ValueRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90b98f3)
 #5 0x0000564e627ff88c mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab88c)
 #6 0x0000564e627ff157 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab157)
 #7 0x0000564e627ff014 mlir::Operation::create(mlir::OperationState const&) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab014)
 #8 0x0000564e627418f0 mlir::OpBuilder::create(mlir::OperationState const&) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8fed8f0)
 #9 0x0000564e620f02e2 mlir::affine::AffineApplyOp::create(mlir::OpBuilder&, mlir::Location, mlir::AffineMap, mlir::ValueRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x899c2e2)
#10 0x0000564e5cc2d861 processParallelLoop(mlir::scf::ParallelOp, mlir::gpu::LaunchOp, mlir::IRMapping&, llvm::SmallVectorImpl<mlir::Operation*>&, llvm::DenseMap<mlir::gpu::Processor, mlir::Value, llvm::DenseMapInfo<mlir::gpu::Processor, void>, llvm::detail::DenseMapPair<mlir::gpu::Processor, mlir::Value>>&, mlir::PatternRewriter&) SCFToGPU.cpp:0:0
#11 0x0000564e5cc2c8c3 (anonymous namespace)::ParallelToGpuLaunchLowering::matchAndRewrite(mlir::scf::ParallelOp, mlir::PatternRewriter&) const SCFToGPU.cpp:0:0
#12 0x0000564e624a739e void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) PatternApplicator.cpp:0:0
#13 0x0000564e624a38eb mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d4f8eb)
#14 0x0000564e6245e927 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#15 0x0000564e6245dc6d mlir::OperationConverter::convert(mlir::Operation*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d09c6d)
#16 0x0000564e6245ecdc mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d0acdc)
#17 0x0000564e6246ddd4 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) DialectConversion.cpp:0:0
#18 0x0000564e624645df applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#19 0x0000564e624646bb mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d106bb)
#20 0x0000564e5cc29c19 (anonymous namespace)::ParallelLoopToGpuPass::runOnOperation() SCFToGPUPass.cpp:0:0
#21 0x0000564e62502bc1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8daebc1)
#22 0x0000564e62503c07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8dafc07)
#23 0x0000564e6250b1a3 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8db71a3)
#24 0x0000564e6250a7b2 mlir::PassManager::run(mlir::Operation*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8db67b2)
#25 0x0000564e5b338603 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#26 0x0000564e5b337864 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#27 0x0000564e6283f928 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90eb928)
#28 0x0000564e5b32cdac mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd8dac)
#29 0x0000564e5b32d0e6 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd90e6)
#30 0x0000564e5b32d2f2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd92f2)
#31 0x0000564e5b27b24f main (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b2724f)
#32 0x00007f5475e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x00007f5475e2a28b call_init ./csu/../csu/libc-start.c:128:20
#34 0x00007f5475e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#35 0x0000564e5b27ad75 _start (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b26d75)
Segmentation fault
```

### Used MLIR command
```bash
mlir-opt --convert-linalg-to-affine-loops \
   --affine-loop-tile="tile-sizes=2,4,8,16,32,64" \
   --affine-parallelize \
   --lower-affine \
   --canonicalize \
   --gpu-map-parallel-loops \
   --convert-parallel-loops-to-gpu \
   main.mlir
```

My mlir-opt version is:
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I used MLIR to lower a linalg program to gpu dialect, and it crashed unexpectedly. Please see the following minimized example.

```mlir
func.func @<!-- -->main() {
  %in_buf = memref.alloc() : memref&lt;8x128x128x4xf32&gt;
  %filter_buf = memref.alloc() : memref&lt;32x5x5x4xf32&gt;
  %out_buf = memref.alloc() : memref&lt;8x124x124x32xf32&gt;

  linalg.generic
    { indexing_maps = [
        affine_map&lt;(n, oh, ow, oc, kh, kw, ic) -&gt; (n, oh + kh, ow + kw, ic)&gt;,
        affine_map&lt;(n, oh, ow, oc, kh, kw, ic) -&gt; (oc, kh, kw, ic)&gt;,
        affine_map&lt;(n, oh, ow, oc, kh, kw, ic) -&gt; (n, oh, ow, oc)&gt;
      ],
      iterator_types = ["parallel", "parallel", "parallel", "parallel",
                        "reduction", "reduction", "reduction"]
    }
    ins(%in_buf, %filter_buf : memref&lt;8x128x128x4xf32&gt;, memref&lt;32x5x5x4xf32&gt;)
    outs(%out_buf : memref&lt;8x124x124x32xf32&gt;) {
  ^bb0(%in: f32, %filt: f32, %out: f32):
    %prod = arith.mulf %in, %filt : f32
    %sum = arith.addf %out, %prod : f32
    linalg.yield %sum : f32
  }
  return
}
```

### Crash Backtrace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --convert-linalg-to-affine-loops --affine-loop-tile=tile-sizes=2,4,8,16,32,64 --affine-parallelize --lower-affine --canonicalize --gpu-map-parallel-loops --convert-parallel-loops-to-gpu main.mlir
 #<!-- -->0 0x0000564e5b293508 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b3f508)
 #<!-- -->1 0x0000564e5b290c35 llvm::sys::RunSignalHandlers() (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b3cc35)
 #<!-- -->2 0x0000564e5b294611 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f5475e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000564e6280d8f3 mlir::detail::OperandStorage::OperandStorage(mlir::Operation*, mlir::OpOperand*, mlir::ValueRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90b98f3)
 #<!-- -->5 0x0000564e627ff88c mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab88c)
 #<!-- -->6 0x0000564e627ff157 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab157)
 #<!-- -->7 0x0000564e627ff014 mlir::Operation::create(mlir::OperationState const&amp;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90ab014)
 #<!-- -->8 0x0000564e627418f0 mlir::OpBuilder::create(mlir::OperationState const&amp;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8fed8f0)
 #<!-- -->9 0x0000564e620f02e2 mlir::affine::AffineApplyOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::AffineMap, mlir::ValueRange) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x899c2e2)
#<!-- -->10 0x0000564e5cc2d861 processParallelLoop(mlir::scf::ParallelOp, mlir::gpu::LaunchOp, mlir::IRMapping&amp;, llvm::SmallVectorImpl&lt;mlir::Operation*&gt;&amp;, llvm::DenseMap&lt;mlir::gpu::Processor, mlir::Value, llvm::DenseMapInfo&lt;mlir::gpu::Processor, void&gt;, llvm::detail::DenseMapPair&lt;mlir::gpu::Processor, mlir::Value&gt;&gt;&amp;, mlir::PatternRewriter&amp;) SCFToGPU.cpp:0:0
#<!-- -->11 0x0000564e5cc2c8c3 (anonymous namespace)::ParallelToGpuLaunchLowering::matchAndRewrite(mlir::scf::ParallelOp, mlir::PatternRewriter&amp;) const SCFToGPU.cpp:0:0
#<!-- -->12 0x0000564e624a739e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) PatternApplicator.cpp:0:0
#<!-- -->13 0x0000564e624a38eb mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d4f8eb)
#<!-- -->14 0x0000564e6245e927 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#<!-- -->15 0x0000564e6245dc6d mlir::OperationConverter::convert(mlir::Operation*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d09c6d)
#<!-- -->16 0x0000564e6245ecdc mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d0acdc)
#<!-- -->17 0x0000564e6246ddd4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) DialectConversion.cpp:0:0
#<!-- -->18 0x0000564e624645df applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->19 0x0000564e624646bb mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8d106bb)
#<!-- -->20 0x0000564e5cc29c19 (anonymous namespace)::ParallelLoopToGpuPass::runOnOperation() SCFToGPUPass.cpp:0:0
#<!-- -->21 0x0000564e62502bc1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8daebc1)
#<!-- -->22 0x0000564e62503c07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8dafc07)
#<!-- -->23 0x0000564e6250b1a3 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8db71a3)
#<!-- -->24 0x0000564e6250a7b2 mlir::PassManager::run(mlir::Operation*) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x8db67b2)
#<!-- -->25 0x0000564e5b338603 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->26 0x0000564e5b337864 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x0000564e6283f928 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x90eb928)
#<!-- -->28 0x0000564e5b32cdac mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd8dac)
#<!-- -->29 0x0000564e5b32d0e6 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd90e6)
#<!-- -->30 0x0000564e5b32d2f2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1bd92f2)
#<!-- -->31 0x0000564e5b27b24f main (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b2724f)
#<!-- -->32 0x00007f5475e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x00007f5475e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x00007f5475e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->35 0x0000564e5b27ad75 _start (/mnt/raid/home/rainyday/bin/mlir/bin/mlir-opt+0x1b26d75)
Segmentation fault
```

### Used MLIR command
```bash
mlir-opt --convert-linalg-to-affine-loops \
   --affine-loop-tile="tile-sizes=2,4,8,16,32,64" \
   --affine-parallelize \
   --lower-affine \
   --canonicalize \
   --gpu-map-parallel-loops \
   --convert-parallel-loops-to-gpu \
   main.mlir
```

My mlir-opt version is:
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```
</details>


---

