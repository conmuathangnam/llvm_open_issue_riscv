# [miscompilation] set to 0 on __m128d ignored: garbage in high element on compare and movemask

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/33897
**Status:** Closed
**Labels:** backend:X86, bugzilla, miscompilation
**Closed Date:** 2025-02-19T09:30:50Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [34549](https://llvm.org/bz34549) |
| Version | 5.0 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @topperc,@RKSimon,@rotateright |

## Extended Description 
reduced testcase:
```cpp
#include <x86intrin.h>
#include <iostream>

std::ostream &operator<<(std::ostream &s, __m128d v)
{
    return s << '{' << v[0] << ", " << v[1] << '}';
}

int main()
{
    for (double lo_ : {1., 1.}) {
        for (double hi_ : {1., 1.}) {
            for (std::size_t pos = 0; pos < 2; ++pos) {
                __m128d lo = _mm_set1_pd(lo_);
                __m128d hi = _mm_set1_pd(hi_);
                if (0 != _mm_movemask_pd(hi < lo)) {
                    std::cerr << hi << ", lo: " << lo;
                    if (3 != _mm_movemask_pd(hi >= hi)) {
                        std::cerr << hi << ", lo: " << lo;
                    }
                }
            }
        }
    }

    __m128d x = _mm_set1_pd(1.);
    for (std::size_t i = 0; i < 2; ++i) {
        asm("ror $64,%%rax" ::"m"(x));
        x[i] = 0;  // #&#8203;1
    }
    asm("ror $64,%%rax" :"+m"(x));
    if (3 != _mm_movemask_pd(x == _mm_setzero_pd())) {
        std::cerr << "!!!FAILED!!!\n";
        return 1;
    }
    return 0;
}
```
Compile with `clang++-5.0 -std=c++14 -O2 -msse2`. clang 4.0 does not fail.

The assignment at line #1 is skipped for i = 1. However, minimal changes to unrelated code lead to the use of a `movsd` from memory, thus zeroing the upper 64 bits, as requested.

The above code is a reduced testcase from a unit test in https://github.com/VcDevel/Vc.

## Comments

### Comment 1 - llvmbot

See https://godbolt.org/g/aLbPuc starting at line 175:

175: load [1., 1.] into xmm0
176: store xmm0 to stack
177: inline-asm marker / syncs x to memory (nop)
178: load from stack to xmm0
179: load [0, 0] into xmm1
180: assign xmm0[0] = xmm1[0]; i.e. xmm0 = [0, 1]
181: store xmm0 to stack
182: inline-asm marker / syncs x to memory (nop)
     there should be an assignment xmm0[1] = 0 here
183: inline-asm marker / syncs x to memory (nop)
184-188: test for failure

Compare with https://godbolt.org/g/QEKmNu starting at line 6:
 6:  load [1., 1.] into xmm0
 7: store xmm0 to stack
 8: inline-asm marker / syncs x to memory (nop)
 9: load from stack to xmm0
10: load [0, 0] into xmm1
11: assign xmm0[0] = xmm1[0]; i.e. xmm0 = [0, 1]
12: store xmm0 to stack
13: inline-asm marker / syncs x to memory (nop)
14: load one double from stack to xmm0; i.e. xmm0 = [0, 0]
15: store xmm0 to stack
16: inline-asm marker / syncs x to memory (nop)
17-21: test for failure

---

### Comment 2 - llvmbot

clang version 7.0.0- (trunk)
installed from clang-7_7~svn325542-1~exp1_amd64.deb

also fails

---

### Comment 3 - llvmbot

clang version 6.0.0-svn325508-1~exp1 (branches/release_60) using libcstdc++ also fails.
I'm using the Ubuntu packages from apt.llvm.org on Ubuntu Xenial

---

### Comment 4 - llvmbot

I'll try to reproduce with the original unit test and a recent clang snapshot.

---

### Comment 5 - RKSimon

Matthias - are you still seeing this? I can't get this to reproduce with clang trunk

---

### Comment 6 - RKSimon

This was fixed between 6.0.1 and 7.0.0 - https://clang.godbolt.org/z/1dcTfcnaq

---

