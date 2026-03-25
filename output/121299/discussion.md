# Possible miscompilation on armv7 FreeBSD 14.1

**Author:** clausecker
**URL:** https://github.com/llvm/llvm-project/issues/121299
**Status:** Closed
**Labels:** backend:ARM, miscompilation
**Closed Date:** 2025-01-07T08:44:20Z

## Body

Greetings,

While investigating a spurious test suite failure of the simdutf test suite, we found that the failure would only reproduce under specific circumstances:

 - recent llvm version (such as, the llvm 18.1.5 shipped with FreeBSD 14.1, but also a very recent devel snapshot)
 - build for armv7 FreeBSD
 - build with `-fno-strict-aliasing`, code works fine with `-fstrict-aliasing`

The result of the miscompilation is that wrong code is generated, leading to unit test failure.

A reduced example can be found in https://github.com/simdutf/simdutf/issues/581#issuecomment-2564785579.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Robert Clausecker (clausecker)

<details>
Greetings,

While investigating a spurious test suite failure of the simdutf test suite, we found that the failure would only reproduce under specific circumstances:

 - recent llvm version (such as, the llvm 18.1.5 shipped with FreeBSD 14.1, but also a very recent devel snapshot)
 - build for armv7 FreeBSD
 - build with `-fno-strict-aliasing`, code works fine with `-fstrict-aliasing`

The result of the miscompilation is that wrong code is generated, leading to unit test failure.

A reduced example can be found in https://github.com/simdutf/simdutf/issues/581#issuecomment-2564785579.
</details>


---

### Comment 2 - pauldreik

@dtcxzyw  what information is needed to make this complete?

---

### Comment 3 - antoniofrighetto

@dtcxzyw Haven't tried clang on trunk yet, but I managed to reproduce this with clang 18.1.5 targeting armv7 on FreeBSD.

---

### Comment 4 - dtcxzyw

> [@dtcxzyw](https://github.com/dtcxzyw) what information is needed to make this complete?

Can you provide a single-file reproducer? A godbolt link would be better :)


---

### Comment 5 - dtcxzyw

> > [@dtcxzyw](https://github.com/dtcxzyw) what information is needed to make this complete?
> 
> Can you provide a single-file reproducer? A godbolt link would be better :)

Oh sorry. I see that in the issue comment.


---

### Comment 6 - antoniofrighetto

The IR looks fine. I think the miscompilation lies immediately before the store `utf16[i + 0] = 0xd800;`, when saving the value of `utf16[i + 0]` in `old_W1`. Here is how the correct codegen looks like:
```asm
bls    std::__1::allocator<char16_t>>::operator[]
add    r1, r0, r7
mov    r2, r5
ldrh   r8, [r1]
mov    r1, #55296
strh   r1, [r0, r7]!
```
`utf16[0]` is saved in `r8` before storing 0xd800 on it. The load however seems to have been incorrectly moved around with `-fno-strict-aliasing`, leading to store 0xd800 without saving its original value:
```asm
bls    std::__1::allocator<char16_t>>::operator[]
mov    r1, r0
mov    r2, #55296
strh   r2, [r1, r7]!
```
Culprit seems to be machine code sinking, MIR before machine-sink:
```
bb.7 (%ir-block.24):
  %27:gpr = ADDrr %5:gpr, %3:gpr, 14, $noreg, $noreg
  %7:gpr = LDRH killed %27:gpr, $noreg, 0, 14, $noreg :: (load (s16) from %ir.scevgep5)
  %8:gpr = ADDri %6:gpr, 1, 14, $noreg, $noreg
  CMPrr %4:gpr, %8:gpr, 14, $noreg, implicit-def $cpsr
  Bcc %bb.9, 8, $cpsr
  B %bb.8

bb.9 (%ir-block.29):
  early-clobber %29:gpr = STRH_PRE %28:gpr, %5:gpr(tied-def 0), %3:gpr, 0, 14, $noreg
```
The buffer value is correctly loaded and saved before the `STRH_PRE` of `%28:gpr = MOVi 55296, 14, $noreg, $noreg`. After the pass, the `LDRH` seems to have been sunk too down in a few successor basic block later, breaking its dependency constraint: 
```
bb.7 (%ir-block.24):
  %8:gpr = ADDri %6:gpr, 1, 14, $noreg, $noreg
  CMPrr %4:gpr, %8:gpr, 14, $noreg, implicit-def $cpsr
  Bcc %bb.9, 8, $cpsr
  B %bb.8

bb.9 (%ir-block.29):
  early-clobber %29:gpr = STRH_PRE %28:gpr, %5:gpr(tied-def 0), %3:gpr, 0, 14, $noreg
  ; ...
  Bcc %bb.11, 3, $cpsr
  B %bb.10

bb.11 (%ir-block.39):
  %27:gpr = ADDrr %5:gpr, %3:gpr, 14, $noreg, $noreg
  %7:gpr = LDRH %27:gpr, $noreg, 0, 14, $noreg :: (load (s16) from %ir.scevgep5)
```
The original value has been overwritten by the time we load `utf16[0]`.

---

### Comment 7 - antoniofrighetto

Should be fixed in 446a426436c0b7e457992981d3a1f2b4fda19992, thank you for reporting this, Robert!

---

### Comment 8 - clausecker

@antoniofrighetto Thank you for your great work!

---

