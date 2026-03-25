# [mlir-opt] Crash in --remove-dead-values due to tensor.collapse_shape folding with mismatched element counts

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179670
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-02-04T14:33:47Z

## Body

mlir-opt crashes with an assertion failure in DenseElementsAttr::reshape when running the --remove-dead-values pass. The crash occurs because **tensor.collapse_shape** attempts to fold a constant tensor into a shape with a different number of elements (in this case, collapsing a 3-element tensor<3xindex> into a 1-element tensor<index>).
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
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() && "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #0 0x000063da45d9ee32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000063da45d9b5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000063da45d9b6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000747cc9245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000747cc929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000747cc929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000747cc929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000747cc924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000747cc92288ff abort ./stdlib/abort.c:81:7
 #9 0x0000747cc922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000747cc923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000063da4d90b92f mlir::DenseElementsAttr::reshape(mlir::ShapedType) (llvmorg-22.1.0-install/bin/mlir-opt+0xa09c92f)
#12 0x000063da4d1e4b5d mlir::OpFoldResult mlir::foldReshapeOp<mlir::tensor::CollapseShapeOp, mlir::tensor::ExpandShapeOp>(mlir::tensor::CollapseShapeOp, llvm::ArrayRef<mlir::Attribute>) TensorOps.cpp:0:0
```

## Comments

### Comment 1 - EugeneZelenko

@Subway2023: Could you please elaborate?

---

