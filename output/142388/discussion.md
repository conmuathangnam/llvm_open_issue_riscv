# [Sparc] V9 only instructions are defined for Sparc V8

**Author:** Rot127
**URL:** https://github.com/llvm/llvm-project/issues/142388
**Status:** Closed
**Labels:** backend:Sparc, miscompilation
**Closed Date:** 2025-06-19T22:12:52Z

## Body

The `LDX` and `STX` instructions are not defined in the Sparc v8 ISA. But assemble in in LLVM instead of being invalid.

```
echo "ldx [%g1], %o7 ; stx %o7, [%g1]"  | llvm-mc --assemble --triple=sparc --show-encoding
	.text
	ldx [%g1], %o7                          ! encoding: [0xde,0x58,0x40,0x00]
	stx %o7, [%g1]                          ! encoding: [0xde,0x70,0x40,0x00]
```

cc @koachan

## Comments

### Comment 1 - Rot127

The `X` version of the shift instruction as well:

```
cat /tmp/asd  | llvm-mc --assemble --triple=sparc --show-encoding
	.text
	sllx %g1, %g2, %g3                      ! encoding: [0x87,0x28,0x50,0x02]
	sllx %g1, 63, %g3                       ! encoding: [0x87,0x28,0x70,0x3f]
	srlx %g1, %g2, %g3                      ! encoding: [0x87,0x30,0x50,0x02]
	srlx %g1, 63, %g3                       ! encoding: [0x87,0x30,0x70,0x3f]
	srax %g1, %g2, %g3                      ! encoding: [0x87,0x38,0x50,0x02]
	srax %g1, 63, %g3                       ! encoding: [0x87,0x38,0x70,0x3f]
```

---

### Comment 2 - Rot127

Also `MULX`, `DIVX`, `ADDX` etc.

---

### Comment 3 - Rot127

Some more mismatches between v8/v9.
The move instructions which use `xcc` are assembled for v8 but should be invalid. MOVR as well.

```
cat /tmp/asd  | llvm-mc --assemble --triple=sparc --show-encoding
	.text
	mov	%g1, %g3                        ! encoding: [0x86,0x10,0x00,0x01]
	mov	255, %g3                        ! encoding: [0x86,0x10,0x20,0xff]

	mova	%xcc, 80, %g1                   ! encoding: [0x83,0x66,0x30,0x50]
	movn	%xcc, 80, %g1                   ! encoding: [0x83,0x64,0x30,0x50]
	movne	%xcc, 80, %g1                   ! encoding: [0x83,0x66,0x70,0x50]
	move	%xcc, 80, %g1                   ! encoding: [0x83,0x64,0x70,0x50]
	movg	%xcc, 80, %g1                   ! encoding: [0x83,0x66,0xb0,0x50]
	movle	%xcc, 80, %g1                   ! encoding: [0x83,0x64,0xb0,0x50]
	movge	%xcc, 80, %g1                   ! encoding: [0x83,0x66,0xf0,0x50]
	movl	%xcc, 80, %g1                   ! encoding: [0x83,0x64,0xf0,0x50]
	movgu	%xcc, 80, %g1                   ! encoding: [0x83,0x67,0x30,0x50]
	movleu	%xcc, 80, %g1                   ! encoding: [0x83,0x65,0x30,0x50]
	movcc	%xcc, 80, %g1                   ! encoding: [0x83,0x67,0x70,0x50]
	movcs	%xcc, 80, %g1                   ! encoding: [0x83,0x65,0x70,0x50]
	movpos	%xcc, 80, %g1                   ! encoding: [0x83,0x67,0xb0,0x50]
	movneg	%xcc, 80, %g1                   ! encoding: [0x83,0x65,0xb0,0x50]
	movvc	%xcc, 80, %g1                   ! encoding: [0x83,0x67,0xf0,0x50]
	movvs	%xcc, 80, %g1                   ! encoding: [0x83,0x65,0xf0,0x50]

	movrz %l0, %g5, %g1                     ! encoding: [0x83,0x7c,0x04,0x05]
	movrlez	%l0, %g5, %g1                   ! encoding: [0x83,0x7c,0x08,0x05]
	movrlz	%l0, %g5, %g1                   ! encoding: [0x83,0x7c,0x0c,0x05]
	movrnz %l0, %g5, %g1                    ! encoding: [0x83,0x7c,0x14,0x05]
	movrgz	%l0, %g5, %g1                   ! encoding: [0x83,0x7c,0x18,0x05]
	movrgez	%l0, %g5, %g1                   ! encoding: [0x83,0x7c,0x1c,0x05]

	movrz %l0, 16, %g1                      ! encoding: [0x83,0x7c,0x24,0x10]
	movrlez	%l0, 16, %g1                    ! encoding: [0x83,0x7c,0x28,0x10]
	movrlz	%l0, 16, %g1                    ! encoding: [0x83,0x7c,0x2c,0x10]
	movrnz %l0, 16, %g1                     ! encoding: [0x83,0x7c,0x34,0x10]
	movrgz	%l0, 16, %g1                    ! encoding: [0x83,0x7c,0x38,0x10]
	movrgez	%l0, 16, %g1                    ! encoding: [0x83,0x7c,0x3c,0x10]

```

Other instructions using `xcc` seem to be affected as well. For example `Tcc` (conditional trap).

---

### Comment 4 - koachan

Yeah seems like a lot of instruction definitions aren't properly gated yet...

---

