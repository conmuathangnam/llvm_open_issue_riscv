# NVPTX: `bf16 sint_to_fp i1` uses wrong `selp` value (+1 instead of -1)

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/187000
**Status:** Closed
**Labels:** miscompilation, backend:NVPTX
**Closed Date:** 2026-03-24T15:09:44Z

## Body

Any GPU kernel on SM90+ (Hopper) that performs a `sitofp i1 → bfloat` conversion receives a silently wrong result: `sitofp(true)` produces `bf16(+1.0)` instead of `bf16(-1.0)`. This is a silent sign inversion with no diagnostic. This is inconsistent with `f16`.

PoC: [https://godbolt.org/z/8Pca8nGGr](https://godbolt.org/z/8Pca8nGGr)

## Vulnerable code location(s)

[https://github.com/llvm/llvm-project/blob/67e47fb5317cabd82317379baf8e31d61984d43d/llvm/lib/Target/NVPTX/NVPTXInstrInfo.td#L2030](https://github.com/llvm/llvm-project/blob/67e47fb5317cabd82317379baf8e31d61984d43d/llvm/lib/Target/NVPTX/NVPTXInstrInfo.td#L2030)

## Vulnerable code analysis

Copy-paste error in the TableGen pattern for `bf16 sint_to_fp i1`. The pattern was copied from the `uint_to_fp` block and the `SELP_b32ii` true-value was never changed from `1` to `-1`:

```tablegen
// Line 2030 — BUGGY
def : Pat<(bf16 (sint_to_fp i1:$a)), (CVT_bf16_s32 (SELP_b32ii 1, 0, $a), CvtRN)>;
//                                                              ^ should be -1
```

`i1 true` has signed value `-1` (2's complement), so `sint_to_fp` must select `-1` before the `cvt.rn.bf16.s32`. All analogous patterns for `f16` (line 2017), `f32` (line 2045), and `f64` (line 2057) correctly use `-1`. The generated PTX confirms the error:

```
ACTUAL:   selp.b32  %r1,  1, 0, %p1   ← wrong
EXPECTED: selp.b32  %r1, -1, 0, %p1
```

**Fix:** change `SELP_b32ii 1` → `SELP_b32ii -1` on line 2030. The bug is only reachable when both `hasSM<90>` and `hasPTX<78>` predicates are satisfied (SM90 / Hopper, PTX 7.8+).





