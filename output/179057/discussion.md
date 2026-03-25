# [21.1.1/22/23 x86 avx512 Regression] miscompilation of `_mm_mask_fixupimm_ss`

**Author:** mr-c
**URL:** https://github.com/llvm/llvm-project/issues/179057
**Status:** Closed
**Labels:** backend:X86, miscompilation, release:backport, regression:22
**Closed Date:** 2026-02-01T16:10:13Z

## Body

Compiler Explorer with reproducible example, and a diff assembly viewer showing the miscompilation: https://godbolt.org/z/hKWxz6cEz

<img width="1891" height="351" alt="Image" src="https://github.com/user-attachments/assets/60622741-5185-42c1-854a-1f4f8a96a4ee" />

Clang 22+ gives
```
../test/x86/avx512/fixupimm.c:32: assertion failed: r[1] ~= simde_mm_loadu_ps[1] (17.730000 ~= nan)
```
clang 21.1.0 (3623fe661ae35c6c80ac221f14d85be76aa870f1) has the expected results

For local testing, note the `-mavx512f -mavx512vl`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Michael R. Crusoe (mr-c)

<details>
Compiler Explorer with reproducible example, and a diff assembly viewer showing the miscompilation: https://godbolt.org/z/hKWxz6cEz

&lt;img width="1891" height="351" alt="Image" src="https://github.com/user-attachments/assets/60622741-5185-42c1-854a-1f4f8a96a4ee" /&gt;

Clang 22+ gives
```
../test/x86/avx512/fixupimm.c:32: assertion failed: r[1] ~= simde_mm_loadu_ps[1] (17.730000 ~= nan)
../test/x86/avx512/fixupimm.c:32: assertion failed: r[3] ~= simde_mm_loadu_ps[3] (583.280029 ~= 338.170013)
```
clang 21.1.0 (3623fe661ae35c6c80ac221f14d85be76aa870f1) has the expected results

For local testing, note the `-mavx512f -mavx512vl`
</details>


---

### Comment 2 - mr-c

I see similar problems with
- `_mm_maskz_fixupimm_ss`
- `_mm_mask_fixupimm_sd`
- `_mm_maskz_fixupimm_sd`
- `_mm_mask_fixupimm_round_ss`
- `_mm_maskz_fixupimm_round_ss`
- `_mm_mask_fixupimm_round_sd`
- `_mm_maskz_fixupimm_round_sd`

I can provide test + expected result data for all of these from the SIMDe test library, upon request

---

### Comment 3 - mr-c

Ah, 21.1.1+ fails as well ; bisected to 0d6736704f3800413d541e130a6fd04d44a5250f which is a backport of https://github.com/llvm/llvm-project/pull/153575 ; tagging @RKSimon @phoebewang @nikic

git bisecting confirms that ba707db840516b2246c6a31ef8a96e41939deeb5 from https://github.com/llvm/llvm-project/pull/153575 is the breaking commit on the main branch

---

### Comment 4 - mr-c

@RKSimon Here is my testing script

``` bash
#!/bin/bash -ex
# Build clang. If the build fails, `exit 125` causes this
# revision to be skipped
ninja -C build clang || exit 125
build/bin/clang -mavx512f -mavx512vl -lm -o foo foo.c && ./foo || false
```

and `foo.c` is from https://godbolt.org/z/hKWxz6cEz with an `abort()` added inside `simde_assert_equal_vf32_()`

---

### Comment 5 - mr-c

To test without `-mavx512vl`, apply the following patch to `foo.c`

``` diff
diff --git a/foo.c b/foo.c
index 4dd412ae7d2f..831c814999ee 100644
--- a/foo.c
+++ b/foo.c
@@ -69,7 +69,7 @@ int main(void) {
   __m128i c;
   a = _mm_loadu_ps(test_vec.a);
   b = _mm_loadu_ps(test_vec.b);
-  c = _mm_loadu_epi32(test_vec.c);
+  c = _mm_loadu_si128((__m128i const *)test_vec.c);
   r = _mm_mask_fixupimm_ss(a, test_vec.k, b, c, 123);
   simde_test_x86_assert_equal_f32x4_(r, _mm_loadu_ps(test_vec.r), 0.1f,
                                      "../test/x86/avx512/fixupimm.c", 32, "r",
```

---

### Comment 6 - RKSimon

Will look at this properly tomorrow, but from the looks of the IR:
```cpp
r = _mm_mask_fixupimm_ss(a, test_vec.k, b, c, 123);
```
->
```ll
  %26 = call <4 x float> @llvm.x86.avx512.mask.fixupimm.ss(<4 x float> %22, <4 x float> %23, <4 x i32> %25, i32 123, i8 -20, i32 4)
```
The test_vec.k mmask8 arg (i8 -20 == 0b11101100) - with the lsb set to 0 - shouldn't this just pass through arg `a` (%22)?

@mr-c Please can you confirm the contents of test_vec.k?

---

### Comment 7 - mr-c

Thanks for the quick response @RKSimon ! The original test vector is at https://github.com/simd-everywhere/simde/blob/51d5f4c456dad12359d1895917db3cd331d902a1/test/x86/avx512/fixupimm.c#L1764

It passes using GCC of various versions and clang before 21.1.1

`test_vec.k` is 236, yes

```
$ echo "obase=2;236" | bc
11101100
```
`imm8` is 123 → 01111011 → ZERO_VALUE_TOKEN | ONE_VALUE_TOKEN | SNAN_TOKEN | NEG_INF_TOKEN  |NEG_VALUE_TOKEN

https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=_mm_mask_fixupimm_ss&ig_expand=3053


---

### Comment 8 - RKSimon

Cheers, I'll take a proper look tomorrow - problem seems to be the passed through upper elements come from the second arg

---

### Comment 9 - RKSimon

@mr-c Please can you confirm if #179101 fixes all the regressions you're seeing?

---

### Comment 10 - RKSimon

/cherry-pick 618d71dc98df760d0c724cff6fa69b780e8c0372 49d2323447aec77c3d1ae8c941f3f8a126ff1480

---

### Comment 11 - llvmbot

/pull-request llvm/llvm-project#179117

---

