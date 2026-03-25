# [MLIR] A GPU dialect program triggers a crash bug in the matchPattern

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/181305
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-18T12:53:46Z

## Body

Hi, I found a crash bug. See the following code example.

#### Code Example
```llvm
module {
  gpu.module @myKernel_module {
    gpu.func @myKernel(%data: memref<?xi32>) kernel attributes {sym_visibility = "private"} {
      %blockIdx_x = gpu.block_id x
      %blockDim_x = gpu.block_dim x
      %threadIdx_x = gpu.thread_id x
      %mul_temp = arith.muli %blockIdx_x, %blockDim_x : index
      %tid_index = arith.addi %mul_temp, %threadIdx_x : index
      %c0 = arith.constant 0 : index
      %len = memref.dim %data, %c0 : memref<?xi32>
      %within_bounds = arith.cmpi ult, %tid_index, %len : index
      scf.if %within_bounds {
        %tid_i32 = arith.index_cast %tid_index : index to i32
        memref.store %tid_i32, %data[%tid_index] : memref<?xi32>
        gpu.barrier
      } else {
        arith.addi %c0, %c0 : index
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --remove-dead-values --convert-gpu-to-nvvm
 #0 0x000063b931d1887d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x000063b931d18dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x000063b931d16c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000063b931d19449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x0000771319245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000077131929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000077131929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000077131929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000077131924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007713192288ff abort ./stdlib/abort.c:81:7
#10 0x000077131922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000077131923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000063b932192920 bool mlir::matchPattern<mlir::detail::constant_op_binder<mlir::Attribute>>(mlir::Value, mlir::detail::constant_op_binder<mlir::Attribute> const&) /mlir/include/mlir/IR/Matchers.h:493:24
#13 0x000063b940676801 mlir::Operation::fold(llvm::SmallVectorImpl<mlir::OpFoldResult>&) /mlir/lib/IR/Operation.cpp:661:54
#14 0x000063b9400874fe (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:498:25
#15 0x000063b94008528b (anonymous namespace)::MultiOpPatternRewriteDriver::simplify(llvm::ArrayRef<mlir::Operation*>, bool*) && /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:993:8
#16 0x000063b940084dca mlir::applyOpPatternsGreedily(llvm::ArrayRef<mlir::Operation*>, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:1066:47
#17 0x000063b93ffd843d (anonymous namespace)::RemoveDeadValues::runOnOperation() /mlir/lib/Transforms/RemoveDeadValues.cpp:789:14
#18 0x000063b940191e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#19 0x000063b940191e15 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#20 0x000063b931d3ea19 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#21 0x000063b94019531b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#22 0x000063b9401892be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#23 0x000063b9401899da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#24 0x000063b94018f2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#25 0x000063b94018e846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#26 0x000063b931e22dfb performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#27 0x000063b931e224d1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#28 0x000063b931e2218c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#29 0x000063b931e22096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#30 0x000063b9406eec5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#31 0x000063b9406ee01a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#32 0x000063b931e1de16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#33 0x000063b931e1e215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#34 0x000063b931e1e3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#35 0x000063b931ced485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#36 0x000077131922a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#37 0x000077131922a28b call_init ./csu/../csu/libc-start.c:128:20
#38 0x000077131922a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#39 0x000063b931ced2f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --remove-dead-values --convert-gpu-to-nvvm`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
Hi, I found a crash bug. See the following code example.

#### Code Example
```llvm
module {
  gpu.module @<!-- -->myKernel_module {
    gpu.func @<!-- -->myKernel(%data: memref&lt;?xi32&gt;) kernel attributes {sym_visibility = "private"} {
      %blockIdx_x = gpu.block_id x
      %blockDim_x = gpu.block_dim x
      %threadIdx_x = gpu.thread_id x
      %mul_temp = arith.muli %blockIdx_x, %blockDim_x : index
      %tid_index = arith.addi %mul_temp, %threadIdx_x : index
      %c0 = arith.constant 0 : index
      %len = memref.dim %data, %c0 : memref&lt;?xi32&gt;
      %within_bounds = arith.cmpi ult, %tid_index, %len : index
      scf.if %within_bounds {
        %tid_i32 = arith.index_cast %tid_index : index to i32
        memref.store %tid_i32, %data[%tid_index] : memref&lt;?xi32&gt;
        gpu.barrier
      } else {
        arith.addi %c0, %c0 : index
      }
      gpu.return
    }
  }
}
```

#### Stack Trace
```bash
mlir-opt: /mlir/include/mlir/IR/Matchers.h:491: bool mlir::matchPattern(Value, const Pattern &amp;) [Pattern = mlir::detail::constant_op_binder&lt;mlir::Attribute&gt;]: Assertion `value' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --remove-dead-values --convert-gpu-to-nvvm
 #<!-- -->0 0x000063b931d1887d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x000063b931d18dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x000063b931d16c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000063b931d19449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x0000771319245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000077131929eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000077131929eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000077131929eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000077131924527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007713192288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000077131922881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000077131923b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000063b932192920 bool mlir::matchPattern&lt;mlir::detail::constant_op_binder&lt;mlir::Attribute&gt;&gt;(mlir::Value, mlir::detail::constant_op_binder&lt;mlir::Attribute&gt; const&amp;) /mlir/include/mlir/IR/Matchers.h:493:24
#<!-- -->13 0x000063b940676801 mlir::Operation::fold(llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) /mlir/lib/IR/Operation.cpp:661:54
#<!-- -->14 0x000063b9400874fe (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:498:25
#<!-- -->15 0x000063b94008528b (anonymous namespace)::MultiOpPatternRewriteDriver::simplify(llvm::ArrayRef&lt;mlir::Operation*&gt;, bool*) &amp;&amp; /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:993:8
#<!-- -->16 0x000063b940084dca mlir::applyOpPatternsGreedily(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*, bool*) /mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:1066:47
#<!-- -->17 0x000063b93ffd843d (anonymous namespace)::RemoveDeadValues::runOnOperation() /mlir/lib/Transforms/RemoveDeadValues.cpp:789:14
#<!-- -->18 0x000063b940191e74 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->19 0x000063b940191e15 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x000063b931d3ea19 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x000063b94019531b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x000063b9401892be mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->23 0x000063b9401899da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->24 0x000063b94018f2bd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->25 0x000063b94018e846 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->26 0x000063b931e22dfb performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->27 0x000063b931e224d1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->28 0x000063b931e2218c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->29 0x000063b931e22096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x000063b9406eec5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->31 0x000063b9406ee01a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->32 0x000063b931e1de16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->33 0x000063b931e1e215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->34 0x000063b931e1e3e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->35 0x000063b931ced485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->36 0x000077131922a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->37 0x000077131922a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->38 0x000077131922a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->39 0x000063b931ced2f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

I used the command: `mlir-opt input.mlir --remove-dead-values --convert-gpu-to-nvvm`
</details>


---

### Comment 2 - meshtag

This was fixed by https://github.com/llvm/llvm-project/pull/181013.

---

