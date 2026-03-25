# mlir-opt crash: UNREACHABLE in XeGPUSubgroupDistribute due to missing uArch

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/181531
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-03T11:19:39Z

## Body

Bug Pass: --xegpu-subgroup-distribute

Bug Op: gpu.func

Description: The XeGPUSubgroupDistribute pass crashes with an UNREACHABLE error when it cannot resolve a matching Intel GPU micro-architecture (uArch) for a function body. This is triggered when a gpu.func contains operations from incompatible dialects (like rocdl) or lacks target-specific attributes, causing the uArch dispatcher to abort rather than fail gracefully.

```mlir
module {
  gpu.module @main_module {
    spirv.EntryPoint "GLCompute" @main
    gpu.func @main() -> index {
      %0 = index.sizeof
      %1 = gpu.thread_id  x
      %2 = rocdl.wavefrontsize : i32
      gpu.return %0 : index
    }
  }
}
```
```
/mlir-opt --xegpu-subgroup-distribute test.mlir
```
```
No matching uArch found
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:274!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --xegpu-subgroup-distribute test.mlir
 #0 0x00005958f78fbe32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005958f78f85af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005958f78f86fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000758776e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000758776e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000758776e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000758776e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000758776e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000758776e288ff abort ./stdlib/abort.c:81:7
 #9 0x00005958f78db12e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x00005958fb49151c (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&) const XeGPUSubgroupDistribute.cpp:0:0
#11 0x00005958ff126e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#12 0x00005958ff1283af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#13 0x00005958ff0ed3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#14 0x00005958ff0f2060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#15 0x00005958fb495d44 (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() XeGPUSubgroupDistribute.cpp:0:0
#16 0x00005958ff19b756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#17 0x00005958ff19ba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#18 0x00005958ff19c1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#19 0x00005958ff19d400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#20 0x00005958f79c2767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x00005958f79c34ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#22 0x00005958ff56b731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#23 0x00005958f79ba8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#24 0x00005958f79c3b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#25 0x00005958f79c3da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#26 0x00005958f77f5023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#27 0x0000758776e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x0000758776e2a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x0000758776e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x00005958f78da405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
Bug Pass: --xegpu-subgroup-distribute

Bug Op: gpu.func

Description: The XeGPUSubgroupDistribute pass crashes with an UNREACHABLE error when it cannot resolve a matching Intel GPU micro-architecture (uArch) for a function body. This is triggered when a gpu.func contains operations from incompatible dialects (like rocdl) or lacks target-specific attributes, causing the uArch dispatcher to abort rather than fail gracefully.

```mlir
module {
  gpu.module @<!-- -->main_module {
    spirv.EntryPoint "GLCompute" @<!-- -->main
    gpu.func @<!-- -->main() -&gt; index {
      %0 = index.sizeof
      %1 = gpu.thread_id  x
      %2 = rocdl.wavefrontsize : i32
      gpu.return %0 : index
    }
  }
}
```
```
/mlir-opt --xegpu-subgroup-distribute test.mlir
```
```
No matching uArch found
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h:274!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --xegpu-subgroup-distribute test.mlir
 #<!-- -->0 0x00005958f78fbe32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005958f78f85af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005958f78f86fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000758776e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000758776e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000758776e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000758776e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000758776e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000758776e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00005958f78db12e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x00005958fb49151c (anonymous namespace)::MoveFuncBodyToWarpOp::matchAndRewrite(mlir::gpu::GPUFuncOp, mlir::PatternRewriter&amp;) const XeGPUSubgroupDistribute.cpp:0:0
#<!-- -->11 0x00005958ff126e03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
#<!-- -->12 0x00005958ff1283af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
#<!-- -->13 0x00005958ff0ed3c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->14 0x00005958ff0f2060 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#<!-- -->15 0x00005958fb495d44 (anonymous namespace)::XeGPUSubgroupDistributePass::runOnOperation() XeGPUSubgroupDistribute.cpp:0:0
#<!-- -->16 0x00005958ff19b756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->17 0x00005958ff19ba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->18 0x00005958ff19c1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#<!-- -->19 0x00005958ff19d400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#<!-- -->20 0x00005958f79c2767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x00005958f79c34ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x00005958ff56b731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->23 0x00005958f79ba8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->24 0x00005958f79c3b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->25 0x00005958f79c3da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->26 0x00005958f77f5023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->27 0x0000758776e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->28 0x0000758776e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x0000758776e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->30 0x00005958f78da405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

