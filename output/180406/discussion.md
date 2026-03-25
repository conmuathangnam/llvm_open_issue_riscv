# lld-link crashes with /FILEALIGN:1 since 21.1.8 (only on Windows)

**Author:** wbenny
**URL:** https://github.com/llvm/llvm-project/issues/180406
**Status:** Closed
**Labels:** lld:COFF, crash
**Closed Date:** 2026-02-10T13:26:05Z

## Body

**Environment:**
Only Windows. Works fine when cross-compiling on macOS. Linux untested, but I would expect it works there.

When I tested with clang 20.1.8 it worked - even on Windows.
Clang 21.1.8 is already broken, 22.1.0-rc2 and current master is broken too.

**Steps to Reproduce:**
```bash
git clone git@github.com:wbenny/scfw.git
cmake -B build-x64 --preset x64 # x86 fails too
cmake --build build-x64 --target writeconsole # but other targets fail too
```

Note that when compiling _without_ specifiying /FILEALIGN, it works:
```bash
cmake -B build-x64-fa0 --preset x64 -DSCFW_FILE_ALIGNMENT=0
cmake --build build-x64-fa0 --target writeconsole # x86 works too!
```

**Expected:**
lld-link is able to produce an exe with /FILEALIGN:1

**Actual:**
```
> cmake --build build-x64 --target writeconsole
[1/1] Linking CXX executable examples\writeconsole\writeconsole.exe
FAILED: examples/writeconsole/writeconsole.exe
C:\WINDOWS\system32\cmd.exe /C "cd . && E:\Sources\llvm-project\build\bin\clang++.exe  -flto -Wl,/FILEALIGN:1 --target=x86_64-pc-windows-msvc -nostdlib -nostartfiles -fuse-ld=lld -Wl,/OPT:REF -Wl,/OPT:ICF -Wl,/ENTRY:_init -Wl,/SUBSYSTEM:CONSOLE -Wl,/NODEFAULTLIB -Wl,/MERGE:.data=.text -Wl,/MERGE:.rdata=.text -Xlinker /SECTION:.text,RWE examples/writeconsole/CMakeFiles/writeconsole.dir/main.cpp.obj -o examples\writeconsole\writeconsole.exe  lib/libscfw.a && C:\WINDOWS\system32\cmd.exe /C "cd /D D:\Work\Sources\scfw\build-x64\examples\writeconsole && D:\Work\Tools\cmake\bin\cmake.exe -DLLVM_READOBJ=E:/Work/llvm-project/build/bin/llvm-readobj.exe -DPE_FILE=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.exe -P D:/Work/Sources/scfw/cmake/post-build/verify_pe.cmake && E:\Sources\llvm-project\build\bin\llvm-objcopy.exe --dump-section=.text=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.bin D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.exe && D:\Work\Tools\cmake\bin\cmake.exe -DBIN_FILE=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.bin -P D:/Work/Sources/scfw/cmake/post-build/print_size.cmake""
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: E:\\Sources\\llvm-project\\build\\bin\\lld-link -out:examples\\writeconsole\\writeconsole.exe "-libpath:C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Tools\\MSVC\\14.40.33807\\lib\\x64" "-libpath:C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Tools\\MSVC\\14.40.33807\\atlmfc\\lib\\x64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\ucrt\\x64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\um\\x64" -nologo /FILEALIGN:1 /OPT:REF /OPT:ICF /ENTRY:_init /SUBSYSTEM:CONSOLE /NODEFAULTLIB /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,RWE examples/writeconsole/CMakeFiles/writeconsole.dir/main.cpp.obj lib/libscfw.a
Exception Code: 0xC0000005
 #0 0x00007ffa39fe02a9 (C:\WINDOWS\SYSTEM32\VCRUNTIME140D.dll+0x202a9)
 #1 0x00007ff7a07ac30e `anonymous namespace'::Writer::writeSections E:\Sources\llvm-project\lld\COFF\Writer.cpp:2630:0
 #2 0x00007ff7a07a32ed `anonymous namespace'::Writer::run E:\Sources\llvm-project\lld\COFF\Writer.cpp:810:0
 #3 0x00007ff7a07a2ed4 lld::coff::writeResult(class lld::coff::COFFLinkerContext &) E:\Sources\llvm-project\lld\COFF\Writer.cpp:364:0
 #4 0x00007ff7a0529e95 lld::coff::LinkerDriver::linkerMain(class llvm::ArrayRef<char const *>) E:\Sources\llvm-project\lld\COFF\Driver.cpp:2934:0
 #5 0x00007ff7a0520bab lld::coff::link(class llvm::ArrayRef<char const *>, class llvm::raw_ostream &, class llvm::raw_ostream &, bool, bool) E:\Sources\llvm-project\lld\COFF\Driver.cpp:98:0
 #6 0x00007ff7a051e9f6 lld::unsafeLldMain(class llvm::ArrayRef<char const *>, class llvm::raw_ostream &, class llvm::raw_ostream &, class llvm::ArrayRef<struct lld::DriverDef>, bool) E:\Sources\llvm-project\lld\Common\DriverDispatcher.cpp:163:0
 #7 0x00007ff7a04b11d2 lld_main(int, char **, struct llvm::ToolContext const &) E:\Sources\llvm-project\lld\tools\lld\lld.cpp:89:0
 #8 0x00007ff7a04b4928 main E:\Sources\llvm-project\build\tools\lld\tools\lld\lld-driver.cpp:17:0
 #9 0x00007ff7a7d1b7a9 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
#10 0x00007ff7a7d1b8e2 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#11 0x00007ff7a7d1b96e __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
#12 0x00007ff7a7d1b98e mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
#13 0x00007ffa6a89e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#14 0x00007ffa6c94c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: linker command failed due to signal (use -v to see invocation)
ninja: build stopped: subcommand failed.
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-coff

Author: Petr Beneš (wbenny)

<details>
**Environment:**
Only Windows. Works fine when cross-compiling on macOS. Linux untested, but I would expect it works there.

When I tested with clang 20.1.8 it worked - even on Windows.
Clang 21.1.8 is already broken, 22.1.0-rc2 and current master is broken too.

**Steps to Reproduce:**
```bash
git clone git@<!-- -->github.com:wbenny/scfw.git
cmake -B build-x64 --preset x64 # x86 fails too
cmake --build build-x64 --target writeconsole # but other targets fail too
```

Note that when compiling _without_ specifiying /FILEALIGN, it works:
```bash
cmake -B build-x64-fa0 --preset x64 -DSCFW_FILE_ALIGNMENT=0
cmake --build build-x64-fa0 --target writeconsole # x86 works too!
```

**Expected:**
lld-link is able to produce an exe with /FILEALIGN:1

**Actual:**
```
&gt; cmake --build build-x64 --target writeconsole
[1/1] Linking CXX executable examples\writeconsole\writeconsole.exe
FAILED: examples/writeconsole/writeconsole.exe
C:\WINDOWS\system32\cmd.exe /C "cd . &amp;&amp; E:\Sources\llvm-project\build\bin\clang++.exe  -flto -Wl,/FILEALIGN:1 --target=x86_64-pc-windows-msvc -nostdlib -nostartfiles -fuse-ld=lld -Wl,/OPT:REF -Wl,/OPT:ICF -Wl,/ENTRY:_init -Wl,/SUBSYSTEM:CONSOLE -Wl,/NODEFAULTLIB -Wl,/MERGE:.data=.text -Wl,/MERGE:.rdata=.text -Xlinker /SECTION:.text,RWE examples/writeconsole/CMakeFiles/writeconsole.dir/main.cpp.obj -o examples\writeconsole\writeconsole.exe  lib/libscfw.a &amp;&amp; C:\WINDOWS\system32\cmd.exe /C "cd /D D:\Work\Sources\scfw\build-x64\examples\writeconsole &amp;&amp; D:\Work\Tools\cmake\bin\cmake.exe -DLLVM_READOBJ=E:/Work/llvm-project/build/bin/llvm-readobj.exe -DPE_FILE=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.exe -P D:/Work/Sources/scfw/cmake/post-build/verify_pe.cmake &amp;&amp; E:\Sources\llvm-project\build\bin\llvm-objcopy.exe --dump-section=.text=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.bin D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.exe &amp;&amp; D:\Work\Tools\cmake\bin\cmake.exe -DBIN_FILE=D:/Work/Sources/scfw/build-x64/examples/writeconsole/writeconsole.bin -P D:/Work/Sources/scfw/cmake/post-build/print_size.cmake""
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: E:\\Sources\\llvm-project\\build\\bin\\lld-link -out:examples\\writeconsole\\writeconsole.exe "-libpath:C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Tools\\MSVC\\14.40.33807\\lib\\x64" "-libpath:C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Tools\\MSVC\\14.40.33807\\atlmfc\\lib\\x64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\ucrt\\x64" "-libpath:C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.26100.0\\um\\x64" -nologo /FILEALIGN:1 /OPT:REF /OPT:ICF /ENTRY:_init /SUBSYSTEM:CONSOLE /NODEFAULTLIB /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,RWE examples/writeconsole/CMakeFiles/writeconsole.dir/main.cpp.obj lib/libscfw.a
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ffa39fe02a9 (C:\WINDOWS\SYSTEM32\VCRUNTIME140D.dll+0x202a9)
 #<!-- -->1 0x00007ff7a07ac30e `anonymous namespace'::Writer::writeSections E:\Sources\llvm-project\lld\COFF\Writer.cpp:2630:0
 #<!-- -->2 0x00007ff7a07a32ed `anonymous namespace'::Writer::run E:\Sources\llvm-project\lld\COFF\Writer.cpp:810:0
 #<!-- -->3 0x00007ff7a07a2ed4 lld::coff::writeResult(class lld::coff::COFFLinkerContext &amp;) E:\Sources\llvm-project\lld\COFF\Writer.cpp:364:0
 #<!-- -->4 0x00007ff7a0529e95 lld::coff::LinkerDriver::linkerMain(class llvm::ArrayRef&lt;char const *&gt;) E:\Sources\llvm-project\lld\COFF\Driver.cpp:2934:0
 #<!-- -->5 0x00007ff7a0520bab lld::coff::link(class llvm::ArrayRef&lt;char const *&gt;, class llvm::raw_ostream &amp;, class llvm::raw_ostream &amp;, bool, bool) E:\Sources\llvm-project\lld\COFF\Driver.cpp:98:0
 #<!-- -->6 0x00007ff7a051e9f6 lld::unsafeLldMain(class llvm::ArrayRef&lt;char const *&gt;, class llvm::raw_ostream &amp;, class llvm::raw_ostream &amp;, class llvm::ArrayRef&lt;struct lld::DriverDef&gt;, bool) E:\Sources\llvm-project\lld\Common\DriverDispatcher.cpp:163:0
 #<!-- -->7 0x00007ff7a04b11d2 lld_main(int, char **, struct llvm::ToolContext const &amp;) E:\Sources\llvm-project\lld\tools\lld\lld.cpp:89:0
 #<!-- -->8 0x00007ff7a04b4928 main E:\Sources\llvm-project\build\tools\lld\tools\lld\lld-driver.cpp:17:0
 #<!-- -->9 0x00007ff7a7d1b7a9 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
#<!-- -->10 0x00007ff7a7d1b8e2 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#<!-- -->11 0x00007ff7a7d1b96e __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
#<!-- -->12 0x00007ff7a7d1b98e mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
#<!-- -->13 0x00007ffa6a89e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->14 0x00007ffa6c94c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: linker command failed due to signal (use -v to see invocation)
ninja: build stopped: subcommand failed.
```

</details>


---

