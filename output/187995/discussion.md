# lld-link crashes when linking clang-cl compiled thin-lto library

**Author:** Jimbopython
**URL:** https://github.com/llvm/llvm-project/issues/187995
**Status:** Open
**Labels:** backend:X86, crash, LTO, llvm:SelectionDAG

## Body

Compiler:
clang-cl: 20.1.8
Linker:
lld-link: 20.1.8
System:
Ubuntu 18.04 x64

The issue is triggered, when the static library (ZXing in this case), compiled with thin-lto is linked into a shared library or executable.

lld-link reports: LLVM ERROR: Do not know how to promote this operator's operand!

Stacktrace:
```
0.  Running pass 'Function Pass Manager' on module '/cache/p/zxingc557a5da05b6f/p/lib/ZXing.libcore\CMakeFiles\ZXing.dir\src\datamatrix\DMDetector.cpp.obj2891780'.
1.  Running pass 'X86 DAG->DAG Instruction Selection' on function '@"?Detect@DataMatrix@ZXing@@YA?AV?$Generator@VDetectorResult@ZXing@@@@AEBVBitMatrix@2@_N11@Z"'
 #0 0x000000000174f477 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/bin/lld-link+0x174f477)
 #1 0x000000000174d25e llvm::sys::RunSignalHandlers() (/usr/bin/lld-link+0x174d25e)
 #2 0x000000000174fdbf SignalHandler(int) Signals.cpp:0:0
 #3 0x00007e9816bc4980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #4 0x00007e98154dae87 raise (/lib/x86_64-linux-gnu/libc.so.6+0x3ee87)
 #5 0x00007e98154dc7f1 abort (/lib/x86_64-linux-gnu/libc.so.6+0x407f1)
 #6 0x00000000016c3dca llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/bin/lld-link+0x16c3dca)
 #7 0x00000000016c3c08 (/usr/bin/lld-link+0x16c3c08)
 #8 0x000000000283610b (/usr/bin/lld-link+0x283610b)
 #9 0x00000000027cdf64 llvm::DAGTypeLegalizer::run() LegalizeTypes.cpp:0:0
#10 0x00000000027d1ebe llvm::SelectionDAG::LegalizeTypes() (/usr/bin/lld-link+0x27d1ebe)
#11 0x00000000027be989 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/bin/lld-link+0x27be989)
#12 0x00000000027be2a4 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/usr/bin/lld-link+0x27be2a4)
#13 0x00000000027bbf0a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/usr/bin/lld-link+0x27bbf0a)
#14 0x00000000027ba3c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/usr/bin/lld-link+0x27ba3c1)
#15 0x0000000002c28606 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/bin/lld-link+0x2c28606)
#16 0x00000000042a23b1 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/bin/lld-link+0x42a23b1)
#17 0x00000000042a82c3 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/bin/lld-link+0x42a82c3)
#18 0x00000000042a2b5f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/bin/lld-link+0x42a2b5f)
#19 0x000000000295ea8a codegen(llvm::lto::Config const&, llvm::TargetMachine*, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, unsigned int, llvm::Module&, llvm::ModuleSummaryIndex const&) LTOBackend.cpp:0:0
#20 0x000000000295f9a2 llvm::lto::thinBackend(llvm::lto::Config const&, unsigned int, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::Module&, llvm::ModuleSummaryIndex const&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>*, bool, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, std::vector<unsigned char, std::allocator<unsigned char>> const&)::$_3::operator()(llvm::Module&, llvm::TargetMachine*, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>) const LTOBackend.cpp:0:0
#21 0x000000000295f7bb llvm::lto::thinBackend(llvm::lto::Config const&, unsigned int, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::Module&, llvm::ModuleSummaryIndex const&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>*, bool, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, std::vector<unsigned char, std::allocator<unsigned char>> const&) (/usr/bin/lld-link+0x295f7bb)
#22 0x00000000029562fe (anonymous namespace)::InProcessThinBackend::runThinLTOBackendThread(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::FileCache, unsigned int, llvm::BitcodeModule, llvm::ModuleSummaryIndex&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseSet<llvm::ValueInfo, llvm::DenseMapInfo<llvm::ValueInfo, void>> const&, std::map<unsigned long, llvm::GlobalValue::LinkageTypes, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::GlobalValue::LinkageTypes>>> const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>&)::'lambda'(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>)::operator()(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>) const LTO.cpp:0:0
#23 0x00000000029557a3 (anonymous namespace)::InProcessThinBackend::runThinLTOBackendThread(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::FileCache, unsigned int, llvm::BitcodeModule, llvm::ModuleSummaryIndex&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseSet<llvm::ValueInfo, llvm::DenseMapInfo<llvm::ValueInfo, void>> const&, std::map<unsigned long, llvm::GlobalValue::LinkageTypes, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::GlobalValue::LinkageTypes>>> const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>&) LTO.cpp:0:0
#24 0x0000000002955efe std::_Function_handler<void (), std::_Bind<(anonymous namespace)::InProcessThinBackend::start(unsigned int, llvm::BitcodeModule, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseSet<llvm::ValueInfo, llvm::DenseMapInfo<llvm::ValueInfo, void>> const&, std::map<unsigned long, llvm::GlobalValue::LinkageTypes, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::GlobalValue::LinkageTypes>>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>&)::'lambda'(llvm::BitcodeModule, llvm::ModuleSummaryIndex&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseSet<llvm::ValueInfo, llvm::DenseMapInfo<llvm::ValueInfo, void>> const&, std::map<unsigned long, llvm::GlobalValue::LinkageTypes, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::GlobalValue::LinkageTypes>>> const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>&) (llvm::BitcodeModule, std::reference_wrapper<llvm::ModuleSummaryIndex>, std::reference_wrapper<llvm::FunctionImporter::ImportMapTy const>, std::reference_wrapper<llvm::DenseSet<llvm::ValueInfo, llvm::DenseMapInfo<llvm::ValueInfo, void>> const>, std::reference_wrapper<std::map<unsigned long, llvm::GlobalValue::LinkageTypes, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, llvm::GlobalValue::LinkageTypes>>> const>, std::reference_wrapper<llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const>, std::reference_wrapper<llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>>)>>::_M_invoke(std::_Any_data const&) LTO.cpp:0:0
#25 0x0000000001b4ae78 std::_Function_handler<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> (), std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<std::function<void ()>>>, void>>::_M_invoke(std::_Any_data const&) BalancedPartitioning.cpp:0:0
#26 0x000000000179e767 std::__future_base::_State_baseV2::_M_do_set(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*) Driver.cpp:0:0
#27 0x00007e9816bc1907 __pthread_once_slow (/lib/x86_64-linux-gnu/libpthread.so.0+0xf907)
#28 0x0000000001b4b30b std::__future_base::_Deferred_state<std::thread::_Invoker<std::tuple<std::function<void ()>>>, void>::_M_complete_async() BalancedPartitioning.cpp:0:0
#29 0x0000000001b4b435 std::_Function_handler<void (), std::shared_future<void> llvm::ThreadPoolInterface::asyncImpl<void>(std::function<void ()>, llvm::ThreadPoolTaskGroup*)::'lambda'()>::_M_invoke(std::_Any_data const&) BalancedPartitioning.cpp:0:0
#30 0x0000000001b513a5 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) (/usr/bin/lld-link+0x1b513a5)
#31 0x0000000001b5267e void* llvm::thread::ThreadProxy<std::tuple<llvm::StdThreadPool::grow(int)::$_0>>(void*) ThreadPool.cpp:0:0
#32 0x00007e9816bb96db start_thread (/lib/x86_64-linux-gnu/libpthread.so.0+0x76db)
#33 0x00007e98155bd61f __clone (/lib/x86_64-linux-gnu/libc.so.6+0x12161f)
```

## Comments

### Comment 1 - frederick-vs-ja

LLVM 20 is no longer maintained. Could you try LLVM 22 or trunk?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (Jimbopython)

<details>
Compiler:
clang-cl: 20.1.8
Linker:
lld-link: 20.1.8
System:
Ubuntu 18.04 x64

The issue is triggered, when the static library (ZXing in this case), compiled with thin-lto is linked into a shared library or executable.

lld-link reports: LLVM ERROR: Do not know how to promote this operator's operand!

Stacktrace:
```
0.  Running pass 'Function Pass Manager' on module '/cache/p/zxingc557a5da05b6f/p/lib/ZXing.libcore\CMakeFiles\ZXing.dir\src\datamatrix\DMDetector.cpp.obj2891780'.
1.  Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@"?Detect@<!-- -->DataMatrix@<!-- -->ZXing@@<!-- -->YA?AV?$Generator@<!-- -->VDetectorResult@<!-- -->ZXing@@@@<!-- -->AEBVBitMatrix@<!-- -->2@<!-- -->_N11@<!-- -->Z"'
 #<!-- -->0 0x000000000174f477 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/bin/lld-link+0x174f477)
 #<!-- -->1 0x000000000174d25e llvm::sys::RunSignalHandlers() (/usr/bin/lld-link+0x174d25e)
 #<!-- -->2 0x000000000174fdbf SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x00007e9816bc4980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #<!-- -->4 0x00007e98154dae87 raise (/lib/x86_64-linux-gnu/libc.so.6+0x3ee87)
 #<!-- -->5 0x00007e98154dc7f1 abort (/lib/x86_64-linux-gnu/libc.so.6+0x407f1)
 #<!-- -->6 0x00000000016c3dca llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/usr/bin/lld-link+0x16c3dca)
 #<!-- -->7 0x00000000016c3c08 (/usr/bin/lld-link+0x16c3c08)
 #<!-- -->8 0x000000000283610b (/usr/bin/lld-link+0x283610b)
 #<!-- -->9 0x00000000027cdf64 llvm::DAGTypeLegalizer::run() LegalizeTypes.cpp:0:0
#<!-- -->10 0x00000000027d1ebe llvm::SelectionDAG::LegalizeTypes() (/usr/bin/lld-link+0x27d1ebe)
#<!-- -->11 0x00000000027be989 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/bin/lld-link+0x27be989)
#<!-- -->12 0x00000000027be2a4 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/usr/bin/lld-link+0x27be2a4)
#<!-- -->13 0x00000000027bbf0a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/bin/lld-link+0x27bbf0a)
#<!-- -->14 0x00000000027ba3c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/bin/lld-link+0x27ba3c1)
#<!-- -->15 0x0000000002c28606 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/bin/lld-link+0x2c28606)
#<!-- -->16 0x00000000042a23b1 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/bin/lld-link+0x42a23b1)
#<!-- -->17 0x00000000042a82c3 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/bin/lld-link+0x42a82c3)
#<!-- -->18 0x00000000042a2b5f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/bin/lld-link+0x42a2b5f)
#<!-- -->19 0x000000000295ea8a codegen(llvm::lto::Config const&amp;, llvm::TargetMachine*, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, unsigned int, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;) LTOBackend.cpp:0:0
#<!-- -->20 0x000000000295f9a2 llvm::lto::thinBackend(llvm::lto::Config const&amp;, unsigned int, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;*, bool, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, std::vector&lt;unsigned char, std::allocator&lt;unsigned char&gt;&gt; const&amp;)::$_3::operator()(llvm::Module&amp;, llvm::TargetMachine*, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;) const LTOBackend.cpp:0:0
#<!-- -->21 0x000000000295f7bb llvm::lto::thinBackend(llvm::lto::Config const&amp;, unsigned int, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;*, bool, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, std::vector&lt;unsigned char, std::allocator&lt;unsigned char&gt;&gt; const&amp;) (/usr/bin/lld-link+0x295f7bb)
#<!-- -->22 0x00000000029562fe (anonymous namespace)::InProcessThinBackend::runThinLTOBackendThread(std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::FileCache, unsigned int, llvm::BitcodeModule, llvm::ModuleSummaryIndex&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseSet&lt;llvm::ValueInfo, llvm::DenseMapInfo&lt;llvm::ValueInfo, void&gt;&gt; const&amp;, std::map&lt;unsigned long, llvm::GlobalValue::LinkageTypes, std::less&lt;unsigned long&gt;, std::allocator&lt;std::pair&lt;unsigned long const, llvm::GlobalValue::LinkageTypes&gt;&gt;&gt; const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;&amp;)::'lambda'(std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;)::operator()(std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;) const LTO.cpp:0:0
#<!-- -->23 0x00000000029557a3 (anonymous namespace)::InProcessThinBackend::runThinLTOBackendThread(std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::FileCache, unsigned int, llvm::BitcodeModule, llvm::ModuleSummaryIndex&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseSet&lt;llvm::ValueInfo, llvm::DenseMapInfo&lt;llvm::ValueInfo, void&gt;&gt; const&amp;, std::map&lt;unsigned long, llvm::GlobalValue::LinkageTypes, std::less&lt;unsigned long&gt;, std::allocator&lt;std::pair&lt;unsigned long const, llvm::GlobalValue::LinkageTypes&gt;&gt;&gt; const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;&amp;) LTO.cpp:0:0
#<!-- -->24 0x0000000002955efe std::_Function_handler&lt;void (), std::_Bind&lt;(anonymous namespace)::InProcessThinBackend::start(unsigned int, llvm::BitcodeModule, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseSet&lt;llvm::ValueInfo, llvm::DenseMapInfo&lt;llvm::ValueInfo, void&gt;&gt; const&amp;, std::map&lt;unsigned long, llvm::GlobalValue::LinkageTypes, std::less&lt;unsigned long&gt;, std::allocator&lt;std::pair&lt;unsigned long const, llvm::GlobalValue::LinkageTypes&gt;&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;&amp;)::'lambda'(llvm::BitcodeModule, llvm::ModuleSummaryIndex&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseSet&lt;llvm::ValueInfo, llvm::DenseMapInfo&lt;llvm::ValueInfo, void&gt;&gt; const&amp;, std::map&lt;unsigned long, llvm::GlobalValue::LinkageTypes, std::less&lt;unsigned long&gt;, std::allocator&lt;std::pair&lt;unsigned long const, llvm::GlobalValue::LinkageTypes&gt;&gt;&gt; const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;&amp;) (llvm::BitcodeModule, std::reference_wrapper&lt;llvm::ModuleSummaryIndex&gt;, std::reference_wrapper&lt;llvm::FunctionImporter::ImportMapTy const&gt;, std::reference_wrapper&lt;llvm::DenseSet&lt;llvm::ValueInfo, llvm::DenseMapInfo&lt;llvm::ValueInfo, void&gt;&gt; const&gt;, std::reference_wrapper&lt;std::map&lt;unsigned long, llvm::GlobalValue::LinkageTypes, std::less&lt;unsigned long&gt;, std::allocator&lt;std::pair&lt;unsigned long const, llvm::GlobalValue::LinkageTypes&gt;&gt;&gt; const&gt;, std::reference_wrapper&lt;llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&gt;, std::reference_wrapper&lt;llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;&gt;)&gt;&gt;::_M_invoke(std::_Any_data const&amp;) LTO.cpp:0:0
#<!-- -->25 0x0000000001b4ae78 std::_Function_handler&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; (), std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;std::function&lt;void ()&gt;&gt;&gt;, void&gt;&gt;::_M_invoke(std::_Any_data const&amp;) BalancedPartitioning.cpp:0:0
#<!-- -->26 0x000000000179e767 std::__future_base::_State_baseV2::_M_do_set(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*) Driver.cpp:0:0
#<!-- -->27 0x00007e9816bc1907 __pthread_once_slow (/lib/x86_64-linux-gnu/libpthread.so.0+0xf907)
#<!-- -->28 0x0000000001b4b30b std::__future_base::_Deferred_state&lt;std::thread::_Invoker&lt;std::tuple&lt;std::function&lt;void ()&gt;&gt;&gt;, void&gt;::_M_complete_async() BalancedPartitioning.cpp:0:0
#<!-- -->29 0x0000000001b4b435 std::_Function_handler&lt;void (), std::shared_future&lt;void&gt; llvm::ThreadPoolInterface::asyncImpl&lt;void&gt;(std::function&lt;void ()&gt;, llvm::ThreadPoolTaskGroup*)::'lambda'()&gt;::_M_invoke(std::_Any_data const&amp;) BalancedPartitioning.cpp:0:0
#<!-- -->30 0x0000000001b513a5 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) (/usr/bin/lld-link+0x1b513a5)
#<!-- -->31 0x0000000001b5267e void* llvm::thread::ThreadProxy&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&gt;(void*) ThreadPool.cpp:0:0
#<!-- -->32 0x00007e9816bb96db start_thread (/lib/x86_64-linux-gnu/libpthread.so.0+0x76db)
#<!-- -->33 0x00007e98155bd61f __clone (/lib/x86_64-linux-gnu/libc.so.6+0x12161f)
```
</details>


---

