# mlir-opt crash: Segmentation fault in TosaInferShapes with Sparse Tensor types in tosa.while_loop

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/181449
**Status:** Closed
**Labels:** crash, mlir:tosa
**Closed Date:** 2026-03-02T13:59:21Z

## Body

**Bug Op**: tosa.while_loop

**Bug Pass**: --tosa-infer-shapes

**Detailed Description**:
The TosaInferShapes pass crashes when processing a tosa.while_loop that involves sparse_tensor encodings. The crash is triggered when the shape inference logic attempts to reconcile types between the loop-carried dependencies and the region yields, especially when there is a mismatch between sparse and dense tensor types that the pass fails to handle safely.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = gpu.grid_dim  x
    %2 = sparse_tensor.convert %0 : tensor<1xi32> to tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    %3 = index.sizeof
    %4 = tosa.while_loop (%arg0 = %2) : (tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>) -> tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>> {
      %5 = "tosa.const"() <{values = dense<3> : tensor<1xi32>}> : () -> tensor<1xi32>
      %6 = sparse_tensor.convert %arg0 : tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>> to tensor<1xi32>
      %7 = tosa.greater_equal %5, %6 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi1>
      tosa.yield %7 : tensor<1xi1>
    } do {
      ^bb0(%arg0: tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>):
      %5 = "tosa.const"() <{values = dense<1> : tensor<1xi32>}> : () -> tensor<1xi32>
      tosa.yield %5 : tensor<1xi32>
    }
    return
  }
}
```
```
mlir-opt --tosa-infer-shapes temp.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --tosa-infer-shapes temp.mlir
 #0 0x000058c793800e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000058c7937fd5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000058c7937fd6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000752469645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000058c797152f0e (anonymous namespace)::TosaInferShapes::runOnOperation() TosaInferShapes.cpp:0:0
 #5 0x000058c79b0a0756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
 #6 0x000058c79b0a0a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
 #7 0x000058c79b0a2673 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const Pass.cpp:0:0
 #8 0x000058c79b09f72d mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dce72d)
 #9 0x000058c79b0a05f0 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf5f0)
#10 0x000058c79b0a0a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#11 0x000058c79b0a11e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#12 0x000058c79b0a2400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#13 0x000058c7938c7767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#14 0x000058c7938c84ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#15 0x000058c79b470731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#16 0x000058c7938bf8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#17 0x000058c7938c8b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#18 0x000058c7938c8da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#19 0x000058c7936fa023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#20 0x000075246962a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#21 0x000075246962a28b call_init ./csu/../csu/libc-start.c:128:20
#22 0x000075246962a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#23 0x000058c7937df405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Segmentation fault (core dumped)
```

