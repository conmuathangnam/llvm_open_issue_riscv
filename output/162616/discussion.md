# [AArch64] Cannot select AArch64ISD::SRAD_MERGE_OP1

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/162616
**Status:** Closed
**Labels:** backend:AArch64, SVE, crash
**Closed Date:** 2025-10-10T11:37:44Z

## Body

Compiling this:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define <16 x i16> @foo(<16 x i16> %0) #0 {
  %2 = shufflevector <16 x i16> zeroinitializer, <16 x i16> splat (i16 1), <16 x i32> <i32 0, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31>
  %3 = sdiv <16 x i16> %0, %2
  ret <16 x i16> %3
}

attributes #0 = { "target-cpu"="neoverse-v2" }
```

with `llc test.ll` results in:

```
LLVM ERROR: Cannot select: 0xaaaaae3d6b50: nxv8i16 = AArch64ISD::SRAD_MERGE_OP1 0xaaaaae3d6ae0, 0xaaaaae3d6990, TargetConstant:i32<0>
  0xaaaaae3d6ae0: nxv8i1 = AArch64ISD::PTRUE TargetConstant:i32<8>
  0xaaaaae3d6990: nxv8i16 = insert_subvector undef:nxv8i16, 0xaaaaae3d6920, Constant:i64<0>
    0xaaaaae3d6920: v8i16,ch = CopyFromReg 0xaaaaae35e3e0, Register:v8i16 %1
In function: foo
```

See also: https://godbolt.org/z/z4GEoneza

FWIW: it seems specific to the Neoverse V2 and one of its architecture extensions that it enables, this e.g. doesn't occur with the V1.

## Comments

### Comment 1 - sjoerdmeijer

CC: @paulwalker-arm , @david-arm , @sdesmalen-arm , @davemgreen 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sjoerd Meijer (sjoerdmeijer)

<details>
Compiling this:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define &lt;16 x i16&gt; @<!-- -->foo(&lt;16 x i16&gt; %0) #<!-- -->0 {
  %2 = shufflevector &lt;16 x i16&gt; zeroinitializer, &lt;16 x i16&gt; splat (i16 1), &lt;16 x i32&gt; &lt;i32 0, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31&gt;
  %3 = sdiv &lt;16 x i16&gt; %0, %2
  ret &lt;16 x i16&gt; %3
}

attributes #<!-- -->0 = { "target-cpu"="neoverse-v2" }
```

with `llc test.ll` results in:

```
LLVM ERROR: Cannot select: 0xaaaaae3d6b50: nxv8i16 = AArch64ISD::SRAD_MERGE_OP1 0xaaaaae3d6ae0, 0xaaaaae3d6990, TargetConstant:i32&lt;0&gt;
  0xaaaaae3d6ae0: nxv8i1 = AArch64ISD::PTRUE TargetConstant:i32&lt;8&gt;
  0xaaaaae3d6990: nxv8i16 = insert_subvector undef:nxv8i16, 0xaaaaae3d6920, Constant:i64&lt;0&gt;
    0xaaaaae3d6920: v8i16,ch = CopyFromReg 0xaaaaae35e3e0, Register:v8i16 %1
In function: foo
```

See also: https://godbolt.org/z/z4GEoneza

FWIW: it seems specific to the Neoverse V2 and one of its architecture extensions that it enables, this e.g. doesn't occur with the V1.
</details>


---

