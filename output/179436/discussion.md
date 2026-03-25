# [mlir-opt] Crash in remove-dead-values pass: DenseElementsAttr::reshape assertion via tensor.collapse_shape folding

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179436
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2026-02-05T20:17:50Z

## Body

mlir-opt version: 22.1.0-rc1

mlir-opt crashes during the **--remove-dead-values** pass when processing a **tensor.collapse_shape** operation that attempts to reshape a constant tensor into an incompatible shape (mismatched element count). The crash is triggered when the pass attempts to fold the operation, specifically hitting an assertion in DenseElementsAttr::reshape.
```mlir
module {
  func.func @main() {
    %0 = ml_program.token
    %1 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    smt.solver(%0, %1) : (!ml_program.token, tensor<3xindex>) -> () {
      ^bb0(%arg0: !ml_program.token, %arg1: tensor<3xindex>):
      smt.yield
    }
    %2 = tensor.collapse_shape %1 [] : tensor<3xindex> into tensor<index>
    return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() && "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #0 0x0000616e8b698e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000616e8b6955af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000616e8b6956fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007dbe09245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007dbe0929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007dbe0929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007dbe0929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007dbe0924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007dbe092288ff abort ./stdlib/abort.c:81:7
 #9 0x00007dbe0922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007dbe0923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000616e9320592f mlir::DenseElementsAttr::reshape(mlir::ShapedType) (llvmorg-22.1.0-install/bin/mlir-opt+0xa09c92f)
#12 0x0000616e92adeb5d mlir::OpFoldResult mlir::foldReshapeOp<mlir::tensor::CollapseShapeOp, mlir::tensor::ExpandShapeOp>(mlir::tensor::CollapseShapeOp, llvm::ArrayRef<mlir::Attribute>) TensorOps.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
mlir-opt version: 22.1.0-rc1

mlir-opt crashes during the **--remove-dead-values** pass when processing a **tensor.collapse_shape** operation that attempts to reshape a constant tensor into an incompatible shape (mismatched element count). The crash is triggered when the pass attempts to fold the operation, specifically hitting an assertion in DenseElementsAttr::reshape.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ml_program.token
    %1 = shape.const_shape [1, 2, 3] : tensor&lt;3xindex&gt;
    smt.solver(%0, %1) : (!ml_program.token, tensor&lt;3xindex&gt;) -&gt; () {
      ^bb0(%arg0: !ml_program.token, %arg1: tensor&lt;3xindex&gt;):
      smt.yield
    }
    %2 = tensor.collapse_shape %1 [] : tensor&lt;3xindex&gt; into tensor&lt;index&gt;
    return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() &amp;&amp; "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #<!-- -->0 0x0000616e8b698e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000616e8b6955af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000616e8b6956fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007dbe09245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007dbe0929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007dbe0929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007dbe0929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007dbe0924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007dbe092288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007dbe0922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007dbe0923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000616e9320592f mlir::DenseElementsAttr::reshape(mlir::ShapedType) (llvmorg-22.1.0-install/bin/mlir-opt+0xa09c92f)
#<!-- -->12 0x0000616e92adeb5d mlir::OpFoldResult mlir::foldReshapeOp&lt;mlir::tensor::CollapseShapeOp, mlir::tensor::ExpandShapeOp&gt;(mlir::tensor::CollapseShapeOp, llvm::ArrayRef&lt;mlir::Attribute&gt;) TensorOps.cpp:0:0
```
</details>


---

### Comment 2 - snarang181

In progress PR https://github.com/llvm/llvm-project/pull/179077 will fix this. 

---

