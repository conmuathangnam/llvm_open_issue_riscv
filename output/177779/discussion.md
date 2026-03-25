# mlir-opt --sparse-tensor-codegen crashes on uninitialized SparseTensorEncodingAttr

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/177779
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-20T18:32:39Z

## Body

# Description
Running mlir-opt --sparse-tensor-codegen on a MLIR module with sparse_tensor.convert operations crashes with an assertion in SparseTensorDialect.cpp

# Reproduce
mlir-opt version: 21.1.8
location: If the **sparse_tensor.convert** operation is commented out, the crash no longer occurs.

```mlir
module {
  spirv.EntryPoint "GLCompute" @main
  func.func @main() {
    %0 = gpu.num_subgroups : index
    %1 = tensor.generate %0 {
      ^bb0(%arg0: index):
      %cst = arith.constant 0.000000e+00 : f32
      tensor.yield %cst : f32
    } : tensor<?xf32>
    %2 = sparse_tensor.convert %1 : tensor<?xf32> to tensor<?xf32, #spirv.storage_class<StorageBuffer>>
    return
  }
}
```
```
mlir-opt --sparse-tensor-codegen test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/Dialect/SparseTensor/IR/SparseTensorDialect.cpp:460: bool mlir::sparse_tensor::SparseTensorEncodingAttr::isSlice() const: Assertion `getImpl() && "Uninitialized SparseTensorEncodingAttr"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --sparse-tensor-codegen /mnt/sdd1/sbw/mlir/data/genProgram_0123_filerted/genProgram_0123_valid/232137_47e4_5.mlir
 #0 0x000056f26ca3a612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x000056f26ca36d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x000056f26ca36ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007e19cf445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007e19cf49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007e19cf49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007e19cf49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007e19cf44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007e19cf4288ff abort ./stdlib/abort.c:81:7
 #9 0x00007e19cf42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007e19cf43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000056f26ee68f95 (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x44cff95)
#12 0x000056f26ee8055b (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x44e755b)
#13 0x000056f26f014fe9 (anonymous namespace)::SparseConvertConverter::matchAndRewrite(mlir::sparse_tensor::ConvertOp, mlir::sparse_tensor::ConvertOpGenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) const SparseTensorCodegen.cpp:0:0
#14 0x000056f26f00645c mlir::OpConversionPattern<mlir::sparse_tensor::ConvertOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x466d45c)
#15 0x000056f2702970ab mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58fe0ab)
#16 0x000056f27335c08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#17 0x000056f27029a6fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&) DialectConversion.cpp:0:0
#18 0x000056f27029acae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#19 0x000056f27029f57c mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#20 0x000056f2702a0bba applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#21 0x000056f2702a0cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#22 0x000056f26ef40068 (anonymous namespace)::SparseTensorCodegenPass::runOnOperation() SparseTensorPasses.cpp:0:0
#23 0x000056f2701ef07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#24 0x000056f2701ef4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#25 0x000056f2701f0523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#26 0x000056f2701e00a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#27 0x000056f2701e09c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#28 0x000056f2701e0b8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x000056f2702f624e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#30 0x000056f2701d9080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#31 0x000056f2701e0cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#32 0x000056f2701e11ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#33 0x000056f26c952577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#34 0x00007e19cf42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x00007e19cf42a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x00007e19cf42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x000056f26ca0b665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
# Description
Running mlir-opt --sparse-tensor-codegen on a MLIR module with sparse_tensor.convert operations crashes with an assertion in SparseTensorDialect.cpp

# Reproduce
mlir-opt version: 21.1.8
location: If the **sparse_tensor.convert** operation is commented out, the crash no longer occurs.

```
module {
  spirv.EntryPoint "GLCompute" @<!-- -->main
  func.func @<!-- -->main() {
    %0 = gpu.num_subgroups : index
    %1 = tensor.generate %0 {
      ^bb0(%arg0: index):
      %cst = arith.constant 0.000000e+00 : f32
      tensor.yield %cst : f32
    } : tensor&lt;?xf32&gt;
    %2 = sparse_tensor.convert %1 : tensor&lt;?xf32&gt; to tensor&lt;?xf32, #spirv.storage_class&lt;StorageBuffer&gt;&gt;
    return
  }
}
```
```
mlir-opt --sparse-tensor-codegen test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/Dialect/SparseTensor/IR/SparseTensorDialect.cpp:460: bool mlir::sparse_tensor::SparseTensorEncodingAttr::isSlice() const: Assertion `getImpl() &amp;&amp; "Uninitialized SparseTensorEncodingAttr"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --sparse-tensor-codegen /mnt/sdd1/sbw/mlir/data/genProgram_0123_filerted/genProgram_0123_valid/232137_47e4_5.mlir
 #<!-- -->0 0x000056f26ca3a612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x000056f26ca36d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x000056f26ca36ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007e19cf445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007e19cf49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007e19cf49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007e19cf49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007e19cf44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007e19cf4288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007e19cf42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007e19cf43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000056f26ee68f95 (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x44cff95)
#<!-- -->12 0x000056f26ee8055b (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x44e755b)
#<!-- -->13 0x000056f26f014fe9 (anonymous namespace)::SparseConvertConverter::matchAndRewrite(mlir::sparse_tensor::ConvertOp, mlir::sparse_tensor::ConvertOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) const SparseTensorCodegen.cpp:0:0
#<!-- -->14 0x000056f26f00645c mlir::OpConversionPattern&lt;mlir::sparse_tensor::ConvertOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x466d45c)
#<!-- -->15 0x000056f2702970ab mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58fe0ab)
#<!-- -->16 0x000056f27335c08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#<!-- -->17 0x000056f27029a6fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) DialectConversion.cpp:0:0
#<!-- -->18 0x000056f27029acae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&amp;, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#<!-- -->19 0x000056f27029f57c mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#<!-- -->20 0x000056f2702a0bba applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->21 0x000056f2702a0cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#<!-- -->22 0x000056f26ef40068 (anonymous namespace)::SparseTensorCodegenPass::runOnOperation() SparseTensorPasses.cpp:0:0
#<!-- -->23 0x000056f2701ef07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->24 0x000056f2701ef4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->25 0x000056f2701f0523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->26 0x000056f2701e00a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x000056f2701e09c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->28 0x000056f2701e0b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x000056f2702f624e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->30 0x000056f2701d9080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->31 0x000056f2701e0cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->32 0x000056f2701e11ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->33 0x000056f26c952577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->34 0x00007e19cf42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x00007e19cf42a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007e19cf42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x000056f26ca0b665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

### Comment 2 - abhamra

This would be my first time contributing to LLVM, but I'd like to take a look at this issue if possible, so I can get familiar with contributing to MLIR. If I don't have any time in the future to work on this issue, I'll inform here ASAP. I'm also happy to defer to others with more experience.

I think because of some set-up stuff, I only have either mlir-opt 21.1.1 or 23git, but the issue is reproduced on both versions.

---

### Comment 3 - abhamra

Based on some preliminary debugging, it seems like the minimal reproducing code causes problems because `#spirv.storage_class<StorageBuffer>` is an invalid encoding for a tensor. When I examine the encoding (both with GDB and print debugging), it turns out to be null. That being said, after trying the following change:
```mlir
#SparseVector = #sparse_tensor.encoding<{
  map = (i) -> (i : compressed)
}>

module {
  spirv.EntryPoint "GLCompute" @main
  func.func @main() {
    %0 = gpu.num_subgroups : index
    %1 = tensor.generate %0 {
      ^bb0(%arg0: index):
      %cst = arith.constant 0.000000e+00 : f32
      tensor.yield %cst : f32
    } : tensor<?xf32>
    %2 = sparse_tensor.convert %1 : tensor<?xf32> to tensor<?xf32, #SparseVector>
    return
  }
}
```
to see if it was reproducible with a "valid" encoding, it's still null, just in a different spot.

Crucially, in the `SparseConvertConverter`'s `matchAndRewrite`, `encDst` is non-null in the second case, whereas it is null in the first, leading to my above conclusion about `#spirv.storage_class<StorageBuffer>`. I'll still be digging into why a "valid" encoding fails, but if anyone has helpful ideas please lmk.

---

### Comment 4 - abhamra

I had a question about `sparse_tensor.convert`, namely, does it require a fully specified source tensor type (static size)? The examples shown in the documentation seem to think so, and I wanted to make sure I didn't miss anything: https://mlir.llvm.org/docs/Dialects/SparseTensorOps/#sparse_tensorconvert-sparse_tensorconvertop

I guess I wasn't sure what the best path forward was when either encSrc or encDst were null; either I'd add a failure check to see if at least one was null or it's a broader problem, but adding the failure check seems irresposible because it should be a valid dense to sparse conversion (dense doesn't need an encoding?)

Any help with this would be great; this is currently a minor blocker to my understanding of next steps. Thank you!

---

### Comment 5 - abhamra

@Men-cotton hopefully pinging you is alright; let me know if there are other avenues to explore first. After exploring this further, I think that the specific case from the above bug report may just be a case of not calling certain passes prior to `sparse-tensor-codegen`, and having an incorrect `SparseTensorEncodingAttr`. However, I may need a sanity-check w.r.t semantics of `sparse_tensor.convert`. For example, I was exploring the following test case:
```mlir
#SparseVector = #sparse_tensor.encoding<{
  map = (d0) -> (d0 : compressed)
}>

module {
  func.func @main() -> tensor<8xf32, #SparseVector> {
    // Create a dense tensor with some values
    %dense = arith.constant dense<[1.0, 0.0, 0.0, 2.0, 0.0, 3.0, 0.0, 0.0]> : tensor<8xf32>
    
    // Convert dense to sparse
    %sparse = sparse_tensor.convert %dense : tensor<8xf32> to tensor<8xf32, #SparseVector>
    
    return %sparse : tensor<8xf32, #SparseVector>
  }
}
```
And even running with the following passes, it still fails:
```
asb@pc ~/p/mlir-testing > ~/personal/llvm-project/build/bin/mlir-opt test_dense.mlir \
                                --stage-sparse-ops \
                                --lower-sparse-ops-to-foreach \
                                --sparse-tensor-conversion \
                                --canonicalize \
                                --cse \
                                --sparse-tensor-codegen
```

This is odd to me because the FileCheck test [`convert_dense2sparse.mlir`](https://github.com/llvm/llvm-project/blob/main/mlir/test/Dialect/SparseTensor/convert_dense2sparse.mlir). Notably, this file's run command specifies using the flags `--stage-sparse-ops --lower-sparse-ops-to-foreach --canonicalize --cse`, and *not* `sparse-tensor-codegen`. 

With the above command, it says that `sparse-tensor-conversion` failed to legalize `sparse_tensor.foreach`; is this the actual underlying bug, or am I missing some passes that would fix this?

Thank you!

---

### Comment 6 - Men-cotton

@abhamra 
Try inserting `--lower-sparse-foreach-to-scf` between `--lower-sparse-ops-to-foreach` and `--sparse-tensor-conversion`.

I believe the conversion pass fails because it does not support `sparse_tensor.foreach`, so that op must be lowered to SCF first (as seen in `mlir/test/Dialect/SparseTensor/conversion.mlir`).

---

### Comment 7 - abhamra

@Men-cotton that seemed to do the trick, thank you! With this in mind, should this bug report be considered an error on the user's end (w.r.t not using the correct passes in sequence)? For reference, the full command I used to get a seemingly correct output was:
```
mlir-opt test.mlir \
  --mlir-print-ir-before-all \
  --stage-sparse-ops \
  --lower-sparse-ops-to-foreach \
  --lower-sparse-foreach-to-scf \
  --sparse-reinterpret-map \
  --sparse-tensor-conversion \
  --canonicalize \
  --cse \
  --sparse-tensor-codegen
```
The output is not meaningful for the sample provided in the original report (since nothing is returned, it is likely just DCE'd and we are left with an empty `main` function), but with the dense example above, the pass works fine.

---

### Comment 8 - Men-cotton

@abhamra 
If the sequence caused a crash, it is still an issue (because the compiler should fail gracefully).
If you have the bandwidth, feel free to submit a separate PR to address the crash.

Otherwise, no further action is needed—please just focus on the original issue.

---

### Comment 9 - abhamra

@Men-cotton that makes sense, I will have a simple PR ideally up soon. My plan is to enable graceful failing by failing to legalize if either the source or destination tensors have a null sparse tensor encoding attribute, and I'll see if I can find (or make) a good spot for a legalization test.

---

