# wrong code at -O2 with "-mllvm -force-vector-width=8 -mllvm -force-vector-interleave=6" on x86_64-linux-gnu

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/155162
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-08-25T17:55:25Z

## Body

Compiler Explorer: https://godbolt.org/z/j167TcqE8

Note:
- works: 20.1.0 & earlier
- fails: trunk
- possibly related: https://github.com/llvm/llvm-project/issues/154967, https://github.com/llvm/llvm-project/issues/153946

```
[564] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git d13f40f386b4c4db15f232c5cfb105d362b84fe2)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.1
[565] % 
[565] % clangtk -O2 small.c; ./a.out
[566] % clangtk -O2 -mllvm -force-vector-width=8 -mllvm -force-vector-interleave=6 small.c
[567] % ./a.out
Aborted
[568] % cat small.c
int printf(const char *, ...);
int a, b;
void c(int *d, int e) {
  int f = 0;
  for (b = 0; b < e; b++)
    for (f = 0; f < 64; f++)
      if (d[b] << f)
        return;
}
int g(int h) {
  int i[] = {h};
  c(i, 8);
  return a;
}
int main() {
  if (g(1) != 0)
    __builtin_abort();
  return 0;
}
```

