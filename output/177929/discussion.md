# [MLIR] gpu.launch verifier crash in `getPrivateAttributions`

**Author:** Zustin
**URL:** https://github.com/llvm/llvm-project/issues/177929
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-23T14:33:03Z

## Body

Hi, I found a crash bug in a gpu dialect program, see the following minimized code example.

### Input Program

```llvm
module {
  func.func @host_launch_kernel_1(%arg0: tensor<64xf32>, %arg1: memref<96xf32, strided<[1], offset: ?>>, %arg2: f32) -> tensor<64xf32> {
    %c0 = arith.constant 0 : index
    %c64 = arith.constant 64 : index
    %c96 = arith.constant 96 : index
    %grid_x = arith.ceildivui %c96, %c64 : index
    %grid_y = arith.constant 1 : index
    %grid_z = arith.constant 1 : index
    %block_x = arith.constant 16 : index
    %block_y = arith.constant 4 : index
    %block_z = arith.constant 1 : index
    %dynamic = tensor.extract_slice %arg0[0] [64] [1] : tensor<64xf32> to tensor<64xf32>
    gpu.launch blocks(%bx, %by, %bz) in (%sgx = %grid_x, %sgy = %grid_y, %sgz = %grid_z)
              threads(%tx, %ty, %tz) in (%stx = %block_x, %sty = %block_y, %stz = %block_z) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      %g_dim = gpu.grid_dim x
      %b_dim = gpu.block_dim x
      %base = arith.muli %bid, %b_dim : index
      %t1 = arith.addi %base, %tid : index
      %t2 = arith.subi %c96, %t1 : index
      %gid = arith.subi %c96, %t2 : index
      %cond = arith.cmpi ule, %gid, %c96 : index
      scf.if %cond {
        %slice = tensor.extract %arg0[%gid] : tensor<64xf32>
        %scaled = arith.mulf %slice, %arg2 : f32
        %neg = arith.negf %scaled : f32
        %abs = math.absf %neg : f32
        %scaled2 = arith.mulf %abs, %arg2 : f32
        memref.store %scaled2, %arg1[%gid] : memref<96xf32, strided<[1], offset: ?>>
      } else {
        %const = arith.constant 42.0 : f32
        %idx = arith.remui %gid, %c64 : index
        memref.store %const, %arg1[%idx] : memref<96xf32, strided<[1], offset: ?>>
      }
      gpu.terminator
    }
    %updated = tensor.insert_slice %dynamic into %arg0[0] [64] [1] : tensor<64xf32> into tensor<64xf32>
    return %updated : tensor<64xf32>
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --test-ir-visitors
 #0 0x000056354981861d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000563549818b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000563549816a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005635498191e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fb6d9a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fb6d9a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fb6d9a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fb6d9a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fb6d9a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fb6d9a288ff abort ./stdlib/abort.c:81:7
#10 0x00007fb6d9a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007fb6d9a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000563549c9564d llvm::ArrayRef<mlir::BlockArgument>::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#13 0x000056354c7e4d67 mlir::gpu::LaunchOp::getPrivateAttributions() /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#14 0x000056355590f7a6 mlir::gpu::LaunchOp::verifyRegions() /llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#15 0x0000563555aa7fc3 mlir::Op<mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<6u>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects>::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#16 0x0000563549b81345 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*>::CallImpl<llvm::LogicalResult (* const)(mlir::Operation*)>(void*, mlir::Operation*) /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#17 0x0000563549b81117 llvm::unique_function<llvm::LogicalResult (mlir::Operation*) const>::operator()(mlir::Operation*) const /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#18 0x0000563555aa66e6 mlir::RegisteredOperationName::Model<mlir::gpu::LaunchOp>::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#19 0x00005635576f8c16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#20 0x00005635576f5cbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#21 0x00005635576f5a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#22 0x00005635576f5935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#23 0x00005635576f51fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#24 0x00005635576f5061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#25 0x00005635576f5012 mlir::verify(mlir::Operation*, bool) /llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#26 0x00005635574eb068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#27 0x00005635574eaf97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&, llvm::DenseMap<mlir::Operation*, std::pair<unsigned int, unsigned int>, llvm::DenseMapInfo<mlir::Operation*, void>, llvm::detail::DenseMapPair<mlir::Operation*, std::pair<unsigned int, unsigned int>>>*, mlir::FallbackAsmResourceMap*) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#28 0x00005635574f252b mlir::Block::printAsOperand(llvm::raw_ostream&, bool) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#29 0x000056354a639bcb printBlock(mlir::Block*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#30 0x000056354a63bc2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#31 0x000056354a63bbdd void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x0000563549a2eb91 llvm::function_ref<void (mlir::Block*)>::operator()(mlir::Block*) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x0000563549a2e9d8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#34 0x0000563549a2e9b4 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#35 0x000056354a63bb82 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, mlir::Block*, void>(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#36 0x000056354a63ba1d std::enable_if<llvm::function_traits<std::decay<testNoSkipErasureCallbacks(mlir::Operation*)::$_1&>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, void>(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#37 0x000056354a63973e testNoSkipErasureCallbacks(mlir::Operation*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#38 0x000056354a6390ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#39 0x00005635571df864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#40 0x00005635571df805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#41 0x000056354983e879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#42 0x00005635571e2f2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#43 0x00005635571d6cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#44 0x00005635571d73da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#45 0x00005635571dcccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#46 0x00005635571dc256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#47 0x000056354991949b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#48 0x0000563549918b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#49 0x000056354991882c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#50 0x0000563549918736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#51 0x0000563557721cba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#52 0x000056355772107a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#53 0x0000563549914126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#54 0x0000563549914525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#55 0x00005635499146f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#56 0x00005635497ed935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#57 0x00007fb6d9a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#58 0x00007fb6d9a2a28b call_init ./csu/../csu/libc-start.c:128:20
#59 0x00007fb6d9a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#60 0x00005635497ed505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command `mlir-opt input.mlir --test-ir-visitors` and it crashed, however, if we used the command `mlir-opt input.mlir -gpu-kernel-outlining --test-ir-visitors`, the program works well.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Zustin)

<details>
Hi, I found a crash bug in a gpu dialect program, see the following minimized code example.

### Input Program

```llvm
module {
  func.func @<!-- -->host_launch_kernel_1(%arg0: tensor&lt;64xf32&gt;, %arg1: memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;, %arg2: f32) -&gt; tensor&lt;64xf32&gt; {
    %c0 = arith.constant 0 : index
    %c64 = arith.constant 64 : index
    %c96 = arith.constant 96 : index
    %grid_x = arith.ceildivui %c96, %c64 : index
    %grid_y = arith.constant 1 : index
    %grid_z = arith.constant 1 : index
    %block_x = arith.constant 16 : index
    %block_y = arith.constant 4 : index
    %block_z = arith.constant 1 : index
    %dynamic = tensor.extract_slice %arg0[0] [64] [1] : tensor&lt;64xf32&gt; to tensor&lt;64xf32&gt;
    gpu.launch blocks(%bx, %by, %bz) in (%sgx = %grid_x, %sgy = %grid_y, %sgz = %grid_z)
              threads(%tx, %ty, %tz) in (%stx = %block_x, %sty = %block_y, %stz = %block_z) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      %g_dim = gpu.grid_dim x
      %b_dim = gpu.block_dim x
      %base = arith.muli %bid, %b_dim : index
      %t1 = arith.addi %base, %tid : index
      %t2 = arith.subi %c96, %t1 : index
      %gid = arith.subi %c96, %t2 : index
      %cond = arith.cmpi ule, %gid, %c96 : index
      scf.if %cond {
        %slice = tensor.extract %arg0[%gid] : tensor&lt;64xf32&gt;
        %scaled = arith.mulf %slice, %arg2 : f32
        %neg = arith.negf %scaled : f32
        %abs = math.absf %neg : f32
        %scaled2 = arith.mulf %abs, %arg2 : f32
        memref.store %scaled2, %arg1[%gid] : memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;
      } else {
        %const = arith.constant 42.0 : f32
        %idx = arith.remui %gid, %c64 : index
        memref.store %const, %arg1[%idx] : memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;
      }
      gpu.terminator
    }
    %updated = tensor.insert_slice %dynamic into %arg0[0] [64] [1] : tensor&lt;64xf32&gt; into tensor&lt;64xf32&gt;
    return %updated : tensor&lt;64xf32&gt;
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --test-ir-visitors
 #<!-- -->0 0x000056354981861d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000563549818b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000563549816a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005635498191e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fb6d9a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fb6d9a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fb6d9a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fb6d9a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fb6d9a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fb6d9a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fb6d9a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fb6d9a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000563549c9564d llvm::ArrayRef&lt;mlir::BlockArgument&gt;::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#<!-- -->13 0x000056354c7e4d67 mlir::gpu::LaunchOp::getPrivateAttributions() /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#<!-- -->14 0x000056355590f7a6 mlir::gpu::LaunchOp::verifyRegions() /llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#<!-- -->15 0x0000563555aa7fc3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#<!-- -->16 0x0000563549b81345 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->17 0x0000563549b81117 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#<!-- -->18 0x0000563555aa66e6 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->19 0x00005635576f8c16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->20 0x00005635576f5cbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->21 0x00005635576f5a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->22 0x00005635576f5935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->23 0x00005635576f51fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->24 0x00005635576f5061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->25 0x00005635576f5012 mlir::verify(mlir::Operation*, bool) /llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->26 0x00005635574eb068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#<!-- -->27 0x00005635574eaf97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#<!-- -->28 0x00005635574f252b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#<!-- -->29 0x000056354a639bcb printBlock(mlir::Block*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->30 0x000056354a63bc2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->31 0x000056354a63bbdd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x0000563549a2eb91 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x0000563549a2e9d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->34 0x0000563549a2e9b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->35 0x000056354a63bb82 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->36 0x000056354a63ba1d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->37 0x000056354a63973e testNoSkipErasureCallbacks(mlir::Operation*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->38 0x000056354a6390ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->39 0x00005635571df864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->40 0x00005635571df805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->41 0x000056354983e879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->42 0x00005635571e2f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->43 0x00005635571d6cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->44 0x00005635571d73da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->45 0x00005635571dcccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->46 0x00005635571dc256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->47 0x000056354991949b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->48 0x0000563549918b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->49 0x000056354991882c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->50 0x0000563549918736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000563557721cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x000056355772107a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->53 0x0000563549914126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->54 0x0000563549914525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->55 0x00005635499146f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->56 0x00005635497ed935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->57 0x00007fb6d9a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->58 0x00007fb6d9a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007fb6d9a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->60 0x00005635497ed505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command `mlir-opt input.mlir --test-ir-visitors` and it crashed, however, if we used the command `mlir-opt input.mlir -gpu-kernel-outlining --test-ir-visitors`, the program works well.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (Zustin)

<details>
Hi, I found a crash bug in a gpu dialect program, see the following minimized code example.

### Input Program

```llvm
module {
  func.func @<!-- -->host_launch_kernel_1(%arg0: tensor&lt;64xf32&gt;, %arg1: memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;, %arg2: f32) -&gt; tensor&lt;64xf32&gt; {
    %c0 = arith.constant 0 : index
    %c64 = arith.constant 64 : index
    %c96 = arith.constant 96 : index
    %grid_x = arith.ceildivui %c96, %c64 : index
    %grid_y = arith.constant 1 : index
    %grid_z = arith.constant 1 : index
    %block_x = arith.constant 16 : index
    %block_y = arith.constant 4 : index
    %block_z = arith.constant 1 : index
    %dynamic = tensor.extract_slice %arg0[0] [64] [1] : tensor&lt;64xf32&gt; to tensor&lt;64xf32&gt;
    gpu.launch blocks(%bx, %by, %bz) in (%sgx = %grid_x, %sgy = %grid_y, %sgz = %grid_z)
              threads(%tx, %ty, %tz) in (%stx = %block_x, %sty = %block_y, %stz = %block_z) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      %g_dim = gpu.grid_dim x
      %b_dim = gpu.block_dim x
      %base = arith.muli %bid, %b_dim : index
      %t1 = arith.addi %base, %tid : index
      %t2 = arith.subi %c96, %t1 : index
      %gid = arith.subi %c96, %t2 : index
      %cond = arith.cmpi ule, %gid, %c96 : index
      scf.if %cond {
        %slice = tensor.extract %arg0[%gid] : tensor&lt;64xf32&gt;
        %scaled = arith.mulf %slice, %arg2 : f32
        %neg = arith.negf %scaled : f32
        %abs = math.absf %neg : f32
        %scaled2 = arith.mulf %abs, %arg2 : f32
        memref.store %scaled2, %arg1[%gid] : memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;
      } else {
        %const = arith.constant 42.0 : f32
        %idx = arith.remui %gid, %c64 : index
        memref.store %const, %arg1[%idx] : memref&lt;96xf32, strided&lt;[1], offset: ?&gt;&gt;
      }
      gpu.terminator
    }
    %updated = tensor.insert_slice %dynamic into %arg0[0] [64] [1] : tensor&lt;64xf32&gt; into tensor&lt;64xf32&gt;
    return %updated : tensor&lt;64xf32&gt;
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --test-ir-visitors
 #<!-- -->0 0x000056354981861d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000563549818b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000563549816a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005635498191e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fb6d9a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fb6d9a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fb6d9a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fb6d9a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fb6d9a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fb6d9a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fb6d9a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fb6d9a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000563549c9564d llvm::ArrayRef&lt;mlir::BlockArgument&gt;::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#<!-- -->13 0x000056354c7e4d67 mlir::gpu::LaunchOp::getPrivateAttributions() /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#<!-- -->14 0x000056355590f7a6 mlir::gpu::LaunchOp::verifyRegions() /llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#<!-- -->15 0x0000563555aa7fc3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#<!-- -->16 0x0000563549b81345 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->17 0x0000563549b81117 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#<!-- -->18 0x0000563555aa66e6 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->19 0x00005635576f8c16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->20 0x00005635576f5cbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->21 0x00005635576f5a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->22 0x00005635576f5935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->23 0x00005635576f51fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->24 0x00005635576f5061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->25 0x00005635576f5012 mlir::verify(mlir::Operation*, bool) /llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->26 0x00005635574eb068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#<!-- -->27 0x00005635574eaf97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#<!-- -->28 0x00005635574f252b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#<!-- -->29 0x000056354a639bcb printBlock(mlir::Block*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->30 0x000056354a63bc2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->31 0x000056354a63bbdd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x0000563549a2eb91 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x0000563549a2e9d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->34 0x0000563549a2e9b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->35 0x000056354a63bb82 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->36 0x000056354a63ba1d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->37 0x000056354a63973e testNoSkipErasureCallbacks(mlir::Operation*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->38 0x000056354a6390ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->39 0x00005635571df864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->40 0x00005635571df805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->41 0x000056354983e879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->42 0x00005635571e2f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->43 0x00005635571d6cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->44 0x00005635571d73da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->45 0x00005635571dcccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->46 0x00005635571dc256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->47 0x000056354991949b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->48 0x0000563549918b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->49 0x000056354991882c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->50 0x0000563549918736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000563557721cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x000056355772107a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->53 0x0000563549914126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->54 0x0000563549914525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->55 0x00005635499146f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->56 0x00005635497ed935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->57 0x00007fb6d9a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->58 0x00007fb6d9a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007fb6d9a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->60 0x00005635497ed505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command `mlir-opt input.mlir --test-ir-visitors` and it crashed, however, if we used the command `mlir-opt input.mlir -gpu-kernel-outlining --test-ir-visitors`, the program works well.
</details>


---

### Comment 3 - Men-cotton

I believe https://github.com/llvm/llvm-project/pull/182086 resolved this issue.

---

