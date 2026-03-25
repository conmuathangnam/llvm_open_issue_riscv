# [MLIR] A crash bug during dialect lowering

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/171827
**Status:** Closed
**Labels:** duplicate, mlir, crash
**Closed Date:** 2025-12-11T13:35:49Z

## Body

When compiling the following program with MLIR, I encountered an unexpected crash. The input program is provided below.

### Input Program
```llvm
// input.mlir
gpu.module @cuda_events {
  gpu.func @increment_kernel(%data: memref<?xi32>, %increment: i32)
      kernel attributes {known_block_size = [512, 1, 1], known_grid_size = [32768, 1, 1]} {
    %tid = gpu.global_id x
    %existing_value = memref.load %data[%tid] : memref<?xi32>
    %updated_value = arith.addi %existing_value, %increment : i32
    memref.store %updated_value, %data[%tid] : memref<?xi32>
    gpu.return
  }
}
```

### Used Command
To reproduce the issue, I used the following command:
```bash
/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt /home/username/evaluation/input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
```

### Stack Trace
The crash bug produced the following stack trace:
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/username/projects/llvm-project/build_nvptx/bin/mlir-opt /home/username/evaluation/input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
 #0 0x000055fc7e058a52 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x2010a52)
 #1 0x000055fc7e055732 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f234b845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x000055fc84976f94 mlir::detail::OperandStorage::OperandStorage(mlir::Operation*, mlir::OpOperand*, mlir::ValueRange) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x892ef94)
 #4 0x000055fc84967904 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x891f904)
 #5 0x000055fc84967c0a mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x891fc0a)
 #6 0x000055fc8496817c mlir::Operation::create(mlir::OperationState const&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x892017c)
 #7 0x000055fc848c0213 mlir::OpBuilder::create(mlir::OperationState const&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8878213)
 #8 0x000055fc82d93af0 mlir::index::AddOp::create(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::Value) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x6d4baf0)
 #9 0x000055fc7f6cc180 mlir::detail::OpOrInterfaceRewritePatternBase<mlir::gpu::GlobalIdOp>::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x3684180)
#10 0x000055fc8461c91c mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x85d491c)
#11 0x000055fc845d69e7 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#12 0x000055fc845d8275 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8590275)
#13 0x000055fc7f2cf544 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() LowerGpuOpsToNVVMOps.cpp:0:0
#14 0x000055fc8466d5ae mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86255ae)
#15 0x000055fc8466d9c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#16 0x000055fc8466ca70 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8624a70)
#17 0x000055fc8466d32e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862532e)
#18 0x000055fc8466d9c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#19 0x000055fc8466e81b mlir::PassManager::run(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862681b)
#20 0x000055fc7e1055ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x000055fc7e106bee llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#22 0x000055fc849c5d08 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x897dd08)
#23 0x000055fc7e101bc7 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#24 0x000055fc7e107288 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf288)
#25 0x000055fc7e1074ff mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf4ff)
#26 0x000055fc7df4e77f main (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1f0677f)
#27 0x00007f234b82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007f234b82a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007f234b82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x000055fc7e034f35 _start (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1fecf35)
Segmentation fault
```

### Version
My MLIR version is:
```bash
./mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: clearlove7 (clearlove2077)

<details>
When compiling the following program with MLIR, I encountered an unexpected crash. The input program is provided below.

### Input Program
```llvm
// input.mlir
gpu.module @<!-- -->cuda_events {
  gpu.func @<!-- -->increment_kernel(%data: memref&lt;?xi32&gt;, %increment: i32)
      kernel attributes {known_block_size = [512, 1, 1], known_grid_size = [32768, 1, 1]} {
    %tid = gpu.global_id x
    %existing_value = memref.load %data[%tid] : memref&lt;?xi32&gt;
    %updated_value = arith.addi %existing_value, %increment : i32
    memref.store %updated_value, %data[%tid] : memref&lt;?xi32&gt;
    gpu.return
  }
}
```

### Used Command
To reproduce the issue, I used the following command:
```bash
/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt /home/username/evaluation/input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
```

### Stack Trace
The crash bug produced the following stack trace:
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/username/projects/llvm-project/build_nvptx/bin/mlir-opt /home/username/evaluation/input.mlir -gpu-kernel-outlining --convert-scf-to-cf --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -allow-unregistered-dialect -reconcile-unrealized-casts
 #<!-- -->0 0x000055fc7e058a52 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x2010a52)
 #<!-- -->1 0x000055fc7e055732 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f234b845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x000055fc84976f94 mlir::detail::OperandStorage::OperandStorage(mlir::Operation*, mlir::OpOperand*, mlir::ValueRange) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x892ef94)
 #<!-- -->4 0x000055fc84967904 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x891f904)
 #<!-- -->5 0x000055fc84967c0a mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x891fc0a)
 #<!-- -->6 0x000055fc8496817c mlir::Operation::create(mlir::OperationState const&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x892017c)
 #<!-- -->7 0x000055fc848c0213 mlir::OpBuilder::create(mlir::OperationState const&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8878213)
 #<!-- -->8 0x000055fc82d93af0 mlir::index::AddOp::create(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::Value) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x6d4baf0)
 #<!-- -->9 0x000055fc7f6cc180 mlir::detail::OpOrInterfaceRewritePatternBase&lt;mlir::gpu::GlobalIdOp&gt;::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x3684180)
#<!-- -->10 0x000055fc8461c91c mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x85d491c)
#<!-- -->11 0x000055fc845d69e7 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->12 0x000055fc845d8275 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8590275)
#<!-- -->13 0x000055fc7f2cf544 (anonymous namespace)::LowerGpuOpsToNVVMOpsPass::runOnOperation() LowerGpuOpsToNVVMOps.cpp:0:0
#<!-- -->14 0x000055fc8466d5ae mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86255ae)
#<!-- -->15 0x000055fc8466d9c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->16 0x000055fc8466ca70 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8624a70)
#<!-- -->17 0x000055fc8466d32e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862532e)
#<!-- -->18 0x000055fc8466d9c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->19 0x000055fc8466e81b mlir::PassManager::run(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862681b)
#<!-- -->20 0x000055fc7e1055ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x000055fc7e106bee llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x000055fc849c5d08 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x897dd08)
#<!-- -->23 0x000055fc7e101bc7 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->24 0x000055fc7e107288 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf288)
#<!-- -->25 0x000055fc7e1074ff mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf4ff)
#<!-- -->26 0x000055fc7df4e77f main (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1f0677f)
#<!-- -->27 0x00007f234b82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->28 0x00007f234b82a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x00007f234b82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->30 0x000055fc7e034f35 _start (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1fecf35)
Segmentation fault
```

### Version
My MLIR version is:
```bash
./mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build.
```
</details>


---

### Comment 2 - joker-eph

It's a duplicate of #171303 I believe

---

### Comment 3 - jiayiw2008

@joker-eph Hi Mehdi, the stack traces for these two bugs look significantly different. Could you please explain why they’ve been identified as the same issue?

---

### Comment 4 - linuxlonelyeagle

> [@joker-eph](https://github.com/joker-eph) Hi Mehdi, the stack traces for these two bugs look significantly different. Could you please explain why they’ve been identified as the same issue?

You can see `mlir::index::AddOp::create` in your report. And you also can see this https://github.com/llvm/llvm-project/pull/171614 and If my guess is correct, I shouldn't use index daielct here anymore.The issue has naturally been fixed.

---

