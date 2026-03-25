# mlir-opt crash: Unknown primary type in sparse_tensor.new during --sparse-tensor-conversion

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180310
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-03T11:42:15Z

## Body

**Bug Op:** sparse_tensor.new 

**Bug Pass:** --sparse-tensor-conversion

**Detailed Description:** The mlir-opt tool hits a llvm_unreachable ("Unknown primary type") during the sparse tensor conversion pass. The crash occurs when sparse_tensor.new attempts to lower a tensor with an index element type. The utility function primaryTypeEncoding in the SparseTensor dialect does not recognize index as a valid primary type for encoding, leading to the crash during reader generation (genReader).

```mlir
module {
  func.func @main() {
    %0 = gpu.num_subgroups : index
    %1 = smt.constant true
    %2 = sparse_tensor.new %0 : index to tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    return
  }
}
```
```
mlir-opt --sparse-tensor-conversion temp.mlir
```
```
Unknown primary type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/SparseTensor/Transforms/Utils/CodegenUtils.cpp:126!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --sparse-tensor-conversion temp.mlir
 #0 0x0000605b9c185e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000605b9c1825af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000605b9c1826fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007a3cbe845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007a3cbe89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007a3cbe89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007a3cbe89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007a3cbe84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007a3cbe8288ff abort ./stdlib/abort.c:81:7
 #9 0x0000605b9c16512e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x0000605b9ce10d96 mlir::sparse_tensor::primaryTypeEncoding(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0x31bad96)
#11 0x0000605b9ce16e30 mlir::sparse_tensor::genReader(mlir::OpBuilder&, mlir::Location, mlir::sparse_tensor::SparseTensorType, mlir::Value, llvm::SmallVectorImpl<mlir::Value>&, mlir::Value&) (llvmorg-22.1.0-install/bin/mlir-opt+0x31c0e30)
#12 0x0000605b9ce8eb3c (anonymous namespace)::SparseTensorNewConverter::matchAndRewrite(mlir::sparse_tensor::NewOp, mlir::sparse_tensor::NewOpAdaptor, mlir::ConversionPatternRewriter&) const SparseTensorConversion.cpp:0:0
#13 0x0000605b9ce83352 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<mlir::sparse_tensor::NewOp>, mlir::sparse_tensor::NewOp>(mlir::OpConversionPattern<mlir::sparse_tensor::NewOp> const&, mlir::sparse_tensor::NewOp, mlir::sparse_tensor::NewOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (llvmorg-22.1.0-install/bin/mlir-opt+0x322d352)
#14 0x0000605b9ce83474 mlir::OpConversionPattern<mlir::sparse_tensor::NewOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x322d474)
#15 0x0000605ba396332e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9d0d32e)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op:** sparse_tensor.new 

**Bug Pass:** --sparse-tensor-conversion

**Detailed Description:** The mlir-opt tool hits a llvm_unreachable ("Unknown primary type") during the sparse tensor conversion pass. The crash occurs when sparse_tensor.new attempts to lower a tensor with an index element type. The utility function primaryTypeEncoding in the SparseTensor dialect does not recognize index as a valid primary type for encoding, leading to the crash during reader generation (genReader).

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.num_subgroups : index
    %1 = smt.constant true
    %2 = sparse_tensor.new %0 : index to tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt;
    return
  }
}
```
```
mlir-opt --sparse-tensor-conversion temp.mlir
```
```
Unknown primary type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/SparseTensor/Transforms/Utils/CodegenUtils.cpp:126!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --sparse-tensor-conversion temp.mlir
 #<!-- -->0 0x0000605b9c185e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000605b9c1825af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000605b9c1826fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007a3cbe845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007a3cbe89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007a3cbe89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007a3cbe89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007a3cbe84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007a3cbe8288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000605b9c16512e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x0000605b9ce10d96 mlir::sparse_tensor::primaryTypeEncoding(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0x31bad96)
#<!-- -->11 0x0000605b9ce16e30 mlir::sparse_tensor::genReader(mlir::OpBuilder&amp;, mlir::Location, mlir::sparse_tensor::SparseTensorType, mlir::Value, llvm::SmallVectorImpl&lt;mlir::Value&gt;&amp;, mlir::Value&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x31c0e30)
#<!-- -->12 0x0000605b9ce8eb3c (anonymous namespace)::SparseTensorNewConverter::matchAndRewrite(mlir::sparse_tensor::NewOp, mlir::sparse_tensor::NewOpAdaptor, mlir::ConversionPatternRewriter&amp;) const SparseTensorConversion.cpp:0:0
#<!-- -->13 0x0000605b9ce83352 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;mlir::sparse_tensor::NewOp&gt;, mlir::sparse_tensor::NewOp&gt;(mlir::OpConversionPattern&lt;mlir::sparse_tensor::NewOp&gt; const&amp;, mlir::sparse_tensor::NewOp, mlir::sparse_tensor::NewOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x322d352)
#<!-- -->14 0x0000605b9ce83474 mlir::OpConversionPattern&lt;mlir::sparse_tensor::NewOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x322d474)
#<!-- -->15 0x0000605ba396332e mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9d0d32e)
```

</details>


---

