# [mlir-opt] Parser/Verifier crash in linalg.batch_reduce_matmul with !amx.tile types

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179680
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-02T14:21:55Z

## Body

The crash is a Parser/Verifier failure.
- Linalg named operations (like batch_reduce_matmul) rely on an automated regionBuilder to define their scalar arithmetic.
- The regionBuilder is designed for standard ShapedType (tensors/memrefs) with numeric element types.
- When the parser encounters !amx.tile, it fails to emit a clean diagnostic error. Instead, the logic proceeds to buildBinaryFn, which cannot handle non-numeric types and hits a hard llvm_unreachable.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = amx.tile_zero : !amx.tile<16x16xi32>
    %2 = linalg.batch_reduce_matmul ins(%0, %0 : tensor<1xi32>, tensor<1xi32>) outs(%1 : !amx.tile<16x16xi32>) -> !amx.tile<16x16xi32>
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
<unknown>:0: warning: could not cast operand of type 'i32' to '!amx.tile<16x16xi32>'
<unknown>:0: warning: could not cast operand of type 'i32' to '!amx.tile<16x16xi32>'
unsupported non numeric type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:514!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.batch_reduce_matmul'
 #0 0x000060a617e30e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000060a617e2d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000060a617e2d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007ac23ac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007ac23ac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007ac23ac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007ac23ac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007ac23ac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007ac23ac288ff abort ./stdlib/abort.c:81:7
 #9 0x000060a617e1012e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x000060a61ddd3e74 (anonymous namespace)::RegionBuilderHelper::buildBinaryFn(mlir::linalg::BinaryFn, mlir::Value, mlir::Value, llvm::function_ref<mlir::InFlightDiagnostic ()>) LinalgOps.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
The crash is a Parser/Verifier failure.
- Linalg named operations (like batch_reduce_matmul) rely on an automated regionBuilder to define their scalar arithmetic.
- The regionBuilder is designed for standard ShapedType (tensors/memrefs) with numeric element types.
- When the parser encounters !amx.tile, it fails to emit a clean diagnostic error. Instead, the logic proceeds to buildBinaryFn, which cannot handle non-numeric types and hits a hard llvm_unreachable.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = amx.tile_zero : !amx.tile&lt;16x16xi32&gt;
    %2 = linalg.batch_reduce_matmul ins(%0, %0 : tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) outs(%1 : !amx.tile&lt;16x16xi32&gt;) -&gt; !amx.tile&lt;16x16xi32&gt;
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
&lt;unknown&gt;:0: warning: could not cast operand of type 'i32' to '!amx.tile&lt;16x16xi32&gt;'
&lt;unknown&gt;:0: warning: could not cast operand of type 'i32' to '!amx.tile&lt;16x16xi32&gt;'
unsupported non numeric type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:514!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.batch_reduce_matmul'
 #<!-- -->0 0x000060a617e30e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000060a617e2d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000060a617e2d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007ac23ac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007ac23ac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007ac23ac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007ac23ac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007ac23ac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007ac23ac288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000060a617e1012e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x000060a61ddd3e74 (anonymous namespace)::RegionBuilderHelper::buildBinaryFn(mlir::linalg::BinaryFn, mlir::Value, mlir::Value, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) LinalgOps.cpp:0:0
```
</details>


---

### Comment 2 - guiolidc

Hi, I'll work on this issue.

---

### Comment 3 - joker-eph

Seems like it is fixed at HEAD, if you can still reproduce please reopen.



---

### Comment 4 - Men-cotton

I also confirmed no crash at HEAD. 
I believe #181616 resolves this.

---

