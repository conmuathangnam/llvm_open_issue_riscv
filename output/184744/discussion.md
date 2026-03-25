# [MLIR] Crash when apply signature conversion due to OOB access in a gpu program

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/184744
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-11T13:25:05Z

## Body

Hi, I found a crash bug when applying a signature conversion on a GPU dialect program. It's caused by an out-of-bounds access in `SignatureConversion::getInputMapping`. Here's the input program to reproduce the issue:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
gpu.module @cuda_events {
  gpu.func @increment_kernel(%arg0: memref<?xi32>, %arg1: i32)
      workgroup(%workgroup_mem: memref<512xi32>)
      kernel attributes {known_block_size = [512, 1, 1], known_grid_size = [32768, 1, 1]} {
    %block_id_x = gpu.block_id x
    %thread_id_x = gpu.thread_id x
    %block_dim_x = gpu.block_dim x
    %linear_index = arith.addi %block_id_x, %thread_id_x : index
    %loaded_val = memref.load %arg0[%linear_index] : memref<?xi32>
    %updated_val = arith.addi %loaded_val, %arg1 : i32
    memref.store %updated_val, %arg0[%linear_index] : memref<?xi32>
    gpu.return
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt asyncAPI.mlir --test-legalize-type-conversion
 #0 0x00006098150b6bed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00006098150b711b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00006098150b4fb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00006098150b78b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007a261d645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007a261d69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007a261d69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007a261d69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007a261d64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007a261d6288ff abort ./stdlib/abort.c:81:7
#10 0x00007a261d62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007a261d63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000609817e063c2 llvm::SmallVectorTemplateCommon<std::optional<mlir::TypeConverter::SignatureConversion::InputMapping>, void>::operator[](unsigned long) const /llvm/include/llvm/ADT/SmallVector.h:0:5
#13 0x0000609817e0499c mlir::TypeConverter::SignatureConversion::getInputMapping(unsigned int) const /mlir/include/mlir/Transforms/DialectConversion.h:85:14
#14 0x00006098238754f7 mlir::detail::ConversionPatternRewriterImpl::applySignatureConversion(mlir::Block*, mlir::TypeConverter const*, mlir::TypeConverter::SignatureConversion&) /mlir/lib/Transforms/Utils/DialectConversion.cpp:1616:10
#15 0x0000609823878fe0 mlir::ConversionPatternRewriter::applySignatureConversion(mlir::Block*, mlir::TypeConverter::SignatureConversion&, mlir::TypeConverter const*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2218:3
#16 0x0000609823899b1b convertFuncOpTypes(mlir::FunctionOpInterface, mlir::TypeConverter const&, mlir::ConversionPatternRewriter&) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3851:36
#17 0x000060982389a899 (anonymous namespace)::AnyFunctionOpInterfaceSignatureConversion::matchAndRewrite(mlir::FunctionOpInterface, llvm::ArrayRef<mlir::Value>, mlir::ConversionPatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:3885:12
#18 0x00006098238ba0c8 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpInterfaceConversionPattern<mlir::FunctionOpInterface>, mlir::FunctionOpInterface>(mlir::OpInterfaceConversionPattern<mlir::FunctionOpInterface> const&, mlir::FunctionOpInterface, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1055:15
#19 0x00006098238b9ead mlir::OpInterfaceConversionPattern<mlir::FunctionOpInterface>::matchAndRewrite(mlir::FunctionOpInterface, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Transforms/DialectConversion.h:775:12
#20 0x00006098238b9e43 mlir::OpInterfaceConversionPattern<mlir::FunctionOpInterface>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Transforms/DialectConversion.h:762:12
#21 0x000060982387a4a9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#22 0x0000609823912738 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#23 0x00006098239123e5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#24 0x00006098150dcfb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#25 0x0000609823913f3b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#26 0x00006098239107ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#27 0x0000609823887088 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#28 0x000060982387b1a2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#29 0x000060982387aa72 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#30 0x000060982387bb3d llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#31 0x000060982387b6ee mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#32 0x000060982388c56c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#33 0x000060982388c4d5 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#34 0x00006098150dcfb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#35 0x00006098238a88d3 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#36 0x000060982387f2e8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#37 0x000060982387f19c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#38 0x000060982387f385 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#39 0x000060981550a676 (anonymous namespace)::TestTypeConversionDriver::runOnOperation() /mlir/test/lib/Dialect/Test/TestPatterns.cpp:2244:16
#40 0x00006098239d41d4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#41 0x00006098239d4175 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#42 0x00006098150dcfb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#43 0x00006098239d767b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#44 0x00006098239cb61e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#45 0x00006098239cbd3a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#46 0x00006098239d161d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#47 0x00006098239d0ba6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#48 0x00006098151c1b9b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#49 0x00006098151c1271 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#50 0x00006098151c0f2c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#51 0x00006098151c0e36 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#52 0x0000609823f3c9da llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#53 0x0000609823f3bd9a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#54 0x00006098151bcae6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#55 0x00006098151bcee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#56 0x00006098151bd0b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#57 0x000060981508b7d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#58 0x00007a261d62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#59 0x00007a261d62a28b call_init ./csu/../csu/libc-start.c:128:20
#60 0x00007a261d62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#61 0x000060981508b645 _start (./mlir-opt+0x47e6645)
Aborted
```

Used Command: `mlir-opt asyncAPI.mlir --test-legalize-type-conversion`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash bug when applying a signature conversion on a GPU dialect program. It's caused by an out-of-bounds access in `SignatureConversion::getInputMapping`. Here's the input program to reproduce the issue:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
gpu.module @<!-- -->cuda_events {
  gpu.func @<!-- -->increment_kernel(%arg0: memref&lt;?xi32&gt;, %arg1: i32)
      workgroup(%workgroup_mem: memref&lt;512xi32&gt;)
      kernel attributes {known_block_size = [512, 1, 1], known_grid_size = [32768, 1, 1]} {
    %block_id_x = gpu.block_id x
    %thread_id_x = gpu.thread_id x
    %block_dim_x = gpu.block_dim x
    %linear_index = arith.addi %block_id_x, %thread_id_x : index
    %loaded_val = memref.load %arg0[%linear_index] : memref&lt;?xi32&gt;
    %updated_val = arith.addi %loaded_val, %arg1 : i32
    memref.store %updated_val, %arg0[%linear_index] : memref&lt;?xi32&gt;
    gpu.return
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt asyncAPI.mlir --test-legalize-type-conversion
 #<!-- -->0 0x00006098150b6bed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00006098150b711b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00006098150b4fb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006098150b78b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007a261d645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007a261d69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007a261d69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007a261d69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007a261d64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007a261d6288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007a261d62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007a261d63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000609817e063c2 llvm::SmallVectorTemplateCommon&lt;std::optional&lt;mlir::TypeConverter::SignatureConversion::InputMapping&gt;, void&gt;::operator[](unsigned long) const /llvm/include/llvm/ADT/SmallVector.h:0:5
#<!-- -->13 0x0000609817e0499c mlir::TypeConverter::SignatureConversion::getInputMapping(unsigned int) const /mlir/include/mlir/Transforms/DialectConversion.h:85:14
#<!-- -->14 0x00006098238754f7 mlir::detail::ConversionPatternRewriterImpl::applySignatureConversion(mlir::Block*, mlir::TypeConverter const*, mlir::TypeConverter::SignatureConversion&amp;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:1616:10
#<!-- -->15 0x0000609823878fe0 mlir::ConversionPatternRewriter::applySignatureConversion(mlir::Block*, mlir::TypeConverter::SignatureConversion&amp;, mlir::TypeConverter const*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2218:3
#<!-- -->16 0x0000609823899b1b convertFuncOpTypes(mlir::FunctionOpInterface, mlir::TypeConverter const&amp;, mlir::ConversionPatternRewriter&amp;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3851:36
#<!-- -->17 0x000060982389a899 (anonymous namespace)::AnyFunctionOpInterfaceSignatureConversion::matchAndRewrite(mlir::FunctionOpInterface, llvm::ArrayRef&lt;mlir::Value&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:3885:12
#<!-- -->18 0x00006098238ba0c8 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpInterfaceConversionPattern&lt;mlir::FunctionOpInterface&gt;, mlir::FunctionOpInterface&gt;(mlir::OpInterfaceConversionPattern&lt;mlir::FunctionOpInterface&gt; const&amp;, mlir::FunctionOpInterface, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1055:15
#<!-- -->19 0x00006098238b9ead mlir::OpInterfaceConversionPattern&lt;mlir::FunctionOpInterface&gt;::matchAndRewrite(mlir::FunctionOpInterface, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Transforms/DialectConversion.h:775:12
#<!-- -->20 0x00006098238b9e43 mlir::OpInterfaceConversionPattern&lt;mlir::FunctionOpInterface&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Transforms/DialectConversion.h:762:12
#<!-- -->21 0x000060982387a4a9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#<!-- -->22 0x0000609823912738 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->23 0x00006098239123e5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->24 0x00006098150dcfb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->25 0x0000609823913f3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->26 0x00006098239107ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->27 0x0000609823887088 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->28 0x000060982387b1a2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->29 0x000060982387aa72 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#<!-- -->30 0x000060982387bb3d llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#<!-- -->31 0x000060982387b6ee mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#<!-- -->32 0x000060982388c56c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#<!-- -->33 0x000060982388c4d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x00006098150dcfb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x00006098238a88d3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x000060982387f2e8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#<!-- -->37 0x000060982387f19c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#<!-- -->38 0x000060982387f385 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#<!-- -->39 0x000060981550a676 (anonymous namespace)::TestTypeConversionDriver::runOnOperation() /mlir/test/lib/Dialect/Test/TestPatterns.cpp:2244:16
#<!-- -->40 0x00006098239d41d4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->41 0x00006098239d4175 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->42 0x00006098150dcfb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->43 0x00006098239d767b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->44 0x00006098239cb61e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->45 0x00006098239cbd3a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->46 0x00006098239d161d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->47 0x00006098239d0ba6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->48 0x00006098151c1b9b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->49 0x00006098151c1271 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->50 0x00006098151c0f2c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->51 0x00006098151c0e36 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->52 0x0000609823f3c9da llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->53 0x0000609823f3bd9a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->54 0x00006098151bcae6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->55 0x00006098151bcee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->56 0x00006098151bd0b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->57 0x000060981508b7d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->58 0x00007a261d62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->59 0x00007a261d62a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007a261d62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->61 0x000060981508b645 _start (./mlir-opt+0x47e6645)
Aborted
```

Used Command: `mlir-opt asyncAPI.mlir --test-legalize-type-conversion`.
</details>


---

