# identifier `z` crashes LLVM

**Author:** bottle2
**URL:** https://github.com/llvm/llvm-project/issues/122793
**Status:** Closed
**Labels:** clang:frontend, crash, needs-reduction
**Closed Date:** 2025-09-20T04:52:05Z

## Body

check out this error report: https://github.com/emscripten-core/emscripten/issues/23383
maybe you have a regression? emcc says to be using clang version 20.0.0git, but I'm using clang 19.1.6 and no such problem exists.

```
$ clang -v
clang version 19.1.6
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bottle2)

<details>
check out this error report: https://github.com/emscripten-core/emscripten/issues/23383
maybe you have a regression? emcc says to be using clang version 20.0.0git, but I'm using clang 19.1.6 and no such problem exists.

```
$ clang -v
clang version 19.1.6
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
```
</details>


---

### Comment 2 - shafik

We are going to need some sort of reduction to make any progress, I don't see any similar bugs so far.

---

### Comment 3 - bottle2

is "reduction" finding which commit introduced the bug?

---

### Comment 4 - Endilll

We need a self-contained reproducer, which means it doesn't depend on any other files (think Compiler Explorer examples). That file is likely to be at least several megabytes in size, and reducing it to something that fits on the screen is the reduction Shafik meant.

---

### Comment 5 - bottle2

ah, okay.

this the first test case:

```c
int z;
```

this is the second test case:

```c
int z = 2;
```

this is the third test case:

```c
int z(int);
```

---

### Comment 6 - Endilll

Neither of them reproduce on their own: https://godbolt.org/z/83o6n1KGf
Additional information needed.

---

### Comment 7 - bottle2

I can no longer reproduce the problem

```
$ uname -a
MINGW64_NT-10.0-19045 DESKTOP-QIBT7ND 3.6.4-23a25d49.x86_64 2025-07-16 16:15 UTC x86_64 Msys
$ emcc -v
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 4.0.14-git (96371ed7888fc78c0
40179f4d4faa82a6a07a116)
clang version 22.0.0git
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: C:/msys64/clang64/opt/emscripten-llvm/bin
```

---

