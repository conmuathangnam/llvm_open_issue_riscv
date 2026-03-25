# mlir-opt crash: UNREACHABLE executed in SelectOp::regionBuilder with index type

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179046
**Status:** Closed
**Labels:** mlir:linalg, crash
**Closed Date:** 2026-03-09T11:50:14Z

## Body

mlir-opt version: 22.1.0
mlir-opt crashes with an UNREACHABLE error when parsing a **linalg.select** operation that uses index type operands. 
The **linalg.select** region builder expects numeric types (Integer or Float) but does not account for the index type, which is commonly used for pointer-as-index or offset calculations.
```mlir
module {
  memref.global "private" @global_var : memref<10xf32>
  func.func @main() {
    %0 = gpu.lane_id
    %1 = memref.get_global @global_var : memref<10xf32>
    %2 = pdl_interp.create_type i32
    %3 = memref.extract_aligned_pointer_as_index %1 : memref<10xf32> -> index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %4 = tensor.empty() : tensor<1xi64>
    %5 = linalg.select ins(%3, %c0, %c1 : index, index, index) outs(%4 : tensor<1xi64>) -> tensor<1xi64>
    return
  }
}
```
```
mlir-opt test.mlir
```
```
unsupported non numeric type
UNREACHABLE executed at /llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:624!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.select'
 #0 0x000062755d164e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000062755d1615af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000062755d1616fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007ccf8ce45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007ccf8ce9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007ccf8ce9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007ccf8ce9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007ccf8ce4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007ccf8ce288ff abort ./stdlib/abort.c:81:7
 #9 0x000062755d14412e (/llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x00006275630e4337 mlir::linalg::SelectOp::regionBuilder(mlir::ImplicitLocOpBuilder&, mlir::Block&, llvm::ArrayRef<mlir::NamedAttribute>, llvm::function_ref<mlir::InFlightDiagnostic ()>) (/llvmorg-22.1.0-install/bin/mlir-opt+0x84af337)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
mlir-opt version: 22.1.0
mlir-opt crashes with an UNREACHABLE error when parsing a **linalg.select** operation that uses index type operands. 
The **linalg.select** region builder expects numeric types (Integer or Float) but does not account for the index type, which is commonly used for pointer-as-index or offset calculations.
```mlir
module {
  memref.global "private" @<!-- -->global_var : memref&lt;10xf32&gt;
  func.func @<!-- -->main() {
    %0 = gpu.lane_id
    %1 = memref.get_global @<!-- -->global_var : memref&lt;10xf32&gt;
    %2 = pdl_interp.create_type i32
    %3 = memref.extract_aligned_pointer_as_index %1 : memref&lt;10xf32&gt; -&gt; index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %4 = tensor.empty() : tensor&lt;1xi64&gt;
    %5 = linalg.select ins(%3, %c0, %c1 : index, index, index) outs(%4 : tensor&lt;1xi64&gt;) -&gt; tensor&lt;1xi64&gt;
    return
  }
}
```
```
mlir-opt test.mlir
```
```
unsupported non numeric type
UNREACHABLE executed at /llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:624!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.select'
 #<!-- -->0 0x000062755d164e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000062755d1615af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000062755d1616fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007ccf8ce45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007ccf8ce9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007ccf8ce9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007ccf8ce9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007ccf8ce4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007ccf8ce288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000062755d14412e (/llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x00006275630e4337 mlir::linalg::SelectOp::regionBuilder(mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) (/llvmorg-22.1.0-install/bin/mlir-opt+0x84af337)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-linalg

Author: None (xvv4402)

<details>
mlir-opt version: 22.1.0
mlir-opt crashes with an UNREACHABLE error when parsing a **linalg.select** operation that uses index type operands. 
The **linalg.select** region builder expects numeric types (Integer or Float) but does not account for the index type, which is commonly used for pointer-as-index or offset calculations.
```mlir
module {
  memref.global "private" @<!-- -->global_var : memref&lt;10xf32&gt;
  func.func @<!-- -->main() {
    %0 = gpu.lane_id
    %1 = memref.get_global @<!-- -->global_var : memref&lt;10xf32&gt;
    %2 = pdl_interp.create_type i32
    %3 = memref.extract_aligned_pointer_as_index %1 : memref&lt;10xf32&gt; -&gt; index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %4 = tensor.empty() : tensor&lt;1xi64&gt;
    %5 = linalg.select ins(%3, %c0, %c1 : index, index, index) outs(%4 : tensor&lt;1xi64&gt;) -&gt; tensor&lt;1xi64&gt;
    return
  }
}
```
```
mlir-opt test.mlir
```
```
unsupported non numeric type
UNREACHABLE executed at /llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:624!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.select'
 #<!-- -->0 0x000062755d164e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000062755d1615af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000062755d1616fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007ccf8ce45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007ccf8ce9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007ccf8ce9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007ccf8ce9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007ccf8ce4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007ccf8ce288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000062755d14412e (/llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x00006275630e4337 mlir::linalg::SelectOp::regionBuilder(mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) (/llvmorg-22.1.0-install/bin/mlir-opt+0x84af337)
```
</details>


---

