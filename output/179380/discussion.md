# [mlir-opt] Crash in sparsification-and-bufferization pass triggered by emitc.unary_minus folding on i1

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179380
**Status:** Closed
**Labels:** crash, mlir:math
**Closed Date:** 2026-02-12T09:28:56Z

## Body

The compiler crashes during the **--sparsification-and-bufferization** pass when encountering an emitc.unary_minus operation acting on an i1 (1-bit integer) type. 
The crash is rooted in an APInt assertion failure, specifically when the folding or constant propagation logic attempts to handle the negation of a 1-bit value.
```mlir
module {
  func.func @main() {
    %0 = irdl.c_pred "::llvm::isa<::mlir::IntegerAttr>($_self)"
    %1 = sparse_tensor.has_runtime_library
    %2 = math.ctlz %1 : i1
    %3 = math.ipowi %2, %1 : i1
    %4 = emitc.unary_minus %3 : (i1) -> i1
    return
  }
}
```
```
mlir-opt --sparsification-and-bufferization test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/ADT/APInt.h:120: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isIntN(BitWidth, val) && "Value is not an N-bit signed value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --sparsification-and-bufferization test.mlir
 #0 0x000063b75b1b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000063b75b1b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000063b75b1b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000071ea6b445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000071ea6b49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000071ea6b49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000071ea6b49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000071ea6b44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000071ea6b4288ff abort ./stdlib/abort.c:81:7
 #9 0x000071ea6b42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000071ea6b43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
The compiler crashes during the **--sparsification-and-bufferization** pass when encountering an emitc.unary_minus operation acting on an i1 (1-bit integer) type. 
The crash is rooted in an APInt assertion failure, specifically when the folding or constant propagation logic attempts to handle the negation of a 1-bit value.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = irdl.c_pred "::llvm::isa&lt;::mlir::IntegerAttr&gt;($_self)"
    %1 = sparse_tensor.has_runtime_library
    %2 = math.ctlz %1 : i1
    %3 = math.ipowi %2, %1 : i1
    %4 = emitc.unary_minus %3 : (i1) -&gt; i1
    return
  }
}
```
```
mlir-opt --sparsification-and-bufferization test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/ADT/APInt.h:120: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isIntN(BitWidth, val) &amp;&amp; "Value is not an N-bit signed value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --sparsification-and-bufferization test.mlir
 #<!-- -->0 0x000063b75b1b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000063b75b1b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000063b75b1b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000071ea6b445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000071ea6b49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000071ea6b49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000071ea6b49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000071ea6b44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000071ea6b4288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000071ea6b42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000071ea6b43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

