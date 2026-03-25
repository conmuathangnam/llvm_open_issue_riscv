# mlir-opt crash: decomposeValue assertion failure in amdgpu.swizzle_bitmode lowering with non-multiple bit-widths

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/180461
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T15:57:31Z

## Body

Mlir-opt Version: 22.1.0-rc1

Bug Op: amdgpu.swizzle_bitmode

Bug Pass: --convert-amdgpu-to-rocdl

Detailed Description: The mlir-opt tool crashes during the AMDGPU to ROCDL conversion pass. When lowering amdgpu.swizzle_bitmode, the transformation calls LLVM::decomposeValue to handle the input vector. This utility requires the source bit-width to be a multiple of the destination bit-width.

```mlir
module {
  func.func @main() {
    %5 = vector.constant_mask [42] : vector<42xi1>
    %6 = amdgpu.swizzle_bitmode %5 1 2 4 : vector<42xi1>
    return
  }
}
```
```
mlir-opt --convert-amdgpu-to-rocdl test.mlir
```
```
mlir-opt: mlir/lib/Conversion/LLVMCommon/Pattern.cpp:415: llvm::SmallVector<mlir::Value> mlir::LLVM::decomposeValue(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::Type): Assertion `srcBitWidth % dstBitWidth == 0 && "src bit width must be a multiple of dst bit width"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --convert-amdgpu-to-rocdl test.mlir
 #0 0x0000558531644e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x00005585316415af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x00005585316416fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007a9a04a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007a9a04a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007a9a04a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007a9a04a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007a9a04a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007a9a04a288ff abort ./stdlib/abort.c:81:7
 #9 0x00007a9a04a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007a9a04a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000558536ceb8d4 (mlir-opt+0x7bd68d4)
#12 0x0000558532df0deb (anonymous namespace)::AMDGPUSwizzleBitModeLowering::matchAndRewrite(mlir::amdgpu::SwizzleBitModeOp, mlir::amdgpu::SwizzleBitModeOpAdaptor, mlir::ConversionPatternRewriter&) const AMDGPUToROCDL.cpp:0:0
#13 0x0000558532ddeff0 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::amdgpu::SwizzleBitModeOp, false>, mlir::amdgpu::SwizzleBitModeOp>(mlir::ConvertOpToLLVMPattern<mlir::amdgpu::SwizzleBitModeOp, false> const&, mlir::amdgpu::SwizzleBitModeOp, mlir::amdgpu::SwizzleBitModeOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (mlir-opt+0x3cc9ff0)
#14 0x0000558532ddf125 mlir::ConvertOpToLLVMPattern<mlir::amdgpu::SwizzleBitModeOp, false>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (mlir-opt+0x3cca125)
#15 0x0000558538e2232e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (mlir-opt+0x9d0d32e)
#16 0x0000558538e6fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#17 0x0000558538e713af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (mlir-opt+0x9d5c3af)
#18 0x0000558538e1d47c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#19 0x0000558538e1d944 mlir::OperationConverter::convert(mlir::Operation*, bool) (mlir-opt+0x9d08944)
#20 0x0000558538e26b16 mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (mlir-opt+0x9d11b16)
#21 0x0000558538e292ab applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#22 0x0000558538e2944f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (mlir-opt+0x9d1444f)
#23 0x0000558532dda57f (anonymous namespace)::ConvertAMDGPUToROCDLPass::runOnOperation() AMDGPUToROCDL.cpp:0:0
#24 0x0000558538ee4756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#25 0x0000558538ee4a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#26 0x0000558538ee51e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#27 0x0000558538ee6400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#28 0x000055853170b767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#29 0x000055853170c4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#30 0x00005585392b4731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#31 0x00005585317038d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#32 0x000055853170cb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7b70)
#33 0x000055853170cda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7da9)
#34 0x000055853153e023 main (mlir-opt+0x2429023)
#35 0x00007a9a04a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#36 0x00007a9a04a2a28b call_init ./csu/../csu/libc-start.c:128:20
#37 0x00007a9a04a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#38 0x0000558531623405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
Mlir-opt Version: 22.1.0-rc1

Bug Op: amdgpu.swizzle_bitmode

Bug Pass: --convert-amdgpu-to-rocdl

Detailed Description: The mlir-opt tool crashes during the AMDGPU to ROCDL conversion pass. When lowering amdgpu.swizzle_bitmode, the transformation calls LLVM::decomposeValue to handle the input vector. This utility requires the source bit-width to be a multiple of the destination bit-width.

```mlir
module {
  func.func @<!-- -->main() {
    %5 = vector.constant_mask [42] : vector&lt;42xi1&gt;
    %6 = amdgpu.swizzle_bitmode %5 1 2 4 : vector&lt;42xi1&gt;
    return
  }
}
```
```
mlir-opt --convert-amdgpu-to-rocdl test.mlir
```
```
mlir-opt: mlir/lib/Conversion/LLVMCommon/Pattern.cpp:415: llvm::SmallVector&lt;mlir::Value&gt; mlir::LLVM::decomposeValue(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::Type): Assertion `srcBitWidth % dstBitWidth == 0 &amp;&amp; "src bit width must be a multiple of dst bit width"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --convert-amdgpu-to-rocdl test.mlir
 #<!-- -->0 0x0000558531644e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x00005585316415af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x00005585316416fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007a9a04a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007a9a04a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007a9a04a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007a9a04a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007a9a04a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007a9a04a288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007a9a04a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007a9a04a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000558536ceb8d4 (mlir-opt+0x7bd68d4)
#<!-- -->12 0x0000558532df0deb (anonymous namespace)::AMDGPUSwizzleBitModeLowering::matchAndRewrite(mlir::amdgpu::SwizzleBitModeOp, mlir::amdgpu::SwizzleBitModeOpAdaptor, mlir::ConversionPatternRewriter&amp;) const AMDGPUToROCDL.cpp:0:0
#<!-- -->13 0x0000558532ddeff0 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::amdgpu::SwizzleBitModeOp, false&gt;, mlir::amdgpu::SwizzleBitModeOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::amdgpu::SwizzleBitModeOp, false&gt; const&amp;, mlir::amdgpu::SwizzleBitModeOp, mlir::amdgpu::SwizzleBitModeOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (mlir-opt+0x3cc9ff0)
#<!-- -->14 0x0000558532ddf125 mlir::ConvertOpToLLVMPattern&lt;mlir::amdgpu::SwizzleBitModeOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (mlir-opt+0x3cca125)
#<!-- -->15 0x0000558538e2232e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (mlir-opt+0x9d0d32e)
#<!-- -->16 0x0000558538e6fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->17 0x0000558538e713af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (mlir-opt+0x9d5c3af)
#<!-- -->18 0x0000558538e1d47c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#<!-- -->19 0x0000558538e1d944 mlir::OperationConverter::convert(mlir::Operation*, bool) (mlir-opt+0x9d08944)
#<!-- -->20 0x0000558538e26b16 mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (mlir-opt+0x9d11b16)
#<!-- -->21 0x0000558538e292ab applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->22 0x0000558538e2944f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (mlir-opt+0x9d1444f)
#<!-- -->23 0x0000558532dda57f (anonymous namespace)::ConvertAMDGPUToROCDLPass::runOnOperation() AMDGPUToROCDL.cpp:0:0
#<!-- -->24 0x0000558538ee4756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#<!-- -->25 0x0000558538ee4a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#<!-- -->26 0x0000558538ee51e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#<!-- -->27 0x0000558538ee6400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#<!-- -->28 0x000055853170b767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x000055853170c4ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->30 0x00005585392b4731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#<!-- -->31 0x00005585317038d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->32 0x000055853170cb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7b70)
#<!-- -->33 0x000055853170cda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7da9)
#<!-- -->34 0x000055853153e023 main (mlir-opt+0x2429023)
#<!-- -->35 0x00007a9a04a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->36 0x00007a9a04a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->37 0x00007a9a04a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->38 0x0000558531623405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

### Comment 2 - abhamra

I'd like to take a crack at this issue, I am able to reproduce the error!

---

### Comment 3 - Men-cotton

No crash at HEAD. I believe #183825 resolved this.

---

