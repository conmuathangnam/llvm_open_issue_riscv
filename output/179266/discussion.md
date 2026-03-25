# [mlir-opt] Crash in arith-int-range-narrowing when handling arith.trunci to i0 (APInt assertion failure)

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/179266
**Status:** Closed
**Labels:** crash, mlir:arith, mlir:vector
**Closed Date:** 2026-03-04T11:34:20Z

## Body

The --arith-int-range-narrowing pass crashes when encountering an **arith.trunci** operation that truncates a value to an i0 (zero-bit integer). The crash is triggered by an assertion failure in LLVM's APInt::ashrInPlace, which is called during the integer range inference process.
```mlir
module {
  func.func @main() {
    %0 = ml_program.token
    %1 = spirv.GroupNonUniformElect <Workgroup> : i1
    %2 = scf.if %1 -> (i1) {
      scf.yield %1 : i1
    } else {
      scf.yield %1 : i1
    }
    %3 = arith.trunci %2 : i1 to i0
    return
  }
}
```
```
mlir-opt --arith-int-range-narrowing
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/ADT/APInt.h:836: void llvm::APInt::ashrInPlace(unsigned int): Assertion `ShiftAmt <= BitWidth && "Invalid shift amount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --arith-int-range-narrowing test.mlir
 #0 0x000058d831ce9e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000058d831ce65af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000058d831ce66fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000761d44e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000761d44e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000761d44e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000761d44e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000761d44e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000761d44e288ff abort ./stdlib/abort.c:81:7
 #9 0x0000761d44e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000761d44e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (mlirBug)

<details>
The --arith-int-range-narrowing pass crashes when encountering an **arith.trunci** operation that truncates a value to an i0 (zero-bit integer). The crash is triggered by an assertion failure in LLVM's APInt::ashrInPlace, which is called during the integer range inference process.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ml_program.token
    %1 = spirv.GroupNonUniformElect &lt;Workgroup&gt; : i1
    %2 = scf.if %1 -&gt; (i1) {
      scf.yield %1 : i1
    } else {
      scf.yield %1 : i1
    }
    %3 = arith.trunci %2 : i1 to i0
    return
  }
}
```
```
mlir-opt --arith-int-range-narrowing
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/ADT/APInt.h:836: void llvm::APInt::ashrInPlace(unsigned int): Assertion `ShiftAmt &lt;= BitWidth &amp;&amp; "Invalid shift amount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --arith-int-range-narrowing test.mlir
 #<!-- -->0 0x000058d831ce9e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000058d831ce65af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000058d831ce66fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000761d44e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000761d44e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000761d44e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000761d44e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000761d44e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000761d44e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000761d44e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000761d44e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

### Comment 2 - shoaibkhan-sde

I can reproduce this crash locally. I am a student and would like to try fixing this, Please assign this to me.

---

