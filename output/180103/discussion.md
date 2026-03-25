# mlir-opt crash: math.fma conversion to APFloat fails on vector types

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180103
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-24T09:40:17Z

## Body

**MLIR Version**: 22.1.0

**Bug Op**: math.fma 

**Bug Pass**: --convert-math-to-apfloat

**Detailed Description**: The mlir-opt tool crashes when attempting to convert math.fma to APFloat if the operands are of VectorType. The conversion pattern FmaOpToAPFloatConversion incorrectly attempts to cast the operand's Type directly to FloatType without checking if the type is a container (like VectorType). This results in a failed assertion: isa<To>(Val) && "cast<Ty>() argument of incompatible type!".

```mlir
module {
  func.func @main() {
    %0 = gpu.subgroup_size : index
    %1 = spirv.Undef : vector<4xf32>
    %2 = gpu.global_id  x
    %3 = vector.broadcast %1 : vector<4xf32> to vector<4xf32>
    %4 = amdgpu.swizzle_bitmode %3 1 2 4 : vector<4xf32>
    %5 = math.fma %4, %1, %1 : vector<4xf32>
    return
  }
}
```
```
mlir-opt --convert-math-to-apfloat temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::FloatType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-math-to-apfloat temp.mlir
 #0 0x00005cff528dde32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005cff528da5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005cff528da6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007082fac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007082fac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007082fac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007082fac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007082fac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007082fac288ff abort ./stdlib/abort.c:81:7
 #9 0x00007082fac2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007082fac3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005cff53f2c8c8 FmaOpToAPFloatConversion::matchAndRewrite(mlir::math::FmaOp, mlir::PatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x3b7e8c8)
#12 0x00005cff5a108e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#13 0x00005cff5a10a775 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c775)
#14 0x00005cff5a102848 mlir::walkAndApplyPatterns(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::RewriterBase::Listener*)::'lambda'()::operator()() const WalkPatternRewriteDriver.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**MLIR Version**: 22.1.0

**Bug Op**: math.fma 

**Bug Pass**: --convert-math-to-apfloat

**Detailed Description**: The mlir-opt tool crashes when attempting to convert math.fma to APFloat if the operands are of VectorType. The conversion pattern FmaOpToAPFloatConversion incorrectly attempts to cast the operand's Type directly to FloatType without checking if the type is a container (like VectorType). This results in a failed assertion: isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!".

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.subgroup_size : index
    %1 = spirv.Undef : vector&lt;4xf32&gt;
    %2 = gpu.global_id  x
    %3 = vector.broadcast %1 : vector&lt;4xf32&gt; to vector&lt;4xf32&gt;
    %4 = amdgpu.swizzle_bitmode %3 1 2 4 : vector&lt;4xf32&gt;
    %5 = math.fma %4, %1, %1 : vector&lt;4xf32&gt;
    return
  }
}
```
```
mlir-opt --convert-math-to-apfloat temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::FloatType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-math-to-apfloat temp.mlir
 #<!-- -->0 0x00005cff528dde32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005cff528da5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005cff528da6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007082fac45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007082fac9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007082fac9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007082fac9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007082fac4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007082fac288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007082fac2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007082fac3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005cff53f2c8c8 FmaOpToAPFloatConversion::matchAndRewrite(mlir::math::FmaOp, mlir::PatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x3b7e8c8)
#<!-- -->12 0x00005cff5a108e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->13 0x00005cff5a10a775 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c775)
#<!-- -->14 0x00005cff5a102848 mlir::walkAndApplyPatterns(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::RewriterBase::Listener*)::'lambda'()::operator()() const WalkPatternRewriteDriver.cpp:0:0
```
</details>


---

### Comment 2 - woruyu

I would like  to fix it!

---

### Comment 3 - woruyu

@xvv4402 , I try it on the latest mlir-opt, It works fine. https://godbolt.org/z/hK5PGb4Kh

---

### Comment 4 - Men-cotton

I believe the PR https://github.com/llvm/llvm-project/pull/172715 is the one.

---

