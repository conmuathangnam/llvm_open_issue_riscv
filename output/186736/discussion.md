# [lldb failure] Crashed using lldb debugging a HelloWorld C++ program.

**Author:** BravoMando
**URL:** https://github.com/llvm/llvm-project/issues/186736
**Status:** Closed
**Labels:** lldb, crash
**Closed Date:** 2026-03-17T10:55:56Z

## Body

I was using clang++ with libc++ to compile a HelloWorld C++ program, it succeeded and ran OK in command line. But when I used lldb to debug, it failed and I have no idea what's going on. I was using WSL2 ArchLinux by the way.

```bash
[shell]$ cat ../main.cpp 
```

```c++
#include <iostream>

int main(int argc, const char *argv[])
{

    std::cout << "Hello\n";

    return 0;
}
```

```
[shell]$ clang++ -g -std=c++23 -stdlib=libc++ -O0 ../main.cpp -o main
[shell]$ ./main 
Hello
[shell]$ lldb main
(lldb) target create "main"
Current executable set to '/home/user/toy/build/main' (x86_64).
(lldb) r
Process 85205 launched: '/home/user/toy/build/main' (x86_64)
LLVM ERROR: CachedFileStream was not committed.

PLEASE submit a bug report to  and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.22.1 0x00007ff472b56aed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 61
1  libLLVM.so.22.1 0x00007ff472b53bc7
2  libc.so.6       0x00007ff471c4d2d0
3  libc.so.6       0x00007ff471ca7a2c
4  libc.so.6       0x00007ff471c4d1a0 gsignal + 32
5  libc.so.6       0x00007ff471c345fe abort + 38
6  libLLVM.so.22.1 0x00007ff47276464c
7  libLLVM.so.22.1 0x00007ff472a394be
8  libLLVM.so.22.1 0x00007ff4729fbb02
9  liblldb.so.22.1 0x00007ff47d132a0f
10 liblldb.so.22.1 0x00007ff47ce5d63d
11 liblldb.so.22.1 0x00007ff47ce5ddf5
12 liblldb.so.22.1 0x00007ff47c7e6c8e
13 liblldb.so.22.1 0x00007ff47ce6094b
14 liblldb.so.22.1 0x00007ff47c7d35fa
15 liblldb.so.22.1 0x00007ff47c7c6b09
16 liblldb.so.22.1 0x00007ff47c7c16bc
17 liblldb.so.22.1 0x00007ff47c7c1a66
18 libc.so.6       0x00007ff471caada4
19 libc.so.6       0x00007ff471caae19 pthread_once + 25
20 liblldb.so.22.1 0x00007ff47c7c1f9f
21 liblldb.so.22.1 0x00007ff47c7c1c94
22 libLLVM.so.22.1 0x00007ff472ac1e41 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 609
23 liblldb.so.22.1 0x00007ff47ce45115
24 libc.so.6       0x00007ff471caada4
25 libc.so.6       0x00007ff471caae19 pthread_once + 25
26 liblldb.so.22.1 0x00007ff47cdb754a
27 liblldb.so.22.1 0x00007ff47cdb7f43
28 liblldb.so.22.1 0x00007ff47c7c6b60
29 liblldb.so.22.1 0x00007ff47c7c16bc
30 liblldb.so.22.1 0x00007ff47c7c1a66
31 libc.so.6       0x00007ff471caada4
32 libc.so.6       0x00007ff471caae19 pthread_once + 25
33 liblldb.so.22.1 0x00007ff47c7c1f9f
34 liblldb.so.22.1 0x00007ff47c7c1c94
35 libLLVM.so.22.1 0x00007ff472ac1e41 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 609
36 libLLVM.so.22.1 0x00007ff472ac2c67
37 libc.so.6       0x00007ff471ca597a
38 libc.so.6       0x00007ff471d292bc
LLDB diagnostics will be written to /tmp/diagnostics-494df3
Please include the directory content when filing a bug report
Aborted                    (core dumped) lldb main
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lldb

Author: YongSong (BravoMando)

<details>
I'm using clang++ with libc++ to compile a HelloWorld C++ program, it succeeded and ran OK in command line. But when I use lldb to debug, it failed and I have no idea what's going on. I'm using WSL2 ArchLinux by the way.

```bash
[shell]$ cat ../main.cpp 
#include &lt;iostream&gt;

int main(int argc, const char *argv[])
{

    std::cout &lt;&lt; "Hello\n";

    return 0;
}
[shell]$ clang++ -g -std=c++23 -stdlib=libc++ -O0 ../main.cpp -o main
[shell]$ ./main 
Hello
[shell]$ lldb main
(lldb) target create "main"
Current executable set to '/home/user/toy/build/main' (x86_64).
(lldb) r
Process 85205 launched: '/home/user/toy/build/main' (x86_64)
LLVM ERROR: CachedFileStream was not committed.

PLEASE submit a bug report to  and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.22.1 0x00007ff472b56aed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 61
1  libLLVM.so.22.1 0x00007ff472b53bc7
2  libc.so.6       0x00007ff471c4d2d0
3  libc.so.6       0x00007ff471ca7a2c
4  libc.so.6       0x00007ff471c4d1a0 gsignal + 32
5  libc.so.6       0x00007ff471c345fe abort + 38
6  libLLVM.so.22.1 0x00007ff47276464c
7  libLLVM.so.22.1 0x00007ff472a394be
8  libLLVM.so.22.1 0x00007ff4729fbb02
9  liblldb.so.22.1 0x00007ff47d132a0f
10 liblldb.so.22.1 0x00007ff47ce5d63d
11 liblldb.so.22.1 0x00007ff47ce5ddf5
12 liblldb.so.22.1 0x00007ff47c7e6c8e
13 liblldb.so.22.1 0x00007ff47ce6094b
14 liblldb.so.22.1 0x00007ff47c7d35fa
15 liblldb.so.22.1 0x00007ff47c7c6b09
16 liblldb.so.22.1 0x00007ff47c7c16bc
17 liblldb.so.22.1 0x00007ff47c7c1a66
18 libc.so.6       0x00007ff471caada4
19 libc.so.6       0x00007ff471caae19 pthread_once + 25
20 liblldb.so.22.1 0x00007ff47c7c1f9f
21 liblldb.so.22.1 0x00007ff47c7c1c94
22 libLLVM.so.22.1 0x00007ff472ac1e41 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 609
23 liblldb.so.22.1 0x00007ff47ce45115
24 libc.so.6       0x00007ff471caada4
25 libc.so.6       0x00007ff471caae19 pthread_once + 25
26 liblldb.so.22.1 0x00007ff47cdb754a
27 liblldb.so.22.1 0x00007ff47cdb7f43
28 liblldb.so.22.1 0x00007ff47c7c6b60
29 liblldb.so.22.1 0x00007ff47c7c16bc
30 liblldb.so.22.1 0x00007ff47c7c1a66
31 libc.so.6       0x00007ff471caada4
32 libc.so.6       0x00007ff471caae19 pthread_once + 25
33 liblldb.so.22.1 0x00007ff47c7c1f9f
34 liblldb.so.22.1 0x00007ff47c7c1c94
35 libLLVM.so.22.1 0x00007ff472ac1e41 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 609
36 libLLVM.so.22.1 0x00007ff472ac2c67
37 libc.so.6       0x00007ff471ca597a
38 libc.so.6       0x00007ff471d292bc
LLDB diagnostics will be written to /tmp/diagnostics-494df3
Please include the directory content when filing a bug report
Aborted                    (core dumped) lldb main
```
</details>


---

### Comment 2 - DhruvSrivastavaX

https://github.com/llvm/llvm-project/issues/184728 might be related

---

### Comment 3 - BravoMando

> [#184728](https://github.com/llvm/llvm-project/issues/184728) might be related

It worked following the instructions! Thanks a lot!

---

