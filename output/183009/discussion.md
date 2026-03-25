# [Clang] `clang-check` Aborted with Unchecked Error

**Author:** XGPSPL
**URL:** https://github.com/llvm/llvm-project/issues/183009
**Status:** Open
**Labels:** clang, crash

## Body

**Environment:**
- Clang-Tidy Version: LLVM version 23.0.0git (Optimized build with assertions)
- Commit Hash: [`9a109fbb6e184ec9bcce10615949f598f4c974a9`](https://github.com/llvm/llvm-project/commit/9a109fbb6e184ec9bcce10615949f598f4c974a9)
- Platform: Linux 6.18.6

**Reproducer:**
```shell
$ touch foo.cpp
$ ./build/bin/clang-check -serialize-diagnostic-file=foo.dia foo.cpp --
clang-check: Unknown command line argument '-serialize-diagnostic-file=foo.dia'.  Try: './build/bin/clang-check --help'
clang-check: Did you mean '--print-options=foo.dia'?
Program aborted due to an unhandled Error:
clang-check: Unknown command line argument '-serialize-diagnostic-file=foo.dia'.  Try: './build/bin/clang-check --help'
clang-check: Did you mean '--print-options=foo.dia'?

 #0 0x00007f12a38b7208 __interceptor_backtrace (/usr/lib/libasan.so.8+0xb7208)
 #1 0x000055cba4cd3311 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/bin/clang-check+0x80f0311)
 #2 0x000055cba4cc3eea llvm::sys::RunSignalHandlers() (./build/bin/clang-check+0x80e0eea)
 #3 0x000055cba4cc47a3 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007f12a324d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #5 0x00007f12a32a7a2c (/usr/lib/libc.so.6+0x98a2c)
 #6 0x00007f12a324d1a0 raise (/usr/lib/libc.so.6+0x3e1a0)
 #7 0x00007f12a32345fe abort (/usr/lib/libc.so.6+0x255fe)
 #8 0x000055cba4a81962 llvm::Error::fatalUncheckedError() const (./build/bin/clang-check+0x7e9e962)
 #9 0x000055cba3134cd6 main (./build/bin/clang-check+0x6551cd6)
#10 0x00007f12a32366c1 (/usr/lib/libc.so.6+0x276c1)
#11 0x00007f12a32367f9 __libc_start_main (/usr/lib/libc.so.6+0x277f9)
#12 0x000055cba3126025 _start (./build/bin/clang-check+0x6543025)
Aborted                    ./build/bin/clang-check -serialize-diagnostic-file=foo.dia foo.cpp --
```

Seems like a regression:
```
$ /usr/bin/clang-check -serialize-diagnostic-file=foo.dia foo.cpp --
clang-check: Unknown command line argument '-serialize-diagnostic-file=foo.dia'.  Try: '/usr/bin/clang-check --help'
clang-check: Did you mean '--analyzer-output-path=foo.dia'?
$ /usr/bin/clang-check --version
LLVM (http://llvm.org/):
  LLVM version 21.1.8
  Optimized build.
```

## Comments

### Comment 1 - zeyi2

(XGPSPL is my account for reporting issues in LLVM)

More info, this also affects `clang-refactor`, `clang-move`....:
```
$ ./build/bin/clang-refactor --nonsense
clang-refactor: Unknown command line argument '--nonsense'.  Try: './build/bin/clang-refactor --help'
clang-refactor: Did you mean '--rng-seed'?
Program aborted due to an unhandled Error:
clang-refactor: Unknown command line argument '--nonsense'.  Try: './build/bin/clang-refactor --help'
clang-refactor: Did you mean '--rng-seed'?

 #0 0x00007fcb25ab7208 __interceptor_backtrace (/usr/lib/libasan.so.8+0xb7208)
 #1 0x0000558586620f91 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/bin/clang-refactor+0x406df91)
 #2 0x0000558586611b6a llvm::sys::RunSignalHandlers() (./build/bin/clang-refactor+0x405eb6a)
 #3 0x0000558586612423 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007fcb2544d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #5 0x00007fcb254a7a2c (/usr/lib/libc.so.6+0x98a2c)
 #6 0x00007fcb2544d1a0 raise (/usr/lib/libc.so.6+0x3e1a0)
 #7 0x00007fcb254345fe abort (/usr/lib/libc.so.6+0x255fe)
 #8 0x0000558586395dd2 llvm::Error::fatalUncheckedError() const (./build/bin/clang-refactor+0x3de2dd2)
 #9 0x00005585862db1be main (./build/bin/clang-refactor+0x3d281be)
#10 0x00007fcb254366c1 (/usr/lib/libc.so.6+0x276c1)
#11 0x00007fcb254367f9 __libc_start_main (/usr/lib/libc.so.6+0x277f9)
#12 0x00005585862a0fe5 _start (./build/bin/clang-refactor+0x3cedfe5)
Aborted                    ./build/bin/clang-refactor --nonsense
```

~~(Possibly introduced in https://github.com/llvm/llvm-project/commit/d47ee525f9e9289815db0864b03e866fc8e5ba01)~~

I'm not sure whether Abort is the intended behaviour here (as we suggest this way of handling error in documentation, so maybe it's intentional? It would be nice if someone familiar with this part could provide more information :) ) https://github.com/llvm/llvm-project/blob/ea6fee062da1b03b12bb0bf8b180da8e39bcfd51/clang/docs/LibTooling.rst?plain=1#L78 

If this is indeed a bug that needs fixing, I'll try to fix it :)

---

