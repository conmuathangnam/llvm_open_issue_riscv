# miscompile from AArch64 backend

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/133928
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2025-04-05T14:52:54Z

## Body

take this function:
```llvm
define i4 @f(ptr %0, i8 %1, i1 %2) {
  %4 = trunc i8 %1 to i4
  %5 = getelementptr i8, ptr %0, i8 %1
  %6 = icmp uge ptr %5, %0
  br i1 %6, label %common.ret, label %7

common.ret:                                       ; preds = %10, %7, %3
  %common.ret.op = phi i4 [ 0, %10 ], [ 1, %3 ], [ 0, %7 ]
  ret i4 %common.ret.op

7:                                                ; preds = %3
  %8 = add nuw i8 0, 0
  %9 = icmp ult i8 %8, %1
  br i1 %2, label %common.ret, label %10

10:                                               ; preds = %7
  %11 = udiv i4 1, 1
  br label %common.ret
}
```
when invoked like this `f((char *)0xd, -5, 0)`, Alive believes that it should return 0, and I agree:
%5 = 0x8
%6 = false
branch to label %7
branch to label %10
branch to label %common.ret
return 0

on the other hand, the AArch64 backend is giving this:
```
~/reduce$ ~/llvm-project/for-alive/bin/llc -march=aarch64 -o - reduced.ll
	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_f                              ; -- Begin function f
	.p2align	2
_f:                                     ; @f
	.cfi_startproc
; %bb.0:
	add	x8, x0, w1, uxtb
	cmp	x8, x0
	b.lo	LBB0_2
; %bb.1:
	mov	w0, #1                          ; =0x1
	ret
LBB0_2:
	mov	w0, wzr
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
~/reduce$ 
```
which returns 1 when called with those same arguments. it looks like maybe the add should be sxtb instead of uxtb since GEP offsets are signed?


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
take this function:
```llvm
define i4 @<!-- -->f(ptr %0, i8 %1, i1 %2) {
  %4 = trunc i8 %1 to i4
  %5 = getelementptr i8, ptr %0, i8 %1
  %6 = icmp uge ptr %5, %0
  br i1 %6, label %common.ret, label %7

common.ret:                                       ; preds = %10, %7, %3
  %common.ret.op = phi i4 [ 0, %10 ], [ 1, %3 ], [ 0, %7 ]
  ret i4 %common.ret.op

7:                                                ; preds = %3
  %8 = add nuw i8 0, 0
  %9 = icmp ult i8 %8, %1
  br i1 %2, label %common.ret, label %10

10:                                               ; preds = %7
  %11 = udiv i4 1, 1
  br label %common.ret
}
```
when invoked like this `f((char *)0xd, -5, 0)`, Alive believes that it should return 0, and I agree:
%5 = 0x8
%6 = false
branch to label %7
branch to label %10
branch to label %common.ret
return 0

on the other hand, the AArch64 backend is giving this:
```
~/reduce$ ~/llvm-project/for-alive/bin/llc -march=aarch64 -o - reduced.ll
	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_f                              ; -- Begin function f
	.p2align	2
_f:                                     ; @<!-- -->f
	.cfi_startproc
; %bb.0:
	add	x8, x0, w1, uxtb
	cmp	x8, x0
	b.lo	LBB0_2
; %bb.1:
	mov	w0, #<!-- -->1                          ; =0x1
	ret
LBB0_2:
	mov	w0, wzr
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
~/reduce$ 
```
which returns 1 when called with those same arguments. it looks like maybe the add should be sxtb instead of uxtb since GEP offsets are signed?

</details>


---

### Comment 2 - davemgreen

This is likely TypePromotion again. https://godbolt.org/z/sxaTWG6qn

---

### Comment 3 - antoniofrighetto

IR after TypePromotion: https://alive2.llvm.org/ce/z/AZhfeD.

---

