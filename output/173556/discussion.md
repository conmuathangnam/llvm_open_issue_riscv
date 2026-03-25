# mlir-opt crashes in mlir::detail::OperandStorage::OperandStorage

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173556
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2026-02-27T18:48:24Z

## Body

When running opt on the following IR, mlir-opt crashes. 

version 2c02e4c7909a

test.mlir

```
func.func @trunci_tensor(%arg0 : tensor<8x4xi32>) -> tensor<8x4xi16> {
  %0 = arith.trunci %arg0 : tensor<8x4xi32> to tensor<8x4xi16>
  return %0 : tensor<8x4xi16>
}
```


commads:

mlir-opt -convert-tensor-to-spirv test.mlir

stacktrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt -convert-tensor-to-spirv test.mlir
 #0 0x0000560b0c12d358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/bin/mlir-opt+0x1e97358)
 #1 0x0000560b0c12aa15 llvm::sys::RunSignalHandlers() (./build/bin/mlir-opt+0x1e94a15)
 #2 0x0000560b0c12e396 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f88f2971420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x0000560b14ad0a35 mlir::detail::OperandStorage::OperandStorage(mlir::Operation*, mlir::OpOperand*, mlir::ValueRange) (./build/bin/mlir-opt+0xa83aa35)
 #5 0x0000560b14ac57ec mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) (./build/bin/mlir-opt+0xa82f7ec)
 #6 0x0000560b14ac50b7 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) (./build/bin/mlir-opt+0xa82f0b7)
 #7 0x0000560b14ac4f74 mlir::Operation::create(mlir::OperationState const&) (./build/bin/mlir-opt+0xa82ef74)
 #8 0x0000560b14a0d600 mlir::OpBuilder::create(mlir::OperationState const&) (./build/bin/mlir-opt+0xa777600)
 #9 0x0000560b110c0667 mlir::spirv::BitwiseAndOp::create(mlir::OpBuilder&, mlir::Location, mlir::Type, mlir::Value, mlir::Value) (./build/bin/mlir-opt+0x6e2a667)
#10 0x0000560b109d6f95 (anonymous namespace)::TruncIPattern::matchAndRewrite(mlir::arith::TruncIOp, mlir::arith::TruncIOpAdaptor, mlir::ConversionPatternRewriter&) const ArithToSPIRV.cpp:0:0
#11 0x0000560b0e44c4c2 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<mlir::arith::TruncIOp>, mlir::arith::TruncIOp>(mlir::OpConversionPattern<mlir::arith::TruncIOp> const&, mlir::arith::TruncIOp, mlir::arith::TruncIOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (./build/bin/mlir-opt+0x41b64c2)
#12 0x0000560b0e44be1a mlir::OpConversionPattern<mlir::arith::TruncIOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (./build/bin/mlir-opt+0x41b5e1a)
#13 0x0000560b14743996 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (./build/bin/mlir-opt+0xa4ad996)
#14 0x0000560b1478c7f4 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) PatternApplicator.cpp:0:0
#15 0x0000560b14789131 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (./build/bin/mlir-opt+0xa4f3131)
#16 0x0000560b14745ab3 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#17 0x0000560b14744f8f mlir::OperationConverter::convert(mlir::Operation*, bool) (./build/bin/mlir-opt+0xa4aef8f)
#18 0x0000560b147462be mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (./build/bin/mlir-opt+0xa4b02be)
#19 0x0000560b147542c4 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) DialectConversion.cpp:0:0
#20 0x0000560b1474b76f applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#21 0x0000560b1474b84b mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (./build/bin/mlir-opt+0xa4b584b)
#22 0x0000560b109c5c0d (anonymous namespace)::ConvertTensorToSPIRVPass::runOnOperation() TensorToSPIRVPass.cpp:0:0
#23 0x0000560b147e77be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (./build/bin/mlir-opt+0xa5517be)
#24 0x0000560b147e8617 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (./build/bin/mlir-opt+0xa552617)
#25 0x0000560b147ef9ca mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (./build/bin/mlir-opt+0xa5599ca)
#26 0x0000560b147ef01f mlir::PassManager::run(mlir::Operation*) (./build/bin/mlir-opt+0xa55901f)
#27 0x0000560b0c1d1377 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#28 0x0000560b0c1d05a6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x0000560b14b02855 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (./build/bin/mlir-opt+0xa86c855)
#30 0x0000560b0c1c6166 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (./build/bin/mlir-opt+0x1f30166)
#31 0x0000560b0c1c64a2 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f304a2)
#32 0x0000560b0c1c6805 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f30805)
#33 0x0000560b0c1139b3 main (./build/bin/mlir-opt+0x1e7d9b3)
#34 0x00007f88f1fab083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#35 0x0000560b0c1134ee _start (./build/bin/mlir-opt+0x1e7d4ee)
```



## Comments

### Comment 1 - ayokunle321

Hi @Emilyaxe could i get assigned this?

---

### Comment 2 - joker-eph

Seems like it is fixed (maybe through #183654)

---

