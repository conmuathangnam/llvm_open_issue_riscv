# LLDB crashes when testing "debug" build with ctest (libjxl)

**Author:** eustas
**URL:** https://github.com/llvm/llvm-project/issues/167853
**Status:** Open
**Labels:** lldb, crash

## Body

I was hoping to find out why tests fail under ctest, but work well when executed manually. But got lldb crash.
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #0 0x0000558824f14e4b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/lldb+0x38e4b)
 #1 0x0000558824f126da SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f54e9049df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #3 0x00007f54e9118779 syscall ./misc/../sysdeps/unix/sysv/linux/x86_64/syscall.S:38:0
 #4 0x00007f54ea757a62 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #5 0x00007f54e9049df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #6 0x00007f54ea1aa414 std::_Sp_counted_ptr<lldb_private::StopInfoUnixSignal*, (__gnu_cxx::_Lock_policy)2>::_M_dispose() (/usr/local/bin/../lib/liblldb.so.21.1+0xbaa414)
 #7 0x00007f54e9e3f397 std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release() (/usr/local/bin/../lib/liblldb.so.21.1+0x83f397)
 #8 0x00007f54ea1e0c7c lldb_private::Thread::GetStopDescriptionRaw[abi:cxx11]() (/usr/local/bin/../lib/liblldb.so.21.1+0xbe0c7c)
 #9 0x00007f54ea1e0dab lldb_private::Thread::GetStopDescription[abi:cxx11]() (/usr/local/bin/../lib/liblldb.so.21.1+0xbe0dab)
#10 0x00007f54e9fc7f3c lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&, lldb_private::Stream&, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c7f3c)
#11 0x00007f54e9fc73e0 lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&, lldb_private::Stream&, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c73e0)
#12 0x00007f54e9fc84f8 lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&, lldb_private::Stream&, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c84f8)
#13 0x00007f54ea003ce8 lldb_private::Statusline::Redraw(bool) (.part.0) Statusline.cpp:0:0
#14 0x00007f54e9fac4c1 lldb_private::Debugger::DefaultEventHandler() (/usr/local/bin/../lib/liblldb.so.21.1+0x9ac4c1)
#15 0x00007f54e9fac8ec std::_Function_handler<void* (), lldb_private::Debugger::StartEventHandlerThread()::'lambda'()>::_M_invoke(std::_Any_data const&) Debugger.cpp:0:0
#16 0x00007f54ea080e5c lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) (/usr/local/bin/../lib/liblldb.so.21.1+0xa80e5c)
#17 0x00007f54e909cb7b start_thread ./nptl/pthread_create.c:448:8
#18 0x00007f54e911a7b8 __GI___clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Segmentation fault (core dumped)
```

Unfortunately, there is nothing to attach:
```
$ ls -la /tmp/diagnostics-620011
total 0
drwxr-x---  2 eustas primarygroup   60 Nov 13 09:37 .
drwxrwxrwt 44 root   root         1660 Nov 13 09:40 ..
-rw-r-----  1 eustas primarygroup    0 Nov 13 09:37 diagnostics.log
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Eugene Kliuchnikov (eustas)

<details>
I was hoping to find out why tests fail under ctest, but work well when executed manually. But got lldb crash.
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #<!-- -->0 0x0000558824f14e4b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/lldb+0x38e4b)
 #<!-- -->1 0x0000558824f126da SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f54e9049df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->3 0x00007f54e9118779 syscall ./misc/../sysdeps/unix/sysv/linux/x86_64/syscall.S:38:0
 #<!-- -->4 0x00007f54ea757a62 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->5 0x00007f54e9049df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->6 0x00007f54ea1aa414 std::_Sp_counted_ptr&lt;lldb_private::StopInfoUnixSignal*, (__gnu_cxx::_Lock_policy)2&gt;::_M_dispose() (/usr/local/bin/../lib/liblldb.so.21.1+0xbaa414)
 #<!-- -->7 0x00007f54e9e3f397 std::_Sp_counted_base&lt;(__gnu_cxx::_Lock_policy)2&gt;::_M_release() (/usr/local/bin/../lib/liblldb.so.21.1+0x83f397)
 #<!-- -->8 0x00007f54ea1e0c7c lldb_private::Thread::GetStopDescriptionRaw[abi:cxx11]() (/usr/local/bin/../lib/liblldb.so.21.1+0xbe0c7c)
 #<!-- -->9 0x00007f54ea1e0dab lldb_private::Thread::GetStopDescription[abi:cxx11]() (/usr/local/bin/../lib/liblldb.so.21.1+0xbe0dab)
#<!-- -->10 0x00007f54e9fc7f3c lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&amp;, lldb_private::Stream&amp;, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c7f3c)
#<!-- -->11 0x00007f54e9fc73e0 lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&amp;, lldb_private::Stream&amp;, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c73e0)
#<!-- -->12 0x00007f54e9fc84f8 lldb_private::FormatEntity::Format(lldb_private::FormatEntity::Entry const&amp;, lldb_private::Stream&amp;, lldb_private::SymbolContext const*, lldb_private::ExecutionContext const*, lldb_private::Address const*, lldb_private::ValueObject*, bool, bool) (/usr/local/bin/../lib/liblldb.so.21.1+0x9c84f8)
#<!-- -->13 0x00007f54ea003ce8 lldb_private::Statusline::Redraw(bool) (.part.0) Statusline.cpp:0:0
#<!-- -->14 0x00007f54e9fac4c1 lldb_private::Debugger::DefaultEventHandler() (/usr/local/bin/../lib/liblldb.so.21.1+0x9ac4c1)
#<!-- -->15 0x00007f54e9fac8ec std::_Function_handler&lt;void* (), lldb_private::Debugger::StartEventHandlerThread()::'lambda'()&gt;::_M_invoke(std::_Any_data const&amp;) Debugger.cpp:0:0
#<!-- -->16 0x00007f54ea080e5c lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) (/usr/local/bin/../lib/liblldb.so.21.1+0xa80e5c)
#<!-- -->17 0x00007f54e909cb7b start_thread ./nptl/pthread_create.c:448:8
#<!-- -->18 0x00007f54e911a7b8 __GI___clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Segmentation fault (core dumped)
```

Unfortunately, there is nothing to attach:
```
$ ls -la /tmp/diagnostics-620011
total 0
drwxr-x---  2 eustas primarygroup   60 Nov 13 09:37 .
drwxrwxrwt 44 root   root         1660 Nov 13 09:40 ..
-rw-r-----  1 eustas primarygroup    0 Nov 13 09:37 diagnostics.log
```
</details>


---

### Comment 2 - DavidSpickett

We need to know how to reproduce this. Including:
* What libjxl is (this perhaps https://github.com/libjxl/libjxl)
* What compiler you built libjxl with and what settings were used (if it reproduces with libjxl's default build instructions, that's great)
* What version of lldb you are using, and how it was built (judging by the backtrace, it was a debug, or release with asserts build)
* How you invoked lldb when debugging the ctest run

I see `Statusline::Redraw` in the stack, so you could try adding `settings set show-statusline off` to your `.lldbinit` file (or create one). If this does not avoid the crash, try starting lldb just as `lldb` then manually entering that command, followed by the one to start the ctest binary.

---

### Comment 3 - JDevlieghere

Based on the backtrace, this is a version of LLDB without https://github.com/llvm/llvm-project/pull/159887 which fixed a handful of race conditions w.r.t. computing the execution context on the default event handler thread. I bet this is one of those.

---

