# [AArch64] Incorrect overflow behaviour for f16 -> s16 scalar NEON conversion intrinsics

**Author:** mrkajetanp
**URL:** https://github.com/llvm/llvm-project/issues/154343
**Status:** Closed
**Labels:** backend:AArch64, llvm:codegen, miscompilation
**Closed Date:** 2025-09-02T10:38:39Z

## Body

```
#include <stdio.h>
#include <stdint.h>
#include <arm_neon.h>
#include <arm_fp16.h>
#include <memory.h>

int main() {
    uint16_t b1 = 0x7bff;
    float16_t one;
    memcpy(&one, &b1, sizeof(float16_t));
    printf("%d\n", vcvth_s16_f16(one));   <-- -32
    printf("%d\n", vcvtah_s16_f16(one));  <-- -32
    printf("%d\n", vcvtmh_s16_f16(one));  <-- -32
    printf("%d\n", vcvtnh_s16_f16(one));  <-- -32
    printf("%d\n", vcvtph_s16_f16(one));  <-- -32
}
```

`clang -march=armv8.6-a+fp16 fp16.c`

This code snippet casts a 16-bit float with a value that would overflow a 16-bit integer to a 16-bit integer. The expected result would be for the cast to saturate, i.e. return 32767. This is what happens with other compilers.
Instead, when compiled with clang, the cast does not saturate. The returned value is -32.

The IR generated from a call to vcvth_s16_f16 is as follows:

```
%fcvt = call i32 @llvm.aarch64.neon.fcvtzs.i32.f16(half %11)
%12 = trunc i32 %fcvt to i16
```

Because the cast is implemented using the LLVM intrinsic for f16->s32 conversions, the emitted instruction is "fcvtzs w8, h0". Because the value being converted would not saturate a 32-bit integer, the saturation does not happen. When it is then truncated to a 16-bit integer, some important bits are zeroed out, returning an incorrect result.

The main issue is that these intrinsics are not overloaded for the `.i16.f16` formats, hence clang cannot generate them and is using the current approach. The fix requires first implementing correct overloads in the backend, and then updating clang to use the newly overloaded intrinsics.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Kajetan Puchalski (mrkajetanp)

<details>
```
#include &lt;stdio.h&gt;
#include &lt;stdint.h&gt;
#include &lt;arm_neon.h&gt;
#include &lt;arm_fp16.h&gt;
#include &lt;memory.h&gt;

int main() {
    uint16_t b1 = 0x7bff;
    float16_t one;
    memcpy(&amp;one, &amp;b1, sizeof(float16_t));
    printf("%d\n", vcvth_s16_f16(one));   &lt;-- -32
    printf("%d\n", vcvtah_s16_f16(one));  &lt;-- -32
    printf("%d\n", vcvtmh_s16_f16(one));  &lt;-- -32
    printf("%d\n", vcvtnh_s16_f16(one));  &lt;-- -32
    printf("%d\n", vcvtph_s16_f16(one));  &lt;-- -32
}
```

`clang -march=armv8.6-a+fp16 fp16.c`

This code snippet casts a 16-bit float with a value that would overflow a 16-bit integer to a 16-bit integer. The expected result would be for the cast to saturate, i.e. return 32767. This is what happens with other compilers.
Instead, when compiled with clang, the cast does not saturate. The returned value is -32.

The IR generated from a call to vcvth_s16_f16 is as follows:

```
%fcvt = call i32 @<!-- -->llvm.aarch64.neon.fcvtzs.i32.f16(half %11)
%12 = trunc i32 %fcvt to i16
```

Because the cast is implemented using the LLVM intrinsic for f16-&gt;f32 conversions, the emitted instruction is "fcvtzs w8, h0". Because the value being converted would not saturate a 32-bit integer, the saturation does not happen. When it is then truncated to a 16-bit integer, some important bits are zeroed out, returning an incorrect result.

The main issue is that these intrinsics are not overloaded for the `.i16.f16` formats, hence clang cannot generate them and is using the current approach. The fix requires first implementing correct overloads in the backend, and then updating clang to use the newly overloaded intrinsics.
</details>


---

