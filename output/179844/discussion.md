# mlir-opt crash: Assertion failed in IRMapping::lookup during --form-expressions involving emitc.bitwise_and and emitc.cast

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179844
**Status:** Closed
**Labels:** crash, mlir:emitc
**Closed Date:** 2026-03-02T10:57:42Z

## Body

mlir-opt version: 22.1.0
The mlir-opt tool crashes with an assertion failure when running the **--emitc-form-expressions** pass on a specific input containing emitc operations mixed with arith and llvm dialects. The crash occurs in mlir::IRMapping::lookup during the FoldExpressionOp rewrite pattern.
```mlir
module {
  func.func @main() {
    %0 = arith.constant 42 : i32
    %1 = mpi.finalize : !mpi.retval
    %2 = emitc.cast %0 : i32 to i32
    %3 = shape.const_size 42
    %4 = llvm.icmp "eq" %2, %0 : i32
    %5 = emitc.bitwise_and %2, %0 : (i32, i32) -> i32
    return
  }
}
```
```
mlir-opt --form-expressions
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/include/mlir/IR/IRMapping.h:74: auto mlir::IRMapping::lookup(T) const [with T = mlir::Operation*]: Assertion `result && "expected 'from' to be contained within the map"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --form-expressions temp.mlir
 #0 0x00005d93c0b0ce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005d93c0b095af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005d93c0b096fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000712d79845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000712d7989eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000712d7989eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000712d7989eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000712d7984527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000712d798288ff abort ./stdlib/abort.c:81:7
 #9 0x0000712d7982881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000712d7983b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005d93c2623508 (anonymous namespace)::FoldExpressionOp::matchAndRewrite(mlir::emitc::ExpressionOp, mlir::PatternRewriter&) const Transforms.cpp:0:0
#12 0x00005d93c8337e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#13 0x00005d93c83393af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#14 0x00005d93c82fe3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#15 0x00005d93c8303060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
mlir-opt version: 22.1.0
The mlir-opt tool crashes with an assertion failure when running the **--emitc-form-expressions** pass on a specific input containing emitc operations mixed with arith and llvm dialects. The crash occurs in mlir::IRMapping::lookup during the FoldExpressionOp rewrite pattern.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = arith.constant 42 : i32
    %1 = mpi.finalize : !mpi.retval
    %2 = emitc.cast %0 : i32 to i32
    %3 = shape.const_size 42
    %4 = llvm.icmp "eq" %2, %0 : i32
    %5 = emitc.bitwise_and %2, %0 : (i32, i32) -&gt; i32
    return
  }
}
```
```
mlir-opt --form-expressions
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/include/mlir/IR/IRMapping.h:74: auto mlir::IRMapping::lookup(T) const [with T = mlir::Operation*]: Assertion `result &amp;&amp; "expected 'from' to be contained within the map"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --form-expressions temp.mlir
 #<!-- -->0 0x00005d93c0b0ce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005d93c0b095af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005d93c0b096fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000712d79845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000712d7989eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000712d7989eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000712d7989eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000712d7984527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000712d798288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000712d7982881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000712d7983b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005d93c2623508 (anonymous namespace)::FoldExpressionOp::matchAndRewrite(mlir::emitc::ExpressionOp, mlir::PatternRewriter&amp;) const Transforms.cpp:0:0
#<!-- -->12 0x00005d93c8337e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->13 0x00005d93c83393af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#<!-- -->14 0x00005d93c82fe3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->15 0x00005d93c8303060 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
```
</details>


---

### Comment 2 - SakshamSinghal20

I would like to work on this could you please assign this to me?

---

### Comment 3 - sweiglbosker

Just prepare a PR and mention this issue

---

### Comment 4 - SakshamSinghal20

okay sure

---

