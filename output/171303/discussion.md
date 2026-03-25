# [MLIR] convert-gpu-to-nvvm crashes with "index.mul created with unregistered dialect"

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/171303
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2025-12-13T10:43:14Z

## Body

Hi, I found a crash bug when lowering programs with MLIR. Here is a code example that reproduces the issue:


```llvm
module {
  gpu.module @vector_add_kernel {
    gpu.func @vector_add_local_tile_multi_elem_per_thread_half(%arg0: memref<?xf16>, %arg1: i32, %arg2: memref<?xf16>, %arg3: memref<?xf16>, %arg4: f16, %arg5: f16, %arg6: f16) {
      %c8_i32 = arith.constant 8 : i32
      %global_id_x = gpu.global_id  x
      %thread_id_x = gpu.thread_id  x
      %block_id_x = gpu.block_id  x
      %0 = arith.index_cast %global_id_x : index to i32
      %1 = arith.index_cast %thread_id_x : index to i32
      %2 = arith.index_cast %block_id_x : index to i32
      %3 = arith.muli %2, %0 : i32
      %4 = arith.addi %1, %3 : i32
      %5 = arith.divsi %arg1, %c8_i32 : i32
      %6 = arith.cmpi slt, %4, %5 : i32
      cf.cond_br %6, ^bb1, ^bb2
    ^bb1:  // pred: ^bb0
      %7 = arith.muli %4, %c8_i32 : i32
      %8 = arith.index_cast %7 : i32 to index
      %9 = vector.load %arg2[%8] : memref<?xf16>, vector<8xf16>
      %10 = vector.load %arg3[%8] : memref<?xf16>, vector<8xf16>
      %11 = arith.extf %arg4 : f16 to f32
      %12 = arith.extf %arg5 : f16 to f32
      %13 = arith.extf %arg6 : f16 to f32
      %14 = vector.broadcast %11 : f32 to vector<8xf32>
      %15 = vector.broadcast %12 : f32 to vector<8xf32>
      %16 = vector.broadcast %13 : f32 to vector<8xf32>
      %17 = arith.truncf %14 : vector<8xf32> to vector<8xf16>
      %18 = arith.truncf %15 : vector<8xf32> to vector<8xf16>
      %19 = arith.truncf %16 : vector<8xf32> to vector<8xf16>
      %20 = arith.mulf %9, %17 : vector<8xf16>
      %21 = arith.mulf %10, %18 : vector<8xf16>
      %22 = arith.addf %21, %19 : vector<8xf16>
      %23 = arith.addf %20, %22 : vector<8xf16>
      vector.store %23, %arg0[%8] : memref<?xf16>, vector<8xf16>
      cf.br ^bb2
    ^bb2:  // 2 preds: ^bb0, ^bb1
      gpu.return
    }
  }
}
```

### Used Command
```bash
mlir-opt ./input.mlir  --convert-gpu-to-nvvm
```

```
LLVM ERROR: index.mul created with unregistered dialect. If this is intended, please call allowUnregisteredDialects() on the MLIRContext, or use -allow-unregistered-dialect with the MLIR tool used.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: bin/mlir-opt -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts /tmp/crash.mlir --mlir-print-ir-after-all
 #0 0x000055f8dadf1e6d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055f8dadf239b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055f8dadf026f llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055f8dadf2a39 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007ff6f5b13520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007ff6f5b679fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #6 0x00007ff6f5b679fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #7 0x00007ff6f5b679fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #8 0x00007ff6f5b13476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007ff6f5af97f3 abort ./stdlib/./stdlib/abort.c:81:7
#10 0x000055f8dacd8885 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000055f8e74f1e81 mlir::Operation::Operation(mlir::Location, mlir::OperationName, unsigned int, unsigned int, unsigned int, unsigned int, int, mlir::DictionaryAttr, mlir::OpaqueProperties, bool) /mlir/lib/IR/Operation.cpp:186:7
#12 0x000055f8e74f1682 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int, unsigned int) /mlir/lib/IR/Operation.cpp:126:7
#13 0x000055f8e74f13ea mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, unsigned int, unsigned int) /mlir/lib/IR/Operation.cpp:78:3
#14 0x000055f8e74f114f mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange, unsigned int) /mlir/lib/IR/Operation.cpp:59:14
#15 0x000055f8e74f0f0e mlir::Operation::create(mlir::OperationState const&) /mlir/lib/IR/Operation.cpp:36:19
#16 0x000055f8e737c3b1 mlir::OpBuilder::create(mlir::OperationState const&) /mlir/lib/IR/Builders.cpp:458:17
#17 0x000055f8e436729a mlir::index::MulOp::create(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::Value) /build-debug/tools/mlir/include/mlir/Dialect/Index/IR/IndexOps.cpp.inc:5542:50
#18 0x000055f8de0144be (anonymous namespace)::GpuGlobalIdRewriter::matchAndRewrite(mlir::gpu::GlobalIdOp, mlir::PatternRewriter&) const /mlir/lib/Dialect/GPU/Transforms/GlobalIdRewriter.cpp:32:16
#19 0x000055f8de014d2b mlir::detail::OpOrInterfaceRewritePatternBase<mlir::gpu::GlobalIdOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#20 0x000055f8e6f57918 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#21 0x000055f8e6f575c5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#22 0x000055f8dacfc929 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#23 0x000055f8e6f5914b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#24 0x000055f8e6f559aa mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#25 0x000055f8e6f104f5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:620:17
#26 0x000055f8e6f0f807 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:890:31
#27 0x000055f8e6f0f775 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#28 0x000055f8dacfc929 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#29 0x000055f8e6f0ee6b void mlir::MLIRContext::executeAction<(anonymous namespace)::GreedyPatternRewriteIteration, long&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, long&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#30 0x000055f8e6f0d03e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:903:3
#31 0x000055f8e6f0cc34 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:935:47
#32 0x000055f8db248500 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#33 0x000055f8dd720cd1 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() /mlir/lib/Conversion/GPUToNVVM/LowerGpuOpsToNVVMOps.cpp:376:18
#34 0x000055f8e7018664 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:17
#35 0x000055f8e7018605 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
```

My MLIR version:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: clearlove7 (clearlove2077)

<details>
Hi, I found a crash bug when lowering programs with MLIR. Here is a code example that reproduces the issue:

**Input file name: input.mlir**
```llvm
module {
  gpu.module @<!-- -->vector_add_kernel {
    gpu.func @<!-- -->vector_add_local_tile_multi_elem_per_thread_half(%z: memref&lt;?xf16&gt;, %num: i32, %x: memref&lt;?xf16&gt;, %y: memref&lt;?xf16&gt;, %a: f16, %b: f16, %c: f16) {
      %kNumElemPerThread = arith.constant 8 : i32
      %blockDim_x = gpu.global_id  x
      %threadIdx_x = gpu.thread_id x
      %blockIdx_x = gpu.block_id x
      
      %blockDim_x_i32 = arith.index_cast %blockDim_x : index to i32
      %threadIdx_x_i32 = arith.index_cast %threadIdx_x : index to i32
      %blockIdx_x_i32 = arith.index_cast %blockIdx_x : index to i32
      
      %grid_index = arith.muli %blockIdx_x_i32, %blockDim_x_i32 : i32
      %idx = arith.addi %threadIdx_x_i32, %grid_index : i32
      
      %num_div_elems = arith.divsi %num, %kNumElemPerThread : i32
      %cond = arith.cmpi slt, %idx, %num_div_elems : i32
      scf.if %cond {
        %offset = arith.muli %idx, %kNumElemPerThread : i32
        %offset_index = arith.index_cast %offset : i32 to index
        
        %tx_data = vector.load %x[%offset_index] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
        %ty_data = vector.load %y[%offset_index] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
        
        %a_extended = arith.extf %a : f16 to f32
        %b_extended = arith.extf %b : f16 to f32
        %c_extended = arith.extf %c : f16 to f32
        
        %a_broadcast = vector.broadcast %a_extended : f32 to vector&lt;8xf32&gt;
        %b_broadcast = vector.broadcast %b_extended : f32 to vector&lt;8xf32&gt;
        %c_broadcast = vector.broadcast %c_extended : f32 to vector&lt;8xf32&gt;
        
        %a_truncated = arith.truncf %a_broadcast : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
        %b_truncated = arith.truncf %b_broadcast : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
        %c_truncated = arith.truncf %c_broadcast : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
        
        %product_a_x = arith.mulf %tx_data, %a_truncated : vector&lt;8xf16&gt;
        %product_b_y = arith.mulf %ty_data, %b_truncated : vector&lt;8xf16&gt;
        %sum_bc = arith.addf %product_b_y, %c_truncated : vector&lt;8xf16&gt;
        %final_result = arith.addf %product_a_x, %sum_bc : vector&lt;8xf16&gt;
        
        vector.store %final_result, %z[%offset_index] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
      }
      gpu.return
    }
  }
}
```

I encountered a crash bug when running MLIR with the following command.

### Used Command
```bash
mlir-opt ./input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt ./input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
 #<!-- -->0 0x000055d0a911a508 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3f508)
 #<!-- -->1 0x000055d0a9117c35 llvm::sys::RunSignalHandlers() (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3cc35)
 #<!-- -->2 0x000055d0a911b611 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f9a53c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f9a53c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f9a53c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f9a53c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f9a53c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f9a53c288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000055d0a910263d llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b2763d)
#<!-- -->10 0x000055d0ae46c62c (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x6e9162c)
#<!-- -->11 0x000055d0ae45c879 mlir::index::MulOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::Value) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x6e81879)
#<!-- -->12 0x000055d0aaac678b (anonymous namespace)::GpuGlobalIdRewriter::matchAndRewrite(mlir::gpu::GlobalIdOp, mlir::PatternRewriter&amp;) const GlobalIdRewriter.cpp:0:0
#<!-- -->13 0x000055d0b032e39e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) PatternApplicator.cpp:0:0
#<!-- -->14 0x000055d0b032a8eb mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8d4f8eb)
#<!-- -->15 0x000055d0b03065bb (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->16 0x000055d0b0303150 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8d28150)
#<!-- -->17 0x000055d0aa61ced8 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() LowerGpuOpsToNVVMOps.cpp:0:0
#<!-- -->18 0x000055d0b0389bc1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8daebc1)
#<!-- -->19 0x000055d0b038ac07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8dafc07)
#<!-- -->20 0x000055d0b0395dae auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const Pass.cpp:0:0
#<!-- -->21 0x000055d0b038d8eb mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db28eb)
#<!-- -->22 0x000055d0b038a01f mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8daf01f)
#<!-- -->23 0x000055d0b038ac07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8dafc07)
#<!-- -->24 0x000055d0b03921a3 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db71a3)
#<!-- -->25 0x000055d0b03917b2 mlir::PassManager::run(mlir::Operation*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db67b2)
#<!-- -->26 0x000055d0a91bf603 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x000055d0a91be864 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->28 0x000055d0b06c6928 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x90eb928)
#<!-- -->29 0x000055d0a91b3dac mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd8dac)
#<!-- -->30 0x000055d0a91b40e6 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd90e6)
#<!-- -->31 0x000055d0a91b42f2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd92f2)
#<!-- -->32 0x000055d0a910224f main (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b2724f)
#<!-- -->33 0x00007f9a53c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->34 0x00007f9a53c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->35 0x00007f9a53c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->36 0x000055d0a9101d75 _start (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b26d75)
Aborted
```

My MLIR version:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: clearlove7 (clearlove2077)

<details>
Hi, I found a crash bug when lowering programs with MLIR. Here is a code example that reproduces the issue:


```llvm
module {
  gpu.module @<!-- -->vector_add_kernel {
    gpu.func @<!-- -->vector_add_local_tile_multi_elem_per_thread_half(%arg0: memref&lt;?xf16&gt;, %arg1: i32, %arg2: memref&lt;?xf16&gt;, %arg3: memref&lt;?xf16&gt;, %arg4: f16, %arg5: f16, %arg6: f16) {
      %c8_i32 = arith.constant 8 : i32
      %global_id_x = gpu.global_id  x
      %thread_id_x = gpu.thread_id  x
      %block_id_x = gpu.block_id  x
      %0 = arith.index_cast %global_id_x : index to i32
      %1 = arith.index_cast %thread_id_x : index to i32
      %2 = arith.index_cast %block_id_x : index to i32
      %3 = arith.muli %2, %0 : i32
      %4 = arith.addi %1, %3 : i32
      %5 = arith.divsi %arg1, %c8_i32 : i32
      %6 = arith.cmpi slt, %4, %5 : i32
      cf.cond_br %6, ^bb1, ^bb2
    ^bb1:  // pred: ^bb0
      %7 = arith.muli %4, %c8_i32 : i32
      %8 = arith.index_cast %7 : i32 to index
      %9 = vector.load %arg2[%8] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
      %10 = vector.load %arg3[%8] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
      %11 = arith.extf %arg4 : f16 to f32
      %12 = arith.extf %arg5 : f16 to f32
      %13 = arith.extf %arg6 : f16 to f32
      %14 = vector.broadcast %11 : f32 to vector&lt;8xf32&gt;
      %15 = vector.broadcast %12 : f32 to vector&lt;8xf32&gt;
      %16 = vector.broadcast %13 : f32 to vector&lt;8xf32&gt;
      %17 = arith.truncf %14 : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
      %18 = arith.truncf %15 : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
      %19 = arith.truncf %16 : vector&lt;8xf32&gt; to vector&lt;8xf16&gt;
      %20 = arith.mulf %9, %17 : vector&lt;8xf16&gt;
      %21 = arith.mulf %10, %18 : vector&lt;8xf16&gt;
      %22 = arith.addf %21, %19 : vector&lt;8xf16&gt;
      %23 = arith.addf %20, %22 : vector&lt;8xf16&gt;
      vector.store %23, %arg0[%8] : memref&lt;?xf16&gt;, vector&lt;8xf16&gt;
      cf.br ^bb2
    ^bb2:  // 2 preds: ^bb0, ^bb1
      gpu.return
    }
  }
}
```

### Used Command
```bash
mlir-opt ./input.mlir  --convert-gpu-to-nvvm
```

```
LLVM ERROR: index.mul created with unregistered dialect. If this is intended, please call allowUnregisteredDialects() on the MLIRContext, or use -allow-unregistered-dialect with the MLIR tool used.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: bin/mlir-opt -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts /tmp/crash.mlir --mlir-print-ir-after-all
 #<!-- -->0 0x000055f8dadf1e6d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055f8dadf239b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055f8dadf026f llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055f8dadf2a39 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007ff6f5b13520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007ff6f5b679fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007ff6f5b679fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007ff6f5b679fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007ff6f5b13476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007ff6f5af97f3 abort ./stdlib/./stdlib/abort.c:81:7
#<!-- -->10 0x000055f8dacd8885 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000055f8e74f1e81 mlir::Operation::Operation(mlir::Location, mlir::OperationName, unsigned int, unsigned int, unsigned int, unsigned int, int, mlir::DictionaryAttr, mlir::OpaqueProperties, bool) /mlir/lib/IR/Operation.cpp:186:7
#<!-- -->12 0x000055f8e74f1682 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int, unsigned int) /mlir/lib/IR/Operation.cpp:126:7
#<!-- -->13 0x000055f8e74f13ea mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, unsigned int, unsigned int) /mlir/lib/IR/Operation.cpp:78:3
#<!-- -->14 0x000055f8e74f114f mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange, unsigned int) /mlir/lib/IR/Operation.cpp:59:14
#<!-- -->15 0x000055f8e74f0f0e mlir::Operation::create(mlir::OperationState const&amp;) /mlir/lib/IR/Operation.cpp:36:19
#<!-- -->16 0x000055f8e737c3b1 mlir::OpBuilder::create(mlir::OperationState const&amp;) /mlir/lib/IR/Builders.cpp:458:17
#<!-- -->17 0x000055f8e436729a mlir::index::MulOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::Value) /build-debug/tools/mlir/include/mlir/Dialect/Index/IR/IndexOps.cpp.inc:5542:50
#<!-- -->18 0x000055f8de0144be (anonymous namespace)::GpuGlobalIdRewriter::matchAndRewrite(mlir::gpu::GlobalIdOp, mlir::PatternRewriter&amp;) const /mlir/lib/Dialect/GPU/Transforms/GlobalIdRewriter.cpp:32:16
#<!-- -->19 0x000055f8de014d2b mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::gpu::GlobalIdOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->20 0x000055f8e6f57918 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->21 0x000055f8e6f575c5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->22 0x000055f8dacfc929 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->23 0x000055f8e6f5914b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->24 0x000055f8e6f559aa mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->25 0x000055f8e6f104f5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:620:17
#<!-- -->26 0x000055f8e6f0f807 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:890:31
#<!-- -->27 0x000055f8e6f0f775 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->28 0x000055f8dacfc929 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->29 0x000055f8e6f0ee6b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->30 0x000055f8e6f0d03e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:903:3
#<!-- -->31 0x000055f8e6f0cc34 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:935:47
#<!-- -->32 0x000055f8db248500 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->33 0x000055f8dd720cd1 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() /mlir/lib/Conversion/GPUToNVVM/LowerGpuOpsToNVVMOps.cpp:376:18
#<!-- -->34 0x000055f8e7018664 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->35 0x000055f8e7018605 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
```

My MLIR version:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```


</details>


---

