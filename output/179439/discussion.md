# [mlir-opt] Crash in --inline pass: DenseElementsAttr::reshape assertion triggered by tensor.collapse_shape on folded shape.broadcast

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179439
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2026-02-07T09:34:24Z

## Body

**mlir-opt** crashes during the **--inline** pass (which triggers canonicalization/folding on newly inlined code) when a **tensor.collapse_shape** operation attempts to fold a constant resulting from a **shape.broadcast**. The assertion in DenseElementsAttr::reshape fails because the operation attempts to collapse a multi-element tensor into a 0-D tensor (1 element), violating the requirement for matching element counts.

```mlir
module {
  func.func @main() {
    %0 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    %1 = irdl.c_pred "::llvm::isa<::mlir::ShapeType>($_self)"
    %2 = shape.broadcast %0, %0, %0 : tensor<3xindex>, tensor<3xindex>, tensor<3xindex> -> tensor<3xindex>
    %3 = tensor.collapse_shape %2 [] : tensor<3xindex> into tensor<index>
    %4 = shape.with_shape %3, %0 : tensor<index>, tensor<3xindex>
    return
  }
}
```
```
mlir-opt --inline temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() && "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #0 0x00005d2b39052e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005d2b3904f5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005d2b3904f6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007cdbf9645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007cdbf969eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007cdbf969eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007cdbf969eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007cdbf964527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007cdbf96288ff abort ./stdlib/abort.c:81:7
 #9 0x00007cdbf962881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007cdbf963b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005d2b40bbf92f mlir::DenseElementsAttr::reshape(mlir::ShapedType) (llvmorg-22.1.0-install/bin/mlir-opt+0xa09c92f)
#12 0x00005d2b40498b5d mlir::OpFoldResult mlir::foldReshapeOp<mlir::tensor::CollapseShapeOp, mlir::tensor::ExpandShapeOp>(mlir::tensor::CollapseShapeOp, llvm::ArrayRef<mlir::Attribute>) TensorOps.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
**mlir-opt** crashes during the **--inline** pass (which triggers canonicalization/folding on newly inlined code) when a **tensor.collapse_shape** operation attempts to fold a constant resulting from a **shape.broadcast**. The assertion in DenseElementsAttr::reshape fails because the operation attempts to collapse a multi-element tensor into a 0-D tensor (1 element), violating the requirement for matching element counts.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = shape.const_shape [1, 2, 3] : tensor&lt;3xindex&gt;
    %1 = irdl.c_pred "::llvm::isa&lt;::mlir::ShapeType&gt;($_self)"
    %2 = shape.broadcast %0, %0, %0 : tensor&lt;3xindex&gt;, tensor&lt;3xindex&gt;, tensor&lt;3xindex&gt; -&gt; tensor&lt;3xindex&gt;
    %3 = tensor.collapse_shape %2 [] : tensor&lt;3xindex&gt; into tensor&lt;index&gt;
    %4 = shape.with_shape %3, %0 : tensor&lt;index&gt;, tensor&lt;3xindex&gt;
    return
  }
}
```
```
mlir-opt --inline temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() &amp;&amp; "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #<!-- -->0 0x00005d2b39052e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005d2b3904f5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005d2b3904f6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007cdbf9645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007cdbf969eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007cdbf969eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007cdbf969eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007cdbf964527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007cdbf96288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007cdbf962881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007cdbf963b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005d2b40bbf92f mlir::DenseElementsAttr::reshape(mlir::ShapedType) (llvmorg-22.1.0-install/bin/mlir-opt+0xa09c92f)
#<!-- -->12 0x00005d2b40498b5d mlir::OpFoldResult mlir::foldReshapeOp&lt;mlir::tensor::CollapseShapeOp, mlir::tensor::ExpandShapeOp&gt;(mlir::tensor::CollapseShapeOp, llvm::ArrayRef&lt;mlir::Attribute&gt;) TensorOps.cpp:0:0
```
</details>


---

### Comment 2 - keremsahn

I am working on that, you can assignee me 

---

### Comment 3 - CoTinker

Actually, I believe the `tensor.collapse_shape` op is invalid.

---

### Comment 4 - keremsahn

> Actually, I believe the `tensor.collapse_shape` op is invalid.

I thought adding a protecting layer to foldReshapeOp would fix the problem, I have not done tests yet with this solutions. Can you elaborate more why should i add the protection to the tensor.collapse_shape instead of foldReshapeOp

---

### Comment 5 - CoTinker

It fixed by https://github.com/llvm/llvm-project/pull/179077.

---

