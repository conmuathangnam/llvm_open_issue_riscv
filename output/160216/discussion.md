# Tests that set `target.process.follow-fork-mode = child` crash when computing the execution context on the event thread

**Author:** JDevlieghere
**URL:** https://github.com/llvm/llvm-project/issues/160216
**Status:** Open
**Labels:** lldb, crash

## Body

The tests that set `target.process.follow-fork-mode = child` crash when computing the execution context on the event thread. I'm observing this in the context of https://github.com/llvm/llvm-project/pull/148994 and https://github.com/llvm/llvm-project/pull/159887. 

The tests fail consistently in CI and relatively reliably when running the test suite in a Linux VM. However, running the exact same `lldb` invocation outside the test never reproduces the issue (not within 200 runs) and neither does it trip up ASan, UBSan or TSan. 

The issue is reproducible by calling `Debugger::GetSelectedContext` from the default event handler. 

"Pretty" backtrace:

```
 #0 0x0000000000264268 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #1 0x0000000000261c75 llvm::sys::RunSignalHandlers() /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x0000000000265021 SignalHandler(int, siginfo_t*, void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #3 0x00007a9f4c6e7330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007a9f4c7c928d syscall (/lib/x86_64-linux-gnu/libc.so.6+0x12728d)
 #5 0x00007a9f52f98ddb SignalHandler(int, siginfo_t*, void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:427:14
 #6 0x00007a9f4c6e7330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #7 0x00007a9f52a245bc operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1523:69
 #8 0x00007a9f52a245bc operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:414:65
 #9 0x00007a9f52a245bc lldb_private::UnwindLLDB::GetRegisterContextForFrameNum(unsigned int) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:472:16
#10 0x00007a9f52a28767 get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1666:16
#11 0x00007a9f52a28767 _M_get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1363:66
#12 0x00007a9f52a28767 operator-> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1357:9
#13 0x00007a9f52a28767 lldb_private::RegisterContextUnwind::GetFullUnwindPlanForFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:804:24
#14 0x00007a9f52a27427 lldb_private::RegisterContextUnwind::InitializeNonZerothFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:645:5
#15 0x00007a9f52a24e33 IsFrameZero /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:728:73
#16 0x00007a9f52a24e33 lldb_private::RegisterContextUnwind::RegisterContextUnwind(lldb_private::Thread&, std::shared_ptr<lldb_private::RegisterContextUnwind> const&, lldb_private::SymbolContext&, unsigned int, lldb_private::UnwindLLDB&) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:80:7
#17 0x00007a9f52a238ba __shared_ptr<lldb_private::RegisterContextUnwind, void> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1469:4
#18 0x00007a9f52a238ba shared_ptr<lldb_private::RegisterContextUnwind, void> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:214:25
#19 0x00007a9f52a238ba lldb_private::UnwindLLDB::GetOneMoreFrame(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:129:25
#20 0x00007a9f52a22dff __shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1532:20
#21 0x00007a9f52a22dff operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1620:2
#22 0x00007a9f52a22dff operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:440:27
#23 0x00007a9f52a22dff lldb_private::UnwindLLDB::AddOneMoreFrame(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:337:15
#24 0x00007a9f52a23514 size /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:993:66
#25 0x00007a9f52a23514 lldb_private::UnwindLLDB::UpdateUnwindPlanForFirstFrameIfInvalid(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:315:37
#26 0x00007a9f52a22c15 lldb_private::UnwindLLDB::AddFirstFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:0:3
#27 0x00007a9f52a2418f lldb_private::UnwindLLDB::DoGetFrameInfoAtIndex(unsigned int, unsigned long&, unsigned long&, bool&) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:400:9
#28 0x00007a9f5299f011 GetFrameInfoAtIndex /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/include/lldb/Target/Unwind.h:53:12
#29 0x00007a9f5299f011 lldb_private::StackFrameList::FetchFramesUpTo(unsigned int, InterruptionControl) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:435:41
#30 0x00007a9f5299e903 lldb_private::StackFrameList::GetFramesUpTo(unsigned int, InterruptionControl) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:368:21
#31 0x00007a9f529a0a0f lldb_private::StackFrameList::GetFrameAtIndex(unsigned int) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:634:7
#32 0x00007a9f529ed3a8 get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1666:16
#33 0x00007a9f529ed3a8 lldb_private::Thread::GetSelectedFrame(SelectMostRelevant) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/Thread.cpp:276:34
#34 0x00007a9f5293e7cd operator bool /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1670:23
#35 0x00007a9f5293e7cd lldb_private::ExecutionContextRef::SetProcessPtr(lldb_private::Process*, bool) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/ExecutionContext.cpp:554:16
#36 0x00007a9f527a201b operator=<lldb_private::ExecutionContextRef> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:852:14
#37 0x00007a9f527a201b lldb_private::Debugger::DefaultEventHandler() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Core/Debugger.cpp:2143:27
#38 0x00007a9f527a6509 std::_Function_handler<void* (), lldb_private::Debugger::StartEventHandlerThread()::$_0>::_M_invoke(std::_Any_data const&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:290:2
#39 0x00007a9f52862069 operator() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:591:9
#40 0x00007a9f52862069 lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Host/common/HostNativeThreadBase.cpp:62:10
#41 0x00007a9f4c73eaa4 (/lib/x86_64-linux-gnu/libc.so.6+0x9caa4)
#42 0x00007a9f4c7cbc6c (/lib/x86_64-linux-gnu/libc.so.6+0x129c6c)
```

## Comments

### Comment 1 - JDevlieghere

Hey @mgorny, any chance you could help me debug this. I can't reproduce this outside of the test run. My suspicion is that we're mucking with the process/thread/frame when we're doing the following of the child, but looking at the PR that adds support for (v)fork, I don't actually see where that might be happening. 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-lldb

Author: Jonas Devlieghere (JDevlieghere)

<details>
The tests that set `target.process.follow-fork-mode = child` crash when computing the execution context on the event thread. I'm observing this in the context of https://github.com/llvm/llvm-project/pull/148994 and https://github.com/llvm/llvm-project/pull/159887. 

The tests fail consistently in CI and relatively reliably when running the test suite in a Linux VM. However, running the exact same `lldb` invocation outside the test never reproduces the issue (not within 200 runs) and neither does it trip up ASan, UBSan or TSan. 

The issue is reproducible by calling `Debugger::GetSelectedContext` from the default event handler. 

"Pretty" backtrace:

```
 #<!-- -->0 0x0000000000264268 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->1 0x0000000000261c75 llvm::sys::RunSignalHandlers() /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->2 0x0000000000265021 SignalHandler(int, siginfo_t*, void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->3 0x00007a9f4c6e7330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007a9f4c7c928d syscall (/lib/x86_64-linux-gnu/libc.so.6+0x12728d)
 #<!-- -->5 0x00007a9f52f98ddb SignalHandler(int, siginfo_t*, void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/llvm/lib/Support/Unix/Signals.inc:427:14
 #<!-- -->6 0x00007a9f4c6e7330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->7 0x00007a9f52a245bc operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1523:69
 #<!-- -->8 0x00007a9f52a245bc operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:414:65
 #<!-- -->9 0x00007a9f52a245bc lldb_private::UnwindLLDB::GetRegisterContextForFrameNum(unsigned int) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:472:16
#<!-- -->10 0x00007a9f52a28767 get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1666:16
#<!-- -->11 0x00007a9f52a28767 _M_get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1363:66
#<!-- -->12 0x00007a9f52a28767 operator-&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1357:9
#<!-- -->13 0x00007a9f52a28767 lldb_private::RegisterContextUnwind::GetFullUnwindPlanForFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:804:24
#<!-- -->14 0x00007a9f52a27427 lldb_private::RegisterContextUnwind::InitializeNonZerothFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:645:5
#<!-- -->15 0x00007a9f52a24e33 IsFrameZero /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:728:73
#<!-- -->16 0x00007a9f52a24e33 lldb_private::RegisterContextUnwind::RegisterContextUnwind(lldb_private::Thread&amp;, std::shared_ptr&lt;lldb_private::RegisterContextUnwind&gt; const&amp;, lldb_private::SymbolContext&amp;, unsigned int, lldb_private::UnwindLLDB&amp;) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/RegisterContextUnwind.cpp:80:7
#<!-- -->17 0x00007a9f52a238ba __shared_ptr&lt;lldb_private::RegisterContextUnwind, void&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1469:4
#<!-- -->18 0x00007a9f52a238ba shared_ptr&lt;lldb_private::RegisterContextUnwind, void&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:214:25
#<!-- -->19 0x00007a9f52a238ba lldb_private::UnwindLLDB::GetOneMoreFrame(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:129:25
#<!-- -->20 0x00007a9f52a22dff __shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1532:20
#<!-- -->21 0x00007a9f52a22dff operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1620:2
#<!-- -->22 0x00007a9f52a22dff operator= /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr.h:440:27
#<!-- -->23 0x00007a9f52a22dff lldb_private::UnwindLLDB::AddOneMoreFrame(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:337:15
#<!-- -->24 0x00007a9f52a23514 size /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:993:66
#<!-- -->25 0x00007a9f52a23514 lldb_private::UnwindLLDB::UpdateUnwindPlanForFirstFrameIfInvalid(lldb_private::ABI*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:315:37
#<!-- -->26 0x00007a9f52a22c15 lldb_private::UnwindLLDB::AddFirstFrame() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:0:3
#<!-- -->27 0x00007a9f52a2418f lldb_private::UnwindLLDB::DoGetFrameInfoAtIndex(unsigned int, unsigned long&amp;, unsigned long&amp;, bool&amp;) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/UnwindLLDB.cpp:400:9
#<!-- -->28 0x00007a9f5299f011 GetFrameInfoAtIndex /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/include/lldb/Target/Unwind.h:53:12
#<!-- -->29 0x00007a9f5299f011 lldb_private::StackFrameList::FetchFramesUpTo(unsigned int, InterruptionControl) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:435:41
#<!-- -->30 0x00007a9f5299e903 lldb_private::StackFrameList::GetFramesUpTo(unsigned int, InterruptionControl) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:368:21
#<!-- -->31 0x00007a9f529a0a0f lldb_private::StackFrameList::GetFrameAtIndex(unsigned int) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/StackFrameList.cpp:634:7
#<!-- -->32 0x00007a9f529ed3a8 get /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1666:16
#<!-- -->33 0x00007a9f529ed3a8 lldb_private::Thread::GetSelectedFrame(SelectMostRelevant) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/Thread.cpp:276:34
#<!-- -->34 0x00007a9f5293e7cd operator bool /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1670:23
#<!-- -->35 0x00007a9f5293e7cd lldb_private::ExecutionContextRef::SetProcessPtr(lldb_private::Process*, bool) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Target/ExecutionContext.cpp:554:16
#<!-- -->36 0x00007a9f527a201b operator=&lt;lldb_private::ExecutionContextRef&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:852:14
#<!-- -->37 0x00007a9f527a201b lldb_private::Debugger::DefaultEventHandler() /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Core/Debugger.cpp:2143:27
#<!-- -->38 0x00007a9f527a6509 std::_Function_handler&lt;void* (), lldb_private::Debugger::StartEventHandlerThread()::$_0&gt;::_M_invoke(std::_Any_data const&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:290:2
#<!-- -->39 0x00007a9f52862069 operator() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_function.h:591:9
#<!-- -->40 0x00007a9f52862069 lldb_private::HostNativeThreadBase::ThreadCreateTrampoline(void*) /home/gha/actions-runner/_work/llvm-project/llvm-project/lldb/source/Host/common/HostNativeThreadBase.cpp:62:10
#<!-- -->41 0x00007a9f4c73eaa4 (/lib/x86_64-linux-gnu/libc.so.6+0x9caa4)
#<!-- -->42 0x00007a9f4c7cbc6c (/lib/x86_64-linux-gnu/libc.so.6+0x129c6c)
```
</details>


---

### Comment 3 - mgorny

To be honest, I don't really know where to start. I haven't debugged C++ for years (and really grateful I didn't have to). Were you perhaps able to attach a debugger while running the tests?

---

