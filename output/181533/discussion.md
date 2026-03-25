# mlir-opt crash: std::optional assertion failure in ConvertGlobal (MemRefToEmitC)

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/181533
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-24T13:52:42Z

## Body

Bug Pass: --convert-memref-to-emitc

Bug Op: memref.global

Description:
The mlir-opt tool crashes during the conversion of memref to EmitC. Specifically, the ConvertGlobal pattern assumes that every memref.global operation has an initial value attribute (like a dense constant).

When a global variable is declared without an initializer (e.g., memref.global @global_var : memref<f32>), the pattern calls .value() on a std::optional<Attribute> that is not "engaged" (is null). This triggers a C++ standard library assertion: Assertion 'this->_M_is_engaged()' failed.

```mlir
module {
  memref.global @global_var : memref<f32>
  func.func @main() {
    %0 = emitc.literal "M_PI" : f32
    %1 = emitc.unary_minus %0 : (f32) -> f32
    %2 = amdgpu.swizzle_bitmode %1 1 2 4 : f32
    %3 = math.cosh %2 : f32
    %4 = math.tanh %3 : f32
    %5 = arith.fptosi %4 : f32 to i32
    %6 = math.asinh %4 : f32
    return
  }
}
```
```
mlir-opt --convert-memref-to-emitc test.mlir
```
```
/usr/include/c++/13/optional:479: constexpr _Tp& std::_Optional_base_impl<_Tp, _Dp>::_M_get() [with _Tp = mlir::Attribute; _Dp = std::_Optional_base<mlir::Attribute, true, true>]: Assertion 'this->_M_is_engaged()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-memref-to-emitc test.mlir
 #0 0x000056d40bfdce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000056d40bfd95af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000056d40bfd96fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007539c1445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007539c149eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007539c149eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007539c149eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007539c144527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007539c14288ff abort ./stdlib/abort.c:81:7
 #9 0x00007539c18df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#10 0x000056d40d835c72 (anonymous namespace)::ConvertGlobal::matchAndRewrite(mlir::memref::GlobalOp, mlir::memref::GlobalOpAdaptor, mlir::ConversionPatternRewriter&) const MemRefToEmitC.cpp:0:0
#11 0x000056d40d83707c llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<mlir::memref::GlobalOp>, mlir::memref::GlobalOp>(mlir::OpConversionPattern<mlir::memref::GlobalOp> const&, mlir::memref::GlobalOp, mlir::memref::GlobalOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (llvmorg-22.1.0-install/bin/mlir-opt+0x3d8a07c)
#12 0x000056d40d8371ce mlir::OpConversionPattern<mlir::memref::GlobalOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x3d8a1ce)
#13 0x000056d4137ba32e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9d0d32e)
#14 0x000056d413807e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#15 0x000056d4138093af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#16 0x000056d4137b547c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#17 0x000056d4137b5944 mlir::OperationConverter::convert(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d08944)
#18 0x000056d4137beb16 mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d11b16)
#19 0x000056d4137c12ab applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#20 0x000056d4137c144f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d1444f)
#21 0x000056d40d83b1a9 (anonymous namespace)::ConvertMemRefToEmitCPass::runOnOperation() MemRefToEmitCPass.cpp:0:0
#22 0x000056d41387c756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#23 0x000056d41387ca8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#24 0x000056d41387d1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#25 0x000056d41387e400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#26 0x000056d40c0a3767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#27 0x000056d40c0a44ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#28 0x000056d413c4c731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#29 0x000056d40c09b8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#30 0x000056d40c0a4b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#31 0x000056d40c0a4da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#32 0x000056d40bed6023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#33 0x00007539c142a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x00007539c142a28b call_init ./csu/../csu/libc-start.c:128:20
#35 0x00007539c142a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x000056d40bfbb405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
Bug Pass: --convert-memref-to-emitc

Bug Op: memref.global

Description:
The mlir-opt tool crashes during the conversion of memref to EmitC. Specifically, the ConvertGlobal pattern assumes that every memref.global operation has an initial value attribute (like a dense constant).

When a global variable is declared without an initializer (e.g., memref.global @<!-- -->global_var : memref&lt;f32&gt;), the pattern calls .value() on a std::optional&lt;Attribute&gt; that is not "engaged" (is null). This triggers a C++ standard library assertion: Assertion 'this-&gt;_M_is_engaged()' failed.

```mlir
module {
  memref.global @<!-- -->global_var : memref&lt;f32&gt;
  func.func @<!-- -->main() {
    %0 = emitc.literal "M_PI" : f32
    %1 = emitc.unary_minus %0 : (f32) -&gt; f32
    %2 = amdgpu.swizzle_bitmode %1 1 2 4 : f32
    %3 = math.cosh %2 : f32
    %4 = math.tanh %3 : f32
    %5 = arith.fptosi %4 : f32 to i32
    %6 = math.asinh %4 : f32
    return
  }
}
```
```
mlir-opt --convert-memref-to-emitc test.mlir
```
```
/usr/include/c++/13/optional:479: constexpr _Tp&amp; std::_Optional_base_impl&lt;_Tp, _Dp&gt;::_M_get() [with _Tp = mlir::Attribute; _Dp = std::_Optional_base&lt;mlir::Attribute, true, true&gt;]: Assertion 'this-&gt;_M_is_engaged()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-memref-to-emitc test.mlir
 #<!-- -->0 0x000056d40bfdce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000056d40bfd95af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000056d40bfd96fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007539c1445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007539c149eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007539c149eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007539c149eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007539c144527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007539c14288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007539c18df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#<!-- -->10 0x000056d40d835c72 (anonymous namespace)::ConvertGlobal::matchAndRewrite(mlir::memref::GlobalOp, mlir::memref::GlobalOpAdaptor, mlir::ConversionPatternRewriter&amp;) const MemRefToEmitC.cpp:0:0
#<!-- -->11 0x000056d40d83707c llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;mlir::memref::GlobalOp&gt;, mlir::memref::GlobalOp&gt;(mlir::OpConversionPattern&lt;mlir::memref::GlobalOp&gt; const&amp;, mlir::memref::GlobalOp, mlir::memref::GlobalOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x3d8a07c)
#<!-- -->12 0x000056d40d8371ce mlir::OpConversionPattern&lt;mlir::memref::GlobalOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x3d8a1ce)
#<!-- -->13 0x000056d4137ba32e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9d0d32e)
#<!-- -->14 0x000056d413807e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->15 0x000056d4138093af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#<!-- -->16 0x000056d4137b547c (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
#<!-- -->17 0x000056d4137b5944 mlir::OperationConverter::convert(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d08944)
#<!-- -->18 0x000056d4137beb16 mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d11b16)
#<!-- -->19 0x000056d4137c12ab applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->20 0x000056d4137c144f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d1444f)
#<!-- -->21 0x000056d40d83b1a9 (anonymous namespace)::ConvertMemRefToEmitCPass::runOnOperation() MemRefToEmitCPass.cpp:0:0
#<!-- -->22 0x000056d41387c756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->23 0x000056d41387ca8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->24 0x000056d41387d1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#<!-- -->25 0x000056d41387e400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#<!-- -->26 0x000056d40c0a3767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x000056d40c0a44ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->28 0x000056d413c4c731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->29 0x000056d40c09b8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->30 0x000056d40c0a4b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->31 0x000056d40c0a4da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->32 0x000056d40bed6023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->33 0x00007539c142a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->34 0x00007539c142a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->35 0x00007539c142a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->36 0x000056d40bfbb405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

