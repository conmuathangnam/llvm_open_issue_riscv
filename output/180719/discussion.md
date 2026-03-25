# mlir-opt crash: ShapeOfOpToConstShapeOp assertion failure when processing block arguments

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180719
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-02-11T01:09:40Z

## Body

**Bug Op**: shape.shape_of

**Bug Pass**: --canonicalize

**Detailed Description**: The ShapeOfOpToConstShapeOp pattern in the Shape dialect crashes when its operand is a block argument (e.g., inside smt.solver or scf.for). The pattern performs an unsafe llvm::cast<TypedValue<TensorType>> without verifying that the value has a defining operation or the expected type refinement, triggering an assertion failure.

```mlir
module {
  func.func @main() {
    %0 = arm_sme.streaming_vl <byte>
    %1 = arm_sme.streaming_vl <byte>
    %2 = tensor.splat %0 : tensor<index>
    smt.solver(%2) : (tensor<index>) -> () {
      ^bb0(%arg0: tensor<index>):
      %3 = shape.shape_of %arg0 : tensor<index> -> !shape.shape
      %4 = shape.broadcast %3, %3 : !shape.shape, !shape.shape -> !shape.shape
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::detail::TypedValue<mlir::TensorType>; From = mlir::OpResult]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x00005bdf44574e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005bdf445715af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005bdf445716fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b86c7245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007b86c729eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007b86c729eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007b86c729eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007b86c724527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007b86c72288ff abort ./stdlib/abort.c:81:7
 #9 0x00007b86c722881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007b86c723b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005bdf47d6f027 (anonymous namespace)::ShapeOfOpToConstShapeOp::matchAndRewrite(mlir::shape::ShapeOfOp, mlir::PatternRewriter&) const Shape.cpp:0:0
#12 0x00005bdf4bd9fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#13 0x00005bdf4bda13af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#14 0x00005bdf4bd663c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#15 0x00005bdf4bd6b060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#16 0x00005bdf4bcb6ac3 (anonymous namespace)::Canonicalizer::runOnOperation() Canonicalizer.cpp:0:0
#17 0x00005bdf4be14756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#18 0x00005bdf4be14a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#19 0x00005bdf4be151e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#20 0x00005bdf4be16400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#21 0x00005bdf4463b767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#22 0x00005bdf4463c4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#23 0x00005bdf4c1e4731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#24 0x00005bdf446338d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#25 0x00005bdf4463cb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#26 0x00005bdf4463cda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#27 0x00005bdf4446e023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#28 0x00007b86c722a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#29 0x00007b86c722a28b call_init ./csu/../csu/libc-start.c:128:20
#30 0x00007b86c722a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#31 0x00005bdf44553405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: shape.shape_of

**Bug Pass**: --canonicalize

**Detailed Description**: The ShapeOfOpToConstShapeOp pattern in the Shape dialect crashes when its operand is a block argument (e.g., inside smt.solver or scf.for). The pattern performs an unsafe llvm::cast&lt;TypedValue&lt;TensorType&gt;&gt; without verifying that the value has a defining operation or the expected type refinement, triggering an assertion failure.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = arm_sme.streaming_vl &lt;byte&gt;
    %1 = arm_sme.streaming_vl &lt;byte&gt;
    %2 = tensor.splat %0 : tensor&lt;index&gt;
    smt.solver(%2) : (tensor&lt;index&gt;) -&gt; () {
      ^bb0(%arg0: tensor&lt;index&gt;):
      %3 = shape.shape_of %arg0 : tensor&lt;index&gt; -&gt; !shape.shape
      %4 = shape.broadcast %3, %3 : !shape.shape, !shape.shape -&gt; !shape.shape
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::detail::TypedValue&lt;mlir::TensorType&gt;; From = mlir::OpResult]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #<!-- -->0 0x00005bdf44574e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005bdf445715af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005bdf445716fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b86c7245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007b86c729eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007b86c729eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007b86c729eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007b86c724527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007b86c72288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007b86c722881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007b86c723b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005bdf47d6f027 (anonymous namespace)::ShapeOfOpToConstShapeOp::matchAndRewrite(mlir::shape::ShapeOfOp, mlir::PatternRewriter&amp;) const Shape.cpp:0:0
#<!-- -->12 0x00005bdf4bd9fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->13 0x00005bdf4bda13af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#<!-- -->14 0x00005bdf4bd663c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->15 0x00005bdf4bd6b060 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#<!-- -->16 0x00005bdf4bcb6ac3 (anonymous namespace)::Canonicalizer::runOnOperation() Canonicalizer.cpp:0:0
#<!-- -->17 0x00005bdf4be14756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->18 0x00005bdf4be14a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->19 0x00005bdf4be151e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#<!-- -->20 0x00005bdf4be16400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#<!-- -->21 0x00005bdf4463b767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x00005bdf4463c4ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x00005bdf4c1e4731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->24 0x00005bdf446338d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->25 0x00005bdf4463cb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->26 0x00005bdf4463cda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->27 0x00005bdf4446e023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->28 0x00007b86c722a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x00007b86c722a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->30 0x00007b86c722a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->31 0x00005bdf44553405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

