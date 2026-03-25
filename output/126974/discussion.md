# [RISC-V] Miscompile using rv64gcv

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/126974
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-02-16T12:18:30Z

## Body

Testcase:
```c
int a[][21][21];
void b(int c, int d, int e, short f, _Bool g, char h, char i, int j, int k,
       int l, unsigned char m, int n, char o, int p, long long q[],
       unsigned short r[][10], unsigned s[][10],
       unsigned long long t[][10][10][10], unsigned long long u[][10][10][10],
       int w[][10][10][10][10], _Bool x) {
  for (signed z = 0; z < (char)m + 3; z = 4)
    for (short aa = ({
                      __typeof__(0) ab = 173;
                      __typeof__(0) ac = ({
                        __typeof__(0) ab = m;
                        ab;
                      });
                      ab > ac ? ab : ac;
                    }) -
                    171;
         aa < 9; aa = k)
      a[z][aa][aa] = ~0;
}
typedef int ad;
int printf(const char *, ...);
long long ae;
void af(long long *ae, int v) { *ae ^= v; }
int c;
int d;
int e;
short f;
_Bool g;
char h;
char i;
int j;
int k = 2805750831;
int l;
char m;
int n;
char o;
int p;
long long q[0];
unsigned short r[0][10];
unsigned s[0][10];
unsigned long long t[0][10][10][10];
unsigned long long u[0][10][10][10];
int w[0][10][10][10][10];
_Bool x;
int a[21][21][21];
void ag() {
  for (ad y = 0; y < 10; ++y)
    for (ad ah = 0; ah < 10; ++ah)
      for (ad ai = 0; ai < 10; ++ai)
        af(&ae, a[y][ah][ai]);
}
int main() {
  b(c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, w, x);
  ag();
  printf("%llu\n", ae);
}
```

Commands:
```
# riscv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
0

# x86
$ ./native.out 1
18446744073709551615
```

Godbolt: https://godbolt.org/z/85rz18rxY

Bisected to cfee344dda7394631f2177a15e56cfeee1d61fc4 as the first bad commit

I tried reducing the testcase down farther but was unsuccessful

Found via fuzzer. First detected 01/24/2025

## Comments

### Comment 1 - dzaima

Some manual simplification, reproduces on x86-64 `-O3` too:

```c
#include<stdio.h>

void f(int more_than_20, unsigned char zero) {
  int i = 0;
  while (i < (signed char)zero + 2) {
    int max = zero < 210 ? 210 : zero; // 210; has to be >127
    int j = max - 200; // 10
    while (j < 20) {
      printf("I should be printed once\n");
      j = more_than_20;
    }
    i = 2; // can be anything >2
  }
}

int main() {
  f(50, 0);
}
```

https://godbolt.org/z/qfn9PT6r5

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```c
int a[][21][21];
void b(int c, int d, int e, short f, _Bool g, char h, char i, int j, int k,
       int l, unsigned char m, int n, char o, int p, long long q[],
       unsigned short r[][10], unsigned s[][10],
       unsigned long long t[][10][10][10], unsigned long long u[][10][10][10],
       int w[][10][10][10][10], _Bool x) {
  for (signed z = 0; z &lt; (char)m + 3; z = 4)
    for (short aa = ({
                      __typeof__(0) ab = 173;
                      __typeof__(0) ac = ({
                        __typeof__(0) ab = m;
                        ab;
                      });
                      ab &gt; ac ? ab : ac;
                    }) -
                    171;
         aa &lt; 9; aa = k)
      a[z][aa][aa] = ~0;
}
typedef int ad;
int printf(const char *, ...);
long long ae;
void af(long long *ae, int v) { *ae ^= v; }
int c;
int d;
int e;
short f;
_Bool g;
char h;
char i;
int j;
int k = 2805750831;
int l;
char m;
int n;
char o;
int p;
long long q[0];
unsigned short r[0][10];
unsigned s[0][10];
unsigned long long t[0][10][10][10];
unsigned long long u[0][10][10][10];
int w[0][10][10][10][10];
_Bool x;
int a[21][21][21];
void ag() {
  for (ad y = 0; y &lt; 10; ++y)
    for (ad ah = 0; ah &lt; 10; ++ah)
      for (ad ai = 0; ai &lt; 10; ++ai)
        af(&amp;ae, a[y][ah][ai]);
}
int main() {
  b(c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, w, x);
  ag();
  printf("%llu\n", ae);
}
```

Commands:
```
# riscv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
0

# x86
$ ./native.out 1
18446744073709551615
```

Godbolt: https://godbolt.org/z/85rz18rxY

Bisected to cfee344dda7394631f2177a15e56cfeee1d61fc4 as the first bad commit

I tried reducing the testcase down farther but was unsuccessful

Found via fuzzer. First detected 01/24/2025
</details>


---

### Comment 3 - dtcxzyw

Reproducer: https://alive2.llvm.org/ce/z/adparc

---

### Comment 4 - dtcxzyw

Further reduced: https://alive2.llvm.org/ce/z/5da83j

---

