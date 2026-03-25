# [MLIR] Crash when erasing blocks during operation walk in gpu program

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/184290
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-03T11:28:50Z

## Body

Hi, I’ve run into a crash bug in my gpu program. Take a look at the following code example:

Git version: 7370091a43e5cbcb4cf56fb83b62e056903cb918

#### Input Program
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

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt vectorAdd_kernel.mlir --test-ir-visitors
 #0 0x0000568c97e73596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #1 0x0000568c97f57e34 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #2 0x0000568c97f592cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #3 0x0000568c97f592cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #4 0x00007b2581e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007b2581e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007b2581e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007b2581e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007b2581e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007b2581e288ff abort ./stdlib/abort.c:81:7
#10 0x0000568c97f14900 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x0000568c97f1447b /llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x0000568cac4b5abd mlir::Operation::~Operation() /mlir/lib/IR/Operation.cpp:189:5
#13 0x0000568cac4b813d mlir::Operation::destroy() /mlir/lib/IR/Operation.cpp:214:3
#14 0x0000568cac4b813d llvm::ilist_traits<mlir::Operation>::deleteNode(mlir::Operation*) /mlir/lib/IR/Operation.cpp:487:7
#15 0x0000568cac2cd179 llvm::iplist_impl<llvm::simple_ilist<mlir::Operation>, llvm::ilist_traits<mlir::Operation>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>, false, false>) /llvm/include/llvm/ADT/ilist.h:206:5
#16 0x0000568cac2c8a75 llvm::detail::PunnedPointer<llvm::ilist_node_base<true, void>*>::asInt() const /llvm/include/llvm/ADT/PointerIntPair.h:41:5
#17 0x0000568cac2c8a75 llvm::detail::PunnedPointer<llvm::ilist_node_base<true, void>*>::operator long() const /llvm/include/llvm/ADT/PointerIntPair.h:45:48
#18 0x0000568cac2c8a75 llvm::PointerIntPair<llvm::ilist_node_base<true, void>*, 1u, unsigned int, llvm::PointerLikeTypeTraits<llvm::ilist_node_base<true, void>*>, llvm::PointerIntPairInfo<llvm::ilist_node_base<true, void>*, 1u, llvm::PointerLikeTypeTraits<llvm::ilist_node_base<true, void>*>>>::getPointer() const /llvm/include/llvm/ADT/PointerIntPair.h:94:58
#19 0x0000568cac2c8a75 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getPrev() const /llvm/include/llvm/ADT/ilist_node_base.h:41:54
#20 0x0000568cac2c8a75 llvm::ilist_node_impl<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>>::getPrev() const /llvm/include/llvm/ADT/ilist_node.h:112:59
#21 0x0000568cac2c8a75 llvm::ilist_sentinel<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>>::empty() const /llvm/include/llvm/ADT/ilist_node.h:308:45
#22 0x0000568cac2c8a75 llvm::simple_ilist<mlir::Operation>::empty() const /llvm/include/llvm/ADT/simple_ilist.h:132:54
#23 0x0000568cac2c8a75 mlir::Block::empty() /mlir/include/mlir/IR/Block.h:158:36
#24 0x0000568cac2c8a75 mlir::Block::clear() /mlir/include/mlir/IR/Block.h:44:13
#25 0x0000568cac2c8a75 mlir::Block::~Block() /mlir/lib/IR/Block.cpp:22:3
#26 0x0000568cac2c9330 llvm::ilist_alloc_traits<mlir::Block>::deleteNode(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:42:39
#27 0x0000568cac2c9330 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>) /llvm/include/llvm/ADT/ilist.h:205:5
#28 0x0000568cac2c9330 llvm::iplist_impl<llvm::simple_ilist<mlir::Block>, llvm::ilist_traits<mlir::Block>>::erase(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:209:39
#29 0x0000568cac2c9330 mlir::Block::erase() /mlir/lib/IR/Block.cpp:68:28
#30 0x0000568c99166667 testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:0:14
#31 0x0000568c99166667 void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#32 0x0000568c981dddaf llvm::early_inc_iterator_impl<llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>>::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#33 0x0000568c981dddaf void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#34 0x0000568c981ddd51 llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>, false, false>::operator++() /llvm/include/llvm/ADT/ilist_iterator.h:187:48
#35 0x0000568c981ddd51 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#36 0x0000568c9916192e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:233:11
#37 0x0000568c9916192e (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:303:5
#38 0x0000568cabdce593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#39 0x0000568cabdce593 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#40 0x0000568cabdbe27e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#41 0x0000568cabdc0026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#42 0x0000568cabdc0026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#43 0x0000568cabdc0026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#44 0x0000568cabdcbbb4 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#45 0x0000568cabdcac34 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#46 0x0000568c98120f63 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#47 0x0000568c98120f63 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#48 0x0000568c98120f63 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#49 0x0000568c9811f7d6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#50 0x0000568c9811f7d6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#51 0x0000568c9811f7d6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#52 0x0000568cac56394a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#53 0x0000568cac56394a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#54 0x0000568c98119b40 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#55 0x0000568c9811a508 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#56 0x0000568c9811ace3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#57 0x0000568c97f13947 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#58 0x00007b2581e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#59 0x00007b2581e2a28b call_init ./csu/../csu/libc-start.c:128:20
#60 0x00007b2581e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#61 0x0000568c97e29d85 _start (./mlir-opt+0x7a85d85)
Aborted
```

Used command: `mlir-opt vectorAdd_kernel.mlir --test-ir-visitors`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Hi, I’ve run into a crash bug in my gpu program. Take a look at the following code example:

Git version: 7370091a43e5cbcb4cf56fb83b62e056903cb918

#### Input Program
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

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt vectorAdd_kernel.mlir --test-ir-visitors
 #<!-- -->0 0x0000568c97e73596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #<!-- -->1 0x0000568c97f57e34 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #<!-- -->2 0x0000568c97f592cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->3 0x0000568c97f592cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #<!-- -->4 0x00007b2581e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007b2581e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007b2581e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007b2581e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007b2581e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007b2581e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000568c97f14900 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x0000568c97f1447b /llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x0000568cac4b5abd mlir::Operation::~Operation() /mlir/lib/IR/Operation.cpp:189:5
#<!-- -->13 0x0000568cac4b813d mlir::Operation::destroy() /mlir/lib/IR/Operation.cpp:214:3
#<!-- -->14 0x0000568cac4b813d llvm::ilist_traits&lt;mlir::Operation&gt;::deleteNode(mlir::Operation*) /mlir/lib/IR/Operation.cpp:487:7
#<!-- -->15 0x0000568cac2cd179 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Operation&gt;, llvm::ilist_traits&lt;mlir::Operation&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;, false, false&gt;) /llvm/include/llvm/ADT/ilist.h:206:5
#<!-- -->16 0x0000568cac2c8a75 llvm::detail::PunnedPointer&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;::asInt() const /llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->17 0x0000568cac2c8a75 llvm::detail::PunnedPointer&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;::operator long() const /llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->18 0x0000568cac2c8a75 llvm::PointerIntPair&lt;llvm::ilist_node_base&lt;true, void&gt;*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;, llvm::PointerIntPairInfo&lt;llvm::ilist_node_base&lt;true, void&gt;*, 1u, llvm::PointerLikeTypeTraits&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;&gt;&gt;::getPointer() const /llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->19 0x0000568cac2c8a75 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getPrev() const /llvm/include/llvm/ADT/ilist_node_base.h:41:54
#<!-- -->20 0x0000568cac2c8a75 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;&gt;::getPrev() const /llvm/include/llvm/ADT/ilist_node.h:112:59
#<!-- -->21 0x0000568cac2c8a75 llvm::ilist_sentinel&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;&gt;::empty() const /llvm/include/llvm/ADT/ilist_node.h:308:45
#<!-- -->22 0x0000568cac2c8a75 llvm::simple_ilist&lt;mlir::Operation&gt;::empty() const /llvm/include/llvm/ADT/simple_ilist.h:132:54
#<!-- -->23 0x0000568cac2c8a75 mlir::Block::empty() /mlir/include/mlir/IR/Block.h:158:36
#<!-- -->24 0x0000568cac2c8a75 mlir::Block::clear() /mlir/include/mlir/IR/Block.h:44:13
#<!-- -->25 0x0000568cac2c8a75 mlir::Block::~Block() /mlir/lib/IR/Block.cpp:22:3
#<!-- -->26 0x0000568cac2c9330 llvm::ilist_alloc_traits&lt;mlir::Block&gt;::deleteNode(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:42:39
#<!-- -->27 0x0000568cac2c9330 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;) /llvm/include/llvm/ADT/ilist.h:205:5
#<!-- -->28 0x0000568cac2c9330 llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Block&gt;, llvm::ilist_traits&lt;mlir::Block&gt;&gt;::erase(mlir::Block*) /llvm/include/llvm/ADT/ilist.h:209:39
#<!-- -->29 0x0000568cac2c9330 mlir::Block::erase() /mlir/lib/IR/Block.cpp:68:28
#<!-- -->30 0x0000568c99166667 testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:0:14
#<!-- -->31 0x0000568c99166667 void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->32 0x0000568c981dddaf llvm::early_inc_iterator_impl&lt;llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;&gt;::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#<!-- -->33 0x0000568c981dddaf void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#<!-- -->34 0x0000568c981ddd51 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;, false, false&gt;::operator++() /llvm/include/llvm/ADT/ilist_iterator.h:187:48
#<!-- -->35 0x0000568c981ddd51 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:127:27
#<!-- -->36 0x0000568c9916192e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:233:11
#<!-- -->37 0x0000568c9916192e (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:303:5
#<!-- -->38 0x0000568cabdce593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#<!-- -->39 0x0000568cabdce593 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->40 0x0000568cabdbe27e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->41 0x0000568cabdc0026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->42 0x0000568cabdc0026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->43 0x0000568cabdc0026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->44 0x0000568cabdcbbb4 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->45 0x0000568cabdcac34 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#<!-- -->46 0x0000568c98120f63 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->47 0x0000568c98120f63 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->48 0x0000568c98120f63 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#<!-- -->49 0x0000568c9811f7d6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->50 0x0000568c9811f7d6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->51 0x0000568c9811f7d6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->52 0x0000568cac56394a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->53 0x0000568cac56394a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->54 0x0000568c98119b40 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->55 0x0000568c9811a508 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->56 0x0000568c9811ace3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->57 0x0000568c97f13947 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->58 0x00007b2581e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->59 0x00007b2581e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->60 0x00007b2581e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->61 0x0000568c97e29d85 _start (./mlir-opt+0x7a85d85)
Aborted
```

Used command: `mlir-opt vectorAdd_kernel.mlir --test-ir-visitors`
</details>


---

### Comment 2 - Men-cotton

I believe this crash was already fixed by https://github.com/llvm/llvm-project/commit/86d0a02e652b3058288a769400e678da0a806e8f (and https://github.com/llvm/llvm-project/commit/7370091a43e5cbcb4cf56fb83b62e056903cb918).
Could you please double-check your build or try again with the latest main branch?

---

### Comment 3 - jiayiw2008

I have tested this issue in the latest branch and can confirm that it has been fixed.

---

