# [MLIR] Crash in MLIR Block Destruction Triggered by a GPU Program

**Author:** kitty-1998
**URL:** https://github.com/llvm/llvm-project/issues/182996
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T10:56:37Z

## Body

When processing a gpu dialect program, MLIR leads to a crash. Below is a code example that reproduces the issue.

```llvm
module {
  func.func @VecAdd_kernel(%arg0: memref<?xf32>, %arg1: memref<?xf32>, %arg2: memref<?xf32>, %arg3: i32) attributes {gpu.kernel} {
    %c0 = arith.constant 0 : index
    %0 = gpu.block_id  x
    %1 = gpu.thread_id  x
    %2 = gpu.block_dim  x
    %3 = arith.index_cast %2 : index to i32
    %4 = arith.index_cast %0 : index to i32
    %5 = arith.muli %3, %4 : i32
    %6 = arith.index_cast %1 : index to i32
    %7 = arith.addi %5, %6 : i32
    cf.br ^bb1
  ^bb1:
    %8 = arith.cmpi slt, %7, %arg3 : i32
    cf.cond_br %8, ^bb2, ^bb4
  ^bb2:
    %9 = memref.load %arg0[%c0] : memref<?xf32>
    %10 = memref.load %arg1[%c0] : memref<?xf32>
    %11 = arith.addf %9, %10 : f32
    %12 = arith.addi %7, %arg3 : i32
    cf.cond_br %8, ^bb3(%11, %12 : f32, i32), ^bb4
  ^bb3(%13: f32, %14: i32):
    memref.store %13, %arg2[%c0] : memref<?xf32>
    %15 = arith.cmpi eq, %14, %arg3 : i32
    cf.cond_br %15, ^bb4, ^bb5
  ^bb4:
    return
  ^bb5:
    memref.store %11, %arg2[%c0] : memref<?xf32>
    return
  }
}
```

MLIR Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt vecAddMMAP.mlir --convert-gpu-to-nvvm --test-ir-visitors
 #0 0x00005bf53d94a1fd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00005bf53d94a72b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00005bf53d9485c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005bf53d94aec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007a4e0ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007a4e0ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007a4e0ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007a4e0ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007a4e0ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007a4e0ba288ff abort ./stdlib/abort.c:81:7
#10 0x00007a4e0ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007a4e0ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005bf54c336634 mlir::IRObjectWithUseList<mlir::OpOperand>::~IRObjectWithUseList() /mlir/include/mlir/IR/UseDefLists.h:199:3
#13 0x00005bf54c3365d5 mlir::detail::ValueImpl::~ValueImpl() /mlir/include/mlir/IR/Value.h:40:18
#14 0x00005bf54c3365b5 mlir::detail::BlockArgumentImpl::~BlockArgumentImpl() /mlir/include/mlir/IR/Value.h:283:7
#15 0x00005bf54c335618 mlir::BlockArgument::destroy() /mlir/include/mlir/IR/Value.h:335:20
#16 0x00005bf54c333691 mlir::Block::~Block() /mlir/lib/IR/Block.cpp:23:26
#17 0x00005bf53e7bd283 llvm::ilist_alloc_traits<mlir::Block>::deleteNode(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:42:39
#18 0x00005bf53e7bd245 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>) /llvm/include/llvm/ADT/ilist.h:206:12
#19 0x00005bf54c335812 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:209:39
#20 0x00005bf54c333be5 mlir::Block::erase() /mlir/lib/IR/Block.cpp:68:28
#21 0x00005bf53e7c9ffd testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:206:5
#22 0x00005bf53e7c9e5d void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#23 0x00005bf53db70551 llvm::function_ref<void (mlir::Block*)>::operator()(mlir::Block*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#24 0x00005bf53db70398 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:5
#25 0x00005bf53db70374 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#26 0x00005bf53e7c9e02 std::enable_if<llvm::is_one_of<mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, mlir::Block*, void>(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /mlir/include/mlir/IR/Visitors.h:278:3
#27 0x00005bf53e7c9c9d std::enable_if<llvm::function_traits<std::decay<testNoSkipErasureCallbacks(mlir::Operation*)::$_1&>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&, void>(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&) /mlir/include/mlir/IR/Operation.h:798:5
#28 0x00005bf53e7c79be testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:222:3
#29 0x00005bf53e7c732d (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:293:3
#30 0x00005bf54bfc4734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#31 0x00005bf54bfc46d5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#32 0x00005bf53d9705c9 llvm::function_ref<void ()>::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#33 0x00005bf54bfc7bdb void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /mlir/include/mlir/IR/MLIRContext.h:291:3
#34 0x00005bf54bfbbb7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#35 0x00005bf54bfbc29a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#36 0x00005bf54bfc1b7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#37 0x00005bf54bfc1106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#38 0x00005bf53da54b4b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#39 0x00005bf53da54221 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#40 0x00005bf53da53edc mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#41 0x00005bf53da53de6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x00005bf54c52688a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x00005bf54c525c4a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x00005bf53da4fb66 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#45 0x00005bf53da4ff68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#46 0x00005bf53da50138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#47 0x00005bf53d91ede5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#48 0x00007a4e0ba2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x00007a4e0ba2a28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x00007a4e0ba2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x00005bf53d91ec55 _start (/mnt/raid/build_all/bin/mlir-opt+0x4742c55)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: kitty-apple (kitty-1998)

<details>
When processing a gpu dialect program, MLIR leads to a crash. Below is a code example that reproduces the issue.

```llvm
module {
  func.func @<!-- -->VecAdd_kernel(%arg0: memref&lt;?xf32&gt;, %arg1: memref&lt;?xf32&gt;, %arg2: memref&lt;?xf32&gt;, %arg3: i32) attributes {gpu.kernel} {
    %c0 = arith.constant 0 : index
    %0 = gpu.block_id  x
    %1 = gpu.thread_id  x
    %2 = gpu.block_dim  x
    %3 = arith.index_cast %2 : index to i32
    %4 = arith.index_cast %0 : index to i32
    %5 = arith.muli %3, %4 : i32
    %6 = arith.index_cast %1 : index to i32
    %7 = arith.addi %5, %6 : i32
    cf.br ^bb1
  ^bb1:
    %8 = arith.cmpi slt, %7, %arg3 : i32
    cf.cond_br %8, ^bb2, ^bb4
  ^bb2:
    %9 = memref.load %arg0[%c0] : memref&lt;?xf32&gt;
    %10 = memref.load %arg1[%c0] : memref&lt;?xf32&gt;
    %11 = arith.addf %9, %10 : f32
    %12 = arith.addi %7, %arg3 : i32
    cf.cond_br %8, ^bb3(%11, %12 : f32, i32), ^bb4
  ^bb3(%13: f32, %14: i32):
    memref.store %13, %arg2[%c0] : memref&lt;?xf32&gt;
    %15 = arith.cmpi eq, %14, %arg3 : i32
    cf.cond_br %15, ^bb4, ^bb5
  ^bb4:
    return
  ^bb5:
    memref.store %11, %arg2[%c0] : memref&lt;?xf32&gt;
    return
  }
}
```

MLIR Version: 7dbfc2bd0078b36e4f9ed14dd25c4d8e97b3ae77

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt vecAddMMAP.mlir --convert-gpu-to-nvvm --test-ir-visitors
 #<!-- -->0 0x00005bf53d94a1fd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00005bf53d94a72b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00005bf53d9485c4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005bf53d94aec9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007a4e0ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007a4e0ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007a4e0ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007a4e0ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007a4e0ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007a4e0ba288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007a4e0ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007a4e0ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005bf54c336634 mlir::IRObjectWithUseList&lt;mlir::OpOperand&gt;::~IRObjectWithUseList() /mlir/include/mlir/IR/UseDefLists.h:199:3
#<!-- -->13 0x00005bf54c3365d5 mlir::detail::ValueImpl::~ValueImpl() /mlir/include/mlir/IR/Value.h:40:18
#<!-- -->14 0x00005bf54c3365b5 mlir::detail::BlockArgumentImpl::~BlockArgumentImpl() /mlir/include/mlir/IR/Value.h:283:7
#<!-- -->15 0x00005bf54c335618 mlir::BlockArgument::destroy() /mlir/include/mlir/IR/Value.h:335:20
#<!-- -->16 0x00005bf54c333691 mlir::Block::~Block() /mlir/lib/IR/Block.cpp:23:26
#<!-- -->17 0x00005bf53e7bd283 llvm::ilist_alloc_traits&lt;mlir::Block&gt;::deleteNode(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:42:39
#<!-- -->18 0x00005bf53e7bd245 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;) /llvm/include/llvm/ADT/ilist.h:206:12
#<!-- -->19 0x00005bf54c335812 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:209:39
#<!-- -->20 0x00005bf54c333be5 mlir::Block::erase() /mlir/lib/IR/Block.cpp:68:28
#<!-- -->21 0x00005bf53e7c9ffd testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:206:5
#<!-- -->22 0x00005bf53e7c9e5d void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->23 0x00005bf53db70551 llvm::function_ref&lt;void (mlir::Block*)&gt;::operator()(mlir::Block*) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->24 0x00005bf53db70398 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:5
#<!-- -->25 0x00005bf53db70374 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->26 0x00005bf53e7c9e02 std::enable_if&lt;llvm::is_one_of&lt;mlir::Block*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, mlir::Block*, void&gt;(mlir::Operation*, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->27 0x00005bf53e7c9c9d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator, testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;, void&gt;(testNoSkipErasureCallbacks(mlir::Operation*)::$_1&amp;) /mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->28 0x00005bf53e7c79be testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:222:3
#<!-- -->29 0x00005bf53e7c732d (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:293:3
#<!-- -->30 0x00005bf54bfc4734 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->31 0x00005bf54bfc46d5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->32 0x00005bf53d9705c9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->33 0x00005bf54bfc7bdb void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->34 0x00005bf54bfbbb7e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /mlir/lib/Pass/Pass.cpp:619:17
#<!-- -->35 0x00005bf54bfbc29a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->36 0x00005bf54bfc1b7d mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->37 0x00005bf54bfc1106 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->38 0x00005bf53da54b4b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->39 0x00005bf53da54221 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->40 0x00005bf53da53edc mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->41 0x00005bf53da53de6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x00005bf54c52688a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x00005bf54c525c4a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x00005bf53da4fb66 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->45 0x00005bf53da4ff68 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->46 0x00005bf53da50138 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->47 0x00005bf53d91ede5 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->48 0x00007a4e0ba2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x00007a4e0ba2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x00007a4e0ba2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x00005bf53d91ec55 _start (/mnt/raid/build_all/bin/mlir-opt+0x4742c55)
Aborted
```
</details>


---

### Comment 2 - woruyu

I would like to fix it!

---

### Comment 3 - joker-eph

Go ahead and send a PR! :)

---

