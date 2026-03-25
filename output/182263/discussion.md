# [MLIR] Optimization crash in processSimpleOp triggered by gpu program

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/182263
**Status:** Open
**Labels:** mlir, crash

## Body

I found a crash bug triggered by the `--remove-dead-values` optimization. Please see the following code example.

```llvm
module {
  func.func @main() -> i64 {
    %c0 = arith.constant 0 : i64
    %c1 = arith.constant 1 : i64
    %sum = arith.addi %c0, %c1 : i64
    %c2 = arith.constant 2 : i64
    %mul = arith.muli %sum, %c2 : i64
    %c3 = arith.constant 3 : i64
    %div = arith.divsi %mul, %c3 : i64
    %c4 = arith.constant 4 : i64
    %sub = arith.subi %div, %c4 : i64
    %c5 = arith.constant 5 : i64
    %cmp = arith.cmpi slt, %sub, %c5 : i64
    cf.cond_br %cmp, ^bb1, ^bb2

  ^bb1:
    %c10 = arith.constant 10 : i64
    cf.br ^bb3(%c10 : i64)

  ^bb2:
    %c20 = arith.constant 20 : i64
    cf.br ^bb3(%c20 : i64)

  ^bb3(%arg0: i64):
    %c1_index = arith.constant 1 : index

    gpu.launch
      blocks(%bx, %by, %bz) in (%grid_x = %c1_index, %grid_y = %c1_index, %grid_z = %c1_index)
      threads(%tx, %ty, %tz) in (%block_x = %c1_index, %block_y = %c1_index, %block_z = %c1_index) {
      %blk_x = gpu.block_id x
      %thr_x = gpu.thread_id x
      gpu.terminator
    }

    %final = arith.addi %sub, %arg0 : i64
    func.return %final : i64
  }
}
```

#### Stack Trace
```bash
mlir-opt: /mlir/lib/Transforms/RemoveDeadValues.cpp:243: void (anonymous namespace)::processSimpleOp(Operation *, RunLivenessAnalysis &, DenseSet<Value> &, RDVFinalCleanupList &): Assertion `!hasLive(op->getResults(), nonLiveSet, la) && "expected the op to have no live results"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /bin/mlir-opt test.mlir --remove-dead-values
 #0 0x000061238621e1fd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000061238621e72b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000061238621c5c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000061238621eec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007f1b07a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f1b07a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f1b07a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f1b07a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f1b07a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f1b07a288ff abort ./stdlib/abort.c:81:7
#10 0x00007f1b07a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f1b07a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006123946e0e34 (anonymous namespace)::processSimpleOp(mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&, llvm::DenseSet<mlir::Value, llvm::DenseMapInfo<mlir::Value, void>>&, (anonymous namespace)::RDVFinalCleanupList&) /mlir/lib/Transforms/RemoveDeadValues.cpp:244:5
#13 0x00006123946df3cf (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0::operator()(mlir::Operation*) const /mlir/lib/Transforms/RemoveDeadValues.cpp:0:7
#14 0x00006123946df26d void llvm::function_ref<void (mlir::Operation*)>::callback_fn<(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0>(long, mlir::Operation*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#15 0x00006123863ccb01 llvm::function_ref<void (mlir::Operation*)>::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#16 0x00006123863ccacd void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:153:1
#17 0x00006123863cca89 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:145:27
#18 0x00006123863cca89 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:145:27
#19 0x00006123946df212 std::enable_if<llvm::is_one_of<mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, mlir::Operation*, void>(mlir::Operation*, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&&) /mlir/include/mlir/IR/Visitors.h:278:3
#20 0x00006123946dcbad std::enable_if<llvm::function_traits<std::decay<(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, void>((anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&&) /mlir/include/mlir/IR/Operation.h:798:5
#21 0x00006123946dc647 (anonymous namespace)::RemoveDeadValues::runOnOperation() /mlir/lib/Transforms/RemoveDeadValues.cpp:793:26
#22 0x0000612394898734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#23 0x00006123948986d5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#24 0x00006123862445c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#25 0x000061239489bbdb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#26 0x000061239488fb7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#27 0x000061239489029a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#28 0x0000612394895b7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#29 0x0000612394895106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#30 0x0000612386328b4b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#31 0x0000612386328221 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#32 0x0000612386327edc mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#33 0x0000612386327de6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#34 0x0000612394dfa88a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#35 0x0000612394df9c4a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#36 0x0000612386323b66 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#37 0x0000612386323f68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#38 0x0000612386324138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#39 0x00006123861f2de5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#40 0x00007f1b07a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#41 0x00007f1b07a2a28b call_init ./csu/../csu/libc-start.c:128:20
#42 0x00007f1b07a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#43 0x00006123861f2c55 _start (./llvm/llvm-project-7dbfc2/build_all/bin/mlir-opt+0x4742c55)
Aborted
```

Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
I found a crash bug triggered by the `--remove-dead-values` optimization. Please see the following code example.

```llvm
module {
  func.func @<!-- -->main() -&gt; i64 {
    %c0 = arith.constant 0 : i64
    %c1 = arith.constant 1 : i64
    %sum = arith.addi %c0, %c1 : i64
    %c2 = arith.constant 2 : i64
    %mul = arith.muli %sum, %c2 : i64
    %c3 = arith.constant 3 : i64
    %div = arith.divsi %mul, %c3 : i64
    %c4 = arith.constant 4 : i64
    %sub = arith.subi %div, %c4 : i64
    %c5 = arith.constant 5 : i64
    %cmp = arith.cmpi slt, %sub, %c5 : i64
    cf.cond_br %cmp, ^bb1, ^bb2

  ^bb1:
    %c10 = arith.constant 10 : i64
    cf.br ^bb3(%c10 : i64)

  ^bb2:
    %c20 = arith.constant 20 : i64
    cf.br ^bb3(%c20 : i64)

  ^bb3(%arg0: i64):
    %c1_index = arith.constant 1 : index

    gpu.launch
      blocks(%bx, %by, %bz) in (%grid_x = %c1_index, %grid_y = %c1_index, %grid_z = %c1_index)
      threads(%tx, %ty, %tz) in (%block_x = %c1_index, %block_y = %c1_index, %block_z = %c1_index) {
      %blk_x = gpu.block_id x
      %thr_x = gpu.thread_id x
      gpu.terminator
    }

    %final = arith.addi %sub, %arg0 : i64
    func.return %final : i64
  }
}
```

#### Stack Trace
```bash
mlir-opt: /mlir/lib/Transforms/RemoveDeadValues.cpp:243: void (anonymous namespace)::processSimpleOp(Operation *, RunLivenessAnalysis &amp;, DenseSet&lt;Value&gt; &amp;, RDVFinalCleanupList &amp;): Assertion `!hasLive(op-&gt;getResults(), nonLiveSet, la) &amp;&amp; "expected the op to have no live results"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /bin/mlir-opt test.mlir --remove-dead-values
 #<!-- -->0 0x000061238621e1fd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000061238621e72b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000061238621c5c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000061238621eec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007f1b07a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f1b07a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f1b07a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f1b07a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f1b07a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f1b07a288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f1b07a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f1b07a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006123946e0e34 (anonymous namespace)::processSimpleOp(mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&amp;, llvm::DenseSet&lt;mlir::Value, llvm::DenseMapInfo&lt;mlir::Value, void&gt;&gt;&amp;, (anonymous namespace)::RDVFinalCleanupList&amp;) /mlir/lib/Transforms/RemoveDeadValues.cpp:244:5
#<!-- -->13 0x00006123946df3cf (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0::operator()(mlir::Operation*) const /mlir/lib/Transforms/RemoveDeadValues.cpp:0:7
#<!-- -->14 0x00006123946df26d void llvm::function_ref&lt;void (mlir::Operation*)&gt;::callback_fn&lt;(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&gt;(long, mlir::Operation*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->15 0x00006123863ccb01 llvm::function_ref&lt;void (mlir::Operation*)&gt;::operator()(mlir::Operation*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->16 0x00006123863ccacd void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:153:1
#<!-- -->17 0x00006123863cca89 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->18 0x00006123863cca89 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->19 0x00006123946df212 std::enable_if&lt;llvm::is_one_of&lt;mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, mlir::Operation*, void&gt;(mlir::Operation*, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&amp;&amp;) /mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->20 0x00006123946dcbad std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, void&gt;((anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&amp;&amp;) /mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->21 0x00006123946dc647 (anonymous namespace)::RemoveDeadValues::runOnOperation() /mlir/lib/Transforms/RemoveDeadValues.cpp:793:26
#<!-- -->22 0x0000612394898734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->23 0x00006123948986d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->24 0x00006123862445c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->25 0x000061239489bbdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->26 0x000061239488fb7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->27 0x000061239489029a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->28 0x0000612394895b7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->29 0x0000612394895106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->30 0x0000612386328b4b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->31 0x0000612386328221 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->32 0x0000612386327edc mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->33 0x0000612386327de6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->34 0x0000612394dfa88a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->35 0x0000612394df9c4a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->36 0x0000612386323b66 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->37 0x0000612386323f68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->38 0x0000612386324138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->39 0x00006123861f2de5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->40 0x00007f1b07a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->41 0x00007f1b07a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->42 0x00007f1b07a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->43 0x00006123861f2c55 _start (./llvm/llvm-project-7dbfc2/build_all/bin/mlir-opt+0x4742c55)
Aborted
```

Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77
</details>


---

### Comment 2 - felichita

I'd like to work on a fix for this. Looks like processSimpleOp incorrectly handles ops with nested regions like gpu-launch

---

