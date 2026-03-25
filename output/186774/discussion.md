# heap corruption bug

**Author:** Z-TEAM-OFFICIAL
**URL:** https://github.com/llvm/llvm-project/issues/186774
**Status:** Open
**Labels:** lld, incomplete, crash

## Body

Stack dump:
```
0.      Program arguments: C:\\msys64\\mingw64\\bin\\ld.lld.exe @C:\\ZOS\\build-x86\\link.rsp
Exception Code: 0xC0000005
0x00007FFD48082816, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FFD48060000) + 0x22816 byte(s), RtlAllocateHeap() + 0x65C6 byte(s)
0x00007FFD480D9250, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FFD48060000) + 0x79250 byte(s), RtlFreeHeap() + 0x620 byte(s)
0x00007FFD45EBD60C, C:\WINDOWS\System32\msvcrt.dll(0x00007FFD45EA0000) + 0x1D60C byte(s), free() + 0x1C byte(s)
0x00007FFC6B89C489, C:\msys64\mingw64\bin\libLLVM-21.dll(0x00007FFC69DB0000) + 0x1AEC489 byte(s), _ZN4llvm13llvm_shutdownEv() + 0x39 byte(s)
0x00007FF7369CC56F, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0x2BC56F byte(s)
0x00007FF7367DD939, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0xCD939 byte(s)
0x00007FF73671B9EC, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0xB9EC byte(s)
0x00007FF736B90A3F, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0x480A3F byte(s)
0x00007FF7367110D9, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0x10D9 byte(s)
0x00007FF736711456, C:\msys64\mingw64\bin\ld.lld.exe(0x00007FF736710000) + 0x1456 byte(s)
0x00007FFD4697E8D7, C:\WINDOWS\System32\KERNEL32.DLL(0x00007FFD46950000) + 0x2E8D7 byte(s), BaseThreadInitThunk() + 0x17 byte(s)
0x00007FFD480EC48C, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FFD48060000) + 0x8C48C byte(s), RtlUserThreadStart() + 0x2C byte(s)
```

## Comments

### Comment 1 - DimitryAndric

This report is not very useful without a reproduction case. Can you please supply one?


---

