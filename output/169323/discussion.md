# lldb crash in Win 11 Arm

**Author:** holleB
**URL:** https://github.com/llvm/llvm-project/issues/169323
**Status:** Open
**Labels:** lldb, platform:windows, crash, Swift

## Body

Running Win 11 Arm under Parallels Desktkop on a Macbook M3. Installed swift as per documentation. Starting lldb results in a crash:

```
PS C:\Users\myuser\projects\myproject> lldb
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: C:\\Users\\myuser\\AppData\\Local\\Programs\\Swift\\Toolchains\\6.2.1+Asserts\\usr\\bin\\lldb.exe
Exception Code: 0xC06D007E
#0 0x00007ffe77b9db04 (C:\Windows\System32\KERNELBASE.dll+0x6db04)
#1 0x00007ff682a62984 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x32984)
#2 0x00007ff682a61d84 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x31d84)
#3 0x00007ff682a38f68 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x8f68)
#4 0x00007ff682a636b4 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x336b4)
#5 0x00007ff682a6374c (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x3374c)
#6 0x00007ffe7bd222d0 (C:\Windows\System32\KERNEL32.DLL+0x122d0)
#7 0x00007ffe7c9f5b9c (C:\Windows\SYSTEM32\ntdll.dll+0x75b9c)
PS C:\Users\myuser\projects\myproject>
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: None (holleB)

<details>
Running Win 11 Arm under Parallels Desktkop on a Macbook M3. Installed swift as per documentation. Starting lldb results in a crash:

PS C:\Users\myuser\projects\myproject&gt; lldb
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: C:\\Users\\myuser\\AppData\\Local\\Programs\\Swift\\Toolchains\\6.2.1+Asserts\\usr\\bin\\lldb.exe
Exception Code: 0xC06D007E
#<!-- -->0 0x00007ffe77b9db04 (C:\Windows\System32\KERNELBASE.dll+0x6db04)
#<!-- -->1 0x00007ff682a62984 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x32984)
#<!-- -->2 0x00007ff682a61d84 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x31d84)
#<!-- -->3 0x00007ff682a38f68 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x8f68)
#<!-- -->4 0x00007ff682a636b4 (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x336b4)
#<!-- -->5 0x00007ff682a6374c (C:\Users\myuser\AppData\Local\Programs\Swift\Toolchains\6.2.1+Asserts\usr\bin\lldb.exe+0x3374c)
#<!-- -->6 0x00007ffe7bd222d0 (C:\Windows\System32\KERNEL32.DLL+0x122d0)
#<!-- -->7 0x00007ffe7c9f5b9c (C:\Windows\SYSTEM32\ntdll.dll+0x75b9c)
PS C:\Users\myuser\projects\myproject&gt;
</details>


---

### Comment 2 - DavidSpickett

Can you confirm that https://www.swift.org/install/windows/ is the documentation you refer to?

This will be built from Apple's Swift fork, I think @charles-zablit would be able find out it's specific to that version.

---

### Comment 3 - charles-zablit

This looks like the same crash as https://github.com/swiftlang/llvm-project/issues/11779.

The TL;DR to fix it is to install `Python 3.10` and make sure it's in your PATH.

---

