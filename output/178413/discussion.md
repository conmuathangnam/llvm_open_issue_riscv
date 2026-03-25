# mlir-opt crash in --eliminate-empty-tensors: bufferizesToMemoryRead not implemented for sparse_tensor.new

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/178413
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-13T02:23:25Z

## Body

Running mlir-opt --eliminate-empty-tensors on a valid MLIR module crashes with an UNREACHABLE due to bufferizesToMemoryRead not implemented.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = pdl_interp.create_attribute {value = dense<0> : tensor<1xi32>}
    %2 = "emitc.variable"() <{value = #emitc.opaque<"">}> : () -> !emitc.lvalue<i32>
    %3 = tensor.cast %0 : tensor<1xi32> to tensor<1xi32>
    %4 = sparse_tensor.new %3 : tensor<1xi32> to tensor<1xi32, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    return
  }
}
```
```
mlir-opt --eliminate-empty-tensors test.mlir
```
```
bufferizesToMemoryRead not implemented
UNREACHABLE executed at mlir/include/mlir/Dialect/Bufferization/IR/BufferizableOpInterface.h.inc:1166!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --eliminate-empty-tensors test.mlir
 #0 0x00005dd7a6c5de32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x00005dd7a6c5a5af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x00005dd7a6c5a6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000713076245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000071307629eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000071307629eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000071307629eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000071307624527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007130762288ff abort ./stdlib/abort.c:81:7
 #9 0x00005dd7a6c3d12e (mlir-opt+0x250f12e)
#10 0x00005dd7a7895262 (mlir-opt+0x3167262)
```

## Comments

### Comment 1 - EugeneZelenko

@compilerStudyer: Please elaborate.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
Running mlir-opt --eliminate-empty-tensors on a valid MLIR module crashes with an UNREACHABLE due to bufferizesToMemoryRead not implemented.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = pdl_interp.create_attribute {value = dense&lt;0&gt; : tensor&lt;1xi32&gt;}
    %2 = "emitc.variable"() &lt;{value = #emitc.opaque&lt;""&gt;}&gt; : () -&gt; !emitc.lvalue&lt;i32&gt;
    %3 = tensor.cast %0 : tensor&lt;1xi32&gt; to tensor&lt;1xi32&gt;
    %4 = sparse_tensor.new %3 : tensor&lt;1xi32&gt; to tensor&lt;1xi32, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt;
    return
  }
}
```
```
mlir-opt --eliminate-empty-tensors test.mlir
```
```
bufferizesToMemoryRead not implemented
UNREACHABLE executed at mlir/include/mlir/Dialect/Bufferization/IR/BufferizableOpInterface.h.inc:1166!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --eliminate-empty-tensors test.mlir
 #<!-- -->0 0x00005dd7a6c5de32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x00005dd7a6c5a5af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x00005dd7a6c5a6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000713076245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000071307629eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000071307629eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000071307629eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000071307624527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007130762288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00005dd7a6c3d12e (mlir-opt+0x250f12e)
#<!-- -->10 0x00005dd7a7895262 (mlir-opt+0x3167262)
```
</details>


---

