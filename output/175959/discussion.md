# [MLIR] `convertFuncOpToLLVMFuncOp` crash with an invalid `NamedAttrList`

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/175959
**Status:** Open
**Labels:** mlir, crash

## Body

Hi, I find a crash bug during using MLIR to lower programs. See the minimized code exampel below:

### Input Program
File Name: **input.mlir**
```llvm
module {
  func.func @main(%arg0: i32) -> i32 attributes {linkage = #llvm.linkage<external>} {
    %c0_i32 = arith.constant 0 : i32
    %c10_i32 = arith.constant 10 : i32
    %c1_i32 = arith.constant 1 : i32
    %cond = arith.cmpi eq, %arg0, %c0_i32 : i32
    cf.cond_br %cond, ^bb1, ^bb2
    
  ^bb1:
    %result = arith.muli %c10_i32, %c1_i32 : i32
    cf.br ^bb3(%result : i32)
    
  ^bb2:
    %div = arith.divsi %c10_i32, %arg0 : i32
    cf.br ^bb3(%div : i32)
    
  ^bb3(%res: i32):
    return %res : i32
  }
}
```

### Crash Stack Trace
```bash
mlir-opt: /path/to/projects/llvm-project/mlir/lib/IR/BuiltinAttributes.cpp:127: static bool mlir::DictionaryAttr::sortInPlace(SmallVectorImpl<NamedAttribute> &): Assertion `!findDuplicateElement(array) && "DictionaryAttr element names must be unique"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts -o test.mlir
 #0 0x000055559a6fc61d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055559a6fcb4b PrintStackTraceSignalHandler(void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055559a6faa1f llvm::sys::RunSignalHandlers() /home/username/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055559a6fd1e9 SignalHandler(int, siginfo_t*, void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f5d34245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f5d3429eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f5d3429eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f5d3429eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f5d3424527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f5d342288ff abort ./stdlib/abort.c:81:7
#10 0x00007f5d3422881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f5d3423b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005555a842ea03 mlir::DictionaryAttr::sortInPlace(llvm::SmallVectorImpl<mlir::NamedAttribute>&) /home/username/projects/llvm-project/mlir/lib/IR/BuiltinAttributes.cpp:128:10
#13 0x00005555a859bbca mlir::NamedAttrList::getDictionary(mlir::MLIRContext*) const /home/username/projects/llvm-project/mlir/lib/IR/OperationSupport.cpp:56:5
#14 0x00005555a858cd00 mlir::Operation::create(mlir::OperationState const&) /home/username/projects/llvm-project/mlir/lib/IR/Operation.cpp:37:31
#15 0x00005555a8421601 mlir::OpBuilder::create(mlir::OperationState const&) /home/username/projects/llvm-project/mlir/lib/IR/Builders.cpp:458:17
#16 0x00005555a6de2410 mlir::LLVM::LLVMFuncOp::create(mlir::OpBuilder&, mlir::Location, llvm::StringRef, mlir::Type, mlir::LLVM::linkage::Linkage, bool, mlir::LLVM::cconv::CConv, mlir::SymbolRefAttr, llvm::ArrayRef<mlir::NamedAttribute>, llvm::ArrayRef<mlir::DictionaryAttr>, std::optional<unsigned long>) /home/username/projects/llvm-project/build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:31926:55
#17 0x00005555a2cf3389 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&, mlir::LLVMTypeConverter const&, mlir::SymbolTableCollection*) /home/username/projects/llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:360:20
#18 0x00005555a2cf8c22 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&) const /home/username/projects/llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:490:45
#19 0x000055559a894969 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>, mlir::func::FuncOp>(mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp> const&, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /home/username/projects/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:1025:15
#20 0x000055559a893583 mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /home/username/projects/llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:246:12
#21 0x000055559a8934dc mlir::ConvertOpToLLVMPattern<mlir::func::FuncOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /home/username/projects/llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:232:12
#22 0x00005555a7f6bc59 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2359:10
#23 0x00005555a80047d8 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /home/username/projects/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#24 0x00005555a8004485 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x000055559a722879 llvm::function_ref<void ()>::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x00005555a800600b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#27 0x00005555a800286a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /home/username/projects/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#28 0x00005555a7f77fa8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2797:21
#29 0x00005555a7f6c922 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2563:17
#30 0x00005555a7f6c21b mlir::OperationConverter::convert(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3318:26
#31 0x00005555a7f6cc7b mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3415:16
#32 0x00005555a7f7e3ac applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4137:30
#33 0x00005555a7f7e315 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#34 0x000055559a722879 llvm::function_ref<void ()>::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#35 0x00005555a7f9b443 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#36 0x00005555a7f707a8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4141:1
#37 0x00005555a7f7065c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4150:10
#38 0x00005555a7f70845 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4157:10
#39 0x000055559d1266a9 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /home/username/projects/llvm-project/mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#40 0x00005555a80c3864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#41 0x00005555a80c3805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#42 0x000055559a722879 llvm::function_ref<void ()>::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#43 0x00005555a80c6f2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#44 0x00005555a80bacfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#45 0x00005555a80bb3da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#46 0x00005555a80c0ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#47 0x00005555a80c0256 mlir::PassManager::run(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#48 0x000055559a7fd49b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#49 0x000055559a7fcb71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#50 0x000055559a7fc82c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#51 0x000055559a7fc736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#52 0x00005555a8605cba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#53 0x00005555a860507a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /home/username/projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#54 0x000055559a7f8126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#55 0x000055559a7f8525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#56 0x000055559a7f86f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#57 0x000055559a6d1935 main /home/username/projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#58 0x00007f5d3422a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#59 0x00007f5d3422a28b call_init ./csu/../csu/libc-start.c:128:20
#60 0x00007f5d3422a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#61 0x000055559a6d1505 _start (/mnt/raid/home/username/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to lowering it:
```bash
mlir-opt input.mlir \
  -gpu-kernel-outlining \
  --convert-scf-to-cf \
  --affine-loop-coalescing \
  --convert-gpu-to-nvvm \
  -reconcile-unrealized-casts \
  --nvvm-attach-target=chip=sm_90 \
  --gpu-module-to-binary \
  -gpu-to-llvm \
  --convert-cf-to-llvm \
  --convert-index-to-llvm \
  --convert-arith-to-llvm \
  --convert-math-to-llvm \
  --convert-func-to-llvm \
  --finalize-memref-to-llvm \
  -convert-nvvm-to-llvm \
  -reconcile-unrealized-casts \
  -o test.mlir
```

The version:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I find a crash bug during using MLIR to lower programs. See the minimized code exampel below:

File Name: **input.mlir**
```llvm
module {
  func.func @<!-- -->main(%arg0: i32) -&gt; i32 attributes {linkage = #llvm.linkage&lt;external&gt;} {
    %c0_i32 = arith.constant 0 : i32
    %c10_i32 = arith.constant 10 : i32
    %c1_i32 = arith.constant 1 : i32
    %cond = arith.cmpi eq, %arg0, %c0_i32 : i32
    cf.cond_br %cond, ^bb1, ^bb2
    
  ^bb1:
    %result = arith.muli %c10_i32, %c1_i32 : i32
    cf.br ^bb3(%result : i32)
    
  ^bb2:
    %div = arith.divsi %c10_i32, %arg0 : i32
    cf.br ^bb3(%div : i32)
    
  ^bb3(%res: i32):
    return %res : i32
  }
}
```

```bash
mlir-opt: /path/to/projects/llvm-project/mlir/lib/IR/BuiltinAttributes.cpp:127: static bool mlir::DictionaryAttr::sortInPlace(SmallVectorImpl&lt;NamedAttribute&gt; &amp;): Assertion `!findDuplicateElement(array) &amp;&amp; "DictionaryAttr element names must be unique"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts -o test.mlir
 #<!-- -->0 0x000055559a6fc61d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055559a6fcb4b PrintStackTraceSignalHandler(void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055559a6faa1f llvm::sys::RunSignalHandlers() /home/username/projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055559a6fd1e9 SignalHandler(int, siginfo_t*, void*) /home/username/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f5d34245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f5d3429eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f5d3429eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f5d3429eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f5d3424527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f5d342288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f5d3422881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f5d3423b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005555a842ea03 mlir::DictionaryAttr::sortInPlace(llvm::SmallVectorImpl&lt;mlir::NamedAttribute&gt;&amp;) /home/username/projects/llvm-project/mlir/lib/IR/BuiltinAttributes.cpp:128:10
#<!-- -->13 0x00005555a859bbca mlir::NamedAttrList::getDictionary(mlir::MLIRContext*) const /home/username/projects/llvm-project/mlir/lib/IR/OperationSupport.cpp:56:5
#<!-- -->14 0x00005555a858cd00 mlir::Operation::create(mlir::OperationState const&amp;) /home/username/projects/llvm-project/mlir/lib/IR/Operation.cpp:37:31
#<!-- -->15 0x00005555a8421601 mlir::OpBuilder::create(mlir::OperationState const&amp;) /home/username/projects/llvm-project/mlir/lib/IR/Builders.cpp:458:17
#<!-- -->16 0x00005555a6de2410 mlir::LLVM::LLVMFuncOp::create(mlir::OpBuilder&amp;, mlir::Location, llvm::StringRef, mlir::Type, mlir::LLVM::linkage::Linkage, bool, mlir::LLVM::cconv::CConv, mlir::SymbolRefAttr, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::ArrayRef&lt;mlir::DictionaryAttr&gt;, std::optional&lt;unsigned long&gt;) /home/username/projects/llvm-project/build_all/tools/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.cpp.inc:31926:55
#<!-- -->17 0x00005555a2cf3389 mlir::convertFuncOpToLLVMFuncOp(mlir::FunctionOpInterface, mlir::ConversionPatternRewriter&amp;, mlir::LLVMTypeConverter const&amp;, mlir::SymbolTableCollection*) /home/username/projects/llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:360:20
#<!-- -->18 0x00005555a2cf8c22 (anonymous namespace)::FuncOpConversion::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /home/username/projects/llvm-project/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp:490:45
#<!-- -->19 0x000055559a894969 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;, mlir::func::FuncOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt; const&amp;, mlir::func::FuncOp, mlir::func::FuncOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /home/username/projects/llvm-project/mlir/include/mlir/Transforms/DialectConversion.h:1025:15
#<!-- -->20 0x000055559a893583 mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::func::FuncOp, mlir::func::FuncOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /home/username/projects/llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:246:12
#<!-- -->21 0x000055559a8934dc mlir::ConvertOpToLLVMPattern&lt;mlir::func::FuncOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /home/username/projects/llvm-project/mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:232:12
#<!-- -->22 0x00005555a7f6bc59 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2359:10
#<!-- -->23 0x00005555a80047d8 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /home/username/projects/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->24 0x00005555a8004485 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x000055559a722879 llvm::function_ref&lt;void ()&gt;::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005555a800600b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x00005555a800286a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /home/username/projects/llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->28 0x00005555a7f77fa8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2797:21
#<!-- -->29 0x00005555a7f6c922 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:2563:17
#<!-- -->30 0x00005555a7f6c21b mlir::OperationConverter::convert(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3318:26
#<!-- -->31 0x00005555a7f6cc7b mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:3415:16
#<!-- -->32 0x00005555a7f7e3ac applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4137:30
#<!-- -->33 0x00005555a7f7e315 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x000055559a722879 llvm::function_ref&lt;void ()&gt;::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x00005555a7f9b443 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x00005555a7f707a8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4141:1
#<!-- -->37 0x00005555a7f7065c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4150:10
#<!-- -->38 0x00005555a7f70845 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /home/username/projects/llvm-project/mlir/lib/Transforms/Utils/DialectConversion.cpp:4157:10
#<!-- -->39 0x000055559d1266a9 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /home/username/projects/llvm-project/mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#<!-- -->40 0x00005555a80c3864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->41 0x00005555a80c3805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->42 0x000055559a722879 llvm::function_ref&lt;void ()&gt;::operator()() const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->43 0x00005555a80c6f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /home/username/projects/llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->44 0x00005555a80bacfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->45 0x00005555a80bb3da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->46 0x00005555a80c0ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->47 0x00005555a80c0256 mlir::PassManager::run(mlir::Operation*) /home/username/projects/llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->48 0x000055559a7fd49b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->49 0x000055559a7fcb71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->50 0x000055559a7fc82c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->51 0x000055559a7fc736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->52 0x00005555a8605cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /home/username/projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->53 0x00005555a860507a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /home/username/projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->54 0x000055559a7f8126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->55 0x000055559a7f8525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->56 0x000055559a7f86f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /home/username/projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->57 0x000055559a6d1935 main /home/username/projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->58 0x00007f5d3422a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->59 0x00007f5d3422a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007f5d3422a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->61 0x000055559a6d1505 _start (/mnt/raid/home/username/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to lowering it:
```bash
mlir-opt input.mlir \
  -gpu-kernel-outlining \
  --convert-scf-to-cf \
  --affine-loop-coalescing \
  --convert-gpu-to-nvvm \
  -reconcile-unrealized-casts \
  --nvvm-attach-target=chip=sm_90 \
  --gpu-module-to-binary \
  -gpu-to-llvm \
  --convert-cf-to-llvm \
  --convert-index-to-llvm \
  --convert-arith-to-llvm \
  --convert-math-to-llvm \
  --convert-func-to-llvm \
  --finalize-memref-to-llvm \
  -convert-nvvm-to-llvm \
  -reconcile-unrealized-casts \
  -o test.mlir
```

The version:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```
</details>


---

### Comment 2 - ianayl

May I ask if you're just finding bugs, or do you need to do something with MLIR? If you need MLIR to do something, have you tried disabling assertions and debug build? I noticed [you've made quite a number of issues recently](https://github.com/llvm/llvm-project/issues?q=is%3Aissue%20state%3Aopen%20author%3ARJerrica) 

A lot of these issues seem to be because of assertions; I'm able to reproduce this one as of 86c2f5d67863254f1d5b2fcd10f402d8e2f99da4, but I noticed the reproducer worked just fine with assertions disabled 

---

### Comment 3 - joker-eph

Disabling assertions is never a good suggestion facing a problem: they are here for good reasons and all of these are serious bugs.

---

### Comment 4 - ianayl

> Disabling assertions is never a good suggestion facing a problem: they are here for good reasons and all of these are serious bugs.

I made the suggestion incase the person needed to do something with MLIR (i.e. university research or personal projects) and needed it working in a time-sensitive manner; I didn't mean to dismiss the bug as not a problem, I'm actually looking at the traces right now 😅

---

### Comment 5 - ianayl

Simpler reproducer:
```
module {
  func.func @main() attributes {linkage = #llvm.linkage<external>} {  # This linkage attribute is causing issues
  return
  }
}
```
To reproduce:
```bash
mlir-opt input.mlir --convert-func-to-llvm
```
Looks like when rewriting `FuncOp` and creating a new `LLVMFuncOp`, an additional `#llvm.linkage<external>` is pushed onto the attributes list regardless of what the original `linkage` attribute is set to, which causes the assertion failed in OP's backtrace.

---

### Comment 6 - ianayl

Looks like FuncToLLVM uses `llvm.linkage` as the name/key for the linkage attribute:
https://github.com/llvm/llvm-project/blob/25e81394ce1cec0f7e6e46fb20d50a1ac1ff0083/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp#L52
Thus, the linkage attribute in OP's reproducer never gets picked up/detected when the new `LLVMFuncOp` replacing the old `FuncOp` is created: 
https://github.com/llvm/llvm-project/blob/25e81394ce1cec0f7e6e46fb20d50a1ac1ff0083/mlir/lib/Conversion/FuncToLLVM/FuncToLLVM.cpp#L326
This results in our new `LLVMFuncOp` having both `linkage` as defined in the reproducer, and also an additional `linkage` attribute that is created by default here:
https://github.com/llvm/llvm-project/blob/0a9d480fadf07aaaee8ccfa26a2a2afa3f621499/mlir/include/mlir/Dialect/LLVMIR/LLVMOps.td#L1950
IIRC ODS would take `$linkage` here as the name/key to use for the attribute (i.e. `hasAttr`, etc)? This should create the assertion failure we see here, as the name that is automatically used by ODS conflicts with the `linkage` attribute defined in the reproducer.


Admittedly, I am still relatively new to MLIR, so I hope I'm understanding this correctly: It would seem like the tablegen files mentioned above results in conflicting names/key for the linkage attribute compared to e.g. the name/key used in FuncToLLVM pass, and in our testing too:
https://github.com/llvm/llvm-project/blob/0d481bf0b9523b5089d70b561dafa5e80615178f/mlir/test/Conversion/FuncToLLVM/convert-funcs.mlir#L32

~~Thus, what should this linkage attribute actually look like in raw mlir? Should it still be called `llvm.linkage`? If we do stick with `llvm_linkage`, how should we reconcile the fact that we use `$linkage` in our tablegen files? Or is it possible to write some sort of custom parser to handle different attribute names?~~

I made a discourse thread to discuss this: https://discourse.llvm.org/t/confusion-regarding-llvm-linkage-attribute-should-it-be-linkage-or-llvm-linkage/89497


---

