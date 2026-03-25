# [MLIR] Crash in --convert-vector-to-xegpu pass when processing vector.store

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/181463
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-03T11:33:05Z

## Body

Hi, I found a crash in `--convert-vector-to-xegpu` pass. See the following code example.

#### Code Example
```llvm
func.func @init_host_matrices(%a: memref<?xf32>, %b: memref<?xvector<4xf32>>, %c: memref<?xf32>) {
  %c0_index = arith.constant 0 : index
  %c1_index = arith.constant 1 : index
  %c4_index = arith.constant 4 : index
  %c13_i32 = arith.constant 13 : i32
  %cvector = arith.constant dense<0.0> : vector<4xf32>
  %size = memref.dim %a, %c0_index : memref<?xf32>
  %quarter = arith.divui %size, %c4_index : index
  %vec_size = arith.constant 128 : index
  %vec_alloc = memref.alloc(%vec_size) : memref<?xvector<4xf32>>
  
  scf.for %i = %c0_index to %quarter step %c1_index {
    %i_i32 = arith.index_cast %i : index to i32
    %mod = arith.remui %i_i32, %c13_i32 : i32
    %fp = arith.sitofp %mod : i32 to f32
    %vec_idx = arith.divui %i, %c4_index : index
    %lane = arith.remui %i, %c4_index : index
    %vec_load = vector.load %vec_alloc[%vec_idx] : memref<?xvector<4xf32>>, vector<4xf32>
    %updated_vec = vector.insert %fp, %vec_load[%lane] : f32 into vector<4xf32>
    vector.store %updated_vec, %vec_alloc[%vec_idx] : memref<?xvector<4xf32>>, vector<4xf32>
    %tanh_val = math.tanh %fp : f32
    memref.store %tanh_val, %c[%i] : memref<?xf32>
  }
  
  scf.for %j = %c0_index to %vec_size step %c1_index {
    %loaded_vec = vector.load %vec_alloc[%j] : memref<?xvector<4xf32>>, vector<4xf32>
    vector.store %loaded_vec, %b[%j] : memref<?xvector<4xf32>>, vector<4xf32>
  }
  
  memref.dealloc %vec_alloc : memref<?xvector<4xf32>>
  
  return
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt tf32TensorCoreGemm_tf32TensorCoreGemm_4.mlir --convert-vector-to-xegpu
 #0 0x000058c15d06c87d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x000058c15d06cdab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x000058c15d06ac64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000058c15d06d449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007c7ac2245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007c7ac229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007c7ac229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007c7ac229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007c7ac224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007c7ac22288ff abort ./stdlib/abort.c:81:7
#10 0x00007c7ac222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007c7ac223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000058c16ba0cf88 mlir::Type::getIntOrFloatBitWidth() const /mlir/lib/IR/Types.cpp:0:3
#13 0x000058c15e293662 mlir::detail::ShapedTypeTrait<mlir::MemRefType>::getElementTypeBitWidth() const /build_all/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:609:7
#14 0x000058c16512b1e7 (anonymous namespace)::createNdDescriptor(mlir::PatternRewriter&, mlir::Location, mlir::xegpu::TensorDescType, mlir::detail::TypedValue<mlir::MemRefType>) /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:136:56
#15 0x000058c1651350a6 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&) const /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:793:36
#16 0x000058c164f4c1db mlir::detail::OpOrInterfaceRewritePatternBase<mlir::vector::StoreOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#17 0x000058c16b425228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#18 0x000058c16b424ed5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#19 0x000058c15d092a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#20 0x000058c16b426a2b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#21 0x000058c16b4232ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#22 0x000058c16b3dbde5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#23 0x000058c16b3db0f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#24 0x000058c16b3db065 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x000058c15d092a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x000058c16b3da75b void mlir::MLIRContext::executeAction<(anonymous namespace)::GreedyPatternRewriteIteration, long&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, long&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#27 0x000058c16b3d892e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#28 0x000058c16b3d8524 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#29 0x000058c15d263b00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#30 0x000058c165128ab0 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:849:16
#31 0x000058c16b4e5e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#32 0x000058c16b4e5e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#33 0x000058c15d092a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#34 0x000058c16b4e931b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#35 0x000058c16b4dd2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#36 0x000058c16b4dd9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#37 0x000058c16b4e32bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#38 0x000058c16b4e2846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#39 0x000058c15d176dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#40 0x000058c15d1764d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#41 0x000058c15d17618c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#42 0x000058c15d176096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#43 0x000058c16ba42c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#44 0x000058c16ba4201a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#45 0x000058c15d171e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#46 0x000058c15d172215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#47 0x000058c15d1723e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#48 0x000058c15d041485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#49 0x00007c7ac222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#50 0x00007c7ac222a28b call_init ./csu/../csu/libc-start.c:128:20
#51 0x00007c7ac222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#52 0x000058c15d0412f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash in `--convert-vector-to-xegpu` pass. See the following code example.

#### Code Example
```llvm
func.func @<!-- -->init_host_matrices(%a: memref&lt;?xf32&gt;, %b: memref&lt;?xvector&lt;4xf32&gt;&gt;, %c: memref&lt;?xf32&gt;) {
  %c0_index = arith.constant 0 : index
  %c1_index = arith.constant 1 : index
  %c4_index = arith.constant 4 : index
  %c13_i32 = arith.constant 13 : i32
  %cvector = arith.constant dense&lt;0.0&gt; : vector&lt;4xf32&gt;
  %size = memref.dim %a, %c0_index : memref&lt;?xf32&gt;
  %quarter = arith.divui %size, %c4_index : index
  %vec_size = arith.constant 128 : index
  %vec_alloc = memref.alloc(%vec_size) : memref&lt;?xvector&lt;4xf32&gt;&gt;
  
  scf.for %i = %c0_index to %quarter step %c1_index {
    %i_i32 = arith.index_cast %i : index to i32
    %mod = arith.remui %i_i32, %c13_i32 : i32
    %fp = arith.sitofp %mod : i32 to f32
    %vec_idx = arith.divui %i, %c4_index : index
    %lane = arith.remui %i, %c4_index : index
    %vec_load = vector.load %vec_alloc[%vec_idx] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    %updated_vec = vector.insert %fp, %vec_load[%lane] : f32 into vector&lt;4xf32&gt;
    vector.store %updated_vec, %vec_alloc[%vec_idx] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    %tanh_val = math.tanh %fp : f32
    memref.store %tanh_val, %c[%i] : memref&lt;?xf32&gt;
  }
  
  scf.for %j = %c0_index to %vec_size step %c1_index {
    %loaded_vec = vector.load %vec_alloc[%j] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    vector.store %loaded_vec, %b[%j] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
  }
  
  memref.dealloc %vec_alloc : memref&lt;?xvector&lt;4xf32&gt;&gt;
  
  return
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt tf32TensorCoreGemm_tf32TensorCoreGemm_4.mlir --convert-vector-to-xegpu
 #<!-- -->0 0x000058c15d06c87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x000058c15d06cdab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x000058c15d06ac64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000058c15d06d449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007c7ac2245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007c7ac229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007c7ac229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007c7ac229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007c7ac224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007c7ac22288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007c7ac222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007c7ac223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000058c16ba0cf88 mlir::Type::getIntOrFloatBitWidth() const /mlir/lib/IR/Types.cpp:0:3
#<!-- -->13 0x000058c15e293662 mlir::detail::ShapedTypeTrait&lt;mlir::MemRefType&gt;::getElementTypeBitWidth() const /build_all/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:609:7
#<!-- -->14 0x000058c16512b1e7 (anonymous namespace)::createNdDescriptor(mlir::PatternRewriter&amp;, mlir::Location, mlir::xegpu::TensorDescType, mlir::detail::TypedValue&lt;mlir::MemRefType&gt;) /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:136:56
#<!-- -->15 0x000058c1651350a6 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&amp;) const /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:793:36
#<!-- -->16 0x000058c164f4c1db mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::vector::StoreOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->17 0x000058c16b425228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->18 0x000058c16b424ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->19 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->20 0x000058c16b426a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->21 0x000058c16b4232ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->22 0x000058c16b3dbde5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->23 0x000058c16b3db0f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->24 0x000058c16b3db065 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x000058c16b3da75b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x000058c16b3d892e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->28 0x000058c16b3d8524 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->29 0x000058c15d263b00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->30 0x000058c165128ab0 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:849:16
#<!-- -->31 0x000058c16b4e5e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->32 0x000058c16b4e5e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->33 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->34 0x000058c16b4e931b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->35 0x000058c16b4dd2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->36 0x000058c16b4dd9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->37 0x000058c16b4e32bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->38 0x000058c16b4e2846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->39 0x000058c15d176dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->40 0x000058c15d1764d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->41 0x000058c15d17618c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->42 0x000058c15d176096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->43 0x000058c16ba42c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->44 0x000058c16ba4201a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->45 0x000058c15d171e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->46 0x000058c15d172215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->47 0x000058c15d1723e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->48 0x000058c15d041485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->49 0x00007c7ac222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x00007c7ac222a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x00007c7ac222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x000058c15d0412f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash in `--convert-vector-to-xegpu` pass. See the following code example.

#### Code Example
```llvm
func.func @<!-- -->init_host_matrices(%a: memref&lt;?xf32&gt;, %b: memref&lt;?xvector&lt;4xf32&gt;&gt;, %c: memref&lt;?xf32&gt;) {
  %c0_index = arith.constant 0 : index
  %c1_index = arith.constant 1 : index
  %c4_index = arith.constant 4 : index
  %c13_i32 = arith.constant 13 : i32
  %cvector = arith.constant dense&lt;0.0&gt; : vector&lt;4xf32&gt;
  %size = memref.dim %a, %c0_index : memref&lt;?xf32&gt;
  %quarter = arith.divui %size, %c4_index : index
  %vec_size = arith.constant 128 : index
  %vec_alloc = memref.alloc(%vec_size) : memref&lt;?xvector&lt;4xf32&gt;&gt;
  
  scf.for %i = %c0_index to %quarter step %c1_index {
    %i_i32 = arith.index_cast %i : index to i32
    %mod = arith.remui %i_i32, %c13_i32 : i32
    %fp = arith.sitofp %mod : i32 to f32
    %vec_idx = arith.divui %i, %c4_index : index
    %lane = arith.remui %i, %c4_index : index
    %vec_load = vector.load %vec_alloc[%vec_idx] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    %updated_vec = vector.insert %fp, %vec_load[%lane] : f32 into vector&lt;4xf32&gt;
    vector.store %updated_vec, %vec_alloc[%vec_idx] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    %tanh_val = math.tanh %fp : f32
    memref.store %tanh_val, %c[%i] : memref&lt;?xf32&gt;
  }
  
  scf.for %j = %c0_index to %vec_size step %c1_index {
    %loaded_vec = vector.load %vec_alloc[%j] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
    vector.store %loaded_vec, %b[%j] : memref&lt;?xvector&lt;4xf32&gt;&gt;, vector&lt;4xf32&gt;
  }
  
  memref.dealloc %vec_alloc : memref&lt;?xvector&lt;4xf32&gt;&gt;
  
  return
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt tf32TensorCoreGemm_tf32TensorCoreGemm_4.mlir --convert-vector-to-xegpu
 #<!-- -->0 0x000058c15d06c87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x000058c15d06cdab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x000058c15d06ac64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000058c15d06d449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007c7ac2245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007c7ac229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007c7ac229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007c7ac229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007c7ac224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007c7ac22288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007c7ac222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007c7ac223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000058c16ba0cf88 mlir::Type::getIntOrFloatBitWidth() const /mlir/lib/IR/Types.cpp:0:3
#<!-- -->13 0x000058c15e293662 mlir::detail::ShapedTypeTrait&lt;mlir::MemRefType&gt;::getElementTypeBitWidth() const /build_all/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:609:7
#<!-- -->14 0x000058c16512b1e7 (anonymous namespace)::createNdDescriptor(mlir::PatternRewriter&amp;, mlir::Location, mlir::xegpu::TensorDescType, mlir::detail::TypedValue&lt;mlir::MemRefType&gt;) /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:136:56
#<!-- -->15 0x000058c1651350a6 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&amp;) const /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:793:36
#<!-- -->16 0x000058c164f4c1db mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::vector::StoreOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->17 0x000058c16b425228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->18 0x000058c16b424ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->19 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->20 0x000058c16b426a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->21 0x000058c16b4232ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->22 0x000058c16b3dbde5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->23 0x000058c16b3db0f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->24 0x000058c16b3db065 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x000058c16b3da75b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x000058c16b3d892e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->28 0x000058c16b3d8524 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->29 0x000058c15d263b00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->30 0x000058c165128ab0 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:849:16
#<!-- -->31 0x000058c16b4e5e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->32 0x000058c16b4e5e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->33 0x000058c15d092a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->34 0x000058c16b4e931b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->35 0x000058c16b4dd2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->36 0x000058c16b4dd9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->37 0x000058c16b4e32bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->38 0x000058c16b4e2846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->39 0x000058c15d176dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->40 0x000058c15d1764d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->41 0x000058c15d17618c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->42 0x000058c15d176096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->43 0x000058c16ba42c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->44 0x000058c16ba4201a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->45 0x000058c15d171e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->46 0x000058c15d172215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->47 0x000058c15d1723e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->48 0x000058c15d041485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->49 0x00007c7ac222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x00007c7ac222a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x00007c7ac222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x000058c15d0412f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```
</details>


---

