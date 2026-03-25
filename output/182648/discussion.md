# [MLIR] Crash in IntegerRangeAnalysis::visitNonControlFlowArguments when processing GPU kernels

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/182648
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T16:00:10Z

## Body

Hi, I found a crash bug in the `IntegerRangeAnalysis::visitNonControlFlowArguments`, please see the following code example.

#### Input Program
```llvm
#map = affine_map<(d0) -> (d0)>
#map1 = affine_map<(d0) -> (d0 * 2)>

module attributes {gpu.container_module} {
  gpu.module @zero_init {
    gpu.func @zero_init_kernel(%A: memref<?xf64>, %lda: i64, %colsA: i64) kernel {
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %cf64 = arith.constant 0.0 : f64
      
      %blockIdx = gpu.block_id x
      %blockDim = gpu.block_dim x
      %threadIdx = gpu.thread_id x
      %gridDim = gpu.grid_dim x
      
      // Use affine.apply for index computation
      %tid_affine = affine.apply #map(%blockIdx)
      %tid_affine2 = affine.apply #map1(%tid_affine)
      %tid = arith.addi %tid_affine2, %threadIdx : index
      
      %total_elements = arith.muli %lda, %colsA : i64
      %total_elements_idx = arith.index_cast %total_elements : i64 to index
      
      // Affine.for loop with custom bounds
      affine.for %i = 0 to %total_elements_idx {
        %scaled_idx = affine.apply #map1(%i)
        %in_bounds = arith.cmpi ult, %scaled_idx, %total_elements_idx : index
        scf.if %in_bounds {
          memref.store %cf64, %A[%scaled_idx] : memref<?xf64>
        }
      }
      
      // Additional block for synchronization
      gpu.barrier
      gpu.return
    }
  }
  
  func.func private @helper(%arg0: index) -> index {
    %c2 = arith.constant 2 : index
    %ret = arith.muli %arg0, %c2 : index
    func.return %ret : index
  }
  
  func.func @launch_zero_init(%d_A: memref<?xf64>, %lda: i64, %colsA: i64) {
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    
    %total_elements = arith.muli %lda, %colsA : i64
    %total_elements_idx = arith.index_cast %total_elements : i64 to index
    
    // Use external helper to compute grid size
    %grid_base = arith.ceildivui %total_elements_idx, %c64 : index
    %grid_size = call @helper(%grid_base) : (index) -> index
    
    gpu.launch_func @zero_init::@zero_init_kernel
        blocks in (%grid_size, %c1, %c1)
        threads in (%c64, %c1, %c1)
        args(%d_A : memref<?xf64>, %lda : i64, %colsA : i64)
    func.return
  }
}
```

#### Crash Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt cuSOLVERMp_mp_getrf_getrs.mlir --arith-int-range-narrowing --convert-gpu-to-nvvm
 #0 0x00005663bd20c87d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005663bd20cdab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005663bd20ac64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005663bd20d449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x0000798e04e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000798e04e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000798e04e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000798e04e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000798e04e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000798e04e288ff abort ./stdlib/abort.c:81:7
#10 0x0000798e052df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#11 0x00005663bdcea944 std::_Optional_base_impl<llvm::SmallVector<mlir::OpFoldResult, 6u>, std::_Optional_base<llvm::SmallVector<mlir::OpFoldResult, 6u>, false, false>>::_M_get() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:479:2
#12 0x00005663cb877105 std::optional<llvm::SmallVector<mlir::OpFoldResult, 6u>>::operator*() && /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:980:9
#13 0x00005663cb87374f mlir::dataflow::IntegerRangeAnalysis::visitNonControlFlowArguments(mlir::Operation*, mlir::RegionSuccessor const&, mlir::ValueRange, llvm::ArrayRef<mlir::dataflow::IntegerValueRangeLattice*>) /mlir/lib/Analysis/DataFlow/IntegerRangeAnalysis.cpp:222:45
#14 0x00005663bd4e68b2 mlir::dataflow::SparseForwardDataFlowAnalysis<mlir::dataflow::IntegerValueRangeLattice>::visitNonControlFlowArgumentsImpl(mlir::Operation*, mlir::RegionSuccessor const&, mlir::ValueRange, llvm::ArrayRef<mlir::dataflow::AbstractSparseLattice*>) /mlir/include/mlir/Analysis/DataFlow/SparseAnalysis.h:392:3
#15 0x00005663cb881eef mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitRegionSuccessors(mlir::ProgramPoint*, mlir::RegionBranchOpInterface, mlir::RegionSuccessor, llvm::ArrayRef<mlir::dataflow::AbstractSparseLattice*>) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:336:7
#16 0x00005663cb880cb0 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitBlock(mlir::Block*) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:184:14
#17 0x00005663cb881254 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visit(mlir::ProgramPoint*) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:112:10
#18 0x00005663cb810992 mlir::DataFlowSolver::initializeAndRun(mlir::Operation*) /mlir/lib/Analysis/DataFlowFramework.cpp:144:26
#19 0x00005663c9232e6a (anonymous namespace)::IntRangeNarrowingPass::runOnOperation() /mlir/lib/Dialect/Arith/Transforms/IntRangeOptimizations.cpp:696:23
#20 0x00005663cb685e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#21 0x00005663cb685e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#22 0x00005663bd232a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#23 0x00005663cb68931b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#24 0x00005663cb67d2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#25 0x00005663cb67d9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#26 0x00005663cb6832bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#27 0x00005663cb682846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#28 0x00005663bd316dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#29 0x00005663bd3164d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#30 0x00005663bd31618c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#31 0x00005663bd316096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#32 0x00005663cbbe2c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#33 0x00005663cbbe201a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#34 0x00005663bd311e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#35 0x00005663bd312215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#36 0x00005663bd3123e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#37 0x00005663bd1e1485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#38 0x0000798e04e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x0000798e04e2a28b call_init ./csu/../csu/libc-start.c:128:20
#40 0x0000798e04e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x00005663bd1e12f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

Version: b52591b329ba3a3085041b2452edcd3c27556ca8

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash bug in the `IntegerRangeAnalysis::visitNonControlFlowArguments`, please see the following code example.

#### Input Program
```llvm
#map = affine_map&lt;(d0) -&gt; (d0)&gt;
#map1 = affine_map&lt;(d0) -&gt; (d0 * 2)&gt;

module attributes {gpu.container_module} {
  gpu.module @<!-- -->zero_init {
    gpu.func @<!-- -->zero_init_kernel(%A: memref&lt;?xf64&gt;, %lda: i64, %colsA: i64) kernel {
      %c0 = arith.constant 0 : index
      %c1 = arith.constant 1 : index
      %cf64 = arith.constant 0.0 : f64
      
      %blockIdx = gpu.block_id x
      %blockDim = gpu.block_dim x
      %threadIdx = gpu.thread_id x
      %gridDim = gpu.grid_dim x
      
      // Use affine.apply for index computation
      %tid_affine = affine.apply #map(%blockIdx)
      %tid_affine2 = affine.apply #map1(%tid_affine)
      %tid = arith.addi %tid_affine2, %threadIdx : index
      
      %total_elements = arith.muli %lda, %colsA : i64
      %total_elements_idx = arith.index_cast %total_elements : i64 to index
      
      // Affine.for loop with custom bounds
      affine.for %i = 0 to %total_elements_idx {
        %scaled_idx = affine.apply #map1(%i)
        %in_bounds = arith.cmpi ult, %scaled_idx, %total_elements_idx : index
        scf.if %in_bounds {
          memref.store %cf64, %A[%scaled_idx] : memref&lt;?xf64&gt;
        }
      }
      
      // Additional block for synchronization
      gpu.barrier
      gpu.return
    }
  }
  
  func.func private @<!-- -->helper(%arg0: index) -&gt; index {
    %c2 = arith.constant 2 : index
    %ret = arith.muli %arg0, %c2 : index
    func.return %ret : index
  }
  
  func.func @<!-- -->launch_zero_init(%d_A: memref&lt;?xf64&gt;, %lda: i64, %colsA: i64) {
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    
    %total_elements = arith.muli %lda, %colsA : i64
    %total_elements_idx = arith.index_cast %total_elements : i64 to index
    
    // Use external helper to compute grid size
    %grid_base = arith.ceildivui %total_elements_idx, %c64 : index
    %grid_size = call @<!-- -->helper(%grid_base) : (index) -&gt; index
    
    gpu.launch_func @<!-- -->zero_init::@<!-- -->zero_init_kernel
        blocks in (%grid_size, %c1, %c1)
        threads in (%c64, %c1, %c1)
        args(%d_A : memref&lt;?xf64&gt;, %lda : i64, %colsA : i64)
    func.return
  }
}
```

#### Crash Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt cuSOLVERMp_mp_getrf_getrs.mlir --arith-int-range-narrowing --convert-gpu-to-nvvm
 #<!-- -->0 0x00005663bd20c87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005663bd20cdab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005663bd20ac64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005663bd20d449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000798e04e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000798e04e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000798e04e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000798e04e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000798e04e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000798e04e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000798e052df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#<!-- -->11 0x00005663bdcea944 std::_Optional_base_impl&lt;llvm::SmallVector&lt;mlir::OpFoldResult, 6u&gt;, std::_Optional_base&lt;llvm::SmallVector&lt;mlir::OpFoldResult, 6u&gt;, false, false&gt;&gt;::_M_get() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:479:2
#<!-- -->12 0x00005663cb877105 std::optional&lt;llvm::SmallVector&lt;mlir::OpFoldResult, 6u&gt;&gt;::operator*() &amp;&amp; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:980:9
#<!-- -->13 0x00005663cb87374f mlir::dataflow::IntegerRangeAnalysis::visitNonControlFlowArguments(mlir::Operation*, mlir::RegionSuccessor const&amp;, mlir::ValueRange, llvm::ArrayRef&lt;mlir::dataflow::IntegerValueRangeLattice*&gt;) /mlir/lib/Analysis/DataFlow/IntegerRangeAnalysis.cpp:222:45
#<!-- -->14 0x00005663bd4e68b2 mlir::dataflow::SparseForwardDataFlowAnalysis&lt;mlir::dataflow::IntegerValueRangeLattice&gt;::visitNonControlFlowArgumentsImpl(mlir::Operation*, mlir::RegionSuccessor const&amp;, mlir::ValueRange, llvm::ArrayRef&lt;mlir::dataflow::AbstractSparseLattice*&gt;) /mlir/include/mlir/Analysis/DataFlow/SparseAnalysis.h:392:3
#<!-- -->15 0x00005663cb881eef mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitRegionSuccessors(mlir::ProgramPoint*, mlir::RegionBranchOpInterface, mlir::RegionSuccessor, llvm::ArrayRef&lt;mlir::dataflow::AbstractSparseLattice*&gt;) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:336:7
#<!-- -->16 0x00005663cb880cb0 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitBlock(mlir::Block*) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:184:14
#<!-- -->17 0x00005663cb881254 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visit(mlir::ProgramPoint*) /mlir/lib/Analysis/DataFlow/SparseAnalysis.cpp:112:10
#<!-- -->18 0x00005663cb810992 mlir::DataFlowSolver::initializeAndRun(mlir::Operation*) /mlir/lib/Analysis/DataFlowFramework.cpp:144:26
#<!-- -->19 0x00005663c9232e6a (anonymous namespace)::IntRangeNarrowingPass::runOnOperation() /mlir/lib/Dialect/Arith/Transforms/IntRangeOptimizations.cpp:696:23
#<!-- -->20 0x00005663cb685e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->21 0x00005663cb685e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->22 0x00005663bd232a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->23 0x00005663cb68931b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->24 0x00005663cb67d2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->25 0x00005663cb67d9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->26 0x00005663cb6832bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->27 0x00005663cb682846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->28 0x00005663bd316dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->29 0x00005663bd3164d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->30 0x00005663bd31618c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->31 0x00005663bd316096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->32 0x00005663cbbe2c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->33 0x00005663cbbe201a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->34 0x00005663bd311e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->35 0x00005663bd312215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->36 0x00005663bd3123e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->37 0x00005663bd1e1485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->38 0x0000798e04e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x0000798e04e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x0000798e04e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x00005663bd1e12f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

Version: b52591b329ba3a3085041b2452edcd3c27556ca8
</details>


---

### Comment 2 - Men-cotton

No crash at HEAD. I believe #183660 resolved this.

---

