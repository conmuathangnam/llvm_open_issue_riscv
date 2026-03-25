# mlir-opt crashes in TypeConverter::convertType with async.execute returning tensor

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/173859
**Status:** Closed
**Labels:** obsolete, mlir, crash
**Closed Date:** 2025-12-31T08:31:46Z

## Body

## Description
mlir-opt --gpu-to-llvm crashes on valid MLIR when converting an async.execute that returns !async.value<tensor<…>>.
## Reproduce
mlir-opt version: 21.1.8
location: If the `%sema2Token, %bodyResults = async.execute -> !async.value<tensor<i32>> {`  operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @main() {
    %0 = arith.constant dense<0> : tensor<i32>
    scf.execute_region -> tensor<i32> {
      scf.yield %0 : tensor<i32>
    }
    %1 = arith.constant dense<0> : tensor<i32>
    scf.execute_region -> tensor<i32> {
      scf.yield %1 : tensor<i32>
    }
    %2 = arith.constant dense<5481> : tensor<i32>
    %sema2Token, %bodyResults = async.execute -> !async.value<tensor<i32>> {
      scf.execute_region -> tensor<i32> {
        scf.yield %2 : tensor<i32>
      }
      async.yield %2 : tensor<i32>
    }
    return
  }
}
```
```
mlir-opt --gpu-to-llvm test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/Transforms/Utils/DialectConversion.cpp:2904: llvm::LogicalResult mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&) const: Assertion `t && "expected non-null type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --gpu-to-llvm /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250456_5e0e_5.mlir
 #0 0x00005a37f3317612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x00005a37f3313d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x00005a37f3313ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073c176c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073c176c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073c176c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073c176c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073c176c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073c176c288ff abort ./stdlib/abort.c:81:7
 #9 0x000073c176c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073c176c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005a37f6b68490 mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl<mlir::Type>&) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2490)
#12 0x00005a37f6b68e27 mlir::TypeConverter::convertType(mlir::Type) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2e27)
#13 0x00005a37f6b68e7d mlir::TypeConverter::isLegal(mlir::Type) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2e7d)
#14 0x00005a37f67f8fd3 _ZN9__gnu_cxx5__ops12_Iter_negateIZNK4mlir13TypeConverter7isLegalINS2_14ValueTypeRangeINS2_11ResultRangeEEEEENSt9enable_ifIXaantsrSt14is_convertibleIT_NS2_4TypeEE5valuentsrS9_ISA_PNS2_9OperationEE5valueEbE4typeEOSA_EUlSB_E_EclINS2_17ValueTypeIteratorIN4llvm6detail27indexed_accessor_range_baseIS6_PNS2_6detail12OpResultImplENS2_8OpResultEST_ST_E8iteratorEEEEEbSA_ (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5582fd3)
#15 0x00005a37f6b72191 mlir::TypeConverter::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58fc191)
#16 0x00005a37f64f7253 std::_Function_handler<std::optional<bool> (mlir::Operation*), mlir::populateAsyncStructuralTypeConversionsAndLegality(mlir::TypeConverter&, mlir::RewritePatternSet&, mlir::ConversionTarget&)::'lambda'(mlir::Operation*)>::_M_invoke(std::_Any_data const&, mlir::Operation*&&) AsyncToLLVM.cpp:0:0
#17 0x00005a37f6b6263f mlir::ConversionTarget::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58ec63f)
#18 0x00005a37f6b772e4 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&) DialectConversion.cpp:0:0
#19 0x00005a37f6b7a4dc (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*, mlir::ConversionPatternRewriter&)::'lambda1'(mlir::Pattern const&)::operator()(mlir::Pattern const&) const DialectConversion.cpp:0:0
#20 0x00005a37f9c38d85 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c2d85)
#21 0x00005a37f6b776fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&) DialectConversion.cpp:0:0
#22 0x00005a37f6b77cae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#23 0x00005a37f6b7c57c mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#24 0x00005a37f6b7dbba applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#25 0x00005a37f6b7dcf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#26 0x00005a37f65bb2d1 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() GPUToLLVMConversion.cpp:0:0
#27 0x00005a37f6acc07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#28 0x00005a37f6acc4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#29 0x00005a37f6acd523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#30 0x00005a37f6abd0a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#31 0x00005a37f6abd9c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#32 0x00005a37f6abdb8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#33 0x00005a37f6bd324e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#34 0x00005a37f6ab6080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#35 0x00005a37f6abdcf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#36 0x00005a37f6abe1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#37 0x00005a37f322f577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#38 0x000073c176c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x000073c176c2a28b call_init ./csu/../csu/libc-start.c:128:20
#40 0x000073c176c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x00005a37f32e8665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
## Description
mlir-opt --gpu-to-llvm crashes on valid MLIR when converting an async.execute that returns !async.value&lt;tensor&lt;…&gt;&gt;.
## Reproduce
mlir-opt version: 21.1.8
location: If the `%sema2Token, %bodyResults = async.execute -&gt; !async.value&lt;tensor&lt;i32&gt;&gt; {`  operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = arith.constant dense&lt;0&gt; : tensor&lt;i32&gt;
    scf.execute_region -&gt; tensor&lt;i32&gt; {
      scf.yield %0 : tensor&lt;i32&gt;
    }
    %1 = arith.constant dense&lt;0&gt; : tensor&lt;i32&gt;
    scf.execute_region -&gt; tensor&lt;i32&gt; {
      scf.yield %1 : tensor&lt;i32&gt;
    }
    %2 = arith.constant dense&lt;5481&gt; : tensor&lt;i32&gt;
    %sema2Token, %bodyResults = async.execute -&gt; !async.value&lt;tensor&lt;i32&gt;&gt; {
      scf.execute_region -&gt; tensor&lt;i32&gt; {
        scf.yield %2 : tensor&lt;i32&gt;
      }
      async.yield %2 : tensor&lt;i32&gt;
    }
    return
  }
}
```
```
mlir-opt --gpu-to-llvm test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/Transforms/Utils/DialectConversion.cpp:2904: llvm::LogicalResult mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const: Assertion `t &amp;&amp; "expected non-null type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --gpu-to-llvm /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250456_5e0e_5.mlir
 #<!-- -->0 0x00005a37f3317612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x00005a37f3313d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x00005a37f3313ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000073c176c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000073c176c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000073c176c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000073c176c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000073c176c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000073c176c288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000073c176c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000073c176c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005a37f6b68490 mlir::TypeConverter::convertType(mlir::Type, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2490)
#<!-- -->12 0x00005a37f6b68e27 mlir::TypeConverter::convertType(mlir::Type) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2e27)
#<!-- -->13 0x00005a37f6b68e7d mlir::TypeConverter::isLegal(mlir::Type) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58f2e7d)
#<!-- -->14 0x00005a37f67f8fd3 _ZN9__gnu_cxx5__ops12_Iter_negateIZNK4mlir13TypeConverter7isLegalINS2_14ValueTypeRangeINS2_11ResultRangeEEEEENSt9enable_ifIXaantsrSt14is_convertibleIT_NS2_4TypeEE5valuentsrS9_ISA_PNS2_9OperationEE5valueEbE4typeEOSA_EUlSB_E_EclINS2_17ValueTypeIteratorIN4llvm6detail27indexed_accessor_range_baseIS6_PNS2_6detail12OpResultImplENS2_8OpResultEST_ST_E8iteratorEEEEEbSA_ (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5582fd3)
#<!-- -->15 0x00005a37f6b72191 mlir::TypeConverter::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58fc191)
#<!-- -->16 0x00005a37f64f7253 std::_Function_handler&lt;std::optional&lt;bool&gt; (mlir::Operation*), mlir::populateAsyncStructuralTypeConversionsAndLegality(mlir::TypeConverter&amp;, mlir::RewritePatternSet&amp;, mlir::ConversionTarget&amp;)::'lambda'(mlir::Operation*)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;) AsyncToLLVM.cpp:0:0
#<!-- -->17 0x00005a37f6b6263f mlir::ConversionTarget::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58ec63f)
#<!-- -->18 0x00005a37f6b772e4 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) DialectConversion.cpp:0:0
#<!-- -->19 0x00005a37f6b7a4dc (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*, mlir::ConversionPatternRewriter&amp;)::'lambda1'(mlir::Pattern const&amp;)::operator()(mlir::Pattern const&amp;) const DialectConversion.cpp:0:0
#<!-- -->20 0x00005a37f9c38d85 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c2d85)
#<!-- -->21 0x00005a37f6b776fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) DialectConversion.cpp:0:0
#<!-- -->22 0x00005a37f6b77cae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&amp;, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#<!-- -->23 0x00005a37f6b7c57c mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#<!-- -->24 0x00005a37f6b7dbba applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->25 0x00005a37f6b7dcf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#<!-- -->26 0x00005a37f65bb2d1 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() GPUToLLVMConversion.cpp:0:0
#<!-- -->27 0x00005a37f6acc07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->28 0x00005a37f6acc4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->29 0x00005a37f6acd523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->30 0x00005a37f6abd0a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->31 0x00005a37f6abd9c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->32 0x00005a37f6abdb8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->33 0x00005a37f6bd324e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->34 0x00005a37f6ab6080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->35 0x00005a37f6abdcf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->36 0x00005a37f6abe1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->37 0x00005a37f322f577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->38 0x000073c176c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x000073c176c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x000073c176c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x00005a37f32e8665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

### Comment 2 - sweiglbosker

Can't reproduce on 2c72af88213c0f9c507d9c8b34a39de8173a6fcc

---

