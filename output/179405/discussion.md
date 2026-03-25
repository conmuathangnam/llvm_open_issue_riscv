# [mlir-opt] Crash in affine.linearize_index folding triggered by index.shru usage: llvm::cast<IntegerAttr> failure with ub.poison

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179405
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-02-28T11:15:30Z

## Body

mlir-opt crashes during the --**canonicalize** pass when an **affine.linearize_index** operation (containing ub.poison) is consumed by another operation (like index.shru). 
The crash is caused by an unsafe llvm::cast<IntegerAttr> in the AffineLinearizeIndexOp::fold implementation, which fails to account for PoisonAttr.

```mlir
module {
  func.func @main() {
    %0 = ub.poison : index
    %1 = "emitc.constant"() <{value = 0 : index}> : () -> index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    %3 = index.shru %2, %1
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x00005a819e61be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005a819e6185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005a819e6186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000079bb3ac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000079bb3ac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000079bb3ac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000079bb3ac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000079bb3ac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000079bb3ac288ff abort ./stdlib/abort.c:81:7
 #9 0x000079bb3ac2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000079bb3ac3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005a81a5a8c8f2 (llvmorg-22.1.0-install/bin/mlir-opt+0x99a08f2)
#12 0x00005a81a5ac8872 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc872)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: None (xvv4402)

<details>
mlir-opt crashes during the --**canonicalize** pass when an **affine.linearize_index** operation (containing ub.poison) is consumed by another operation (like index.shru). 
The crash is caused by an unsafe llvm::cast&lt;IntegerAttr&gt; in the AffineLinearizeIndexOp::fold implementation, which fails to account for PoisonAttr.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = ub.poison : index
    %1 = "emitc.constant"() &lt;{value = 0 : index}&gt; : () -&gt; index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    %3 = index.shru %2, %1
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #<!-- -->0 0x00005a819e61be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005a819e6185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005a819e6186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000079bb3ac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000079bb3ac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000079bb3ac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000079bb3ac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000079bb3ac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000079bb3ac288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000079bb3ac2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000079bb3ac3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005a81a5a8c8f2 (llvmorg-22.1.0-install/bin/mlir-opt+0x99a08f2)
#<!-- -->12 0x00005a81a5ac8872 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc872)
```
</details>


---

