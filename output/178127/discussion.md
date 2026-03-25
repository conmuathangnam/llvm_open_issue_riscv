# mlir-opt crashes when casting vector to spirv::CompositeType

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/178127
**Status:** Closed
**Labels:** mlir:spirv, crash
**Closed Date:** 2026-01-27T15:08:27Z

## Body

mlir-opt version: 22.1.0-rc1
When running mlir-opt on the following MLIR code, it triggers an assertion failure in Casting.h:
```mlir
module {
  func.func @main() {
    %0 = vector.constant_mask [1, 1] : vector<2x2xi1>
    %1 = gpu.subgroup_id : index
    affine.if affine_set<(d0, d1) : (d0 >= 0, d1 >= 0)>(%1, %1) {
    }
    %2 = spirv.CompositeConstruct %0, %0 : (vector<2x2xi1>, vector<2x2xi1>) -> vector<4x2xi1>
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::spirv::CompositeType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
 #0 0x000063d52e87ee32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000063d52e87b5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000063d52e87b6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b99a8645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007b99a869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007b99a869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007b99a869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007b99a864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007b99a86288ff abort ./stdlib/abort.c:81:7
 #9 0x00007b99a862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007b99a863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-spirv

Author: None (mlirBug)

<details>
mlir-opt version: 22.1.0-rc1
When running mlir-opt on the following MLIR code, it triggers an assertion failure in Casting.h:
```
module {
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1, 1] : vector&lt;2x2xi1&gt;
    %1 = gpu.subgroup_id : index
    affine.if affine_set&lt;(d0, d1) : (d0 &gt;= 0, d1 &gt;= 0)&gt;(%1, %1) {
    }
    %2 = spirv.CompositeConstruct %0, %0 : (vector&lt;2x2xi1&gt;, vector&lt;2x2xi1&gt;) -&gt; vector&lt;4x2xi1&gt;
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::spirv::CompositeType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
 #<!-- -->0 0x000063d52e87ee32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000063d52e87b5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000063d52e87b6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b99a8645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007b99a869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007b99a869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007b99a869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007b99a864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007b99a86288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007b99a862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007b99a863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

### Comment 2 - ghost

So, multi-dimensional vectors are not supported in SPIRV-V, so I don't think that's a correct code. However, this should definitely not crash.

The code hits this assert:

https://github.com/llvm/llvm-project/blob/b232970172dd0df0c8ab6c8147b6bf81a477fa03/mlir/lib/Dialect/SPIRV/IR/SPIRVOps.cpp#L363

I believe it uses `cast` because the CompositeConstruct requires the result to be a `SPIRV_Composite`:

https://github.com/llvm/llvm-project/blob/ba833a616554ce50868a562e525faafea0d387d4/mlir/include/mlir/Dialect/SPIRV/IR/SPIRVCompositeOps.td#L66

However, it seems there is a verification gap between `SPIRV_Composite` and `spirv::CompositeType`. The former requires vector to be `SPIRV_Vector`, but `SPIRV_Vector` itself only enforces length but not rank. On the other hand, the `isValid` function in the `spirv::CompositeType` does check the rank and the cast fails:

https://github.com/llvm/llvm-project/blob/8dfca82a7cb3ee4d9f768a6c3a61236ffb329a22/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp#L188

I see two options here. We either replace `cast` with `dyn_cast` and catch it there (it only fixes this particular problem) or we update the `SPIRV_Vector` type so it enforces that the rank is one, so this cast will never be reached as the verification fails at the operands level.

---

### Comment 3 - ghost

I have create a PR addressing the rank of `SPIR_Vector` (#178185) that fixes the root cause of this issue.

---

