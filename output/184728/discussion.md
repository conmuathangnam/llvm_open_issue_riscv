# [LLDB] crash when debuginfod download fails: `CachedFileStream was not committed`

**Author:** patryk4815
**URL:** https://github.com/llvm/llvm-project/issues/184728
**Status:** Open
**Labels:** lldb, crash

## Body

### Version
- lldb: 22.1.0

### Summary
LLDB crashes when using debuginfod if the download of an artifact fails or is interrupted.  
The crash happens in the destructor of `llvm::CachedFileStream`, which triggers a fatal error:
```
LLVM ERROR: CachedFileStream was not committed.
Fatal Python error: Aborted
```

This appears to occur when the debuginfod connection drops or becomes very slow while downloading a file.

### Reproduction

Reproduction is somewhat unreliable, but it consistently happens when the debuginfod server is very slow or the connection drops mid-download.

Example:
```bash
export DEBUGINFOD_URLS=http://your-laggy-server
lldb ./any-linux-binary-with-buildid
```
(lldb must be compiled with libcurl.so.4, otherwise debuginfod don't work)

Conditions that seem to trigger the issue:
- debuginfod server responds slowly?
- connection drops after only a few bytes of the file are downloaded
- artifact download does not complete successfully

### Crash Location
The crash occurs when the destructor of llvm::CachedFileStream runs.
Screenshot of the location in the debugger:
<img width="649" height="467" alt="Image" src="https://github.com/user-attachments/assets/33fd420d-90c6-4a30-a279-d6aae563dd84" />

```
pwndbg> p Client.responseCode()
$10 = 200
```
So `Client` logic must better handle connection drops maybe

### Backtrack of crash:
```
pwndbg> bt
#0  __pthread_kill_implementation (threadid=281474630541664, signo=signo@entry=6, no_tid=no_tid@entry=0) at pthread_kill.c:44
#1  0x0000fffff7d41394 in __pthread_kill_internal (threadid=<optimized out>, signo=6) at pthread_kill.c:89
#2  0x0000fffff7ceba3c in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#3  0x0000fffff7cd5a08 in __GI_abort () at abort.c:77
#4  0x0000fffff2880860 in llvm::report_fatal_error (Reason=..., GenCrashDiag=<optimized out>) at /build/source/llvm/lib/Support/ErrorHandling.cpp:135
#5  0x0000fffff28806a4 in llvm::report_fatal_error (Reason=<optimized out>, GenCrashDiag=<optimized out>) at /build/source/llvm/lib/Support/ErrorHandling.cpp:96
#6  0x0000fffff28675a8 in llvm::CachedFileStream::~CachedFileStream (this=0xffffe01a0370) at /build/source/llvm/include/llvm/Support/Caching.h:51
#7  llvm::localCache(llvm::Twine const&, llvm::Twine const&, llvm::Twine const&, std::__1::function<void (unsigned int, llvm::Twine const&, std::__1::unique_ptr<llvm::MemoryBuffer, std::__1::default_delete<llvm::MemoryBuffer> >)>)::$_0::operator()(unsigned int, llvm::StringRef, llvm::Twine const&) const::CacheStream::~CacheStream() (
    this=0xffffe01a0370) at /build/source/llvm/lib/Support/Caching.cpp:78
#8  llvm::localCache(llvm::Twine const&, llvm::Twine const&, llvm::Twine const&, std::__1::function<void (unsigned int, llvm::Twine const&, std::__1::unique_ptr<llvm::MemoryBuffer, std::__1::default_delete<llvm::MemoryBuffer> >)>)::$_0::operator()(unsigned int, llvm::StringRef, llvm::Twine const&) const::CacheStream::~CacheStream() (
    this=0xffffe01a0370) at /build/source/llvm/lib/Support/Caching.cpp:78
#9  0x0000fffff4cbe434 in std::__1::default_delete<llvm::CachedFileStream>::operator()[abi:sn200100](llvm::CachedFileStream*) const (this=0xffffeb5dca68, __ptr=0x0)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:78
#10 std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream> >::reset[abi:sn200100](llvm::CachedFileStream*) (this=0xffffeb5dca68,
    __p=0x0) at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:300
#11 std::__1::unique_ptr<llvm::CachedFileStream, std::__1::default_delete<llvm::CachedFileStream> >::~unique_ptr[abi:sn200100]() (this=0xffffeb5dca68)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:269
#12 llvm::(anonymous namespace)::StreamedHTTPResponseHandler::~StreamedHTTPResponseHandler (this=0xffffeb5dca20) at /build/source/llvm/lib/Debuginfod/Debuginfod.cpp:199
#13 llvm::getCachedOrDownloadArtifact (UniqueKey=..., UrlPath=..., CacheDirectoryPath=..., DebuginfodUrls=..., Timeout=...)
    at /build/source/llvm/lib/Debuginfod/Debuginfod.cpp:320
#14 0x0000fffff27fd554 in GetFileForModule (module_spec=..., UrlBuilder=...)
    at /build/source/lldb/source/Plugins/SymbolLocator/Debuginfod/SymbolLocatorDebuginfod.cpp:190
#15 0x0000fffff27fd7c8 in lldb_private::SymbolLocatorDebuginfod::LocateExecutableSymbolFile (module_spec=..., default_search_paths=...)
    at /build/source/lldb/source/Plugins/SymbolLocator/Debuginfod/SymbolLocatorDebuginfod.cpp:210
#16 0x0000fffff221a660 in lldb_private::PluginManager::LocateExecutableSymbolFile (module_spec=..., default_search_paths=..., map=...)
    at /build/source/lldb/source/Core/PluginManager.cpp:1553
#17 0x0000fffff27ffb64 in SymbolVendorELF::CreateInstance (module_sp=..., feedback_strm=<optimized out>)
    at /build/source/lldb/source/Plugins/SymbolVendor/ELF/SymbolVendorELF.cpp:106
#18 0x0000fffff231a55c in lldb_private::SymbolVendor::FindPlugin (module_sp=..., feedback_strm=0x0) at /build/source/lldb/source/Symbol/SymbolVendor.cpp:35
#19 0x0000fffff21fd480 in lldb_private::Module::GetSymbolFile (this=0x15434b8, can_create=<optimized out>, feedback_strm=0x0)
    at /build/source/lldb/source/Core/Module.cpp:1024
#20 0x0000fffff21ff820 in lldb_private::Module::PreloadSymbols (this=0x15434b8) at /build/source/lldb/source/Core/Module.cpp:1347
#21 0x0000fffff220a2b4 in llvm::unique_function<void()>::operator() (this=0x0) at /build/source/llvm/include/llvm/ADT/FunctionExtras.h:364
#22 std::__1::__invoke[abi:sn200100]<llvm::unique_function<void ()>>(llvm::unique_function<void ()>&&) (__f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__type_traits/invoke.h:179
#23 __execute<> (this=0x0) at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:1858
#24 std::__1::__async_func<llvm::unique_function<void ()>>::operator()[abi:sn200100]() (this=0x0)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:1852
#25 std::__1::__deferred_assoc_state<void, std::__1::__async_func<llvm::unique_function<void()> > >::__execute (this=0x1520390)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:827
#26 0x0000fffff2897e40 in llvm::unique_function<void()>::operator() (this=0xffffeb5dd770) at /build/source/llvm/include/llvm/ADT/FunctionExtras.h:364
#27 llvm::StdThreadPool::processTasks (this=this@entry=0x1134ce0, WaitingForGroup=0x0, WaitingForGroup@entry=0xffffeb5dd830)
    at /build/source/llvm/lib/Support/ThreadPool.cpp:119
#28 0x0000fffff28998b8 in llvm::StdThreadPool::grow(int)::$_0::operator()() const (this=0x155f640) at /build/source/llvm/lib/Support/ThreadPool.cpp:62
#29 llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}::operator()<llvm::StdThreadPool::grow(int)::$_0&>(llvm::StdThreadPool::grow(int)::$_0&) const (F=..., this=<optimized out>) at /build/source/llvm/include/llvm/Support/thread.h:46
#30 std::__1::__invoke[abi:sn200100]<llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}, llvm::StdThreadPool::grow(int)::$_0&>(llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}&&, llvm::StdThreadPool::grow(int)::$_0&) (__args=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__type_traits/invoke.h:179
#31 std::__1::__apply_tuple_impl[abi:sn200100]<llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}, std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0>&, 0ul>(llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}&&, std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0>&, std::__1::__tuple_indices<0ul>) (__t=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/tuple:1375
#32 std::__1::apply[abi:sn200100]<llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}, std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0>&>(llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> >(void*)::{lambda(auto:1&&, (auto:2&&)...)#1}&&, std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0>&) (__t=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/tuple:1379
#33 llvm::thread::GenericThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> > (Ptr=0x155f640) at /build/source/llvm/include/llvm/Support/thread.h:44
#34 llvm::thread::ThreadProxy<std::__1::tuple<llvm::StdThreadPool::grow(int)::$_0> > (Ptr=0x155f640) at /build/source/llvm/include/llvm/Support/thread.h:58
#35 0x0000fffff7d3f504 in start_thread (arg=0xffffeb5de160) at pthread_create.c:448
#36 0x0000fffff7daa99c in thread_start () at ../sysdeps/unix/sysv/linux/aarch64/clone3.S:79
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: None (patryk4815)

<details>
### Version
- lldb: 22.1.0

### Summary
LLDB crashes when using debuginfod if the download of an artifact fails or is interrupted.  
The crash happens in the destructor of `llvm::CachedFileStream`, which triggers a fatal error:
```
LLVM ERROR: CachedFileStream was not committed.
Fatal Python error: Aborted
```

This appears to occur when the debuginfod connection drops or becomes very slow while downloading a file.

### Reproduction

Reproduction is somewhat unreliable, but it consistently happens when the debuginfod server is very slow or the connection drops mid-download.

Example:
```bash
export DEBUGINFOD_URLS=http://your-laggy-server
lldb ./any-linux-binary-with-buildid
```
(lldb must be compiled with libcurl.so.4, otherwise debuginfod don't work)

Conditions that seem to trigger the issue:
- debuginfod server responds slowly?
- connection drops after only a few bytes of the file are downloaded
- artifact download does not complete successfully

### Crash Location
The crash occurs when the destructor of llvm::CachedFileStream runs.
Screenshot of the location in the debugger:
&lt;img width="649" height="467" alt="Image" src="https://github.com/user-attachments/assets/33fd420d-90c6-4a30-a279-d6aae563dd84" /&gt;

### Backtrack of crash:
```
pwndbg&gt; bt
#<!-- -->0  __pthread_kill_implementation (threadid=281474630541664, signo=signo@<!-- -->entry=6, no_tid=no_tid@<!-- -->entry=0) at pthread_kill.c:44
#<!-- -->1  0x0000fffff7d41394 in __pthread_kill_internal (threadid=&lt;optimized out&gt;, signo=6) at pthread_kill.c:89
#<!-- -->2  0x0000fffff7ceba3c in __GI_raise (sig=sig@<!-- -->entry=6) at ../sysdeps/posix/raise.c:26
#<!-- -->3  0x0000fffff7cd5a08 in __GI_abort () at abort.c:77
#<!-- -->4  0x0000fffff2880860 in llvm::report_fatal_error (Reason=..., GenCrashDiag=&lt;optimized out&gt;) at /build/source/llvm/lib/Support/ErrorHandling.cpp:135
#<!-- -->5  0x0000fffff28806a4 in llvm::report_fatal_error (Reason=&lt;optimized out&gt;, GenCrashDiag=&lt;optimized out&gt;) at /build/source/llvm/lib/Support/ErrorHandling.cpp:96
#<!-- -->6  0x0000fffff28675a8 in llvm::CachedFileStream::~CachedFileStream (this=0xffffe01a0370) at /build/source/llvm/include/llvm/Support/Caching.h:51
#<!-- -->7  llvm::localCache(llvm::Twine const&amp;, llvm::Twine const&amp;, llvm::Twine const&amp;, std::__1::function&lt;void (unsigned int, llvm::Twine const&amp;, std::__1::unique_ptr&lt;llvm::MemoryBuffer, std::__1::default_delete&lt;llvm::MemoryBuffer&gt; &gt;)&gt;)::$_0::operator()(unsigned int, llvm::StringRef, llvm::Twine const&amp;) const::CacheStream::~CacheStream() (
    this=0xffffe01a0370) at /build/source/llvm/lib/Support/Caching.cpp:78
#<!-- -->8  llvm::localCache(llvm::Twine const&amp;, llvm::Twine const&amp;, llvm::Twine const&amp;, std::__1::function&lt;void (unsigned int, llvm::Twine const&amp;, std::__1::unique_ptr&lt;llvm::MemoryBuffer, std::__1::default_delete&lt;llvm::MemoryBuffer&gt; &gt;)&gt;)::$_0::operator()(unsigned int, llvm::StringRef, llvm::Twine const&amp;) const::CacheStream::~CacheStream() (
    this=0xffffe01a0370) at /build/source/llvm/lib/Support/Caching.cpp:78
#<!-- -->9  0x0000fffff4cbe434 in std::__1::default_delete&lt;llvm::CachedFileStream&gt;::operator()[abi:sn200100](llvm::CachedFileStream*) const (this=0xffffeb5dca68, __ptr=0x0)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:78
#<!-- -->10 std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt; &gt;::reset[abi:sn200100](llvm::CachedFileStream*) (this=0xffffeb5dca68,
    __p=0x0) at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:300
#<!-- -->11 std::__1::unique_ptr&lt;llvm::CachedFileStream, std::__1::default_delete&lt;llvm::CachedFileStream&gt; &gt;::~unique_ptr[abi:sn200100]() (this=0xffffeb5dca68)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__memory/unique_ptr.h:269
#<!-- -->12 llvm::(anonymous namespace)::StreamedHTTPResponseHandler::~StreamedHTTPResponseHandler (this=0xffffeb5dca20) at /build/source/llvm/lib/Debuginfod/Debuginfod.cpp:199
#<!-- -->13 llvm::getCachedOrDownloadArtifact (UniqueKey=..., UrlPath=..., CacheDirectoryPath=..., DebuginfodUrls=..., Timeout=...)
    at /build/source/llvm/lib/Debuginfod/Debuginfod.cpp:320
#<!-- -->14 0x0000fffff27fd554 in GetFileForModule (module_spec=..., UrlBuilder=...)
    at /build/source/lldb/source/Plugins/SymbolLocator/Debuginfod/SymbolLocatorDebuginfod.cpp:190
#<!-- -->15 0x0000fffff27fd7c8 in lldb_private::SymbolLocatorDebuginfod::LocateExecutableSymbolFile (module_spec=..., default_search_paths=...)
    at /build/source/lldb/source/Plugins/SymbolLocator/Debuginfod/SymbolLocatorDebuginfod.cpp:210
#<!-- -->16 0x0000fffff221a660 in lldb_private::PluginManager::LocateExecutableSymbolFile (module_spec=..., default_search_paths=..., map=...)
    at /build/source/lldb/source/Core/PluginManager.cpp:1553
#<!-- -->17 0x0000fffff27ffb64 in SymbolVendorELF::CreateInstance (module_sp=..., feedback_strm=&lt;optimized out&gt;)
    at /build/source/lldb/source/Plugins/SymbolVendor/ELF/SymbolVendorELF.cpp:106
#<!-- -->18 0x0000fffff231a55c in lldb_private::SymbolVendor::FindPlugin (module_sp=..., feedback_strm=0x0) at /build/source/lldb/source/Symbol/SymbolVendor.cpp:35
#<!-- -->19 0x0000fffff21fd480 in lldb_private::Module::GetSymbolFile (this=0x15434b8, can_create=&lt;optimized out&gt;, feedback_strm=0x0)
    at /build/source/lldb/source/Core/Module.cpp:1024
#<!-- -->20 0x0000fffff21ff820 in lldb_private::Module::PreloadSymbols (this=0x15434b8) at /build/source/lldb/source/Core/Module.cpp:1347
#<!-- -->21 0x0000fffff220a2b4 in llvm::unique_function&lt;void()&gt;::operator() (this=0x0) at /build/source/llvm/include/llvm/ADT/FunctionExtras.h:364
#<!-- -->22 std::__1::__invoke[abi:sn200100]&lt;llvm::unique_function&lt;void ()&gt;&gt;(llvm::unique_function&lt;void ()&gt;&amp;&amp;) (__f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__type_traits/invoke.h:179
#<!-- -->23 __execute&lt;&gt; (this=0x0) at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:1858
#<!-- -->24 std::__1::__async_func&lt;llvm::unique_function&lt;void ()&gt;&gt;::operator()[abi:sn200100]() (this=0x0)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:1852
#<!-- -->25 std::__1::__deferred_assoc_state&lt;void, std::__1::__async_func&lt;llvm::unique_function&lt;void()&gt; &gt; &gt;::__execute (this=0x1520390)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/future:827
#<!-- -->26 0x0000fffff2897e40 in llvm::unique_function&lt;void()&gt;::operator() (this=0xffffeb5dd770) at /build/source/llvm/include/llvm/ADT/FunctionExtras.h:364
#<!-- -->27 llvm::StdThreadPool::processTasks (this=this@<!-- -->entry=0x1134ce0, WaitingForGroup=0x0, WaitingForGroup@<!-- -->entry=0xffffeb5dd830)
    at /build/source/llvm/lib/Support/ThreadPool.cpp:119
#<!-- -->28 0x0000fffff28998b8 in llvm::StdThreadPool::grow(int)::$_0::operator()() const (this=0x155f640) at /build/source/llvm/lib/Support/ThreadPool.cpp:62
#<!-- -->29 llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}::operator()&lt;llvm::StdThreadPool::grow(int)::$_0&amp;&gt;(llvm::StdThreadPool::grow(int)::$_0&amp;) const (F=..., this=&lt;optimized out&gt;) at /build/source/llvm/include/llvm/Support/thread.h:46
#<!-- -->30 std::__1::__invoke[abi:sn200100]&lt;llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}, llvm::StdThreadPool::grow(int)::$_0&amp;&gt;(llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}&amp;&amp;, llvm::StdThreadPool::grow(int)::$_0&amp;) (__args=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/__type_traits/invoke.h:179
#<!-- -->31 std::__1::__apply_tuple_impl[abi:sn200100]&lt;llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}, std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&amp;, 0ul&gt;(llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}&amp;&amp;, std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&amp;, std::__1::__tuple_indices&lt;0ul&gt;) (__t=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/tuple:1375
#<!-- -->32 std::__1::apply[abi:sn200100]&lt;llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}, std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&amp;&gt;(llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt;(void*)::{lambda(auto:1&amp;&amp;, (auto:2&amp;&amp;)...)#<!-- -->1}&amp;&amp;, std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt;&amp;) (__t=..., __f=...)
    at /nix/store/8v4sfpbc3maj5vsii6dc4kxh6ql6xyjn-zig-0.15.2/lib/zig/libcxx/include/tuple:1379
#<!-- -->33 llvm::thread::GenericThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt; (Ptr=0x155f640) at /build/source/llvm/include/llvm/Support/thread.h:44
#<!-- -->34 llvm::thread::ThreadProxy&lt;std::__1::tuple&lt;llvm::StdThreadPool::grow(int)::$_0&gt; &gt; (Ptr=0x155f640) at /build/source/llvm/include/llvm/Support/thread.h:58
#<!-- -->35 0x0000fffff7d3f504 in start_thread (arg=0xffffeb5de160) at pthread_create.c:448
#<!-- -->36 0x0000fffff7daa99c in thread_start () at ../sysdeps/unix/sysv/linux/aarch64/clone3.S:79
```
</details>


---

