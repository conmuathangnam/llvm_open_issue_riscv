# [mlir-opt] Crash in scf.while canonicalization: DenseElementsAttr::reshape assertion via tensor.collapse_shape

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179402
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2026-02-07T09:33:21Z

## Body

mlir-opt crashes during the --**canonicalize** pass when an scf.while loop uses a value produced by an invalid **tensor.collapse_shape operation**. The crash occurs because the **scf** canonicalization logic attempts to propagate constants through the loop, triggering a reshape of a DenseElementsAttr with mismatched element counts.

```mlir
module {
  func.func @main() {
    %0 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    %1 = shape.const_size 0
    %2 = tensor.collapse_shape %0 [] : tensor<3xindex> into tensor<index>
    %3 = scf.while (%arg0 = %2) : (tensor<index>) -> tensor<index> {
      %4 = arith.constant true
      scf.condition(%4) %arg0 : tensor<index>
    } do {
      ^bb0(%arg0: tensor<index>):
      scf.yield %arg0 : tensor<index>
    }
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() && "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize test.mlir
 #0 0x00005a56ca2b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005a56ca2b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005a56ca2b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000077b038845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000077b03889eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000077b03889eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000077b03889eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000077b03884527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000077b0388288ff abort ./stdlib/abort.c:81:7
 #9 0x000077b03882881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000077b03883b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
mlir-opt crashes during the --**canonicalize** pass when an scf.while loop uses a value produced by an invalid **tensor.collapse_shape operation**. The crash occurs because the **scf** canonicalization logic attempts to propagate constants through the loop, triggering a reshape of a DenseElementsAttr with mismatched element counts.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = shape.const_shape [1, 2, 3] : tensor&lt;3xindex&gt;
    %1 = shape.const_size 0
    %2 = tensor.collapse_shape %0 [] : tensor&lt;3xindex&gt; into tensor&lt;index&gt;
    %3 = scf.while (%arg0 = %2) : (tensor&lt;index&gt;) -&gt; tensor&lt;index&gt; {
      %4 = arith.constant true
      scf.condition(%4) %arg0 : tensor&lt;index&gt;
    } do {
      ^bb0(%arg0: tensor&lt;index&gt;):
      scf.yield %arg0 : tensor&lt;index&gt;
    }
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1246: mlir::DenseElementsAttr mlir::DenseElementsAttr::reshape(mlir::ShapedType): Assertion `newType.getNumElements() == curType.getNumElements() &amp;&amp; "expected the same number of elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize test.mlir
 #<!-- -->0 0x00005a56ca2b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005a56ca2b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005a56ca2b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000077b038845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000077b03889eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000077b03889eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000077b03889eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000077b03884527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000077b0388288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000077b03882881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000077b03883b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

### Comment 2 - CoTinker

Fixed by https://github.com/llvm/llvm-project/pull/179077.

---

