# mlir-opt crash: RankedTensorType cast failure in tosa.scatter to SCF conversion

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/181592
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-24T03:15:47Z

## Body

Bug Pass: --tosa-to-scf

Bug Op: tosa.scatter

Description:
The mlir-opt tool crashes during the conversion of tosa.scatter to scf loops. The transformation logic in ScatterOpConverter assumes all operands are ranked tensors. When it encounters unranked tensors (tensor<*xf32>), it performs an unsafe llvm::cast<RankedTensorType>, which fails and triggers an assertion because unranked tensors lack the required shape information for loop-nest generation.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = ml_program.token
    %2 = tensor.rank %0 : tensor<1xi32>
    %3 = tosa.cast %0 : (tensor<1xi32>) -> tensor<1xf32>
    %4 = tosa.sin %3 : (tensor<1xf32>) -> tensor<1xf32>
    %5 = tensor.cast %4 : tensor<1xf32> to tensor<*xf32>
    %6 = math.ceil %5 : tensor<*xf32>
    %7 = tensor.expand_shape %3 [[0, 1, 2]] output_shape [1, 1, 1] : tensor<1xf32> into tensor<1x1x1xf32>
    %8 = tosa.scatter %5, %6, %7 : (tensor<*xf32>, tensor<*xf32>, tensor<1x1x1xf32>) -> tensor<*xf32>
    return
  }
}
```
```
mlir-opt --tosa-to-scf test.mlir
```
```
mlir-opt: llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::detail::TypedValue<mlir::RankedTensorType>; From = mlir::OpResult]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --tosa-to-scf test.mlir
 #0 0x00005a95dca76e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x00005a95dca735af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x00005a95dca736fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000077bc36c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000077bc36c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000077bc36c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000077bc36c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000077bc36c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000077bc36c288ff abort ./stdlib/abort.c:81:7
 #9 0x000077bc36c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000077bc36c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005a95e03ae077 (anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&) const::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange)::operator()(mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange) const TosaToSCF.cpp:0:0
#12 0x00005a95e03ae0eb llvm::SmallVector<mlir::Value, 6u> llvm::function_ref<llvm::SmallVector<mlir::Value, 6u> (mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange)>::callback_fn<(anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&) const::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange)>(long, mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange) TosaToSCF.cpp:0:0
#13 0x00005a95e3e028ac mlir::scf::buildLoopNest(mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange, mlir::ValueRange, mlir::ValueRange, llvm::function_ref<llvm::SmallVector<mlir::Value, 6u> (mlir::OpBuilder&, mlir::Location, mlir::ValueRange, mlir::ValueRange)>) (mlir-opt+0x98bb8ac)
#14 0x00005a95e03acef5 (anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&) const TosaToSCF.cpp:0:0
#15 0x00005a95e42a1e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#16 0x00005a95e42a33af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (mlir-opt+0x9d5c3af)
#17 0x00005a95e424f47c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#18 0x00005a95e424f944 mlir::OperationConverter::convert(mlir::Operation*, bool) (mlir-opt+0x9d08944)
#19 0x00005a95e4258b16 mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (mlir-opt+0x9d11b16)
#20 0x00005a95e425b2ab applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#21 0x00005a95e425b44f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (mlir-opt+0x9d1444f)
#22 0x00005a95e03aad31 (anonymous namespace)::TosaToSCF::runOnOperation() TosaToSCFPass.cpp:0:0
#23 0x00005a95e4316756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#24 0x00005a95e4316a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#25 0x00005a95e43171e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#26 0x00005a95e4318400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#27 0x00005a95dcb3d767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#28 0x00005a95dcb3e4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x00005a95e46e6731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#30 0x00005a95dcb358d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#31 0x00005a95dcb3eb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7b70)
#32 0x00005a95dcb3eda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7da9)
#33 0x00005a95dc970023 main (mlir-opt+0x2429023)
#34 0x000077bc36c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x000077bc36c2a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x000077bc36c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x00005a95dca55405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - CoTinker

Fixed by #178188.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
Bug Pass: --tosa-to-scf

Bug Op: tosa.scatter

Description:
The mlir-opt tool crashes during the conversion of tosa.scatter to scf loops. The transformation logic in ScatterOpConverter assumes all operands are ranked tensors. When it encounters unranked tensors (tensor&lt;*xf32&gt;), it performs an unsafe llvm::cast&lt;RankedTensorType&gt;, which fails and triggers an assertion because unranked tensors lack the required shape information for loop-nest generation.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = ml_program.token
    %2 = tensor.rank %0 : tensor&lt;1xi32&gt;
    %3 = tosa.cast %0 : (tensor&lt;1xi32&gt;) -&gt; tensor&lt;1xf32&gt;
    %4 = tosa.sin %3 : (tensor&lt;1xf32&gt;) -&gt; tensor&lt;1xf32&gt;
    %5 = tensor.cast %4 : tensor&lt;1xf32&gt; to tensor&lt;*xf32&gt;
    %6 = math.ceil %5 : tensor&lt;*xf32&gt;
    %7 = tensor.expand_shape %3 [[0, 1, 2]] output_shape [1, 1, 1] : tensor&lt;1xf32&gt; into tensor&lt;1x1x1xf32&gt;
    %8 = tosa.scatter %5, %6, %7 : (tensor&lt;*xf32&gt;, tensor&lt;*xf32&gt;, tensor&lt;1x1x1xf32&gt;) -&gt; tensor&lt;*xf32&gt;
    return
  }
}
```
```
mlir-opt --tosa-to-scf test.mlir
```
```
mlir-opt: llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::detail::TypedValue&lt;mlir::RankedTensorType&gt;; From = mlir::OpResult]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --tosa-to-scf test.mlir
 #<!-- -->0 0x00005a95dca76e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x00005a95dca735af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x00005a95dca736fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000077bc36c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000077bc36c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000077bc36c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000077bc36c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000077bc36c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000077bc36c288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000077bc36c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000077bc36c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005a95e03ae077 (anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&amp;) const::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange)::operator()(mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange) const TosaToSCF.cpp:0:0
#<!-- -->12 0x00005a95e03ae0eb llvm::SmallVector&lt;mlir::Value, 6u&gt; llvm::function_ref&lt;llvm::SmallVector&lt;mlir::Value, 6u&gt; (mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange)&gt;::callback_fn&lt;(anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&amp;) const::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange)&gt;(long, mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange) TosaToSCF.cpp:0:0
#<!-- -->13 0x00005a95e3e028ac mlir::scf::buildLoopNest(mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange, mlir::ValueRange, mlir::ValueRange, llvm::function_ref&lt;llvm::SmallVector&lt;mlir::Value, 6u&gt; (mlir::OpBuilder&amp;, mlir::Location, mlir::ValueRange, mlir::ValueRange)&gt;) (mlir-opt+0x98bb8ac)
#<!-- -->14 0x00005a95e03acef5 (anonymous namespace)::ScatterOpConverter::matchAndRewrite(mlir::tosa::ScatterOp, mlir::PatternRewriter&amp;) const TosaToSCF.cpp:0:0
#<!-- -->15 0x00005a95e42a1e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->16 0x00005a95e42a33af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (mlir-opt+0x9d5c3af)
#<!-- -->17 0x00005a95e424f47c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#<!-- -->18 0x00005a95e424f944 mlir::OperationConverter::convert(mlir::Operation*, bool) (mlir-opt+0x9d08944)
#<!-- -->19 0x00005a95e4258b16 mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (mlir-opt+0x9d11b16)
#<!-- -->20 0x00005a95e425b2ab applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->21 0x00005a95e425b44f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (mlir-opt+0x9d1444f)
#<!-- -->22 0x00005a95e03aad31 (anonymous namespace)::TosaToSCF::runOnOperation() TosaToSCFPass.cpp:0:0
#<!-- -->23 0x00005a95e4316756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#<!-- -->24 0x00005a95e4316a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#<!-- -->25 0x00005a95e43171e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#<!-- -->26 0x00005a95e4318400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#<!-- -->27 0x00005a95dcb3d767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->28 0x00005a95dcb3e4ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x00005a95e46e6731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#<!-- -->30 0x00005a95dcb358d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->31 0x00005a95dcb3eb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7b70)
#<!-- -->32 0x00005a95dcb3eda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7da9)
#<!-- -->33 0x00005a95dc970023 main (mlir-opt+0x2429023)
#<!-- -->34 0x000077bc36c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x000077bc36c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x000077bc36c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x00005a95dca55405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

