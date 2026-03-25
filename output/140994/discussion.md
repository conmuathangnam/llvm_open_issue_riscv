# [InstCombine] `(fcmp une T, F) ? T : F --> T` should not rely on the nsz flag on fcmp

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/140994
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, floating-point
**Closed Date:** 2025-05-31T08:25:12Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/_FgncC
```
define double @src(double %x, double %y) {
  %cmp = fcmp nsz une double %x, %y
  %cond = select i1 %cmp, double %x, double %y
  ret double %cond
}

define double @tgt(double returned %x, double %y) {
  ret double %x
}
```
```
Transformation doesn't verify!

ERROR: Value mismatch

Example:
double %x = #x8000000000000000 (-0.0)
double %y = #x0000000000000000 (+0.0)

Source:
i1 %cmp = #x0 (0)
double %cond = #x0000000000000000 (+0.0)

Target:
Source value: #x0000000000000000 (+0.0)
Target value: #x8000000000000000 (-0.0)
```
`nsz` flag should not be propagated from fcmp into select.


