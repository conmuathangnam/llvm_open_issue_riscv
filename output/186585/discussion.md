# [MLIR] Crash in AffineToStandard conversion when lowering gpu program

**Author:** karlls12321
**URL:** https://github.com/llvm/llvm-project/issues/186585
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-03-15T13:15:20Z

## Body

Hi, a crash occurs when I use the nvvm pipeline to lower an input program with reduction operations. See the following program for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @gemv_execution_module {
    gpu.func @row_parallel_gemv(%n: i32, %A: memref<?x?xi8>, %x: memref<?xi8>, %result: memref<?xi32>) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      
      %tid_i32 = arith.index_cast %tid : index to i32
      %bid_i32 = arith.index_cast %bid : index to i32
      %global = arith.addi %bid_i32, %tid_i32 : i32
      %global_idx = arith.index_cast %global : i32 to index
      %n_idx = arith.index_cast %n : i32 to index
      
      %active = arith.cmpi ult, %global_idx, %n_idx : index
      scf.if %active {
        %c0_i32 = arith.constant 0 : i32
        %c1_i32 = arith.constant 1 : i32
        %c0_idx = arith.constant 0 : index
        %c1_idx = arith.constant 1 : index
        %n_cast = arith.index_cast %n : i32 to index
        %summation = scf.for %col = %c0_idx to %n_cast step %c1_idx iter_args(%accum = %c0_i32) -> (i32) {
          %a_byte = memref.load %A[%global_idx, %col] : memref<?x?xi8>
          %x_byte = memref.load %x[%col] : memref<?xi8>
          %a_int = arith.extsi %a_byte : i8 to i32
          %x_int = arith.extsi %x_byte : i8 to i32
          %mul = arith.muli %a_int, %x_int : i32
          %new_accum = arith.addi %accum, %mul : i32
          scf.yield %new_accum : i32
        }
        memref.store %summation, %result[%global_idx] : memref<?xi32>
      }
      gpu.return
    }
  }
}
```

#### Reproduction Command
```bash
mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
 #0 0x000061d8e4711dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000061d8e47122eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000061d8e4710184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000061d8e4712a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007dcc83245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007dcc8329eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007dcc8329eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007dcc8329eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007dcc8324527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007dcc832288ff abort ./stdlib/abort.c:81:7
#10 0x000061d8e46eaa85 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000061d8e46eaad9 (./mlir-opt+0x4822ad9)
#12 0x000061d8f330287f (./mlir-opt+0x1343a87f)
#13 0x000061d8f2cc73e6 mlir::arith::ConstantOp::build(mlir::OpBuilder&, mlir::OperationState&, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4187:1
#14 0x000061d8f2c9878a mlir::arith::ConstantOp::create(mlir::OpBuilder&, mlir::Location, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4192:47
#15 0x000061d8f2ca7413 mlir::arith::getIdentityValue(mlir::arith::AtomicRMWKind, mlir::Type, mlir::OpBuilder&, mlir::Location, bool) /mlir/lib/Dialect/Arith/IR/ArithOps.cpp:2794:10
#16 0x000061d8ed8a2df9 (anonymous namespace)::AffineParallelLowering::matchAndRewrite(mlir::affine::AffineParallelOp, mlir::PatternRewriter&) const /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:234:11
#17 0x000061d8ed8abcab mlir::detail::OpOrInterfaceRewritePatternBase<mlir::affine::AffineParallelOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#18 0x000061d8f2ff9338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#19 0x000061d8f2ff8fe5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#20 0x000061d8e4738189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#21 0x000061d8f2ffab3b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#22 0x000061d8f2ff73ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#23 0x000061d8f2f6db28 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#24 0x000061d8f2f61a72 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#25 0x000061d8f2f6d59d (anonymous namespace)::OperationLegalizer::legalizeWithFold(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2678:12
#26 0x000061d8f2f619bb (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2599:19
#27 0x000061d8f2f6145a mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3317:26
#28 0x000061d8f2f625bd llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3416:16
#29 0x000061d8f2f6216e mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3462:26
#30 0x000061d8f2f7300c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4185:30
#31 0x000061d8f2f72f75 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x000061d8e4738189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x000061d8f2f8f4a3 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#34 0x000061d8f2f65d68 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:1
#35 0x000061d8f2f65c1c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4198:10
#36 0x000061d8f2f65e05 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4205:10
#37 0x000061d8ed89c7d0 (anonymous namespace)::LowerAffine::runOnOperation() /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:564:16
#38 0x000061d8f30bafa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#39 0x000061d8f30baf45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#40 0x000061d8e4738189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#41 0x000061d8f30be44b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#42 0x000061d8f30b23ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#43 0x000061d8f30b2b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#44 0x000061d8f30b83ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#45 0x000061d8f30b7976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#46 0x000061d8e481ce7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#47 0x000061d8e481c551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#48 0x000061d8e481c20c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#49 0x000061d8e481c116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#50 0x000061d8f36246fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#51 0x000061d8f3623aba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#52 0x000061d8e4817dc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#53 0x000061d8e48181c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#54 0x000061d8e4818398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#55 0x000061d8e46e68d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#56 0x00007dcc8322a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#57 0x00007dcc8322a28b call_init ./csu/../csu/libc-start.c:128:20
#58 0x00007dcc8322a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#59 0x000061d8e46e6745 _start (./mlir-opt+0x481e745)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: bling (karlls12321)

<details>
Hi, a crash occurs when I use the nvvm pipeline to lower an input program with reduction operations. See the following program for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @<!-- -->gemv_execution_module {
    gpu.func @<!-- -->row_parallel_gemv(%n: i32, %A: memref&lt;?x?xi8&gt;, %x: memref&lt;?xi8&gt;, %result: memref&lt;?xi32&gt;) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      
      %tid_i32 = arith.index_cast %tid : index to i32
      %bid_i32 = arith.index_cast %bid : index to i32
      %global = arith.addi %bid_i32, %tid_i32 : i32
      %global_idx = arith.index_cast %global : i32 to index
      %n_idx = arith.index_cast %n : i32 to index
      
      %active = arith.cmpi ult, %global_idx, %n_idx : index
      scf.if %active {
        %c0_i32 = arith.constant 0 : i32
        %c1_i32 = arith.constant 1 : i32
        %c0_idx = arith.constant 0 : index
        %c1_idx = arith.constant 1 : index
        %n_cast = arith.index_cast %n : i32 to index
        %summation = scf.for %col = %c0_idx to %n_cast step %c1_idx iter_args(%accum = %c0_i32) -&gt; (i32) {
          %a_byte = memref.load %A[%global_idx, %col] : memref&lt;?x?xi8&gt;
          %x_byte = memref.load %x[%col] : memref&lt;?xi8&gt;
          %a_int = arith.extsi %a_byte : i8 to i32
          %x_int = arith.extsi %x_byte : i8 to i32
          %mul = arith.muli %a_int, %x_int : i32
          %new_accum = arith.addi %accum, %mul : i32
          scf.yield %new_accum : i32
        }
        memref.store %summation, %result[%global_idx] : memref&lt;?xi32&gt;
      }
      gpu.return
    }
  }
}
```

#### Reproduction Command
```bash
mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
 #<!-- -->0 0x000061d8e4711dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000061d8e47122eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000061d8e4710184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000061d8e4712a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007dcc83245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007dcc8329eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007dcc8329eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007dcc8329eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007dcc8324527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007dcc832288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000061d8e46eaa85 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000061d8e46eaad9 (./mlir-opt+0x4822ad9)
#<!-- -->12 0x000061d8f330287f (./mlir-opt+0x1343a87f)
#<!-- -->13 0x000061d8f2cc73e6 mlir::arith::ConstantOp::build(mlir::OpBuilder&amp;, mlir::OperationState&amp;, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4187:1
#<!-- -->14 0x000061d8f2c9878a mlir::arith::ConstantOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4192:47
#<!-- -->15 0x000061d8f2ca7413 mlir::arith::getIdentityValue(mlir::arith::AtomicRMWKind, mlir::Type, mlir::OpBuilder&amp;, mlir::Location, bool) /mlir/lib/Dialect/Arith/IR/ArithOps.cpp:2794:10
#<!-- -->16 0x000061d8ed8a2df9 (anonymous namespace)::AffineParallelLowering::matchAndRewrite(mlir::affine::AffineParallelOp, mlir::PatternRewriter&amp;) const /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:234:11
#<!-- -->17 0x000061d8ed8abcab mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::affine::AffineParallelOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->18 0x000061d8f2ff9338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->19 0x000061d8f2ff8fe5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x000061d8f2ffab3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x000061d8f2ff73ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->23 0x000061d8f2f6db28 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->24 0x000061d8f2f61a72 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->25 0x000061d8f2f6d59d (anonymous namespace)::OperationLegalizer::legalizeWithFold(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2678:12
#<!-- -->26 0x000061d8f2f619bb (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2599:19
#<!-- -->27 0x000061d8f2f6145a mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3317:26
#<!-- -->28 0x000061d8f2f625bd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3416:16
#<!-- -->29 0x000061d8f2f6216e mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3462:26
#<!-- -->30 0x000061d8f2f7300c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4185:30
#<!-- -->31 0x000061d8f2f72f75 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x000061d8f2f8f4a3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x000061d8f2f65d68 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:1
#<!-- -->35 0x000061d8f2f65c1c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4198:10
#<!-- -->36 0x000061d8f2f65e05 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4205:10
#<!-- -->37 0x000061d8ed89c7d0 (anonymous namespace)::LowerAffine::runOnOperation() /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:564:16
#<!-- -->38 0x000061d8f30bafa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->39 0x000061d8f30baf45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->40 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->41 0x000061d8f30be44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->42 0x000061d8f30b23ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->43 0x000061d8f30b2b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->44 0x000061d8f30b83ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->45 0x000061d8f30b7976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->46 0x000061d8e481ce7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->47 0x000061d8e481c551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->48 0x000061d8e481c20c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->49 0x000061d8e481c116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x000061d8f36246fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->51 0x000061d8f3623aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->52 0x000061d8e4817dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->53 0x000061d8e48181c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->54 0x000061d8e4818398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->55 0x000061d8e46e68d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->56 0x00007dcc8322a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x00007dcc8322a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x00007dcc8322a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x000061d8e46e6745 _start (./mlir-opt+0x481e745)
Aborted
```
</details>


---

### Comment 2 - Men-cotton

I could not reproduce this on my side. Using the exact same commit da8d1b52d0d344068a8bd3e46a8af3a692d2efc2 and the exact mlir snippet from the issue, I ran the same command line, but it did not reach the `AffineParallelLowering` crash. Instead, the pipeline completed successfully.

Could you please double-check the reproduction setup? Could you confirm:

1. that the pasted MLIR file is exactly the file that crashed,
2. that the command line is exactly the one you used, with no extra passes or preprocessing steps,
3. and that there were no local modifications in your tree at the time of the crash?

---

### Comment 3 - karlls12321

Hi, I made a mistake when I was trying to simplify the input program. Please see the following program, as it triggers the crash bug:

```llvm
module {
  gpu.module @gemv_execution_module {
    gpu.func @row_parallel_gemv(%n: i32, %A: memref<?x?xi8>, %x: memref<?xi8>, %result: memref<?xi32>) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      
      %tid_i32 = arith.index_cast %tid : index to i32
      %bid_i32 = arith.index_cast %bid : index to i32
      %global = arith.addi %bid_i32, %tid_i32 : i32
      %global_idx = arith.index_cast %global : i32 to index
      %n_idx = arith.index_cast %n : i32 to index
      
      %active = arith.cmpi ult, %global_idx, %n_idx : index
      scf.if %active {
        %c0_i32 = arith.constant 0 : i32
        %c1_i32 = arith.constant 1 : i32
        %c0_idx = arith.constant 0 : index
        %c1_idx = arith.constant 1 : index
        %n_cast = arith.index_cast %n : i32 to index
        %summation = scf.for %col = %c0_idx to %n_cast step %c1_idx iter_args(%accum = %c0_i32) -> (i32) {
          %a_byte = memref.load %A[%global_idx, %col] : memref<?x?xi8>
          %x_byte = memref.load %x[%col] : memref<?xi8>
          %a_int = arith.extsi %a_byte : i8 to i32
          %x_int = arith.extsi %x_byte : i8 to i32
          %mul = arith.muli %a_int, %x_int : i32
          %new_accum = arith.addi %accum, %mul : i32
          scf.yield %new_accum : i32
        }
        memref.store %summation, %result[%global_idx] : memref<?xi32>
      }
      gpu.return
    }
  }
  
  func.func @host_side_gemv(%n: i32, %A: memref<?x?xi8>, %x: memref<?xi8>, %result: memref<?xi32>) {
    %c0_i32 = arith.constant 0 : i32
    %c1_i8 = arith.constant 1 : i8
    %c1_i32 = arith.constant 1 : i32
    %n_idx = arith.index_cast %n : i32 to index
    
    %sum = affine.parallel (%r) = (0) to (%n_idx) reduce ("assign") -> i32 {
      %row_idx = affine.apply affine_map<(d0) -> (d0)>(%r)
      %c0_idx = arith.constant 0 : index
      %col_idx_cast = arith.index_cast %n : i32 to index
      %c1_idx = arith.constant 1 : index
      %row_sum = scf.for %c = %c0_idx to %col_idx_cast step %c1_idx iter_args(%inner_sum = %c0_i32) -> (i32) {
        %col_idx = affine.apply affine_map<(d0) -> (d0)>(%c)
        %a = memref.load %A[%row_idx, %col_idx] : memref<?x?xi8>
        %xv = memref.load %x[%col_idx] : memref<?xi8>
        %a_ext = arith.extsi %a : i8 to i32
        %x_ext = arith.extsi %xv : i8 to i32
        %product = arith.muli %a_ext, %x_ext : i32
        %new_sum = arith.addi %inner_sum, %product : i32
        scf.yield %new_sum : i32
      }
      memref.store %row_sum, %result[%row_idx] : memref<?xi32>
      affine.yield %row_sum : i32
    }
    return
  }
}
```

---

### Comment 4 - Men-cotton

Thank you for the update! Now I confirmed that this crash is fixed by #186189.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: bling (karlls12321)

<details>
Hi, a crash occurs when I use the nvvm pipeline to lower an input program with reduction operations. See the following program for the reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  gpu.module @<!-- -->gemv_execution_module {
    gpu.func @<!-- -->row_parallel_gemv(%n: i32, %A: memref&lt;?x?xi8&gt;, %x: memref&lt;?xi8&gt;, %result: memref&lt;?xi32&gt;) {
      %tid = gpu.thread_id x
      %bid = gpu.block_id x
      
      %tid_i32 = arith.index_cast %tid : index to i32
      %bid_i32 = arith.index_cast %bid : index to i32
      %global = arith.addi %bid_i32, %tid_i32 : i32
      %global_idx = arith.index_cast %global : i32 to index
      %n_idx = arith.index_cast %n : i32 to index
      
      %active = arith.cmpi ult, %global_idx, %n_idx : index
      scf.if %active {
        %c0_i32 = arith.constant 0 : i32
        %c1_i32 = arith.constant 1 : i32
        %c0_idx = arith.constant 0 : index
        %c1_idx = arith.constant 1 : index
        %n_cast = arith.index_cast %n : i32 to index
        %summation = scf.for %col = %c0_idx to %n_cast step %c1_idx iter_args(%accum = %c0_i32) -&gt; (i32) {
          %a_byte = memref.load %A[%global_idx, %col] : memref&lt;?x?xi8&gt;
          %x_byte = memref.load %x[%col] : memref&lt;?xi8&gt;
          %a_int = arith.extsi %a_byte : i8 to i32
          %x_int = arith.extsi %x_byte : i8 to i32
          %mul = arith.muli %a_int, %x_int : i32
          %new_accum = arith.addi %accum, %mul : i32
          scf.yield %new_accum : i32
        }
        memref.store %summation, %result[%global_idx] : memref&lt;?xi32&gt;
      }
      gpu.return
    }
  }
}
```

#### Reproduction Command
```bash
mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt gemv_execution.mlir  -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm
 #<!-- -->0 0x000061d8e4711dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000061d8e47122eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000061d8e4710184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000061d8e4712a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007dcc83245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007dcc8329eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007dcc8329eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007dcc8329eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007dcc8324527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007dcc832288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000061d8e46eaa85 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000061d8e46eaad9 (./mlir-opt+0x4822ad9)
#<!-- -->12 0x000061d8f330287f (./mlir-opt+0x1343a87f)
#<!-- -->13 0x000061d8f2cc73e6 mlir::arith::ConstantOp::build(mlir::OpBuilder&amp;, mlir::OperationState&amp;, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4187:1
#<!-- -->14 0x000061d8f2c9878a mlir::arith::ConstantOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::TypedAttr) /build_new/tools/mlir/include/mlir/Dialect/Arith/IR/ArithOps.cpp.inc:4192:47
#<!-- -->15 0x000061d8f2ca7413 mlir::arith::getIdentityValue(mlir::arith::AtomicRMWKind, mlir::Type, mlir::OpBuilder&amp;, mlir::Location, bool) /mlir/lib/Dialect/Arith/IR/ArithOps.cpp:2794:10
#<!-- -->16 0x000061d8ed8a2df9 (anonymous namespace)::AffineParallelLowering::matchAndRewrite(mlir::affine::AffineParallelOp, mlir::PatternRewriter&amp;) const /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:234:11
#<!-- -->17 0x000061d8ed8abcab mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::affine::AffineParallelOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/include/mlir/IR/PatternMatch.h:299:12
#<!-- -->18 0x000061d8f2ff9338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->19 0x000061d8f2ff8fe5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x000061d8f2ffab3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x000061d8f2ff73ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->23 0x000061d8f2f6db28 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->24 0x000061d8f2f61a72 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->25 0x000061d8f2f6d59d (anonymous namespace)::OperationLegalizer::legalizeWithFold(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2678:12
#<!-- -->26 0x000061d8f2f619bb (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2599:19
#<!-- -->27 0x000061d8f2f6145a mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3317:26
#<!-- -->28 0x000061d8f2f625bd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3416:16
#<!-- -->29 0x000061d8f2f6216e mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3462:26
#<!-- -->30 0x000061d8f2f7300c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4185:30
#<!-- -->31 0x000061d8f2f72f75 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x000061d8f2f8f4a3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x000061d8f2f65d68 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:1
#<!-- -->35 0x000061d8f2f65c1c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4198:10
#<!-- -->36 0x000061d8f2f65e05 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4205:10
#<!-- -->37 0x000061d8ed89c7d0 (anonymous namespace)::LowerAffine::runOnOperation() /mlir/lib/Conversion/AffineToStandard/AffineToStandard.cpp:564:16
#<!-- -->38 0x000061d8f30bafa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->39 0x000061d8f30baf45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->40 0x000061d8e4738189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->41 0x000061d8f30be44b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->42 0x000061d8f30b23ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->43 0x000061d8f30b2b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->44 0x000061d8f30b83ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->45 0x000061d8f30b7976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->46 0x000061d8e481ce7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->47 0x000061d8e481c551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->48 0x000061d8e481c20c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->49 0x000061d8e481c116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x000061d8f36246fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->51 0x000061d8f3623aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->52 0x000061d8e4817dc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->53 0x000061d8e48181c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->54 0x000061d8e4818398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->55 0x000061d8e46e68d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->56 0x00007dcc8322a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x00007dcc8322a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x00007dcc8322a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x000061d8e46e6745 _start (./mlir-opt+0x481e745)
Aborted
```
</details>


---

