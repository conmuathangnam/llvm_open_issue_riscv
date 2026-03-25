# wrong code at -O1 and above on x86_64-linux-gnu

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/124275
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2025-01-28T21:54:02Z

## Body

It appears to be a recent regression from 19.1.0. 

Compiler Explorer: https://godbolt.org/z/haMfGoo6h

```
[548] % clangtk -v
clang version 20.0.0git (https://github.com/llvm/llvm-project.git ddd2f57b29661f21308eec0400fa92a6d075b0c6)
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
[549] % 
[549] % clangtk -O1 small.c
[550] % ./a.out
Aborted
[551] % 
[551] % cat small.c
int printf(const char *, ...);
unsigned a;
int b, c = 1;
int main() {
  int d;
  a = -1;
  if (a < 1)
    goto e;
  d = c;
  if (d) {
  e:;
  }
  if (!d)
    __builtin_abort();
  if (b)
    goto e;
  return 0;
}
```

## Comments

### Comment 1 - zhendongsu

Another likely related test:
```
[552] % clangtk -v
clang version 20.0.0git (https://github.com/llvm/llvm-project.git ddd2f57b29661f21308eec0400fa92a6d075b0c6)
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
[553] % 
[553] % clangtk -O0 small.c
[554] % ./a.out
[555] % 
[555] % clangtk -O1 small.c
[556] % timeout -s 9 5 ./a.out
Killed
[557] % cat small.c
int a, b;
int main() {
  {
    int c;
    if (a && 0)
      goto d;
  }
  int e = -~b;
  if (e) {
  d:;
  }
  while (!e)
    goto d;
  return 0;
}
```

---

### Comment 2 - antoniofrighetto

Reduced to:

```llvm
; opt -p instcombine small.ll
define i32 @small(i32 noundef %0) {
  %2 = xor i32 %0, 1
  %3 = sub i32 0, %2
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %5, label %6

5:                                                ; preds = %6, %1
  %.0 = phi i32 [ %.1, %6 ], [ %3, %1 ]
  br label %6

6:                                                ; preds = %5, %1
  %.1 = phi i32 [ %.0, %5 ], [ 0, %1 ]
  %7 = icmp ne i32 %.1, 0
  %8 = xor i1 %7, true
  br i1 %8, label %5, label %9

9:                                                ; preds = %6
  ret i32 0
}
```

---

### Comment 3 - goldsteinn

Fix at: https://github.com/llvm/llvm-project/pull/124481

---

