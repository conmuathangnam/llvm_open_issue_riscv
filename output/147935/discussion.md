# [ARM] Incorrect __gnu_h2f_ieee ABI

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/147935
**Status:** Closed
**Labels:** backend:ARM, miscompilation
**Closed Date:** 2025-07-10T13:47:45Z

## Body

```llvm
target triple = "arm-unknown-linux-gnueabihf"

define i1 @test(half %self) #0 {
  %_0 = fcmp une half %self, %self
  ret i1 %_0
}

attributes #0 = { "target-features"="+vfp2" }
```

Produces:
```asm
	.save	{r11, lr}
	push	{r11, lr}
	vmov	r0, s0
	bl	__gnu_h2f_ieee
	vcmp.f32	s0, s0
	mov	r0, #0
	vmrs	APSR_nzcv, fpscr
	movvs	r0, #1
	pop	{r11, lr}
	mov	pc, lr
```
LLVM 20 produced the correct:
```asm
	.save	{r11, lr}
	push	{r11, lr}
	vmov	r0, s0
	bl	__gnu_h2f_ieee
	vmov	s0, r0
	mov	r0, #0
	vcmp.f32	s0, s0
	vmrs	APSR_nzcv, fpscr
	movvs	r0, #1
	pop	{r11, lr}
	mov	pc, lr

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Nikita Popov (nikic)

<details>
```llvm
target triple = "arm-unknown-linux-gnueabihf"

define i1 @<!-- -->test(half %self) #<!-- -->0 {
  %_0 = fcmp une half %self, %self
  ret i1 %_0
}

attributes #<!-- -->0 = { "target-features"="+vfp2" }
```

Produces:
```asm
	.save	{r11, lr}
	push	{r11, lr}
	vmov	r0, s0
	bl	__gnu_h2f_ieee
	vcmp.f32	s0, s0
	mov	r0, #<!-- -->0
	vmrs	APSR_nzcv, fpscr
	movvs	r0, #<!-- -->1
	pop	{r11, lr}
	mov	pc, lr
```
LLVM 20 produced the correct:
```asm
	.save	{r11, lr}
	push	{r11, lr}
	vmov	r0, s0
	bl	__gnu_h2f_ieee
	vmov	s0, r0
	mov	r0, #<!-- -->0
	vcmp.f32	s0, s0
	vmrs	APSR_nzcv, fpscr
	movvs	r0, #<!-- -->1
	pop	{r11, lr}
	mov	pc, lr

```
</details>


---

### Comment 2 - nikic

@arsenm This issue was introduced in the RuntimeLibCalls migration.

From a quick look https://github.com/llvm/llvm-project/blob/a446300d1bd0b97b400b2f246f1aa861d62115e6/llvm/lib/Target/ARM/ARMISelLowering.cpp#L710-L723 now specifies AAPCS on __extendhfsf2 specifically, while previously it was on all FPEXT_F16_F32 libcalls, including __gnu_h2f_ieee.

---

