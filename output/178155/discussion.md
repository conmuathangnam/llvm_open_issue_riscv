# [lldb-dap] Crash on startup on Windows

**Author:** DrSergei
**URL:** https://github.com/llvm/llvm-project/issues/178155
**Status:** Open
**Labels:** crash, lldb-dap

## Body

I tried to build latest lldb and lldb-dap on Windows. LLDB works fine, but lldb-dap crashes after startup without messages.

Build command:
```
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DLLDB_EXPORT_ALL_SYMBOLS=On -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DCMAKE_C_FLAGS=" /MT" -DCMAKE_CXX_FLAGS=" /MT" -DCMAKE_INSTALL_PREFIX=install -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DLLVM_ENABLE_PROJECTS="clang;lld;lldb" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCLANG_ENABLE_LIBXML2=Off -DLLDB_ENABLE_LIBXML2=Off -DLLDB_ENABLE_PYTHON=Off -DLLVM_ENABLE_ZLIB=Off -DLLVM_ENABLE_ZSTD=Off -DLLDB_ENABLE_LZMA=Off -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=Off ../llvm
ninja lldb lldb-dap lldb-server
```

Running lldb-dap under lldb (from the same build):
```
PS D:\projects\llvm-project\build-nopython-debug\bin> ./lldb.exe ./lldb-dap.exe
(lldb) target create "./lldb-dap.exe"
Current executable set to 'D:\projects\llvm-project\build-nopython-debug\bin\lldb-dap.exe' (x86_64).
(lldb) run
Process 18764 launched: 'D:\projects\llvm-project\build-nopython-debug\bin\lldb-dap.exe' (x86_64)
Process 18764 stopped
* thread #1, stop reason = Exception 0xc0000409 encountered at address 0x7ff87112de18
    frame #0: 0x00007ff87112de18 liblldb.dll`_invoke_watson(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:237

(lldb) error: no auto repeat
(lldb) bt
* thread #1, stop reason = Exception 0xc0000409 encountered at address 0x7ff87112de18
  * frame #0: 0x00007ff87112de18 liblldb.dll`_invoke_watson(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:237
    frame #1: 0x00007ff87112dd67 liblldb.dll`_invalid_parameter_internal(expression=0x0000000000000000, function_name=0x0000000000000000, file_name=0x0000000000000000, line_number=1798886096, reserved=<unavailable>, ptd=<unavailable>) at invalid_parameter.cpp:113
    frame #2: 0x00007ff87112dc2a liblldb.dll`_invalid_parameter(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:125
    frame #3: 0x00007ff87112ddb5 liblldb.dll`_invalid_parameter_noinfo at invalid_parameter.cpp:130
    frame #4: 0x00007ff871163930 liblldb.dll`_get_osfhandle(fh=<unavailable>) at osfinfo.cpp:264
    frame #5: 0x00007ff85fda12f1 liblldb.dll`lldb_private::NativeFile::NativeFile(this=0x0000021917c311d0, fd=3, options=eOpenOptionWriteOnly) at File.cpp:291
    frame #6: 0x00007ff66e4da81a lldb-dap.exe`void std::_Construct_in_place<class lldb_private::NativeFile, int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(_Obj=0x0000021917c311d0, <_Args_0>=0x000000b06b38e004, <_Args_1>=0x000000b06b38f7fc, <_Args_2>=0x000000b06b38f7f8) at xutility:506
    frame #7: 0x00007ff66e4d74c0 lldb-dap.exe`std::_Ref_count_obj2<class lldb_private::NativeFile>::_Ref_count_obj2<class lldb_private::NativeFile><int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(this=0x0000021917c311c0, <_Args_0>=0x000000b06b38e004, <_Args_1>=0x000000b06b38f7fc) at memory:2092
    frame #8: 0x00007ff66e4e0c3b lldb-dap.exe`class std::shared_ptr<class lldb_private::NativeFile> std::make_shared<class lldb_private::NativeFile, int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(<_Args_0>=0x000000b06b38e004, <_Args_1>=0x000000b06b38f7fc, <_Args_2>=0x000000b06b38f7f8) at memory:2903
    frame #9: 0x00007ff66e4d4854 lldb-dap.exe`main(argc=1, argv=0x0000021917b6ec60) at lldb-dap.cpp:735
    frame #10: 0x00007ff66e90d580 lldb-dap.exe`invoke_main at exe_common.inl:288 [inlined]
    frame #11: 0x00007ff66e90d55e lldb-dap.exe`__scrt_common_main_seh at exe_common.inl:288
    frame #12: 0x00007ff99a21e8d7 kernel32.dll`BaseThreadInitThunk + 23
    frame #13: 0x00007ff99af2c53c ntdll.dll`RtlUserThreadStart + 44
(lldb) exit
Quitting LLDB will kill one or more processes. Do you really want to proceed: [Y/n] y
```

CC: @charles-zablit 

## Comments

### Comment 1 - charles-zablit

I hit an assertion crash in lldb when trying to reproduce this with top of tree (b7c68c3a394660fd2a8e8cf496ebb5c684aa84de):

```log
./llvm-build/debug-oft/bin/lldb.exe ./llvm-build/debug-oft/bin/lldb-dap.exe
sys:1: DeprecationWarning: builtin type SwigPyPacked has no __module__ attribute
sys:1: DeprecationWarning: builtin type SwigPyObject has no __module__ attribute
(lldb) target create "./llvm-build/debug-oft/bin/lldb-dap.exe"
Assertion failed: (i == Size || Traits::stopLess(b, start(i))) && "Overlapping insert", file C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm/ADT/IntervalMap.h, line 639
LLDB diagnostics will be written to C:\Users\charleszablit\AppData\Local\Temp\diagnostics-da471a
Please include the directory content when filing a bug report
Exception Code: 0x80000003
 #0 0x00007ff78991135c HandleAbort C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\lib\Support\Windows\Signals.inc:371:0
 #1 0x00007ffe75a09e77 (C:\windows\SYSTEM32\ucrtbased.dll+0x99e77)
 #2 0x00007ffe75a0bab9 (C:\windows\SYSTEM32\ucrtbased.dll+0x9bab9)
 #3 0x00007ffe75a11715 (C:\windows\SYSTEM32\ucrtbased.dll+0xa1715)
 #4 0x00007ffe75a11387 (C:\windows\SYSTEM32\ucrtbased.dll+0xa1387)
 #5 0x00007ffe75a0f351 (C:\windows\SYSTEM32\ucrtbased.dll+0x9f351)
 #6 0x00007ffe75a11c7f (C:\windows\SYSTEM32\ucrtbased.dll+0xa1c7f)
 #7 0x00007ffe0d80bca9 llvm::IntervalMapImpl::LeafNode<unsigned __int64, unsigned short, 10, struct llvm::IntervalMapInfo<unsigned __int64>>::insertFrom(unsigned int &, unsigned int, unsigned __int64, unsigned __int64, unsigned short) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\IntervalMap.h:639:0
 #8 0x00007ffe0d80e5d0 llvm::IntervalMap<unsigned __int64, unsigned short, 10, struct llvm::IntervalMapInfo<unsigned __int64>>::iterator::treeInsert(unsigned __int64, unsigned __int64, unsigned short) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\IntervalMap.h:1902:0
 #9 0x00007ffe0d80b910 llvm::IntervalMap<unsigned __int64, unsigned short, 10, struct llvm::IntervalMapInfo<unsigned __int64>>::iterator::insert(unsigned __int64, unsigned __int64, unsigned short) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\IntervalMap.h:1836:0
#10 0x00007ffe0d80b725 llvm::IntervalMap<unsigned __int64, unsigned short, 10, struct llvm::IntervalMapInfo<unsigned __int64>>::insert(unsigned __int64, unsigned __int64, unsigned short) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\IntervalMap.h:1132:0
#11 0x00007ffe0d8035f8 `lldb_private::npdb::PdbIndex::ParseSectionContribs'::`2'::Visitor::visit C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Plugins\SymbolFile\NativePDB\PdbIndex.cpp:108:0
#12 0x00007ffe17d9c617 llvm::pdb::DbiStream::visitSectionContributions(class llvm::pdb::ISectionContribVisitor &) const C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\lib\DebugInfo\PDB\Native\DbiStream.cpp:226:0
#13 0x00007ffe0d802c3b lldb_private::npdb::PdbIndex::ParseSectionContribs(void) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Plugins\SymbolFile\NativePDB\PdbIndex.cpp:114:0
#14 0x00007ffe0d76ca05 lldb_private::npdb::SymbolFileNativePDB::InitializeObject(void) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Plugins\SymbolFile\NativePDB\SymbolFileNativePDB.cpp:426:0
#15 0x00007ffe0b61d107 lldb_private::SymbolFile::FindPlugin(class std::shared_ptr<class lldb_private::ObjectFile>) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Symbol\SymbolFile.cpp:102:0
#16 0x00007ffe0b5eeff3 lldb_private::SymbolVendor::AddSymbolFileRepresentation(class std::shared_ptr<class lldb_private::ObjectFile> const &) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Symbol\SymbolVendor.cpp:69:0
#17 0x00007ffe0b5eee52 lldb_private::SymbolVendor::FindPlugin(class std::shared_ptr<class lldb_private::Module> const &, class lldb_private::Stream *) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Symbol\SymbolVendor.cpp:55:0
#18 0x00007ffe0b2cf657 lldb_private::Module::GetSymbolFile(bool, class lldb_private::Stream *) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Core\Module.cpp:1023:0
#19 0x00007ffe0b2cd405 lldb_private::Module::PreloadSymbols(void) C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Core\Module.cpp:1347:0
#20 0x00007ffe0b1d45e3 `lldb_private::ModuleList::PreloadSymbols'::`11'::<lambda_1>::operator() C:\Users\charleszablit\Developer\lldb\llvm-project\lldb\source\Core\ModuleList.cpp:1402:0
#21 0x00007ffe0b1cc21e llvm::detail::UniqueFunctionBase<void>::CallImpl<`lldb_private::ModuleList::PreloadSymbols'::`11'::<lambda_1> > C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\FunctionExtras.h:212:0
#22 0x00007ffe0b1e7d81 llvm::unique_function<(void)>::operator()(void) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\FunctionExtras.h:364:0
#23 0x00007ffe0b1de9e4 std::invoke<class llvm::unique_function<(void)>>(class llvm::unique_function<(void)> &&) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\type_traits:1670:0
#24 0x00007ffe0b1db320 std::_Invoke_stored_explicit<class llvm::unique_function<(void)>, 0>(class std::tuple<class llvm::unique_function<(void)>> &&, struct std::integer_sequence<unsigned __int64, 0>) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:1310:0
#25 0x00007ffe0b1db2ea std::_Invoke_stored<class llvm::unique_function<(void)>>(class std::tuple<class llvm::unique_function<(void)>> &&) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:1315:0
#26 0x00007ffe0b1e7ac7 std::_Fake_no_copy_callable_adapter<class llvm::unique_function<(void)>>::operator()(void) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:1342:0
#27 0x00007ffe0b1de914 std::invoke<class std::_Fake_no_copy_callable_adapter<class llvm::unique_function<(void)>> &>(class std::_Fake_no_copy_callable_adapter<class llvm::unique_function<(void)>> &) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\type_traits:1670:0
#28 0x00007ffe0b1eb22b std::_Func_impl_no_alloc<class std::_Fake_no_copy_callable_adapter<class llvm::unique_function<(void)>>, void>::_Do_call(void) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\functional:880:0
#29 0x00007ffe0b1e7bf0 std::_Func_class<void>::operator()(void) const C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\functional:926:0
#30 0x00007ffe0b1e9324 std::_Packaged_state<(void)>::_Call_immediate(void) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:533:0
#31 0x00007ffe0b1ed8a3 std::_Deferred_async_state<void>::_Run_deferred_function(class std::unique_lock<class std::mutex> &) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:608:0
#32 0x00007ffe0b1ec70a std::_Associated_state<int>::_Maybe_run_deferred_function(class std::unique_lock<class std::mutex> &) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:396:0
#33 0x00007ffe0b1ef095 std::_Associated_state<int>::_Wait(void) C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:238:0
#34 0x00007ffe0b1f4cac std::_State_manager<int>::wait(void) const C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\future:708:0
#35 0x00007ffe0b1e7617 `private: class std::shared_future<void> __cdecl llvm::ThreadPoolInterface::asyncImpl<void>(class llvm::unique_function<void __cdecl(void)>, class llvm::ThreadPoolTaskGroup *)'::`2'::<lambda_1>::operator()(void) const C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\Support\ThreadPool.h:116:0
#36 0x00007ffe0b1d89be llvm::detail::UniqueFunctionBase<void>::CallImpl<class `private: class std::shared_future<void> __cdecl llvm::ThreadPoolInterface::asyncImpl<void>(class llvm::unique_function<void __cdecl(void)>, class llvm::ThreadPoolTaskGroup *)'::`2'::<lambda_1>>(void *) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\FunctionExtras.h:212:0
#37 0x00007ffe0b1e7d81 llvm::unique_function<(void)>::operator()(void) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\ADT\FunctionExtras.h:364:0
#38 0x00007ffe0c6abe54 llvm::StdThreadPool::processTasks(class llvm::ThreadPoolTaskGroup *) C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\lib\Support\ThreadPool.cpp:119:0
#39 0x00007ffe0c6aca5b `llvm::StdThreadPool::grow'::`6'::<lambda_1>::operator() C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\lib\Support\ThreadPool.cpp:62:0
#40 0x00007ffe0c6adde9 `llvm::thread::GenericThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > >'::`2'::<lambda_1>::operator()<`llvm::StdThreadPool::grow'::`6'::<lambda_1> &> C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\Support\thread.h:46:0
#41 0x00007ffe0c6b110e std::invoke<`llvm::thread::GenericThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > >'::`2'::<lambda_1>,`llvm::StdThreadPool::grow'::`6'::<lambda_1> &> C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\type_traits:1680:0 
#42 0x00007ffe0c6ae3fb std::_Apply_impl<`llvm::thread::GenericThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > >'::`2'::<lambda_1>,std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > &,0> C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\tuple:1058:0
#43 0x00007ffe0c6b00b5 std::apply<`llvm::thread::GenericThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > >'::`2'::<lambda_1>,std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > &> C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\include\tuple:1069:0
#44 0x00007ffe0c6aded8 llvm::thread::GenericThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > > C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\Support\thread.h:44:0
#45 0x00007ffe0c6ae1f4 llvm::thread::ThreadProxy<std::tuple<`llvm::StdThreadPool::grow'::`6'::<lambda_1> > > C:\Users\charleszablit\Developer\lldb\llvm-project\llvm\include\llvm\Support\thread.h:73:0
#46 0x00007ffe75a12ec5 (C:\windows\SYSTEM32\ucrtbased.dll+0xa2ec5)
#47 0x00007ffef7f8e8d7 (C:\windows\System32\KERNEL32.DLL+0x2e8d7)
#48 0x00007ffef930c53c (C:\windows\SYSTEM32\ntdll.dll+0x8c53c)
```

However, I'm not reproducing your crash when lldb 21, `lldb-dap` just returns 1.

```
&"C:\Program Files\LLVM\bin\lldb.exe" "./llvm-build/debug-oft/bin/lldb-dap.exe"
(lldb) target create "./llvm-build/debug-oft/bin/lldb-dap.exe"
Current executable set to 'C:\Users\charleszablit\Developer\lldb\llvm-build\debug-oft\bin\lldb-dap.exe' (x86_64).
(lldb) r
Process 39644 launched: 'C:\Users\charleszablit\Developer\lldb\llvm-build\debug-oft\bin\lldb-dap.exe' (x86_64)
Process 39644 exited with status = 1 (0x00000001) 
```

---

### Comment 2 - DrSergei

I am not sure that exit code 1 is an expected behavior. On Linux lldb-dap waits for stdio interaction. I guess this problems are present in release branch, that is not good. Do you have plans to check and fix it?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-lldb-dap

Author: Sergei Druzhkov (DrSergei)

<details>
I tried to build latest lldb and lldb-dap on Windows. LLDB works fine, but lldb-dap crashes after startup without messages.

Build command:
```
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DLLDB_EXPORT_ALL_SYMBOLS=On -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DCMAKE_C_FLAGS=" /MT" -DCMAKE_CXX_FLAGS=" /MT" -DCMAKE_INSTALL_PREFIX=install -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DLLVM_ENABLE_PROJECTS="clang;lld;lldb" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCLANG_ENABLE_LIBXML2=Off -DLLDB_ENABLE_LIBXML2=Off -DLLDB_ENABLE_PYTHON=Off -DLLVM_ENABLE_ZLIB=Off -DLLVM_ENABLE_ZSTD=Off -DLLDB_ENABLE_LZMA=Off -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=Off ../llvm
ninja lldb lldb-dap lldb-server
```

Running lldb-dap under lldb (from the same build):
```
PS D:\projects\llvm-project\build-nopython-debug\bin&gt; ./lldb.exe ./lldb-dap.exe
(lldb) target create "./lldb-dap.exe"
Current executable set to 'D:\projects\llvm-project\build-nopython-debug\bin\lldb-dap.exe' (x86_64).
(lldb) run
Process 18764 launched: 'D:\projects\llvm-project\build-nopython-debug\bin\lldb-dap.exe' (x86_64)
Process 18764 stopped
* thread #<!-- -->1, stop reason = Exception 0xc0000409 encountered at address 0x7ff87112de18
    frame #<!-- -->0: 0x00007ff87112de18 liblldb.dll`_invoke_watson(expression=&lt;unavailable&gt;, function_name=&lt;unavailable&gt;, file_name=&lt;unavailable&gt;, line_number=&lt;unavailable&gt;, reserved=&lt;unavailable&gt;) at invalid_parameter.cpp:237

(lldb) error: no auto repeat
(lldb) bt
* thread #<!-- -->1, stop reason = Exception 0xc0000409 encountered at address 0x7ff87112de18
  * frame #<!-- -->0: 0x00007ff87112de18 liblldb.dll`_invoke_watson(expression=&lt;unavailable&gt;, function_name=&lt;unavailable&gt;, file_name=&lt;unavailable&gt;, line_number=&lt;unavailable&gt;, reserved=&lt;unavailable&gt;) at invalid_parameter.cpp:237
    frame #<!-- -->1: 0x00007ff87112dd67 liblldb.dll`_invalid_parameter_internal(expression=0x0000000000000000, function_name=0x0000000000000000, file_name=0x0000000000000000, line_number=1798886096, reserved=&lt;unavailable&gt;, ptd=&lt;unavailable&gt;) at invalid_parameter.cpp:113
    frame #<!-- -->2: 0x00007ff87112dc2a liblldb.dll`_invalid_parameter(expression=&lt;unavailable&gt;, function_name=&lt;unavailable&gt;, file_name=&lt;unavailable&gt;, line_number=&lt;unavailable&gt;, reserved=&lt;unavailable&gt;) at invalid_parameter.cpp:125
    frame #<!-- -->3: 0x00007ff87112ddb5 liblldb.dll`_invalid_parameter_noinfo at invalid_parameter.cpp:130
    frame #<!-- -->4: 0x00007ff871163930 liblldb.dll`_get_osfhandle(fh=&lt;unavailable&gt;) at osfinfo.cpp:264
    frame #<!-- -->5: 0x00007ff85fda12f1 liblldb.dll`lldb_private::NativeFile::NativeFile(this=0x0000021917c311d0, fd=3, options=eOpenOptionWriteOnly) at File.cpp:291
    frame #<!-- -->6: 0x00007ff66e4da81a lldb-dap.exe`void std::_Construct_in_place&lt;class lldb_private::NativeFile, int &amp;, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership&gt;(_Obj=0x0000021917c311d0, &lt;_Args_0&gt;=0x000000b06b38e004, &lt;_Args_1&gt;=0x000000b06b38f7fc, &lt;_Args_2&gt;=0x000000b06b38f7f8) at xutility:506
    frame #<!-- -->7: 0x00007ff66e4d74c0 lldb-dap.exe`std::_Ref_count_obj2&lt;class lldb_private::NativeFile&gt;::_Ref_count_obj2&lt;class lldb_private::NativeFile&gt;&lt;int &amp;, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership&gt;(this=0x0000021917c311c0, &lt;_Args_0&gt;=0x000000b06b38e004, &lt;_Args_1&gt;=0x000000b06b38f7fc) at memory:2092
    frame #<!-- -->8: 0x00007ff66e4e0c3b lldb-dap.exe`class std::shared_ptr&lt;class lldb_private::NativeFile&gt; std::make_shared&lt;class lldb_private::NativeFile, int &amp;, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership&gt;(&lt;_Args_0&gt;=0x000000b06b38e004, &lt;_Args_1&gt;=0x000000b06b38f7fc, &lt;_Args_2&gt;=0x000000b06b38f7f8) at memory:2903
    frame #<!-- -->9: 0x00007ff66e4d4854 lldb-dap.exe`main(argc=1, argv=0x0000021917b6ec60) at lldb-dap.cpp:735
    frame #<!-- -->10: 0x00007ff66e90d580 lldb-dap.exe`invoke_main at exe_common.inl:288 [inlined]
    frame #<!-- -->11: 0x00007ff66e90d55e lldb-dap.exe`__scrt_common_main_seh at exe_common.inl:288
    frame #<!-- -->12: 0x00007ff99a21e8d7 kernel32.dll`BaseThreadInitThunk + 23
    frame #<!-- -->13: 0x00007ff99af2c53c ntdll.dll`RtlUserThreadStart + 44
(lldb) exit
Quitting LLDB will kill one or more processes. Do you really want to proceed: [Y/n] y
```

CC: @<!-- -->charles-zablit 
</details>


---

### Comment 4 - charles-zablit

I have created the following to track the return 1 error:
- https://github.com/llvm/llvm-project/issues/178194

I'm not able to reproduce the crash from @DrSergei however. Just to confirm: was this on the `release/22.x` branch or `main`?

---

### Comment 5 - DrSergei

@charles-zablit I have the same problem on release branch ([commit](https://github.com/llvm/llvm-project/commit/a47b42eb9f9b302167b4fc413e6c92798d65dd0b)).

```
PS D:\llvm-project\build-nopython-debug-rc\bin> ./lldb-dap.exe --version
lldb-dap: LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc2
  Optimized build with assertions.
liblldb: lldb version 22.1.0-rc2 (git@github.com:llvm/llvm-project.git revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b)
  clang revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b
  llvm revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b
PS D:\llvm-project\build-nopython-debug-rc\bin> ./lldb.exe ./lldb-dap.exe          
(lldb) target create "./lldb-dap.exe"
Current executable set to 'D:\llvm-project\build-nopython-debug-rc\bin\lldb-dap.exe' (x86_64).
(lldb) run
Process 9748 launched: 'D:\llvm-project\build-nopython-debug-rc\bin\lldb-dap.exe' (x86_64)
Process 9748 stopped
* thread #1, stop reason = Exception 0xc0000409 encountered at address 0x7ff871143128
    frame #0: 0x00007ff871143128 liblldb.dll`_invoke_watson(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:237

(lldb) error: no auto repeat
(lldb) bt
* thread #1, stop reason = Exception 0xc0000409 encountered at address 0x7ff871143128
  * frame #0: 0x00007ff871143128 liblldb.dll`_invoke_watson(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:237
    frame #1: 0x00007ff871143077 liblldb.dll`_invalid_parameter_internal(expression=0x0000000000000000, function_name=0x0000000000000000, file_name=0x0000000000000000, line_number=1645792496, reserved=<unavailable>, ptd=<unavailable>) at invalid_parameter.cpp:113
    frame #2: 0x00007ff871142f3a liblldb.dll`_invalid_parameter(expression=<unavailable>, function_name=<unavailable>, file_name=<unavailable>, line_number=<unavailable>, reserved=<unavailable>) at invalid_parameter.cpp:125
    frame #3: 0x00007ff8711430c5 liblldb.dll`_invalid_parameter_noinfo at invalid_parameter.cpp:130
    frame #4: 0x00007ff871178c40 liblldb.dll`_get_osfhandle(fh=<unavailable>) at osfinfo.cpp:264
    frame #5: 0x00007ff85fe66521 liblldb.dll`lldb_private::NativeFile::NativeFile(this=0x0000029f19570ed0, fd=3, options=eOpenOptionWriteOnly) at File.cpp:291
    frame #6: 0x00007ff798a4a65a lldb-dap.exe`void std::_Construct_in_place<class lldb_private::NativeFile, int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(_Obj=0x0000029f19570ed0, <_Args_0>=0x000000a86218da24, <_Args_1>=0x000000a86218f21c, <_Args_2>=0x000000a86218f218) at xutility:506
    frame #7: 0x00007ff798a47340 lldb-dap.exe`std::_Ref_count_obj2<class lldb_private::NativeFile>::_Ref_count_obj2<class lldb_private::NativeFile><int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(this=0x0000029f19570ec0, <_Args_0>=0x000000a86218da24, <_Args_1>=0x000000a86218f21c) at memory:2092
    frame #8: 0x00007ff798a50a5b lldb-dap.exe`class std::shared_ptr<class lldb_private::NativeFile> std::make_shared<class lldb_private::NativeFile, int &, enum lldb_private::File::OpenOptions, enum lldb_private::NativeFile::TransferOwnership>(<_Args_0>=0x000000a86218da24, <_Args_1>=0x000000a86218f21c, <_Args_2>=0x000000a86218f218) at memory:2903
    frame #9: 0x00007ff798a446d4 lldb-dap.exe`main(argc=1, argv=0x0000029f194b0fd0) at lldb-dap.cpp:735
    frame #10: 0x00007ff798e7bf40 lldb-dap.exe`invoke_main at exe_common.inl:288 [inlined]
    frame #11: 0x00007ff798e7bf1e lldb-dap.exe`__scrt_common_main_seh at exe_common.inl:288
    frame #12: 0x00007ff99a21e8d7 kernel32.dll`BaseThreadInitThunk + 23
    frame #13: 0x00007ff99af2c53c ntdll.dll`RtlUserThreadStart + 44
(lldb) exit
Quitting LLDB will kill one or more processes. Do you really want to proceed: [Y/n] y
```

Some information about system:
- Windows 11
- Visual Studio 22
- No python build

CMake log:
```
-- The C compiler identification is MSVC 19.43.34810.0
-- The CXX compiler identification is MSVC 19.43.34810.0
-- The ASM compiler identification is MSVC
-- Found assembler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/bin/Hostx64/x64/cl.exe
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
...
```

---

### Comment 6 - charles-zablit

I'm only able to reproduce this error with `-DCMAKE_C_FLAGS=" /MT" -DCMAKE_CXX_FLAGS=" /MT"`. Using `/MD` this does not reproduce.

This makes liblldb use another runtime than lldb-dap. `lldb-dap` creates a CRT file descriptor which does not exists in `liblldb`.

Is there a reason you are using the `/MT` flag? I don't think that's what we test against.

---

### Comment 7 - DrSergei

I removed " /MT" and after that `lldb-dap` doesn't crash, but shows error messages.

New build command:
```
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DLLDB_EXPORT_ALL_SYMBOLS=On -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DCMAKE_INSTALL_PREFIX=install -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DLLVM_ENABLE_PROJECTS="clang;lld;lldb" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCLANG_ENABLE_LIBXML2=Off -DLLDB_ENABLE_LIBXML2=Off -DLLDB_ENABLE_PYTHON=Off -DLLVM_ENABLE_ZLIB=Off -DLLVM_ENABLE_ZSTD=Off -DLLDB_ENABLE_LZMA=Off -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=Off ../llvm
```

Output:
```
Content-Length: 134

{"body":{"category":"console","output":"Failure value returned from cantFail wrapped call\n"},"event":"output","seq":1,"type":"event"}Content-Length: 106

{"body":{"category":"console","output":"Unsupported file type 2"},"event":"output","seq":2,"type":"event"}Content-Length: 85

{"body":{"category":"console","output":"\n"},"event":"output","seq":3,"type":"event"}Content-Length: 167

{"body":{"category":"console","output":"UNREACHABLE executed at D:\\llvm-project\\llvm\\include\\llvm/Support/Error.h:779!\n"},"event":"output","seq":4,"type":"event"}Content-Length: 192

{"body":{"category":"console","output":"PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.\n"},"event":"output","seq":5,"type":"event"}Content-Length: 111

{"body":{"category":"console","output":"Exception Code: 0x80000003\n"},"event":"output","seq":6,"type":"event"}Content-Length: 101

{"body":{"category":"console","output":"0x00007FF65EB6EDEC"},"event":"output","seq":7,"type":"event"}Content-Length: 181

{"body":{"category":"console","output":", D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x11EDEC byte(s)"},"event":"output","seq":8,"type":"event"}Content-Length: 112

{"body":{"category":"console","output":", HandleAbort() + 0xC byte(s)"},"event":"output","seq":9,"type":"event"}Content-Length: 170

{"body":{"category":"console","output":", D:\\llvm-project\\llvm\\lib\\Support\\Windows\\Signals.inc, line 371 + 0x0 byte(s)\n"},"event":"output","seq":10,"type":"event"}Content-Length: 205

{"body":{"category":"console","output":"0x00007FF9435490ED, C:\\WINDOWS\\SYSTEM32\\ucrtbased.dll(0x00007FF9434A0000) + 0xA90ED byte(s), raise() + 0x46D byte(s)\n"},"event":"output","seq":11,"type":"event"}Content-Length: 204

{"body":{"category":"console","output":"0x00007FF94354AE49, C:\\WINDOWS\\SYSTEM32\\ucrtbased.dll(0x00007FF9434A0000) + 0xAAE49 byte(s), abort() + 0x39 byte(s)\n"},"event":"output","seq":12,"type":"event"}Content-Length: 200

{"body":{"category":"console","output":"0x00007FF65EB960FD, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x1460FD byte(s)"},"event":"output","seq":13,"type":"event"}Content-Length: 217

{"body":{"category":"console","output":", llvm::llvm_unreachable_internal() + 0xED byte(s), D:\\llvm-project\\llvm\\lib\\Support\\ErrorHandling.cpp, line 244 + 0x6 byte(s)\n"},"event":"output","seq":14,"type":"event"}Content-Length: 233

{"body":{"category":"console","output":"0x00007FF65EABDBDF, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x6DBDF byte(s), llvm::cantFail() + 0x10F byte(s)"},"event":"output","seq":15,"type":"event"}Content-Length: 168

{"body":{"category":"console","output":", D:\\llvm-project\\llvm\\include\\llvm\\Support\\Error.h, line 779 + 0x1A byte(s)\n"},"event":"output","seq":16,"type":"event"}Content-Length: 275

{"body":{"category":"console","output":"0x00007FF65EAD996E, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x8996E byte(s), llvm::handleAllErrors<`llvm::toString'::`2'::<lambda_1> >() + 0x4E byte(s)"},"event":"output","seq":17,"type":"event"}Content-Length: 168

{"body":{"category":"console","output":", D:\\llvm-project\\llvm\\include\\llvm\\Support\\Error.h, line 991 + 0x3F byte(s)\n"},"event":"output","seq":18,"type":"event"}Content-Length: 199

{"body":{"category":"console","output":"0x00007FF65EAD7DE5, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x87DE5 byte(s)"},"event":"output","seq":19,"type":"event"}Content-Length: 192

{"body":{"category":"console","output":", llvm::toString() + 0xA5 byte(s), D:\\llvm-project\\llvm\\lib\\Support\\Error.cpp, line 79 + 0x5C byte(s)\n"},"event":"output","seq":20,"type":"event"}Content-Length: 200

{"body":{"category":"console","output":"0x00007FF65ECF492B, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2A492B byte(s)"},"event":"output","seq":21,"type":"event"}Content-Length: 126

{"body":{"category":"console","output":", lldb_dap::FormatError<>() + 0x5B byte(s)"},"event":"output","seq":22,"type":"event"}Content-Length: 159

{"body":{"category":"console","output":", D:\\llvm-project\\lldb\\tools\\lldb-dap\\DAPLog.h, line 74 + 0x32 byte(s)"},"event":"output","seq":23,"type":"event"}Content-Length: 86

{"body":{"category":"console","output":"\n"},"event":"output","seq":24,"type":"event"}Content-Length: 329

{"body":{"category":"console","output":"0x00007FF65ECE1FFD, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x291FFD byte(s), lldb_dap::DAP::TransportHandler() + 0x18D byte(s), D:\\llvm-project\\lldb\\tools\\lldb-dap\\DAP.cpp, line 1046 + 0xC1 byte(s)\n"},"event":"output","seq":25,"type":"event"}Content-Length: 428

{"body":{"category":"console","output":"0x00007FF65ED06D4E, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2B6D4E byte(s), std::invoke<void (__cdecl lldb_dap::DAP::*&)(void),lldb_dap::DAP * &>() + 0x1E byte(s), C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.43.34808\\include\\type_traits, line 1712 + 0xF byte(s)\n"},"event":"output","seq":26,"type":"event"}Content-Length: 200

{"body":{"category":"console","output":"0x00007FF65ECF809E, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2A809E byte(s)"},"event":"output","seq":27,"type":"event"}Content-Length: 201

{"body":{"category":"console","output":", std::_Invoker_ret<std::_Unforced>::_Call<void (__cdecl lldb_dap::DAP::*&)(void),lldb_dap::DAP * &>() + 0x1E byte(s)"},"event":"output","seq":28,"type":"event"}Content-Length: 223

{"body":{"category":"console","output":", C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.43.34808\\include\\functional, line 2111 + 0xF byte(s)\n"},"event":"output","seq":29,"type":"event"}Content-Length: 200

{"body":{"category":"console","output":"0x00007FF65ECF80EB, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2A80EB byte(s)"},"event":"output","seq":30,"type":"event"}Content-Length: 358

{"body":{"category":"console","output":", std::_Call_binder<std::_Unforced,0,void (__cdecl lldb_dap::DAP::*)(void),std::tuple<lldb_dap::DAP *>,std::tuple<> >() + 0x3B byte(s), C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.43.34808\\include\\functional, line 2121 + 0x24 byte(s)\n"},"event":"output","seq":31,"type":"event"}Content-Length: 457

{"body":{"category":"console","output":"0x00007FF65ECF411B, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2A411B byte(s), std::_Binder<std::_Unforced,void (__cdecl lldb_dap::DAP::*)(void),lldb_dap::DAP *>::operator()<,0>() + 0x9B byte(s), C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.43.34808\\include\\functional, line 2193 + 0x91 byte(s)\n"},"event":"output","seq":32,"type":"event"}Content-Length: 455

{"body":{"category":"console","output":"0x00007FF65ED071E4, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2B71E4 byte(s), std::invoke<std::_Binder<std::_Unforced,void (__cdecl lldb_dap::DAP::*)(void),lldb_dap::DAP *> >() + 0x14 byte(s), C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.43.34808\\include\\type_traits, line 1696 + 0xA byte(s)\n"},"event":"output","seq":33,"type":"event"}Content-Length: 200

{"body":{"category":"console","output":"0x00007FF65ECFF260, D:\\llvm-project\\build-nopython-debug\\bin\\lldb-dap.exe(0x00007FF65EA50000) + 0x2AF260 byte(s)"},"event":"output","seq":34,"type":"event"}
```

Seems the problem is here (DAP.cpp, line 1046)

```cpp
  auto handle = transport.RegisterMessageHandler(m_loop, *this);
  if (!handle) {
    DAP_LOG_ERROR(log, handle.takeError(),  //    <--------
                  "registering message handler failed: {0}");
    std::lock_guard<std::mutex> guard(m_queue_mutex);
    m_error_occurred = true;
    return;
  }

```


Update: Release build has the same problem

Build command:
```
cmake  -GNinja -DCMAKE_BUILD_TYPE=Debug -DLLDB_EXPORT_ALL_SYMBOLS=On -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DCMAKE_C_FLAGS=" /MD" -DCMAKE_CXX_FLAGS=" /MD" -DCMAKE_INSTALL_PREFIX=install -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DLLVM_ENABLE_PROJECTS="clang;lld;lldb" -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x86_64 -DCLANG_ENABLE_LIBXML2=Off -DLLDB_ENABLE_LIBXML2=Off -DLLDB_ENABLE_PYTHON=Off -DLLVM_ENABLE_ZLIB=Off -DLLVM_ENABLE_ZSTD=Off -DLLDB_ENABLE_LZMA=Off -DLLVM_WINDOWS_PREFER_FORWARD_SLASH=Off ../llvm
```

Output:
```
PS D:\llvm-project\build-nopython-release-md\bin> ./lldb-dap.exe --version
lldb-dap: LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc2
  Optimized build.
liblldb: lldb version 22.1.0-rc2 (git@github.com:llvm/llvm-project.git revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b)
  clang revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b
  llvm revision a47b42eb9f9b302167b4fc413e6c92798d65dd0b
PS D:\llvm-project\build-nopython-release-md\bin> ./lldb-dap.exe
DAP session error: DAP Loop terminated due to an internal error, see DAP Logs for more information.
```

---

### Comment 8 - DrSergei

I tested 21.1.8 and everything works good, so it is a regression that affects new release.

Output:
```
PS D:\llvm-project\build-nopython-release-md-old\bin> ./lldb-dap.exe --version
lldb-dap: LLVM (http://llvm.org/):
  LLVM version 21.1.8
  Optimized build.
liblldb: lldb version 21.1.8 (git@github.com:llvm/llvm-project.git revision 2078da43e25a4623cab2d0d60decddf709aaea28)
  clang revision 2078da43e25a4623cab2d0d60decddf709aaea28
  llvm revision 2078da43e25a4623cab2d0d60decddf709aaea28
PS D:\llvm-project\build-nopython-release-md-old\bin> ./lldb-dap.exe
test
DAP session error: expected 'Content-Length: ' and got 'test
'
PS D:\llvm-project\build-nopython-release-md-old\bin>
```

---

### Comment 9 - DrSergei

@charles-zablit I found the problem [line](https://github.com/llvm/llvm-project/blob/main/lldb/source/Host/windows/MainLoopWindows.cpp#L250). Before [PR](https://github.com/llvm/llvm-project/pull/145621) `lldb-dap` can work on Windows in shell, but after it only pipe or socket modes are allowed. Because old version (21.1.8) works fine in console, I think we should restore this functionality. CC: @ashgti 

Using lldb-dap through extension works fine with release branch, so it is not blocking issue. Thank you very much! 

---

### Comment 10 - charles-zablit

Thanks for looking into it @DrSergei !

There is already an issue tracking this: https://github.com/llvm/llvm-project/issues/178194.

I will put up a PR for the fix today.

---

### Comment 11 - charles-zablit

Opened a fix for #178194 here:
- https://github.com/llvm/llvm-project/pull/178409

Let's keep this issue open to find a way to avoid CRT filedescriptor errors when missing runtimes.

---

