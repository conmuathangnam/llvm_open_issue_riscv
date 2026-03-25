# [MLIR] `convertFuncOpToLLVMFuncOp` asserts with `!resAttrDicts.empty() && "expected array to be non-empty"' 

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/176858
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-11T13:06:37Z

## Body

I encountered a crash while using MLIR. The reproducer is the input program below:

```mlir
"builtin.module"() ({
  "func.func"() ({
    "gpu.barrier"() : () -> ()
    "func.return"() : () -> ()
  }) {sym_name = "main", function_type = () -> (), arg_attrs = [], res_attrs = []} : () -> ()
}) {} : () -> ()
```

**Stack Trace**
```
mlir-opt: /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:386: FailureOr<LLVM::LLVMFuncOp> mlir::convertFuncOpToLLVMFuncOp(FunctionOpInterface, ConversionPatternRewriter &, const LLVMTypeConverter &, SymbolTableCollection *): Assertion `!resAttrDicts.empty() && "expected array to be non-empty"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-func-to-llvm
 #0 0x000056499524461d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000564995244b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000564995242a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005649952451e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fa2d9445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fa2d949eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fa2d949eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fa2d949eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fa2d944527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fa2d94288ff abort ./stdlib/abort.c:81:7
#10 0x00007fa2d942881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007fa2d943b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000056499d83b5b9 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&, mlir::LLVMTypeConverter const&, mlir::SymbolTableCollection*) /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:387:16
#13 0x000056499d840c22 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&) const /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:490:45
#14 0x00005649953dc969 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>, mlir::func::FuncOp>(mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp> const&, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:1025:15
#15 0x00005649953db583 mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:246:12
#16 0x00005649953db4dc mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:232:12
#17 0x00005649a2ab3c59 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2359:10
#18 0x00005649a2b4c7d8 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#19 0x00005649a2b4c485 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#20 0x000056499526a879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#21 0x00005649a2b4e00b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#22 0x00005649a2b4a86a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#23 0x00005649a2abffa8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2797:21
#24 0x00005649a2ab4922 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2563:17
#25 0x00005649a2ab421b mlir::OperationConverter::convert(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3318:26
#26 0x00005649a2ab4c7b mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3415:16
#27 0x00005649a2ac63ac applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4137:30
#28 0x00005649a2ac6315 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#29 0x000056499526a879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#30 0x00005649a2ae3443 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#31 0x00005649a2ab87a8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4141:1
#32 0x00005649a2ab865c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4150:10
#33 0x00005649a2ab8845 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4157:10
#34 0x0000564997c6e6a9 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /llvm-project/mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#35 0x00005649a2c0b864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#36 0x00005649a2c0b805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#37 0x000056499526a879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#38 0x00005649a2c0ef2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#39 0x00005649a2c02cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#40 0x00005649a2c033da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#41 0x00005649a2c08ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#42 0x00005649a2c08256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#43 0x000056499534549b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#44 0x0000564995344b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#45 0x000056499534482c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#46 0x0000564995344736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#47 0x00005649a314dcba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#48 0x00005649a314d07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#49 0x0000564995340126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#50 0x0000564995340525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#51 0x00005649953406f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#52 0x0000564995219935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#53 0x00007fa2d942a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#54 0x00007fa2d942a28b call_init ./csu/../csu/libc-start.c:128:20
#55 0x00007fa2d942a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#56 0x0000564995219505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: AnnaDev (LynnBroe)

<details>
I encountered a crash while using MLIR. The reproducer is the input program below:

```llvm
"builtin.module"() ({
  "func.func"() ({
    "gpu.barrier"() : () -&gt; ()
    "func.return"() : () -&gt; ()
  }) {sym_name = "main", function_type = () -&gt; (), arg_attrs = [], res_attrs = []} : () -&gt; ()
}) {} : () -&gt; ()
```

**Stack Trace**
```bash
mlir-opt: /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:386: FailureOr&lt;LLVM::LLVMFuncOp&gt; mlir::convertFuncOpToLLVMFuncOp(FunctionOpInterface, ConversionPatternRewriter &amp;, const LLVMTypeConverter &amp;, SymbolTableCollection *): Assertion `!resAttrDicts.empty() &amp;&amp; "expected array to be non-empty"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-func-to-llvm
 #<!-- -->0 0x000056499524461d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000564995244b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000564995242a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005649952451e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fa2d9445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fa2d949eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fa2d949eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fa2d949eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fa2d944527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fa2d94288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fa2d942881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fa2d943b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000056499d83b5b9 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&amp;, mlir::LLVMTypeConverter const&amp;, mlir::SymbolTableCollection*) /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:387:16
#<!-- -->13 0x000056499d840c22 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:490:45
#<!-- -->14 0x00005649953dc969 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;, mlir::func::FuncOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt; const&amp;, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:1025:15
#<!-- -->15 0x00005649953db583 mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:246:12
#<!-- -->16 0x00005649953db4dc mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:232:12
#<!-- -->17 0x00005649a2ab3c59 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2359:10
#<!-- -->18 0x00005649a2b4c7d8 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->19 0x00005649a2b4c485 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x000056499526a879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x00005649a2b4e00b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x00005649a2b4a86a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->23 0x00005649a2abffa8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2797:21
#<!-- -->24 0x00005649a2ab4922 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2563:17
#<!-- -->25 0x00005649a2ab421b mlir::OperationConverter::convert(mlir::Operation*) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3318:26
#<!-- -->26 0x00005649a2ab4c7b mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3415:16
#<!-- -->27 0x00005649a2ac63ac applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4137:30
#<!-- -->28 0x00005649a2ac6315 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->29 0x000056499526a879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->30 0x00005649a2ae3443 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->31 0x00005649a2ab87a8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4141:1
#<!-- -->32 0x00005649a2ab865c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4150:10
#<!-- -->33 0x00005649a2ab8845 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4157:10
#<!-- -->34 0x0000564997c6e6a9 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /llvm-project/mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#<!-- -->35 0x00005649a2c0b864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->36 0x00005649a2c0b805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->37 0x000056499526a879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->38 0x00005649a2c0ef2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->39 0x00005649a2c02cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->40 0x00005649a2c033da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->41 0x00005649a2c08ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->42 0x00005649a2c08256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->43 0x000056499534549b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->44 0x0000564995344b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->45 0x000056499534482c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->46 0x0000564995344736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->47 0x00005649a314dcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->48 0x00005649a314d07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->49 0x0000564995340126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->50 0x0000564995340525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->51 0x00005649953406f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->52 0x0000564995219935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->53 0x00007fa2d942a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->54 0x00007fa2d942a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->55 0x00007fa2d942a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->56 0x0000564995219505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```
</details>


---

### Comment 2 - Men-cotton

I confirmed that this crash is fixed by #185219.

---

