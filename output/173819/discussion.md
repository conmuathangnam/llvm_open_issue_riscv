# mlir-opt --xegpu-blocking crashes in getLayoutAttr with affine.for iter_args + tensor.extract

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/173819
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-08T09:22:06Z

## Body



## Description:
The XeGPU pass assumes all tensors have a XeGPU layout attribute. In an affine.for with iter_args, calling tensor.extract on a plain tensor without a layout causes getLayoutAttr to dereference null, leading to a crash.

If the `%3 = tensor.extract %acc[%arg0] : tensor<4xi32>` operation is commented out, the crash no longer occurs.

## Reproduce
mlir-opt version: 21.1.8

```mlir
module {
  func.func @main() {
    %tensor = scf.execute_region -> tensor<4xi32> {
      %c0 = arith.constant 0 : i32
      %c1 = arith.constant 1 : i32
      %c2 = arith.constant 2 : i32
      %c3 = arith.constant 3 : i32
      %tensor_val = tensor.from_elements %c0, %c1, %c2, %c3 : tensor<4xi32>
      scf.yield %tensor_val : tensor<4xi32>
    }
    %c2 = arith.constant 2 : index
    %2 = affine.for %arg0 = 0 to %c2 iter_args(%acc = %tensor) -> (tensor<4xi32>) {
      %3 = tensor.extract %acc[%arg0] : tensor<4xi32>
      affine.yield %acc : tensor<4xi32>
    }
    %c-1_i32 = arith.constant -1 : i32
    return
  }
}
```
```
mlir-opt --xegpu-blocking crash.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt --xegpu-blocking /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250108_17d7_5.mlir
 #0 0x00005a3c1c466612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x20a1612)
 #1 0x00005a3c1c462d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x209dd7f)
 #2 0x00005a3c1c462ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b5b3cc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00005a3c1c375ebe mlir::xegpu::getLayoutAttr(mlir::Value) (.cold) XeGPUUtils.cpp:0:0
 #5 0x00005a3c1f553bef void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #6 0x00005a3c1f553ba8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #7 0x00005a3c1f553ba8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #8 0x00005a3c1f553ba8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #9 0x00005a3c1f553dc8 mlir::xegpu::setLayoutAttrs(mlir::Operation*, llvm::function_ref<mlir::xegpu::LayoutAttr (mlir::Value)>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x518edc8)
#10 0x00005a3c1f51bd32 (anonymous namespace)::XeGPUBlockingPass::runOnOperation() XeGPUBlocking.cpp:0:0
#11 0x00005a3c1fc1b07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x585607e)
#12 0x00005a3c1fc1b4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x58564c1)
#13 0x00005a3c1fc1c523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5857523)
#14 0x00005a3c1fc0c0a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#15 0x00005a3c1fc0c9c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#16 0x00005a3c1fc0cb8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#17 0x00005a3c1fd2224e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x595d24e)
#18 0x00005a3c1fc05080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5840080)
#19 0x00005a3c1fc0ccf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5847cf0)
#20 0x00005a3c1fc0d1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x58481ef)
#21 0x00005a3c1c37e577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x1fb9577)
#22 0x00007b5b3cc2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007b5b3cc2a28b call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007b5b3cc2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x00005a3c1c437665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x2072665)
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>


## Description:
The XeGPU pass assumes all tensors have a XeGPU layout attribute. In an affine.for with iter_args, calling tensor.extract on a plain tensor without a layout causes getLayoutAttr to dereference null, leading to a crash.

If the `%3 = tensor.extract %acc[%arg0] : tensor&lt;4xi32&gt;` operation is commented out, the crash no longer occurs.

## Reproduce
mlir-opt version: 21.1.8

```mlir
module {
  func.func @<!-- -->main() {
    %tensor = scf.execute_region -&gt; tensor&lt;4xi32&gt; {
      %c0 = arith.constant 0 : i32
      %c1 = arith.constant 1 : i32
      %c2 = arith.constant 2 : i32
      %c3 = arith.constant 3 : i32
      %tensor_val = tensor.from_elements %c0, %c1, %c2, %c3 : tensor&lt;4xi32&gt;
      scf.yield %tensor_val : tensor&lt;4xi32&gt;
    }
    %c2 = arith.constant 2 : index
    %2 = affine.for %arg0 = 0 to %c2 iter_args(%acc = %tensor) -&gt; (tensor&lt;4xi32&gt;) {
      %3 = tensor.extract %acc[%arg0] : tensor&lt;4xi32&gt;
      affine.yield %acc : tensor&lt;4xi32&gt;
    }
    %c-1_i32 = arith.constant -1 : i32
    return
  }
}
```
```
mlir-opt --xegpu-blocking crash.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt --xegpu-blocking /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250108_17d7_5.mlir
 #<!-- -->0 0x00005a3c1c466612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x00005a3c1c462d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x00005a3c1c462ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b5b3cc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00005a3c1c375ebe mlir::xegpu::getLayoutAttr(mlir::Value) (.cold) XeGPUUtils.cpp:0:0
 #<!-- -->5 0x00005a3c1f553bef void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #<!-- -->6 0x00005a3c1f553ba8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #<!-- -->7 0x00005a3c1f553ba8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #<!-- -->8 0x00005a3c1f553ba8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) XeGPUUtils.cpp:0:0
 #<!-- -->9 0x00005a3c1f553dc8 mlir::xegpu::setLayoutAttrs(mlir::Operation*, llvm::function_ref&lt;mlir::xegpu::LayoutAttr (mlir::Value)&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x518edc8)
#<!-- -->10 0x00005a3c1f51bd32 (anonymous namespace)::XeGPUBlockingPass::runOnOperation() XeGPUBlocking.cpp:0:0
#<!-- -->11 0x00005a3c1fc1b07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x585607e)
#<!-- -->12 0x00005a3c1fc1b4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x58564c1)
#<!-- -->13 0x00005a3c1fc1c523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5857523)
#<!-- -->14 0x00005a3c1fc0c0a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->15 0x00005a3c1fc0c9c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->16 0x00005a3c1fc0cb8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->17 0x00005a3c1fd2224e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x595d24e)
#<!-- -->18 0x00005a3c1fc05080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5840080)
#<!-- -->19 0x00005a3c1fc0ccf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x5847cf0)
#<!-- -->20 0x00005a3c1fc0d1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x58481ef)
#<!-- -->21 0x00005a3c1c37e577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x1fb9577)
#<!-- -->22 0x00007b5b3cc2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007b5b3cc2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007b5b3cc2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x00005a3c1c437665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8-install/bin/mlir-opt+0x2072665)
Segmentation fault (core dumped)
```
</details>


---

### Comment 2 - CoTinker

This crash maybe fixed.

---

