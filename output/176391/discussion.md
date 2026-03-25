# [MLIR] A crash bug in gpu dialect

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/176391
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-26T01:48:15Z

## Body

I used MLIR to analyze the following program and triggered a crash bug.

### Input Program

```llvm
module {
  func.func @main(%arg0: tensor<4x4xf32>, %arg1: tensor<*xf32>, %arg2: tensor<*xf32>, %arg3: tensor<2x3xi32>, %arg4: tensor<2x3xi32>) {
    %c1 = arith.constant 1 : index
    %0 = llvm.mlir.constant(0 : i32) : i32
    %1 = llvm.mlir.poison : vector<128xi32>
    %2 = llvm.mlir.constant(1 : index) : i64
    %3 = llvm.mlir.constant(dense<3.40282347E+38> : vector<128xf32>) : vector<128xf32>
    %alloc = memref.alloc() {alignment = 64 : i64} : memref<4xf32, #spirv.storage_class<StorageBuffer>>
    %5 = builtin.unrealized_conversion_cast %alloc : memref<4xf32, #spirv.storage_class<StorageBuffer>> to !llvm.struct<(ptr, ptr, i64, array<1 x i64>, array<1 x i64>)>
    gpu.launch blocks(%arg5, %arg6, %arg7) in (%arg11 = %c1, %arg12 = %c1, %arg13 = %c1)
               threads(%arg8, %arg9, %arg10) in (%arg14 = %c1, %arg15 = %c1, %arg16 = %c1) {
      // ...
      gpu.terminator
    }
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -sccp -test-ir-visitors
 #0 0x0000557f9459d61d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000557f9459db4b PrintStackTraceSignalHandler(void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000557f9459ba1f llvm::sys::RunSignalHandlers() /path/to/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000557f9459e1e9 SignalHandler(int, siginfo_t*, void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f99bd445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f99bd49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f99bd49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f99bd49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f99bd44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f99bd4288ff abort ./stdlib/abort.c:81:7
#10 0x00007f99bd42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f99bd43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000557f94a1a64d llvm::ArrayRef<mlir::BlockArgument>::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /path/to/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#13 0x0000557f97569d67 mlir::gpu::LaunchOp::getPrivateAttributions() /path/to/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#14 0x0000557fa06947a6 mlir::gpu::LaunchOp::verifyRegions() /path/to/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#15 0x0000557fa082cfc3 mlir::Op<mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<6u>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects>::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#16 0x0000557f94906345 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*>::CallImpl<llvm::LogicalResult (* const)(mlir::Operation*)>(void*, mlir::Operation*) /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#17 0x0000557f94906117 llvm::unique_function<llvm::LogicalResult (mlir::Operation*) const>::operator()(mlir::Operation*) const /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#18 0x0000557fa082b6e6 mlir::RegisteredOperationName::Model<mlir::gpu::LaunchOp>::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#19 0x0000557fa247dc16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#20 0x0000557fa247acbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#21 0x0000557fa247aa00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#22 0x0000557fa247a935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#23 0x0000557fa247a1fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#24 0x0000557fa247a061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#25 0x0000557fa247a012 mlir::verify(mlir::Operation*, bool) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#26 0x0000557fa2270068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#27 0x0000557fa226ff97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&, llvm::DenseMap<mlir::Operation*, std::pair<unsigned int, unsigned int>, llvm::DenseMapInfo<mlir::Operation*, void>, llvm::detail::DenseMapPair<mlir::Operation*, std::pair<unsigned int, unsigned int>>>*, mlir::FallbackAsmResourceMap*) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#28 0x0000557fa227752b mlir::Block::printAsOperand(llvm::raw_ostream&, bool) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#29 0x0000557f953bebcb printBlock(mlir::Block*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#30 0x0000557f953c0c2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#31 0x0000557f953c0bdd void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x0000557f947b3b91 llvm::function_ref<void (mlir::Block*)>::operator()(mlir::Block*) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x0000557f947b39d8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#34 0x0000557f947b39b4 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#35 0x0000557f953c0b82 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, mlir::Block*, void>(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#36 0x0000557f953c0a1d std::enable_if<llvm::function_traits<std::decay<testNoSkipErasureCallbacks(mlir::Operation*)::$_1&>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, void>(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /path/to/llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#37 0x0000557f953be73e testNoSkipErasureCallbacks(mlir::Operation*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#38 0x0000557f953be0ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#39 0x0000557fa1f64864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#40 0x0000557fa1f64805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#41 0x0000557f945c3879 llvm::function_ref<void ()>::operator()() const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#42 0x0000557fa1f67f2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /path/to/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#43 0x0000557fa1f5bcfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#44 0x0000557fa1f5c3da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#45 0x0000557fa1f61ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#46 0x0000557fa1f61256 mlir::PassManager::run(mlir::Operation*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#47 0x0000557f9469e49b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#48 0x0000557f9469db71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#49 0x0000557f9469d82c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#50 0x0000557f9469d736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#51 0x0000557fa24a6cba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#52 0x0000557fa24a607a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /path/to/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#53 0x0000557f94699126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#54 0x0000557f94699525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#55 0x0000557f946996f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#56 0x0000557f94572935 main /path/to/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#57 0x00007f99bd42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#58 0x00007f99bd42a28b call_init ./csu/../csu/libc-start.c:128:20
#59 0x00007f99bd42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#60 0x0000557f94572505 _start (/mnt/raid/path/to/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -sccp -test-ir-visitors
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
I used MLIR to analyze the following program and triggered a crash bug.

### Input Program

```llvm
module {
  func.func @<!-- -->main(%arg0: tensor&lt;4x4xf32&gt;, %arg1: tensor&lt;*xf32&gt;, %arg2: tensor&lt;*xf32&gt;, %arg3: tensor&lt;2x3xi32&gt;, %arg4: tensor&lt;2x3xi32&gt;) {
    %c1 = arith.constant 1 : index
    %0 = llvm.mlir.constant(0 : i32) : i32
    %1 = llvm.mlir.poison : vector&lt;128xi32&gt;
    %2 = llvm.mlir.constant(1 : index) : i64
    %3 = llvm.mlir.constant(dense&lt;3.40282347E+38&gt; : vector&lt;128xf32&gt;) : vector&lt;128xf32&gt;
    %alloc = memref.alloc() {alignment = 64 : i64} : memref&lt;4xf32, #spirv.storage_class&lt;StorageBuffer&gt;&gt;
    %5 = builtin.unrealized_conversion_cast %alloc : memref&lt;4xf32, #spirv.storage_class&lt;StorageBuffer&gt;&gt; to !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt;
    gpu.launch blocks(%arg5, %arg6, %arg7) in (%arg11 = %c1, %arg12 = %c1, %arg13 = %c1)
               threads(%arg8, %arg9, %arg10) in (%arg14 = %c1, %arg15 = %c1, %arg16 = %c1) {
      // ...
      gpu.terminator
    }
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -sccp -test-ir-visitors
 #<!-- -->0 0x0000557f9459d61d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000557f9459db4b PrintStackTraceSignalHandler(void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000557f9459ba1f llvm::sys::RunSignalHandlers() /path/to/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000557f9459e1e9 SignalHandler(int, siginfo_t*, void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f99bd445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f99bd49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f99bd49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f99bd49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f99bd44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f99bd4288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f99bd42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f99bd43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000557f94a1a64d llvm::ArrayRef&lt;mlir::BlockArgument&gt;::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /path/to/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#<!-- -->13 0x0000557f97569d67 mlir::gpu::LaunchOp::getPrivateAttributions() /path/to/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#<!-- -->14 0x0000557fa06947a6 mlir::gpu::LaunchOp::verifyRegions() /path/to/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#<!-- -->15 0x0000557fa082cfc3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#<!-- -->16 0x0000557f94906345 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->17 0x0000557f94906117 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#<!-- -->18 0x0000557fa082b6e6 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->19 0x0000557fa247dc16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->20 0x0000557fa247acbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->21 0x0000557fa247aa00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->22 0x0000557fa247a935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->23 0x0000557fa247a1fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->24 0x0000557fa247a061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->25 0x0000557fa247a012 mlir::verify(mlir::Operation*, bool) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->26 0x0000557fa2270068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#<!-- -->27 0x0000557fa226ff97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#<!-- -->28 0x0000557fa227752b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#<!-- -->29 0x0000557f953bebcb printBlock(mlir::Block*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->30 0x0000557f953c0c2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->31 0x0000557f953c0bdd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x0000557f947b3b91 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x0000557f947b39d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->34 0x0000557f947b39b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->35 0x0000557f953c0b82 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->36 0x0000557f953c0a1d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /path/to/llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->37 0x0000557f953be73e testNoSkipErasureCallbacks(mlir::Operation*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->38 0x0000557f953be0ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->39 0x0000557fa1f64864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->40 0x0000557fa1f64805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->41 0x0000557f945c3879 llvm::function_ref&lt;void ()&gt;::operator()() const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->42 0x0000557fa1f67f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /path/to/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->43 0x0000557fa1f5bcfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->44 0x0000557fa1f5c3da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->45 0x0000557fa1f61ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->46 0x0000557fa1f61256 mlir::PassManager::run(mlir::Operation*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->47 0x0000557f9469e49b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->48 0x0000557f9469db71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->49 0x0000557f9469d82c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->50 0x0000557f9469d736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000557fa24a6cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x0000557fa24a607a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /path/to/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->53 0x0000557f94699126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->54 0x0000557f94699525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->55 0x0000557f946996f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->56 0x0000557f94572935 main /path/to/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->57 0x00007f99bd42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->58 0x00007f99bd42a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007f99bd42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->60 0x0000557f94572505 _start (/mnt/raid/path/to/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -sccp -test-ir-visitors
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (tisble)

<details>
I used MLIR to analyze the following program and triggered a crash bug.

### Input Program

```llvm
module {
  func.func @<!-- -->main(%arg0: tensor&lt;4x4xf32&gt;, %arg1: tensor&lt;*xf32&gt;, %arg2: tensor&lt;*xf32&gt;, %arg3: tensor&lt;2x3xi32&gt;, %arg4: tensor&lt;2x3xi32&gt;) {
    %c1 = arith.constant 1 : index
    %0 = llvm.mlir.constant(0 : i32) : i32
    %1 = llvm.mlir.poison : vector&lt;128xi32&gt;
    %2 = llvm.mlir.constant(1 : index) : i64
    %3 = llvm.mlir.constant(dense&lt;3.40282347E+38&gt; : vector&lt;128xf32&gt;) : vector&lt;128xf32&gt;
    %alloc = memref.alloc() {alignment = 64 : i64} : memref&lt;4xf32, #spirv.storage_class&lt;StorageBuffer&gt;&gt;
    %5 = builtin.unrealized_conversion_cast %alloc : memref&lt;4xf32, #spirv.storage_class&lt;StorageBuffer&gt;&gt; to !llvm.struct&lt;(ptr, ptr, i64, array&lt;1 x i64&gt;, array&lt;1 x i64&gt;)&gt;
    gpu.launch blocks(%arg5, %arg6, %arg7) in (%arg11 = %c1, %arg12 = %c1, %arg13 = %c1)
               threads(%arg8, %arg9, %arg10) in (%arg14 = %c1, %arg15 = %c1, %arg16 = %c1) {
      // ...
      gpu.terminator
    }
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -sccp -test-ir-visitors
 #<!-- -->0 0x0000557f9459d61d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000557f9459db4b PrintStackTraceSignalHandler(void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000557f9459ba1f llvm::sys::RunSignalHandlers() /path/to/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000557f9459e1e9 SignalHandler(int, siginfo_t*, void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f99bd445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f99bd49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f99bd49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f99bd49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f99bd44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f99bd4288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f99bd42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f99bd43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000557f94a1a64d llvm::ArrayRef&lt;mlir::BlockArgument&gt;::ArrayRef(mlir::BlockArgument const*, mlir::BlockArgument const*) /path/to/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:82:5
#<!-- -->13 0x0000557f97569d67 mlir::gpu::LaunchOp::getPrivateAttributions() /path/to/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.h.inc:9504:5
#<!-- -->14 0x0000557fa06947a6 mlir::gpu::LaunchOp::verifyRegions() /path/to/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:901:49
#<!-- -->15 0x0000557fa082cfc3 mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2065:39
#<!-- -->16 0x0000557f94906345 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->17 0x0000557f94906117 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /path/to/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#<!-- -->18 0x0000557fa082b6e6 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::verifyRegionInvariants(mlir::Operation*) /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->19 0x0000557fa247dc16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /path/to/llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->20 0x0000557fa247acbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->21 0x0000557fa247aa00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->22 0x0000557fa247a935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->23 0x0000557fa247a1fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->24 0x0000557fa247a061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->25 0x0000557fa247a012 mlir::verify(mlir::Operation*, bool) /path/to/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->26 0x0000557fa2270068 verifyOpAndAdjustFlags(mlir::Operation*, mlir::OpPrintingFlags) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2072:14
#<!-- -->27 0x0000557fa226ff97 mlir::AsmState::AsmState(mlir::Operation*, mlir::OpPrintingFlags const&amp;, llvm::DenseMap&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Operation*, std::pair&lt;unsigned int, unsigned int&gt;&gt;&gt;*, mlir::FallbackAsmResourceMap*) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:2084:15
#<!-- -->28 0x0000557fa227752b mlir::Block::printAsOperand(llvm::raw_ostream&amp;, bool) /path/to/llvm-project/mlir/lib/IR/AsmPrinter.cpp:4169:12
#<!-- -->29 0x0000557f953bebcb printBlock(mlir::Block*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:24:3
#<!-- -->30 0x0000557f953c0c2c testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:186:7
#<!-- -->31 0x0000557f953c0bdd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x0000557f947b3b91 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x0000557f947b39d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->34 0x0000557f947b39b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->35 0x0000557f953c0b82 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /path/to/llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->36 0x0000557f953c0a1d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /path/to/llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->37 0x0000557f953be73e testNoSkipErasureCallbacks(mlir::Operation*) /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->38 0x0000557f953be0ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /path/to/llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->39 0x0000557fa1f64864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->40 0x0000557fa1f64805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->41 0x0000557f945c3879 llvm::function_ref&lt;void ()&gt;::operator()() const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->42 0x0000557fa1f67f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /path/to/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->43 0x0000557fa1f5bcfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->44 0x0000557fa1f5c3da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->45 0x0000557fa1f61ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->46 0x0000557fa1f61256 mlir::PassManager::run(mlir::Operation*) /path/to/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->47 0x0000557f9469e49b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->48 0x0000557f9469db71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->49 0x0000557f9469d82c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->50 0x0000557f9469d736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000557fa24a6cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /path/to/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x0000557fa24a607a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /path/to/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->53 0x0000557f94699126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->54 0x0000557f94699525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->55 0x0000557f946996f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /path/to/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->56 0x0000557f94572935 main /path/to/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->57 0x00007f99bd42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->58 0x00007f99bd42a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007f99bd42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->60 0x0000557f94572505 _start (/mnt/raid/path/to/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -sccp -test-ir-visitors
```
</details>


---

### Comment 3 - woruyu

I like to fix it!

---

### Comment 4 - woruyu

Fixed in https://github.com/llvm/llvm-project/pull/181320

---

