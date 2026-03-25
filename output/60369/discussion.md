# possible illegal asm from AArch64 + global isel

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/60369
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, llvm:globalisel, confirmed
**Closed Date:** 2025-06-10T11:13:01Z

## Body

this function 
```llvm
define signext i64 @f() {
  %mask = trunc i63 -272678883688449 to i6
  %mask1 = sext i6 %mask to i63
  %1 = lshr i63 -3179364354444895371, %mask1
  %2 = trunc i63 %1 to i9
  %_sext = sext i9 %2 to i32
  %_zext = zext i32 %_sext to i64
  ret i64 %_zext
}
```

gives the following output using Global Isel:
```console
$ llc foo.ll -o - -global-isel -global-isel-abort=0 -march=aarch64
	.text
	.file	"foo.ll"
	.globl	f                               // -- Begin function f
	.p2align	2
	.type	f,@function
f:                                      // @f
	.cfi_startproc
// %bb.0:
	mov	x8, #56181
	movk	x8, #61893, lsl #16
	movk	x8, #41206, lsl #32
	movk	x8, #21472, lsl #48
	sbfiz	x8, x8, #-9223372036854775743, #-9223372036854775800
	and	x0, x8, #0xffffffff
	ret
.Lfunc_end0:
	.size	f, .Lfunc_end0-f
	.cfi_endproc
                                        // -- End function
	.section	".note.GNU-stack","",@progbits
$ 
```

but then this can't be parsed back:
```console
$ llvm-mc foo.s --arch=aarch64
	.text
	.file	"foo.ll"
	.globl	f
	.p2align	2
	.type	f,@function
f:
	.cfi_startproc
	mov	x8, #56181
	movk	x8, #61893, lsl #16
	movk	x8, #41206, lsl #32
	movk	x8, #21472, lsl #48
foo.s:13:16: error: expected integer in range [0, 31]
        sbfiz   x8, x8, #-9223372036854775743, #-9223372036854775800
                        ^
	and	x0, x8, #0xffffffff
	ret
.Lfunc_end0:
	.size	f, .Lfunc_end0-f
	.cfi_endproc
	.section	".note.GNU-stack","",@progbits
$ 
```
I'm not sure which side has the bug, but it seems like one side or the other should be fixed.

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-backend-aarch64

---

### Comment 2 - DavidSpickett

This instruction is `C6.2.267 SBFIZ` in the ARMARM document.

I think this is first an issue with the assembler output. If you compile to an object then disassemble it, Arm Compiler's fromelf, gnu objdump and llvm-objdump agree it's not what the assembler printed it as.

```console
$ ./bin/fromelf -c /tmp/test.o
        0x00000010:    937f1d08    ....    SBFIZ    x8,x8,#1,#8
$ ./bin/llvm-objdump -d /tmp/test.o
      10: 937f1d08      sbfiz   x8, x8, #1, #8
$ objdump -d /tmp/test.o
  10:   937f1d08        sbfiz   x8, x8, #1, #8
```
My guess is that the assembler is outputting them as some form of signed number, despite the fields not accepting negative values.

This instruction is an alias of `sbfm`.
```
Applies when sf == 1 && N == 1.
SBFIZ <Xd>, <Xn>, #<lsb>, #<width>
 is equivalent to
SBFM <Xd>, <Xn>, #(-<lsb> MOD 64), #(<width>-1)
and is the preferred disassembly when UInt(imms) < UInt(immr).
```
So we have `immr=0b111111` and `imms=0b000111`. `#1` is the result of `immr` being 63, negated and mod 64 resulting in 1. `#8` is the result of `imms` of 7 plus 1 being 8.

Second issue is that the validation of the immediate values is not correct for 64 bit.
```console
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #63, #1"
        sbfiz   x8, x8, #63, #1
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #64, #1"
<stdin>:1:15: error: expected integer in range [0, 31]
sbfiz x8, x8, #64, #1
              ^
```
For the 64 bit variant (x registers) this field can be 0 to 63. So the check itself is correct, but the resulting error message is talking about the 32 bit variant (w registers). The w variant gets both right.
```console
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz w8, w8, #31, #1"
        sbfiz   w8, w8, #31, #1
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz w8, w8, #32, #1"
<stdin>:1:15: error: expected integer in range [0, 31]
sbfiz w8, w8, #32, #1
              ^
```
Same thing with the second immediate. It should be 1-32 for 32 bit and 1-64 for the 64 bit variant.
```console
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz w8, w8, #8, #0"
<stdin>:1:19: error: expected integer in range [1, 32]
sbfiz w8, w8, #8, #0
                  ^
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #8, #0"
<stdin>:1:19: error: expected integer in range [1, 32]
sbfiz x8, x8, #8, #0
                  ^
```
There is a further restriction between the two values, to check that you wouldn't overflow the register. That error is correct for both variants.
```console
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz w8, w8, #8, #24"
        sbfiz   w8, w8, #8, #24
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz w8, w8, #8, #25"
<stdin>:1:19: error: requested insert overflows register
sbfiz w8, w8, #8, #25
                  ^
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #8, #55"
        sbfiz   x8, x8, #8, #55
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #8, #56"
        sbfiz   x8, x8, #8, #56
$ ./bin/llvm-mc --arch=aarch64 <<< "sbfiz x8, x8, #8, #57"
<stdin>:1:19: error: requested insert overflows register
sbfiz x8, x8, #8, #57
                  ^
```

All this may be due to this instruction being one of many preferred aliases of `sbfm`. Perhaps some of those do accept negative values, and we may have forgotten to customise the error message when we changed the check for 32 vs 64 bit variants.

---

### Comment 3 - c-rhodes

reproduced on 16: https://godbolt.org/z/E7aYWx1PT. In 17 the `sbfiz` is gone, I bisected this to f7d1baa414e6. On trunk it's simply lowered to `mov x0, xzr`. Given the bad MI is no longer emitted I think this one can be closed.

---

