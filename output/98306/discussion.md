# Buggy optimization of `vfmaddcsh` intrinsics

**Author:** sayantn
**URL:** https://github.com/llvm/llvm-project/issues/98306
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2024-11-29T12:58:49Z

## Body

The `llvm.x86.avx512fp16.maskz.vfmadd.csh` intrinsic (and due to that, `_mm_maskz_fmadd_sch`) is being incorrectly optimized. This code snippet

```C
#include<immintrin.h>
#include<stdio.h>

int main() {
    __m128h a, b, c, r;
    _Float16 array[8];

    a = _mm_setr_ph(0.0, 1.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0);
    b = _mm_setr_ph(0.0, 2.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0);
    c = _mm_setr_ph(0.0, 3.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0);

    r = _mm_maskz_fmadd_sch(0, a, b, c);
    _mm_storeu_ph(array, r);

    for (int i = 0; i < 8; i++){
        printf("%f\n", (float) array[i]);
    }

    return 0;
}
```

In `clang`, the unoptimized and optimized output is different. The unoptimized output is the correct one according to Intel. `gcc` gives the correct output in both.

![image](https://github.com/llvm/llvm-project/assets/142906350/3e75696e-fb02-4ae0-ab2f-25f1d0637cc7)

System specification:
 - `mingw-w64-x86_64-gcc 14.2.0`
 - `mingw-w64-x86_64-clang 18.1.8`
 - Intel Software Development Emulator v9.44.0


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Sayantan Chakraborty (sayantn)

<details>
The `llvm.x86.avx512fp16.maskz.vfmadd.csh` intrinsic (and due to that, `_mm_maskz_fmadd_sch`) is being incorrectly optimized. This code snippet

```C
#include&lt;immintrin.h&gt;
#include&lt;stdio.h&gt;

int main() {
    __m128h a, b, c, r;
    _Float16 array[8];

    a = _mm_setr_ph(0.0, 1.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0);
    b = _mm_setr_ph(0.0, 2.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0);
    c = _mm_setr_ph(0.0, 3.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0);

    r = _mm_maskz_fmadd_sch(0, a, b, c);
    _mm_storeu_ph(array, r);

    for (int i = 0; i &lt; 8; i++){
        printf("%f\n", (float) array[i]);
    }

    return 0;
}
```

In `clang`, the unoptimized and optimized output is different. The unoptimized output is the correct one according to Intel. `gcc` gives the correct output in both.

![image](https://github.com/llvm/llvm-project/assets/142906350/3e75696e-fb02-4ae0-ab2f-25f1d0637cc7)

System specification:
 - `mingw-w64-x86_64-gcc 14.1.0-3`
 - `mingw-w64-x86_64-clang 18.1.8-1`
 - Intel Software Development Emulator v9.33.0

</details>


---

### Comment 2 - RKSimon

CC @phoebewang @KanRobert 

---

