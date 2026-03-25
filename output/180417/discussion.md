# mlir-opt crash: ShapeOfOpToConstShapeOp assertion failure during --inline

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180417
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-02-11T01:50:26Z

## Body

**Bug Op**: shape.shape_of 

**Bug Pass**: --inline

**Detailed Description**: The mlir-opt tool crashes during the --inline pass (which often triggers cleanup/folding patterns). The pattern ShapeOfOpToConstShapeOp within the Shape dialect incorrectly performs a llvm::cast<TypedValue<TensorType>> on an operation's result without verifying the type. When the inliner attempts to simplify the IR, it triggers this assertion failure in Shape.cpp.

```mlir
module {
  emitc.class @MyClass {
    emitc.field @fieldName0 : !emitc.array<1xf32>
  }
  func.func @main() {
    %0 = "emitc.constant"() <{value = 0 : i32}> : () -> i32
    %1 = arith.extsi %0 : i32 to i64
    %2 = arith.trunci %1 : i64 to i32
    %3 = tensor.from_elements %2 : tensor<1xi32>
    %4 = tosa.clamp %3 {max_val = 100 : i32, min_val = 0 : i32} : (tensor<1xi32>) -> tensor<1xi32>
    %shape3 = shape.shape_of %3 : tensor<1xi32> -> !shape.shape
    %shape4 = shape.shape_of %4 : tensor<1xi32> -> !shape.shape
    %5 = shape.cstr_eq %shape4, %shape3 : !shape.shape, !shape.shape
    return
  }
}
```
```
mlir-opt --inline temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::detail::TypedValue<mlir::TensorType>; From = mlir::OpResult]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --inline temp.mlir
 #0 0x0000595b9356be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000595b935685af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000595b935686fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000070cbbc845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000070cbbc89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000070cbbc89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000070cbbc89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000070cbbc84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000070cbbc8288ff abort ./stdlib/abort.c:81:7
 #9 0x000070cbbc82881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000070cbbc83b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000595b96d66027 (anonymous namespace)::ShapeOfOpToConstShapeOp::matchAndRewrite(mlir::shape::ShapeOfOp, mlir::PatternRewriter&) const Shape.cpp:0:0
#12 0x0000595b9ad96e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: shape.shape_of 

**Bug Pass**: --inline

**Detailed Description**: The mlir-opt tool crashes during the --inline pass (which often triggers cleanup/folding patterns). The pattern ShapeOfOpToConstShapeOp within the Shape dialect incorrectly performs a llvm::cast&lt;TypedValue&lt;TensorType&gt;&gt; on an operation's result without verifying the type. When the inliner attempts to simplify the IR, it triggers this assertion failure in Shape.cpp.

```mlir
module {
  emitc.class @<!-- -->MyClass {
    emitc.field @<!-- -->fieldName0 : !emitc.array&lt;1xf32&gt;
  }
  func.func @<!-- -->main() {
    %0 = "emitc.constant"() &lt;{value = 0 : i32}&gt; : () -&gt; i32
    %1 = arith.extsi %0 : i32 to i64
    %2 = arith.trunci %1 : i64 to i32
    %3 = tensor.from_elements %2 : tensor&lt;1xi32&gt;
    %4 = tosa.clamp %3 {max_val = 100 : i32, min_val = 0 : i32} : (tensor&lt;1xi32&gt;) -&gt; tensor&lt;1xi32&gt;
    %shape3 = shape.shape_of %3 : tensor&lt;1xi32&gt; -&gt; !shape.shape
    %shape4 = shape.shape_of %4 : tensor&lt;1xi32&gt; -&gt; !shape.shape
    %5 = shape.cstr_eq %shape4, %shape3 : !shape.shape, !shape.shape
    return
  }
}
```
```
mlir-opt --inline temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::detail::TypedValue&lt;mlir::TensorType&gt;; From = mlir::OpResult]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --inline temp.mlir
 #<!-- -->0 0x0000595b9356be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000595b935685af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000595b935686fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000070cbbc845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000070cbbc89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000070cbbc89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000070cbbc89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000070cbbc84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000070cbbc8288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000070cbbc82881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000070cbbc83b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000595b96d66027 (anonymous namespace)::ShapeOfOpToConstShapeOp::matchAndRewrite(mlir::shape::ShapeOfOp, mlir::PatternRewriter&amp;) const Shape.cpp:0:0
#<!-- -->12 0x0000595b9ad96e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
```
</details>


---

### Comment 2 - CoTinker

Fixed by https://github.com/llvm/llvm-project/pull/180737.

---

