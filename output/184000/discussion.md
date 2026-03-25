# [MLIR] Crash in VectorToXeGPU: getIntOrFloatBitWidth() called on non-int/float type

**Author:** Zustin
**URL:** https://github.com/llvm/llvm-project/issues/184000
**Status:** Closed
**Labels:** duplicate, crash, mlir:xegpu
**Closed Date:** 2026-03-01T19:47:09Z

## Body

Hi, I found a crash bug when lowering a gpu program to xegpu-dialect program. See the following code example:

Git version: https://github.com/llvm/llvm-project/commit/7370091a43e5cbcb4cf56fb83b62e056903cb918
Used command: `mlir-opt tf32TensorCoreGemm.mlir --convert-vector-to-xegpu`

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
  
  %dead_start = arith.constant 100 : index
  %dead_end = arith.constant 100 : index
  scf.for %dead = %dead_start to %dead_end step %c1_index {}
  
  return
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt tf32TensorCoreGemm.mlir --convert-vector-to-xegpu
 #0 0x000057d8e2517596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #1 0x000057d8e25fbe34 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #2 0x000057d8e25fd2cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #3 0x000057d8e25fd2cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #4 0x0000735fb6845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000735fb689eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000735fb689eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000735fb689eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000735fb684527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000735fb68288ff abort ./stdlib/abort.c:81:7
#10 0x0000735fb682881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000735fb683b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000057d8f6bc8860 mlir::Type::getIntOrFloatBitWidth() const /mlir/lib/IR/Types.cpp:125:3
#13 0x000057d8ed490dd0 mlir::detail::ShapedTypeTrait<mlir::MemRefType>::getElementTypeBitWidth() const /build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:609:7
#14 0x000057d8ed490dd0 (anonymous namespace)::createNdDescriptor(mlir::PatternRewriter&, mlir::Location, mlir::xegpu::TensorDescType, mlir::detail::TypedValue<mlir::MemRefType>) /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:136:31
#15 0x000057d8ed4979b1 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&) const /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:0:36
#16 0x000057d8f63944e5 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:0:31
#17 0x000057d8f63944e5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#18 0x000057d8f63910fa mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:197:5
#19 0x000057d8f6336b49 llvm::LogicalResult::succeeded() const /llvm/include/llvm/Support/LogicalResult.h:40:45
#20 0x000057d8f6336b49 llvm::succeeded(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:67:61
#21 0x000057d8f6336b49 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:621:9
#22 0x000057d8f6334ca5 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:11
#23 0x000057d8f6334ca5 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#24 0x000057d8f632e50e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#25 0x000057d8f632e50e mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#26 0x000057d8e2857f3f mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:212:15
#27 0x000057d8ed48ca04 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:849:16
#28 0x000057d8f6472593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#29 0x000057d8f6472593 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#30 0x000057d8f646227e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#31 0x000057d8f6464026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#32 0x000057d8f6464026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#33 0x000057d8f6464026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#34 0x000057d8f646fbb4 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#35 0x000057d8f646ec34 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#36 0x000057d8e27c4f63 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#37 0x000057d8e27c4f63 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#38 0x000057d8e27c4f63 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#39 0x000057d8e27c37d6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#40 0x000057d8e27c37d6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#41 0x000057d8e27c37d6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x000057d8f6c0794a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x000057d8f6c0794a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x000057d8e27bdb40 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#45 0x000057d8e27be508 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#46 0x000057d8e27bece3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#47 0x000057d8e25b7947 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#48 0x0000735fb682a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x0000735fb682a28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x0000735fb682a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x000057d8e24cdd85 _start (./mlir-opt+0x7a85d85)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Zustin)

<details>
Hi, I found a crash bug when lowering a gpu program to xegpu-dialect program. See the following code example:

Git version: https://github.com/llvm/llvm-project/commit/7370091a43e5cbcb4cf56fb83b62e056903cb918
Used command: `mlir-opt tf32TensorCoreGemm.mlir --convert-vector-to-xegpu`

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
  
  %dead_start = arith.constant 100 : index
  %dead_end = arith.constant 100 : index
  scf.for %dead = %dead_start to %dead_end step %c1_index {}
  
  return
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt tf32TensorCoreGemm.mlir --convert-vector-to-xegpu
 #<!-- -->0 0x000057d8e2517596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #<!-- -->1 0x000057d8e25fbe34 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #<!-- -->2 0x000057d8e25fd2cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->3 0x000057d8e25fd2cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #<!-- -->4 0x0000735fb6845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000735fb689eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000735fb689eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000735fb689eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000735fb684527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000735fb68288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000735fb682881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000735fb683b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000057d8f6bc8860 mlir::Type::getIntOrFloatBitWidth() const /mlir/lib/IR/Types.cpp:125:3
#<!-- -->13 0x000057d8ed490dd0 mlir::detail::ShapedTypeTrait&lt;mlir::MemRefType&gt;::getElementTypeBitWidth() const /build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:609:7
#<!-- -->14 0x000057d8ed490dd0 (anonymous namespace)::createNdDescriptor(mlir::PatternRewriter&amp;, mlir::Location, mlir::xegpu::TensorDescType, mlir::detail::TypedValue&lt;mlir::MemRefType&gt;) /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:136:31
#<!-- -->15 0x000057d8ed4979b1 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&amp;) const /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:0:36
#<!-- -->16 0x000057d8f63944e5 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:0:31
#<!-- -->17 0x000057d8f63944e5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->18 0x000057d8f63910fa mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:197:5
#<!-- -->19 0x000057d8f6336b49 llvm::LogicalResult::succeeded() const /llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->20 0x000057d8f6336b49 llvm::succeeded(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:67:61
#<!-- -->21 0x000057d8f6336b49 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:621:9
#<!-- -->22 0x000057d8f6334ca5 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:11
#<!-- -->23 0x000057d8f6334ca5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->24 0x000057d8f632e50e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->25 0x000057d8f632e50e mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->26 0x000057d8e2857f3f mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:212:15
#<!-- -->27 0x000057d8ed48ca04 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() /mlir/lib/Conversion/VectorToXeGPU/VectorToXeGPU.cpp:849:16
#<!-- -->28 0x000057d8f6472593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#<!-- -->29 0x000057d8f6472593 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x000057d8f646227e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->31 0x000057d8f6464026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->32 0x000057d8f6464026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->33 0x000057d8f6464026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->34 0x000057d8f646fbb4 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->35 0x000057d8f646ec34 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#<!-- -->36 0x000057d8e27c4f63 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->37 0x000057d8e27c4f63 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->38 0x000057d8e27c4f63 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#<!-- -->39 0x000057d8e27c37d6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x000057d8e27c37d6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x000057d8e27c37d6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000057d8f6c0794a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000057d8f6c0794a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x000057d8e27bdb40 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x000057d8e27be508 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x000057d8e27bece3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x000057d8e25b7947 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->48 0x0000735fb682a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x0000735fb682a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x0000735fb682a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x000057d8e24cdd85 _start (./mlir-opt+0x7a85d85)
Aborted
```
</details>


---

### Comment 2 - sweiglbosker

weird, this should have been fixed by #173922. will take a look

---

