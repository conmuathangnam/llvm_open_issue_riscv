# mlir-opt crash in --tosa-to-scf with unranked tensors in TOSA ops

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/177966
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-27T17:27:42Z

## Body

mlir-opt crashes when running --tosa-to-scf on TOSA operations using unranked tensors (tensor<*xi32>).
The pass assumes RankedTensorType and unconditionally casts tensor types, triggering an assertion failure when unranked tensors are present.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = mpi.init : !mpi.retval
    %2 = tensor.cast %0 : tensor<1xi32> to tensor<*xi32>
    %3 = tosa.gather %2, %2 : (tensor<*xi32>, tensor<*xi32>) -> tensor<*xi32>
    %4 = tosa.avg_pool2d %3, %0, %0 {acc_type = i32, kernel = array<i64: 2, 2>, pad = array<i64: 0, 1, 0, 1>, stride = array<i64: 1, 1>} : (tensor<*xi32>, tensor<1xi32>, tensor<1xi32>) -> tensor<*xi32>
    %5 = tosa.max_pool2d %3 {kernel = array<i64: 2, 2>, pad = array<i64: 0, 1, 0, 1>, stride = array<i64: 1, 1>} : (tensor<*xi32>) -> tensor<*xi32>
    %6 = tosa.conv3d %4, %5, %0, %0, %0 {acc_type = i32, dilation = array<i64: 1, 1, 1>, pad = array<i64: 0, 0, 0, 0, 0, 0>, stride = array<i64: 1, 1, 1>} : (tensor<*xi32>, tensor<*xi32>, tensor<1xi32>, tensor<1xi32>, tensor<1xi32>) -> tensor<*xi32>
    %7 = tosa.scatter %6, %2, %2 : (tensor<*xi32>, tensor<*xi32>, tensor<*xi32>) -> tensor<*xi32>
    return
  }
}
```


```
mlir-opt --tosa-to-scf test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::RankedTensorType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --tosa-to-scf test.mlir
 #0 0x000057ea04156e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x000057ea041535af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x000057ea041536fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000767291845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000076729189eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000076729189eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (mlirBug)

<details>
mlir-opt crashes when running --tosa-to-scf on TOSA operations using unranked tensors (tensor&lt;*xi32&gt;).
The pass assumes RankedTensorType and unconditionally casts tensor types, triggering an assertion failure when unranked tensors are present.

```
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = mpi.init : !mpi.retval
    %2 = tensor.cast %0 : tensor&lt;1xi32&gt; to tensor&lt;*xi32&gt;
    %3 = tosa.gather %2, %2 : (tensor&lt;*xi32&gt;, tensor&lt;*xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    %4 = tosa.avg_pool2d %3, %0, %0 {acc_type = i32, kernel = array&lt;i64: 2, 2&gt;, pad = array&lt;i64: 0, 1, 0, 1&gt;, stride = array&lt;i64: 1, 1&gt;} : (tensor&lt;*xi32&gt;, tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    %5 = tosa.max_pool2d %3 {kernel = array&lt;i64: 2, 2&gt;, pad = array&lt;i64: 0, 1, 0, 1&gt;, stride = array&lt;i64: 1, 1&gt;} : (tensor&lt;*xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    %6 = tosa.conv3d %4, %5, %0, %0, %0 {acc_type = i32, dilation = array&lt;i64: 1, 1, 1&gt;, pad = array&lt;i64: 0, 0, 0, 0, 0, 0&gt;, stride = array&lt;i64: 1, 1, 1&gt;} : (tensor&lt;*xi32&gt;, tensor&lt;*xi32&gt;, tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    %7 = tosa.scatter %6, %2, %2 : (tensor&lt;*xi32&gt;, tensor&lt;*xi32&gt;, tensor&lt;*xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    return
  }
}
```


```
mlir-opt --tosa-to-scf test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::RankedTensorType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --tosa-to-scf test.mlir
 #<!-- -->0 0x000057ea04156e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x000057ea041535af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x000057ea041536fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000767291845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000076729189eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000076729189eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
```
</details>


---

