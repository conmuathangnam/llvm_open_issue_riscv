# [MLIR] Crash in raw_ostream write triggered by a gpu program

**Author:** Zustin
**URL:** https://github.com/llvm/llvm-project/issues/183999
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-03T12:09:02Z

## Body

Hi, I found a crash bug triggered by a GPU program. It appears to be a concurrency issue similar to (but different from) https://github.com/llvm/llvm-project/issues/182255. See the code example below:

Version: 7370091a43e5cbcb4cf56fb83b62e056903cb918

```mlir
module {
  func.func @kernelAddConstant(%arg0: memref<?xi32>, %arg1: i32) attributes {llvm.emit_c_interface} {
    %0 = gpu.global_id  x
    %1 = memref.load %arg0[%0] : memref<?xi32>
    func.call @helper(%1, %arg1) : (i32, i32) -> i32
    %result = func.call @helper(%1, %arg1) : (i32, i32) -> i32
    memref.store %result, %arg0[%0] : memref<?xi32>
    return
  }
  func.func private @helper(%a: i32, %b: i32) -> i32 {
    %sum = arith.addi %a, %b : i32
    return %sum : i32
  }
}
```

Used command: `mlir-opt cudaOpenMP.mlir --test-block-is-in-loop`.

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
 #0 0x00005fdc984c0596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #1 0x00005fdc985a4e34 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #2 0x00005fdc985a62cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #3 0x00005fdc985a62cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #4 0x0000702aa9845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000702aa989eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000702aa989eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000702aa989eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000702aa984527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000702aa98288ff abort ./stdlib/abort.c:81:7
#10 0x0000702aa982881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000702aa983b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005fdc985670f1 llvm::raw_ostream::SetBufferAndMode(char*, unsigned long, llvm::raw_ostream::BufferKind) /llvm/lib/Support/raw_ostream.cpp:103:3
#13 0x00005fdc98568e4d llvm::raw_ostream::write(char const*, unsigned long) /llvm/lib/Support/raw_ostream.cpp:0:7
#14 0x00005fdc9882a8c0 llvm::raw_ostream::operator<<(llvm::StringRef) /llvm/include/llvm/Support/raw_ostream.h:231:7
#15 0x00005fdc9882a8c0 llvm::raw_ostream::operator<<(char const*) /llvm/include/llvm/Support/raw_ostream.h:258:18
#16 0x00005fdc9882a8c0 (anonymous namespace)::IsInLoopPass::runOnOperation()::'lambda'(mlir::Block*)::operator()(mlir::Block*) const /mlir/test/lib/Interfaces/LoopLikeInterface/TestBlockInLoop.cpp:32:20
#17 0x00005fdc9882a8c0 void llvm::function_ref<void (mlir::Block*)>::callback_fn<(anonymous namespace)::IsInLoopPass::runOnOperation()::'lambda'(mlir::Block*)>(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#18 0x00005fdc9882adaf llvm::early_inc_iterator_impl<llvm::ilist_iterator<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void>, false, false>>::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#19 0x00005fdc9882adaf void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Block*)>, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#20 0x00005fdc9882a74c (anonymous namespace)::IsInLoopPass::runOnOperation() /mlir/test/lib/Interfaces/LoopLikeInterface/TestBlockInLoop.cpp:31:5
#21 0x00005fdcac41b593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#22 0x00005fdcac41b593 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#23 0x00005fdcac40b27e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#24 0x00005fdcac40d026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#25 0x00005fdcac40d026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#26 0x00005fdcac40d026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#27 0x00005fdcac41dbe5 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const /mlir/lib/Pass/Pass.cpp:1001:36
#28 0x00005fdcac41dbe5 auto void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)::operator()<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&>(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&) const /mlir/include/mlir/IR/Threading.h:120:12
#29 0x00005fdcac41e0ae llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#30 0x00005fdcac41e0ae llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#31 0x00005fdcac41e0ae llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'()::operator()() const /mlir/include/mlir/IR/Threading.h:62:11
#32 0x00005fdcac41e0ae void llvm::detail::UniqueFunctionBase<void>::CallImpl<llvm::LogicalResult mlir::failableParallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, void mlir::parallelForEach<__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'(__gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>&&)>(mlir::MLIRContext*, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, __gnu_cxx::__normal_iterator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator<mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo>>>, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&&)::'lambda'()>(void*) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#33 0x00005fdcac2f9c20 std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>::operator()() const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:1442:20
#34 0x00005fdcac2f9a92 std::enable_if<is_invocable_r_v<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>&>, std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter>>::type std::__invoke_r<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter>, std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>&>(std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/invoke.h:114:9
#35 0x00005fdcac2f9a92 std::_Function_handler<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> (), std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>>::_M_invoke(std::_Any_data const&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:290:9
#36 0x00005fdcac2f9765 std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>::operator()() const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:0:9
#37 0x00005fdcac2f9765 std::__future_base::_State_baseV2::_M_do_set(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:589:27
#38 0x0000702aa98a1ed3 __pthread_once_slow ./nptl/pthread_once.c:118:7
#39 0x00005fdcac2f94aa void std::call_once<void (std::__future_base::_State_baseV2::*)(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*), std::__future_base::_State_baseV2*, std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*>(std::once_flag&, void (std::__future_base::_State_baseV2::*&&)(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*), std::__future_base::_State_baseV2*&&, std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*&&, bool*&&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/mutex:907:15
#40 0x00005fdcac2f94aa std::__future_base::_State_baseV2::_M_set_result(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>, bool) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:428:2
#41 0x00005fdcac2fab0a std::_Function_base::~_Function_base() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:243:11
#42 0x00005fdcac2fab0a std::__future_base::_Deferred_state<std::thread::_Invoker<std::tuple<llvm::unique_function<void ()>>>, void>::_M_complete_async() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:1705:9
#43 0x00005fdcac2fb191 std::__future_base::_State_baseV2::wait() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:353:2
#44 0x00005fdcacc86819 llvm::detail::PunnedPointer<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>>::asInt() const /llvm/include/llvm/ADT/PointerIntPair.h:41:5
#45 0x00005fdcacc86819 llvm::detail::PunnedPointer<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>>::operator long() const /llvm/include/llvm/ADT/PointerIntPair.h:45:48
#46 0x00005fdcacc86819 llvm::PointerIntPair<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>, 1u, bool, llvm::PointerLikeTypeTraits<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>>, llvm::PointerIntPairInfo<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>, 1u, llvm::PointerLikeTypeTraits<llvm::PointerUnion<llvm::detail::UniqueFunctionBase<void>::TrivialCallback*, llvm::detail::UniqueFunctionBase<void>::NonTrivialCallbacks*>>>>::getPointer() const /llvm/include/llvm/ADT/PointerIntPair.h:94:58
#47 0x00005fdcacc86819 llvm::detail::UniqueFunctionBase<void>::~UniqueFunctionBase() /llvm/include/llvm/ADT/FunctionExtras.h:274:32
#48 0x00005fdcacc86819 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) /llvm/lib/Support/ThreadPool.cpp:119:5
#49 0x00005fdcacc89844 std::default_delete<std::tuple<llvm::StdThreadPool::grow(int)::$_0>>::operator()(std::tuple<llvm::StdThreadPool::grow(int)::$_0>*) const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:99:2
#50 0x00005fdcacc89844 std::unique_ptr<std::tuple<llvm::StdThreadPool::grow(int)::$_0>, std::default_delete<std::tuple<llvm::StdThreadPool::grow(int)::$_0>>>::~unique_ptr() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:404:4
#51 0x00005fdcacc89844 void llvm::thread::GenericThreadProxy<std::tuple<llvm::StdThreadPool::grow(int)::$_0>>(void*) /llvm/include/llvm/Support/thread.h:49:3
#52 0x00005fdcacc89844 void* llvm::thread::ThreadProxy<std::tuple<llvm::StdThreadPool::grow(int)::$_0>>(void*) /llvm/include/llvm/Support/thread.h:62:5
#53 0x00005fdc98518207 asan_thread_start(void*) -9b4c99/compiler-rt/lib/asan/asan_interceptors.cpp:306:28
#54 0x0000702aa989caa4 start_thread ./nptl/pthread_create.c:447:8
#55 0x0000702aa9929c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
```




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Zustin)

<details>
Hi, I found a crash bug triggered by a GPU program. It appears to be a concurrency issue similar to (but different from) https://github.com/llvm/llvm-project/issues/182255. See the code example below:

Version: 7370091a43e5cbcb4cf56fb83b62e056903cb918

```mlir
module {
  func.func @<!-- -->kernelAddConstant(%arg0: memref&lt;?xi32&gt;, %arg1: i32) attributes {llvm.emit_c_interface} {
    %0 = gpu.global_id  x
    %1 = memref.load %arg0[%0] : memref&lt;?xi32&gt;
    func.call @<!-- -->helper(%1, %arg1) : (i32, i32) -&gt; i32
    %result = func.call @<!-- -->helper(%1, %arg1) : (i32, i32) -&gt; i32
    memref.store %result, %arg0[%0] : memref&lt;?xi32&gt;
    return
  }
  func.func private @<!-- -->helper(%a: i32, %b: i32) -&gt; i32 {
    %sum = arith.addi %a, %b : i32
    return %sum : i32
  }
}
```

Used command: `mlir-opt cudaOpenMP.mlir --test-block-is-in-loop --convert-gpu-to-nvvnm`.

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
 #<!-- -->0 0x00005fdc984c0596 ___interceptor_backtrace /compiler-rt/lib/asan/../sanitizer_common/sanitizer_common_interceptors.inc:4556:13
 #<!-- -->1 0x00005fdc985a4e34 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:884:8
 #<!-- -->2 0x00005fdc985a62cc llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->3 0x00005fdc985a62cc SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:431:3
 #<!-- -->4 0x0000702aa9845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x0000702aa989eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x0000702aa989eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x0000702aa989eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x0000702aa984527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x0000702aa98288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x0000702aa982881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x0000702aa983b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005fdc985670f1 llvm::raw_ostream::SetBufferAndMode(char*, unsigned long, llvm::raw_ostream::BufferKind) /llvm/lib/Support/raw_ostream.cpp:103:3
#<!-- -->13 0x00005fdc98568e4d llvm::raw_ostream::write(char const*, unsigned long) /llvm/lib/Support/raw_ostream.cpp:0:7
#<!-- -->14 0x00005fdc9882a8c0 llvm::raw_ostream::operator&lt;&lt;(llvm::StringRef) /llvm/include/llvm/Support/raw_ostream.h:231:7
#<!-- -->15 0x00005fdc9882a8c0 llvm::raw_ostream::operator&lt;&lt;(char const*) /llvm/include/llvm/Support/raw_ostream.h:258:18
#<!-- -->16 0x00005fdc9882a8c0 (anonymous namespace)::IsInLoopPass::runOnOperation()::'lambda'(mlir::Block*)::operator()(mlir::Block*) const /mlir/test/lib/Interfaces/LoopLikeInterface/TestBlockInLoop.cpp:32:20
#<!-- -->17 0x00005fdc9882a8c0 void llvm::function_ref&lt;void (mlir::Block*)&gt;::callback_fn&lt;(anonymous namespace)::IsInLoopPass::runOnOperation()::'lambda'(mlir::Block*)&gt;(long, mlir::Block*) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->18 0x00005fdc9882adaf llvm::early_inc_iterator_impl&lt;llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt;, false, false&gt;&gt;::operator++() /llvm/include/llvm/ADT/STLExtras.h:605:12
#<!-- -->19 0x00005fdc9882adaf void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Block*)&gt;, mlir::WalkOrder) /mlir/include/mlir/IR/Visitors.h:123:22
#<!-- -->20 0x00005fdc9882a74c (anonymous namespace)::IsInLoopPass::runOnOperation() /mlir/test/lib/Interfaces/LoopLikeInterface/TestBlockInLoop.cpp:31:5
#<!-- -->21 0x00005fdcac41b593 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /mlir/lib/Pass/Pass.cpp:613:24
#<!-- -->22 0x00005fdcac41b593 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->23 0x00005fdcac40b27e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->24 0x00005fdcac40d026 llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->25 0x00005fdcac40d026 llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->26 0x00005fdcac40d026 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /mlir/lib/Pass/Pass.cpp:688:9
#<!-- -->27 0x00005fdcac41dbe5 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const /mlir/lib/Pass/Pass.cpp:1001:36
#<!-- -->28 0x00005fdcac41dbe5 auto void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)::operator()&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;) const /mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->29 0x00005fdcac41e0ae llvm::LogicalResult::failed() const /llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->30 0x00005fdcac41e0ae llvm::failed(llvm::LogicalResult) /llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->31 0x00005fdcac41e0ae llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'()::operator()() const /mlir/include/mlir/IR/Threading.h:62:11
#<!-- -->32 0x00005fdcac41e0ae void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;llvm::LogicalResult mlir::failableParallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, void mlir::parallelForEach&lt;__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'(__gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;&amp;&amp;)&gt;(mlir::MLIRContext*, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, __gnu_cxx::__normal_iterator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo*, std::vector&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo, std::allocator&lt;mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&gt;&gt;&gt;, mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::$_12&amp;&amp;)::'lambda'()&gt;(void*) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->33 0x00005fdcac2f9c20 std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;::operator()() const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:1442:20
#<!-- -->34 0x00005fdcac2f9a92 std::enable_if&lt;is_invocable_r_v&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt;, std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;&amp;&gt;, std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt;&gt;::type std::__invoke_r&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt;, std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;&amp;&gt;(std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/invoke.h:114:9
#<!-- -->35 0x00005fdcac2f9a92 std::_Function_handler&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; (), std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;&gt;::_M_invoke(std::_Any_data const&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:290:9
#<!-- -->36 0x00005fdcac2f9765 std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;::operator()() const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:0:9
#<!-- -->37 0x00005fdcac2f9765 std::__future_base::_State_baseV2::_M_do_set(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:589:27
#<!-- -->38 0x0000702aa98a1ed3 __pthread_once_slow ./nptl/pthread_once.c:118:7
#<!-- -->39 0x00005fdcac2f94aa void std::call_once&lt;void (std::__future_base::_State_baseV2::*)(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*), std::__future_base::_State_baseV2*, std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*&gt;(std::once_flag&amp;, void (std::__future_base::_State_baseV2::*&amp;&amp;)(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*), std::__future_base::_State_baseV2*&amp;&amp;, std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*&amp;&amp;, bool*&amp;&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/mutex:907:15
#<!-- -->40 0x00005fdcac2f94aa std::__future_base::_State_baseV2::_M_set_result(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;, bool) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:428:2
#<!-- -->41 0x00005fdcac2fab0a std::_Function_base::~_Function_base() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:243:11
#<!-- -->42 0x00005fdcac2fab0a std::__future_base::_Deferred_state&lt;std::thread::_Invoker&lt;std::tuple&lt;llvm::unique_function&lt;void ()&gt;&gt;&gt;, void&gt;::_M_complete_async() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:1705:9
#<!-- -->43 0x00005fdcac2fb191 std::__future_base::_State_baseV2::wait() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/future:353:2
#<!-- -->44 0x00005fdcacc86819 llvm::detail::PunnedPointer&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;&gt;::asInt() const /llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->45 0x00005fdcacc86819 llvm::detail::PunnedPointer&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;&gt;::operator long() const /llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->46 0x00005fdcacc86819 llvm::PointerIntPair&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;, 1u, bool, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;&gt;, llvm::PointerIntPairInfo&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;, 1u, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;llvm::detail::UniqueFunctionBase&lt;void&gt;::TrivialCallback*, llvm::detail::UniqueFunctionBase&lt;void&gt;::NonTrivialCallbacks*&gt;&gt;&gt;&gt;::getPointer() const /llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->47 0x00005fdcacc86819 llvm::detail::UniqueFunctionBase&lt;void&gt;::~UniqueFunctionBase() /llvm/include/llvm/ADT/FunctionExtras.h:274:32
#<!-- -->48 0x00005fdcacc86819 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) /llvm/lib/Support/ThreadPool.cpp:119:5
#<!-- -->49 0x00005fdcacc89844 std::default_delete&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&gt;::operator()(std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;*) const /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:99:2
#<!-- -->50 0x00005fdcacc89844 std::unique_ptr&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;, std::default_delete&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&gt;&gt;::~unique_ptr() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:404:4
#<!-- -->51 0x00005fdcacc89844 void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&gt;(void*) /llvm/include/llvm/Support/thread.h:49:3
#<!-- -->52 0x00005fdcacc89844 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&gt;(void*) /llvm/include/llvm/Support/thread.h:62:5
#<!-- -->53 0x00005fdc98518207 asan_thread_start(void*) -9b4c99/compiler-rt/lib/asan/asan_interceptors.cpp:306:28
#<!-- -->54 0x0000702aa989caa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->55 0x0000702aa9929c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
```



</details>


---

### Comment 2 - joker-eph

Removed ` --convert-gpu-to-nvvnm`, seems spurious since the crash is in the early pass.

---

