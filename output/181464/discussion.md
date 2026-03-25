# [MLIR] Crash in FuncToLLVM conversion during -gpu-lower-to-nvvm-pipeline when creating LLVMFuncOp

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/181464
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I found a crash in the pass `-gpu-lower-to-nvvm-pipeline`. See the following code example:

#### Code Example
```llvm
module {
  func.func @host() attributes {linkage = #llvm.linkage<internal>} {
    %0 = arith.constant 1.0 : f64
    %1 = arith.constant 2.0 : f64
    %2 = arith.cmpf "ogt", %0, %1 : f64
    %3 = arith.select %2, %0, %1 : f64
    return
  }
  gpu.module @gpu_mod {
    gpu.func @gpu_kernel() kernel {
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt ext_integer.mlir -gpu-lower-to-nvvm-pipeline
 #0 0x00005747e6ece87d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005747e6ecedab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005747e6eccc64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005747e6ecf449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x000078a20e445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000078a20e49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000078a20e49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000078a20e49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000078a20e44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000078a20e4288ff abort ./stdlib/abort.c:81:7
#10 0x000078a20e42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000078a20e43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005747f56cb793 mlir::DictionaryAttr::sortInPlace(llvm::SmallVectorImpl<mlir::NamedAttribute>&) /mlir/lib/IR/BuiltinAttributes.cpp:128:10
#13 0x00005747f583884a mlir::NamedAttrList::getDictionary(mlir::MLIRContext*) const /mlir/lib/IR/OperationSupport.cpp:56:5
#14 0x00005747f58299f0 mlir::Operation::create(mlir::OperationState const&) /mlir/lib/IR/Operation.cpp:37:31
#15 0x00005747f56be391 mlir::OpBuilder::create(mlir::OperationState const&) /mlir/lib/IR/Builders.cpp:458:17
#16 0x00005747f404a460 mlir::LLVM::LLVMFuncOp::create(mlir::OpBuilder&, mlir::Location, llvm::StringRef, mlir::Type, mlir::LLVM::linkage::Linkage, bool, mlir::LLVM::cconv::CConv, mlir::SymbolRefAttr, llvm::ArrayRef<mlir::NamedAttribute>, llvm::ArrayRef<mlir::DictionaryAttr>, std::optional<unsigned long>) /build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:33007:55
#17 0x00005747efc8b3c9 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&, mlir::LLVMTypeConverter const&, mlir::SymbolTableCollection*) /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:360:20
#18 0x00005747efc90c52 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&) const /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:493:45
#19 0x00005747e7073f59 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp, false>, mlir::func::FuncOp>(mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp, false> const&, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#20 0x00005747e7072b73 mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp, false>::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:265:12
#21 0x00005747e7072acc mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp, false>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:251:12
#22 0x00005747f51eff69 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2409:10
#23 0x00005747f5287228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#24 0x00005747f5286ed5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x00005747e6ef4a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x00005747f5288a2b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#27 0x00005747f52852ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#28 0x00005747f51fca58 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2829:21
#29 0x00005747f51f0c62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2608:17
#30 0x00005747f51f0532 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3306:26
#31 0x00005747f51f15fd llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3407:16
#32 0x00005747f51f11ae mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3453:26
#33 0x00005747f5201d2c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4176:30
#34 0x00005747f5201c95 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#35 0x00005747e6ef4a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#36 0x00005747f521df83 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#37 0x00005747f51f4da8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4180:1
#38 0x00005747f51f4c5c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:10
#39 0x00005747f51f4e45 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4196:10
#40 0x00005747efc8ef9c (anonymous namespace)::ConvertFuncToLLVMPass::runOnOperation() /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:843:16
#41 0x00005747f5347e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#42 0x00005747f5347e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#43 0x00005747e6ef4a19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#44 0x00005747f534b31b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#45 0x00005747f533f2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#46 0x00005747f533f9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#47 0x00005747f53452bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#48 0x00005747f5344846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#49 0x00005747e6fd8dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#50 0x00005747e6fd84d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#51 0x00005747e6fd818c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#52 0x00005747e6fd8096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#53 0x00005747f58a4c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#54 0x00005747f58a401a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#55 0x00005747e6fd3e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#56 0x00005747e6fd4215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#57 0x00005747e6fd43e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#58 0x00005747e6ea3485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#59 0x000078a20e42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#60 0x000078a20e42a28b call_init ./csu/../csu/libc-start.c:128:20
#61 0x000078a20e42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#62 0x00005747e6ea32f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash in the pass `-gpu-lower-to-nvvm-pipeline`. See the following code example:

#### Code Example
```llvm
module {
  func.func @<!-- -->host() attributes {linkage = #llvm.linkage&lt;internal&gt;} {
    %0 = arith.constant 1.0 : f64
    %1 = arith.constant 2.0 : f64
    %2 = arith.cmpf "ogt", %0, %1 : f64
    %3 = arith.select %2, %0, %1 : f64
    return
  }
  gpu.module @<!-- -->gpu_mod {
    gpu.func @<!-- -->gpu_kernel() kernel {
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt ext_integer.mlir -gpu-lower-to-nvvm-pipeline
 #<!-- -->0 0x00005747e6ece87d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005747e6ecedab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005747e6eccc64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005747e6ecf449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x000078a20e445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000078a20e49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000078a20e49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000078a20e49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000078a20e44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000078a20e4288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000078a20e42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000078a20e43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005747f56cb793 mlir::DictionaryAttr::sortInPlace(llvm::SmallVectorImpl&lt;mlir::NamedAttribute&gt;&amp;) /mlir/lib/IR/BuiltinAttributes.cpp:128:10
#<!-- -->13 0x00005747f583884a mlir::NamedAttrList::getDictionary(mlir::MLIRContext*) const /mlir/lib/IR/OperationSupport.cpp:56:5
#<!-- -->14 0x00005747f58299f0 mlir::Operation::create(mlir::OperationState const&amp;) /mlir/lib/IR/Operation.cpp:37:31
#<!-- -->15 0x00005747f56be391 mlir::OpBuilder::create(mlir::OperationState const&amp;) /mlir/lib/IR/Builders.cpp:458:17
#<!-- -->16 0x00005747f404a460 mlir::LLVM::LLVMFuncOp::create(mlir::OpBuilder&amp;, mlir::Location, llvm::StringRef, mlir::Type, mlir::LLVM::linkage::Linkage, bool, mlir::LLVM::cconv::CConv, mlir::SymbolRefAttr, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::ArrayRef&lt;mlir::DictionaryAttr&gt;, std::optional&lt;unsigned long&gt;) /build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:33007:55
#<!-- -->17 0x00005747efc8b3c9 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&amp;, mlir::LLVMTypeConverter const&amp;, mlir::SymbolTableCollection*) /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:360:20
#<!-- -->18 0x00005747efc90c52 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:493:45
#<!-- -->19 0x00005747e7073f59 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp, false&gt;, mlir::func::FuncOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp, false&gt; const&amp;, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->20 0x00005747e7072b73 mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp, false&gt;::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:265:12
#<!-- -->21 0x00005747e7072acc mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:251:12
#<!-- -->22 0x00005747f51eff69 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2409:10
#<!-- -->23 0x00005747f5287228 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->24 0x00005747f5286ed5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x00005747e6ef4a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005747f5288a2b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x00005747f52852ba mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->28 0x00005747f51fca58 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2829:21
#<!-- -->29 0x00005747f51f0c62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2608:17
#<!-- -->30 0x00005747f51f0532 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3306:26
#<!-- -->31 0x00005747f51f15fd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3407:16
#<!-- -->32 0x00005747f51f11ae mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3453:26
#<!-- -->33 0x00005747f5201d2c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4176:30
#<!-- -->34 0x00005747f5201c95 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->35 0x00005747e6ef4a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->36 0x00005747f521df83 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->37 0x00005747f51f4da8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4180:1
#<!-- -->38 0x00005747f51f4c5c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:10
#<!-- -->39 0x00005747f51f4e45 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4196:10
#<!-- -->40 0x00005747efc8ef9c (anonymous namespace)::ConvertFuncToLLVMPass::runOnOperation() /mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:843:16
#<!-- -->41 0x00005747f5347e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->42 0x00005747f5347e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->43 0x00005747e6ef4a19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->44 0x00005747f534b31b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->45 0x00005747f533f2be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->46 0x00005747f533f9da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->47 0x00005747f53452bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->48 0x00005747f5344846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->49 0x00005747e6fd8dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->50 0x00005747e6fd84d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->51 0x00005747e6fd818c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->52 0x00005747e6fd8096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->53 0x00005747f58a4c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->54 0x00005747f58a401a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->55 0x00005747e6fd3e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->56 0x00005747e6fd4215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->57 0x00005747e6fd43e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->58 0x00005747e6ea3485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->59 0x000078a20e42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->60 0x000078a20e42a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->61 0x000078a20e42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->62 0x00005747e6ea32f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```
</details>


---

