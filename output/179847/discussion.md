# mlir-opt crash: Assertion isIntOrFloat() failed in getIntOrFloatBitWidth during --math-expand-ops with math.ctlz on index type

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179847
**Status:** Closed
**Labels:** crash, mlir:math
**Closed Date:** 2026-02-15T23:56:46Z

## Body

Mlir-opt version: 22.1.0-rc1
Bug Op: **math.ctlz**
Bug Pass: **--math-expand-ops**
Commenting out does not trigger the error: **memref.realloc**
Detailed Description: The crash occurs during the --math-expand-ops pass when it encounters a math.ctlz operation using the index type. While removing memref.realloc appears to "fix" the issue, this is actually due to Dead Code Elimination (DCE); without the realloc operation consuming the result, the pass simply skips the unused math.ctlz and avoids the buggy code path.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = gpu.block_id  x
    %2 = math.ctlz %1 : index
    %3 = index.maxu %2, %1
    %4 = bufferization.to_buffer %0 : tensor<1xi32> to memref<1xi32>
    %5 = memref.realloc %4(%3) : memref<1xi32> to memref<?xi32>
    return
  }
}
```
```
mlir-opt --math-expand-ops temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() && "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --math-expand-ops temp.mlir
 #0 0x00005757bd0a9e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005757bd0a65af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005757bd0a66fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007eb4a5245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007eb4a529eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007eb4a529eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007eb4a529eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007eb4a524527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007eb4a52288ff abort ./stdlib/abort.c:81:7
 #9 0x00007eb4a522881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007eb4a523b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005757c4cf56b6 mlir::Type::getIntOrFloatBitWidth() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa17b6b6)
#12 0x00005757be625927 convertCtlzOp(mlir::math::CountLeadingZerosOp, mlir::PatternRewriter&) ExpandOps.cpp:0:0
#13 0x00005757c48d4e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#14 0x00005757c48d63af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#15 0x00005757c489b3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#16 0x00005757c48a0060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#17 0x00005757be624e71 (anonymous namespace)::MathExpandOpsPass::runOnOperation() ExpandOps.cpp:0:0
#18 0x00005757c4949756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
Mlir-opt version: 22.1.0-rc1
Bug Op: **math.ctlz**
Bug Pass: **--math-expand-ops**
Commenting out does not trigger the error: **memref.realloc**
Detailed Description: The crash occurs during the --math-expand-ops pass when it encounters a math.ctlz operation using the index type. While removing memref.realloc appears to "fix" the issue, this is actually due to Dead Code Elimination (DCE); without the realloc operation consuming the result, the pass simply skips the unused math.ctlz and avoids the buggy code path.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = gpu.block_id  x
    %2 = math.ctlz %1 : index
    %3 = index.maxu %2, %1
    %4 = bufferization.to_buffer %0 : tensor&lt;1xi32&gt; to memref&lt;1xi32&gt;
    %5 = memref.realloc %4(%3) : memref&lt;1xi32&gt; to memref&lt;?xi32&gt;
    return
  }
}
```
```
mlir-opt --math-expand-ops temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() &amp;&amp; "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --math-expand-ops temp.mlir
 #<!-- -->0 0x00005757bd0a9e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005757bd0a65af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005757bd0a66fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007eb4a5245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007eb4a529eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007eb4a529eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007eb4a529eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007eb4a524527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007eb4a52288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007eb4a522881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007eb4a523b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005757c4cf56b6 mlir::Type::getIntOrFloatBitWidth() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa17b6b6)
#<!-- -->12 0x00005757be625927 convertCtlzOp(mlir::math::CountLeadingZerosOp, mlir::PatternRewriter&amp;) ExpandOps.cpp:0:0
#<!-- -->13 0x00005757c48d4e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->14 0x00005757c48d63af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#<!-- -->15 0x00005757c489b3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->16 0x00005757c48a0060 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#<!-- -->17 0x00005757be624e71 (anonymous namespace)::MathExpandOpsPass::runOnOperation() ExpandOps.cpp:0:0
#<!-- -->18 0x00005757c4949756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
```
</details>


---

### Comment 2 - SakshamSinghal20

I would like to work on this could you please assign this to me?


---

### Comment 3 - darshan-opensource

https://github.com/llvm/llvm-project/pull/181539

---

