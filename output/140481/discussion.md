# wrong code at -O{s,2,3} on x86_64-linux-gnu

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/140481
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-05-22T03:31:05Z

## Body

Compiler Explorer: https://godbolt.org/z/KGW6a53xf

It appears to be a regression from 16.0.0, and affects 17.0.1 and later. 

```
[590] % clangtk -v
clang version 21.0.0git (https://github.com/llvm/llvm-project.git fb86b3d96b73f4e628288b180ef4e038da8b7bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.1
[591] % 
[591] % clangtk -O1 small.c; ./a.out
[592] % 
[592] % clangtk -O3 small.c
[593] % ./a.out
Aborted
[594] % 
[594] % cat small.c
int a = 1, b, c;
int main() {
  b = -5001001 * a + 5001000;
  while (b >= 5001001)
    b = a + 5001000;
  c = -5001000 * b - 5001001;
  if (5001000 * c >= b)
    __builtin_abort();
  return 0;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Zhendong Su (zhendongsu)

<details>
Compiler Explorer: https://godbolt.org/z/KGW6a53xf

It appears to be a regression from 16.0.0, and affects 17.0.1 and later. 

```
[590] % clangtk -v
clang version 21.0.0git (https://github.com/llvm/llvm-project.git fb86b3d96b73f4e628288b180ef4e038da8b7bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /usr/local/cuda, version 12.1
[591] % 
[591] % clangtk -O1 small.c; ./a.out
[592] % 
[592] % clangtk -O3 small.c
[593] % ./a.out
Aborted
[594] % 
[594] % cat small.c
int a = 1, b, c;
int main() {
  b = -5001001 * a + 5001000;
  while (b &gt;= 5001001)
    b = a + 5001000;
  c = -5001000 * b - 5001001;
  if (5001000 * c &gt;= b)
    __builtin_abort();
  return 0;
}
```
</details>


---

### Comment 2 - hstk30-hw

https://godbolt.org/z/noce5aa3Y CC @nikic take a look

---

### Comment 3 - dtcxzyw

Reproducer: https://alive2.llvm.org/ce/z/JVTQCY


---

### Comment 4 - dtcxzyw

Further reduced version: https://alive2.llvm.org/ce/z/3_XCwc

---

