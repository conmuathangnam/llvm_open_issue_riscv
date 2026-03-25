# clang-tidy.exe crash

**Author:** CarlT789
**URL:** https://github.com/llvm/llvm-project/issues/175250
**Status:** Closed
**Labels:** duplicate, clang-tidy, crash
**Closed Date:** 2026-01-14T07:06:41Z

## Body

There didn't seem to be much of crash trace, as some others have reported.  I got this in VS 2026.    LLVM version 20.1.8

I saw previous posts indicating that it happened with the "-modernize" flag, but I am not explicitly including that.

```txt
2>  Running Code Analysis for C/C++...
2>  Generating Code...
  [1/32] Processing file C:\Repos\Software\Test\mctp\MessageMuxServerTests.cpp.
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\18\\Enterprise\\VC\\Tools\\Llvm\\bin\\clang-tidy.exe" C:\\Repos\\Software\\Test\\mctp\\MessageMuxServerTests.cpp C:\\Repos\\Software\\Test\\mctp\\MessageMuxTests.cpp C:\\Repos\\Software\\Test\\mctp\\TunnelingStreamTests.cpp C:\\Repos\\Software\\Test\\mctp\\TunnelledMessageMuxTests.cpp C:\\Repos\\Software\\Test\\PerformanceTimerTests.cpp C:\\Repos\\Software\\Test\\ReaderWriterLockTests.cpp C:\\Repos\\Software\\Test\\StdAfx.cpp C:\\Repos\\Software\\Test\\TestBarcodeCodec.cpp C:\\Repos\\Software\\Test\\TestCommonRegKey.cpp C:\\Repos\\Software\\Test\\TestException.cpp C:\\Repos\\Software\\Test\\TestEventMap.cpp C:\\Repos\\Software\\Test\\TestHistogram.cpp C:\\Repos\\Software\\Test\\TestOdbcClient.cpp C:\\Repos\\Software\\Test\\TestODBCStatement.cpp C:\\Repos\\Software\\Test\\TestODBCStatement_dbo.cpp C:\\Repos\\Software\\Test\\TestRegKey.cpp C:\\Repos\\Software\\Test\\TestResponseManager.cpp C:\\Repos\\Software\\Test\\TestRollingTimer.cpp C:\\Repos\\Software\\Test\\TestSchemeLoader.cpp C:\\Repos\\Software\\Test\\TestSharedMemoryManager.cpp C:\\Repos\\Software\\Test\\TestStdString.cpp C:\\Repos\\Software\\Test\\TestStreamLogger.cpp C:\\Repos\\Software\\Test\\TestStringRegKey.cpp C:\\Repos\\Software\\Test\\TestTestCaseLogger.cpp C:\\Repos\\Software\\Test\\TestThreadedSamplingTimer.cpp C:\\Repos\\Software\\Test\\TestTrackingWindow.cpp C:\\Repos\\Software\\Test\\TestTranslationMap.cpp C:\\Repos\\Software\\Test\\TestUTF8.cpp C:\\Repos\\Software\\Test\\TestWindowsGeneral.cpp C:\\Repos\\Software\\Test\\TestXmlSerializerNoDOM.cpp C:\\Repos\\Software\\Test\\YamlRegKeyTests.cpp C:\\Repos\\Software\\Test\\YamlTests.cpp "-p=..\\build\\Temp\\Test\\x86 Debug\\Test.ClangTidy" "--checks=--checks=bugprone-*, clang-analyzer-*, misc-*, hicpp-*, concurrency-*, -header-filter=.*, -performance-unnecessary-value-param, -performance-inefficient-algorithm, -hicpp-signed-bitwise, -nonportable-include-path, -optin.cplusplus.VirtualCall"
  Exception Code: 0xC0000005
  0x015693B0, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000DC0000) + 0x7A93B0 byte(s)
  0x77291C01, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x81C01 byte(s), wcstombs() + 0x1 byte(s)
C:\Program Files\Microsoft Visual Studio\18\Enterprise\MSBuild\Microsoft\VC\v160\Microsoft.Cpp.ClangTidy.targets(228,5): error MSB6006: "clang-tidy.exe" exited with code -1073741819.
========== Rebuild All: 0 succeeded, 2 failed, 0 skipped ==========
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (CarlT789)

<details>
There didn't seem to be much of crash trace, as some others have reported.  I got this in VS 2026.    LLVM version 20.1.8

I saw previous posts indicating that it happened with the "-modernize" flag, but I am not explicitly including that.

2&gt;  Running Code Analysis for C/C++...
2&gt;  Generating Code...
  [1/32] Processing file C:\Repos\Software\Test\mctp\MessageMuxServerTests.cpp.
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\18\\Enterprise\\VC\\Tools\\Llvm\\bin\\clang-tidy.exe" C:\\Repos\\Software\\Test\\mctp\\MessageMuxServerTests.cpp C:\\Repos\\Software\\Test\\mctp\\MessageMuxTests.cpp C:\\Repos\\Software\\Test\\mctp\\TunnelingStreamTests.cpp C:\\Repos\\Software\\Test\\mctp\\TunnelledMessageMuxTests.cpp C:\\Repos\\Software\\Test\\PerformanceTimerTests.cpp C:\\Repos\\Software\\Test\\ReaderWriterLockTests.cpp C:\\Repos\\Software\\Test\\StdAfx.cpp C:\\Repos\\Software\\Test\\TestBarcodeCodec.cpp C:\\Repos\\Software\\Test\\TestCommonRegKey.cpp C:\\Repos\\Software\\Test\\TestException.cpp C:\\Repos\\Software\\Test\\TestEventMap.cpp C:\\Repos\\Software\\Test\\TestHistogram.cpp C:\\Repos\\Software\\Test\\TestOdbcClient.cpp C:\\Repos\\Software\\Test\\TestODBCStatement.cpp C:\\Repos\\Software\\Test\\TestODBCStatement_dbo.cpp C:\\Repos\\Software\\Test\\TestRegKey.cpp C:\\Repos\\Software\\Test\\TestResponseManager.cpp C:\\Repos\\Software\\Test\\TestRollingTimer.cpp C:\\Repos\\Software\\Test\\TestSchemeLoader.cpp C:\\Repos\\Software\\Test\\TestSharedMemoryManager.cpp C:\\Repos\\Software\\Test\\TestStdString.cpp C:\\Repos\\Software\\Test\\TestStreamLogger.cpp C:\\Repos\\Software\\Test\\TestStringRegKey.cpp C:\\Repos\\Software\\Test\\TestTestCaseLogger.cpp C:\\Repos\\Software\\Test\\TestThreadedSamplingTimer.cpp C:\\Repos\\Software\\Test\\TestTrackingWindow.cpp C:\\Repos\\Software\\Test\\TestTranslationMap.cpp C:\\Repos\\Software\\Test\\TestUTF8.cpp C:\\Repos\\Software\\Test\\TestWindowsGeneral.cpp C:\\Repos\\Software\\Test\\TestXmlSerializerNoDOM.cpp C:\\Repos\\Software\\Test\\YamlRegKeyTests.cpp C:\\Repos\\Software\\Test\\YamlTests.cpp "-p=..\\build\\Temp\\Test\\x86 Debug\\Test.ClangTidy" "--checks=--checks=bugprone-*, clang-analyzer-*, misc-*, hicpp-*, concurrency-*, -header-filter=.*, -performance-unnecessary-value-param, -performance-inefficient-algorithm, -hicpp-signed-bitwise, -nonportable-include-path, -optin.cplusplus.VirtualCall"
  Exception Code: 0xC0000005
  0x015693B0, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000DC0000) + 0x7A93B0 byte(s)
  0x77291C01, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x81C01 byte(s), wcstombs() + 0x1 byte(s)
C:\Program Files\Microsoft Visual Studio\18\Enterprise\MSBuild\Microsoft\VC\v160\Microsoft.Cpp.ClangTidy.targets(228,5): error MSB6006: "clang-tidy.exe" exited with code -1073741819.
========== Rebuild All: 0 succeeded, 2 failed, 0 skipped ==========

</details>


---

### Comment 2 - localspook

Can you narrow down the crash to a single category? E.g.:
```sh
clang-tidy --checks=-*,readability-* [...]
```
...and then to a single check?
```sh
clang-tidy --checks=-*,some-individual-check [...]
```

---

### Comment 3 - EugeneZelenko

Could you please provide reproducer, preferably on https://godbolt.org? You could also try version 21 in Visual Studio Code with C++ Microsoft extensions.

---

### Comment 4 - CarlT789

So the issue is specific to the 32 bit version of my build environment.  When I run the same checks in my 64 bit build, there is no crash. 
[I assume, but don’t know how to verify, that the 64 bit VS build is using a 64 bit clang-tidy.exe.  I found another post where a user reported crashes only with the 32 bit built.  I only see the path to the EXE when it crashes.  I tried running command line tests referencing the 32 bit exe, but I was having problems getting clang-tidy to find the scattered header files.]

It looks like I was able to narrow it down not to an option, but to the syntax. It only crashes when I have more than 1 parameter. If there are quotation marks around the “checks” parameter string, then there is no crash.  If there are no quotes, then the exe crashes in the 32 bit build.  I haven’t been putting the string in quotes , but have primarily been using 64 builds.  This is also the first time I’ve used VS 2026 for clang-tidy work.  
The same syntax that crashes for 32 bit VS2026 does not crash in 32 bit VS2019.

These crash in VS2026 32 bit, but not 64 bit builds:
```txt
--checks=bugprone-*, hicpp-*
--checks=bugprone-*,hicpp-*
```
These don’t crash in either 32 or 64 bit builds, for both VS2026 and VS2019:
```txt
--checks='bugprone-*,hicpp-*'
--checks='bugprone-*, hicpp-*'
```

Another oddity is that the 32 bit version does not create any output related to the code even when it does not crash, while the 64 bit version does.  Below are output examples from running both with the same set of options.

```
Build started at 11:06 AM...
1>------ Build started: Project: Software, Configuration: Debug Win32 ------
1>  YamlRegKey.cpp
  [1/2] Processing file C:\Repos\Software\code\Util\YamlRegKey.cpp.
  [2/2] Processing file C:\Repos\Software\code\StdAfx.cpp.
CLANGTIDY : warning : argument unused during compilation: '/c' [clang-diagnostic-unused-command-line-argument]
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build completed at 11:06 AM and took 07.799 seconds ==========
```

```
Build started at 11:07 AM...
1>------ Build started: Project: Software, Configuration: Debug x64 ------
1>  YamlRegKey.cpp
  [1/2] Processing file C:\Repos\Software\code\Util\YamlRegKey.cpp.
  [2/2] Processing file C:\Repos\Software\code\StdAfx.cpp.
CLANGTIDY : warning : argument unused during compilation: '/c' [clang-diagnostic-unused-command-line-argument]
C:\Repos\Software\code\StdAfx.cpp(12,1): warning GDF5DA4D1: included header StdAfx.h is not used directly [misc-include-cleaner]
     12 | #include "StdAfx.h"
        | ^~~~~~~~~~~~~~~~~~~
{....  Additional warnings are listed...}

========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build completed at 11:07 AM and took 06.932 seconds ==========
```

---

### Comment 5 - localspook

I'm not sure if this gets us closer to fixing the crash, but here's something I noticed. At the end of the clang-tidy invocation in your original post there is:
```
"--checks=--checks=bugprone-*, clang-analyzer-*, misc-*, hicpp-*, concurrency-*, -header-filter=.*, -performance-unnecessary-value-param, -performance-inefficient-algorithm, -hicpp-signed-bitwise, -nonportable-include-path, -optin.cplusplus.VirtualCall"
```
Which seems suspicious:
1. Notice the duplicated `--checks=--checks=`.
2. `-header-filter=.*` should be a separate option, not an argument to the `--checks` option.

I'm guessing (and tell me if I'm wrong) that you're entering these options in some sort of GUI, which is then constructing the actual command line for you. It seems the GUI expects you to enter a list of checks *without the `--checks=` prefix* (it prepends one by itself), and *doesn't* expect you to enter other options, like `-header-filter=.*`.

---

### Comment 6 - CarlT789

@localspook ,
You are correct - I am using the GUI in Visual Studio to enter the values and run the tool.  And I had missed that "--checks" was repeated.  The VS GUI must be adding that by default.
And I hadn't realized that -header-filter was a separate option.  Because of the way that VS prepends "--checks" to the whole string, there does not appear to be a way to use the -header-filter from the VS GUI.

I fixed both of those issues and reran the tests. It still succeeds for the 64 bit version, and crashes for the 32 bit version.   I then worked on isolating which option was at fault.  At different times I got full stack dumps or the the shorter 2 line dumps.   The "hicpp-use-nullptr" option is the one causing the problem.  It causes every CPP that I tested to crash with the 32 bit exe.  Some report the full stack trace, and others report only the short trace.

A new, full, dump stack is posted below using just the "hicpp-use-nullptr" option.

```
  Stack dump:
  0.	Program arguments: "C:\\Program Files\\Microsoft Visual Studio\\18\\Enterprise\\VC\\Tools\\Llvm\\bin\\clang-tidy.exe" C:\\Repos\\Software\\code\\DDCSClient\\Blob.cpp C:\\Repos\\Software\\code\\StdAfx.cpp "-p=..\\build\\Temp\\x86 Debug\\Software142.ClangTidy" --checks=hicpp-use-nullptr
  Exception Code: 0xC0000005
  0x016293B0, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x7A93B0 byte(s)
  0x77291C01, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x81C01 byte(s), wcstombs() + 0x1 byte(s)
  0x772A0B85, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x90B85 byte(s), RtlGetSystemTimeAndBias() + 0x5A5 byte(s)
  0x013F4A5C, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x574A5C byte(s)
  0x00ED16FC, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x516FC byte(s)
  0x772523D7, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x423D7 byte(s), RtlGetCurrentServiceSessionId() + 0x87 byte(s)
  0x772A0B26, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x90B26 byte(s), RtlGetSystemTimeAndBias() + 0x546 byte(s)
  0x772A0CB7, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x90CB7 byte(s), RtlGetSystemTimeAndBias() + 0x6D7 byte(s)
  0x00ED99CA, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x599CA byte(s)
  0x772A0B26, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x90B26 byte(s), RtlGetSystemTimeAndBias() + 0x546 byte(s)
  0x772A0CB7, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x90CB7 byte(s), RtlGetSystemTimeAndBias() + 0x6D7 byte(s)
  0x00EDF7EF, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x5F7EF byte(s)
  0x0104AF60, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1CAF60 byte(s)
  0x00E988CC, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x188CC byte(s)
  0x00EDF23D, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x5F23D byte(s)
  0x010442EA, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1C42EA byte(s)
  0x00ED9901, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x59901 byte(s)
  0x010441B7, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1C41B7 byte(s)
  0x010430B7, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1C30B7 byte(s)
  0x01046546, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1C6546 byte(s)
  0x00ED30B8, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x530B8 byte(s)
  0x00E852C8, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x52C8 byte(s)
  0x02D185FF, C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Tools\Llvm\bin\clang-tidy.exe(0x0000000000E80000) + 0x1E985FF byte(s)
  0x754E5D49, C:\WINDOWS\System32\KERNEL32.DLL(0x00000000754D0000) + 0x15D49 byte(s), BaseThreadInitThunk() + 0x19 byte(s)
  0x7727D6DB, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x6D6DB byte(s), RtlInitializeExceptionChain() + 0x6B byte(s)
  0x7727D661, C:\WINDOWS\SYSTEM32\ntdll.dll(0x0000000077210000) + 0x6D661 byte(s), RtlGetAppContainerNamedObjectPath() + 0x231 byte(s)
C:\Program Files\Microsoft Visual Studio\18\Enterprise\MSBuild\Microsoft\VC\v160\Microsoft.Cpp.ClangTidy.targets(228,5): error MSB6006: "clang-tidy.exe" exited with code -1073741819.
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
========== Build completed at 11:29 AM and took 00.994 seconds ==========
```

---

### Comment 7 - localspook

> using just the "hicpp-use-nullptr" option.

Then this seems like #53778 (`hicpp-use-nullptr` and `modernize-use-nullptr` are aliases for the same check). Fixed in the upcoming release.

> 

---

