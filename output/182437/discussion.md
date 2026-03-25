# ICE on invalid code with clang 21.1.6 and Qt

**Author:** jcelerier
**URL:** https://github.com/llvm/llvm-project/issues/182437

## Body

Repro: (Qt 6, up-to-date archlinux)

```c++
#include <QTimer>
void f(QObject& proc)
{
  QTimer::singleShot(0, proc, [] { });
}
```

```
$ clang++ -c -std=c++23 foo.cpp -I /usr/include/qt6/ -I /usr/include/qt6/QtCore
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++23 foo.cpp -I /usr/include/qt6/ -I /usr/include/qt6/QtCore
1.	foo.cpp:4:37: current parser token ')'
2.	foo.cpp:3:1: parsing function body 'f'
3.	foo.cpp:3:1: in compound statement ('{}')
#0 0x00007f8a490d99ba llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.21.1+0x78d99ba)
#1 0x00007f8a490d974b llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/libLLVM.so.21.1+0x78d974b)
#2 0x00007f8a490947e0 (/usr/lib/libLLVM.so.21.1+0x78947e0)
#3 0x00007f8a490949fd (/usr/lib/libLLVM.so.21.1+0x78949fd)
#4 0x00007f8a41044900 (/usr/lib/libc.so.6+0x44900)
#5 0x00007f8a4a6c1028 (/usr/lib/libclang-cpp.so.21.1+0x8c1028)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/foo-727ec4.cpp
clang++: note: diagnostic msg: /tmp/foo-727ec4.sh
clang++: note: diagnostic msg:

********************
```

[foo-727ec4.sh](https://github.com/user-attachments/files/25433352/foo-727ec4.sh)
[foo-727ec4.cpp](https://github.com/user-attachments/files/25433353/foo-727ec4.cpp)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jean-Michaël Celerier (jcelerier)

<details>
Repro: (Qt 6, up-to-date archlinux)

```c++
#include &lt;QTimer&gt;
void f(QObject&amp; proc)
{
  QTimer::singleShot(0, proc, [] { });
}
```

```
$ clang++ -c -std=c++23 foo.cpp -I /usr/include/qt6/ -I /usr/include/qt6/QtCore
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -c -std=c++23 foo.cpp -I /usr/include/qt6/ -I /usr/include/qt6/QtCore
1.	foo.cpp:4:37: current parser token ')'
2.	foo.cpp:3:1: parsing function body 'f'
3.	foo.cpp:3:1: in compound statement ('{}')
#<!-- -->0 0x00007f8a490d99ba llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.21.1+0x78d99ba)
#<!-- -->1 0x00007f8a490d974b llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/libLLVM.so.21.1+0x78d974b)
#<!-- -->2 0x00007f8a490947e0 (/usr/lib/libLLVM.so.21.1+0x78947e0)
#<!-- -->3 0x00007f8a490949fd (/usr/lib/libLLVM.so.21.1+0x78949fd)
#<!-- -->4 0x00007f8a41044900 (/usr/lib/libc.so.6+0x44900)
#<!-- -->5 0x00007f8a4a6c1028 (/usr/lib/libclang-cpp.so.21.1+0x8c1028)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/foo-727ec4.cpp
clang++: note: diagnostic msg: /tmp/foo-727ec4.sh
clang++: note: diagnostic msg:

********************
```

[foo-727ec4.sh](https://github.com/user-attachments/files/25433352/foo-727ec4.sh)
[foo-727ec4.cpp](https://github.com/user-attachments/files/25433353/foo-727ec4.cpp)
</details>


---

### Comment 3 - chrchr-github

Reproduced with 21.1, but not with trunk:
https://godbolt.org/z/137aEb8q4

---

