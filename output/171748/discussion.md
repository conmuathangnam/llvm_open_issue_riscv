# [llvm][PowerPC] LLVM fails to build working Zig compiler for `powerpc64le-linux` due to bogus `bl` target address

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/171748
**Status:** Open
**Labels:** backend:PowerPC, miscompilation

## Body

I've been trying to bootstrap the Zig compiler for `powerpc64le-linux` so we can get native CI machines set up for this target. Unfortunately, the bootstrap compiler dies with a `SIGSEGV` due to a jump to some seemingly random address that happens to contain a zero word. (I'm not sure why the kernel produces `SIGSEGV` rather than `SIGILL` here, but that's besides the point.)

I have narrowed it down to a bogus `bl` instruction:

```
❯ objdump zig.o -r --disassemble=heap.c_allocator_impl.alloc

zig.o:     file format elf64-powerpcle


Disassembly of section .text:

00000000039ef320 <heap.c_allocator_impl.alloc>:
 39ef320:	00 00 4c 3c 	addis   r2,r12,0
			39ef320: R_PPC64_REL16_HA	.TOC.
 39ef324:	00 00 42 38 	addi    r2,r2,0
			39ef324: R_PPC64_REL16_LO	.TOC.+0x4
 39ef328:	a6 02 08 7c 	mflr    r0
 39ef32c:	f8 ff e1 fb 	std     r31,-8(r1)
 39ef330:	c1 fe 21 f8 	stdu    r1,-320(r1)
 39ef334:	50 01 01 f8 	std     r0,336(r1)
 39ef338:	78 0b 3f 7c 	mr      r31,r1
 39ef33c:	98 00 df f8 	std     r6,152(r31)
 39ef340:	90 00 bf f8 	std     r5,144(r31)
 39ef344:	78 23 86 7c 	mr      r6,r4
 39ef348:	98 00 9f e8 	ld      r4,152(r31)
 39ef34c:	a8 00 7f f8 	std     r3,168(r31)
 39ef350:	a4 00 9f 90 	stw     r4,164(r31)
 39ef354:	f0 8f 0d e9 	ld      r8,-28688(r13)
 39ef358:	30 01 1f f9 	std     r8,304(r31)
 39ef35c:	b8 00 df f8 	std     r6,184(r31)
 39ef360:	c0 00 bf f8 	std     r5,192(r31)
 39ef364:	be 06 84 54 	clrlwi  r4,r4,26
 39ef368:	cf 00 9f 98 	stb     r4,207(r31)
 39ef36c:	d0 00 ff f8 	std     r7,208(r31)
 39ef370:	df 00 9f 98 	stb     r4,223(r31)
 39ef374:	74 00 a4 7c 	cntlzd  r4,r5
 39ef378:	f8 20 84 7c 	not     r4,r4
 39ef37c:	e2 d7 84 78 	rldicl  r4,r4,58,63
 39ef380:	01 00 00 48 	bl      39ef380 <heap.c_allocator_impl.alloc+0x60>
			39ef380: R_PPC64_REL24	debug.assert
 39ef384:	a4 00 bf 80 	lwz     r5,164(r31)
 39ef388:	a8 00 7f e8 	ld      r3,168(r31)
 39ef38c:	78 2b a4 7c 	mr      r4,r5
 39ef390:	20 00 84 78 	clrldi  r4,r4,32
 39ef394:	01 00 00 48 	bl      39ef394 <heap.c_allocator_impl.alloc+0x74>
			39ef394: R_PPC64_REL24	heap.c_allocator_impl.allocStrat
 39ef398:	b4 00 7f 90 	stw     r3,180(r31)
 39ef39c:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3a0:	02 00 03 28 	cmplwi  r3,2
 39ef3a4:	f8 00 82 41 	beq     39ef49c <heap.c_allocator_impl.alloc+0x17c>
 39ef3a8:	04 00 00 48 	b       39ef3ac <heap.c_allocator_impl.alloc+0x8c>
 39ef3ac:	b4 00 7f 80 	lwz     r3,180(r31)
 39ef3b0:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3b4:	00 00 03 28 	cmplwi  r3,0
 39ef3b8:	1c 00 82 41 	beq     39ef3d4 <heap.c_allocator_impl.alloc+0xb4>
 39ef3bc:	04 00 00 48 	b       39ef3c0 <heap.c_allocator_impl.alloc+0xa0>
 39ef3c0:	b4 00 7f 80 	lwz     r3,180(r31)
 39ef3c4:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3c8:	01 00 03 28 	cmplwi  r3,1
 39ef3cc:	34 00 82 41 	beq     39ef400 <heap.c_allocator_impl.alloc+0xe0>
 39ef3d0:	cc 00 00 48 	b       39ef49c <heap.c_allocator_impl.alloc+0x17c>
 39ef3d4:	90 00 7f e8 	ld      r3,144(r31)
 39ef3d8:	08 00 23 28 	cmpldi  r3,8
 39ef3dc:	08 00 80 38 	li      r4,8
 39ef3e0:	5e 20 63 7c 	iselgt  r3,r3,r4
 39ef3e4:	e0 00 7f f8 	std     r3,224(r31)
 39ef3e8:	01 00 00 48 	bl      39ef3e8 <heap.c_allocator_impl.alloc+0xc8>
			39ef3e8: R_PPC64_REL24	malloc
 39ef3ec:	00 00 00 60 	nop
 39ef3f0:	88 00 7f f8 	std     r3,136(r31)
 39ef3f4:	00 00 23 2c 	cmpdi   r3,0
 39ef3f8:	14 01 82 40 	bne     39ef50c <heap.c_allocator_impl.alloc+0x1ec>
 39ef3fc:	1c 01 00 48 	b       39ef518 <heap.c_allocator_impl.alloc+0x1f8>
 39ef400:	a8 00 7f e8 	ld      r3,168(r31)
 39ef404:	df 00 bf 88 	lbz     r5,223(r31)
 39ef408:	78 2b a4 7c 	mr      r4,r5
 39ef40c:	20 00 84 78 	clrldi  r4,r4,32
 39ef410:	d1 ae 72 48 	bl      411a2e0 <hash_map.HashMapUnmanaged([]const u8,codegen.x86_64.CodeGen.airAsm.Label,hash_map.StringContext,80).dbHelper+0x6dd7b0>
 39ef414:	90 00 bf e8     ld      r5,144(r31)
...snip...
```

That `bl` at `39ef410` should actually be calling `mem.Alignment.toByteUnits`, and indeed that is what you see if you look at the assembly output:

```
    .p2align    4
    .type   heap.c_allocator_impl.alloc,@function
heap.c_allocator_impl.alloc:
.Lfunc_begin69258:
    .loc    13 223 0 is_stmt 1
    .cfi_startproc
.Lfunc_gep69258:
    addis 2, 12, .TOC.-.Lfunc_gep69258@ha
    addi 2, 2, .TOC.-.Lfunc_gep69258@l
.Lfunc_lep69258:
    .localentry heap.c_allocator_impl.alloc, .Lfunc_lep69258-.Lfunc_gep69258
    mflr 0
    std 31, -8(1)
    stdu 1, -336(1)
    std 0, 352(1)
    .cfi_def_cfa_offset 336
    .cfi_offset r31, -8
    .cfi_offset lr, 16
    mr  31, 1
    .cfi_def_cfa_register r31
    std 6, 160(31)
    std 5, 152(31)
    mr  6, 4
    ld 4, 160(31)
    std 3, 176(31)
    stw 4, 172(31)
    ld 8, -28688(13)
    std 8, 256(31)
    ld 8, -28688(13)
    std 8, 320(31)
.Ltmp1953179:
    .loc    13 223 14 prologue_end
    std 6, 192(31)
    std 5, 200(31)
    clrlwi  4, 4, 26
    stb 4, 215(31)
    std 7, 216(31)
    stb 4, 231(31)
    .loc    13 225 15
    cntlzd 4, 5
    not 4, 4
    rldicl 4, 4, 58, 63
    bl debug.assert
    lwz 5, 172(31)
    ld 3, 176(31)
    .loc    13 226 27
    mr  4, 5
    clrldi  4, 4, 32
    bl heap.c_allocator_impl.allocStrat
    stw 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17 is_stmt 0
    cmplwi  3, 2
    beq 0, .LBB69258_5
    b .LBB69258_1
.LBB69258_1:
    .loc    13 0 17
    lwz 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17
    cmplwi  3, 0
    beq 0, .LBB69258_3
    b .LBB69258_2
.LBB69258_2:
    .loc    13 0 17
    lwz 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17
    cmplwi  3, 1
    beq 0, .LBB69258_4
    b .LBB69258_6
.LBB69258_3:
.Ltmp1953180:
    .loc    13 241 17 is_stmt 1
    ld 3, 152(31)
    cmpldi  3, 8
    li 4, 8
    iselgt  3, 3, 4
    std 3, 232(31)
.Ltmp1953181:
    .loc    13 242 37
    bl malloc
    nop
    std 3, 144(31)
    cmpdi   3, 0
    bne 0, .LBB69258_10
    b .LBB69258_11
.Ltmp1953182:
.LBB69258_4:
    .loc    13 249 71
    ld 3, 176(31)
    lbz 5, 231(31)
    mr  4, 5
    clrldi  4, 4, 32
    bl mem.Alignment.toByteUnits
    ld 5, 152(31)
...snip...
```

Yet it's clearly not calling that function in the object file, and also lacks a relocation.

Interestingly, here's where that function is actually located:

```
❯ nm zig.o | grep mem.Alignment.toByteUnits
000000000011a2e0 t mem.Alignment.toByteUnits
```

So out of `bl` range by a fairly large margin.

Relevant repro files are available here: https://files.alexrp.com/zig.tar.xz

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Alex Rønne Petersen (alexrp)

<details>
I've been trying to bootstrap the Zig compiler for `powerpc64le-linux` so we can get native CI machines set up for this target. Unfortunately, the bootstrap compiler dies with a `SIGSEGV` due to a jump to some seemingly random address that happens to contain a zero word. (I'm not sure why the kernel produces `SIGSEGV` rather than `SIGILL` here, but that's besides the point.)

I have narrowed it down to a bogus `bl` instruction:

```
❯ objdump zig.o -r --disassemble=heap.c_allocator_impl.alloc

zig.o:     file format elf64-powerpcle


Disassembly of section .text:

00000000039ef320 &lt;heap.c_allocator_impl.alloc&gt;:
 39ef320:	00 00 4c 3c 	addis   r2,r12,0
			39ef320: R_PPC64_REL16_HA	.TOC.
 39ef324:	00 00 42 38 	addi    r2,r2,0
			39ef324: R_PPC64_REL16_LO	.TOC.+0x4
 39ef328:	a6 02 08 7c 	mflr    r0
 39ef32c:	f8 ff e1 fb 	std     r31,-8(r1)
 39ef330:	c1 fe 21 f8 	stdu    r1,-320(r1)
 39ef334:	50 01 01 f8 	std     r0,336(r1)
 39ef338:	78 0b 3f 7c 	mr      r31,r1
 39ef33c:	98 00 df f8 	std     r6,152(r31)
 39ef340:	90 00 bf f8 	std     r5,144(r31)
 39ef344:	78 23 86 7c 	mr      r6,r4
 39ef348:	98 00 9f e8 	ld      r4,152(r31)
 39ef34c:	a8 00 7f f8 	std     r3,168(r31)
 39ef350:	a4 00 9f 90 	stw     r4,164(r31)
 39ef354:	f0 8f 0d e9 	ld      r8,-28688(r13)
 39ef358:	30 01 1f f9 	std     r8,304(r31)
 39ef35c:	b8 00 df f8 	std     r6,184(r31)
 39ef360:	c0 00 bf f8 	std     r5,192(r31)
 39ef364:	be 06 84 54 	clrlwi  r4,r4,26
 39ef368:	cf 00 9f 98 	stb     r4,207(r31)
 39ef36c:	d0 00 ff f8 	std     r7,208(r31)
 39ef370:	df 00 9f 98 	stb     r4,223(r31)
 39ef374:	74 00 a4 7c 	cntlzd  r4,r5
 39ef378:	f8 20 84 7c 	not     r4,r4
 39ef37c:	e2 d7 84 78 	rldicl  r4,r4,58,63
 39ef380:	01 00 00 48 	bl      39ef380 &lt;heap.c_allocator_impl.alloc+0x60&gt;
			39ef380: R_PPC64_REL24	debug.assert
 39ef384:	a4 00 bf 80 	lwz     r5,164(r31)
 39ef388:	a8 00 7f e8 	ld      r3,168(r31)
 39ef38c:	78 2b a4 7c 	mr      r4,r5
 39ef390:	20 00 84 78 	clrldi  r4,r4,32
 39ef394:	01 00 00 48 	bl      39ef394 &lt;heap.c_allocator_impl.alloc+0x74&gt;
			39ef394: R_PPC64_REL24	heap.c_allocator_impl.allocStrat
 39ef398:	b4 00 7f 90 	stw     r3,180(r31)
 39ef39c:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3a0:	02 00 03 28 	cmplwi  r3,2
 39ef3a4:	f8 00 82 41 	beq     39ef49c &lt;heap.c_allocator_impl.alloc+0x17c&gt;
 39ef3a8:	04 00 00 48 	b       39ef3ac &lt;heap.c_allocator_impl.alloc+0x8c&gt;
 39ef3ac:	b4 00 7f 80 	lwz     r3,180(r31)
 39ef3b0:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3b4:	00 00 03 28 	cmplwi  r3,0
 39ef3b8:	1c 00 82 41 	beq     39ef3d4 &lt;heap.c_allocator_impl.alloc+0xb4&gt;
 39ef3bc:	04 00 00 48 	b       39ef3c0 &lt;heap.c_allocator_impl.alloc+0xa0&gt;
 39ef3c0:	b4 00 7f 80 	lwz     r3,180(r31)
 39ef3c4:	be 07 63 54 	clrlwi  r3,r3,30
 39ef3c8:	01 00 03 28 	cmplwi  r3,1
 39ef3cc:	34 00 82 41 	beq     39ef400 &lt;heap.c_allocator_impl.alloc+0xe0&gt;
 39ef3d0:	cc 00 00 48 	b       39ef49c &lt;heap.c_allocator_impl.alloc+0x17c&gt;
 39ef3d4:	90 00 7f e8 	ld      r3,144(r31)
 39ef3d8:	08 00 23 28 	cmpldi  r3,8
 39ef3dc:	08 00 80 38 	li      r4,8
 39ef3e0:	5e 20 63 7c 	iselgt  r3,r3,r4
 39ef3e4:	e0 00 7f f8 	std     r3,224(r31)
 39ef3e8:	01 00 00 48 	bl      39ef3e8 &lt;heap.c_allocator_impl.alloc+0xc8&gt;
			39ef3e8: R_PPC64_REL24	malloc
 39ef3ec:	00 00 00 60 	nop
 39ef3f0:	88 00 7f f8 	std     r3,136(r31)
 39ef3f4:	00 00 23 2c 	cmpdi   r3,0
 39ef3f8:	14 01 82 40 	bne     39ef50c &lt;heap.c_allocator_impl.alloc+0x1ec&gt;
 39ef3fc:	1c 01 00 48 	b       39ef518 &lt;heap.c_allocator_impl.alloc+0x1f8&gt;
 39ef400:	a8 00 7f e8 	ld      r3,168(r31)
 39ef404:	df 00 bf 88 	lbz     r5,223(r31)
 39ef408:	78 2b a4 7c 	mr      r4,r5
 39ef40c:	20 00 84 78 	clrldi  r4,r4,32
 39ef410:	d1 ae 72 48 	bl      411a2e0 &lt;hash_map.HashMapUnmanaged([]const u8,codegen.x86_64.CodeGen.airAsm.Label,hash_map.StringContext,80).dbHelper+0x6dd7b0&gt;
 39ef414:       90 00 bf e8     ld      r5,144(r31)
...snip...
```

That `bl` at `39ef410` should actually be calling `mem.Alignment.toByteUnits`, and indeed that is what you see if you look at the assembly output:

```
    .p2align    4
    .type   heap.c_allocator_impl.alloc,@<!-- -->function
heap.c_allocator_impl.alloc:
.Lfunc_begin69258:
    .loc    13 223 0 is_stmt 1
    .cfi_startproc
.Lfunc_gep69258:
    addis 2, 12, .TOC.-.Lfunc_gep69258@<!-- -->ha
    addi 2, 2, .TOC.-.Lfunc_gep69258@<!-- -->l
.Lfunc_lep69258:
    .localentry heap.c_allocator_impl.alloc, .Lfunc_lep69258-.Lfunc_gep69258
    mflr 0
    std 31, -8(1)
    stdu 1, -336(1)
    std 0, 352(1)
    .cfi_def_cfa_offset 336
    .cfi_offset r31, -8
    .cfi_offset lr, 16
    mr  31, 1
    .cfi_def_cfa_register r31
    std 6, 160(31)
    std 5, 152(31)
    mr  6, 4
    ld 4, 160(31)
    std 3, 176(31)
    stw 4, 172(31)
    ld 8, -28688(13)
    std 8, 256(31)
    ld 8, -28688(13)
    std 8, 320(31)
.Ltmp1953179:
    .loc    13 223 14 prologue_end
    std 6, 192(31)
    std 5, 200(31)
    clrlwi  4, 4, 26
    stb 4, 215(31)
    std 7, 216(31)
    stb 4, 231(31)
    .loc    13 225 15
    cntlzd 4, 5
    not 4, 4
    rldicl 4, 4, 58, 63
    bl debug.assert
    lwz 5, 172(31)
    ld 3, 176(31)
    .loc    13 226 27
    mr  4, 5
    clrldi  4, 4, 32
    bl heap.c_allocator_impl.allocStrat
    stw 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17 is_stmt 0
    cmplwi  3, 2
    beq 0, .LBB69258_5
    b .LBB69258_1
.LBB69258_1:
    .loc    13 0 17
    lwz 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17
    cmplwi  3, 0
    beq 0, .LBB69258_3
    b .LBB69258_2
.LBB69258_2:
    .loc    13 0 17
    lwz 3, 188(31)
    clrlwi  3, 3, 30
    .loc    13 226 17
    cmplwi  3, 1
    beq 0, .LBB69258_4
    b .LBB69258_6
.LBB69258_3:
.Ltmp1953180:
    .loc    13 241 17 is_stmt 1
    ld 3, 152(31)
    cmpldi  3, 8
    li 4, 8
    iselgt  3, 3, 4
    std 3, 232(31)
.Ltmp1953181:
    .loc    13 242 37
    bl malloc
    nop
    std 3, 144(31)
    cmpdi   3, 0
    bne 0, .LBB69258_10
    b .LBB69258_11
.Ltmp1953182:
.LBB69258_4:
    .loc    13 249 71
    ld 3, 176(31)
    lbz 5, 231(31)
    mr  4, 5
    clrldi  4, 4, 32
    bl mem.Alignment.toByteUnits
    ld 5, 152(31)
...snip...
```

Yet it's clearly not calling that function in the object file, and also lacks a relocation.

Interestingly, here's where that function is actually located:

```
❯ nm zig.o | grep mem.Alignment.toByteUnits
000000000011a2e0 t mem.Alignment.toByteUnits
```

So out of `bl` range by a fairly large margin.

Relevant repro files are available here: https://files.alexrp.com/zig.tar.xz
</details>


---

### Comment 2 - nikic

After https://github.com/llvm/llvm-project/pull/165859 this produces
```
<unknown>:0: error: branch target out of range (-33554472 not between -33554432 and 33554428)
```
errors instead of a silent miscompile...

---

### Comment 3 - nikic

From a cursory look, it's not really clear to me where the PPC backend handles large jumps at all. A search for "relax" in the PPC MC backend comes up empty.

Possibly a br24 fixup overflow should force emission of a relocation rather than erroring? I assume the linker does handle this correctly.

(I don't plan to work on this.)

---

### Comment 4 - alexrp

As I understand it, the way this is supposed to work is that the compiler should emit a relocation on the `bl` so that the linker can produce a range extension thunk if necessary - and LLD does indeed have support for that.

You can see that the disassembly contains exactly this for other internal calls, and indeed they link and run just fine:

```
 39ef380:	01 00 00 48 	bl      39ef380 <heap.c_allocator_impl.alloc+0x60>
			39ef380: R_PPC64_REL24	debug.assert
 39ef384:	a4 00 bf 80 	lwz     r5,164(r31)
 39ef388:	a8 00 7f e8 	ld      r3,168(r31)
 39ef38c:	78 2b a4 7c 	mr      r4,r5
 39ef390:	20 00 84 78 	clrldi  r4,r4,32
 39ef394:	01 00 00 48 	bl      39ef394 <heap.c_allocator_impl.alloc+0x74>
			39ef394: R_PPC64_REL24	heap.c_allocator_impl.allocStrat
```

So why not this particular `bl`? 🤷

---

### Comment 5 - alexrp

FWIW, this hack produces an apparently working compiler:

```diff
diff --git a/llvm/lib/Target/PowerPC/MCTargetDesc/PPCAsmBackend.cpp b/llvm/lib/Target/PowerPC/MCTargetDesc/PPCAsmBackend.cpp
index 5779d4e8c..9691daed5 100644
--- a/llvm/lib/Target/PowerPC/MCTargetDesc/PPCAsmBackend.cpp
+++ b/llvm/lib/Target/PowerPC/MCTargetDesc/PPCAsmBackend.cpp
@@ -113,12 +113,7 @@ public:
       // resolution of the final target address to the linker.
       if (const auto *A = Target.getAddSym()) {
         if (const auto *S = dyn_cast<MCSymbolELF>(A)) {
-          // The "other" values are stored in the last 6 bits of the second
-          // byte. The traditional defines for STO values assume the full byte
-          // and thus the shift to pack it.
-          unsigned Other = S->getOther() << 2;
-          if ((Other & ELF::STO_PPC64_LOCAL_MASK) != 0)
-            return true;
+          return true;
         } else if (const auto *S = dyn_cast<MCSymbolXCOFF>(A)) {
           return !Target.isAbsolute() && S->isExternal() &&
                  S->getStorageClass() == XCOFF::C_WEAKEXT;
diff --git a/llvm/lib/Target/PowerPC/MCTargetDesc/PPCELFObjectWriter.cpp b/llvm/lib/Target/PowerPC/MCTargetDesc/PPCELFObjectWriter.cpp
index a5d3be40c..0e8482497 100644
--- a/llvm/lib/Target/PowerPC/MCTargetDesc/PPCELFObjectWriter.cpp
+++ b/llvm/lib/Target/PowerPC/MCTargetDesc/PPCELFObjectWriter.cpp
@@ -494,13 +494,7 @@ bool PPCELFObjectWriter::needsRelocateWithSymbol(const MCValue &V,
 
     case ELF::R_PPC_REL24:
     case ELF::R_PPC64_REL24_NOTOC: {
-      // If the target symbol has a local entry point, we must keep the
-      // target symbol to preserve that information for the linker.
-      // The "other" values are stored in the last 6 bits of the second byte.
-      // The traditional defines for STO values assume the full byte and thus
-      // the shift to pack it.
-      unsigned Other = cast<MCSymbolELF>(V.getAddSym())->getOther() << 2;
-      return (Other & ELF::STO_PPC64_LOCAL_MASK) != 0;
+      return true;
     }
 
     case ELF::R_PPC64_GOT16:
```

I'm not yet familiar enough with the details here to say why or whether this patch is at least on the right trajectory.

---

### Comment 6 - efriedma-quic

If a single section in an object file is larger than what you can encode in a branch immediate (in this case 32MB, I guess), you're going to have problems: the LLVM assembler doesn't know how to split sections, and it doesn't know how to insert range extension thunks.

Adding relocations doesn't really solve the issue: in general, if the section is too large, the linker can't insert a range extension thunk in range of the call.

The easy workaround is to enable function sections.

---

### Comment 7 - alexrp

> If a single section in an object file is larger than what you can encode in a branch immediate (in this case 32MB, I guess), you're going to have problems: the LLVM assembler doesn't know how to split sections, and it doesn't know how to insert range extension thunks.

This makes sense to me in isolation. However:

> Adding relocations doesn't really solve the issue: in general, if the section is too large, the linker can't insert a range extension thunk in range of the call.

This leaves me wondering why the hack above actually produces a working compiler then? 🤔

---

### Comment 8 - efriedma-quic

See also #23373, which was solved by enabling function sections by default for LTO.

---

### Comment 9 - efriedma-quic

> > Adding relocations doesn't really solve the issue: in general, if the section is too large, the linker can't insert a range extension thunk in range of the call.
> 
> This leaves me wondering why the hack above actually produces a working compiler then? 🤔

If your section is large enough that you can't branch from one end to the other, but small enough that you can branch out of the section, the linker can insert range extension thunks before and after the section, I guess.

---

### Comment 10 - alexrp

FWIW, I can confirm that applying `-ffunction-sections -fdata-sections` to all compilation units involved resolves this particular issue. Much better than the hack above, too.

So, is this issue actually actionable? Should I leave it open to track an enhancement in LLVM to support large jumps as seen here?

---

### Comment 11 - efriedma-quic

We no longer silently miscompile.

In theory we could implement an assembler directive to denote legal locations to insert range extension thunks, and teach the assembler to actually emit such thunks when necessary. But there's no incentive for anyone to implement the complex assembler infrastructure necessary for that, given it would almost never be used.

We could consider turning on -ffunction-sections/-fdata-sections by default.  There's not really much reason to have them off.

---

### Comment 12 - nemanjai

I would suggest not doing anything here other than perhaps adding a suggestion to use `-ffunction-sections` to the error that we emit when the target is out of range.

---

