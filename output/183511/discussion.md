# [MLIR] Crash in testNoSkipErasureCallbacks when accessing parent region during walk

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/183511
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T11:15:08Z

## Body

Version: bded7e9e49e5cc4cbb73c54ac7b113a006f0534c

Hi, I encountered a crash in MLIR when processing an "empty" input file. Please see the minimal input program below.

```llvm
//
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --test-ir-visitors --convert-gpu-to-nvvm
 #0 0x00005f38fe0a84e6 ___interceptor_backtrace /home/zhanghuaien/llvm/llvm-project-7dbfc2/compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #1 0x00005f38fe18ce74 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #2 0x00005f38fe18e30c llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #3 0x00005f38fe18e30c SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #4 0x00007c512cc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007c512cc9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007c512cc9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007c512cc9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007c512cc4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007c512cc288ff abort ./stdlib/abort.c:81:7
#10 0x00007c512cc2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007c512cc3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005f38fe3721bd llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>, false, false>::operator*() const /llvm/include/llvm/ADT/ilist_iterator.h:168:5
#13 0x00005f38ff39a33d mlir::Operation::getParentRegion() /mlir/include/mlir/IR/Operation.h:230:38
#14 0x00005f38ff39a33d testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:195:50
#15 0x00005f38ff39a33d void llvm::function_ref<void (mlir::Block*)>::callback_fn<testNoSkipErasureCallbacks(mlir::Operation*)::$_1>(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#16 0x00005f38fe41251f llvm::early_inc_iterator_impl<llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>>::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#17 0x00005f38fe41251f void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#18 0x00005f38ff395b6e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:222:11
#19 0x00005f38ff395b6e (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:292:5
#20 0x00005f3911f52c23 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#21 0x00005f3911f52c23 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#22 0x00005f3911f4293e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#23 0x00005f3911f446e6 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#24 0x00005f3911f446e6 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#25 0x00005f3911f446e6 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#26 0x00005f3911f50244 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#27 0x00005f3911f4f2c4 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#28 0x00005f38fe355ef6 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#29 0x00005f38fe355ef6 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#30 0x00005f38fe355ef6 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:7
#31 0x00005f38fe354756 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#32 0x00005f38fe354756 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#33 0x00005f38fe354756 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#34 0x00005f39126e6dfa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#35 0x00005f39126e6dfa mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#36 0x00005f38fe34eb60 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#37 0x00005f38fe34f528 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#38 0x00005f38fe34fd03 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#39 0x00005f38fe148987 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#40 0x00007c512cc2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#41 0x00007c512cc2a28b call_init ./csu/../csu/libc-start.c:128:20
#42 0x00007c512cc2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#43 0x00005f38fe05ecd5 _start (/mnt/raid/build_all/bin/mlir-opt+0x7a53cd5)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
Version: bded7e9e49e5cc4cbb73c54ac7b113a006f0534c

Hi, I encountered a crash in MLIR when processing an "empty" input file. Please see the minimal input program below.

```llvm
//
```

#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --test-ir-visitors --convert-gpu-to-nvvm
 #<!-- -->0 0x00005f38fe0a84e6 ___interceptor_backtrace /home/zhanghuaien/llvm/llvm-project-7dbfc2/compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #<!-- -->1 0x00005f38fe18ce74 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #<!-- -->2 0x00005f38fe18e30c llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->3 0x00005f38fe18e30c SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #<!-- -->4 0x00007c512cc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007c512cc9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007c512cc9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007c512cc9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007c512cc4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007c512cc288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007c512cc2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007c512cc3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005f38fe3721bd llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;, false, false&gt;::operator*() const /llvm/include/llvm/ADT/ilist_iterator.h:168:5
#<!-- -->13 0x00005f38ff39a33d mlir::Operation::getParentRegion() /mlir/include/mlir/IR/Operation.h:230:38
#<!-- -->14 0x00005f38ff39a33d testNoSkipErasureCallbacks(mlir::Operation*)::$_1::operator()(mlir::Block*) const /mlir/test/lib/IR/TestVisitors.cpp:195:50
#<!-- -->15 0x00005f38ff39a33d void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;testNoSkipErasureCallbacks(mlir::Operation*)::$_1&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->16 0x00005f38fe41251f llvm::early_inc_iterator_impl&lt;llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;&gt;::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#<!-- -->17 0x00005f38fe41251f void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#<!-- -->18 0x00005f38ff395b6e testNoSkipErasureCallbacks(mlir::Operation*) /mlir/test/lib/IR/TestVisitors.cpp:222:11
#<!-- -->19 0x00005f38ff395b6e (anonymous namespace)::TestIRVisitorsPass::runOnOperation() /mlir/test/lib/IR/TestVisitors.cpp:292:5
#<!-- -->20 0x00005f3911f52c23 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#<!-- -->21 0x00005f3911f52c23 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->22 0x00005f3911f4293e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->23 0x00005f3911f446e6 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->24 0x00005f3911f446e6 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->25 0x00005f3911f446e6 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->26 0x00005f3911f50244 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->27 0x00005f3911f4f2c4 mlir::PassManager::run(mlir::Operation*) /mlir/lib/Pass/Pass.cpp:1097:0
#<!-- -->28 0x00005f38fe355ef6 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->29 0x00005f38fe355ef6 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->30 0x00005f38fe355ef6 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:7
#<!-- -->31 0x00005f38fe354756 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->32 0x00005f38fe354756 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->33 0x00005f38fe354756 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->34 0x00005f39126e6dfa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->35 0x00005f39126e6dfa mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->36 0x00005f38fe34eb60 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->37 0x00005f38fe34f528 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->38 0x00005f38fe34fd03 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->39 0x00005f38fe148987 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->40 0x00007c512cc2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->41 0x00007c512cc2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->42 0x00007c512cc2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->43 0x00005f38fe05ecd5 _start (/mnt/raid/build_all/bin/mlir-opt+0x7a53cd5)
Aborted
```
</details>


---

### Comment 2 - Men-cotton

Please add the repro command (`mlir-opt input.mlir --test-ir-visitors --convert-gpu-to-nvvm`) to the issue description. Although it appears in the crash trace, stating it explicitly will make the report clearer and easier for contributors to reproduce.

---

