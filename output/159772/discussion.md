# [clang] Crash on aarch64 bf16 SIMD

**Author:** jan-wassenberg
**URL:** https://github.com/llvm/llvm-project/issues/159772
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2025-09-29T06:24:58Z

## Body

```
fatal error: error in backend: Cannot select: 0x703cbd93e700: v4bf16 = bitcast 0x703cba89b230
  0x703cba89b230: v4f16 = insert_vector_elt 0x703cbe83b460, 0x703cb942cc40, Constant:i64<2>
    0x703cbe83b460: v4f16 = bitcast 0x703cbe82f7e0
      0x703cbe82f7e0: v4i16 = AArch64ISD::ZIP1 0x703cbe83bd90, 0x703cb942cf50
        0x703cbe83bd90: v4i16 = scalar_to_vector 0x703cba89baf0
          0x703cba89baf0: i32,ch = load<(load (s16) from %ir.1830, !alias.scope !19723), anyext from i16> 0x703cbb931d08, 0x703cbd275070, undef:i64
            0x703cbd275070: i64,ch = CopyFromReg 0x703cbb931d08, Register:i64 %568
        0x703cb942cf50: v4i16 = BUILD_VECTOR Constant:i32<0>, Constant:i32<0>, undef:i32, undef:i32
    0x703cb942cc40: f16 = extract_vector_elt 0x703cbd275d90, Constant:i64<0>
      0x703cbd275d90: v8f16 = bitcast 0x703cbe82f8c0
        0x703cbe82f8c0: v8i16 = insert_subvector undef:v8i16, 0x703cbe83bd90, Constant:i64<0>
          0x703cbe83bd90: v4i16 = scalar_to_vector 0x703cba89baf0
            0x703cba89baf0: i32,ch = load<(load (s16) from %ir.1830, !alias.scope !19723), anyext from i16> 0x703cbb931d08, 0x703cbd275070, undef:i64
              0x703cbd275070: i64,ch = CopyFromReg 0x703cbb931d08, Register:i64 %568
In function: _ZN3hwy11N_NEON_BF166detail14ForeachCappedRINS_10bfloat16_tELm2ELm2ENS0_12_GLOBAL__N_17TestDotELi0EE2DoEmm
```

This comes after replacing a scalar loop with masked vector code in https://github.com/google/highway/pull/2703.
The code being patched is https://github.com/google/highway/blob/master/hwy/contrib/dot/dot-inl.h.

Compilation is for aarch64, optimized build, and NEON with BF16, hence something like armv9-a+sve2-aes. (Arm 8.6 may be sufficient).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Jan Wassenberg (jan-wassenberg)

<details>
```
fatal error: error in backend: Cannot select: 0x703cbd93e700: v4bf16 = bitcast 0x703cba89b230
  0x703cba89b230: v4f16 = insert_vector_elt 0x703cbe83b460, 0x703cb942cc40, Constant:i64&lt;2&gt;
    0x703cbe83b460: v4f16 = bitcast 0x703cbe82f7e0
      0x703cbe82f7e0: v4i16 = AArch64ISD::ZIP1 0x703cbe83bd90, 0x703cb942cf50
        0x703cbe83bd90: v4i16 = scalar_to_vector 0x703cba89baf0
          0x703cba89baf0: i32,ch = load&lt;(load (s16) from %ir.1830, !alias.scope !19723), anyext from i16&gt; 0x703cbb931d08, 0x703cbd275070, undef:i64
            0x703cbd275070: i64,ch = CopyFromReg 0x703cbb931d08, Register:i64 %568
        0x703cb942cf50: v4i16 = BUILD_VECTOR Constant:i32&lt;0&gt;, Constant:i32&lt;0&gt;, undef:i32, undef:i32
    0x703cb942cc40: f16 = extract_vector_elt 0x703cbd275d90, Constant:i64&lt;0&gt;
      0x703cbd275d90: v8f16 = bitcast 0x703cbe82f8c0
        0x703cbe82f8c0: v8i16 = insert_subvector undef:v8i16, 0x703cbe83bd90, Constant:i64&lt;0&gt;
          0x703cbe83bd90: v4i16 = scalar_to_vector 0x703cba89baf0
            0x703cba89baf0: i32,ch = load&lt;(load (s16) from %ir.1830, !alias.scope !19723), anyext from i16&gt; 0x703cbb931d08, 0x703cbd275070, undef:i64
              0x703cbd275070: i64,ch = CopyFromReg 0x703cbb931d08, Register:i64 %568
In function: _ZN3hwy11N_NEON_BF166detail14ForeachCappedRINS_10bfloat16_tELm2ELm2ENS0_12_GLOBAL__N_17TestDotELi0EE2DoEmm
```

This comes after replacing a scalar loop with masked vector code in https://github.com/google/highway/pull/2703.
The code being patched is https://github.com/google/highway/blob/master/hwy/contrib/dot/dot-inl.h.

Compilation is for aarch64, optimized build, and NEON with BF16, hence something like armv9-a+sve2-aes. (Arm 8.6 may be sufficient).
</details>


---

### Comment 2 - vekterli

A somewhat reduced (but still dependent on `<hwy/highway.h>`) test case reproducing the crash: https://godbolt.org/z/jMTn9vWz7

Compiles OK on 19.1.0, fails on 20.1.0...trunk

---

### Comment 3 - davemgreen

Oh, that's kind of surprising that we haven't ran into it already.
llvm reproducer from above: https://godbolt.org/z/5bo9TT59s
And a smaller version: https://godbolt.org/z/Yenh3Weav

---

