# [MLIR] Crash when lowering scf.while with memref-typed variables in GPU matrix multiplication

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/182649
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-04T11:03:48Z

## Body

Hi, I get a crash when lowering a matrix multiplication program, see the following code example.

```llvm
module {
  func.func @computeGold(%C: memref<?xf32>, %A: memref<?xf32>, %B: memref<?xf32>, %hA: index, %wA: index, %wB: index) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c0_f32 = arith.constant 0.0 : f32
    %zero_f64 = arith.constant 0.0 : f64
    %init = memref.alloca() : memref<1xf64>
    memref.store %zero_f64, %init[%c0] : memref<1xf64>
    scf.while (%i = %c0, %j = %c0, %acc = %init) : (index, index, memref<1xf64>) -> (index, index, memref<1xf64>) {
      %i_lt = arith.cmpi slt, %i, %hA : index
      %j_lt = arith.cmpi slt, %j, %wB : index
      %and = arith.andi %i_lt, %j_lt : i1
      scf.condition(%and) %i, %j, %acc : index, index, memref<1xf64>
    } do {
    ^bb0(%i: index, %j: index, %acc: memref<1xf64>):
      %inner = memref.alloca() : memref<1xf64>
      memref.store %zero_f64, %inner[%c0] : memref<1xf64>
      scf.for %k = %c0 to %wA step %c1 {
        %A_idx = arith.muli %i, %wA : index
        %A_idx2 = arith.addi %A_idx, %k : index
        %a_val = memref.load %A[%A_idx2] : memref<?xf32>
        %a_f64 = arith.extf %a_val : f32 to f64
        %B_idx = arith.muli %k, %wB : index
        %B_idx2 = arith.addi %B_idx, %j : index
        %b_val = memref.load %B[%B_idx2] : memref<?xf32>
        %b_f64 = arith.extf %b_val : f32 to f64
        %mul = arith.mulf %a_f64, %b_f64 : f64
        %old_sum = memref.load %inner[%c0] : memref<1xf64>
        %new_sum = arith.addf %old_sum, %mul : f64
        memref.store %new_sum, %inner[%c0] : memref<1xf64>
      }
      %C_idx = arith.muli %i, %wB : index
      %C_idx2 = arith.addi %C_idx, %j : index
      %final_sum = memref.load %inner[%c0] : memref<1xf64>
      %final_f32 = arith.truncf %final_sum : f64 to f32
      memref.store %final_f32, %C[%C_idx2] : memref<?xf32>
      %next_i = arith.addi %i, %c1 : index
      %next_j = arith.addi %j, %c1 : index
      scf.yield %next_i, %next_j, %acc : index, index, memref<1xf64>
    }
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt matrixMulDynlinkJIT.mlir --convert-to-emitc --convert-gpu-to-nvvm
 #0 0x000061946c0631fd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000061946c06372b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000061946c0615c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000061946c063ec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000706ff7645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000706ff769eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000706ff769eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000706ff769eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000706ff764527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000706ff76288ff abort ./stdlib/abort.c:81:7
#10 0x0000706ff762881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000706ff763b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000061946f1ba560 mlir::emitc::LValueType mlir::detail::StorageUserBase<mlir::emitc::LValueType, mlir::Type, mlir::emitc::detail::LValueTypeStorage, mlir::detail::TypeUniquer>::get<mlir::Type&>(mlir::MLIRContext*, mlir::Type&) /mlir/include/mlir/IR/StorageUniquerSupport.h:179:5
#13 0x000061946f139871 mlir::emitc::LValueType::get(mlir::Type) /build_all/tools/mlir/include/mlir/Dialect/EmitC/IR/EmitCTypes.cpp.inc:200:10
#14 0x000061946f11f526 llvm::LogicalResult (anonymous namespace)::createVariablesForResults<mlir::scf::WhileOp>(mlir::scf::WhileOp, mlir::TypeConverter const*, mlir::ConversionPatternRewriter&, llvm::SmallVector<mlir::Value, 6u>&) /mlir/lib/Conversion/SCFToEmitC/SCFToEmitC.cpp:92:20
#15 0x000061946f12478d WhileLowering::matchAndRewrite(mlir::scf::WhileOp, mlir::scf::WhileOpAdaptor, mlir::ConversionPatternRewriter&) const /mlir/lib/Conversion/SCFToEmitC/SCFToEmitC.cpp:354:16
#16 0x000061946f126998 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<mlir::scf::WhileOp>, mlir::scf::WhileOp>(mlir::OpConversionPattern<mlir::scf::WhileOp> const&, mlir::scf::WhileOp, mlir::scf::WhileOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#17 0x000061946f124a6f mlir::OpConversionPattern<mlir::scf::WhileOp>::matchAndRewrite(mlir::scf::WhileOp, mlir::scf::WhileOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Transforms/DialectConversion.h:727:12
#18 0x000061946f124708 mlir::OpConversionPattern<mlir::scf::WhileOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Transforms/DialectConversion.h:713:12
#19 0x000061947a5838d9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#20 0x000061947a61bc98 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#21 0x000061947a61b945 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#22 0x000061946c0895c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#23 0x000061947a61d49b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#24 0x000061947a619d2a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#25 0x000061947a5904b8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#26 0x000061947a5845d2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#27 0x000061947a583ea2 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#28 0x000061947a584f6d llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#29 0x000061947a584b1e mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#30 0x000061947a59599c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#31 0x000061947a595905 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x000061946c0895c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x000061947a5b1db3 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#34 0x000061947a588718 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#35 0x000061947a5885cc mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#36 0x000061947a5887b5 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#37 0x000061946e8cf6c1 (anonymous namespace)::StaticConvertToEmitC::transform(mlir::Operation*, mlir::AnalysisManager) const /mlir/lib/Conversion/ConvertToEmitC/ConvertToEmitCPass.cpp:139:16
#38 0x000061946e8cdf40 (anonymous namespace)::ConvertToEmitC::runOnOperation() /mlir/lib/Conversion/ConvertToEmitC/ConvertToEmitCPass.cpp:171:22
#39 0x000061947a6dd734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#40 0x000061947a6dd6d5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#41 0x000061946c0895c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#42 0x000061947a6e0bdb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#43 0x000061947a6d4b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#44 0x000061947a6d529a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#45 0x000061947a6dab7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#46 0x000061947a6da106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#47 0x000061946c16db4b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#48 0x000061946c16d221 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#49 0x000061946c16cedc mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#50 0x000061946c16cde6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#51 0x000061947ac3f88a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#52 0x000061947ac3ec4a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#53 0x000061946c168b66 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#54 0x000061946c168f68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#55 0x000061946c169138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#56 0x000061946c037de5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#57 0x0000706ff762a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#58 0x0000706ff762a28b call_init ./csu/../csu/libc-start.c:128:20
#59 0x0000706ff762a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#60 0x000061946c037c55 _start (/mnt/raid/build_all/bin/mlir-opt+0x4742c55)
Aborted
```

Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I get a crash when lowering a matrix multiplication program, see the following code example.

```llvm
module {
  func.func @<!-- -->computeGold(%C: memref&lt;?xf32&gt;, %A: memref&lt;?xf32&gt;, %B: memref&lt;?xf32&gt;, %hA: index, %wA: index, %wB: index) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c0_f32 = arith.constant 0.0 : f32
    %zero_f64 = arith.constant 0.0 : f64
    %init = memref.alloca() : memref&lt;1xf64&gt;
    memref.store %zero_f64, %init[%c0] : memref&lt;1xf64&gt;
    scf.while (%i = %c0, %j = %c0, %acc = %init) : (index, index, memref&lt;1xf64&gt;) -&gt; (index, index, memref&lt;1xf64&gt;) {
      %i_lt = arith.cmpi slt, %i, %hA : index
      %j_lt = arith.cmpi slt, %j, %wB : index
      %and = arith.andi %i_lt, %j_lt : i1
      scf.condition(%and) %i, %j, %acc : index, index, memref&lt;1xf64&gt;
    } do {
    ^bb0(%i: index, %j: index, %acc: memref&lt;1xf64&gt;):
      %inner = memref.alloca() : memref&lt;1xf64&gt;
      memref.store %zero_f64, %inner[%c0] : memref&lt;1xf64&gt;
      scf.for %k = %c0 to %wA step %c1 {
        %A_idx = arith.muli %i, %wA : index
        %A_idx2 = arith.addi %A_idx, %k : index
        %a_val = memref.load %A[%A_idx2] : memref&lt;?xf32&gt;
        %a_f64 = arith.extf %a_val : f32 to f64
        %B_idx = arith.muli %k, %wB : index
        %B_idx2 = arith.addi %B_idx, %j : index
        %b_val = memref.load %B[%B_idx2] : memref&lt;?xf32&gt;
        %b_f64 = arith.extf %b_val : f32 to f64
        %mul = arith.mulf %a_f64, %b_f64 : f64
        %old_sum = memref.load %inner[%c0] : memref&lt;1xf64&gt;
        %new_sum = arith.addf %old_sum, %mul : f64
        memref.store %new_sum, %inner[%c0] : memref&lt;1xf64&gt;
      }
      %C_idx = arith.muli %i, %wB : index
      %C_idx2 = arith.addi %C_idx, %j : index
      %final_sum = memref.load %inner[%c0] : memref&lt;1xf64&gt;
      %final_f32 = arith.truncf %final_sum : f64 to f32
      memref.store %final_f32, %C[%C_idx2] : memref&lt;?xf32&gt;
      %next_i = arith.addi %i, %c1 : index
      %next_j = arith.addi %j, %c1 : index
      scf.yield %next_i, %next_j, %acc : index, index, memref&lt;1xf64&gt;
    }
    return
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt matrixMulDynlinkJIT.mlir --convert-to-emitc
 #<!-- -->0 0x000061946c0631fd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000061946c06372b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000061946c0615c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000061946c063ec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000706ff7645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000706ff769eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000706ff769eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000706ff769eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000706ff764527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000706ff76288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000706ff762881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000706ff763b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000061946f1ba560 mlir::emitc::LValueType mlir::detail::StorageUserBase&lt;mlir::emitc::LValueType, mlir::Type, mlir::emitc::detail::LValueTypeStorage, mlir::detail::TypeUniquer&gt;::get&lt;mlir::Type&amp;&gt;(mlir::MLIRContext*, mlir::Type&amp;) /mlir/include/mlir/IR/StorageUniquerSupport.h:179:5
#<!-- -->13 0x000061946f139871 mlir::emitc::LValueType::get(mlir::Type) /build_all/tools/mlir/include/mlir/Dialect/EmitC/IR/EmitCTypes.cpp.inc:200:10
#<!-- -->14 0x000061946f11f526 llvm::LogicalResult (anonymous namespace)::createVariablesForResults&lt;mlir::scf::WhileOp&gt;(mlir::scf::WhileOp, mlir::TypeConverter const*, mlir::ConversionPatternRewriter&amp;, llvm::SmallVector&lt;mlir::Value, 6u&gt;&amp;) /mlir/lib/Conversion/SCFToEmitC/SCFToEmitC.cpp:92:20
#<!-- -->15 0x000061946f12478d WhileLowering::matchAndRewrite(mlir::scf::WhileOp, mlir::scf::WhileOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/SCFToEmitC/SCFToEmitC.cpp:354:16
#<!-- -->16 0x000061946f126998 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;mlir::scf::WhileOp&gt;, mlir::scf::WhileOp&gt;(mlir::OpConversionPattern&lt;mlir::scf::WhileOp&gt; const&amp;, mlir::scf::WhileOp, mlir::scf::WhileOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->17 0x000061946f124a6f mlir::OpConversionPattern&lt;mlir::scf::WhileOp&gt;::matchAndRewrite(mlir::scf::WhileOp, mlir::scf::WhileOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Transforms/DialectConversion.h:727:12
#<!-- -->18 0x000061946f124708 mlir::OpConversionPattern&lt;mlir::scf::WhileOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Transforms/DialectConversion.h:713:12
#<!-- -->19 0x000061947a5838d9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#<!-- -->20 0x000061947a61bc98 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->21 0x000061947a61b945 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->22 0x000061946c0895c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->23 0x000061947a61d49b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->24 0x000061947a619d2a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->25 0x000061947a5904b8 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->26 0x000061947a5845d2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->27 0x000061947a583ea2 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#<!-- -->28 0x000061947a584f6d llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#<!-- -->29 0x000061947a584b1e mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#<!-- -->30 0x000061947a59599c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#<!-- -->31 0x000061947a595905 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x000061946c0895c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x000061947a5b1db3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x000061947a588718 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#<!-- -->35 0x000061947a5885cc mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#<!-- -->36 0x000061947a5887b5 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#<!-- -->37 0x000061946e8cf6c1 (anonymous namespace)::StaticConvertToEmitC::transform(mlir::Operation*, mlir::AnalysisManager) const /mlir/lib/Conversion/ConvertToEmitC/ConvertToEmitCPass.cpp:139:16
#<!-- -->38 0x000061946e8cdf40 (anonymous namespace)::ConvertToEmitC::runOnOperation() /mlir/lib/Conversion/ConvertToEmitC/ConvertToEmitCPass.cpp:171:22
#<!-- -->39 0x000061947a6dd734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->40 0x000061947a6dd6d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->41 0x000061946c0895c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->42 0x000061947a6e0bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->43 0x000061947a6d4b7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->44 0x000061947a6d529a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->45 0x000061947a6dab7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->46 0x000061947a6da106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->47 0x000061946c16db4b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->48 0x000061946c16d221 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->49 0x000061946c16cedc mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->50 0x000061946c16cde6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x000061947ac3f88a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x000061947ac3ec4a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->53 0x000061946c168b66 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->54 0x000061946c168f68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->55 0x000061946c169138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->56 0x000061946c037de5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->57 0x0000706ff762a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->58 0x0000706ff762a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x0000706ff762a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->60 0x000061946c037c55 _start (/mnt/raid/build_all/bin/mlir-opt+0x4742c55)
Aborted
```

Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77
</details>


---

### Comment 2 - rishabhmadan19

- [ ] Memref not supported by 'emitc' Dialect

```c++
LogicalResult mlir::emitc::LValueType::verify(
    llvm::function_ref<mlir::InFlightDiagnostic()> emitError,
    mlir::Type value) {
  // Check that the wrapped type is valid. This especially forbids nested
  // lvalue types.
  if (!isSupportedEmitCType(value))
    return emitError()
           << "!emitc.lvalue must wrap supported emitc type, but got " << value;

  if (llvm::isa<emitc::ArrayType>(value))
    return emitError() << "!emitc.lvalue cannot wrap !emitc.array type"; //  -------------------> **_Memref not supported by 'emitc' Dialect_** 

  return success();
}
```

---
To check run: /mlir-opt matrixMulDynlinkJIT.mlir --convert-to-emitc -mlir-print-debuginfo


---

### Comment 3 - LynnBroe

Understood, but here we should allow MLIR to exit gracefully instead of crashing directly. How can this be fixed? @rishabhmadan19 

---

