# [RISC-V] Miscompile at -O3

**Author:** patrick-rivos
**URL:** https://github.com/llvm/llvm-project/issues/111555

## Body

Testcase:
```c
int printf(const char *, ...);
short a, d, k, r, s;
int b, c, f, i, w, y = -963555834;
char e, g, h, j, n, p, u, x;
unsigned l;
long m;
long long aa;
void ab(short ac, int ad, int ae, unsigned af) {
  for (long o = 0; o < 10969; o += af)
    for (int q = 0; q < (s ?: (0 >= ae) > p); q = -10827206556692219962ULL) {
      for (int t = j; t; t = u) {
        for (int v = 0; v < i; v += 2) {
          for (long z = r; z; z += 4) {
            a = w ? k / j : m;
            b = x ? ad ? p ?: d : 0 : r;
          }
          for (; 0 < ({
                   __typeof__(0) ag = l;
                   ag;
                 });)
            ;
          for (char ah = 0; ah < 1; ah += 178)
            ;
          for (; 2 < w;)
            ;
          for (; 0 < s;)
            ;
          for (int ai = 0; ai < 21; ai += 4)
            ;
          for (; p;)
            c = 0;
        }
        for (short aj = 0; aj < 3; aj += r) {
          for (char ak = j; ak; ak++)
            d ^= ~0;
          for (short al = (unsigned short)ad - 24655; al < (short)l; al += 4)
            e += ae;
          for (short am = (unsigned short)ad - 24655; am; am = 4)
            ;
        }
      }
      for (int an = 0; an < ae + 963555845; an += 4)
        for (long ao = s; ao < ae + 963555845; ao += 4) {
          for (char ap = -7; ap < 3; ap += ac)
            for (char aq = n; aq < 1; aq += 2) {
              f += ac;
              g = ({
                __typeof__(0) ar = b ? ({ r < (unsigned short)p; }) : 0;
                g ?: ar;
              });
            }
          h = ({ h ?: x; });
        }
    }
}
void as(long long *ac, int ad) { *ac = ad; }
int main() {
  ab(2, 10958, y, 8);
  as(&aa, f);
  printf("%llu\n", aa);
}
```

Commands:
```
> /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
red.c:10:51: warning: implicit conversion from 'unsigned long long' to 'int' changes value from 7619537517017331654 to 1855223750 [-Wconstant-conversion]
   10 |     for (int q = 0; q < (s ?: (0 >= ae) > p); q = -10827206556692219962ULL) {
      |                                                 ~ ^~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
> QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
0

> /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
red.c:10:51: warning: implicit conversion from 'unsigned long long' to 'int' changes value from 7619537517017331654 to 1855223750 [-Wconstant-conversion]
   10 |     for (int q = 0; q < (s ?: (0 >= ae) > p); q = -10827206556692219962ULL) {
      |                                                 ~ ^~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
> QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
123480
```

-opt-bisect-limit points to Stack Slot Coloring on function (ab)

Found via fuzzer

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Patrick O'Neill (patrick-rivos)

<details>
Testcase:
```c
int printf(const char *, ...);
short a, d, k, r, s;
int b, c, f, i, w, y = -963555834;
char e, g, h, j, n, p, u, x;
unsigned l;
long m;
long long aa;
void ab(short ac, int ad, int ae, unsigned af) {
  for (long o = 0; o &lt; 10969; o += af)
    for (int q = 0; q &lt; (s ?: (0 &gt;= ae) &gt; p); q = -10827206556692219962ULL) {
      for (int t = j; t; t = u) {
        for (int v = 0; v &lt; i; v += 2) {
          for (long z = r; z; z += 4) {
            a = w ? k / j : m;
            b = x ? ad ? p ?: d : 0 : r;
          }
          for (; 0 &lt; ({
                   __typeof__(0) ag = l;
                   ag;
                 });)
            ;
          for (char ah = 0; ah &lt; 1; ah += 178)
            ;
          for (; 2 &lt; w;)
            ;
          for (; 0 &lt; s;)
            ;
          for (int ai = 0; ai &lt; 21; ai += 4)
            ;
          for (; p;)
            c = 0;
        }
        for (short aj = 0; aj &lt; 3; aj += r) {
          for (char ak = j; ak; ak++)
            d ^= ~0;
          for (short al = (unsigned short)ad - 24655; al &lt; (short)l; al += 4)
            e += ae;
          for (short am = (unsigned short)ad - 24655; am; am = 4)
            ;
        }
      }
      for (int an = 0; an &lt; ae + 963555845; an += 4)
        for (long ao = s; ao &lt; ae + 963555845; ao += 4) {
          for (char ap = -7; ap &lt; 3; ap += ac)
            for (char aq = n; aq &lt; 1; aq += 2) {
              f += ac;
              g = ({
                __typeof__(0) ar = b ? ({ r &lt; (unsigned short)p; }) : 0;
                g ?: ar;
              });
            }
          h = ({ h ?: x; });
        }
    }
}
void as(long long *ac, int ad) { *ac = ad; }
int main() {
  ab(2, 10958, y, 8);
  as(&amp;aa, f);
  printf("%llu\n", aa);
}
```

Commands:
```
&gt; /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
red.c:10:51: warning: implicit conversion from 'unsigned long long' to 'int' changes value from 7619537517017331654 to 1855223750 [-Wconstant-conversion]
   10 |     for (int q = 0; q &lt; (s ?: (0 &gt;= ae) &gt; p); q = -10827206556692219962ULL) {
      |                                                 ~ ^~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
&gt; QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
0

&gt; /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/build-llvm-linux/bin/clang -march=rv64gcv -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv
red.c:10:51: warning: implicit conversion from 'unsigned long long' to 'int' changes value from 7619537517017331654 to 1855223750 [-Wconstant-conversion]
   10 |     for (int q = 0; q &lt; (s ?: (0 &gt;= ae) &gt; p); q = -10827206556692219962ULL) {
      |                                                 ~ ^~~~~~~~~~~~~~~~~~~~~~~~
1 warning generated.
&gt; QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/tc-testing/tc-compiler-fuzz-trunk/build-gcv/bin/qemu-riscv64 user-config.out
123480
```

-opt-bisect-limit points to Stack Slot Coloring on function (ab)

Found via fuzzer
</details>


---

### Comment 2 - topperc

There was a stackcoloring bug file just today. I wonder if its related https://github.com/llvm/llvm-project/issues/126252

---

