# [mlir-opt] Crash in getMixedValues during --convert-func-to-llvm involving affine.linearize_index

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/179265
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-02-27T10:43:59Z

## Body

mlir-opt version: 22.1.0-rc1

A crash occurs in mlir-opt when attempting to fold or lower the **affine.linearize_index** operation. The error is triggered by an assertion in StaticValueUtils.cpp, where the logic for handling mixed static/dynamic values fails to reconcile the provided basis with the actual dynamic operands.

```
mlir-opt --convert-func-to-llvm test.mlir
```
```mlir
module {
  func.func @main() {
    %0 = ub.poison : index
    %1 = gpu.subgroup_size : index
    %2 = index.mul %1, %0
    %3 = index.shl %2, %0
    %4 = vector.from_elements %3 : vector<1xindex>
    %5 = affine.linearize_index [%3] by (%0) : index
    return
  }
}
```
```
oRepair/validSeed/300848_17fd_5.mlir
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector<mlir::OpFoldResult> mlir::getMixedValues(llvm::ArrayRef<long int>, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast<size_t>(llvm::count_if( staticValues, ShapedType::isDynamic)) && "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-llvm test.mlir
 #0 0x000055e631a80e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000055e631a7d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000055e631a7d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f4e0ea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f4e0ea9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f4e0ea9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f4e0ea9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f4e0ea4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f4e0ea288ff abort ./stdlib/abort.c:81:7
 #9 0x00007f4e0ea2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f4e0ea3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000055e6391be2ee (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d2ee)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (mlirBug)

<details>
mlir-opt version: 22.1.0-rc1

A crash occurs in mlir-opt when attempting to fold or lower the **affine.linearize_index** operation. The error is triggered by an assertion in StaticValueUtils.cpp, where the logic for handling mixed static/dynamic values fails to reconcile the provided basis with the actual dynamic operands.

```
mlir-opt --convert-func-to-llvm test.mlir
```
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ub.poison : index
    %1 = gpu.subgroup_size : index
    %2 = index.mul %1, %0
    %3 = index.shl %2, %0
    %4 = vector.from_elements %3 : vector&lt;1xindex&gt;
    %5 = affine.linearize_index [%3] by (%0) : index
    return
  }
}
```
```
oRepair/validSeed/300848_17fd_5.mlir
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector&lt;mlir::OpFoldResult&gt; mlir::getMixedValues(llvm::ArrayRef&lt;long int&gt;, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast&lt;size_t&gt;(llvm::count_if( staticValues, ShapedType::isDynamic)) &amp;&amp; "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-llvm test.mlir
 #<!-- -->0 0x000055e631a80e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000055e631a7d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000055e631a7d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f4e0ea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f4e0ea9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f4e0ea9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f4e0ea9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f4e0ea4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f4e0ea288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f4e0ea2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f4e0ea3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000055e6391be2ee (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d2ee)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: None (mlirBug)

<details>
mlir-opt version: 22.1.0-rc1

A crash occurs in mlir-opt when attempting to fold or lower the **affine.linearize_index** operation. The error is triggered by an assertion in StaticValueUtils.cpp, where the logic for handling mixed static/dynamic values fails to reconcile the provided basis with the actual dynamic operands.

```
mlir-opt --convert-func-to-llvm test.mlir
```
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ub.poison : index
    %1 = gpu.subgroup_size : index
    %2 = index.mul %1, %0
    %3 = index.shl %2, %0
    %4 = vector.from_elements %3 : vector&lt;1xindex&gt;
    %5 = affine.linearize_index [%3] by (%0) : index
    return
  }
}
```
```
oRepair/validSeed/300848_17fd_5.mlir
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector&lt;mlir::OpFoldResult&gt; mlir::getMixedValues(llvm::ArrayRef&lt;long int&gt;, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast&lt;size_t&gt;(llvm::count_if( staticValues, ShapedType::isDynamic)) &amp;&amp; "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-llvm test.mlir
 #<!-- -->0 0x000055e631a80e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000055e631a7d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000055e631a7d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f4e0ea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f4e0ea9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f4e0ea9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f4e0ea9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f4e0ea4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f4e0ea288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f4e0ea2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f4e0ea3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000055e6391be2ee (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d2ee)
```
</details>


---

