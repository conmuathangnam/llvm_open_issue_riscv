# [X86] Incorrect handling for truncated masked store

**Author:** JaydeepChauhan14
**URL:** https://github.com/llvm/llvm-project/issues/179489
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2026-02-05T13:06:19Z

## Body

**testcase**
```llvm
define void @foo(<8 x i16> %arg, ptr addrspace(1) %add.ptr, i64 %dim0) {
entry:
  %i3 = shufflevector <8 x i16> %arg, <8 x i16> <i16 poison, i16 257, i16 257, i16 257, i16 257, i16 257, i16 257, i16 257>, <8 x i32> <i32 0, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
  %i4 = lshr <8 x i16> %i3, splat (i16 8)
  %i5 = trunc <8 x i16> %i4 to <8 x i8>
  call void @llvm.masked.store.v8i8.p1(<8 x i8> %i5, ptr addrspace(1) %add.ptr, <8 x i1> <i1 true, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false>)
  ret void
}
```
**Assembly with Bug**
```nasm
        vpextrw $0, %xmm0, %eax
        shrl    $8, %eax
        movw    %ax, (%rdi)
        retq
```
**Correct assembly**
```nasm
        vbroadcastss    .LCPI0_1(%rip), %xmm1   # xmm1 = [257,257,257,257,257,257,257,257]
        vmovsh  %xmm0, %xmm1, %xmm0             # xmm0 = xmm0[0],xmm1[1,2,3,4,5,6,7]
        vpsrlw  $8, %xmm0, %xmm0
        movb    $1, %al
        kmovd   %eax, %k1
        vpmovwb %xmm0, (%rdi) {%k1}
```
- In Bug case we extract 16 bit from xmm0[0], shift right by 8 (bit 0-7 valid, 8-16 zero) and write 16 bits at ptr.
In correct case shift all elements of xmm0 by 8 bits to right, and then writes just a 8-bit like intended in IR.

**Reference PR** - https://github.com/llvm/llvm-project/pull/169827


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (JaydeepChauhan14)

<details>
**testcase**
```
define void @<!-- -->foo(&lt;8 x i16&gt; %arg, ptr addrspace(1) %add.ptr, i64 %dim0) {
entry:
  %i3 = shufflevector &lt;8 x i16&gt; %arg, &lt;8 x i16&gt; &lt;i16 poison, i16 257, i16 257, i16 257, i16 257, i16 257, i16 257, i16 257&gt;, &lt;8 x i32&gt; &lt;i32 0, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15&gt;
  %i4 = lshr &lt;8 x i16&gt; %i3, splat (i16 8)
  %i5 = trunc &lt;8 x i16&gt; %i4 to &lt;8 x i8&gt;
  call void @<!-- -->llvm.masked.store.v8i8.p1(&lt;8 x i8&gt; %i5, ptr addrspace(1) %add.ptr, &lt;8 x i1&gt; &lt;i1 true, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false&gt;)
  ret void
}
```
**Assembly with Bug**
```
        vpextrw $0, %xmm0, %eax
        shrl    $8, %eax
        movw    %ax, (%rdi)
        retq
```
**Correct assembly**
```
        vbroadcastss    .LCPI0_1(%rip), %xmm1   # xmm1 = [257,257,257,257,257,257,257,257]
        vmovsh  %xmm0, %xmm1, %xmm0             # xmm0 = xmm0[0],xmm1[1,2,3,4,5,6,7]
        vpsrlw  $8, %xmm0, %xmm0
        movb    $1, %al
        kmovd   %eax, %k1
        vpmovwb %xmm0, (%rdi) {%k1}
```
- In Bug case we extract 16 bit from xmm0[0], shift right by 8 (bit 0-7 valid, 8-16 zero) and write 16 bits at ptr.
In correct case shift all elements of xmm0 by 8 bits to right, and then writes just a 8-bit like intended in IR.

**Reference PR** - https://github.com/llvm/llvm-project/pull/169827

</details>


---

### Comment 2 - phoebewang

Not sure if I understood it correctly, but the "Correct assembly" doesn't look correct to me. `vpmovwb` doesn't just write 8-bit. It writes 16-bit in fact, which doesn't fit the IR.

Isn't the correct assembly:
```assembly
        vpextrw $0, %xmm0, %eax
        shrl    $8, %eax
        movb    %al, (%rdi)
        retq
```

---

### Comment 3 - phoebewang

Oh, I misunderstood it. `vpmovwb` indeed writes 8-bit. It just not as efficient as the scalar code.

---

### Comment 4 - JaydeepChauhan14

Issue here is current masked store example https://github.com/llvm/llvm-project/issues/179489#issue-3892006379 is **truncating store**. But as **reduceMaskedStoreToScalarStore** is shifted above **isTruncatingStore** in https://github.com/llvm/llvm-project/pull/169827, **isTruncatingStore** become unused/redundant and it is performing incorrect combine. 

---

### Comment 5 - phoebewang

Maybe teach `reduceMaskedStoreToScalarStore` to handle TruncatingStore?

---

