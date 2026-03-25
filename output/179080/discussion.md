# mlir-opt --convert-complex-to-libm crashes: Assertion isa<To>(Val) failed in arith::AddUIExtendedOp::fold with ub.poison

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179080
**Status:** Closed
**Labels:** crash, mlir:arith
**Closed Date:** 2026-03-11T13:07:45Z

## Body

When using mlir-opt with the **--convert-complex-to-libm** flag, the compiler crashes with an assertion failure in the arith dialect's folding logic. The crash occurs when **arith.addui_extended** operates on a **ub.poison** value, as the folder fails to handle non-typed attributes correctly during the casting process.
```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = ub.poison : tensor<1xi32>
    %2, %3 = arith.addui_extended %0, %1 : tensor<1xi32>, tensor<1xi1>
    return
  }
}
```
```
mlir-opt --convert-complex-to-libm test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::TypedAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-complex-to-libm test.mlir
 #0 0x000057ca52808e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000057ca528055af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000057ca528056fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007bfc32c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007bfc32c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007bfc32c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007bfc32c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007bfc32c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007bfc32c288ff abort ./stdlib/abort.c:81:7
 #9 0x00007bfc32c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
When using mlir-opt with the **--convert-complex-to-libm** flag, the compiler crashes with an assertion failure in the arith dialect's folding logic. The crash occurs when **arith.addui_extended** operates on a **ub.poison** value, as the folder fails to handle non-typed attributes correctly during the casting process.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = ub.poison : tensor&lt;1xi32&gt;
    %2, %3 = arith.addui_extended %0, %1 : tensor&lt;1xi32&gt;, tensor&lt;1xi1&gt;
    return
  }
}
```
```
mlir-opt --convert-complex-to-libm test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::TypedAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-complex-to-libm test.mlir
 #<!-- -->0 0x000057ca52808e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000057ca528055af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000057ca528056fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007bfc32c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007bfc32c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007bfc32c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007bfc32c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007bfc32c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007bfc32c288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007bfc32c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
```

</details>


---

### Comment 2 - xvv4402

There’s a similar bug here, but with a different crash message
```mlir
module {
  func.func @main() {
    %0 = arith.constant 42 : i32
    %1 = ub.poison : i32
    %2, %3 = arith.addui_extended %0, %1 : i32, i1
    return
  }
}
```
```
mlir-opt --arith-int-range-narrowing temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::TypedAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --arith-int-range-narrowing temp.mlir
 #0 0x00005c00787f3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005c00787f05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005c00787f06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073a319245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073a31929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073a31929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073a31929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073a31924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073a3192288ff abort ./stdlib/abort.c:81:7
 #9 0x000073a31922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073a31923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
······
#32 0x00005c00786ed023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#33 0x000073a31922a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x000073a31922a28b call_init ./csu/../csu/libc-start.c:128:20
#35 0x000073a31922a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x00005c00787d2405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

Crash information of 179080
```
mlir-opt --convert-complex-to-libm temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::TypedAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-complex-to-libm temp.mlir
 #0 0x0000578d6227be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000578d622785af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000578d622786fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007bbd60c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007bbd60c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007bbd60c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007bbd60c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007bbd60c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007bbd60c288ff abort ./stdlib/abort.c:81:7
 #9 0x00007bbd60c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007bbd60c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000578d6992c05e mlir::arith::AddUIExtendedOp::fold(mlir::arith::AddUIExtendedOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x9be005e)
······
#33 0x0000578d62343da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#34 0x0000578d62175023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#35 0x00007bbd60c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#36 0x00007bbd60c2a28b call_init ./csu/../csu/libc-start.c:128:20
#37 0x00007bbd60c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#38 0x0000578d6225a405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

---

### Comment 3 - Men-cotton

I confirmed that this crash is fixed by #183596.

---

