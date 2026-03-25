# Miscompilation of `VMINMAXS{H,S,D}` intrinsics

**Author:** sayantn
**URL:** https://github.com/llvm/llvm-project/issues/184245
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2026-03-10T06:58:56Z

## Body

Intrinsics such as `_mm_mask_minmax_ss` should copy the upper elements from the first argument (`a`), but the LLVM implementation seems to copy it from the fallback argument (`src`).

[Intel Advanced Vector Extensions 10.2 Architecture Specification, Revision 6.0](https://www.intel.com/content/www/us/en/content-details/913918/intel-advanced-vector-extensions-10-2-intel-avx10-2-architecture-specification.html), Page 190
> Bits: 127:16/32/64 are copied to the destination from the respective elements of the first operand

As a concrete example, consider

`experiment.c`
```C
#include<immintrin.h>
#include<stdio.h>

int main() {
    __m128 a = _mm_setr_ps(1.0, 2.0, 3.0, 4.0);
    __m128 b = _mm_setr_ps(5.0, 6.0, 7.0, 8.0);
    __m128 src = _mm_setr_ps(20.0, 30.0, 40.0, 50.0);

    __m128 r = _mm_mask_minmax_ss(src, 1, a, b, 0);

    float array[4];
    _mm_storeu_ps(array, r);

    for (int i =0; i < 4; ++i) {
        printf("%f\n", array[i]);
    }
}
```

<img width="843" height="342" alt="Image" src="https://github.com/user-attachments/assets/aafcd16a-2111-4462-a192-203e943b3c37" />

The versions are as follows
`clang --version`
```
ClangBuiltLinux clang version 22.1.0 (https://github.com/llvm/llvm-project.git 4434dabb69916856b824f68a64b029c67175e532)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/schak/Documents/llvm-22.1.0-x86_64/bin
```
`gcc --version`
```
gcc (GCC) 15.2.1 20260209
Copyright (C) 2025 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
`sde64 --version`
```
Intel(R) Software Development Emulator.  Version:  10.5.0 external (0)
Copyright (C) 2008-2026, Intel Corporation. All rights reserved.
```

The equivalent(?) program in Rust behaves a bit differently (Rust doesn't yet have AVX10.2 intrinsics, in fact I am currently drafting a PR to add them, that's how I came across this)
`main.rs`
```rust
#![feature(link_llvm_intrinsics, abi_unadjusted, simd_ffi, avx10_target_feature)]
#![allow(internal_features)]

use std::arch::x86_64::*;

#[allow(improper_ctypes)]
unsafe extern "unadjusted" {
    #[link_name = "llvm.x86.avx10.mask.vminmaxss.round"]
    fn vminmaxss(a: __m128, b: __m128, imm8: i32, src: __m128, k: __mmask8, sae: i32) -> __m128;
}

#[inline]
#[target_feature(enable = "avx10.2")]
pub fn _mm_mask_minmax_ss<const IMM8: i32>(
    src: __m128,
    k: __mmask8,
    a: __m128,
    b: __m128,
) -> __m128 {
    unsafe { vminmaxss(a, b, IMM8, src, k as u8, _MM_FROUND_CUR_DIRECTION) }
}

fn main() {
    unsafe {
        let a = _mm_setr_ps(1.0, 2.0, 3.0, 4.0);
        let b = _mm_setr_ps(5.0, 6.0, 7.0, 8.0);
        let src = _mm_setr_ps(20.0, 30.0, 40.0, 50.0);

        let r = _mm_mask_minmax_ss::<0>(src, 1, a, b);

        let mut array = [0.0f32; 4];
        _mm_storeu_ps(array.as_mut_ptr(), r);

        for elem in array {
            println!("{elem}")
        }
    }
}
```

<img width="900" height="170" alt="Image" src="https://github.com/user-attachments/assets/8fe782c0-f792-4fa8-bee3-981da6c7349c" />

`rustc --version --verbose`
```
rustc 1.96.0-nightly (80381278a 2026-03-01)
binary: rustc
commit-hash: 80381278a08582356c13b0f52af92d27c567c230
commit-date: 2026-03-01
host: x86_64-unknown-linux-gnu
release: 1.96.0-nightly
LLVM version: 22.1.0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Sayantan Chakraborty (sayantn)

<details>
Intrinsics such as `_mm_mask_minmax_ss` should copy the upper elements from the first argument (`a`), but the LLVM implementation seems to copy it from the fallback argument (`src`).

[Intel Advanced Vector Extensions 10.2 Architecture Specification, Revision 6.0](https://www.intel.com/content/www/us/en/content-details/913918/intel-advanced-vector-extensions-10-2-intel-avx10-2-architecture-specification.html), Page 190
&gt; Bits: 127:16/32/64 are copied to the destination from the respective elements of the first operand

As a concrete example, consider

`experiment.c`
```C
#include&lt;immintrin.h&gt;
#include&lt;stdio.h&gt;

int main() {
    __m128 a = _mm_setr_ps(1.0, 2.0, 3.0, 4.0);
    __m128 b = _mm_setr_ps(5.0, 6.0, 7.0, 8.0);
    __m128 src = _mm_setr_ps(20.0, 30.0, 40.0, 50.0);

    __m128 r = _mm_mask_minmax_ss(src, 1, a, b, 0);

    float array[4];
    _mm_storeu_ps(array, r);

    for (int i =0; i &lt; 4; ++i) {
        printf("%f\n", array[i]);
    }
}
```

&lt;img width="843" height="342" alt="Image" src="https://github.com/user-attachments/assets/aafcd16a-2111-4462-a192-203e943b3c37" /&gt;

The versions are as follows
`clang --version`
```
ClangBuiltLinux clang version 22.1.0 (https://github.com/llvm/llvm-project.git 4434dabb69916856b824f68a64b029c67175e532)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/schak/Documents/llvm-22.1.0-x86_64/bin
```
`gcc --version`
```
gcc (GCC) 15.2.1 20260209
Copyright (C) 2025 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
`sde64 --version`
```
Intel(R) Software Development Emulator.  Version:  10.5.0 external (0)
Copyright (C) 2008-2026, Intel Corporation. All rights reserved.
```

The equivalent(?) program in Rust behaves a bit differently (Rust doesn't yet have AVX10.2 intrinsics, in fact I am currently drafting a PR to add them, that's how I came across this)
`main.rs`
```rust
#![feature(link_llvm_intrinsics, abi_unadjusted, simd_ffi, avx10_target_feature)]
#![allow(internal_features)]

use std::arch::x86_64::*;

#[allow(improper_ctypes)]
unsafe extern "unadjusted" {
    #[link_name = "llvm.x86.avx10.mask.vminmaxss.round"]
    fn vminmaxss(a: __m128, b: __m128, imm8: i32, src: __m128, k: __mmask8, sae: i32) -&gt; __m128;
}

#[inline]
#[target_feature(enable = "avx10.2")]
pub fn _mm_mask_minmax_ss&lt;const IMM8: i32&gt;(
    src: __m128,
    k: __mmask8,
    a: __m128,
    b: __m128,
) -&gt; __m128 {
    unsafe { vminmaxss(a, b, IMM8, src, k as u8, _MM_FROUND_CUR_DIRECTION) }
}

fn main() {
    unsafe {
        let a = _mm_setr_ps(1.0, 2.0, 3.0, 4.0);
        let b = _mm_setr_ps(5.0, 6.0, 7.0, 8.0);
        let src = _mm_setr_ps(20.0, 30.0, 40.0, 50.0);

        let r = _mm_mask_minmax_ss::&lt;0&gt;(src, 1, a, b);

        let mut array = [0.0f32; 4];
        _mm_storeu_ps(array.as_mut_ptr(), r);

        for elem in array {
            println!("{elem}")
        }
    }
}
```

&lt;img width="900" height="170" alt="Image" src="https://github.com/user-attachments/assets/8fe782c0-f792-4fa8-bee3-981da6c7349c" /&gt;

`rustc --version --verbose`
```
rustc 1.96.0-nightly (80381278a 2026-03-01)
binary: rustc
commit-hash: 80381278a08582356c13b0f52af92d27c567c230
commit-date: 2026-03-01
host: x86_64-unknown-linux-gnu
release: 1.96.0-nightly
LLVM version: 22.1.0
```
</details>


---

### Comment 2 - asmull

I started to dig around in this a bit, so I can volunteer for this one.

---

### Comment 3 - RKSimon

Similar to #179057 / #179101 ?

---

### Comment 4 - asmull

Confirmed after this change the example program executes correctly:

<img width="839" height="166" alt="Image" src="https://github.com/user-attachments/assets/7f64ccd0-23cd-4721-9ce1-ac9cb0c852d6" />

Review appreciated

---

