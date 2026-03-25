# [AArch64] Miscompile due to 32-bit insertelement lowered to 8-bit move

**Author:** aleks-tmb
**URL:** https://github.com/llvm/llvm-project/issues/140707
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-05-20T17:09:19Z

## Body

After the changes introduced in [#136091](https://github.com/llvm/llvm-project/pull/136091), we started experiencing a miscompile on AArch64 in our local testing. Here is a reduced LLVM IR example:

```llvm
; ModuleID = 'Test.ll'
target triple = "aarch64-none-linux-gnu"

define i32 @main(ptr addrspace(1) %p) {
  %1 = load <2 x i32>, ptr addrspace(1) %p, align 4
  %2 = extractelement <2 x i32> %1, i64 0
  %3 = call i32 @llvm.ctpop.i32(i32 %2)
  %4 = insertelement <2 x i32> <i32 -1, i32 poison>, i32 %3, i64 1
  %5 = sub <2 x i32> %1, %4
  store <2 x i32> %5, ptr addrspace(1) %p, align 4
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.ctpop.i32(i32)
```
Here is an `llc (trunk)` output:
```s
main:
        ldr     d0, [x0]
        movi    v2.2d, #0xffffffffffffffff
        mov     x8, x0
        mov     w0, wzr
        fmov    w9, s0
        fmov    s1, w9
        cnt     v1.8b, v1.8b
        addv    b1, v1.8b
        mov     v2.b[4], v1.b[0]
        sub     v0.2s, v0.2s, v2.2s
        str     d0, [x8]
        ret
```
https://godbolt.org/z/c5YP7rYbE

Current transformation seems to be incorrect because:

- The instruction `mov v2.b[4], v1.b[0]` only updates a single byte (byte 4) of the v2 vector register.
- However, the LLVM IR expects a full 32-bit insertion into the second element (insertelement at index 1).
- Because the rest of the 32-bit lane in v2.s[1] remains filled with 0xFF (due to `movi v2.2d, #0xFFFFFFFFFFFFFFFF`), the resulting subtraction computes an incorrect value.

`llc 20.1.0` output (before applying #136091)
```s
main:                                   // @main
        ldr     d1, [x0]
        movi    v0.2d, #0xffffffffffffffff
        mov     x8, x0
        mov     w0, wzr
        fmov    w9, s1
        fmov    s2, w9
        cnt     v2.8b, v2.8b
        addv    b2, v2.8b
        fmov    w9, s2
        mov     v0.s[1], w9
        sub     v0.2s, v1.2s, v0.2s
        str     d0, [x8]
        ret
```
https://godbolt.org/z/qqvM1b3hc

Why this is correct?
- `mov v0.s[1], w9` fully overwrites the entire 32-bit lane in v0, matching the semantics of LLVM IR's insertelement.
- This avoids leftover bytes from the earlier `movi` initialization, ensuring the result is correct.

@davemgreen David, could you please take a look when you have a moment?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Aleksandr Popov (aleks-tmb)

<details>
After the changes introduced in [#<!-- -->136091](https://github.com/llvm/llvm-project/pull/136091), we started experiencing a miscompile on AArch64 in our local testing. Here is a reduced LLVM IR example:

```llvm
; ModuleID = 'Test.ll'
target triple = "aarch64-none-linux-gnu"

define i32 @<!-- -->main(ptr addrspace(1) %p) {
  %1 = load &lt;2 x i32&gt;, ptr addrspace(1) %p, align 4
  %2 = extractelement &lt;2 x i32&gt; %1, i64 0
  %3 = call i32 @<!-- -->llvm.ctpop.i32(i32 %2)
  %4 = insertelement &lt;2 x i32&gt; &lt;i32 -1, i32 poison&gt;, i32 %3, i64 1
  %5 = sub &lt;2 x i32&gt; %1, %4
  store &lt;2 x i32&gt; %5, ptr addrspace(1) %p, align 4
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.ctpop.i32(i32)
```
Here is an `llc (trunk)` output:
```s
main:
        ldr     d0, [x0]
        movi    v2.2d, #<!-- -->0xffffffffffffffff
        mov     x8, x0
        mov     w0, wzr
        fmov    w9, s0
        fmov    s1, w9
        cnt     v1.8b, v1.8b
        addv    b1, v1.8b
        mov     v2.b[4], v1.b[0]
        sub     v0.2s, v0.2s, v2.2s
        str     d0, [x8]
        ret
```
https://godbolt.org/z/c5YP7rYbE

Current transformation seems to be incorrect because:

- The instruction `mov v2.b[4], v1.b[0]` only updates a single byte (byte 4) of the v2 vector register.
- However, the LLVM IR expects a full 32-bit insertion into the second element (insertelement at index 1).
- Because the rest of the 32-bit lane in v2.s[1] remains filled with 0xFF (due to `movi v2.2d, #<!-- -->0xFFFFFFFFFFFFFFFF`), the resulting subtraction computes an incorrect value.

`llc 20.1.0` output (before applying #<!-- -->136091)
```s
main:                                   // @<!-- -->main
        ldr     d1, [x0]
        movi    v0.2d, #<!-- -->0xffffffffffffffff
        mov     x8, x0
        mov     w0, wzr
        fmov    w9, s1
        fmov    s2, w9
        cnt     v2.8b, v2.8b
        addv    b2, v2.8b
        fmov    w9, s2
        mov     v0.s[1], w9
        sub     v0.2s, v1.2s, v0.2s
        str     d0, [x8]
        ret
```
https://godbolt.org/z/qqvM1b3hc

Why this is correct?
- `mov v0.s[1], w9` fully overwrites the entire 32-bit lane in v0, matching the semantics of LLVM IR's insertelement.
- This avoids leftover bytes from the earlier `movi` initialization, ensuring the result is correct.
</details>


---

### Comment 2 - davemgreen

Thanks for the report - it was a well made ticket.

The problem appears to be the ctpop->UADDV which isn't accounting for the extract being an any-ext. It should be simple enough to switch it over to a cast and a i32 extract.

---

### Comment 3 - davemgreen

Please let us know if this is not fixed. Thanks.

---

