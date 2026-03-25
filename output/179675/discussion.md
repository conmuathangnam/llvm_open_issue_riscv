# [mlir-opt] Crash in convert-func-to-spirv pass: llvm::cast<SPIRVType> failure on non-SPIRV struct members

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179675
**Status:** Closed
**Labels:** mlir:spirv, crash
**Closed Date:** 2026-02-28T21:21:39Z

## Body

The compiler crashes with a cast<SPIRVType> assertion failure when running the --convert-func-to-spirv pass. 
The issue occurs because **spirv.Variable** is defined with a !spirv.struct that contains members of type vector<2x2xi1>. During the conversion process, the SPIRV conversion target attempts to verify the legality of the operation by inspecting its types, but fails when it encounters a member type that has not been converted to a valid SPIRV type.
```mlir
module {
  spirv.EntryPoint "GLCompute" @main
  func.func @main() {
    %0 = vector.constant_mask [1, 1] : vector<2x2xi1>
    %1 = shape.const_shape [1, 1] : !shape.shape
    %2 = spirv.Variable : !spirv.ptr<!spirv.struct<(vector<2x2xi1>, vector<2x2xi1>)>, Function>
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-spirv temp.mlir
 #0 0x00005c8622fc5e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005c8622fc25af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005c8622fc26fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f07e7445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f07e749eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f07e749eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f07e749eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f07e744527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f07e74288ff abort ./stdlib/abort.c:81:7
 #9 0x00007f07e742881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f07e743b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005c8627108848 (llvmorg-22.1.0-install/bin/mlir-opt+0x6672848)
#12 0x00005c862710a0cc (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
#13 0x00005c862710a11c (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
The compiler crashes with a cast&lt;SPIRVType&gt; assertion failure when running the --convert-func-to-spirv pass. 
The issue occurs because **spirv.Variable** is defined with a !spirv.struct that contains members of type vector&lt;2x2xi1&gt;. During the conversion process, the SPIRV conversion target attempts to verify the legality of the operation by inspecting its types, but fails when it encounters a member type that has not been converted to a valid SPIRV type.
```mlir
module {
  spirv.EntryPoint "GLCompute" @<!-- -->main
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1, 1] : vector&lt;2x2xi1&gt;
    %1 = shape.const_shape [1, 1] : !shape.shape
    %2 = spirv.Variable : !spirv.ptr&lt;!spirv.struct&lt;(vector&lt;2x2xi1&gt;, vector&lt;2x2xi1&gt;)&gt;, Function&gt;
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-spirv temp.mlir
 #<!-- -->0 0x00005c8622fc5e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005c8622fc25af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005c8622fc26fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f07e7445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f07e749eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f07e749eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f07e749eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f07e744527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f07e74288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f07e742881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f07e743b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005c8627108848 (llvmorg-22.1.0-install/bin/mlir-opt+0x6672848)
#<!-- -->12 0x00005c862710a0cc (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
#<!-- -->13 0x00005c862710a11c (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
```
</details>


---

### Comment 2 - spider-yamet

@EugeneZelenko @Subway2023 May I pick up this issue?

---

### Comment 3 - EugeneZelenko

@spider-yamet: Just prepare pull request and mention it on this page.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-mlir-spirv

Author: None (Subway2023)

<details>
The compiler crashes with a cast&lt;SPIRVType&gt; assertion failure when running the --convert-func-to-spirv pass. 
The issue occurs because **spirv.Variable** is defined with a !spirv.struct that contains members of type vector&lt;2x2xi1&gt;. During the conversion process, the SPIRV conversion target attempts to verify the legality of the operation by inspecting its types, but fails when it encounters a member type that has not been converted to a valid SPIRV type.
```mlir
module {
  spirv.EntryPoint "GLCompute" @<!-- -->main
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1, 1] : vector&lt;2x2xi1&gt;
    %1 = shape.const_shape [1, 1] : !shape.shape
    %2 = spirv.Variable : !spirv.ptr&lt;!spirv.struct&lt;(vector&lt;2x2xi1&gt;, vector&lt;2x2xi1&gt;)&gt;, Function&gt;
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-func-to-spirv temp.mlir
 #<!-- -->0 0x00005c8622fc5e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005c8622fc25af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005c8622fc26fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f07e7445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f07e749eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f07e749eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f07e749eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f07e744527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f07e74288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f07e742881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f07e743b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005c8627108848 (llvmorg-22.1.0-install/bin/mlir-opt+0x6672848)
#<!-- -->12 0x00005c862710a0cc (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
#<!-- -->13 0x00005c862710a11c (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
```
</details>


---

