# [mlir-opt] --affine-expand-index-ops-as-affine crashes on affine.linearize_index with ub.poison

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179081
**Status:** Closed
**Labels:** duplicate, mlir, crash
**Closed Date:** 2026-02-01T02:03:23Z

## Body

The --affine-expand-index-ops-as-affine pass crashes when affine.linearize_index uses a ub.poison operand. The crash is triggered during the transformation of "live" operations (those with users) due to an invalid cast in the affine folding logic.

```mlir
module {
  func.func @main() {
    %0 = gpu.block_id  x
    %1 = ub.poison : index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    %3 = emitc.cast %2 : index to i32
    return
  }
}
```
```
mlir-opt --affine-expand-index-ops-as-affine
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --affine-expand-index-ops-as-affine test.mlir
 #0 0x00005671f20e6e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005671f20e35af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005671f20e36fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f4160045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f416009eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f416009eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f416009eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f416004527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f41600288ff abort ./stdlib/abort.c:81:7
 #9 0x00007f416002881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f416003b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
The --affine-expand-index-ops-as-affine pass crashes when affine.linearize_index uses a ub.poison operand. The crash is triggered during the transformation of "live" operations (those with users) due to an invalid cast in the affine folding logic.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.block_id  x
    %1 = ub.poison : index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    %3 = emitc.cast %2 : index to i32
    return
  }
}
```
```
mlir-opt --affine-expand-index-ops-as-affine
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --affine-expand-index-ops-as-affine test.mlir
 #<!-- -->0 0x00005671f20e6e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005671f20e35af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005671f20e36fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f4160045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f416009eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f416009eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f416009eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f416004527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f41600288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f416002881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f416003b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

