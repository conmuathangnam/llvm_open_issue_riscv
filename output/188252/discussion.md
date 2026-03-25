# [MLIR] Mem2Reg crashes due to destroyed op with uses

**Author:** vlstill
**URL:** https://github.com/llvm/llvm-project/issues/188252
**Status:** Open
**Labels:** mlir, crash

## Body

For certain programs Mem2Reg crashes on current `main`:

```
mlir/test/Transforms/mem2reg-store-back-crash.mlir:16:13: error: 'memref.load' op operation destroyed but still has uses
  %loaded = memref.load %alloca[] : memref<i32>
            ^
mlir/test/Transforms/mem2reg-store-back-crash.mlir:16:13: note: see current operation: %0 = "memref.load"(<<UNKNOWN SSA VALUE>>) : (memref<i32>) -> i32
mlir/test/Transforms/mem2reg-store-back-crash.mlir:21:3: note: - use: "cf.br"(<<UNKNOWN SSA VALUE>>)[^bb3] : (i32) -> ()

  cf.br ^merge
  ^
```

The lit test for this is [mem2reg-store-back-crash.mlir.txt](https://github.com/user-attachments/files/26216179/mem2reg-store-back-crash.mlir.txt):

```mlir
func.func @store_back_to_alloca(%cond: i1) -> i32 {
  %c0 = arith.constant 0 : i32
  %alloca = memref.alloca() : memref<i32>
  memref.store %c0, %alloca[] : memref<i32>
  %loaded = memref.load %alloca[] : memref<i32>
  cf.cond_br %cond, ^store_back, ^skip

^store_back:
  memref.store %loaded, %alloca[] : memref<i32>
  cf.br ^merge

^skip:
  cf.br ^merge

^merge:
  %result = memref.load %alloca[] : memref<i32>
  return %result : i32
}
```

This seems to be related to 239ca11a55b40ce12b21bc47e45cb4065d1cc3d4 / #185036 by @tdegioanni-nvidia, reverting that fixes the lit test.

backtrace:
```
LLVM ERROR: operation destroyed but still has uses
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt mlir/test/Transforms/mem2reg-store-back-crash.mlir --pass-pipeline=builtin.module(any(mem2reg))
 #0 0x00007efd8a894fd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/vstill/repo/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:13
 #1 0x00007efd8a8928a3 llvm::sys::RunSignalHandlers() /home/vstill/repo/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x00007efd8a895de1 SignalHandler(int, siginfo_t*, void*) /home/vstill/repo/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:38
 #3 0x00007efd87c3fc30 __restore_rt (/lib64/libc.so.6+0x3fc30)
 #4 0x00007efd87c8d02c __pthread_kill_implementation (/lib64/libc.so.6+0x8d02c)
 #5 0x00007efd87c3fb86 gsignal (/lib64/libc.so.6+0x3fb86)
 #6 0x00007efd87c29873 abort (/lib64/libc.so.6+0x29873)
 #7 0x00007efd8a7ce1c7 llvm::report_fatal_error(llvm::Twine const&, bool) /home/vstill/repo/llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
 #8 0x00007efd8a7ce016 (/home/vstill/repo/llvm-project/build/bin/../lib/libLLVM.so.23.0git+0x23ce016)
 #9 0x00007efd94ae0646 mlir::InFlightDiagnostic::attachNote(std::optional<mlir::Location>) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Diagnostics.h:366:5
#10 0x00007efd94ae0646 mlir::Operation::~Operation() /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:187:14
#11 0x00007efd94ae123d mlir::Operation::destroy() /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:214:3
#12 0x00007efd94ae123d llvm::ilist_traits<mlir::Operation>::deleteNode(mlir::Operation*) /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:487:7
#13 0x00007efd94a33ece llvm::iplist_impl<llvm::simple_ilist<mlir::Operation>, llvm::ilist_traits<mlir::Operation>>::erase(llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Operation, true, false, void, false, void>, false, false>) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/ilist.h:206:5
#14 0x00007efd94ecb5d9 (anonymous namespace)::MemorySlotPromoter::promoteSlot() /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:838:29
#15 0x00007efd94ecb5d9 mlir::tryToPromoteMemorySlots(llvm::ArrayRef<mlir::PromotableAllocationOpInterface>, mlir::OpBuilder&, mlir::DataLayout const&, mlir::DominanceInfo&, mlir::Mem2RegStatistics) /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:882:20
#16 0x00007efd94ed2d69 (anonymous namespace)::Mem2Reg::runOnOperation() /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:940:21
#17 0x00007efd94b64a24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:0:19
#18 0x00007efd94b64a24 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#19 0x00007efd94b64a24 llvm::function_ref<void ()>::operator()() const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#20 0x00007efd94b64a24 void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/MLIRContext.h:290:7
#21 0x00007efd94b64a24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:606:23
#22 0x00007efd94b6587a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:688:9
#23 0x00007efd94b6fda2 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1004:9
#24 0x00007efd94b6fda2 auto void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)::operator()<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&>(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&) const /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#25 0x00007efd94b6835b llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:0:18
#26 0x00007efd94b6835b void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#27 0x00007efd94b6835b void mlir::parallelForEach<std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>&, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#28 0x00007efd94b6835b mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:991:3
#29 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:0:5
#30 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:610:22
#31 0x00007efd94b64c6a void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#32 0x00007efd94b64c6a llvm::function_ref<void ()>::operator()() const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#33 0x00007efd94b64c6a void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/MLIRContext.h:290:7
#34 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:606:23
#35 0x00007efd94b6587a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:688:9
#36 0x00007efd94b6c693 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1123:3
#37 0x00007efd94b6bd1d mlir::PassManager::run(mlir::Operation*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1097:0
#38 0x00007efd94e4a3f4 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#39 0x00007efd94e4969c processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#40 0x00007efd94e4969c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#41 0x00007efd94e4969c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x00007efd94bd3b3b llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x00007efd94bd3b3b mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /home/vstill/repo/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x00007efd94e41033 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#45 0x00007efd94e41369 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#46 0x00007efd94e415cb mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#47 0x000000000020225a main /home/vstill/repo/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#48 0x00007efd87c2a610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#49 0x00007efd87c2a6c0 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#50 0x0000000000202055 _start (./build/bin/mlir-opt+0x202055)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Vladimír Štill (vlstill)

<details>
For certain programs Mem2Reg crashes on current `main`:

```
mlir/test/Transforms/mem2reg-store-back-crash.mlir:16:13: error: 'memref.load' op operation destroyed but still has uses
  %loaded = memref.load %alloca[] : memref&lt;i32&gt;
            ^
mlir/test/Transforms/mem2reg-store-back-crash.mlir:16:13: note: see current operation: %0 = "memref.load"(&lt;&lt;UNKNOWN SSA VALUE&gt;&gt;) : (memref&lt;i32&gt;) -&gt; i32
mlir/test/Transforms/mem2reg-store-back-crash.mlir:21:3: note: - use: "cf.br"(&lt;&lt;UNKNOWN SSA VALUE&gt;&gt;)[^bb3] : (i32) -&gt; ()

  cf.br ^merge
  ^
```

The lit test for this is [mem2reg-store-back-crash.mlir.txt](https://github.com/user-attachments/files/26216179/mem2reg-store-back-crash.mlir.txt):

```mlir
func.func @<!-- -->store_back_to_alloca(%cond: i1) -&gt; i32 {
  %c0 = arith.constant 0 : i32
  %alloca = memref.alloca() : memref&lt;i32&gt;
  memref.store %c0, %alloca[] : memref&lt;i32&gt;
  %loaded = memref.load %alloca[] : memref&lt;i32&gt;
  cf.cond_br %cond, ^store_back, ^skip

^store_back:
  memref.store %loaded, %alloca[] : memref&lt;i32&gt;
  cf.br ^merge

^skip:
  cf.br ^merge

^merge:
  %result = memref.load %alloca[] : memref&lt;i32&gt;
  return %result : i32
}
```

This seems to be related to 239ca11a55b40ce12b21bc47e45cb4065d1cc3d4 / #<!-- -->185036 by @<!-- -->tdegioanni-nvidia, reverting that fixes the lit test.

backtrace:
```
LLVM ERROR: operation destroyed but still has uses
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt mlir/test/Transforms/mem2reg-store-back-crash.mlir --pass-pipeline=builtin.module(any(mem2reg))
 #<!-- -->0 0x00007efd8a894fd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/vstill/repo/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:13
 #<!-- -->1 0x00007efd8a8928a3 llvm::sys::RunSignalHandlers() /home/vstill/repo/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->2 0x00007efd8a895de1 SignalHandler(int, siginfo_t*, void*) /home/vstill/repo/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->3 0x00007efd87c3fc30 __restore_rt (/lib64/libc.so.6+0x3fc30)
 #<!-- -->4 0x00007efd87c8d02c __pthread_kill_implementation (/lib64/libc.so.6+0x8d02c)
 #<!-- -->5 0x00007efd87c3fb86 gsignal (/lib64/libc.so.6+0x3fb86)
 #<!-- -->6 0x00007efd87c29873 abort (/lib64/libc.so.6+0x29873)
 #<!-- -->7 0x00007efd8a7ce1c7 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /home/vstill/repo/llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
 #<!-- -->8 0x00007efd8a7ce016 (/home/vstill/repo/llvm-project/build/bin/../lib/libLLVM.so.23.0git+0x23ce016)
 #<!-- -->9 0x00007efd94ae0646 mlir::InFlightDiagnostic::attachNote(std::optional&lt;mlir::Location&gt;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Diagnostics.h:366:5
#<!-- -->10 0x00007efd94ae0646 mlir::Operation::~Operation() /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:187:14
#<!-- -->11 0x00007efd94ae123d mlir::Operation::destroy() /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:214:3
#<!-- -->12 0x00007efd94ae123d llvm::ilist_traits&lt;mlir::Operation&gt;::deleteNode(mlir::Operation*) /home/vstill/repo/llvm-project/mlir/lib/IR/Operation.cpp:487:7
#<!-- -->13 0x00007efd94a33ece llvm::iplist_impl&lt;llvm::simple_ilist&lt;mlir::Operation&gt;, llvm::ilist_traits&lt;mlir::Operation&gt;&gt;::erase(llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Operation, true, false, void, false, void&gt;, false, false&gt;) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/ilist.h:206:5
#<!-- -->14 0x00007efd94ecb5d9 (anonymous namespace)::MemorySlotPromoter::promoteSlot() /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:838:29
#<!-- -->15 0x00007efd94ecb5d9 mlir::tryToPromoteMemorySlots(llvm::ArrayRef&lt;mlir::PromotableAllocationOpInterface&gt;, mlir::OpBuilder&amp;, mlir::DataLayout const&amp;, mlir::DominanceInfo&amp;, mlir::Mem2RegStatistics) /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:882:20
#<!-- -->16 0x00007efd94ed2d69 (anonymous namespace)::Mem2Reg::runOnOperation() /home/vstill/repo/llvm-project/mlir/lib/Transforms/Mem2Reg.cpp:940:21
#<!-- -->17 0x00007efd94b64a24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->18 0x00007efd94b64a24 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->19 0x00007efd94b64a24 llvm::function_ref&lt;void ()&gt;::operator()() const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->20 0x00007efd94b64a24 void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/MLIRContext.h:290:7
#<!-- -->21 0x00007efd94b64a24 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:606:23
#<!-- -->22 0x00007efd94b6587a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->23 0x00007efd94b6fda2 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1004:9
#<!-- -->24 0x00007efd94b6fda2 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->25 0x00007efd94b6835b llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:0:18
#<!-- -->26 0x00007efd94b6835b void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->27 0x00007efd94b6835b void mlir::parallelForEach&lt;std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&amp;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->28 0x00007efd94b6835b mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:991:3
#<!-- -->29 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::runOnOperation(bool) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:0:5
#<!-- -->30 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:610:22
#<!-- -->31 0x00007efd94b64c6a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->32 0x00007efd94b64c6a llvm::function_ref&lt;void ()&gt;::operator()() const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->33 0x00007efd94b64c6a void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /home/vstill/repo/llvm-project/mlir/include/mlir/IR/MLIRContext.h:290:7
#<!-- -->34 0x00007efd94b64c6a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:606:23
#<!-- -->35 0x00007efd94b6587a mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->36 0x00007efd94b6c693 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1123:3
#<!-- -->37 0x00007efd94b6bd1d mlir::PassManager::run(mlir::Operation*) /home/vstill/repo/llvm-project/mlir/lib/Pass/Pass.cpp:1097:0
#<!-- -->38 0x00007efd94e4a3f4 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:596:7
#<!-- -->39 0x00007efd94e4969c processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x00007efd94e4969c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x00007efd94e4969c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x00007efd94bd3b3b llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /home/vstill/repo/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x00007efd94bd3b3b mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /home/vstill/repo/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x00007efd94e41033 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x00007efd94e41369 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x00007efd94e415cb mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /home/vstill/repo/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x000000000020225a main /home/vstill/repo/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->48 0x00007efd87c2a610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#<!-- -->49 0x00007efd87c2a6c0 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#<!-- -->50 0x0000000000202055 _start (./build/bin/mlir-opt+0x202055)
Aborted (core dumped)
```
</details>


---

### Comment 2 - tdegioanni-nvidia

Looking at it.

---

### Comment 3 - tdegioanni-nvidia

Ah yes, I see the issue. I'll submit a fix shortly.

---

