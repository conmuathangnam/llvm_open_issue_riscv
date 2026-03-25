# [MLIR] Crash in DenseIntOrFPElementsAttr::getRaw triggered by a gpu-dialect program

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/181303
**Status:** Closed
**Labels:** mlir:spirv, crash
**Closed Date:** 2026-03-08T10:14:51Z

## Body

Hi, I found a crash bug in MLIR. See the following code example.

#### Code Example
```llvm
module {
  func.func @calculateEdgesInfo(%arg0: tensor<?xi32>, %arg1: memref<?xi32>, %arg2: memref<?xi32>, %arg3: memref<?xf32>, %arg4: memref<?xi32>, %arg5: memref<?xi32>, %arg6: i32, %arg7: i32) {
    %c0 = arith.constant 0 : index
    %cst = arith.constant 0.0 : f32
    %0 = gpu.thread_id x
    %1 = gpu.block_id x
    %2 = gpu.thread_id y
    %3 = arith.index_cast %0 : index to i32
    %4 = arith.index_cast %2 : index to i32
    %5 = arith.addi %3, %4 : i32
    %6 = arith.cmpi ne, %5, %arg7 : i32
    scf.if %6 {
      %7 = tensor.extract %arg0[%c0] : tensor<?xi32>
      %8 = memref.load %arg1[%c0] : memref<?xi32>
      %9 = arith.andi %7, %8 : i32
      %10 = arith.extui %9 : i32 to i64
      %11 = arith.index_cast %10 : i64 to index
      %12 = memref.load %arg2[%11] : memref<?xi32>
      memref.store %12, %arg4[%c0] : memref<?xi32>
      memref.store %cst, %arg3[%c0] : memref<?xf32>
    }
    gpu.barrier
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv
 #0 0x00005d6ff8e8887d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005d6ff8e88dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005d6ff8e86c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005d6ff8e89449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x0000799803c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000799803c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000799803c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000799803c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000799803c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000799803c288ff abort ./stdlib/abort.c:81:7
#10 0x0000799803c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000799803c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005d7007688ed0 mlir::DenseIntOrFPElementsAttr::getRaw(mlir::ShapedType, llvm::ArrayRef<char>) /mlir/lib/IR/BuiltinAttributes.cpp:1354:8
#13 0x00005d700768897e mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef<mlir::Attribute>) /mlir/lib/IR/BuiltinAttributes.cpp:991:10
#14 0x00005d7007677950 mlir::Builder::getZeroAttr(mlir::Type) /mlir/lib/IR/Builders.cpp:337:12
#15 0x00005d7001252c9f (anonymous namespace)::FuncOpVectorUnroll::matchAndRewrite(mlir::func::FuncOp, mlir::PatternRewriter&) const /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1090:47
#16 0x00005d6ffa4e951b mlir::detail::OpOrInterfaceRewritePatternBase<mlir::func::FuncOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#17 0x00005d7007241228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#18 0x00005d7007240ed5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#19 0x00005d6ff8eaea19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#20 0x00005d7007242a2b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#21 0x00005d700723f2ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#22 0x00005d70071f7de5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#23 0x00005d70071f70f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#24 0x00005d70071f7065 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_2>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x00005d6ff8eaea19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x00005d70071f675b void mlir::MLIRContext::executeAction<(anonymous namespace)::GreedyPatternRewriteIteration, long&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, long&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#27 0x00005d70071f492e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#28 0x00005d70071f4524 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#29 0x00005d6ff907fb00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#30 0x00005d7001245ce7 mlir::spirv::unrollVectorsInSignatures(mlir::Operation*) /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1486:10
#31 0x00005d6ff9d09f3c (anonymous namespace)::TestConvertToSPIRVPass::runOnOperation() /mlir/test/lib/Pass/TestConvertToSPIRVPass.cpp:111:42
#32 0x00005d7007301e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#33 0x00005d7007301e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#34 0x00005d6ff8eaea19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#35 0x00005d700730531b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#36 0x00005d70072f92be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#37 0x00005d70072f99da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#38 0x00005d70072ff2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#39 0x00005d70072fe846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#40 0x00005d6ff8f92dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#41 0x00005d6ff8f924d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#42 0x00005d6ff8f9218c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#43 0x00005d6ff8f92096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#44 0x00005d700785ec5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#45 0x00005d700785e01a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#46 0x00005d6ff8f8de16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#47 0x00005d6ff8f8e215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#48 0x00005d6ff8f8e3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#49 0x00005d6ff8e5d485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#50 0x0000799803c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#51 0x0000799803c2a28b call_init ./csu/../csu/libc-start.c:128:20
#52 0x0000799803c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#53 0x00005d6ff8e5d2f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
Hi, I found a crash bug in MLIR. See the following code example.

#### Code Example
```llvm
module {
  func.func @<!-- -->calculateEdgesInfo(%arg0: tensor&lt;?xi32&gt;, %arg1: memref&lt;?xi32&gt;, %arg2: memref&lt;?xi32&gt;, %arg3: memref&lt;?xf32&gt;, %arg4: memref&lt;?xi32&gt;, %arg5: memref&lt;?xi32&gt;, %arg6: i32, %arg7: i32) {
    %c0 = arith.constant 0 : index
    %cst = arith.constant 0.0 : f32
    %0 = gpu.thread_id x
    %1 = gpu.block_id x
    %2 = gpu.thread_id y
    %3 = arith.index_cast %0 : index to i32
    %4 = arith.index_cast %2 : index to i32
    %5 = arith.addi %3, %4 : i32
    %6 = arith.cmpi ne, %5, %arg7 : i32
    scf.if %6 {
      %7 = tensor.extract %arg0[%c0] : tensor&lt;?xi32&gt;
      %8 = memref.load %arg1[%c0] : memref&lt;?xi32&gt;
      %9 = arith.andi %7, %8 : i32
      %10 = arith.extui %9 : i32 to i64
      %11 = arith.index_cast %10 : i64 to index
      %12 = memref.load %arg2[%11] : memref&lt;?xi32&gt;
      memref.store %12, %arg4[%c0] : memref&lt;?xi32&gt;
      memref.store %cst, %arg3[%c0] : memref&lt;?xf32&gt;
    }
    gpu.barrier
    return
  }
}
```

#### Stack Trace
```bash
mlir-opt: /mlir/lib/IR/BuiltinAttributes.cpp:1353: static DenseElementsAttr mlir::DenseIntOrFPElementsAttr::getRaw(ShapedType, ArrayRef&lt;char&gt;): Assertion `type.hasStaticShape() &amp;&amp; "type must have static shape"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv
 #<!-- -->0 0x00005d6ff8e8887d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005d6ff8e88dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005d6ff8e86c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005d6ff8e89449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000799803c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000799803c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000799803c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000799803c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000799803c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000799803c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000799803c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000799803c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005d7007688ed0 mlir::DenseIntOrFPElementsAttr::getRaw(mlir::ShapedType, llvm::ArrayRef&lt;char&gt;) /mlir/lib/IR/BuiltinAttributes.cpp:1354:8
#<!-- -->13 0x00005d700768897e mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef&lt;mlir::Attribute&gt;) /mlir/lib/IR/BuiltinAttributes.cpp:991:10
#<!-- -->14 0x00005d7007677950 mlir::Builder::getZeroAttr(mlir::Type) /mlir/lib/IR/Builders.cpp:337:12
#<!-- -->15 0x00005d7001252c9f (anonymous namespace)::FuncOpVectorUnroll::matchAndRewrite(mlir::func::FuncOp, mlir::PatternRewriter&amp;) const /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1090:47
#<!-- -->16 0x00005d6ffa4e951b mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->17 0x00005d7007241228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->18 0x00005d7007240ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->19 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->20 0x00005d7007242a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->21 0x00005d700723f2ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->22 0x00005d70071f7de5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->23 0x00005d70071f70f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->24 0x00005d70071f7065 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005d70071f675b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x00005d70071f492e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->28 0x00005d70071f4524 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->29 0x00005d6ff907fb00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->30 0x00005d7001245ce7 mlir::spirv::unrollVectorsInSignatures(mlir::Operation*) /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1486:10
#<!-- -->31 0x00005d6ff9d09f3c (anonymous namespace)::TestConvertToSPIRVPass::runOnOperation() /mlir/test/lib/Pass/TestConvertToSPIRVPass.cpp:111:42
#<!-- -->32 0x00005d7007301e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->33 0x00005d7007301e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x00005d700730531b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x00005d70072f92be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->37 0x00005d70072f99da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->38 0x00005d70072ff2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->39 0x00005d70072fe846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->40 0x00005d6ff8f92dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->41 0x00005d6ff8f924d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->42 0x00005d6ff8f9218c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->43 0x00005d6ff8f92096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->44 0x00005d700785ec5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->45 0x00005d700785e01a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->46 0x00005d6ff8f8de16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->47 0x00005d6ff8f8e215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->48 0x00005d6ff8f8e3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->49 0x00005d6ff8e5d485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->50 0x0000799803c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x0000799803c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->52 0x0000799803c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->53 0x00005d6ff8e5d2f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv`
</details>


---

### Comment 2 - Men-cotton

I confirmed that this crash is fixed by https://github.com/llvm/llvm-project/pull/184626.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-spirv

Author: None (tisble)

<details>
Hi, I found a crash bug in MLIR. See the following code example.

#### Code Example
```llvm
module {
  func.func @<!-- -->calculateEdgesInfo(%arg0: tensor&lt;?xi32&gt;, %arg1: memref&lt;?xi32&gt;, %arg2: memref&lt;?xi32&gt;, %arg3: memref&lt;?xf32&gt;, %arg4: memref&lt;?xi32&gt;, %arg5: memref&lt;?xi32&gt;, %arg6: i32, %arg7: i32) {
    %c0 = arith.constant 0 : index
    %cst = arith.constant 0.0 : f32
    %0 = gpu.thread_id x
    %1 = gpu.block_id x
    %2 = gpu.thread_id y
    %3 = arith.index_cast %0 : index to i32
    %4 = arith.index_cast %2 : index to i32
    %5 = arith.addi %3, %4 : i32
    %6 = arith.cmpi ne, %5, %arg7 : i32
    scf.if %6 {
      %7 = tensor.extract %arg0[%c0] : tensor&lt;?xi32&gt;
      %8 = memref.load %arg1[%c0] : memref&lt;?xi32&gt;
      %9 = arith.andi %7, %8 : i32
      %10 = arith.extui %9 : i32 to i64
      %11 = arith.index_cast %10 : i64 to index
      %12 = memref.load %arg2[%11] : memref&lt;?xi32&gt;
      memref.store %12, %arg4[%c0] : memref&lt;?xi32&gt;
      memref.store %cst, %arg3[%c0] : memref&lt;?xf32&gt;
    }
    gpu.barrier
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv
 #<!-- -->0 0x00005d6ff8e8887d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005d6ff8e88dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005d6ff8e86c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005d6ff8e89449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000799803c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000799803c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000799803c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000799803c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000799803c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000799803c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000799803c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000799803c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005d7007688ed0 mlir::DenseIntOrFPElementsAttr::getRaw(mlir::ShapedType, llvm::ArrayRef&lt;char&gt;) /mlir/lib/IR/BuiltinAttributes.cpp:1354:8
#<!-- -->13 0x00005d700768897e mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef&lt;mlir::Attribute&gt;) /mlir/lib/IR/BuiltinAttributes.cpp:991:10
#<!-- -->14 0x00005d7007677950 mlir::Builder::getZeroAttr(mlir::Type) /mlir/lib/IR/Builders.cpp:337:12
#<!-- -->15 0x00005d7001252c9f (anonymous namespace)::FuncOpVectorUnroll::matchAndRewrite(mlir::func::FuncOp, mlir::PatternRewriter&amp;) const /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1090:47
#<!-- -->16 0x00005d6ffa4e951b mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->17 0x00005d7007241228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->18 0x00005d7007240ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->19 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->20 0x00005d7007242a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->21 0x00005d700723f2ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->22 0x00005d70071f7de5 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:619:17
#<!-- -->23 0x00005d70071f70f7 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2::operator()() const /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:889:31
#<!-- -->24 0x00005d70071f7065 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_2&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005d70071f675b void mlir::MLIRContext::executeAction&lt;(anonymous namespace)::GreedyPatternRewriteIteration, long&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, long&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x00005d70071f492e (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:902:3
#<!-- -->28 0x00005d70071f4524 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:934:47
#<!-- -->29 0x00005d6ff907fb00 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) /mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->30 0x00005d7001245ce7 mlir::spirv::unrollVectorsInSignatures(mlir::Operation*) /mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1486:10
#<!-- -->31 0x00005d6ff9d09f3c (anonymous namespace)::TestConvertToSPIRVPass::runOnOperation() /mlir/test/lib/Pass/TestConvertToSPIRVPass.cpp:111:42
#<!-- -->32 0x00005d7007301e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->33 0x00005d7007301e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x00005d6ff8eaea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x00005d700730531b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x00005d70072f92be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->37 0x00005d70072f99da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->38 0x00005d70072ff2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->39 0x00005d70072fe846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->40 0x00005d6ff8f92dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->41 0x00005d6ff8f924d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->42 0x00005d6ff8f9218c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->43 0x00005d6ff8f92096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->44 0x00005d700785ec5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->45 0x00005d700785e01a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->46 0x00005d6ff8f8de16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->47 0x00005d6ff8f8e215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->48 0x00005d6ff8f8e3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->49 0x00005d6ff8e5d485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->50 0x0000799803c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x0000799803c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->52 0x0000799803c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->53 0x00005d6ff8e5d2f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --test-vulkan-runner-pipeline=spirv-webgpu-prepare --convert-gpu-to-spirv`
</details>


---

