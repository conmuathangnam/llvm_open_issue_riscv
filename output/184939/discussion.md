# [MLIR] Crash during gpu to spir-v conversion: UNREACHABLE at mlir::LLVMTypeConverter::promoteOperands

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/184939
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-09T11:53:10Z

## Body

Hi, I encountered a crash while lowering a gpu-dialect program to spir-v. Here's a code example:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
module attributes {gpu.container_module} {
    func.func @initMergeSort() -> i32 {
        %c32768_i32 = arith.constant 32768 : i32
        %c1_i32 = arith.constant 1 : i32
        %c2_i32 = arith.constant 2 : i32
        
        call @closeMergeSort() : () -> ()
        cf.br ^bb1
        
        ^bb1:
        %iter = arith.muli %c32768_i32, %c2_i32 : i32
        %cond = arith.cmpi ne, %iter, %c32768_i32 : i32
        cf.cond_br %cond, ^bb2, ^bb3
        
        ^bb2:
        %d_RanksA = gpu.alloc () : memref<32x32xi32>
        %c2 = arith.constant 2 : index
        %c1 = arith.constant 1 : index
        %c16 = arith.constant 16 : index
        gpu.launch blocks(%bX, %bY, %bZ) in (%gridX = %c2, %gridY = %c1, %gridZ = %c1) threads(%tX, %tY, %tZ) in (%threadX = %c16, %threadY = %c1, %threadZ = %c1) {
            %bX_ = gpu.block_id x
            %bY_ = gpu.block_id y
            %bZ_ = gpu.block_id z
            %tX_ = gpu.thread_id x
            %tY_ = gpu.thread_id y
            %tZ_ = gpu.thread_id z
            %0 = memref.cast %d_RanksA : memref<32x32xi32> to memref<*xi32>
            gpu.launch_func @kernel blocks in (%c2, %c1, %c1) threads in (%c16, %c1, %c1) args(%0 : memref<*xi32>)
            gpu.terminator
        }
        cf.br ^bb1
        
        ^bb3:
        %c0_i = arith.constant 0 : index
        %c2_i = arith.constant 2 : index
        %c1_i = arith.constant 1 : index
        %d_RanksB = memref.alloc(%c2_i) : memref<?xi32>
        %d_LimitsA = memref.alloc(%c2_i) : memref<?xi32>
        scf.for %i = %c0_i to %c2_i step %c1_i {
            %tmp = arith.constant 1 : i32
        }
        return %iter : i32
    }

    func.func private @kernel(%arg0: memref<*xi32>) attributes {gpu.kernel} {
        gpu.barrier
        return
    }

    func.func @closeMergeSort() {
        %c0 = arith.constant 0 : i32
        %cond = arith.cmpi sgt, %c0, %c0 : i32
        scf.if %cond {
            gpu.wait
        }
        return
    }
}
```

### Reproduction Command
```bash
mlir-opt mergeSort.mlir "-pass-pipeline=builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true})"
```


### Crash Trace
```bash
UNREACHABLE executed at /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:774!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt mergeSort.mlir "-pass-pipeline=builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true})"
 #0 0x0000631a0db7cbed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x0000631a0db7d11b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000631a0db7afb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000631a0db7d8b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x0000794c29445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000794c2949eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000794c2949eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000794c2949eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000794c2944527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000794c294288ff abort ./stdlib/abort.c:81:7
#10 0x0000631a0db55dd8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#11 0x0000631a189f2539 mlir::LLVMTypeConverter::promoteOperands(mlir::Location, mlir::ValueRange, llvm::ArrayRef<mlir::ValueRange>, mlir::OpBuilder&, bool) const /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:0:9
#12 0x0000631a189f22cb mlir::LLVMTypeConverter::promoteOperands(mlir::Location, mlir::ValueRange, mlir::ValueRange, mlir::OpBuilder&, bool) const /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:755:1
#13 0x0000631a1089b8e6 (anonymous namespace)::LegalizeLaunchFuncOpPattern::matchAndRewrite(mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOpAdaptor, mlir::ConversionPatternRewriter&) const /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:979:41
#14 0x0000631a108c09db llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::gpu::LaunchFuncOp, false>, mlir::gpu::LaunchFuncOp>(mlir::ConvertOpToLLVMPattern<mlir::gpu::LaunchFuncOp, false> const&, mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#15 0x0000631a108bf35e mlir::ConvertOpToLLVMPattern<mlir::gpu::LaunchFuncOp, false>::matchAndRewrite(mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:276:12
#16 0x0000631a108bf2a7 mlir::ConvertOpToLLVMPattern<mlir::gpu::LaunchFuncOp, false>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:262:12
#17 0x0000631a1c3404a9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#18 0x0000631a1c3d8738 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#19 0x0000631a1c3d83e5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#20 0x0000631a0dba2fb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#21 0x0000631a1c3d9f3b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#22 0x0000631a1c3d67ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#23 0x0000631a1c34d088 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#24 0x0000631a1c3411a2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#25 0x0000631a1c340a72 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#26 0x0000631a1c341b3d llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#27 0x0000631a1c3416ee mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#28 0x0000631a1c35256c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#29 0x0000631a1c3524d5 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#30 0x0000631a0dba2fb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#31 0x0000631a1c36e8d3 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#32 0x0000631a1c3452e8 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#33 0x0000631a1c34519c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#34 0x0000631a1c345385 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#35 0x0000631a1082c069 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#36 0x0000631a1c49a1d4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#37 0x0000631a1c49a175 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#38 0x0000631a0dba2fb9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#39 0x0000631a1c49d67b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#40 0x0000631a1c49161e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#41 0x0000631a1c491d3a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#42 0x0000631a1c49761d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#43 0x0000631a1c496ba6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#44 0x0000631a0dc87b9b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#45 0x0000631a0dc87271 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#46 0x0000631a0dc86f2c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#47 0x0000631a0dc86e36 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#48 0x0000631a1ca029da llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#49 0x0000631a1ca01d9a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#50 0x0000631a0dc82ae6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#51 0x0000631a0dc82ee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#52 0x0000631a0dc830b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#53 0x0000631a0db517d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#54 0x0000794c2942a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#55 0x0000794c2942a28b call_init ./csu/../csu/libc-start.c:128:20
#56 0x0000794c2942a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#57 0x0000631a0db51645 _start (./mlir-opt+0x47e6645)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Hi, I encountered a crash while lowering a gpu-dialect program to spir-v. Here's a code example:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
module attributes {gpu.container_module} {
    func.func @<!-- -->initMergeSort() -&gt; i32 {
        %c32768_i32 = arith.constant 32768 : i32
        %c1_i32 = arith.constant 1 : i32
        %c2_i32 = arith.constant 2 : i32
        
        call @<!-- -->closeMergeSort() : () -&gt; ()
        cf.br ^bb1
        
        ^bb1:
        %iter = arith.muli %c32768_i32, %c2_i32 : i32
        %cond = arith.cmpi ne, %iter, %c32768_i32 : i32
        cf.cond_br %cond, ^bb2, ^bb3
        
        ^bb2:
        %d_RanksA = gpu.alloc () : memref&lt;32x32xi32&gt;
        %c2 = arith.constant 2 : index
        %c1 = arith.constant 1 : index
        %c16 = arith.constant 16 : index
        gpu.launch blocks(%bX, %bY, %bZ) in (%gridX = %c2, %gridY = %c1, %gridZ = %c1) threads(%tX, %tY, %tZ) in (%threadX = %c16, %threadY = %c1, %threadZ = %c1) {
            %bX_ = gpu.block_id x
            %bY_ = gpu.block_id y
            %bZ_ = gpu.block_id z
            %tX_ = gpu.thread_id x
            %tY_ = gpu.thread_id y
            %tZ_ = gpu.thread_id z
            %0 = memref.cast %d_RanksA : memref&lt;32x32xi32&gt; to memref&lt;*xi32&gt;
            gpu.launch_func @<!-- -->kernel blocks in (%c2, %c1, %c1) threads in (%c16, %c1, %c1) args(%0 : memref&lt;*xi32&gt;)
            gpu.terminator
        }
        cf.br ^bb1
        
        ^bb3:
        %c0_i = arith.constant 0 : index
        %c2_i = arith.constant 2 : index
        %c1_i = arith.constant 1 : index
        %d_RanksB = memref.alloc(%c2_i) : memref&lt;?xi32&gt;
        %d_LimitsA = memref.alloc(%c2_i) : memref&lt;?xi32&gt;
        scf.for %i = %c0_i to %c2_i step %c1_i {
            %tmp = arith.constant 1 : i32
        }
        return %iter : i32
    }

    func.func private @<!-- -->kernel(%arg0: memref&lt;*xi32&gt;) attributes {gpu.kernel} {
        gpu.barrier
        return
    }

    func.func @<!-- -->closeMergeSort() {
        %c0 = arith.constant 0 : i32
        %cond = arith.cmpi sgt, %c0, %c0 : i32
        scf.if %cond {
            gpu.wait
        }
        return
    }
}
```

### Reproduction Command
```bash
mlir-opt mergeSort.mlir "-pass-pipeline=builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true})"
```


### Crash Trace
```bash
UNREACHABLE executed at /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:774!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt mergeSort.mlir "-pass-pipeline=builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true})"
 #<!-- -->0 0x0000631a0db7cbed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x0000631a0db7d11b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x0000631a0db7afb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000631a0db7d8b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x0000794c29445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000794c2949eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000794c2949eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000794c2949eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000794c2944527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000794c294288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000631a0db55dd8 /llvm/lib/Support/ErrorHandling.cpp:244:3
#<!-- -->11 0x0000631a189f2539 mlir::LLVMTypeConverter::promoteOperands(mlir::Location, mlir::ValueRange, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::OpBuilder&amp;, bool) const /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:0:9
#<!-- -->12 0x0000631a189f22cb mlir::LLVMTypeConverter::promoteOperands(mlir::Location, mlir::ValueRange, mlir::ValueRange, mlir::OpBuilder&amp;, bool) const /mlir/lib/Conversion/LLVMCommon/TypeConverter.cpp:755:1
#<!-- -->13 0x0000631a1089b8e6 (anonymous namespace)::LegalizeLaunchFuncOpPattern::matchAndRewrite(mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:979:41
#<!-- -->14 0x0000631a108c09db llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::LaunchFuncOp, false&gt;, mlir::gpu::LaunchFuncOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::LaunchFuncOp, false&gt; const&amp;, mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->15 0x0000631a108bf35e mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::LaunchFuncOp, false&gt;::matchAndRewrite(mlir::gpu::LaunchFuncOp, mlir::gpu::LaunchFuncOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:276:12
#<!-- -->16 0x0000631a108bf2a7 mlir::ConvertOpToLLVMPattern&lt;mlir::gpu::LaunchFuncOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:262:12
#<!-- -->17 0x0000631a1c3404a9 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#<!-- -->18 0x0000631a1c3d8738 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->19 0x0000631a1c3d83e5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x0000631a0dba2fb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x0000631a1c3d9f3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x0000631a1c3d67ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->23 0x0000631a1c34d088 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->24 0x0000631a1c3411a2 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->25 0x0000631a1c340a72 mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3309:26
#<!-- -->26 0x0000631a1c341b3d llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3410:16
#<!-- -->27 0x0000631a1c3416ee mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3456:26
#<!-- -->28 0x0000631a1c35256c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4179:30
#<!-- -->29 0x0000631a1c3524d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->30 0x0000631a0dba2fb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->31 0x0000631a1c36e8d3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->32 0x0000631a1c3452e8 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4183:1
#<!-- -->33 0x0000631a1c34519c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4192:10
#<!-- -->34 0x0000631a1c345385 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4199:10
#<!-- -->35 0x0000631a1082c069 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#<!-- -->36 0x0000631a1c49a1d4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->37 0x0000631a1c49a175 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->38 0x0000631a0dba2fb9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->39 0x0000631a1c49d67b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->40 0x0000631a1c49161e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->41 0x0000631a1c491d3a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->42 0x0000631a1c49761d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->43 0x0000631a1c496ba6 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->44 0x0000631a0dc87b9b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->45 0x0000631a0dc87271 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->46 0x0000631a0dc86f2c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->47 0x0000631a0dc86e36 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->48 0x0000631a1ca029da llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->49 0x0000631a1ca01d9a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->50 0x0000631a0dc82ae6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->51 0x0000631a0dc82ee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->52 0x0000631a0dc830b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->53 0x0000631a0db517d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->54 0x0000794c2942a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->55 0x0000794c2942a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->56 0x0000794c2942a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->57 0x0000631a0db51645 _start (./mlir-opt+0x47e6645)
Aborted
```
</details>


---

