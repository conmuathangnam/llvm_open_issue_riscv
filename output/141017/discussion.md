# [InstCombine] Miscompilation `select (fcmp nsz olt X, 0.0), -0.0, X -> select (fcmp nsz ole X, -0.0), -0.0, X`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/141017
**Status:** Closed
**Labels:** miscompilation, floating-point, llvm:analysis
**Closed Date:** 2025-06-02T10:21:16Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/Uydp8F
```
define float @src(float %x) {
  %cmp = fcmp nsz olt float %x, 0.0
  %sel = select i1 %cmp, float -0.0, float %x
  ret float %sel
}

define float @tgt(float %x) {
  %.inv = fcmp nsz ole float %x, -0.000000e+00
  %sel1 = select i1 %.inv, float -0.000000e+00, float %x
  ret float %sel1
}
```
```
Transformation doesn't verify!

ERROR: Target's return value is more undefined

Example:
float %x = #x00000000 (+0.0)

Source:
i1 %cmp = #x0 (0)
float %sel = #x00000000 (+0.0)

Target:
i1 %.inv = #x1 (1)
float %sel1 = #x00000000 (+0.0)
Source value: #x00000000 (+0.0)
Target value: #x00000000 (+0.0)
```


