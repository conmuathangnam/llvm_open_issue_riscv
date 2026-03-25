# mlir-opt crash: Assertion failure in ArmSME tile allocation due to nested regions

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/181593
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-19T19:40:41Z

## Body

Bug Pass: --test-arm-sme-tile-allocation

Bug Op: arm_sme.get_tile

Description:
The ArmSME tile allocation pass crashes when tile-related operations (e.g., arm_sme.get_tile) are placed inside nested regions like scf.index_switch. The pass's internal numbering logic assumes a flat operation list and explicitly triggers an assertion when it detects an operation residing within a nested block.

```mlir
module {
  func.func @main() {
    %0 = index.constant 0
    %1 = arm_sme.get_tile : vector<[8]x[8]xi16>
    %2 = scf.index_switch %0 -> vector<[8]x[8]xi16> default {
      %3 = arm_sme.get_tile : vector<[8]x[8]xi16>
      scf.yield %3 : vector<[8]x[8]xi16>
    }
    return
  }
}
```
```
mlir-opt --test-arm-sme-tile-allocation test.mlir
```
```
mlir-opt: mlir/lib/Dialect/ArmSME/Transforms/TileAllocation.cpp:356: {anonymous}::generateOperationNumbering(mlir::FunctionOpInterface)::<lambda(mlir::arm_sme::ArmSMETileOpInterface)>: Assertion `&op == nestedOp.getOperation() && "ArmSME tile allocation does not support nested regions"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --test-arm-sme-tile-allocation test.mlir
 #0 0x00005ec9d2609e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x00005ec9d26065af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x00005ec9d26066fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007244b8245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007244b829eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007244b829eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007244b829eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007244b824527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007244b82288ff abort ./stdlib/abort.c:81:7
 #9 0x00007244b822881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007244b823b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005ec9d3c9429d (mlir-opt+0x3bba29d)
#12 0x00005ec9d3c94194 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) TileAllocation.cpp:0:0
#13 0x00005ec9d3c9edba mlir::arm_sme::allocateSMETiles(mlir::FunctionOpInterface, bool) (mlir-opt+0x3bc4dba)
#14 0x00005ec9d3ca172a (anonymous namespace)::TestTileAllocationPass::runOnOperation() TileAllocation.cpp:0:0
#15 0x00005ec9d9ea9756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#16 0x00005ec9d9ea9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#17 0x00005ec9d9eab673 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const Pass.cpp:0:0
#18 0x00005ec9d9ea872d mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (mlir-opt+0x9dce72d)
#19 0x00005ec9d9ea95f0 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf5f0)
#20 0x00005ec9d9ea9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#21 0x00005ec9d9eaa1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#22 0x00005ec9d9eab400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#23 0x00005ec9d26d0767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#24 0x00005ec9d26d14ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#25 0x00005ec9da279731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#26 0x00005ec9d26c88d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#27 0x00005ec9d26d1b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7b70)
#28 0x00005ec9d26d1da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7da9)
#29 0x00005ec9d2503023 main (mlir-opt+0x2429023)
#30 0x00007244b822a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x00007244b822a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x00007244b822a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00005ec9d25e8405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
Bug Pass: --test-arm-sme-tile-allocation

Bug Op: arm_sme.get_tile

Description:
The ArmSME tile allocation pass crashes when tile-related operations (e.g., arm_sme.get_tile) are placed inside nested regions like scf.index_switch. The pass's internal numbering logic assumes a flat operation list and explicitly triggers an assertion when it detects an operation residing within a nested block.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = index.constant 0
    %1 = arm_sme.get_tile : vector&lt;[8]x[8]xi16&gt;
    %2 = scf.index_switch %0 -&gt; vector&lt;[8]x[8]xi16&gt; default {
      %3 = arm_sme.get_tile : vector&lt;[8]x[8]xi16&gt;
      scf.yield %3 : vector&lt;[8]x[8]xi16&gt;
    }
    return
  }
}
```
```
mlir-opt --test-arm-sme-tile-allocation test.mlir
```
```
mlir-opt: mlir/lib/Dialect/ArmSME/Transforms/TileAllocation.cpp:356: {anonymous}::generateOperationNumbering(mlir::FunctionOpInterface)::&lt;lambda(mlir::arm_sme::ArmSMETileOpInterface)&gt;: Assertion `&amp;op == nestedOp.getOperation() &amp;&amp; "ArmSME tile allocation does not support nested regions"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --test-arm-sme-tile-allocation test.mlir
 #<!-- -->0 0x00005ec9d2609e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x00005ec9d26065af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x00005ec9d26066fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007244b8245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007244b829eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007244b829eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007244b829eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007244b824527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007244b82288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007244b822881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007244b823b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005ec9d3c9429d (mlir-opt+0x3bba29d)
#<!-- -->12 0x00005ec9d3c94194 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) TileAllocation.cpp:0:0
#<!-- -->13 0x00005ec9d3c9edba mlir::arm_sme::allocateSMETiles(mlir::FunctionOpInterface, bool) (mlir-opt+0x3bc4dba)
#<!-- -->14 0x00005ec9d3ca172a (anonymous namespace)::TestTileAllocationPass::runOnOperation() TileAllocation.cpp:0:0
#<!-- -->15 0x00005ec9d9ea9756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#<!-- -->16 0x00005ec9d9ea9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#<!-- -->17 0x00005ec9d9eab673 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const Pass.cpp:0:0
#<!-- -->18 0x00005ec9d9ea872d mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (mlir-opt+0x9dce72d)
#<!-- -->19 0x00005ec9d9ea95f0 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf5f0)
#<!-- -->20 0x00005ec9d9ea9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#<!-- -->21 0x00005ec9d9eaa1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#<!-- -->22 0x00005ec9d9eab400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#<!-- -->23 0x00005ec9d26d0767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x00005ec9d26d14ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->25 0x00005ec9da279731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#<!-- -->26 0x00005ec9d26c88d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->27 0x00005ec9d26d1b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7b70)
#<!-- -->28 0x00005ec9d26d1da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7da9)
#<!-- -->29 0x00005ec9d2503023 main (mlir-opt+0x2429023)
#<!-- -->30 0x00007244b822a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x00007244b822a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x00007244b822a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00005ec9d25e8405 _start (mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

