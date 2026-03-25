# [MLIR] SCF to SPIRV conversion crashes on zero-sized memref

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159738
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-21T13:59:47Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The SPIR-V type converter crashes with an assertion failure when processing zero-sized memrefs during `--convert-scf-to-spirv`, attempting to create a SPIR-V ArrayType with zero elements. Possibly related to [PR #73351](https://github.com/llvm/llvm-project/pull/73351) which fixed a similar issue.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "func.func"() <{function_type = (tensor<123456x8192x8192xf32>) -> tensor<123456x8192x8192xf32>, sym_name = "test"}> ({
  ^bb0(%arg0: tensor<123456x8192x8192xf32>):
    "func.return"(%arg0) : (tensor<123456x8192x8192xf32>) -> ()
  }) : () -> ()
}) : () -> ()
```

**Command**
```
mlir-opt -convert-scf-to-spirv test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp:53: static mlir::spirv::ArrayType mlir::spirv::ArrayType::get(mlir::Type, unsigned int): Assertion `elementCount && "ArrayType needs at least one element"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt -convert-scf-to-spirv test.mlir 
 #0 0x0000557a6cd02a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x0000557a6cf87cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x0000557a6cf8217f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x0000557a6cf8a316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007ff847d10520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007ff847d649fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007ff847d10476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007ff847cf67f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007ff847cf671b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007ff847d07e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x0000557a74e83493 mlir::spirv::ArrayType::get(mlir::Type, unsigned int) /workdir/llvm-project/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp:53:3
#11 0x0000557a74f0dd63 (anonymous namespace)::convertTensorType(mlir::spirv::TargetEnv const&, mlir::SPIRVConversionOptions const&, mlir::TensorType) /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:469:10
#12 0x0000557a74f0dd63 mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11::operator()(mlir::TensorType) const /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1454:12
#13 0x0000557a74f0dd63 std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&)::operator()(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:358:43
#14 0x0000557a74f0dd63 std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&)>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)::operator()(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:379:14
#15 0x0000557a74f0dd63 mlir::TensorType std::__invoke_impl<std::optional<llvm::LogicalResult>, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&)>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)&, mlir::Type, llvm::SmallVectorImpl<mlir::Type>&>(std::__invoke_other, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&, mlir::Type&&, llvm::SmallVectorImpl<mlir::Type>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:14
#16 0x0000557a74f0dd63 std::enable_if<is_invocable_r_v<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::Type, llvm::SmallVectorImpl<mlir::Type>&>, mlir::TensorType>::type std::__invoke_r<std::optional<llvm::LogicalResult>, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&)>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)&, mlir::Type, llvm::SmallVectorImpl<mlir::Type>&>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&, mlir::Type&&, llvm::SmallVectorImpl<mlir::Type>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:9
#17 0x0000557a74f0dd63 std::_Function_handler<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&), std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, std::enable_if<std::is_invocable_v<mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11, mlir::TensorType>, std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)> >::type mlir::TypeConverter::wrapCallback<mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl<mlir::Type>&)>(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&)::$_11&&) const::'lambda'(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)>::_M_invoke(std::_Any_data const&, mlir::Type&&, llvm::SmallVectorImpl<mlir::Type>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:9
#18 0x0000557a76faf000 std::function<std::optional<llvm::LogicalResult> (mlir::Type, llvm::SmallVectorImpl<mlir::Type>&)>::operator()(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&) const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#19 0x0000557a76f98584 mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2931:47
#20 0x0000557a76fafbf9 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#21 0x0000557a76fafbf9 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#22 0x0000557a76fafbf9 mlir::TypeConverter::convertType(mlir::Type) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2957:7
#23 0x0000557a74f1507a (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&) const /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:0:48
#24 0x0000557a74f15d8e mlir::OpConversionPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange> >, mlir::ConversionPatternRewriter&) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:625:12
#25 0x0000557a74f14c50 mlir::OpConversionPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:609:12
#26 0x0000557a76fa7968 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:1931:10
#27 0x0000557a7ebe3a61 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_2::operator()() const /workdir/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:212:13
#28 0x0000557a7ebe3a61 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_2>(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#29 0x0000557a7ebd91ce mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#30 0x0000557a76fa9d62 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*, mlir::ConversionPatternRewriter&) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2278:21
#31 0x0000557a76fa9d62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2143:17
#32 0x0000557a76fa7e07 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#33 0x0000557a76fa7e07 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#34 0x0000557a76fa7e07 mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&, mlir::Operation*) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2651:7
#35 0x0000557a76faaeed llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#36 0x0000557a76faaeed llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#37 0x0000557a76faaeed mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2750:9
#38 0x0000557a76fcdf0e applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_30::operator()() const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3457:9
#39 0x0000557a76fcdf0e void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_30>(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#40 0x0000557a76fb8a9b applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3454:3
#41 0x0000557a76fb8db8 mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3470:10
#42 0x0000557a76fb8db8 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3477:10
#43 0x0000557a76a49e75 (anonymous namespace)::SCFToSPIRVPass::runOnOperation() /workdir/llvm-project/mlir/lib/Conversion/SCFToSPIRV/SCFToSPIRVPass.cpp:57:14
#44 0x0000557a76e845e4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_7::operator()() const /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:558:22
#45 0x0000557a76e845e4 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_7>(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#46 0x0000557a76e72e1f mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#47 0x0000557a76e747f3 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#48 0x0000557a76e747f3 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#49 0x0000557a76e747f3 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:623:9
#50 0x0000557a76e7b08a mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:937:3
#51 0x0000557a76e7acf9 mlir::PassManager::run(mlir::Operation*) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:0:60
#52 0x0000557a76e6492f llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#53 0x0000557a76e6492f llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#54 0x0000557a76e6492f performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:484:7
#55 0x0000557a76e63fe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#56 0x0000557a76e63fe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#57 0x0000557a76e63fe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#58 0x0000557a7709ceac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#59 0x0000557a7709ceac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#60 0x0000557a7709c4c3 std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#61 0x0000557a7709c4c3 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#62 0x0000557a76e52b1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#63 0x0000557a76e53405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#64 0x0000557a76e53405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#65 0x0000557a76e53405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#66 0x0000557a76e53b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#67 0x0000557a6cd87987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#68 0x0000557a6cd87987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#69 0x0000557a6cd87987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#70 0x00007ff847cf7d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#71 0x00007ff847cf7e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#72 0x0000557a6ccc6ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The SPIR-V type converter crashes with an assertion failure when processing zero-sized memrefs during `--convert-scf-to-spirv`, attempting to create a SPIR-V ArrayType with zero elements. Possibly related to [PR #<!-- -->73351](https://github.com/llvm/llvm-project/pull/73351) which fixed a similar issue.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "func.func"() &lt;{function_type = (tensor&lt;123456x8192x8192xf32&gt;) -&gt; tensor&lt;123456x8192x8192xf32&gt;, sym_name = "test"}&gt; ({
  ^bb0(%arg0: tensor&lt;123456x8192x8192xf32&gt;):
    "func.return"(%arg0) : (tensor&lt;123456x8192x8192xf32&gt;) -&gt; ()
  }) : () -&gt; ()
}) : () -&gt; ()
```

**Command**
```
mlir-opt -convert-scf-to-spirv test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp:53: static mlir::spirv::ArrayType mlir::spirv::ArrayType::get(mlir::Type, unsigned int): Assertion `elementCount &amp;&amp; "ArrayType needs at least one element"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt -convert-scf-to-spirv test.mlir 
 #<!-- -->0 0x0000557a6cd02a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x0000557a6cf87cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x0000557a6cf8217f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x0000557a6cf8a316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007ff847d10520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007ff847d649fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007ff847d10476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007ff847cf67f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007ff847cf671b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007ff847d07e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x0000557a74e83493 mlir::spirv::ArrayType::get(mlir::Type, unsigned int) /workdir/llvm-project/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp:53:3
#<!-- -->11 0x0000557a74f0dd63 (anonymous namespace)::convertTensorType(mlir::spirv::TargetEnv const&amp;, mlir::SPIRVConversionOptions const&amp;, mlir::TensorType) /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:469:10
#<!-- -->12 0x0000557a74f0dd63 mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11::operator()(mlir::TensorType) const /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:1454:12
#<!-- -->13 0x0000557a74f0dd63 std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)::operator()(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:358:43
#<!-- -->14 0x0000557a74f0dd63 std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)::operator()(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:379:14
#<!-- -->15 0x0000557a74f0dd63 mlir::TensorType std::__invoke_impl&lt;std::optional&lt;llvm::LogicalResult&gt;, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&amp;, mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;(std::__invoke_other, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;, mlir::Type&amp;&amp;, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:61:14
#<!-- -->16 0x0000557a74f0dd63 std::enable_if&lt;is_invocable_r_v&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;, mlir::TensorType&gt;::type std::__invoke_r&lt;std::optional&lt;llvm::LogicalResult&gt;, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&amp;, mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;, mlir::Type&amp;&amp;, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/invoke.h:114:9
#<!-- -->17 0x0000557a74f0dd63 std::_Function_handler&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;), std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, std::enable_if&lt;std::is_invocable_v&lt;mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11, mlir::TensorType&gt;, std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt; &gt;::type mlir::TypeConverter::wrapCallback&lt;mlir::TensorType, mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::TensorType, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;(mlir::SPIRVTypeConverter::SPIRVTypeConverter(mlir::spirv::TargetEnvAttr, mlir::SPIRVConversionOptions const&amp;)::$_11&amp;&amp;) const::'lambda'(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Type&amp;&amp;, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:290:9
#<!-- -->18 0x0000557a76faf000 std::function&lt;std::optional&lt;llvm::LogicalResult&gt; (mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;)&gt;::operator()(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/std_function.h:590:2
#<!-- -->19 0x0000557a76f98584 mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2931:47
#<!-- -->20 0x0000557a76fafbf9 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->21 0x0000557a76fafbf9 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->22 0x0000557a76fafbf9 mlir::TypeConverter::convertType(mlir::Type) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2957:7
#<!-- -->23 0x0000557a74f1507a (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /workdir/llvm-project/mlir/lib/Dialect/SPIRV/Transforms/SPIRVConversion.cpp:0:48
#<!-- -->24 0x0000557a74f15d8e mlir::OpConversionPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt; &gt;, mlir::ConversionPatternRewriter&amp;) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:625:12
#<!-- -->25 0x0000557a74f14c50 mlir::OpConversionPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /workdir/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:609:12
#<!-- -->26 0x0000557a76fa7968 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:1931:10
#<!-- -->27 0x0000557a7ebe3a61 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_2::operator()() const /workdir/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:212:13
#<!-- -->28 0x0000557a7ebe3a61 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_2&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->29 0x0000557a7ebd91ce mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->30 0x0000557a76fa9d62 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2278:21
#<!-- -->31 0x0000557a76fa9d62 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2143:17
#<!-- -->32 0x0000557a76fa7e07 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->33 0x0000557a76fa7e07 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->34 0x0000557a76fa7e07 mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&amp;, mlir::Operation*) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2651:7
#<!-- -->35 0x0000557a76faaeed llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->36 0x0000557a76faaeed llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->37 0x0000557a76faaeed mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2750:9
#<!-- -->38 0x0000557a76fcdf0e applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_30::operator()() const /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3457:9
#<!-- -->39 0x0000557a76fcdf0e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_30&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->40 0x0000557a76fb8a9b applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3454:3
#<!-- -->41 0x0000557a76fb8db8 mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3470:10
#<!-- -->42 0x0000557a76fb8db8 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /workdir/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3477:10
#<!-- -->43 0x0000557a76a49e75 (anonymous namespace)::SCFToSPIRVPass::runOnOperation() /workdir/llvm-project/mlir/lib/Conversion/SCFToSPIRV/SCFToSPIRVPass.cpp:57:14
#<!-- -->44 0x0000557a76e845e4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_7::operator()() const /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:558:22
#<!-- -->45 0x0000557a76e845e4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_7&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x0000557a76e72e1f mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->47 0x0000557a76e747f3 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->48 0x0000557a76e747f3 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->49 0x0000557a76e747f3 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:623:9
#<!-- -->50 0x0000557a76e7b08a mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:937:3
#<!-- -->51 0x0000557a76e7acf9 mlir::PassManager::run(mlir::Operation*) /workdir/llvm-project/mlir/lib/Pass/Pass.cpp:0:60
#<!-- -->52 0x0000557a76e6492f llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->53 0x0000557a76e6492f llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->54 0x0000557a76e6492f performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:484:7
#<!-- -->55 0x0000557a76e63fe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->56 0x0000557a76e63fe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->57 0x0000557a76e63fe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->58 0x0000557a7709ceac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->59 0x0000557a7709ceac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->60 0x0000557a7709c4c3 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->61 0x0000557a7709c4c3 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#<!-- -->62 0x0000557a76e52b1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->63 0x0000557a76e53405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->64 0x0000557a76e53405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->65 0x0000557a76e53405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->66 0x0000557a76e53b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->67 0x0000557a6cd87987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->68 0x0000557a6cd87987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->69 0x0000557a6cd87987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->70 0x00007ff847cf7d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->71 0x00007ff847cf7e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->72 0x0000557a6ccc6ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

### Comment 2 - veera-sivarajan

Looks like this is fixed in trunk: https://godbolt.org/z/1rTvhv6Pz

---

