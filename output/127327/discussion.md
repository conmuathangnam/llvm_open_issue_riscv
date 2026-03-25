# [clang-cl] The `[[noreturn]]` attribute causes parsing to crash by declaring a concurrent function that returns.

**Author:** YuHuanTin
**URL:** https://github.com/llvm/llvm-project/issues/127327
**Status:** Closed
**Labels:** clang:frontend, clang-cl, crash, coroutines
**Closed Date:** 2025-02-18T21:30:41Z

## Body

```
D:\LLVM\bin\clang-cl.exe   -TP -DCRCPP_IMPORT_API -imsvcD:\Clion\vcpkg\installed\x64-windows\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\New.dir\main.cpp.obj /FdCMakeFiles\New.dir\ -c -- D:\Clion\Project\New\main.cpp
D:\Clion\Project\New\main.cpp(7,41): warning: function 'foo' declared 'noreturn' should not return [-Winvalid-noreturn]
    7 | [[noreturn]] concurrencpp::result<void> foo() {
      |                                         ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: D:\\LLVM\\bin\\clang-cl.exe -TP -DCRCPP_IMPORT_API -imsvcD:\\Clion\\vcpkg\\installed\\x64-windows\\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\\New.dir\\main.cpp.obj /FdCMakeFiles\\New.dir\\ -c -- D:\\Clion\\Project\\New\\main.cpp
1.	<eof> parser at end of file
2.	D:\Clion\Project\New\main.cpp:7:47: parsing function body 'foo'
Exception Code: 0xC0000005
 #0 0x00007ff7e22fcd44 (D:\LLVM\bin\clang-cl.exe+0x77cd44)
 #1 0x00007ff7e22fcb30 (D:\LLVM\bin\clang-cl.exe+0x77cb30)
 #2 0x00007ff7e3a2cf6f (D:\LLVM\bin\clang-cl.exe+0x1eacf6f)
 #3 0x00007ff7e2c95037 (D:\LLVM\bin\clang-cl.exe+0x1115037)
 #4 0x00007ff7e2c9433a (D:\LLVM\bin\clang-cl.exe+0x111433a)
 #5 0x00007ff7e2c93e83 (D:\LLVM\bin\clang-cl.exe+0x1113e83)
 #6 0x00007ff7e2c90eb5 (D:\LLVM\bin\clang-cl.exe+0x1110eb5)
 #7 0x00007ff7e2c90e2a (D:\LLVM\bin\clang-cl.exe+0x1110e2a)
 #8 0x00007ff7e2d41fa8 (D:\LLVM\bin\clang-cl.exe+0x11c1fa8)
 #9 0x00007ff7e2c82167 (D:\LLVM\bin\clang-cl.exe+0x1102167)
#10 0x00007ff7e2c7f6b4 (D:\LLVM\bin\clang-cl.exe+0x10ff6b4)
#11 0x00007ff7e2c76d59 (D:\LLVM\bin\clang-cl.exe+0x10f6d59)
#12 0x00007ff7e2ae8ea5 (D:\LLVM\bin\clang-cl.exe+0xf68ea5)
#13 0x00007ff7e2ae0c12 (D:\LLVM\bin\clang-cl.exe+0xf60c12)
#14 0x00007ff7e2add88a (D:\LLVM\bin\clang-cl.exe+0xf5d88a)
#15 0x00007ff7e36e09e6 (D:\LLVM\bin\clang-cl.exe+0x1b609e6)
#16 0x00007ff7e36e064e (D:\LLVM\bin\clang-cl.exe+0x1b6064e)
#17 0x00007ff7e2143a61 (D:\LLVM\bin\clang-cl.exe+0x5c3a61)
#18 0x00007ff7e21435f2 (D:\LLVM\bin\clang-cl.exe+0x5c35f2)
#19 0x00007ff7e2140366 (D:\LLVM\bin\clang-cl.exe+0x5c0366)
#20 0x00007ff7e213d325 (D:\LLVM\bin\clang-cl.exe+0x5bd325)
#21 0x00007ff7e213bcbe (D:\LLVM\bin\clang-cl.exe+0x5bbcbe)
#22 0x00007ff7e28555cd (D:\LLVM\bin\clang-cl.exe+0xcd55cd)
#23 0x00007ff7e22c4175 (D:\LLVM\bin\clang-cl.exe+0x744175)
#24 0x00007ff7e22c3f68 (D:\LLVM\bin\clang-cl.exe+0x743f68)
#25 0x00007ff7e1fcc0c8 (D:\LLVM\bin\clang-cl.exe+0x44c0c8)
#26 0x00007ff7e1fcb44e (D:\LLVM\bin\clang-cl.exe+0x44b44e)
#27 0x00007ff7e1fcafc7 (D:\LLVM\bin\clang-cl.exe+0x44afc7)
#28 0x00007ff7e1fc2756 (D:\LLVM\bin\clang-cl.exe+0x442756)
#29 0x00007ff7e1fbfed3 (D:\LLVM\bin\clang-cl.exe+0x43fed3)
#30 0x00007ff7e3782bc8 (D:\LLVM\bin\clang-cl.exe+0x1c02bc8)
#31 0x00007ffbdf6b7374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#32 0x00007ffbe135cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\LLVM\bin
clang-cl: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.cpp
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.sh
clang-cl: note: diagnostic msg: 

********************
ninja: build stopped: subcommand failed.
```

The full code
```c++
#include <concurrencpp/concurrencpp.h>

concurrencpp::result<void> bar() {
    co_return;
}

[[noreturn]] concurrencpp::result<void> foo() {
    co_await bar();
}


```

Yes, I know it's a bad practice to flag `[[noreturn]]` on a function that might return, but it should get a warn or error, but the compiler crashes

the crash file

[crash.zip](https://github.com/user-attachments/files/18811028/crash.zip)

## Comments

### Comment 1 - Nerixyz

Reduced to 

```cpp
namespace std {
template <class = void> struct coroutine_handle {
  static constexpr coroutine_handle from_address(void *const) noexcept {
    return {};
  }
  constexpr operator coroutine_handle<>() const noexcept { return {}; }
};

struct suspend_always {
  bool await_ready() noexcept { return false; }
  void await_suspend(coroutine_handle<>) noexcept {}
  void await_resume() noexcept {}
};

template <class _Ret, class...>
struct coroutine_traits {
    using promise_type = typename _Ret::promise_type;
};
} // namespace std

struct promise;

struct awaitable {
  bool await_ready() { return false; }
  void await_suspend(std::coroutine_handle<>) {}
  void await_resume() {}
};

struct coroutine : std::coroutine_handle<> {
  using promise_type = promise;
};

struct promise {
  coroutine get_return_object() { return {}; }
  std::suspend_always initial_suspend() noexcept { return {}; }
  std::suspend_always final_suspend() noexcept { return {}; }
  void return_void() {}
  void unhandled_exception() {}
};

[[noreturn]] coroutine f() { co_await awaitable{}; }
```

---

Coroutines [don't have diagnostic IDs set](https://github.com/llvm/llvm-project/blob/f75126eeabba13ce2aab53c2e4296fca12b9da0d/clang/lib/Sema/AnalysisBasedWarnings.cpp#L591-L594) when [`CheckFallThroughForBody`](https://github.com/llvm/llvm-project/blob/f75126eeabba13ce2aab53c2e4296fca12b9da0d/clang/lib/Sema/AnalysisBasedWarnings.cpp#L2764-L2766) is called, which then tries to emit [`diag_AlwaysFallThrough_HasNoReturn`](https://github.com/llvm/llvm-project/blob/f75126eeabba13ce2aab53c2e4296fca12b9da0d/clang/lib/Sema/AnalysisBasedWarnings.cpp#L723).

A simple fix would be to use the same diagnostics as for functions. This would, however, generate two warnings:

```
main.cpp(41,24): warning: function 'f' declared 'noreturn' should not return [-Winvalid-noreturn]
   41 | [[noreturn]] coroutine f() { co_await awaitable{}; }
      |                        ^
main.cpp(41,52): warning: function declared 'noreturn' should not return [-Winvalid-noreturn]
   41 | [[noreturn]] coroutine f() { co_await awaitable{}; }
      |                                                    ^
2 warnings generated.
```

The first warning, also visible in the current output, is emitted [when building the return statement](https://github.com/llvm/llvm-project/blob/f75126eeabba13ce2aab53c2e4296fca12b9da0d/clang/lib/Sema/SemaCoroutine.cpp#L1823), which [discovers that the function is `[[noreturn]]`](https://github.com/llvm/llvm-project/blob/f75126eeabba13ce2aab53c2e4296fca12b9da0d/clang/lib/Sema/SemaStmt.cpp#L3914).

I'd fix this, but would like to know if there is any preference for the diagnostic (i.e. should it be omitted or more descriptive - in this case we know that the function is a coroutine).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: YuHuanTin (YuHuanTin)

<details>
```
D:\LLVM\bin\clang-cl.exe   -TP -DCRCPP_IMPORT_API -imsvcD:\Clion\vcpkg\installed\x64-windows\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\New.dir\main.cpp.obj /FdCMakeFiles\New.dir\ -c -- D:\Clion\Project\New\main.cpp
D:\Clion\Project\New\main.cpp(7,41): warning: function 'foo' declared 'noreturn' should not return [-Winvalid-noreturn]
    7 | [[noreturn]] concurrencpp::result&lt;void&gt; foo() {
      |                                         ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: D:\\LLVM\\bin\\clang-cl.exe -TP -DCRCPP_IMPORT_API -imsvcD:\\Clion\\vcpkg\\installed\\x64-windows\\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\\New.dir\\main.cpp.obj /FdCMakeFiles\\New.dir\\ -c -- D:\\Clion\\Project\\New\\main.cpp
1.	&lt;eof&gt; parser at end of file
2.	D:\Clion\Project\New\main.cpp:7:47: parsing function body 'foo'
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff7e22fcd44 (D:\LLVM\bin\clang-cl.exe+0x77cd44)
 #<!-- -->1 0x00007ff7e22fcb30 (D:\LLVM\bin\clang-cl.exe+0x77cb30)
 #<!-- -->2 0x00007ff7e3a2cf6f (D:\LLVM\bin\clang-cl.exe+0x1eacf6f)
 #<!-- -->3 0x00007ff7e2c95037 (D:\LLVM\bin\clang-cl.exe+0x1115037)
 #<!-- -->4 0x00007ff7e2c9433a (D:\LLVM\bin\clang-cl.exe+0x111433a)
 #<!-- -->5 0x00007ff7e2c93e83 (D:\LLVM\bin\clang-cl.exe+0x1113e83)
 #<!-- -->6 0x00007ff7e2c90eb5 (D:\LLVM\bin\clang-cl.exe+0x1110eb5)
 #<!-- -->7 0x00007ff7e2c90e2a (D:\LLVM\bin\clang-cl.exe+0x1110e2a)
 #<!-- -->8 0x00007ff7e2d41fa8 (D:\LLVM\bin\clang-cl.exe+0x11c1fa8)
 #<!-- -->9 0x00007ff7e2c82167 (D:\LLVM\bin\clang-cl.exe+0x1102167)
#<!-- -->10 0x00007ff7e2c7f6b4 (D:\LLVM\bin\clang-cl.exe+0x10ff6b4)
#<!-- -->11 0x00007ff7e2c76d59 (D:\LLVM\bin\clang-cl.exe+0x10f6d59)
#<!-- -->12 0x00007ff7e2ae8ea5 (D:\LLVM\bin\clang-cl.exe+0xf68ea5)
#<!-- -->13 0x00007ff7e2ae0c12 (D:\LLVM\bin\clang-cl.exe+0xf60c12)
#<!-- -->14 0x00007ff7e2add88a (D:\LLVM\bin\clang-cl.exe+0xf5d88a)
#<!-- -->15 0x00007ff7e36e09e6 (D:\LLVM\bin\clang-cl.exe+0x1b609e6)
#<!-- -->16 0x00007ff7e36e064e (D:\LLVM\bin\clang-cl.exe+0x1b6064e)
#<!-- -->17 0x00007ff7e2143a61 (D:\LLVM\bin\clang-cl.exe+0x5c3a61)
#<!-- -->18 0x00007ff7e21435f2 (D:\LLVM\bin\clang-cl.exe+0x5c35f2)
#<!-- -->19 0x00007ff7e2140366 (D:\LLVM\bin\clang-cl.exe+0x5c0366)
#<!-- -->20 0x00007ff7e213d325 (D:\LLVM\bin\clang-cl.exe+0x5bd325)
#<!-- -->21 0x00007ff7e213bcbe (D:\LLVM\bin\clang-cl.exe+0x5bbcbe)
#<!-- -->22 0x00007ff7e28555cd (D:\LLVM\bin\clang-cl.exe+0xcd55cd)
#<!-- -->23 0x00007ff7e22c4175 (D:\LLVM\bin\clang-cl.exe+0x744175)
#<!-- -->24 0x00007ff7e22c3f68 (D:\LLVM\bin\clang-cl.exe+0x743f68)
#<!-- -->25 0x00007ff7e1fcc0c8 (D:\LLVM\bin\clang-cl.exe+0x44c0c8)
#<!-- -->26 0x00007ff7e1fcb44e (D:\LLVM\bin\clang-cl.exe+0x44b44e)
#<!-- -->27 0x00007ff7e1fcafc7 (D:\LLVM\bin\clang-cl.exe+0x44afc7)
#<!-- -->28 0x00007ff7e1fc2756 (D:\LLVM\bin\clang-cl.exe+0x442756)
#<!-- -->29 0x00007ff7e1fbfed3 (D:\LLVM\bin\clang-cl.exe+0x43fed3)
#<!-- -->30 0x00007ff7e3782bc8 (D:\LLVM\bin\clang-cl.exe+0x1c02bc8)
#<!-- -->31 0x00007ffbdf6b7374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#<!-- -->32 0x00007ffbe135cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\LLVM\bin
clang-cl: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.cpp
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.sh
clang-cl: note: diagnostic msg: 

********************
ninja: build stopped: subcommand failed.
```

The full code
```c++
#include &lt;concurrencpp/concurrencpp.h&gt;

concurrencpp::result&lt;void&gt; bar() {
    co_return;
}

[[noreturn]] concurrencpp::result&lt;void&gt; foo() {
    co_await bar();
}


```

Yes, I know it's a bad practice to flag `[[noreturn]]` on a function that might return, but it should get a warn or error, but the compiler crashes

the crash file

[crash.zip](https://github.com/user-attachments/files/18811028/crash.zip)
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-coroutines

Author: YuHuanTin (YuHuanTin)

<details>
```
D:\LLVM\bin\clang-cl.exe   -TP -DCRCPP_IMPORT_API -imsvcD:\Clion\vcpkg\installed\x64-windows\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\New.dir\main.cpp.obj /FdCMakeFiles\New.dir\ -c -- D:\Clion\Project\New\main.cpp
D:\Clion\Project\New\main.cpp(7,41): warning: function 'foo' declared 'noreturn' should not return [-Winvalid-noreturn]
    7 | [[noreturn]] concurrencpp::result&lt;void&gt; foo() {
      |                                         ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: D:\\LLVM\\bin\\clang-cl.exe -TP -DCRCPP_IMPORT_API -imsvcD:\\Clion\\vcpkg\\installed\\x64-windows\\include /DWIN32 /D_WINDOWS /EHsc /Ob0 /Od /RTC1 -std:c++latest -MDd -Zi /clang:-Wall -Wextra -Wpedantic -Wconversion /permissive- /showIncludes /FoCMakeFiles\\New.dir\\main.cpp.obj /FdCMakeFiles\\New.dir\\ -c -- D:\\Clion\\Project\\New\\main.cpp
1.	&lt;eof&gt; parser at end of file
2.	D:\Clion\Project\New\main.cpp:7:47: parsing function body 'foo'
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff7e22fcd44 (D:\LLVM\bin\clang-cl.exe+0x77cd44)
 #<!-- -->1 0x00007ff7e22fcb30 (D:\LLVM\bin\clang-cl.exe+0x77cb30)
 #<!-- -->2 0x00007ff7e3a2cf6f (D:\LLVM\bin\clang-cl.exe+0x1eacf6f)
 #<!-- -->3 0x00007ff7e2c95037 (D:\LLVM\bin\clang-cl.exe+0x1115037)
 #<!-- -->4 0x00007ff7e2c9433a (D:\LLVM\bin\clang-cl.exe+0x111433a)
 #<!-- -->5 0x00007ff7e2c93e83 (D:\LLVM\bin\clang-cl.exe+0x1113e83)
 #<!-- -->6 0x00007ff7e2c90eb5 (D:\LLVM\bin\clang-cl.exe+0x1110eb5)
 #<!-- -->7 0x00007ff7e2c90e2a (D:\LLVM\bin\clang-cl.exe+0x1110e2a)
 #<!-- -->8 0x00007ff7e2d41fa8 (D:\LLVM\bin\clang-cl.exe+0x11c1fa8)
 #<!-- -->9 0x00007ff7e2c82167 (D:\LLVM\bin\clang-cl.exe+0x1102167)
#<!-- -->10 0x00007ff7e2c7f6b4 (D:\LLVM\bin\clang-cl.exe+0x10ff6b4)
#<!-- -->11 0x00007ff7e2c76d59 (D:\LLVM\bin\clang-cl.exe+0x10f6d59)
#<!-- -->12 0x00007ff7e2ae8ea5 (D:\LLVM\bin\clang-cl.exe+0xf68ea5)
#<!-- -->13 0x00007ff7e2ae0c12 (D:\LLVM\bin\clang-cl.exe+0xf60c12)
#<!-- -->14 0x00007ff7e2add88a (D:\LLVM\bin\clang-cl.exe+0xf5d88a)
#<!-- -->15 0x00007ff7e36e09e6 (D:\LLVM\bin\clang-cl.exe+0x1b609e6)
#<!-- -->16 0x00007ff7e36e064e (D:\LLVM\bin\clang-cl.exe+0x1b6064e)
#<!-- -->17 0x00007ff7e2143a61 (D:\LLVM\bin\clang-cl.exe+0x5c3a61)
#<!-- -->18 0x00007ff7e21435f2 (D:\LLVM\bin\clang-cl.exe+0x5c35f2)
#<!-- -->19 0x00007ff7e2140366 (D:\LLVM\bin\clang-cl.exe+0x5c0366)
#<!-- -->20 0x00007ff7e213d325 (D:\LLVM\bin\clang-cl.exe+0x5bd325)
#<!-- -->21 0x00007ff7e213bcbe (D:\LLVM\bin\clang-cl.exe+0x5bbcbe)
#<!-- -->22 0x00007ff7e28555cd (D:\LLVM\bin\clang-cl.exe+0xcd55cd)
#<!-- -->23 0x00007ff7e22c4175 (D:\LLVM\bin\clang-cl.exe+0x744175)
#<!-- -->24 0x00007ff7e22c3f68 (D:\LLVM\bin\clang-cl.exe+0x743f68)
#<!-- -->25 0x00007ff7e1fcc0c8 (D:\LLVM\bin\clang-cl.exe+0x44c0c8)
#<!-- -->26 0x00007ff7e1fcb44e (D:\LLVM\bin\clang-cl.exe+0x44b44e)
#<!-- -->27 0x00007ff7e1fcafc7 (D:\LLVM\bin\clang-cl.exe+0x44afc7)
#<!-- -->28 0x00007ff7e1fc2756 (D:\LLVM\bin\clang-cl.exe+0x442756)
#<!-- -->29 0x00007ff7e1fbfed3 (D:\LLVM\bin\clang-cl.exe+0x43fed3)
#<!-- -->30 0x00007ff7e3782bc8 (D:\LLVM\bin\clang-cl.exe+0x1c02bc8)
#<!-- -->31 0x00007ffbdf6b7374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#<!-- -->32 0x00007ffbe135cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\LLVM\bin
clang-cl: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.cpp
clang-cl: note: diagnostic msg: F:\Temp\WinTemp\main-efd334.sh
clang-cl: note: diagnostic msg: 

********************
ninja: build stopped: subcommand failed.
```

The full code
```c++
#include &lt;concurrencpp/concurrencpp.h&gt;

concurrencpp::result&lt;void&gt; bar() {
    co_return;
}

[[noreturn]] concurrencpp::result&lt;void&gt; foo() {
    co_await bar();
}


```

Yes, I know it's a bad practice to flag `[[noreturn]]` on a function that might return, but it should get a warn or error, but the compiler crashes

the crash file

[crash.zip](https://github.com/user-attachments/files/18811028/crash.zip)
</details>


---

