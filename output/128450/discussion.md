# [clang][X86] Wrong result for __builtin_elementwise_fma on _Float16

**Author:** SEt-t
**URL:** https://github.com/llvm/llvm-project/issues/128450
**Status:** Closed
**Labels:** duplicate, miscompilation, llvm:SelectionDAG
**Closed Date:** 2026-01-28T09:14:56Z

## Body

Godbolt: https://godbolt.org/z/Ydj17K17b

Clang uses single-precision FMA to emulate half-precision FMA, what is wrong as it doesn't have enough precision.

Example, round to even: 0x1.400p+8 * 0x1.008p+7 + 0x1.000p-24
Precise result: 0x1.40a0000002p+15
Half-precision FMA: 0x1.40cp+15
Single-precision FMA: 0x1.40a000p+15
(clang) Single-precision FMA -> half-precision: 0x1.408p+15

Another example: 0x1.eb8p-12 * 0x1.9p-11 - 0x1p-11

To produce correct result single-precision multiplication, then double-precision addition seems to be enough.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: SEt (SEt-t)

<details>
Godbolt: https://godbolt.org/z/Ydj17K17b

Clang uses single-precision FMA to emulate half-precision FMA, what is wrong as it doesn't have enough precision.

Example, round to even: 0x1.400p+8 * 0x1.008p+7 + 0x1.000p-24
Precise result: 0x1.40a0000002p+15
Half-precision FMA: 0x1.40cp+15
Single-precision FMA: 0x1.40a000p+15
(clang) Single-precision FMA -&gt; half-precision: 0x1.408p+15

Another example: 0x1.eb8p-12 * 0x1.9p-11 - 0x1p-11

To produce correct result single-precision multiplication, then double-precision addition seems to be enough.
</details>


---

### Comment 2 - akshaykumars614

I want to work on this issue. Any input or suggestions on where to start would be really helpful!

---

### Comment 3 - beetrees

This bug is a duplicate of #98389.

@akshaykumars614 AFAICT this is where the half FMA operation is lowered incorrectly:

https://github.com/llvm/llvm-project/blob/5cc2ae0b519065c39df124d46de1ba75d4b144eb/llvm/lib/CodeGen/SelectionDAG/LegalizeFloatTypes.cpp#L3396-L3414

---

### Comment 4 - RalfJung

> To produce correct result single-precision multiplication, then double-precision addition seems to be enough.

Are you sure this will always be enough for all inputs?

---

### Comment 5 - beetrees

Yes. Building off [this paper](https://dl.acm.org/doi/pdf/10.1145/221332.221334), 16-bit floats have an 11-bit significand (including the implicit bit), meaning that the result of the multiplication requires 22 bits to store losslessly (`0b111_1111_1111 * 0b111_1111_1111 = 0b11_1111_1111_0000_0000_0001`). (EDIT: The following sentence if only true when the multiplication result is less than the number being added: see https://github.com/llvm/llvm-project/issues/128450#issuecomment-2727540179 for details) The addition therefore requires 22 + 11 + 1 = 34 bits of precision in order to prevent double rounding problems. 64-bit floats have 53-bit significands, whereas 32-bit floats only have 24-bit significands.

---

### Comment 6 - RalfJung

Okay so single precision is enough for the multiplication (24 >= 22) and then double-precision is enough for the total operation (53 >= 34)?

---

### Comment 7 - beetrees

Yes EDIT: see https://github.com/llvm/llvm-project/issues/128450#issuecomment-2727540179

---

### Comment 8 - SEt-t

> The addition therefore requires 22 + 11 + 1 = 34 bits of precision in order to prevent double rounding problems.

You are wrong about addition bits (see my [example](https://github.com/llvm/llvm-project/issues/128450#issue-2873660884) of 40 bits), but conclusion that double is enough for it is correct.

---

### Comment 9 - beetrees

Ah yes, I see where I messed up.

---

### Comment 10 - beetrees

### Corrected explanation for `fmaf16(a, b, c) == fma(a, b, c)`:

As 16-bit floats only have 11-bit significands (64-bit floats have 53-bit significands), the result of `a * b` (henceforth referred to as `x`) will always be exactly representable in 11 + 11 = 22 bits. In most cases (since 16-bit floats only have small exponent range), the result of the addition/subtraction of `x` and `c` will also be exactly representable; there are two scenarios where this is not the case:

#### `c > x`, such that their exponents differ by more than 53 - 22 = 32
As `c` is only 11-bits in size, my [previous comment](https://github.com/llvm/llvm-project/issues/128450#issuecomment-2727353366) is still correct for this case as the digits required to separate the two numbers will be in the correct place to prevent double rounding from being an issue when the final result is rounded to a 64-bit float and then to a 16-bit float.

#### `x > c`, such that their exponents differ by more than 53 - 11 = 42
As `c` is a regular 16-bit float it has a minimum exponent of -14, meaning that `x` must have an exponent of at least -14 + 42 + 1 = 29. The result's exponent will be larger than the maximum 16-bit float exponent of 15, meaning that the result will always be rounded to infinity when converting back to a 16-bit float (for reference, this means that the intermediate float type needs at least a 11 + 14 + 15 = 40 bit significand; this means that results where `x > c` will either be exactly representable or round to infinity when converted to a 16-bit float).

---

