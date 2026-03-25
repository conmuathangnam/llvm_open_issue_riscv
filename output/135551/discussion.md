# [AArch64] Argument passing is not ABI-compatible with GCC

**Author:** hvdijk
**URL:** https://github.com/llvm/llvm-project/issues/135551
**Status:** Closed
**Labels:** clang:frontend, miscompilation
**Closed Date:** 2025-04-18T01:11:03Z

## Body

```console
$ cat test.cc
struct X {
  alignas(16) int F[4];
};
struct Y : X {};
void f(Y);
void g(int, Y y) { f(y); }
$ g++ -S -O3 -o - test.cc
	.arch armv8-a
	.file	"test.cc"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z1gi1Y
	.type	_Z1gi1Y, %function
_Z1gi1Y:
.LFB0:
	.cfi_startproc
	mov	x0, x2
	mov	x1, x3
	b	_Z1f1Y
	.cfi_endproc
.LFE0:
	.size	_Z1gi1Y, .-_Z1gi1Y
	.ident	"GCC: (Ubuntu 14.2.0-19ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
$ clang++ -S -O3 -o - test.cc
	.file	"test.cc"
	.text
	.globl	_Z1gi1Y                         // -- Begin function _Z1gi1Y
	.p2align	2
	.type	_Z1gi1Y,@function
_Z1gi1Y:                                // @_Z1gi1Y
	.cfi_startproc
// %bb.0:
	mov	x0, x1
	mov	x1, x2
	b	_Z1f1Y
.Lfunc_end0:
	.size	_Z1gi1Y, .Lfunc_end0-_Z1gi1Y
	.cfi_endproc
                                        // -- End function
	.ident	"Ubuntu clang version 20.1.2 (0ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
```
Clang is expecting `g`'s `Y` parameter to be passed in `x1`, `x2`, but GCC is expecting it to be passed in `x2`, `x3`.

If I remove the `alignas(16)`, GCC and Clang agree that it should be passed in `x1`, `x2`.

I do not know whether this is a bug in GCC or in Clang.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Harald van Dijk (hvdijk)

<details>
```console
$ cat test.cc
struct X {
  alignas(16) int F[4];
};
struct Y : X {};
void f(Y);
void g(int, Y y) { f(y); }
$ g++ -S -O3 -o - test.cc
	.arch armv8-a
	.file	"test.cc"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z1gi1Y
	.type	_Z1gi1Y, %function
_Z1gi1Y:
.LFB0:
	.cfi_startproc
	mov	x0, x2
	mov	x1, x3
	b	_Z1f1Y
	.cfi_endproc
.LFE0:
	.size	_Z1gi1Y, .-_Z1gi1Y
	.ident	"GCC: (Ubuntu 14.2.0-19ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@<!-- -->progbits
$ clang++ -S -O3 -o - test.cc
	.file	"test.cc"
	.text
	.globl	_Z1gi1Y                         // -- Begin function _Z1gi1Y
	.p2align	2
	.type	_Z1gi1Y,@<!-- -->function
_Z1gi1Y:                                // @<!-- -->_Z1gi1Y
	.cfi_startproc
// %bb.0:
	mov	x0, x1
	mov	x1, x2
	b	_Z1f1Y
.Lfunc_end0:
	.size	_Z1gi1Y, .Lfunc_end0-_Z1gi1Y
	.cfi_endproc
                                        // -- End function
	.ident	"Ubuntu clang version 20.1.2 (0ubuntu1)"
	.section	".note.GNU-stack","",@<!-- -->progbits
	.addrsig
```
Clang is expecting `g`'s `Y` parameter to be passed in `x1`, `x2`, but GCC is expecting it to be passed in `x2`, `x3`.

If I remove the `alignas(16)`, GCC and Clang agree that it should be passed in `x1`, `x2`.

I do not know whether this is a bug in GCC or in Clang.
</details>


---

### Comment 2 - hvdijk

I *think* this is a Clang bug. The relevant rule of [AAPCS64](https://github.com/ARM-software/abi-aa/blob/main/aapcs64/aapcs64.rst) is Parameter passing C.10:

> If the argument has an alignment of 16 then the NGRN is rounded up to the next even number.

It looks like Clang is only taking into account the alignment of members in `Y`, ignoring alignment that comes from `Y`'s base classes. Since the AAPCS64 does not address C++, how base classes affect the ABI is not answered by it, but GCC and Clang agree that this should be passed in `x2`, `x3` if `X` is passed directly, or if `X` is made a member of `Y` rather than a base class. I would think that what Clang is doing cannot be intended, GCC is surely doing the right thing here?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Harald van Dijk (hvdijk)

<details>
```console
$ cat test.cc
struct X {
  alignas(16) int F[4];
};
struct Y : X {};
void f(Y);
void g(int, Y y) { f(y); }
$ g++ -S -O3 -o - test.cc
	.arch armv8-a
	.file	"test.cc"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z1gi1Y
	.type	_Z1gi1Y, %function
_Z1gi1Y:
.LFB0:
	.cfi_startproc
	mov	x0, x2
	mov	x1, x3
	b	_Z1f1Y
	.cfi_endproc
.LFE0:
	.size	_Z1gi1Y, .-_Z1gi1Y
	.ident	"GCC: (Ubuntu 14.2.0-19ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@<!-- -->progbits
$ clang++ -S -O3 -o - test.cc
	.file	"test.cc"
	.text
	.globl	_Z1gi1Y                         // -- Begin function _Z1gi1Y
	.p2align	2
	.type	_Z1gi1Y,@<!-- -->function
_Z1gi1Y:                                // @<!-- -->_Z1gi1Y
	.cfi_startproc
// %bb.0:
	mov	x0, x1
	mov	x1, x2
	b	_Z1f1Y
.Lfunc_end0:
	.size	_Z1gi1Y, .Lfunc_end0-_Z1gi1Y
	.cfi_endproc
                                        // -- End function
	.ident	"Ubuntu clang version 20.1.2 (0ubuntu1)"
	.section	".note.GNU-stack","",@<!-- -->progbits
	.addrsig
```
Clang is expecting `g`'s `Y` parameter to be passed in `x1`, `x2`, but GCC is expecting it to be passed in `x2`, `x3`.

If I remove the `alignas(16)`, GCC and Clang agree that it should be passed in `x1`, `x2`.

I do not know whether this is a bug in GCC or in Clang.
</details>


---

