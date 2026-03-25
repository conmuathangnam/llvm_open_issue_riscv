# llc 18.1.6 incorrectly optimizes out llvm.s390.vperm when an U8 vector is zero-extended to an U16

**Author:** johnplatts
**URL:** https://github.com/llvm/llvm-project/issues/92615
**Status:** Closed
**Labels:** backend:SystemZ, miscompilation
**Closed Date:** 2025-04-24T09:31:12Z

## Body

Here is a snippet of LLVM IR code that generates incorrect code with llc 18.1.6 with the `-O2 -filetype=asm -mtriple=s390x-linux-gnu` options:
```
target datalayout = "E-m:e-i1:8:16-i8:8:16-i64:64-f128:64-v128:64-a:8:16-n32:64"
target triple = "s390x-unknown-linux-gnu"

; Function Attrs: nofree nosync nounwind memory(none)
define dso_local <8 x i16> @U8WidenReverse4AndAdd_1(<16 x i8> noundef %0) local_unnamed_addr #0 {
  %2 = shufflevector <16 x i8> <i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison>, <16 x i8> %0, <16 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23>
  %3 = bitcast <16 x i8> %2 to <8 x i16>
  %4 = tail call <16 x i8> @llvm.s390.vperm(<16 x i8> %2, <16 x i8> %2, <16 x i8> <i8 6, i8 7, i8 4, i8 5, i8 2, i8 3, i8 0, i8 1, i8 14, i8 15, i8 12, i8 13, i8 10, i8 11, i8 8, i8 9>)
  %5 = bitcast <16 x i8> %4 to <8 x i16>
  %6 = add <8 x i16> %3, %5
  ret <8 x i16> %6
}

; Function Attrs: nofree nosync nounwind memory(none)
define dso_local <8 x i16> @U8WidenReverse4AndAdd_2(<16 x i8> noundef %0) local_unnamed_addr #0 {
  %2 = shufflevector <16 x i8> %0, <16 x i8> %0, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  %3 = zext <8 x i8> %2 to <8 x i16>
  %4 = bitcast <8 x i16> %3 to <16 x i8>
  %5 = tail call <16 x i8> @llvm.s390.vperm(<16 x i8> %4, <16 x i8> %4, <16 x i8> <i8 6, i8 7, i8 4, i8 5, i8 2, i8 3, i8 0, i8 1, i8 14, i8 15, i8 12, i8 13, i8 10, i8 11, i8 8, i8 9>)
  %6 = bitcast <16 x i8> %5 to <8 x i16>
  %7 = add <8 x i16> %3, %6
  ret <8 x i16> %7
}

; Function Attrs: nofree nosync nounwind memory(none)
define dso_local <8 x i16> @U8WidenReverse4AndAdd_3(<16 x i8> noundef %0) local_unnamed_addr #0 {
  %2 = shufflevector <16 x i8> <i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison, i8 poison>, <16 x i8> %0, <16 x i32> <i32 0, i32 16, i32 1, i32 17, i32 2, i32 18, i32 3, i32 19, i32 4, i32 20, i32 5, i32 21, i32 6, i32 22, i32 7, i32 23>
  %3 = bitcast <16 x i8> %2 to <8 x i16>
  %4 = shufflevector <16 x i8> %2, <16 x i8> %2, <16 x i32> <i32 6, i32 7, i32 4, i32 5, i32 2, i32 3, i32 0, i32 1, i32 14, i32 15, i32 12, i32 13, i32 10, i32 11, i32 8, i32 9>
  %5 = bitcast <16 x i8> %4 to <8 x i16>
  %6 = add <8 x i16> %3, %5
  ret <8 x i16> %6
}

; Function Attrs: nofree nosync nounwind memory(none)
define dso_local <8 x i16> @U8WidenReverse4AndAdd_4(<16 x i8> noundef %0) local_unnamed_addr #0 {
  %2 = shufflevector <16 x i8> %0, <16 x i8> %0, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
  %3 = zext <8 x i8> %2 to <8 x i16>
  %4 = bitcast <8 x i16> %3 to <16 x i8>
  %5 = shufflevector <16 x i8> %4, <16 x i8> %4, <16 x i32> <i32 6, i32 7, i32 4, i32 5, i32 2, i32 3, i32 0, i32 1, i32 14, i32 15, i32 12, i32 13, i32 10, i32 11, i32 8, i32 9>
  %6 = bitcast <16 x i8> %5 to <8 x i16>
  %7 = add <8 x i16> %3, %6
  ret <8 x i16> %7
}

; Function Attrs: nofree nosync nounwind memory(none)
declare <16 x i8> @llvm.s390.vperm(<16 x i8>, <16 x i8>, <16 x i8>) #1

attributes #0 = { nofree nosync nounwind memory(none) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="z13" "target-features"="+transactional-execution,+vector" }
attributes #1 = { nofree nosync nounwind memory(none) }
```

Here is the assembly code that llc 18.1.6 generates when the above LLVM IR code is compiled with the `-O2 -filetype=asm -mtriple=s390x-linux-gnu' options:
```
	.text
	.file	"s390x_u8_widen_rev4_and_add_051724.ll"
	.globl	U8WidenReverse4AndAdd_1         # -- Begin function U8WidenReverse4AndAdd_1
	.p2align	4
	.type	U8WidenReverse4AndAdd_1,@function
U8WidenReverse4AndAdd_1:                # @U8WidenReverse4AndAdd_1
# %bb.0:
	vuplhb	%v24, %v24
	br	%r14
.Lfunc_end0:
	.size	U8WidenReverse4AndAdd_1, .Lfunc_end0-U8WidenReverse4AndAdd_1
                                        # -- End function
	.globl	U8WidenReverse4AndAdd_2         # -- Begin function U8WidenReverse4AndAdd_2
	.p2align	4
	.type	U8WidenReverse4AndAdd_2,@function
U8WidenReverse4AndAdd_2:                # @U8WidenReverse4AndAdd_2
# %bb.0:
	vuplhb	%v24, %v24
	br	%r14
.Lfunc_end1:
	.size	U8WidenReverse4AndAdd_2, .Lfunc_end1-U8WidenReverse4AndAdd_2
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	3, 0x0                          # -- Begin function U8WidenReverse4AndAdd_3
.LCPI2_0:
	.byte	6                               # 0x6
	.byte	7                               # 0x7
	.byte	4                               # 0x4
	.byte	5                               # 0x5
	.byte	2                               # 0x2
	.byte	3                               # 0x3
	.byte	0                               # 0x0
	.byte	1                               # 0x1
	.byte	14                              # 0xe
	.byte	15                              # 0xf
	.byte	12                              # 0xc
	.byte	13                              # 0xd
	.byte	10                              # 0xa
	.byte	11                              # 0xb
	.byte	8                               # 0x8
	.byte	9                               # 0x9
	.text
	.globl	U8WidenReverse4AndAdd_3
	.p2align	4
	.type	U8WidenReverse4AndAdd_3,@function
U8WidenReverse4AndAdd_3:                # @U8WidenReverse4AndAdd_3
# %bb.0:
	larl	%r1, .LCPI2_0
	vl	%v1, 0(%r1), 3
	vuplhb	%v0, %v24
	vperm	%v1, %v0, %v0, %v1
	vah	%v24, %v0, %v1
	br	%r14
.Lfunc_end2:
	.size	U8WidenReverse4AndAdd_3, .Lfunc_end2-U8WidenReverse4AndAdd_3
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	3, 0x0                          # -- Begin function U8WidenReverse4AndAdd_4
.LCPI3_0:
	.byte	6                               # 0x6
	.byte	7                               # 0x7
	.byte	4                               # 0x4
	.byte	5                               # 0x5
	.byte	2                               # 0x2
	.byte	3                               # 0x3
	.byte	0                               # 0x0
	.byte	1                               # 0x1
	.byte	14                              # 0xe
	.byte	15                              # 0xf
	.byte	12                              # 0xc
	.byte	13                              # 0xd
	.byte	10                              # 0xa
	.byte	11                              # 0xb
	.byte	8                               # 0x8
	.byte	9                               # 0x9
	.text
	.globl	U8WidenReverse4AndAdd_4
	.p2align	4
	.type	U8WidenReverse4AndAdd_4,@function
U8WidenReverse4AndAdd_4:                # @U8WidenReverse4AndAdd_4
# %bb.0:
	larl	%r1, .LCPI3_0
	vl	%v1, 0(%r1), 3
	vuplhb	%v0, %v24
	vperm	%v1, %v0, %v0, %v1
	vah	%v24, %v0, %v1
	br	%r14
.Lfunc_end3:
	.size	U8WidenReverse4AndAdd_4, .Lfunc_end3-U8WidenReverse4AndAdd_4
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
```

llc 18.1.6 fails to emit the vperm and vah instructions that are expected in all four functions above when the @llvm.s390.vperm intrinsic is used, but llc 18.1.6 correctly emits the vperm and vah instructions when shufflevector is used instead of @llvm.s390.vperm.

## Comments

### Comment 1 - dominik-steenken

I think as already pointed out in #95454, @JonPsson1 fixed this with commit https://github.com/llvm/llvm-project/commit/7e4c6e98fa05f5c3bf14f96365ae74a8d12c6257 . Can this issue be closed?

---

### Comment 2 - JonPsson1

The above test case works now on main, so this was indeed likely fixed by #88623 (7e4c6e9).

---

