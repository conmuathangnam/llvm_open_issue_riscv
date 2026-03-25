# [mlir-opt] Crash in getMixedValues triggered by shape.mul consuming affine.delinearize_index results during canonicalization

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179401
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T15:56:16Z

## Body

mlir-opt crashes with an assertion failure in mlir::getMixedValues during the **--canonicalize** pass. 
The crash occurs when a **shape.mul** operation attempts to process results from affine.delinearize_index. This triggers a folding sequence where the dynamic basis of the delinearization operation causes an internal static/dynamic count mismatch.
```mlir
module {
  func.func @main() {
    %0 = ub.poison : index
    %1 = gpu.cluster_id  x
    %2 = affine.delinearize_index %0 into (%1) : index
    %3 = affine.delinearize_index %1 into (%2) : index
    %4 = ml_program.token
    %5 = shape.mul %3, %2 : index, index -> index
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector<mlir::OpFoldResult> mlir::getMixedValues(llvm::ArrayRef<long int>, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast<size_t>(llvm::count_if( staticValues, ShapedType::isDynamic)) && "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize test.mlir
 #0 0x000064a7fff70e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000064a7fff6d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000064a7fff6d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007dd07ce45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007dd07ce9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007dd07ce9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007dd07ce9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007dd07ce4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007dd07ce288ff abort ./stdlib/abort.c:81:7
 #9 0x00007dd07ce2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007dd07ce3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
mlir-opt crashes with an assertion failure in mlir::getMixedValues during the **--canonicalize** pass. 
The crash occurs when a **shape.mul** operation attempts to process results from affine.delinearize_index. This triggers a folding sequence where the dynamic basis of the delinearization operation causes an internal static/dynamic count mismatch.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ub.poison : index
    %1 = gpu.cluster_id  x
    %2 = affine.delinearize_index %0 into (%1) : index
    %3 = affine.delinearize_index %1 into (%2) : index
    %4 = ml_program.token
    %5 = shape.mul %3, %2 : index, index -&gt; index
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector&lt;mlir::OpFoldResult&gt; mlir::getMixedValues(llvm::ArrayRef&lt;long int&gt;, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast&lt;size_t&gt;(llvm::count_if( staticValues, ShapedType::isDynamic)) &amp;&amp; "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize test.mlir
 #<!-- -->0 0x000064a7fff70e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000064a7fff6d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000064a7fff6d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007dd07ce45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007dd07ce9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007dd07ce9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007dd07ce9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007dd07ce4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007dd07ce288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007dd07ce2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007dd07ce3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

### Comment 2 - Men-cotton

No crash at HEAD. I believe #183650 resolved this.

---

