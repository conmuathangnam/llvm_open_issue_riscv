# lldb crash

**Author:** jcelerier
**URL:** https://github.com/llvm/llvm-project/issues/170998
**Status:** Open
**Labels:** lldb, crash

## Body

```

 #0 0x00007f24fd0dc1ea llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.21.1+0x78dc1ea)
 #1 0x00007f24fd04a7ac (/usr/lib/libLLVM.so.21.1+0x784a7ac)
 #2 0x00007f24f5044900 (/usr/lib/libc.so.6+0x44900)
 #3 0x00007f24fdea451b std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release_last_use() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:174:12
 #4 0x00007f24fdea451b std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>::_M_release_last_use_cold() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:198:28
 #5 0x00007f24fe39fbdd std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1069:21
 #6 0x00007f24fe39fbdd std::__shared_ptr<lldb_private::StopInfo, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
 #7 0x00007f24fe39fbdd std::shared_ptr<lldb_private::StopInfo>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
 #8 0x00007f24fe39fbdd lldb_private::Thread::ShouldStop(lldb_private::Event*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Thread.cpp:995:1
 #9 0x00007f24fe39fbdd lldb_private::ThreadList::ShouldStop(lldb_private::Event*) (.constprop.0) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/ThreadList.cpp:323:58
#10 0x00007f24fe3182ee lldb_private::Process::ShouldBroadcastEvent(lldb_private::Event*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3706:50
#11 0x00007f24fe3182ee lldb_private::Process::HandlePrivateEvent(std::shared_ptr<lldb_private::Event>&) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3935:53
#12 0x00007f24fe328dc9 lldb_private::Process::RunPrivateStateThread(bool) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:4133:25
#13 0x00007f24fe328dc9 operator() /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3802:41
#14 0x00007f24fe328dc9 __invoke_impl<void*, lldb_private::Process::StartPrivateStateThread(bool)::<lambda()>&> /usr/include/c++/15.2.1/bits/invoke.h:63:36
#15 0x00007f24fe328dc9 __invoke_r<void*, lldb_private::Process::StartPrivateStateThread(bool)::<lambda()>&> /usr/include/c++/15.2.1/bits/invoke.h:116:35
#16 0x00007f24fe328dc9 _M_invoke /usr/include/c++/15.2.1/bits/std_function.h:292:30
#17 0x00007f24fe225457 std::function<void* ()>::operator()() const /usr/include/c++/15.2.1/bits/std_function.h:593:9
#18 0x00007f24fe225457 lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Host/common/HostNativeThreadBase.cpp:62:23
#19 0x00007f24f50a80d6 start_thread /usr/src/debug/glibc/glibc/nptl/pthread_create.c:448:8
#20 0x00007f24f513b05c (/usr/lib/libc.so.6+0x13b05c)
LLDB diagnostics will be written to /tmp/diagnostics-3b9b9c
Please include the directory content when filing a bug report
fish: Job 1, 'lldb ./ossia-score' terminated by signal SIGSEGV (Address boundary error)
```


when trying to run my app.

os: archlinux (cachyos, up-to-date as of 2025-12-06) x86_64
lldb: lldb version 21.1.6

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: Jean-Michaël Celerier (jcelerier)

<details>
```

 #<!-- -->0 0x00007f24fd0dc1ea llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.21.1+0x78dc1ea)
 #<!-- -->1 0x00007f24fd04a7ac (/usr/lib/libLLVM.so.21.1+0x784a7ac)
 #<!-- -->2 0x00007f24f5044900 (/usr/lib/libc.so.6+0x44900)
 #<!-- -->3 0x00007f24fdea451b std::_Sp_counted_base&lt;(__gnu_cxx::_Lock_policy)2&gt;::_M_release_last_use() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:174:12
 #<!-- -->4 0x00007f24fdea451b std::_Sp_counted_base&lt;(__gnu_cxx::_Lock_policy)2&gt;::_M_release_last_use_cold() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:198:28
 #<!-- -->5 0x00007f24fe39fbdd std::__shared_count&lt;(__gnu_cxx::_Lock_policy)2&gt;::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1069:21
 #<!-- -->6 0x00007f24fe39fbdd std::__shared_ptr&lt;lldb_private::StopInfo, (__gnu_cxx::_Lock_policy)2&gt;::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
 #<!-- -->7 0x00007f24fe39fbdd std::shared_ptr&lt;lldb_private::StopInfo&gt;::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
 #<!-- -->8 0x00007f24fe39fbdd lldb_private::Thread::ShouldStop(lldb_private::Event*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Thread.cpp:995:1
 #<!-- -->9 0x00007f24fe39fbdd lldb_private::ThreadList::ShouldStop(lldb_private::Event*) (.constprop.0) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/ThreadList.cpp:323:58
#<!-- -->10 0x00007f24fe3182ee lldb_private::Process::ShouldBroadcastEvent(lldb_private::Event*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3706:50
#<!-- -->11 0x00007f24fe3182ee lldb_private::Process::HandlePrivateEvent(std::shared_ptr&lt;lldb_private::Event&gt;&amp;) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3935:53
#<!-- -->12 0x00007f24fe328dc9 lldb_private::Process::RunPrivateStateThread(bool) /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:4133:25
#<!-- -->13 0x00007f24fe328dc9 operator() /usr/src/debug/lldb/lldb-21.1.6.src/source/Target/Process.cpp:3802:41
#<!-- -->14 0x00007f24fe328dc9 __invoke_impl&lt;void*, lldb_private::Process::StartPrivateStateThread(bool)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/15.2.1/bits/invoke.h:63:36
#<!-- -->15 0x00007f24fe328dc9 __invoke_r&lt;void*, lldb_private::Process::StartPrivateStateThread(bool)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/15.2.1/bits/invoke.h:116:35
#<!-- -->16 0x00007f24fe328dc9 _M_invoke /usr/include/c++/15.2.1/bits/std_function.h:292:30
#<!-- -->17 0x00007f24fe225457 std::function&lt;void* ()&gt;::operator()() const /usr/include/c++/15.2.1/bits/std_function.h:593:9
#<!-- -->18 0x00007f24fe225457 lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) /usr/src/debug/lldb/lldb-21.1.6.src/source/Host/common/HostNativeThreadBase.cpp:62:23
#<!-- -->19 0x00007f24f50a80d6 start_thread /usr/src/debug/glibc/glibc/nptl/pthread_create.c:448:8
#<!-- -->20 0x00007f24f513b05c (/usr/lib/libc.so.6+0x13b05c)
LLDB diagnostics will be written to /tmp/diagnostics-3b9b9c
Please include the directory content when filing a bug report
fish: Job 1, 'lldb ./ossia-score' terminated by signal SIGSEGV (Address boundary error)
```


when trying to run my app.

os: archlinux (cachyos, up-to-date as of 2025-12-06) x86_64
lldb: lldb version 21.1.6
</details>


---

### Comment 2 - EugeneZelenko

@jcelerier: Could you please provide reproducer?

---

### Comment 3 - jcelerier

Okay, here's a docker-based setup that recreates my userland. It's pretty hard as I have a few gigabytes of build artifacts: https://drive.google.com/file/d/1VoRKU0pHOR34NG6hT3GUW0xp2WTrgpHI/view?usp=sharing

The extracted folder contains a home subfolder which should map to /home in the container.

```bash
$ sudo docker run -v $PWD/home:/home -it archlinux:latest /bin/bash
# pacman -Syyu --noconfirm
# pacman -Sy fftw ffmpeg libbacktrace clang lldb libxcb libxkbcommon-x11 xcb-util  pipewire-jack libc++ double-conversion libb2 md4c
# lldb -- ./home/jcelerier/builds/build-asan-ubsan/ossia-score --platform minimal
```

I am stuck at this place with docker as it complains with

    error: Cannot launch '/home/jcelerier/builds/build-asan-ubsan/ossia-score': personality set failed: Operation not permitted

but I assume this is an host-side thing that you can solve by granting appropriate permissions to docker ?

---

### Comment 4 - DavidSpickett

If the bug does not rely on ALSR being disabled, you can add `-o "(lldb) settings set target.disable-aslr false"` to the lldb command line, before the program file itself.

(unlike GDB, we do not try to disable and carry on if it doesn't work - https://github.com/llvm/llvm-project/issues/61899, we should though)

In the unlikely case you need ASLR to hit the bug, I think you have to allow disabling it on the host machine, then run the container. Again, unlikely, and best avoided.

---

