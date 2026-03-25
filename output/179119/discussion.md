# Miscompilation when using 32-bit ucmp on 64-bit PowerPC

**Author:** SiliconA-Z
**URL:** https://github.com/llvm/llvm-project/issues/179119
**Status:** Closed
**Labels:** backend:PowerPC, miscompilation
**Closed Date:** 2026-02-02T08:00:42Z

## Body

Rust team found an issue when compiling one of their packages where their sort function broke entirely. https://github.com/rust-lang/rust/issues/151807

Now, after reading the backtrace, and studying the assembler, and running tests, I realized that there was a mistake due to the quirks of 64-bit PowerPC and an incorrect assumption I made about the backend.

You see: PowerPC on 64 bits in the backend, i32 appears to be legal because i guess one of the instructions can check the bottom 32 bits, but there are no 32 bit subregisters. Now, while we can definitely deal with that as an issue after talking with maintainers another time, for now what matters is that because of how this lowering of ucmp relies on the carry register, which unlike ARM64, you cannot use subregisters directly. It has to be manually cleared. So: 

```ppcasm
; CHECK-NEXT:    subc 6, 4, 3
; CHECK-NEXT:    sub 5, 3, 4
; CHECK-NEXT:    subfe 3, 4, 3
; CHECK-NEXT:    subfe 3, 3, 5
; CHECK-NEXT:    blr
```

works great assuming zeroext to 64 bits, but nope. It is 32 bits. 

While I can definitely look into alternative ways to fix this in the long term, in the short term, to prevent miscompiles when the input is 32 bit, I need to manually clear these bits for now to fix the issue in the release version of LLVM 22 and in main.

## The "Lie" ##
In the LLVM backend, i32 is indeed marked as legal (via addRegisterClass(MVT::i32, &PPC::GPRCRegClass)). This is because the PowerPC ISA has a full suite of "word" instructions (like lwz, stw, add, subf, etc.) that provide 32-bit semantics.

However, there are no independent 32-bit physical registers. In PPC64 hardware, there is only one set of 32 General Purpose Registers (GPRs), and they are all 64 bits wide. This differs from x86-64 and AArch64.

We model this by defining Rn (32-bit) as a subregister of Xn (64-bit) in PPCRegisterInfo.td.

```
class GP8<GPR SubReg, string n> : PPCReg<n> {
  let HWEncoding = SubReg.HWEncoding;
  let SubRegs = [SubReg];
  let SubRegIndices = [sub_32];
}
```

This is a convenient abstraction that allows the register allocator to treat them as separate entities when targetting 32-bit vs 64-bit, even though they share the same hardware encoding.

## Where I messed up in my first ucmp patch ##
The "Quirk": Carry logic is always 64-bit.

The carry flag (XER[CA]) is always computed based on the full 64-bit operation when the processor is in 64-bit mode.

If you perform an i32 unsigned comparison using carry-producing instructions (like subfc), any "garbage" bits in the upper 32 bits of the 64-bit registers (bits 0-31 in IBM notation) will participate in the borrow/carry calculation. 

To get around this, I need to manually zero-extend to 64 bits in 32-bit mode on a 64-bit PowerPC target.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: None (SiliconA-Z)

<details>
Rust team found an issue when compiling one of their packages where their sort function broke entirely.

Now, after reading the backtrace, and studying the assembler, and running tests, I realized that there was a mistake due to the quirks of 64-bit PowerPC and an incorrect assumption I made about the backend.

You see: PowerPC on 64 bits in the backend, i32 appears to be legal because i guess one of the instructions can check the bottom 32 bits, but there are no 32 bit subregisters. Now, while we can definitely deal with that as an issue after talking with maintainers another time, for now what matters is that because of how this lowering of ucmp relies on the carry register, which unlike ARM64, you cannot use subregisters directly. It has to be manually cleared. So: 

; CHECK-NEXT:    subc 6, 4, 3

; CHECK-NEXT:    sub 5, 3, 4

; CHECK-NEXT:    subfe 3, 4, 3
; CHECK-NEXT:    subfe 3, 3, 5
; CHECK-NEXT:    blr

works great assuming zeroext to 64 bits, but nope. It is 32 bits. 

While I can definitely look into alternative ways to fix this in the long term, in the short term, to prevent miscompiles when the input is 32 bit, I need to manually clear these bits for now to fix the issue in the release version of LLVM 22 and in main for now.

## The "Lie" ##
In the LLVM backend, i32 is indeed marked as legal (via addRegisterClass(MVT::i32, &amp;PPC::GPRCRegClass)). This is because the PowerPC ISA has a full suite of "word" instructions (like lwz, stw, add, subf, etc.) that provide 32-bit semantics.

However, there are no independent 32-bit physical registers. In PPC64 hardware, there is only one set of 32 General Purpose Registers (GPRs), and they are all 64 bits wide. This differs from x86-64 and AArch64.

We model this by defining Rn (32-bit) as a subregister of Xn (64-bit) in PPCRegisterInfo.td.

```
class GP8&lt;GPR SubReg, string n&gt; : PPCReg&lt;n&gt; {
  let HWEncoding = SubReg.HWEncoding;
  let SubRegs = [SubReg];
  let SubRegIndices = [sub_32];
}
```

This is a convenient abstraction that allows the register allocator to treat them as separate entities when targetting 32-bit vs 64-bit, even though they share the same hardware encoding.

## Where I messed up in my first ucmp patch ##
The "Quirk": Carry logic is always 64-bit.

The carry flag (XER[CA]) is always computed based on the full 64-bit operation when the processor is in 64-bit mode.

If you perform an i32 unsigned comparison using carry-producing instructions (like subfc), any "garbage" bits in the upper 32 bits of the 64-bit registers (bits 0-31 in IBM notation) will participate in the borrow/carry calculation. 

To get around this, I need to manually zero-extend to 64 bits in 32-bit mode on a 64-bit PowerPC target.
</details>


---

