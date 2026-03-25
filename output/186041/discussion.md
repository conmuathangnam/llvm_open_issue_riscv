# [MLIR] Crash in MemRefToLLVM conversion during GPU to XeVM lowering

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/186041
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-13T15:42:13Z

## Body

Hi, I found a crash bug when lowering gpu programs. See the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @kernels {
    gpu.func @main_kernel(%arg0: memref<64xf32>, %arg1: memref<8xf32>, %arg2: memref<128xi32, #gpu.address_space<workgroup>>) kernel attributes {block_align = 16 : i32} {
      %c0 = arith.constant 0 : index
      %c64 = arith.constant 64 : index
      %c128 = arith.constant 128 : index
      %c1 = arith.constant 1 : index
      %timer_shared = memref.alloc() : memref<128xi32, #gpu.address_space<workgroup>>
      %thread_clock = gpu.thread_id x
      %val = arith.constant 42 : i32
      memref.store %val, %timer_shared[%thread_clock] : memref<128xi32, #gpu.address_space<workgroup>>
      gpu.barrier
      func.call @timedReduction(%arg0, %arg1, %timer_shared) : (memref<64xf32>, memref<8xf32>, memref<128xi32, #gpu.address_space<workgroup>>) -> ()
      gpu.return
    }
    func.func @timedReduction(%arg0: memref<64xf32>, %arg1: memref<8xf32>, %arg2: memref<128xi32, #gpu.address_space<workgroup>>) {
      return
    }
  }

  func.func @host() {
    %c0_i32 = arith.constant 0 : i32
    %c0 = arith.constant 0 : index
    %c64 = arith.constant 64 : index
    %c1 = arith.constant 1 : index
    %c128 = arith.constant 128 : index
    %input_host = memref.alloc() : memref<64xf32>
    %output_host = memref.alloc() : memref<8xf32>
    %timer_host = memref.alloc() {alignment = 16 : i64} : memref<128xi32, 3>
    %cast_timer = memref.memory_space_cast %timer_host : memref<128xi32, 3> to memref<128xi32, #gpu.address_space<workgroup>>
    gpu.launch_func @kernels::@main_kernel blocks in (%c64, %c1, %c1) threads in (%c128, %c1, %c1) dynamic_shared_memory_size %c0_i32 args(%input_host : memref<64xf32>, %output_host : memref<8xf32>, %cast_timer : memref<128xi32, #gpu.address_space<workgroup>>)
    return
  }
}
```

#### Reproduction Command
```bash
mlir-opt time_reduce.mlir --gpu-lower-to-xevm-pipeline
```

#### Crash Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt time_reduce.mlir --gpu-lower-to-xevm-pipeline
 #0 0x0000558b3ea48dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x0000558b3ea492eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000558b3ea47184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x0000558b3ea49a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000071d0f0a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000558b3ebe3cc6 mlir::TypeStorage::getAbstractType() /mlir/include/mlir/IR/TypeSupport.h:173:12
 #6 0x0000558b3ebe3c78 mlir::Type::getTypeID() /mlir/include/mlir/IR/Types.h:101:37
 #7 0x0000558b4016bc65 bool mlir::detail::StorageUserBase<mlir::LLVM::LLVMStructType, mlir::Type, mlir::LLVM::detail::LLVMStructTypeStorage, mlir::detail::TypeUniquer, mlir::TypeTrait::IsMutable, mlir::DataLayoutTypeInterface::Trait, mlir::DestructurableTypeInterface::Trait>::classof<mlir::Type>(mlir::Type) /mlir/include/mlir/IR/StorageUniquerSupport.h:113:16
 #8 0x0000558b4016bc3d llvm::CastInfo<mlir::LLVM::LLVMStructType, mlir::Type const, void>::isPossible(mlir::Type) /mlir/include/mlir/IR/Types.h:377:7
 #9 0x0000558b4016bbe0 bool llvm::isa<mlir::LLVM::LLVMStructType, mlir::Type>(mlir::Type const&) /llvm/include/llvm/Support/Casting.h:548:3
#10 0x0000558b413c65d5 decltype(auto) llvm::cast<mlir::LLVM::LLVMStructType, mlir::Type>(mlir::Type const&) /llvm/include/llvm/Support/Casting.h:560:10
#11 0x0000558b47cacc34 (anonymous namespace)::MemorySpaceCastOpLowering::matchAndRewrite(mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOpAdaptor, mlir::ConversionPatternRewriter&) const /mlir/lib/Conversion/MemRefToLLVM/MemRefToLLVM.cpp:1259:11
#12 0x0000558b47cc6d68 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::memref::MemorySpaceCastOp, false>, mlir::memref::MemorySpaceCastOp>(mlir::ConvertOpToLLVMPattern<mlir::memref::MemorySpaceCastOp, false> const&, mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#13 0x0000558b47cc68df mlir::ConvertOpToLLVMPattern<mlir::memref::MemorySpaceCastOp, false>::matchAndRewrite(mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:276:12
#14 0x0000558b47cc6858 mlir::ConvertOpToLLVMPattern<mlir::memref::MemorySpaceCastOp, false>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:262:12
#15 0x0000558b4d297e89 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#16 0x0000558b4d330338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#17 0x0000558b4d32ffe5 void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#18 0x0000558b3ea6f189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#19 0x0000558b4d331b3b void mlir::MLIRContext::executeAction<mlir::ApplyPatternAction, mlir::Pattern const&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pattern const&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#20 0x0000558b4d32e3ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#21 0x0000558b4d2a4b28 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#22 0x0000558b4d298a72 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#23 0x0000558b4d29845a mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3317:26
#24 0x0000558b4d2995bd llvm::LogicalResult mlir::OperationConverter::legalizeOperations<mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0>(llvm::ArrayRef<mlir::Operation*>, mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3416:16
#25 0x0000558b4d29916e mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3462:26
#26 0x0000558b4d2aa00c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4185:30
#27 0x0000558b4d2a9f75 void llvm::function_ref<void ()>::callback_fn<applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#28 0x0000558b3ea6f189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#29 0x0000558b4d2c64a3 void mlir::MLIRContext::executeAction<ApplyConversionAction>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>) /mlir/include/mlir/IR/MLIRContext.h:291:3
#30 0x0000558b4d29cd68 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:1
#31 0x0000558b4d29cc1c mlir::applyPartialConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4198:10
#32 0x0000558b4d29ce05 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4205:10
#33 0x0000558b4171d559 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#34 0x0000558b4d3f1fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#35 0x0000558b4d3f1f45 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#36 0x0000558b3ea6f189 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#37 0x0000558b4d3f544b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#38 0x0000558b4d3e93ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#39 0x0000558b4d3e9b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#40 0x0000558b4d3ef3ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#41 0x0000558b4d3ee976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#42 0x0000558b3eb53e7b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#43 0x0000558b3eb53551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#44 0x0000558b3eb5320c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#45 0x0000558b3eb53116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#46 0x0000558b4d95b6fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#47 0x0000558b4d95aaba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#48 0x0000558b3eb4edc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#49 0x0000558b3eb4f1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#50 0x0000558b3eb4f398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#51 0x0000558b3ea1d8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#52 0x000071d0f0a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#53 0x000071d0f0a2a28b call_init ./csu/../csu/libc-start.c:128:20
#54 0x000071d0f0a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#55 0x0000558b3ea1d745 _start (./mlir-opt+0x481e745)
Segmentation fault
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I found a crash bug when lowering gpu programs. See the following code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    gpu.func @<!-- -->main_kernel(%arg0: memref&lt;64xf32&gt;, %arg1: memref&lt;8xf32&gt;, %arg2: memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;) kernel attributes {block_align = 16 : i32} {
      %c0 = arith.constant 0 : index
      %c64 = arith.constant 64 : index
      %c128 = arith.constant 128 : index
      %c1 = arith.constant 1 : index
      %timer_shared = memref.alloc() : memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;
      %thread_clock = gpu.thread_id x
      %val = arith.constant 42 : i32
      memref.store %val, %timer_shared[%thread_clock] : memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;
      gpu.barrier
      func.call @<!-- -->timedReduction(%arg0, %arg1, %timer_shared) : (memref&lt;64xf32&gt;, memref&lt;8xf32&gt;, memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;) -&gt; ()
      gpu.return
    }
    func.func @<!-- -->timedReduction(%arg0: memref&lt;64xf32&gt;, %arg1: memref&lt;8xf32&gt;, %arg2: memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;) {
      return
    }
  }

  func.func @<!-- -->host() {
    %c0_i32 = arith.constant 0 : i32
    %c0 = arith.constant 0 : index
    %c64 = arith.constant 64 : index
    %c1 = arith.constant 1 : index
    %c128 = arith.constant 128 : index
    %input_host = memref.alloc() : memref&lt;64xf32&gt;
    %output_host = memref.alloc() : memref&lt;8xf32&gt;
    %timer_host = memref.alloc() {alignment = 16 : i64} : memref&lt;128xi32, 3&gt;
    %cast_timer = memref.memory_space_cast %timer_host : memref&lt;128xi32, 3&gt; to memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;
    gpu.launch_func @<!-- -->kernels::@<!-- -->main_kernel blocks in (%c64, %c1, %c1) threads in (%c128, %c1, %c1) dynamic_shared_memory_size %c0_i32 args(%input_host : memref&lt;64xf32&gt;, %output_host : memref&lt;8xf32&gt;, %cast_timer : memref&lt;128xi32, #gpu.address_space&lt;workgroup&gt;&gt;)
    return
  }
}
```

#### Reproduction Command
```bash
mlir-opt time_reduce.mlir --gpu-lower-to-xevm-pipeline
```

#### Crash Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt time_reduce.mlir --gpu-lower-to-xevm-pipeline
 #<!-- -->0 0x0000558b3ea48dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x0000558b3ea492eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x0000558b3ea47184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x0000558b3ea49a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000071d0f0a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000558b3ebe3cc6 mlir::TypeStorage::getAbstractType() /mlir/include/mlir/IR/TypeSupport.h:173:12
 #<!-- -->6 0x0000558b3ebe3c78 mlir::Type::getTypeID() /mlir/include/mlir/IR/Types.h:101:37
 #<!-- -->7 0x0000558b4016bc65 bool mlir::detail::StorageUserBase&lt;mlir::LLVM::LLVMStructType, mlir::Type, mlir::LLVM::detail::LLVMStructTypeStorage, mlir::detail::TypeUniquer, mlir::TypeTrait::IsMutable, mlir::DataLayoutTypeInterface::Trait, mlir::DestructurableTypeInterface::Trait&gt;::classof&lt;mlir::Type&gt;(mlir::Type) /mlir/include/mlir/IR/StorageUniquerSupport.h:113:16
 #<!-- -->8 0x0000558b4016bc3d llvm::CastInfo&lt;mlir::LLVM::LLVMStructType, mlir::Type const, void&gt;::isPossible(mlir::Type) /mlir/include/mlir/IR/Types.h:377:7
 #<!-- -->9 0x0000558b4016bbe0 bool llvm::isa&lt;mlir::LLVM::LLVMStructType, mlir::Type&gt;(mlir::Type const&amp;) /llvm/include/llvm/Support/Casting.h:548:3
#<!-- -->10 0x0000558b413c65d5 decltype(auto) llvm::cast&lt;mlir::LLVM::LLVMStructType, mlir::Type&gt;(mlir::Type const&amp;) /llvm/include/llvm/Support/Casting.h:560:10
#<!-- -->11 0x0000558b47cacc34 (anonymous namespace)::MemorySpaceCastOpLowering::matchAndRewrite(mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOpAdaptor, mlir::ConversionPatternRewriter&amp;) const /mlir/lib/Conversion/MemRefToLLVM/MemRefToLLVM.cpp:1259:11
#<!-- -->12 0x0000558b47cc6d68 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::memref::MemorySpaceCastOp, false&gt;, mlir::memref::MemorySpaceCastOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::memref::MemorySpaceCastOp, false&gt; const&amp;, mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) /mlir/include/mlir/Transforms/DialectConversion.h:1069:15
#<!-- -->13 0x0000558b47cc68df mlir::ConvertOpToLLVMPattern&lt;mlir::memref::MemorySpaceCastOp, false&gt;::matchAndRewrite(mlir::memref::MemorySpaceCastOp, mlir::memref::MemorySpaceCastOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:276:12
#<!-- -->14 0x0000558b47cc6858 mlir::ConvertOpToLLVMPattern&lt;mlir::memref::MemorySpaceCastOp, false&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const /mlir/include/mlir/Conversion/LLVMCommon/Pattern.h:262:12
#<!-- -->15 0x0000558b4d297e89 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const /mlir/lib/Transforms/Utils/DialectConversion.cpp:2410:10
#<!-- -->16 0x0000558b4d330338 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0::operator()() const /mlir/lib/Rewrite/PatternApplicator.cpp:223:31
#<!-- -->17 0x0000558b4d32ffe5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->18 0x0000558b3ea6f189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->19 0x0000558b4d331b3b void mlir::MLIRContext::executeAction&lt;mlir::ApplyPatternAction, mlir::Pattern const&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pattern const&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->20 0x0000558b4d32e3ca mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) /mlir/lib/Rewrite/PatternApplicator.cpp:242:9
#<!-- -->21 0x0000558b4d2a4b28 (anonymous namespace)::OperationLegalizer::legalizeWithPattern(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2831:21
#<!-- -->22 0x0000558b4d298a72 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) /mlir/lib/Transforms/Utils/DialectConversion.cpp:2609:17
#<!-- -->23 0x0000558b4d29845a mlir::OperationConverter::convert(mlir::Operation*, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3317:26
#<!-- -->24 0x0000558b4d2995bd llvm::LogicalResult mlir::OperationConverter::legalizeOperations&lt;mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0&gt;(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;)::$_0, bool) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3416:16
#<!-- -->25 0x0000558b4d29916e mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) /mlir/lib/Transforms/Utils/DialectConversion.cpp:3462:26
#<!-- -->26 0x0000558b4d2aa00c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0::operator()() const /mlir/lib/Transforms/Utils/DialectConversion.cpp:4185:30
#<!-- -->27 0x0000558b4d2a9f75 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode)::$_0&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->28 0x0000558b3ea6f189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->29 0x0000558b4d2c64a3 void mlir::MLIRContext::executeAction&lt;ApplyConversionAction&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->30 0x0000558b4d29cd68 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4189:1
#<!-- -->31 0x0000558b4d29cc1c mlir::applyPartialConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4198:10
#<!-- -->32 0x0000558b4d29ce05 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) /mlir/lib/Transforms/Utils/DialectConversion.cpp:4205:10
#<!-- -->33 0x0000558b4171d559 (anonymous namespace)::GpuToLLVMConversionPass::runOnOperation() /mlir/lib/Conversion/GPUCommon/GPUToLLVMConversion.cpp:575:11
#<!-- -->34 0x0000558b4d3f1fa4 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->35 0x0000558b4d3f1f45 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->36 0x0000558b3ea6f189 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->37 0x0000558b4d3f544b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->38 0x0000558b4d3e93ee mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->39 0x0000558b4d3e9b0a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->40 0x0000558b4d3ef3ed mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->41 0x0000558b4d3ee976 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->42 0x0000558b3eb53e7b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:17
#<!-- -->43 0x0000558b3eb53551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->44 0x0000558b3eb5320c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->45 0x0000558b3eb53116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->46 0x0000558b4d95b6fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->47 0x0000558b4d95aaba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->48 0x0000558b3eb4edc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->49 0x0000558b3eb4f1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->50 0x0000558b3eb4f398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->51 0x0000558b3ea1d8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->52 0x000071d0f0a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->53 0x000071d0f0a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->54 0x000071d0f0a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->55 0x0000558b3ea1d745 _start (./mlir-opt+0x481e745)
Segmentation fault
```
</details>


---

