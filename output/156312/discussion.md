# [AArch64] vector of u1 extraction broken under big endian targets

**Author:** GiuseppeCesarano
**URL:** https://github.com/llvm/llvm-project/issues/156312
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-09-07T12:33:14Z

## Body

The following IR should always return the value 1 but compiling with: `llc -O0 -mtriple=aarch64_be-linux-gnu -filetype=asm ir.ll -o be.s` the resulting assembly will actually return 0.

IR:
```ll
@haystack4 = internal unnamed_addr constant [4 x i32] [i32 0, i32 1, i32 2, i32 3], align 4

define i8 @test4() {
  %matches = alloca <4 x i1>, align 1
  %index_ptr = alloca i64, align 8
  store i64 0, ptr %index_ptr, align 8
  %index_val = load i64, ptr %index_ptr, align 8
  %haystack = getelementptr inbounds i32, ptr getelementptr inbounds (i8, ptr @haystack4, i64 0), i64 %index_val
  %h_vec = load <4 x i32>, ptr %haystack, align 4
  %cmp_vec = icmp eq <4 x i32> %h_vec, <i32 2, i32 2, i32 2, i32 2>
  store <4 x i1> %cmp_vec, ptr %matches, align 1
  %cmp_load = load <4 x i1>, ptr %matches, align 1
  %extr = extractelement <4 x i1> %cmp_load, i64 2
  %ret = zext i1 %extr to i8
  ret i8 %ret
}
```

Big endian asm:
```diff
	.file	"ir.ll"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function test4
.LCPI0_0:
	.word	1                               // 0x1
	.word	2                               // 0x2
	.word	4                               // 0x4
	.word	8                               // 0x8
	.text
	.globl	test4
	.p2align	2
	.type	test4,@function
test4:                                  // @test4
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	x8, xzr
	str	x8, [sp]
	ldr	x9, [sp]
	adrp	x8, haystack4
	add	x8, x8, :lo12:haystack4
	add	x8, x8, x9, lsl #2
	ld1	{ v0.4s }, [x8]		// v0.4s = { 0, 1, 2, 3 }
	movi	v1.4s, #2		// v1.4s = { 2, 2, 2, 2 }
	cmeq	v0.4s, v0.4s, v1.4s	// v0.4s = { 0, 0,-1, 0 }
	adrp	x8, .LCPI0_0
	add	x8, x8, :lo12:.LCPI0_0
	ld1	{ v1.4s }, [x8]		// v14s =  { 1, 2, 4, 8}
	and	v0.16b, v0.16b, v1.16b  // v0.16b = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0}
	addv	s1, v0.4s		// s1 = 4 = 0b100
                                        // implicit-def: $q0
	fmov	s0, s1
	fmov	w8, s0
	strb	w8, [sp, #15]
	ldrb	w8, [sp, #15]
	ubfx	w0, w8, #1, #1		// w0 = (0b100 >> 1) && 1 = 0
	add	sp, sp, #16
	ret
.Lfunc_end0:
	.size	test4, .Lfunc_end0-test4
	.cfi_endproc
                                        // -- End function
	.type	haystack4,@object               // @haystack4
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	2, 0x0
haystack4:
	.word	0                               // 0x0
	.word	1                               // 0x1
	.word	2                               // 0x2
	.word	3                               // 0x3
	.size	haystack4, 16

	.type	haystack16,@object              // @haystack16
	.p2align	4, 0x0
haystack16:
	.ascii	"\000\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017"
	.size	haystack16, 16

	.section	".note.GNU-stack","",@progbits
```

The problem is in the generated bit mask used to do the extraction, assumes that the bits inside `cmp_vec` will be in the same order as the lane meaning:
 
```
lane0 = bit0
lane1 = bit1
lane2 = bit2
...
```

Which is not the case under `aarch64_be`:
BE:
```
Vector-legalized selection DAG: %bb.0 'test4:'
SelectionDAG has 49 nodes:
    t0: ch,glue = EntryToken
  t4: ch = store<(store (s64) into %ir.index_ptr)> t0, Constant:i64<0>, FrameIndex:i64<1>, undef:i64
  t5: i64,ch = load<(dereferenceable load (s64) from %ir.index_ptr)> t4, FrameIndex:i64<1>, undef:i64
      t8: i64 = shl nsw t5, Constant:i64<2>
    t9: i64 = add GlobalAddress:i64<ptr @haystack4> 0, t8
  t10: v4i32,ch = load<(load (s128) from %ir.haystack, align 4)> t4, t9, undef:i64
    t16: ch = TokenFactor t5:1, t10:1
            t12: v4i32 = BUILD_VECTOR Constant:i32<2>, Constant:i32<2>, Constant:i32<2>, Constant:i32<2>
          t46: v4i32 = AArch64ISD::CMEQ t10, t12
          t40: v4i32 = BUILD_VECTOR Constant:i64<1>, Constant:i64<2>, Constant:i64<4>, Constant:i64<8>
        t41: v4i32 = and t46, t40
      t47: v4i32 = AArch64ISD::UADDV t41
    t48: i32 = extract_vector_elt t47, Constant:i64<0>
  t45: ch = store<(store (s8) into %ir.matches), trunc to i8> t16, t48, FrameIndex:i64<0>, undef:i64
                t52: i8 = srl t50, Constant:i64<3>
              t53: i8 = and t52, Constant:i8<1>
            t54: i1 = truncate t53
          t55: i16 = any_extend t54
                t56: i8 = srl t50, Constant:i64<2>
              t57: i8 = and t56, Constant:i8<1>
            t58: i1 = truncate t57
          t59: i16 = any_extend t58
                t60: i8 = srl t50, Constant:i64<1>
              t61: i8 = and t60, Constant:i8<1>
            t62: i1 = truncate t61
          t63: i16 = any_extend t62
              t64: i8 = and t50, Constant:i8<1>
            t65: i1 = truncate t64
          t66: i16 = any_extend t65
        t67: v4i16 = BUILD_VECTOR t55, t59, t63, t66
      t34: i32 = extract_vector_elt t67, Constant:i64<2>
    t31: i32 = and t34, Constant:i32<1>
  t23: ch,glue = CopyToReg t45, Register:i32 $w0, t31
  t50: i8,ch = load<(dereferenceable load (s8) from %ir.matches), anyext from i4> t45, FrameIndex:i64<0>, undef:i64
  t24: ch = AArch64ISD::RET_GLUE t23, Register:i32 $w0, t23:1
```
LE:
```
Vector-legalized selection DAG: %bb.0 'test4:'
SelectionDAG has 49 nodes:
    t0: ch,glue = EntryToken
  t4: ch = store<(store (s64) into %ir.index_ptr)> t0, Constant:i64<0>, FrameIndex:i64<1>, undef:i64
  t5: i64,ch = load<(dereferenceable load (s64) from %ir.index_ptr)> t4, FrameIndex:i64<1>, undef:i64
      t8: i64 = shl nsw t5, Constant:i64<2>
    t9: i64 = add GlobalAddress:i64<ptr @haystack4> 0, t8
  t10: v4i32,ch = load<(load (s128) from %ir.haystack, align 4)> t4, t9, undef:i64
    t16: ch = TokenFactor t5:1, t10:1
            t12: v4i32 = BUILD_VECTOR Constant:i32<2>, Constant:i32<2>, Constant:i32<2>, Constant:i32<2>
          t46: v4i32 = AArch64ISD::CMEQ t10, t12
          t40: v4i32 = BUILD_VECTOR Constant:i64<1>, Constant:i64<2>, Constant:i64<4>, Constant:i64<8>
        t41: v4i32 = and t46, t40
      t47: v4i32 = AArch64ISD::UADDV t41
    t48: i32 = extract_vector_elt t47, Constant:i64<0>
  t45: ch = store<(store (s8) into %ir.matches), trunc to i8> t16, t48, FrameIndex:i64<0>, undef:i64
              t51: i8 = and t50, Constant:i8<1>
            t52: i1 = truncate t51
          t53: i16 = any_extend t52
                t54: i8 = srl t50, Constant:i64<1>
              t55: i8 = and t54, Constant:i8<1>
            t56: i1 = truncate t55
          t57: i16 = any_extend t56
                t58: i8 = srl t50, Constant:i64<2>
              t59: i8 = and t58, Constant:i8<1>
            t60: i1 = truncate t59
          t61: i16 = any_extend t60
                t63: i8 = srl t50, Constant:i64<3>
              t64: i8 = and t63, Constant:i8<1>
            t65: i1 = truncate t64
          t66: i16 = any_extend t65
        t67: v4i16 = BUILD_VECTOR t53, t57, t61, t66
      t34: i32 = extract_vector_elt t67, Constant:i64<2>
    t31: i32 = and t34, Constant:i32<1>
  t23: ch,glue = CopyToReg t45, Register:i32 $w0, t31
  t50: i8,ch = load<(dereferenceable load (s8) from %ir.matches), anyext from i4> t45, FrameIndex:i64<0>, undef:i64
  t24: ch = AArch64ISD::RET_GLUE t23, Register:i32 $w0, t23:1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Giuseppe Cesarano (GiuseppeCesarano)

<details>
The following IR should always return the value 1 but compiling with: `llc -O0 -mtriple=aarch64_be-linux-gnu -filetype=asm ir.ll -o be.s` the resulting assembly will actually return 0.

IR:
```ll
@<!-- -->haystack4 = internal unnamed_addr constant [4 x i32] [i32 0, i32 1, i32 2, i32 3], align 4

define i8 @<!-- -->test4() {
  %matches = alloca &lt;4 x i1&gt;, align 1
  %index_ptr = alloca i64, align 8
  store i64 0, ptr %index_ptr, align 8
  %index_val = load i64, ptr %index_ptr, align 8
  %haystack = getelementptr inbounds i32, ptr getelementptr inbounds (i8, ptr @<!-- -->haystack4, i64 0), i64 %index_val
  %h_vec = load &lt;4 x i32&gt;, ptr %haystack, align 4
  %cmp_vec = icmp eq &lt;4 x i32&gt; %h_vec, &lt;i32 2, i32 2, i32 2, i32 2&gt;
  store &lt;4 x i1&gt; %cmp_vec, ptr %matches, align 1
  %cmp_load = load &lt;4 x i1&gt;, ptr %matches, align 1
  %extr = extractelement &lt;4 x i1&gt; %cmp_load, i64 2
  %ret = zext i1 %extr to i8
  ret i8 %ret
}
```

Big endian asm:
```diff
	.file	"ir.ll"
	.section	.rodata.cst16,"aM",@<!-- -->progbits,16
	.p2align	4, 0x0                          // -- Begin function test4
.LCPI0_0:
	.word	1                               // 0x1
	.word	2                               // 0x2
	.word	4                               // 0x4
	.word	8                               // 0x8
	.text
	.globl	test4
	.p2align	2
	.type	test4,@<!-- -->function
test4:                                  // @<!-- -->test4
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #<!-- -->16
	.cfi_def_cfa_offset 16
	mov	x8, xzr
	str	x8, [sp]
	ldr	x9, [sp]
	adrp	x8, haystack4
	add	x8, x8, :lo12:haystack4
	add	x8, x8, x9, lsl #<!-- -->2
	ld1	{ v0.4s }, [x8]		// v0.4s = { 0, 1, 2, 3 }
	movi	v1.4s, #<!-- -->2		// v1.4s = { 2, 2, 2, 2 }
	cmeq	v0.4s, v0.4s, v1.4s	// v0.4s = { 0, 0,-1, 0 }
	adrp	x8, .LCPI0_0
	add	x8, x8, :lo12:.LCPI0_0
	ld1	{ v1.4s }, [x8]		// v14s =  { 1, 2, 4, 8}
	and	v0.16b, v0.16b, v1.16b  // v0.16b = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0}
	addv	s1, v0.4s		// s1 = 4 = 0b100
                                        // implicit-def: $q0
	fmov	s0, s1
	fmov	w8, s0
	strb	w8, [sp, #<!-- -->15]
	ldrb	w8, [sp, #<!-- -->15]
	ubfx	w0, w8, #<!-- -->1, #<!-- -->1		// w0 = (0b100 &gt;&gt; 1) &amp;&amp; 1 = 0
	add	sp, sp, #<!-- -->16
	ret
.Lfunc_end0:
	.size	test4, .Lfunc_end0-test4
	.cfi_endproc
                                        // -- End function
	.type	haystack4,@<!-- -->object               // @<!-- -->haystack4
	.section	.rodata.cst16,"aM",@<!-- -->progbits,16
	.p2align	2, 0x0
haystack4:
	.word	0                               // 0x0
	.word	1                               // 0x1
	.word	2                               // 0x2
	.word	3                               // 0x3
	.size	haystack4, 16

	.type	haystack16,@<!-- -->object              // @<!-- -->haystack16
	.p2align	4, 0x0
haystack16:
	.ascii	"\000\001\002\003\004\005\006\007\b\t\n\013\f\r\016\017"
	.size	haystack16, 16

	.section	".note.GNU-stack","",@<!-- -->progbits
```

The problem is in the generated bit mask used to do the extraction, assumes that the bits inside `cmp_vec` will be in the same order as the lane meaning:
 
```
lane0 = bit0
lane1 = bit1
lane2 = bit2
...
```

Which is not the case under `aarch64_be`:
BE:
```
Vector-legalized selection DAG: %bb.0 'test4:'
SelectionDAG has 49 nodes:
    t0: ch,glue = EntryToken
  t4: ch = store&lt;(store (s64) into %ir.index_ptr)&gt; t0, Constant:i64&lt;0&gt;, FrameIndex:i64&lt;1&gt;, undef:i64
  t5: i64,ch = load&lt;(dereferenceable load (s64) from %ir.index_ptr)&gt; t4, FrameIndex:i64&lt;1&gt;, undef:i64
      t8: i64 = shl nsw t5, Constant:i64&lt;2&gt;
    t9: i64 = add GlobalAddress:i64&lt;ptr @<!-- -->haystack4&gt; 0, t8
  t10: v4i32,ch = load&lt;(load (s128) from %ir.haystack, align 4)&gt; t4, t9, undef:i64
    t16: ch = TokenFactor t5:1, t10:1
            t12: v4i32 = BUILD_VECTOR Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;
          t46: v4i32 = AArch64ISD::CMEQ t10, t12
          t40: v4i32 = BUILD_VECTOR Constant:i64&lt;1&gt;, Constant:i64&lt;2&gt;, Constant:i64&lt;4&gt;, Constant:i64&lt;8&gt;
        t41: v4i32 = and t46, t40
      t47: v4i32 = AArch64ISD::UADDV t41
    t48: i32 = extract_vector_elt t47, Constant:i64&lt;0&gt;
  t45: ch = store&lt;(store (s8) into %ir.matches), trunc to i8&gt; t16, t48, FrameIndex:i64&lt;0&gt;, undef:i64
                t52: i8 = srl t50, Constant:i64&lt;3&gt;
              t53: i8 = and t52, Constant:i8&lt;1&gt;
            t54: i1 = truncate t53
          t55: i16 = any_extend t54
                t56: i8 = srl t50, Constant:i64&lt;2&gt;
              t57: i8 = and t56, Constant:i8&lt;1&gt;
            t58: i1 = truncate t57
          t59: i16 = any_extend t58
                t60: i8 = srl t50, Constant:i64&lt;1&gt;
              t61: i8 = and t60, Constant:i8&lt;1&gt;
            t62: i1 = truncate t61
          t63: i16 = any_extend t62
              t64: i8 = and t50, Constant:i8&lt;1&gt;
            t65: i1 = truncate t64
          t66: i16 = any_extend t65
        t67: v4i16 = BUILD_VECTOR t55, t59, t63, t66
      t34: i32 = extract_vector_elt t67, Constant:i64&lt;2&gt;
    t31: i32 = and t34, Constant:i32&lt;1&gt;
  t23: ch,glue = CopyToReg t45, Register:i32 $w0, t31
  t50: i8,ch = load&lt;(dereferenceable load (s8) from %ir.matches), anyext from i4&gt; t45, FrameIndex:i64&lt;0&gt;, undef:i64
  t24: ch = AArch64ISD::RET_GLUE t23, Register:i32 $w0, t23:1
```
LE:
```
Vector-legalized selection DAG: %bb.0 'test4:'
SelectionDAG has 49 nodes:
    t0: ch,glue = EntryToken
  t4: ch = store&lt;(store (s64) into %ir.index_ptr)&gt; t0, Constant:i64&lt;0&gt;, FrameIndex:i64&lt;1&gt;, undef:i64
  t5: i64,ch = load&lt;(dereferenceable load (s64) from %ir.index_ptr)&gt; t4, FrameIndex:i64&lt;1&gt;, undef:i64
      t8: i64 = shl nsw t5, Constant:i64&lt;2&gt;
    t9: i64 = add GlobalAddress:i64&lt;ptr @<!-- -->haystack4&gt; 0, t8
  t10: v4i32,ch = load&lt;(load (s128) from %ir.haystack, align 4)&gt; t4, t9, undef:i64
    t16: ch = TokenFactor t5:1, t10:1
            t12: v4i32 = BUILD_VECTOR Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;, Constant:i32&lt;2&gt;
          t46: v4i32 = AArch64ISD::CMEQ t10, t12
          t40: v4i32 = BUILD_VECTOR Constant:i64&lt;1&gt;, Constant:i64&lt;2&gt;, Constant:i64&lt;4&gt;, Constant:i64&lt;8&gt;
        t41: v4i32 = and t46, t40
      t47: v4i32 = AArch64ISD::UADDV t41
    t48: i32 = extract_vector_elt t47, Constant:i64&lt;0&gt;
  t45: ch = store&lt;(store (s8) into %ir.matches), trunc to i8&gt; t16, t48, FrameIndex:i64&lt;0&gt;, undef:i64
              t51: i8 = and t50, Constant:i8&lt;1&gt;
            t52: i1 = truncate t51
          t53: i16 = any_extend t52
                t54: i8 = srl t50, Constant:i64&lt;1&gt;
              t55: i8 = and t54, Constant:i8&lt;1&gt;
            t56: i1 = truncate t55
          t57: i16 = any_extend t56
                t58: i8 = srl t50, Constant:i64&lt;2&gt;
              t59: i8 = and t58, Constant:i8&lt;1&gt;
            t60: i1 = truncate t59
          t61: i16 = any_extend t60
                t63: i8 = srl t50, Constant:i64&lt;3&gt;
              t64: i8 = and t63, Constant:i8&lt;1&gt;
            t65: i1 = truncate t64
          t66: i16 = any_extend t65
        t67: v4i16 = BUILD_VECTOR t53, t57, t61, t66
      t34: i32 = extract_vector_elt t67, Constant:i64&lt;2&gt;
    t31: i32 = and t34, Constant:i32&lt;1&gt;
  t23: ch,glue = CopyToReg t45, Register:i32 $w0, t31
  t50: i8,ch = load&lt;(dereferenceable load (s8) from %ir.matches), anyext from i4&gt; t45, FrameIndex:i64&lt;0&gt;, undef:i64
  t24: ch = AArch64ISD::RET_GLUE t23, Register:i32 $w0, t23:1
```
</details>


---

