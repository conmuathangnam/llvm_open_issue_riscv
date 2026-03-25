# mlir-opt crash: Assertion failure isIntOrFloat() in Type::getIntOrFloatBitWidth when parsing tosa.variable with index array

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179045
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-01T10:57:26Z

## Body

A crash occurs in mlir-opt when parsing a **tosa.variable** operation that contains a dense array of index types (e.g., array<index: ...>). 
The crash happens during attribute parsing because the parser calls getIntOrFloatBitWidth() on an IndexType, which is not allowed as it is target-dependent and doesn't satisfy the isIntOrFloat() check.

```mlir
module {
  func.func @main() {
    tosa.variable @my_variable {var_shape = array<index: 1, 2>, type = tensor<1x2xi32>, initial_value = dense<0> : tensor<1x2xi32>}
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: /llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() && "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'tosa.variable'
 #0 0x000064e743827e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000064e7438245af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000064e7438246fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007fdb2ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007fdb2ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007fdb2ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007fdb2ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007fdb2ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007fdb2ba288ff abort ./stdlib/abort.c:81:7
 #9 0x00007fdb2ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007fdb2ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
A crash occurs in mlir-opt when parsing a **tosa.variable** operation that contains a dense array of index types (e.g., array&lt;index: ...&gt;). 
The crash happens during attribute parsing because the parser calls getIntOrFloatBitWidth() on an IndexType, which is not allowed as it is target-dependent and doesn't satisfy the isIntOrFloat() check.

```mlir
module {
  func.func @<!-- -->main() {
    tosa.variable @<!-- -->my_variable {var_shape = array&lt;index: 1, 2&gt;, type = tensor&lt;1x2xi32&gt;, initial_value = dense&lt;0&gt; : tensor&lt;1x2xi32&gt;}
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: /llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() &amp;&amp; "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'tosa.variable'
 #<!-- -->0 0x000064e743827e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000064e7438245af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000064e7438246fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007fdb2ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007fdb2ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007fdb2ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007fdb2ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007fdb2ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007fdb2ba288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007fdb2ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007fdb2ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

</details>


---

### Comment 2 - EugeneZelenko

@xvv4402: Please state clearly in issue descriptions if problematic code is result of fuzzing.

---

