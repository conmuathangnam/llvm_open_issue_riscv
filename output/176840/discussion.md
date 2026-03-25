# BUG: failed test for v22.1.0-rc1: `ExecutionEngine/JITLink/x86-64/ELF_vtune.s`

**Author:** h-vetinari
**URL:** https://github.com/llvm/llvm-project/issues/176840
**Status:** Closed
**Labels:** backend:X86, jitlink, crash
**Closed Date:** 2026-01-29T18:29:44Z

## Body

While [building](https://github.com/conda-forge/llvmdev-feedstock/pull/358) llvm v22.1.0-rc1 on linux-64 (GCC 14, glibc 2.17), I'm getting

```
[808/809] Running the LLVM regression tests
-- Testing: 60417 tests, 2 workers --
Testing:  0.. 10.. 20.. 30.. 40.. 50
FAIL: LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s (33370 of 60417)
******************** TEST 'LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s' FAILED ********************
Exit Code: 2

Command Output (stdout):
--
# RUN: at line 3
rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp && mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# RUN: at line 4
$SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux      -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# RUN: at line 6
$SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o |  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o
# .---command stderr------------
# | terminate called after throwing an instance of 'std::future_error'
# |   what():  std::future_error: No associated state
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# |  #0 0x00007f70596dd0bd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0xcdd0bd)
# |  #1 0x00007f70596da060 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
# |  #2 0x00007f7058436c30 __restore_rt (/lib64/libc.so.6+0x3fc30)
# |  #3 0x00007f705848402c __pthread_kill_implementation (/lib64/libc.so.6+0x8d02c)
# |  #4 0x00007f7058436b86 gsignal (/lib64/libc.so.6+0x3fb86)
# |  #5 0x00007f7058420873 abort (/lib64/libc.so.6+0x29873)
# |  #6 0x00007f70586aeaa4 __gnu_cxx::__verbose_terminate_handler() (.cold) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/vterminate.cc:75:10
# |  #7 0x00007f70586c0ffa __cxxabiv1::__terminate(void (*)()) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:48:15
# |  #8 0x00007f70586ae62e std::unexpected() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:71:1
# |  #9 0x00007f70586c1297 ($PREFIX/lib/libstdc++.so.6+0xc1297)
# | #10 0x00007f70586b1d89 std::__throw_future_error(int) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/future.cc:86:5
# | #11 0x00007f7059375581 llvm::Error llvm::detail::UniqueFunctionBase<llvm::Error, llvm::jitlink::LinkGraph&>::CallImpl<llvm::orc::ELFDebugObjectPlugin::modifyPassConfig(llvm::orc::MaterializationResponsibility&, llvm::jitlink::LinkGraph&, llvm::jitlink::PassConfiguration&)::'lambda0'(llvm::jitlink::LinkGraph&)>(void*, llvm::jitlink::LinkGraph&) (.cold) ELFDebugObjectPlugin.cpp:0:0
# | #12 0x00007f705c476c16 llvm::jitlink::JITLinkerBase::linkPhase3(std::unique_ptr<llvm::jitlink::JITLinkerBase, std::default_delete<llvm::jitlink::JITLinkerBase>>, llvm::Expected<llvm::DenseMap<llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef, llvm::DenseMapInfo<llvm::orc::SymbolStringPtr, void>, llvm::detail::DenseMapPair<llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef>>>) (.part.0) JITLinkGeneric.cpp:0:0
# | #13 0x00007f705c47742a llvm::jitlink::JITLinkerBase::linkPhase2(std::unique_ptr<llvm::jitlink::JITLinkerBase, std::default_delete<llvm::jitlink::JITLinkerBase>>, llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a7742a)
# | #14 0x00007f705c477529 void llvm::detail::UniqueFunctionBase<void, llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>>::CallImpl<llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr<llvm::jitlink::JITLinkerBase, std::default_delete<llvm::jitlink::JITLinkerBase>>)::'lambda'(llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>)>(void*, llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>&) JITLinkGeneric.cpp:0:0
# | #15 0x00007f705c63f413 llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&, llvm::unique_function<void (llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>)>)::'lambda'(llvm::Expected<llvm::orc::ExecutorAddrRange>)::operator()(llvm::Expected<llvm::orc::ExecutorAddrRange>) MapperJITLinkMemoryManager.cpp:0:0
# | #16 0x00007f705c6415ae llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&, llvm::unique_function<void (llvm::Expected<std::unique_ptr<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete<llvm::jitlink::JITLinkMemoryManager::InFlightAlloc>>>)>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c415ae)
# | #17 0x00007f705c478b82 llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr<llvm::jitlink::JITLinkerBase, std::default_delete<llvm::jitlink::JITLinkerBase>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a78b82)
# | #18 0x00007f705c4f00e4 llvm::jitlink::link_ELF_x86_64(std::unique_ptr<llvm::jitlink::LinkGraph, std::default_delete<llvm::jitlink::LinkGraph>>, std::unique_ptr<llvm::jitlink::JITLinkContext, std::default_delete<llvm::jitlink::JITLinkContext>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3af00e4)
# | #19 0x00007f705c4abcfe llvm::jitlink::link_ELF(std::unique_ptr<llvm::jitlink::LinkGraph, std::default_delete<llvm::jitlink::LinkGraph>>, std::unique_ptr<llvm::jitlink::JITLinkContext, std::default_delete<llvm::jitlink::JITLinkContext>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3aabcfe)
# | #20 0x00007f705c46f6c0 llvm::jitlink::link(std::unique_ptr<llvm::jitlink::LinkGraph, std::default_delete<llvm::jitlink::LinkGraph>>, std::unique_ptr<llvm::jitlink::JITLinkContext, std::default_delete<llvm::jitlink::JITLinkContext>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a6f6c0)
# | #21 0x00007f705c5f36e9 llvm::orc::LinkGraphLinkingLayer::emit(std::unique_ptr<llvm::orc::MaterializationResponsibility, std::default_delete<llvm::orc::MaterializationResponsibility>>, std::unique_ptr<llvm::jitlink::LinkGraph, std::default_delete<llvm::jitlink::LinkGraph>>, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3bf36e9)
# | #22 0x00007f705c6614ac llvm::orc::ObjectLinkingLayer::emit(std::unique_ptr<llvm::orc::MaterializationResponsibility, std::default_delete<llvm::orc::MaterializationResponsibility>>, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c614ac)
# | #23 0x00007f705c5e986e llvm::orc::BasicObjectLayerMaterializationUnit::materialize(std::unique_ptr<llvm::orc::MaterializationResponsibility, std::default_delete<llvm::orc::MaterializationResponsibility>>) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3be986e)
# | #24 0x00007f705c55b6f6 llvm::orc::MaterializationTask::run() ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3b5b6f6)
# | #25 0x00007f705c6a5032 std::thread::_State_impl<std::thread::_Invoker<std::tuple<llvm::orc::DynamicThreadPoolTaskDispatcher::dispatch(std::unique_ptr<llvm::orc::Task, std::default_delete<llvm::orc::Task>>)::'lambda'()>>>::_M_run() TaskDispatch.cpp:0:0
# | #26 0x00007f70586ef5e4 std::default_delete<std::thread::_State>::operator()(std::thread::_State*) const /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:93:2
# | #27 0x00007f70586ef5e4 std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State>>::~unique_ptr() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:399:17
# | #28 0x00007f70586ef5e4 execute_native_thread_routine /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/thread.cc:106:5
# | #29 0x00007f70584822ea start_thread (/lib64/libc.so.6+0x8b2ea)
# | #30 0x00007f70585065e4 __clone (/lib64/libc.so.6+0x10f5e4)
# `-----------------------------
# error: command failed with exit status: -6
# executed command: $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# .---command stderr------------
# | FileCheck error: '<stdin>' is empty.
# | FileCheck command line:  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# `-----------------------------
# error: command failed with exit status: 2

--

********************
Testing:  0.. 10.. 20.. 30.. 40.. 50.. 60.. 70.. 80.. 90.. 
********************
Failed Tests (1):
  LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s


Testing Time: 3229.39s

Total Discovered Tests: 71391
  Skipped          :    17 (0.02%)
  Unsupported      :  2436 (3.41%)
  Passed           : 68758 (96.31%)
  Expectedly Failed:   179 (0.25%)
  Failed           :     1 (0.00%)
FAILED: [code=1] test/CMakeFiles/check-llvm $SRC_DIR/build/test/CMakeFiles/check-llvm 
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-jitlink

Author: None (h-vetinari)

<details>
While [building](https://github.com/conda-forge/llvmdev-feedstock/pull/358) llvm v22.1.0-rc1 on linux-64 (GCC 14, glibc 2.17), I'm getting

```
[808/809] Running the LLVM regression tests
-- Testing: 60417 tests, 2 workers --
Testing:  0.. 10.. 20.. 30.. 40.. 50
FAIL: LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s (33370 of 60417)
******************** TEST 'LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s' FAILED ********************
Exit Code: 2

Command Output (stdout):
--
# RUN: at line 3
rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp &amp;&amp; mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# RUN: at line 4
$SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux      -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# RUN: at line 6
$SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o |  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o
# .---command stderr------------
# | terminate called after throwing an instance of 'std::future_error'
# |   what():  std::future_error: No associated state
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# |  #<!-- -->0 0x00007f70596dd0bd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0xcdd0bd)
# |  #<!-- -->1 0x00007f70596da060 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
# |  #<!-- -->2 0x00007f7058436c30 __restore_rt (/lib64/libc.so.6+0x3fc30)
# |  #<!-- -->3 0x00007f705848402c __pthread_kill_implementation (/lib64/libc.so.6+0x8d02c)
# |  #<!-- -->4 0x00007f7058436b86 gsignal (/lib64/libc.so.6+0x3fb86)
# |  #<!-- -->5 0x00007f7058420873 abort (/lib64/libc.so.6+0x29873)
# |  #<!-- -->6 0x00007f70586aeaa4 __gnu_cxx::__verbose_terminate_handler() (.cold) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/vterminate.cc:75:10
# |  #<!-- -->7 0x00007f70586c0ffa __cxxabiv1::__terminate(void (*)()) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:48:15
# |  #<!-- -->8 0x00007f70586ae62e std::unexpected() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:71:1
# |  #<!-- -->9 0x00007f70586c1297 ($PREFIX/lib/libstdc++.so.6+0xc1297)
# | #<!-- -->10 0x00007f70586b1d89 std::__throw_future_error(int) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/future.cc:86:5
# | #<!-- -->11 0x00007f7059375581 llvm::Error llvm::detail::UniqueFunctionBase&lt;llvm::Error, llvm::jitlink::LinkGraph&amp;&gt;::CallImpl&lt;llvm::orc::ELFDebugObjectPlugin::modifyPassConfig(llvm::orc::MaterializationResponsibility&amp;, llvm::jitlink::LinkGraph&amp;, llvm::jitlink::PassConfiguration&amp;)::'lambda0'(llvm::jitlink::LinkGraph&amp;)&gt;(void*, llvm::jitlink::LinkGraph&amp;) (.cold) ELFDebugObjectPlugin.cpp:0:0
# | #<!-- -->12 0x00007f705c476c16 llvm::jitlink::JITLinkerBase::linkPhase3(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;, llvm::Expected&lt;llvm::DenseMap&lt;llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef, llvm::DenseMapInfo&lt;llvm::orc::SymbolStringPtr, void&gt;, llvm::detail::DenseMapPair&lt;llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef&gt;&gt;&gt;) (.part.0) JITLinkGeneric.cpp:0:0
# | #<!-- -->13 0x00007f705c47742a llvm::jitlink::JITLinkerBase::linkPhase2(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a7742a)
# | #<!-- -->14 0x00007f705c477529 void llvm::detail::UniqueFunctionBase&lt;void, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;&gt;::CallImpl&lt;llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;)::'lambda'(llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;(void*, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;&amp;) JITLinkGeneric.cpp:0:0
# | #<!-- -->15 0x00007f705c63f413 llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;)::'lambda'(llvm::Expected&lt;llvm::orc::ExecutorAddrRange&gt;)::operator()(llvm::Expected&lt;llvm::orc::ExecutorAddrRange&gt;) MapperJITLinkMemoryManager.cpp:0:0
# | #<!-- -->16 0x00007f705c6415ae llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c415ae)
# | #<!-- -->17 0x00007f705c478b82 llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a78b82)
# | #<!-- -->18 0x00007f705c4f00e4 llvm::jitlink::link_ELF_x86_64(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3af00e4)
# | #<!-- -->19 0x00007f705c4abcfe llvm::jitlink::link_ELF(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3aabcfe)
# | #<!-- -->20 0x00007f705c46f6c0 llvm::jitlink::link(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a6f6c0)
# | #<!-- -->21 0x00007f705c5f36e9 llvm::orc::LinkGraphLinkingLayer::emit(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3bf36e9)
# | #<!-- -->22 0x00007f705c6614ac llvm::orc::ObjectLinkingLayer::emit(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c614ac)
# | #<!-- -->23 0x00007f705c5e986e llvm::orc::BasicObjectLayerMaterializationUnit::materialize(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3be986e)
# | #<!-- -->24 0x00007f705c55b6f6 llvm::orc::MaterializationTask::run() ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3b5b6f6)
# | #<!-- -->25 0x00007f705c6a5032 std::thread::_State_impl&lt;std::thread::_Invoker&lt;std::tuple&lt;llvm::orc::DynamicThreadPoolTaskDispatcher::dispatch(std::unique_ptr&lt;llvm::orc::Task, std::default_delete&lt;llvm::orc::Task&gt;&gt;)::'lambda'()&gt;&gt;&gt;::_M_run() TaskDispatch.cpp:0:0
# | #<!-- -->26 0x00007f70586ef5e4 std::default_delete&lt;std::thread::_State&gt;::operator()(std::thread::_State*) const /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:93:2
# | #<!-- -->27 0x00007f70586ef5e4 std::unique_ptr&lt;std::thread::_State, std::default_delete&lt;std::thread::_State&gt;&gt;::~unique_ptr() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:399:17
# | #<!-- -->28 0x00007f70586ef5e4 execute_native_thread_routine /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/thread.cc:106:5
# | #<!-- -->29 0x00007f70584822ea start_thread (/lib64/libc.so.6+0x8b2ea)
# | #<!-- -->30 0x00007f70585065e4 __clone (/lib64/libc.so.6+0x10f5e4)
# `-----------------------------
# error: command failed with exit status: -6
# executed command: $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# .---command stderr------------
# | FileCheck error: '&lt;stdin&gt;' is empty.
# | FileCheck command line:  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# `-----------------------------
# error: command failed with exit status: 2

--

********************
Testing:  0.. 10.. 20.. 30.. 40.. 50.. 60.. 70.. 80.. 90.. 
********************
Failed Tests (1):
  LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s


Testing Time: 3229.39s

Total Discovered Tests: 71391
  Skipped          :    17 (0.02%)
  Unsupported      :  2436 (3.41%)
  Passed           : 68758 (96.31%)
  Expectedly Failed:   179 (0.25%)
  Failed           :     1 (0.00%)
FAILED: [code=1] test/CMakeFiles/check-llvm $SRC_DIR/build/test/CMakeFiles/check-llvm 
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (h-vetinari)

<details>
While [building](https://github.com/conda-forge/llvmdev-feedstock/pull/358) llvm v22.1.0-rc1 on linux-64 (GCC 14, glibc 2.17), I'm getting

```
[808/809] Running the LLVM regression tests
-- Testing: 60417 tests, 2 workers --
Testing:  0.. 10.. 20.. 30.. 40.. 50
FAIL: LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s (33370 of 60417)
******************** TEST 'LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s' FAILED ********************
Exit Code: 2

Command Output (stdout):
--
# RUN: at line 3
rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp &amp;&amp; mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: rm -rf $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# executed command: mkdir $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp
# RUN: at line 4
$SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux      -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-mc -triple=x86_64-unknown-linux -filetype=obj -o $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# RUN: at line 6
$SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o |  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# executed command: $SRC_DIR/build/bin/llvm-jitlink -vtune-support $SRC_DIR/build/test/ExecutionEngine/JITLink/x86-64/Output/ELF_vtune.s.tmp/ELF_x86-64_vtune.o
# .---command stderr------------
# | terminate called after throwing an instance of 'std::future_error'
# |   what():  std::future_error: No associated state
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# |  #<!-- -->0 0x00007f70596dd0bd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0xcdd0bd)
# |  #<!-- -->1 0x00007f70596da060 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
# |  #<!-- -->2 0x00007f7058436c30 __restore_rt (/lib64/libc.so.6+0x3fc30)
# |  #<!-- -->3 0x00007f705848402c __pthread_kill_implementation (/lib64/libc.so.6+0x8d02c)
# |  #<!-- -->4 0x00007f7058436b86 gsignal (/lib64/libc.so.6+0x3fb86)
# |  #<!-- -->5 0x00007f7058420873 abort (/lib64/libc.so.6+0x29873)
# |  #<!-- -->6 0x00007f70586aeaa4 __gnu_cxx::__verbose_terminate_handler() (.cold) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/vterminate.cc:75:10
# |  #<!-- -->7 0x00007f70586c0ffa __cxxabiv1::__terminate(void (*)()) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:48:15
# |  #<!-- -->8 0x00007f70586ae62e std::unexpected() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/libsupc++/../../../../libstdc++-v3/libsupc++/eh_terminate.cc:71:1
# |  #<!-- -->9 0x00007f70586c1297 ($PREFIX/lib/libstdc++.so.6+0xc1297)
# | #<!-- -->10 0x00007f70586b1d89 std::__throw_future_error(int) /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/future.cc:86:5
# | #<!-- -->11 0x00007f7059375581 llvm::Error llvm::detail::UniqueFunctionBase&lt;llvm::Error, llvm::jitlink::LinkGraph&amp;&gt;::CallImpl&lt;llvm::orc::ELFDebugObjectPlugin::modifyPassConfig(llvm::orc::MaterializationResponsibility&amp;, llvm::jitlink::LinkGraph&amp;, llvm::jitlink::PassConfiguration&amp;)::'lambda0'(llvm::jitlink::LinkGraph&amp;)&gt;(void*, llvm::jitlink::LinkGraph&amp;) (.cold) ELFDebugObjectPlugin.cpp:0:0
# | #<!-- -->12 0x00007f705c476c16 llvm::jitlink::JITLinkerBase::linkPhase3(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;, llvm::Expected&lt;llvm::DenseMap&lt;llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef, llvm::DenseMapInfo&lt;llvm::orc::SymbolStringPtr, void&gt;, llvm::detail::DenseMapPair&lt;llvm::orc::SymbolStringPtr, llvm::orc::ExecutorSymbolDef&gt;&gt;&gt;) (.part.0) JITLinkGeneric.cpp:0:0
# | #<!-- -->13 0x00007f705c47742a llvm::jitlink::JITLinkerBase::linkPhase2(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a7742a)
# | #<!-- -->14 0x00007f705c477529 void llvm::detail::UniqueFunctionBase&lt;void, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;&gt;::CallImpl&lt;llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;)::'lambda'(llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;(void*, llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;&amp;) JITLinkGeneric.cpp:0:0
# | #<!-- -->15 0x00007f705c63f413 llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;)::'lambda'(llvm::Expected&lt;llvm::orc::ExecutorAddrRange&gt;)::operator()(llvm::Expected&lt;llvm::orc::ExecutorAddrRange&gt;) MapperJITLinkMemoryManager.cpp:0:0
# | #<!-- -->16 0x00007f705c6415ae llvm::orc::MapperJITLinkMemoryManager::allocate(llvm::jitlink::JITLinkDylib const*, llvm::jitlink::LinkGraph&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::unique_ptr&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc, std::default_delete&lt;llvm::jitlink::JITLinkMemoryManager::InFlightAlloc&gt;&gt;&gt;)&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c415ae)
# | #<!-- -->17 0x00007f705c478b82 llvm::jitlink::JITLinkerBase::linkPhase1(std::unique_ptr&lt;llvm::jitlink::JITLinkerBase, std::default_delete&lt;llvm::jitlink::JITLinkerBase&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a78b82)
# | #<!-- -->18 0x00007f705c4f00e4 llvm::jitlink::link_ELF_x86_64(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3af00e4)
# | #<!-- -->19 0x00007f705c4abcfe llvm::jitlink::link_ELF(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3aabcfe)
# | #<!-- -->20 0x00007f705c46f6c0 llvm::jitlink::link(std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::JITLinkContext, std::default_delete&lt;llvm::jitlink::JITLinkContext&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3a6f6c0)
# | #<!-- -->21 0x00007f705c5f36e9 llvm::orc::LinkGraphLinkingLayer::emit(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;, std::unique_ptr&lt;llvm::jitlink::LinkGraph, std::default_delete&lt;llvm::jitlink::LinkGraph&gt;&gt;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3bf36e9)
# | #<!-- -->22 0x00007f705c6614ac llvm::orc::ObjectLinkingLayer::emit(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3c614ac)
# | #<!-- -->23 0x00007f705c5e986e llvm::orc::BasicObjectLayerMaterializationUnit::materialize(std::unique_ptr&lt;llvm::orc::MaterializationResponsibility, std::default_delete&lt;llvm::orc::MaterializationResponsibility&gt;&gt;) ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3be986e)
# | #<!-- -->24 0x00007f705c55b6f6 llvm::orc::MaterializationTask::run() ($SRC_DIR/build/bin/../lib/libLLVM.so.22.1-rc1+0x3b5b6f6)
# | #<!-- -->25 0x00007f705c6a5032 std::thread::_State_impl&lt;std::thread::_Invoker&lt;std::tuple&lt;llvm::orc::DynamicThreadPoolTaskDispatcher::dispatch(std::unique_ptr&lt;llvm::orc::Task, std::default_delete&lt;llvm::orc::Task&gt;&gt;)::'lambda'()&gt;&gt;&gt;::_M_run() TaskDispatch.cpp:0:0
# | #<!-- -->26 0x00007f70586ef5e4 std::default_delete&lt;std::thread::_State&gt;::operator()(std::thread::_State*) const /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:93:2
# | #<!-- -->27 0x00007f70586ef5e4 std::unique_ptr&lt;std::thread::_State, std::default_delete&lt;std::thread::_State&gt;&gt;::~unique_ptr() /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/include/bits/unique_ptr.h:399:17
# | #<!-- -->28 0x00007f70586ef5e4 execute_native_thread_routine /home/conda/feedstock_root/build_artifacts/gcc_compilers_1765252935691/work/build/x86_64-conda-linux-gnu/libstdc++-v3/src/c++11/../../../../../libstdc++-v3/src/c++11/thread.cc:106:5
# | #<!-- -->29 0x00007f70584822ea start_thread (/lib64/libc.so.6+0x8b2ea)
# | #<!-- -->30 0x00007f70585065e4 __clone (/lib64/libc.so.6+0x10f5e4)
# `-----------------------------
# error: command failed with exit status: -6
# executed command: $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# .---command stderr------------
# | FileCheck error: '&lt;stdin&gt;' is empty.
# | FileCheck command line:  $SRC_DIR/build/bin/FileCheck $SRC_DIR/llvm/test/ExecutionEngine/JITLink/x86-64/ELF_vtune.s
# `-----------------------------
# error: command failed with exit status: 2

--

********************
Testing:  0.. 10.. 20.. 30.. 40.. 50.. 60.. 70.. 80.. 90.. 
********************
Failed Tests (1):
  LLVM :: ExecutionEngine/JITLink/x86-64/ELF_vtune.s


Testing Time: 3229.39s

Total Discovered Tests: 71391
  Skipped          :    17 (0.02%)
  Unsupported      :  2436 (3.41%)
  Passed           : 68758 (96.31%)
  Expectedly Failed:   179 (0.25%)
  Failed           :     1 (0.00%)
FAILED: [code=1] test/CMakeFiles/check-llvm $SRC_DIR/build/test/CMakeFiles/check-llvm 
```
</details>


---

### Comment 3 - lhames

The future error indicates that some other failure tore down the JIT, including the system holding the promise.

Without a local reproducer (and given my current workload) I don't think we'll have a fix for this in time for the release. I think it's reasonable to disable the test either in the release branch, or selectively in downstream repos.

---

### Comment 4 - h-vetinari

Thanks for the quick response!

> Without a local reproducer [...]

I mean, the test itself is the reproducer? It "just" doesn't reproduce on every system necessarily. That's why I mentioned compiler & glibc baselines. It does reproduce in our CI, and I'm happy to test patches (or add debug stuff). It's a bit of a pain because it requires building llvm first (stuff is not persisted between runs). I may be able to get a downloadable snapshot of the environment during the build, including all the temporary artefacts, in case you want to look at that. It'll be pretty big to download though, at least 2 GB I think.


---

### Comment 5 - lhames

By "local reproducer" I mean a failure that I can look at locally.

I do think we should investigate, but given the constraints (my bandwidth, and the ones that you cited) I suspect that this won't be a quick fix. If we need this for the release I'd be inclined to disable the test.

---

### Comment 6 - mshockwave

Potential solution: #178541 

---

### Comment 7 - mshockwave

Closed by #178541 as mentioned here: https://github.com/llvm/llvm-project/pull/178541#issuecomment-3815717842

---

