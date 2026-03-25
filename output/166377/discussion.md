# Windows clang++ crash when using -fprofile-generate-cold-function-coverage

**Author:** cosminpolifronie
**URL:** https://github.com/llvm/llvm-project/issues/166377
**Status:** Open
**Labels:** coverage, crash

## Body

LLVM version: 21.1.4
System: Windows 11 23H2 22631.6060
MSVC version: 19.44.35219 (v17.14.18)
CMake version: 4.1.1
Ninja version: 1.13.1

Errors:
```
[1/4] Building CXX object CMakeFiles/testproj.dir/funcs.cpp.obj
FAILED: [code=1] CMakeFiles/testproj.dir/funcs.cpp.obj
C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe   -O2 -DNDEBUG -g -Xclang -gcodeview -std=gnu++14 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage -MD -MT CMakeFiles/testproj.dir/funcs.cpp.obj -MF CMakeFiles\testproj.dir\funcs.cpp.obj.d -o CMakeFiles/testproj.dir/funcs.cpp.obj -c C:/Users/user/Desktop/test_proj/funcs.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\tools\\clang+llvm-21.1.4-x86_64-pc-windows-msvc\\bin\\clang++.exe -O2 -DNDEBUG -g -Xclang -gcodeview -std=gnu++14 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage -MD -MT CMakeFiles/testproj.dir/funcs.cpp.obj -MF CMakeFiles\\testproj.dir\\funcs.cpp.obj.d -o CMakeFiles/testproj.dir/funcs.cpp.obj -c C:/Users/user/Desktop/test_proj/funcs.cpp
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
Exception Code: 0xC0000005
 #0 0x00007ff68eead6cc (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x304d6cc)
 #1 0x00007ff68c6c4602 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x864602)
 #2 0x00007ff68c6c37fd (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x8637fd)
 #3 0x00007ff68d83111d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x19d111d)
 #4 0x00007ff68dd84f05 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x1f24f05)
 #5 0x00007ff68c4892c3 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x6292c3)
 #6 0x00007ff68c488f0d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x628f0d)
 #7 0x00007ff68c485bc6 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x625bc6)
 #8 0x00007ff68c482728 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x622728)
 #9 0x00007ff68c480fbe (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x620fbe)
#10 0x00007ff68cd6e48d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0xf0e48d)
#11 0x00007ff68c611c25 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x7b1c25)
#12 0x00007ff68c611a18 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x7b1a18)
#13 0x00007ff68c335315 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d5315)
#14 0x00007ff68c334c5e (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d4c5e)
#15 0x00007ff68c33483d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d483d)
#16 0x00007ff68c32b576 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4cb576)
#17 0x00007ff68c328b13 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4c8b13)
#18 0x00007ff68de4c6d0 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x1fec6d0)
#19 0x00007ffb5b67259d (C:\WINDOWS\System32\KERNEL32.DLL+0x1259d)
#20 0x00007ffb5ceaaf78 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x5af78)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.4
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\user\AppData\Local\Temp\funcs-cb4411.cpp
clang++: note: diagnostic msg: C:\Users\user\AppData\Local\Temp\funcs-cb4411.sh
clang++: note: diagnostic msg:

********************
[2/4] Building CXX object CMakeFiles/testproj.dir/main.cpp.obj
FAILED: [code=1] CMakeFiles/testproj.dir/main.cpp.obj
C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe   -O2 -DNDEBUG -g -Xclang -gcodeview -std=gnu++14 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage -MD -MT CMakeFiles/testproj.dir/main.cpp.obj -MF CMakeFiles\testproj.dir\main.cpp.obj.d -o CMakeFiles/testproj.dir/main.cpp.obj -c C:/Users/user/Desktop/test_proj/main.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\tools\\clang+llvm-21.1.4-x86_64-pc-windows-msvc\\bin\\clang++.exe -O2 -DNDEBUG -g -Xclang -gcodeview -std=gnu++14 -D_DLL -D_MT -Xclang --dependent-lib=msvcrt -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage -MD -MT CMakeFiles/testproj.dir/main.cpp.obj -MF CMakeFiles\\testproj.dir\\main.cpp.obj.d -o CMakeFiles/testproj.dir/main.cpp.obj -c C:/Users/user/Desktop/test_proj/main.cpp
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
Exception Code: 0xC0000005
 #0 0x00007ff68eead6cc (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x304d6cc)
 #1 0x00007ff68c6c4602 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x864602)
 #2 0x00007ff68c6c37fd (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x8637fd)
 #3 0x00007ff68d83111d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x19d111d)
 #4 0x00007ff68dd84f05 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x1f24f05)
 #5 0x00007ff68c4892c3 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x6292c3)
 #6 0x00007ff68c488f0d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x628f0d)
 #7 0x00007ff68c485bc6 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x625bc6)
 #8 0x00007ff68c482728 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x622728)
 #9 0x00007ff68c480fbe (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x620fbe)
#10 0x00007ff68cd6e48d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0xf0e48d)
#11 0x00007ff68c611c25 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x7b1c25)
#12 0x00007ff68c611a18 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x7b1a18)
#13 0x00007ff68c335315 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d5315)
#14 0x00007ff68c334c5e (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d4c5e)
#15 0x00007ff68c33483d (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4d483d)
#16 0x00007ff68c32b576 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4cb576)
#17 0x00007ff68c328b13 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x4c8b13)
#18 0x00007ff68de4c6d0 (C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin\clang++.exe+0x1fec6d0)
#19 0x00007ffb5b67259d (C:\WINDOWS\System32\KERNEL32.DLL+0x1259d)
#20 0x00007ffb5ceaaf78 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x5af78)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.4
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\tools\clang+llvm-21.1.4-x86_64-pc-windows-msvc\bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\user\AppData\Local\Temp\main-4d4711.cpp
clang++: note: diagnostic msg: C:\Users\user\AppData\Local\Temp\main-4d4711.sh
clang++: note: diagnostic msg:

********************
ninja: build stopped: subcommand failed.
```
Sample project:
CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.18)
project(testproj LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

add_executable(testproj main.cpp funcs.cpp)
target_compile_options(testproj PRIVATE -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage)
target_link_options(testproj PRIVATE -fprofile-instr-generate -fcoverage-mapping -fcoverage-mcdc -fprofile-generate-cold-function-coverage)
```

main.cpp:
```c++
#include "funcs.h"

int main()
{
    return a();
}
```

funcs.h:
```c++
#ifndef FUNCS_H
#define FUNCS_H

int a();
int b();

#endif // FUNCS_H
```

funcs.cpp:
```c++
#include "funcs.h"

int a()
{
    return 3;
}

int b()
{
    return 4;
}
```

Attached generated LLVM files.

[funcs-cb4411.sh](https://github.com/user-attachments/files/23338106/funcs-cb4411.sh)
[main-4d4711.sh](https://github.com/user-attachments/files/23338107/main-4d4711.sh)

## Comments

### Comment 1 - cosminpolifronie

Crash also happening on Linux.
clang version 22.1.1 is also affected. 

---

