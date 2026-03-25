# Miscompilation of `VCVT{T}P{H,S}2I{U}BS` intrinsics

**Author:** sayantn
**URL:** https://github.com/llvm/llvm-project/issues/184251
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2026-03-09T06:06:31Z

## Body

Intrinsics such as `_mm_ipcvts_ph_epi8` have different output depending on optimization level. As typical, the `O0` output is correct, while the `O3` output seems to be completely wrong.

Concretely, consider the example

`experiment.c`
```C
#include<immintrin.h>
#include<stdio.h>
#include<math.h>
#include<stdint.h>

int main() {
    __m128h a = _mm_setr_ph(1.0, -2.5, 127.0, 128.0, 255.0, -128.0, 0.0, NAN);

    __m128i r = _mm_ipcvtts_ph_epi8(a);

    uint16_t array[8];
    _mm_storeu_si128((__m128i*) array, r);

    for (int i = 0; i < 8; ++i) {
        printf("%d\n", (int8_t) array[i]);
    }
}
```

<img width="811" height="609" alt="Image" src="https://github.com/user-attachments/assets/d423893a-2de7-4312-9088-b2865e7ac16c" />

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


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Sayantan Chakraborty (sayantn)

<details>
Intrinsics such as `_mm_ipcvts_ph_epi8` have different output depending on optimization level. As typical, the `O0` output is correct, while the `O3` output seems to be completely wrong.

Concretely, consider the example

`experiment.c`
```C
#include&lt;immintrin.h&gt;
#include&lt;stdio.h&gt;
#include&lt;math.h&gt;
#include&lt;stdint.h&gt;

int main() {
    __m128h a = _mm_setr_ph(1.0, -2.5, 127.0, 128.0, 255.0, -128.0, 0.0, NAN);

    __m128i r = _mm_ipcvtts_ph_epi8(a);

    uint16_t array[8];
    _mm_storeu_si128((__m128i*) array, r);

    for (int i = 0; i &lt; 8; ++i) {
        printf("%d\n", (int8_t) array[i]);
    }
}
```

&lt;img width="811" height="609" alt="Image" src="https://github.com/user-attachments/assets/d423893a-2de7-4312-9088-b2865e7ac16c" /&gt;

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

</details>


---

### Comment 2 - phoebewang

It is not a CodeGen issue, using `-fno-integrated-as` gets the same result as GCC. When objdump the binary, we can see the offset of the constant is at 0x200f:
```
0000000000001140 <main>:
    1140: 41 57                         pushq   %r15
    1142: 41 56                         pushq   %r14
    1144: 53                            pushq   %rbx
    1145: 62 f5 7c 08 68 05 c0 0e 00 00 vcvttph2ibs     0xec0(%rip), %xmm0 # 0x200f <_IO_stdin_used+0xf>
```
However, the data is stored actually at 0x2010:
```
$ readelf -x .rodata a.out

Hex dump of section '.rodata':
  0x00002000 01000200 00000000 00000000 00000000 ................
  0x00002010 003c00c1 f0570058 f85b00d8 0000007e .<...W.X.[.....~
  0x00002020 25640a00                            %d..
```
GDB shows the same problem:
```
(gdb) x/8hx 0x40200f
0x40200f:  0x0000  0x003c  0xf0c1  0x0057  0xf858  0x005b  0x00d8  0x0000
(gdb) x/8hx 0x402010
0x402010:  0x3c00  0xc100  0x57f0  0x5800  0x5bf8  0xd800  0x0000  0x7e00
```

I didn't see such bugs before, but it doesn't look to me individue instruction related. @MaskRay do you have any clue?

---

### Comment 3 - MaskRay

This is an integrated assembler bug due to TableGen setting. https://github.com/llvm/llvm-project/pull/185254

We probably should have more `llvm-mc -filetype=obj ... | llvm-objdump -dr` memory reference tests to catch such bugs.

---

### Comment 4 - phoebewang

> This is an integrated assembler bug due to TableGen setting. [#185254](https://github.com/llvm/llvm-project/pull/185254)
> 
> We probably should have more `llvm-mc -filetype=obj ... | llvm-objdump -dr` memory reference tests to catch such bugs.

Thanks @MaskRay ! I was surprised it cannot be caught by encoding test. Is it possible to enhance it, e.g, check `!hasImm()` if there's no `RemainingOps` in https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp#L2001?

---

