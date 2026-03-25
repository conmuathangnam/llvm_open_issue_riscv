# [MLIR] A gpu-dialect program crash in the operation desctructor

**Author:** Zustin
**URL:** https://github.com/llvm/llvm-project/issues/178099
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-17T20:59:47Z

## Body

Hi, I found a crash bug in a gpu program, see the following minmized code example.

### Minimized Program
```llvm
module {
  func.func @main() {
    %c0 = arith.constant 0 : index
    %c3 = arith.constant 3 : index
    cf.br ^bb1
  ^bb1:
    %c1 = arith.constant 1 : index
    %c128 = arith.constant 128 : index
    %c256 = arith.constant 256 : index
    %cond = arith.cmpi eq, %c0, %c3 : index
    cf.cond_br %cond, ^bb2, ^bb4
  ^bb2:
    gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c256, %grid_y = %c1, %grid_z = %c1)
              threads(%tx, %ty, %tz) in (%block_x = %c128, %block_y = %c1, %block_z = %c1) {
      %inner_cst = arith.constant 1.5 : f64
      cf.br ^bb3
    ^bb3:
      gpu.terminator
    }
    cf.br ^bb4
  ^bb4:
    return
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /llvm-project/build_all/bin/mlir-opt input.mlir --test-ir-visitors
 #0 0x00005588274c261d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00005588274c2b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005588274c0a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005588274c31e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f69c5845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f69c589eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f69c589eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f69c589eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f69c584527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f69c58288ff abort ./stdlib/abort.c:81:7
#10 0x000055882749b845 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000055882749b6c2 /llvm-project/llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x0000558835353e42 mlir::Operation::~Operation() /llvm-project/mlir/lib/IR/Operation.cpp:0:5
#13 0x00005588353540fe mlir::Operation::destroy() /llvm-project/mlir/lib/IR/Operation.cpp:214:8
#14 0x0000558835354f85 llvm::ilist_traits<mlir::Operation>::deleteNode(mlir::Operation*) /llvm-project/mlir/lib/IR/Operation.cpp:491:1
#15 0x00005588351e1fb5 llvm::iplist_impl<llvm::simple_ilist<mlir::Operation>, llvm::ilist_traits<mlir::Operation>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>, false, false>) /llvm-project/llvm/include/llvm/ADT/ilist.h:206:12
#16 0x00005588351e1f80 llvm::iplist_impl<llvm::simple_ilist<mlir::Operation>, llvm::ilist_traits<mlir::Operation>>::pop_back() /llvm-project/llvm/include/llvm/ADT/ilist.h:257:25
#17 0x00005588351e0f57 mlir::Block::clear() /llvm-project/mlir/include/mlir/IR/Block.h:44:5
#18 0x00005588351df12f mlir::Block::~Block() /llvm-project/mlir/lib/IR/Block.cpp:22:3
#19 0x00005588282d8b13 llvm::ilist_alloc_traits<mlir::Block>::deleteNode(mlir::Block*) /llvm-project/llvm/include/llvm/ADT/ilist.h:42:39
#20 0x00005588282d8ad5 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>) /llvm-project/llvm/include/llvm/ADT/ilist.h:206:12
#21 0x00005588351e1242 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(mlir::Block*) /llvm-project/llvm/include/llvm/ADT/ilist.h:209:39
#22 0x00005588351df6d5 mlir::Block::erase() /llvm-project/mlir/lib/IR/Block.cpp:68:28
#23 0x00005588282e5c49 testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:188:5
#24 0x00005588282e5bdd void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x00005588276d8b91 llvm::function_ref<void (mlir::Block*)>::operator()(mlir::Block*) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x00005588276d89d8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#27 0x00005588276d89b4 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#28 0x00005588282e5b82 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, mlir::Block*, void>(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#29 0x00005588282e5a1d std::enable_if<llvm::function_traits<std::decay<testNoSkipErasureCallbacks(mlir::Operation*)::$_1&>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, void>(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#30 0x00005588282e373e testNoSkipErasureCallbacks(mlir::Operation*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#31 0x00005588282e30ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#32 0x0000558834e89864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#33 0x0000558834e89805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#34 0x00005588274e8879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#35 0x0000558834e8cf2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#36 0x0000558834e80cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#37 0x0000558834e813da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#38 0x0000558834e86ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#39 0x0000558834e86256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#40 0x00005588275c349b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#41 0x00005588275c2b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#42 0x00005588275c282c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#43 0x00005588275c2736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#44 0x00005588353cbcba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#45 0x00005588353cb07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#46 0x00005588275be126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#47 0x00005588275be525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#48 0x00005588275be6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#49 0x0000558827497935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#50 0x00007f69c582a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#51 0x00007f69c582a28b call_init ./csu/../csu/libc-start.c:128:20
#52 0x00007f69c582a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#53 0x0000558827497505 _start (/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Version
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Zustin)

<details>
Hi, I found a crash bug in a gpu program, see the following minmized code example.

### Minimized Program
```llvm
module {
  func.func @<!-- -->main() {
    %c0 = arith.constant 0 : index
    %c3 = arith.constant 3 : index
    cf.br ^bb1
  ^bb1:
    %c1 = arith.constant 1 : index
    %c128 = arith.constant 128 : index
    %c256 = arith.constant 256 : index
    %cond = arith.cmpi eq, %c0, %c3 : index
    cf.cond_br %cond, ^bb2, ^bb4
  ^bb2:
    gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c256, %grid_y = %c1, %grid_z = %c1)
              threads(%tx, %ty, %tz) in (%block_x = %c128, %block_y = %c1, %block_z = %c1) {
      %inner_cst = arith.constant 1.5 : f64
      cf.br ^bb3
    ^bb3:
      gpu.terminator
    }
    cf.br ^bb4
  ^bb4:
    return
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /llvm-project/build_all/bin/mlir-opt input.mlir --test-ir-visitors
 #<!-- -->0 0x00005588274c261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x00005588274c2b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x00005588274c0a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005588274c31e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f69c5845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f69c589eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f69c589eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f69c589eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f69c584527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f69c58288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000055882749b845 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000055882749b6c2 /llvm-project/llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x0000558835353e42 mlir::Operation::~Operation() /llvm-project/mlir/lib/IR/Operation.cpp:0:5
#<!-- -->13 0x00005588353540fe mlir::Operation::destroy() /llvm-project/mlir/lib/IR/Operation.cpp:214:8
#<!-- -->14 0x0000558835354f85 llvm::ilist_traits&lt;mlir::Operation&gt;::deleteNode(mlir::Operation*) /llvm-project/mlir/lib/IR/Operation.cpp:491:1
#<!-- -->15 0x00005588351e1fb5 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Operation&gt;, llvm::ilist_traits&lt;mlir::Operation&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;, false, false&gt;) /llvm-project/llvm/include/llvm/ADT/ilist.h:206:12
#<!-- -->16 0x00005588351e1f80 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Operation&gt;, llvm::ilist_traits&lt;mlir::Operation&gt;&gt;::pop_back() /llvm-project/llvm/include/llvm/ADT/ilist.h:257:25
#<!-- -->17 0x00005588351e0f57 mlir::Block::clear() /llvm-project/mlir/include/mlir/IR/Block.h:44:5
#<!-- -->18 0x00005588351df12f mlir::Block::~Block() /llvm-project/mlir/lib/IR/Block.cpp:22:3
#<!-- -->19 0x00005588282d8b13 llvm::ilist_alloc_traits&lt;mlir::Block&gt;::deleteNode(mlir::Block*) /llvm-project/llvm/include/llvm/ADT/ilist.h:42:39
#<!-- -->20 0x00005588282d8ad5 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;) /llvm-project/llvm/include/llvm/ADT/ilist.h:206:12
#<!-- -->21 0x00005588351e1242 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(mlir::Block*) /llvm-project/llvm/include/llvm/ADT/ilist.h:209:39
#<!-- -->22 0x00005588351df6d5 mlir::Block::erase() /llvm-project/mlir/lib/IR/Block.cpp:68:28
#<!-- -->23 0x00005588282e5c49 testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:188:5
#<!-- -->24 0x00005588282e5bdd void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x00005588276d8b91 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005588276d89d8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->27 0x00005588276d89b4 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /llvm-project/mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->28 0x00005588282e5b82 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->29 0x00005588282e5a1d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /llvm-project/mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->30 0x00005588282e373e testNoSkipErasureCallbacks(mlir::Operation*) /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:204:3
#<!-- -->31 0x00005588282e30ad (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /llvm-project/mlir/test/lib/IR/TestVisitors.cpp:275:3
#<!-- -->32 0x0000558834e89864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->33 0x0000558834e89805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->34 0x00005588274e8879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->35 0x0000558834e8cf2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->36 0x0000558834e80cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->37 0x0000558834e813da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->38 0x0000558834e86ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->39 0x0000558834e86256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->40 0x00005588275c349b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->41 0x00005588275c2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->42 0x00005588275c282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->43 0x00005588275c2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->44 0x00005588353cbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->45 0x00005588353cb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->46 0x00005588275be126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->47 0x00005588275be525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->48 0x00005588275be6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->49 0x0000558827497935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->50 0x00007f69c582a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x00007f69c582a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->52 0x00007f69c582a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->53 0x0000558827497505 _start (/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Version
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```
</details>


---

### Comment 2 - meshtag

The crash will happen even without the GPU dialect op. See the following IR 
```mlir
module {
  func.func @main() {
    %c0 = arith.constant 0 : index
    %c3 = arith.constant 3 : index
    cf.br ^bb1
  ^bb1:
    %c1 = arith.constant 1 : index
    %c128 = arith.constant 128 : index
    %c256 = arith.constant 256 : index
    %cond = arith.cmpi eq, %c0, %c3 : index
    cf.cond_br %cond, ^bb2, ^bb4
  ^bb2:
    cf.br ^bb4
  ^bb4:
    return
  }
}
```

---

### Comment 3 - meshtag

I have a question regarding the `test-ir-visitors` pass (which is resulting in above crash). 

The crash is coming from the function `testNoSkipErasureCallbacks()`. Here is the relevant useful info revealed by the logs
```
...
Op post-order erasures (no skip)
Erasing op 'arith.constant'
Erasing op 'arith.constant'
Erasing op 'cf.br'
Erasing op 'arith.constant'
Erasing op 'arith.constant'
Erasing op 'arith.constant'
Erasing op 'arith.cmpi'
Erasing op 'cf.cond_br'
Erasing op 'cf.br'
Erasing op 'func.return'
Erasing op 'func.func'
Erasing op 'builtin.module'
Block post-order erasures (no skip)
Erasing block ^bb0 from region 0 from operation 'func.func'
c1.mlir:4:11: error: 'arith.constant' op operation destroyed but still has uses
    %c3 = arith.constant 3 : index
          ^
c1.mlir:4:11: note: see current operation: %0 = "arith.constant"() <{value = 3 : index}> : () -> index
c1.mlir:10:13: note: - use: %3 = "arith.cmpi"(<<UNKNOWN SSA VALUE>>, <<UNKNOWN SSA VALUE>>) <{predicate = 0 : i64}> : (index, index) -> i1

    %cond = arith.cmpi eq, %c0, %c3 : index
            ^
LLVM ERROR: operation destroyed but still has uses
...
```

This is coming because the pass erases the first block inside `main()` [here](https://github.com/llvm/llvm-project/blob/e6560a3c5741612c26cce34275a07f65a90919aa/mlir/test/lib/IR/TestVisitors.cpp#L187). It does the erasure after checking whether the block has any uses [here](https://github.com/llvm/llvm-project/blob/e6560a3c5741612c26cce34275a07f65a90919aa/mlir/test/lib/IR/TestVisitors.cpp#L183). Is this the sufficient thing to check? 

AFAICT, we are checking whether there is something else that points to this block and erase that block if no one is pointing to it. But in this case, that block is providing global scope to other blocks in the function and hence we get a crash after erasing it there (since it still provides information to others even if no one is pointing to it). Should we instead drop all uses of all ops inside the block (if it is providing global context for other blocks like here) and only then erase it? 

CC: @matthias-springer

---

### Comment 4 - matthias-springer

The `test-ir-visitors` pass was meant to test the visitor C++ API, in particular if iterator are incremented correctly. Let's just do the smallest possible fix here, the fewer lines the better. This code will never run in production. Something along the lines of:

```c++
if (block->computeBlockNumber() != 0) {
  emitError() << "pass does not support unstructured control flow";
  return WalkResult::interrupt();
}
```

And then `signalPassError` if `walkResult.wasInterrupted()`.


---

### Comment 5 - meshtag

> Something along the lines of:
> 
> if (block->computeBlockNumber() != 0) {
>   emitError() << "pass does not support unstructured control flow";
>   return WalkResult::interrupt();
> }

I am not sure I follow. The block number for the block whose erasure causes the crash is 0 in the above case, this snippet will not skip that, right? 

I was thinking of checking whether the current block dominates any other block in the op before erasing it. If it does dominate some block, its erasure will cause a crash and we can either 
a. Drop all uses of all ops in current block.
b. Skip the current block (it might eventually get erased when the block that is being dominated is also erased). 

This will involve invoking the expensive dominance analysis in the pass though. Please let me know what you think of it.

---

### Comment 6 - matthias-springer

Ah yes, that won't work. How about putting this at the very beginning of the pass:
```c++
WalkResult status = getOperation()->walk([](Region *r) {
  if (r->getBlocks().size() > 1) {
    r->getParentOp()->emitError("regions with multiple blocks are not supported");
    return WalkResult::interrupt();
  }
});
if (status.wasInterrupted()) {
  signalPassError();
```


---

### Comment 7 - meshtag

The pass does have a test that works on a case where a region has multiple blocks. I am not sure if we should early exit in such cases. Attaching the test IR here for reference
```
func.func @unstructured_cfg() {
  "regionOp0"() ({
    ^bb0:
      "op0"() : () -> ()
      cf.br ^bb2
    ^bb1:
      "op1"() : () -> ()
      cf.br ^bb2
    ^bb2:
      "op2"() : () -> ()
  }) : () -> ()
  return
}

// CHECK-LABEL: Op pre-order visits
// CHECK:       Visiting op 'builtin.module'
// CHECK:       Visiting op 'func.func'
// CHECK:       Visiting op 'regionOp0'
// CHECK:       Visiting op 'op0'
// CHECK:       Visiting op 'cf.br'
// CHECK:       Visiting op 'op1'
// CHECK:       Visiting op 'cf.br'
// CHECK:       Visiting op 'op2'
// CHECK:       Visiting op 'func.return'
...
```

---

### Comment 8 - matthias-springer

Ok, how about this?
```c++
      for (Operation &op : *block)
        op.dropAllUses();
      block->erase();
```


---

### Comment 9 - meshtag

This will make the IR not crash even when the block parent op visit order is not following post-order semantics. The tests expect the pass to work without crashing only when the visit order is following post-order semantics while visiting these ops. 

Dropping all uses for intra-block ops which are present inside an operation with multiple blocks (and no other region holding op) should work though, I think. Because there is no expectation for the order in which these intra-op blocks are visited (we only need to visit the block parent ops themselves in post-order fashion). 

I will create a PR for this in some time. It should make this more clear.

---

