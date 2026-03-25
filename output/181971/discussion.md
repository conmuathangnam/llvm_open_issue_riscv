# [MLIR] Crash in the LaunchOp::verifyRegions of GPU dialect

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/181971
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-20T00:40:09Z

## Body

Hi, I encounter a crash bug in the `LaunchOp::verifyRegions` of GPU dialect. See the following code example.

```llvm
module {
  func.func @host_launch_hgemm_t_8x8_sliced_k_f16x4_kernel(%arg0: memref<?x?xf16>, %arg1: memref<?x?xf16>, %arg2: memref<?x?xf16>) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %M = memref.dim %arg0, %c0 : memref<?x?xf16>
    %N = memref.dim %arg1, %c1 : memref<?x?xf16>
    %grid = arith.muli %M, %N : index
    gpu.launch blocks(%bx, %by, %bz) in (%grid_sz_x = %grid, %grid_sz_y = %c1, %grid_sz_z = %c1)
                threads(%tx, %ty, %tz) in (%block_sz_x = %c2, %block_sz_y = %c2, %block_sz_z = %c1)
                workgroup(%s : memref<4x4xf16, 3>) {
      %idx = arith.muli %bx, %c2 : index
      %idy = arith.muli %by, %c2 : index
      %idx_t = arith.addi %idx, %tx : index
      %idy_t = arith.addi %idy, %ty : index
      %cond_m = arith.cmpi ult, %idx_t, %M : index
      %cond_n = arith.cmpi ult, %idy_t, %N : index
      %cond = arith.andi %cond_m, %cond_n : i1
      scf.if %cond {
        %val = memref.load %arg0[%idx_t, %c0] : memref<?x?xf16>
        %val2 = memref.load %arg1[%c0, %idy_t] : memref<?x?xf16>
        %prod = arith.mulf %val, %val2 : f16
        memref.store %prod, %s[%tx, %ty] : memref<4x4xf16, 3>
        gpu.barrier
        %sum = memref.load %s[%c0, %c0] : memref<4x4xf16, 3>
        %sum2 = memref.load %s[%c1, %c1] : memref<4x4xf16, 3>
        %total = arith.addf %sum, %sum2 : f16
        memref.store %total, %arg2[%idx_t, %idy_t] : memref<?x?xf16>
      }
      gpu.terminator
    }
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt hgemm_2.mlir --test-ir-visitors
 #0 0x00005921fdac087d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005921fdac0dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005921fdabec64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005921fdac1449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x000077c92aa45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000059220a6438ce verifyAttributions(mlir::Operation*, llvm::ArrayRef<mlir::BlockArgument>, mlir::gpu::AddressSpace) /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:567:16
 #6 0x000059220a643551 mlir::gpu::LaunchOp::verifyRegions() /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:902:14
 #7 0x000059220a7dead3 mlir::Op<mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<6u>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects>::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OpDefinition.h:2065:39
 #8 0x00005921fde45ce5 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*>::CallImpl<llvm::LogicalResult (* const)(mlir::Operation*)>(void*, mlir::Operation*) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
 #9 0x00005921fde45ab7 llvm::unique_function<llvm::LogicalResult (mlir::Operation*) const>::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/FunctionExtras.h:390:12
#10 0x000059220a7dd266 mlir::RegisteredOperationName::Model<mlir::gpu::LaunchOp>::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OperationSupport.h:558:14
#11 0x000059220c46e3f6 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /mlir/include/mlir/IR/OperationSupport.h:317:23
#12 0x000059220c46b72f (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /mlir/lib/IR/Verifier.cpp:236:48
#13 0x000059220c46b470 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /mlir/lib/IR/Verifier.cpp:287:53
#14 0x000059220c46b3a5 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /mlir/lib/IR/Verifier.cpp:276:16
#15 0x000059220c46ac6a (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /mlir/lib/IR/Verifier.cpp:287:15
#16 0x000059220c46aad1 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /mlir/lib/IR/Verifier.cpp:81:14
#17 0x000059220c46aa82 mlir::verify(mlir::Operation*, bool) /mlir/lib/IR/Verifier.cpp:425:19
#18 0x000059220c25c928 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /mlir/lib/IR/AsmPrinter.cpp:2099:14
#19 0x000059220c25c857 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&, llvm::DenseMap<mlir::Operation*, std::pair<unsigned int, unsigned int>, llvm::DenseMapInfo<mlir::Operation*, void>, llvm::detail::DenseMapPair<mlir::Operation*, std::pair<unsigned int, unsigned int>>>*, mlir::FallbackAsmResourceMap*) /mlir/lib/IR/AsmPrinter.cpp:2111:15
#20 0x000059220c263a4b mlir::Block::printAsOperand(llvm::raw_ostream&, bool) /mlir/lib/IR/AsmPrinter.cpp:4206:12
#21 0x00005921fe9311ab printBlock(mlir::Block*) /mlir/test/lib/IR/TestVisitors.cpp:24:3
#22 0x00005921fe93320c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:186:7
#23 0x00005921fe9331bd void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#24 0x00005921fdce6791 llvm::function_ref<void (mlir::Block*)>::operator()(mlir::Block*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#25 0x00005921fdce65d8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:5
#26 0x00005921fdce65b4 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#27 0x00005921fe933162 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, mlir::Block*, void>(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /mlir/include/mlir/IR/Visitors.h:278:3
#28 0x00005921fe932ffd std::enable_if<llvm::function_traits<std::decay<testNoSkipErasureCallbacks(mlir::Operation*)::$_1&>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, void>(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /mlir/include/mlir/IR/Operation.h:798:5
#29 0x00005921fe930d1e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:204:3
#30 0x00005921fe93068d (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:275:3
#31 0x000059220bf39e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#32 0x000059220bf39e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#33 0x00005921fdae6a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#34 0x000059220bf3d31b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#35 0x000059220bf312be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#36 0x000059220bf319da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#37 0x000059220bf372bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#38 0x000059220bf36846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#39 0x00005921fdbcadfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#40 0x00005921fdbca4d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#41 0x00005921fdbca18c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#42 0x00005921fdbca096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#43 0x000059220c496c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#44 0x000059220c49601a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#45 0x00005921fdbc5e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#46 0x00005921fdbc6215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#47 0x00005921fdbc63e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#48 0x00005921fda95485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#49 0x000077c92aa2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#50 0x000077c92aa2a28b call_init ./csu/../csu/libc-start.c:128:20
#51 0x000077c92aa2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#52 0x00005921fda952f5 _start (/build_all/bin/mlir-opt+0x46a92f5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
Hi, I encounter a crash bug in the `LaunchOp::verifyRegions` of GPU dialect. See the following code example.

```llvm
module {
  func.func @<!-- -->host_launch_hgemm_t_8x8_sliced_k_f16x4_kernel(%arg0: memref&lt;?x?xf16&gt;, %arg1: memref&lt;?x?xf16&gt;, %arg2: memref&lt;?x?xf16&gt;) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %M = memref.dim %arg0, %c0 : memref&lt;?x?xf16&gt;
    %N = memref.dim %arg1, %c1 : memref&lt;?x?xf16&gt;
    %grid = arith.muli %M, %N : index
    gpu.launch blocks(%bx, %by, %bz) in (%grid_sz_x = %grid, %grid_sz_y = %c1, %grid_sz_z = %c1)
                threads(%tx, %ty, %tz) in (%block_sz_x = %c2, %block_sz_y = %c2, %block_sz_z = %c1)
                workgroup(%s : memref&lt;4x4xf16, 3&gt;) {
      %idx = arith.muli %bx, %c2 : index
      %idy = arith.muli %by, %c2 : index
      %idx_t = arith.addi %idx, %tx : index
      %idy_t = arith.addi %idy, %ty : index
      %cond_m = arith.cmpi ult, %idx_t, %M : index
      %cond_n = arith.cmpi ult, %idy_t, %N : index
      %cond = arith.andi %cond_m, %cond_n : i1
      scf.if %cond {
        %val = memref.load %arg0[%idx_t, %c0] : memref&lt;?x?xf16&gt;
        %val2 = memref.load %arg1[%c0, %idy_t] : memref&lt;?x?xf16&gt;
        %prod = arith.mulf %val, %val2 : f16
        memref.store %prod, %s[%tx, %ty] : memref&lt;4x4xf16, 3&gt;
        gpu.barrier
        %sum = memref.load %s[%c0, %c0] : memref&lt;4x4xf16, 3&gt;
        %sum2 = memref.load %s[%c1, %c1] : memref&lt;4x4xf16, 3&gt;
        %total = arith.addf %sum, %sum2 : f16
        memref.store %total, %arg2[%idx_t, %idy_t] : memref&lt;?x?xf16&gt;
      }
      gpu.terminator
    }
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt hgemm_2.mlir --test-ir-visitors
 #<!-- -->0 0x00005921fdac087d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005921fdac0dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005921fdabec64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005921fdac1449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x000077c92aa45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000059220a6438ce verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace) /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:567:16
 #<!-- -->6 0x000059220a643551 mlir::gpu::LaunchOp::verifyRegions() /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:902:14
 #<!-- -->7 0x000059220a7dead3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OpDefinition.h:2065:39
 #<!-- -->8 0x00005921fde45ce5 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
 #<!-- -->9 0x00005921fde45ab7 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/FunctionExtras.h:390:12
#<!-- -->10 0x000059220a7dd266 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->11 0x000059220c46e3f6 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->12 0x000059220c46b72f (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->13 0x000059220c46b470 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->14 0x000059220c46b3a5 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->15 0x000059220c46ac6a (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->16 0x000059220c46aad1 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->17 0x000059220c46aa82 mlir::verify(mlir::Operation*, bool) /mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->18 0x000059220c25c928 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /mlir/lib/IR/AsmPrinter.cpp:2099:14
#<!-- -->19 0x000059220c25c857 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /mlir/lib/IR/AsmPrinter.cpp:2111:15
#<!-- -->20 0x000059220c263a4b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /mlir/lib/IR/AsmPrinter.cpp:4206:12
#<!-- -->21 0x00005921fe9311ab printBlock(mlir::Block*) /mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->22 0x00005921fe93320c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->23 0x00005921fe9331bd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->24 0x00005921fdce6791 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->25 0x00005921fdce65d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->26 0x00005921fdce65b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->27 0x00005921fe933162 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->28 0x00005921fe932ffd std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->29 0x00005921fe930d1e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->30 0x00005921fe93068d (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->31 0x000059220bf39e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->32 0x000059220bf39e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->33 0x00005921fdae6a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->34 0x000059220bf3d31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->35 0x000059220bf312be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->36 0x000059220bf319da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->37 0x000059220bf372bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->38 0x000059220bf36846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->39 0x00005921fdbcadfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->40 0x00005921fdbca4d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->41 0x00005921fdbca18c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->42 0x00005921fdbca096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->43 0x000059220c496c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->44 0x000059220c49601a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->45 0x00005921fdbc5e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->46 0x00005921fdbc6215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->47 0x00005921fdbc63e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->48 0x00005921fda95485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->49 0x000077c92aa2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x000077c92aa2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x000077c92aa2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x00005921fda952f5 _start (/build_all/bin/mlir-opt+0x46a92f5)
```
</details>


---

### Comment 2 - EugeneZelenko

@tisble: Please include version that you run. It'll be good idea to provide reproducer on https://godbolt.org.

---

### Comment 3 - tisble

Thanks for your reminder, my version is: b52591b329ba3a3085041b2452edcd3c27556ca8

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (tisble)

<details>
Hi, I encounter a crash bug in the `LaunchOp::verifyRegions` of GPU dialect. See the following code example.

```llvm
module {
  func.func @<!-- -->host_launch_hgemm_t_8x8_sliced_k_f16x4_kernel(%arg0: memref&lt;?x?xf16&gt;, %arg1: memref&lt;?x?xf16&gt;, %arg2: memref&lt;?x?xf16&gt;) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %M = memref.dim %arg0, %c0 : memref&lt;?x?xf16&gt;
    %N = memref.dim %arg1, %c1 : memref&lt;?x?xf16&gt;
    %grid = arith.muli %M, %N : index
    gpu.launch blocks(%bx, %by, %bz) in (%grid_sz_x = %grid, %grid_sz_y = %c1, %grid_sz_z = %c1)
                threads(%tx, %ty, %tz) in (%block_sz_x = %c2, %block_sz_y = %c2, %block_sz_z = %c1)
                workgroup(%s : memref&lt;4x4xf16, 3&gt;) {
      %idx = arith.muli %bx, %c2 : index
      %idy = arith.muli %by, %c2 : index
      %idx_t = arith.addi %idx, %tx : index
      %idy_t = arith.addi %idy, %ty : index
      %cond_m = arith.cmpi ult, %idx_t, %M : index
      %cond_n = arith.cmpi ult, %idy_t, %N : index
      %cond = arith.andi %cond_m, %cond_n : i1
      scf.if %cond {
        %val = memref.load %arg0[%idx_t, %c0] : memref&lt;?x?xf16&gt;
        %val2 = memref.load %arg1[%c0, %idy_t] : memref&lt;?x?xf16&gt;
        %prod = arith.mulf %val, %val2 : f16
        memref.store %prod, %s[%tx, %ty] : memref&lt;4x4xf16, 3&gt;
        gpu.barrier
        %sum = memref.load %s[%c0, %c0] : memref&lt;4x4xf16, 3&gt;
        %sum2 = memref.load %s[%c1, %c1] : memref&lt;4x4xf16, 3&gt;
        %total = arith.addf %sum, %sum2 : f16
        memref.store %total, %arg2[%idx_t, %idy_t] : memref&lt;?x?xf16&gt;
      }
      gpu.terminator
    }
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt hgemm_2.mlir --test-ir-visitors
 #<!-- -->0 0x00005921fdac087d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005921fdac0dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005921fdabec64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005921fdac1449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x000077c92aa45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000059220a6438ce verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace) /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:567:16
 #<!-- -->6 0x000059220a643551 mlir::gpu::LaunchOp::verifyRegions() /mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:902:14
 #<!-- -->7 0x000059220a7dead3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OpDefinition.h:2065:39
 #<!-- -->8 0x00005921fde45ce5 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
 #<!-- -->9 0x00005921fde45ab7 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/FunctionExtras.h:390:12
#<!-- -->10 0x000059220a7dd266 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->11 0x000059220c46e3f6 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->12 0x000059220c46b72f (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->13 0x000059220c46b470 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->14 0x000059220c46b3a5 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->15 0x000059220c46ac6a (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->16 0x000059220c46aad1 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->17 0x000059220c46aa82 mlir::verify(mlir::Operation*, bool) /mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->18 0x000059220c25c928 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /mlir/lib/IR/AsmPrinter.cpp:2099:14
#<!-- -->19 0x000059220c25c857 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /mlir/lib/IR/AsmPrinter.cpp:2111:15
#<!-- -->20 0x000059220c263a4b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /mlir/lib/IR/AsmPrinter.cpp:4206:12
#<!-- -->21 0x00005921fe9311ab printBlock(mlir::Block*) /mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->22 0x00005921fe93320c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->23 0x00005921fe9331bd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->24 0x00005921fdce6791 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->25 0x00005921fdce65d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->26 0x00005921fdce65b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->27 0x00005921fe933162 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->28 0x00005921fe932ffd std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->29 0x00005921fe930d1e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->30 0x00005921fe93068d (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->31 0x000059220bf39e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->32 0x000059220bf39e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->33 0x00005921fdae6a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->34 0x000059220bf3d31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->35 0x000059220bf312be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->36 0x000059220bf319da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->37 0x000059220bf372bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->38 0x000059220bf36846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->39 0x00005921fdbcadfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->40 0x00005921fdbca4d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->41 0x00005921fdbca18c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->42 0x00005921fdbca096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->43 0x000059220c496c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->44 0x000059220c49601a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->45 0x00005921fdbc5e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->46 0x00005921fdbc6215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->47 0x00005921fdbc63e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->48 0x00005921fda95485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->49 0x000077c92aa2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x000077c92aa2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x000077c92aa2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x00005921fda952f5 _start (/build_all/bin/mlir-opt+0x46a92f5)
```
</details>


---

