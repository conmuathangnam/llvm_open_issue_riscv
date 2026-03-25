# wrong code at -Os with "-mllvm -force-vector-width=4 -mllvm -force-vector-interleave=2" on x86_64-linux-gnu

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/153946
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-08-25T17:55:24Z

## Body

Compiler Explorer: https://godbolt.org/z/KdM7Pao71 

Note:
- works: 20.1.0 and earlier
- fails: trunk, only at -Os

```
[522] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 1d73b2c10d3bb1a2caf91f44847b9759ed252b21)
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
clangtk: warning: argument unused during compilation: '-I /usr/local/include' [-Wunused-command-line-argument]
clangtk: warning: argument unused during compilation: '-I /local/suz-local/software/local/include' [-Wunused-command-line-argument]
[523] % 
[523] % clangtk -Os small.c; ./a.out
[524] % clangtk -Os -mllvm -force-vector-width=4 -mllvm -force-vector-interleave=2 small.c
[525] % ./a.out
Floating point exception
[526] % 
[526] % cat small.c
int a, b, c, d, e, h, i;
int f(int g) {
  a = 0;
  for (; a < 8; a++) {
    b = g >> a * 8;
    if (b)
      return 0;
  }
  return 0;
}
int main() {
  int j = 1 << c;
  while (e)
    ;
  if (!i)
    h = 1 / (f(j) + j);
  d = 1 % h;
  return 0;
}
```



## Comments

### Comment 1 - fhahn

Taking a look.

---

