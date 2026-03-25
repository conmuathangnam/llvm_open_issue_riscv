# clang-cl crash

**Author:** Teselka
**URL:** https://github.com/llvm/llvm-project/issues/169736
**Status:** Open
**Labels:** lld:COFF, crash

## Body

`clang-cl -v`
```
clang version 21.1.6
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\VisualStudio\VC\Tools\Llvm\x64\bin
```

```
1>  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
1>  Stack dump:
1>  0.	Program arguments: D:\\VisualStudio\\VC\\Tools\\Llvm\\x64\\bin\\lld-link.exe @D:\\TEMP\\MSBuildTemp\\tmpc1b0d3f1de7c4ebfa2304a8660d6345a.rsp
1>  Exception Code: 0xC0000005
1>   #0 0x00007ff6e4eb049a (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0xe5049a)
1>   #1 0x00007ff6e4eade8c (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0xe4de8c)
1>   #2 0x00007ff6e489247d (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x83247d)
1>   #3 0x00007ff6e489034d (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x83034d)
1>   #4 0x00007ff6e46e8720 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x688720)
1>   #5 0x00007ff6e46dbd5a (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x67bd5a)
1>   #6 0x00007ff6e46db2ef (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x67b2ef)
1>   #7 0x00007ff6e448bee0 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x42bee0)
1>   #8 0x00007ff6e448b4b3 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x42b4b3)
1>   #9 0x00007ff6e5af930c xmlLinkGetData (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x1a9930c)
1>  #10 0x00007ffeaf66e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
1>  #11 0x00007ffeb0c68d9c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8d9c)
```

Compiler options:
```
/EHs-c- /GR- /std:c++20 /permissive- -Wno-deprecated-declarations -msse4.2 /Zc:tlsGuards- /GS- /JMC- -Wno-unused-parameter -Wno-microsoft-enum-forward-reference -Wno-inconsistent-dllimport /clang:-fno-threadsafe-statics /clang:-fno-unwind-tables /clang:-fno-asynchronous-unwind-tables -Wno-pragma-pack -Wno-unused-local-typedef -Wno-unknown-pragmas /fp:except-
```

Linker options (exports.def) is an empty file:
```
/FORCE:MULTIPLE /MAP /ENTRY:main /DEF:exports.def 
```

Unfortunately i can't reproduce this in the empty project, i assume that's it's caused by the symbol override like this:
```
void* __cdecl malloc(size_t size) {
	return 0;
}

decltype(&malloc) __imp_malloc = &malloc;
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-coff

Author: None (Teselka)

<details>
`clang-cl -v`
```
clang version 21.1.6
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\VisualStudio\VC\Tools\Llvm\x64\bin
```

```
1&gt;  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
1&gt;  Stack dump:
1&gt;  0.	Program arguments: D:\\VisualStudio\\VC\\Tools\\Llvm\\x64\\bin\\lld-link.exe @<!-- -->D:\\TEMP\\MSBuildTemp\\tmpc1b0d3f1de7c4ebfa2304a8660d6345a.rsp
1&gt;  Exception Code: 0xC0000005
1&gt;   #<!-- -->0 0x00007ff6e4eb049a (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0xe5049a)
1&gt;   #<!-- -->1 0x00007ff6e4eade8c (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0xe4de8c)
1&gt;   #<!-- -->2 0x00007ff6e489247d (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x83247d)
1&gt;   #<!-- -->3 0x00007ff6e489034d (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x83034d)
1&gt;   #<!-- -->4 0x00007ff6e46e8720 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x688720)
1&gt;   #<!-- -->5 0x00007ff6e46dbd5a (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x67bd5a)
1&gt;   #<!-- -->6 0x00007ff6e46db2ef (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x67b2ef)
1&gt;   #<!-- -->7 0x00007ff6e448bee0 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x42bee0)
1&gt;   #<!-- -->8 0x00007ff6e448b4b3 (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x42b4b3)
1&gt;   #<!-- -->9 0x00007ff6e5af930c xmlLinkGetData (D:\VisualStudio\VC\Tools\Llvm\x64\bin\lld-link.exe+0x1a9930c)
1&gt;  #<!-- -->10 0x00007ffeaf66e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
1&gt;  #<!-- -->11 0x00007ffeb0c68d9c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8d9c)
```

Compiler options:
```
/EHs-c- /GR- /std:c++20 /permissive- -Wno-deprecated-declarations -msse4.2 /Zc:tlsGuards- /GS- /JMC- -Wno-unused-parameter -Wno-microsoft-enum-forward-reference -Wno-inconsistent-dllimport /clang:-fno-threadsafe-statics /clang:-fno-unwind-tables /clang:-fno-asynchronous-unwind-tables -Wno-pragma-pack -Wno-unused-local-typedef -Wno-unknown-pragmas /fp:except-
```

Linker options (exports.def) is an empty file:
```
/FORCE:MULTIPLE /MAP /ENTRY:main /DEF:exports.def 
```

Unfortunately i can't reproduce this in the empty project, i assume that's it's caused by the symbol override like this:
```
void* __cdecl malloc(size_t size) {
	return 0;
}

decltype(&amp;malloc) __imp_malloc = &amp;malloc;
```
</details>


---

### Comment 2 - Teselka

I can confirm that's it's caused by the `/DEBUG` lld-link switch even if object files were generated without `/Z7`,`/Zi` or `/ZI`.
EDIT: Also happens on `19.1.0`

---

