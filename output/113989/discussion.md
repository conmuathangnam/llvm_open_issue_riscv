# Wrong folding to `fabs` (`NaN` not handled)

**Author:** bongjunj
**URL:** https://github.com/llvm/llvm-project/issues/113989
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, floating-point
**Closed Date:** 2025-02-02T07:29:21Z

## Body

https://github.com/llvm/llvm-project/blob/1ceccbb0dd9d8539fec2213566fe6cc2a05b7993/llvm/lib/Transforms/InstCombine/InstCombineSelect.cpp#L2814-L2885

Alive2 report: https://alive2.llvm.org/ce/z/y2UAwe


```llvm
----------------------------------------
define double @select_fcmp_ole_zero.2(double %x) {
#0:
  %lezero = fcmp ole double %x, 0.000000
  %negx = fsub double 0.000000, %x
  %fabs = select i1 %lezero, double %negx, double %x
  ret double %fabs
}
=>
define double @select_fcmp_ole_zero.2(double %x) {
#0:
  %fabs = fabs double %x
  ret double %fabs
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
double %x = #xfff0000000000008 (SNaN)

Source:
i1 %lezero = #x0 (0)
double %negx = #x7ff0000000000008 (SNaN)
double %fabs = #xfff0000000000008 (SNaN)

Target:
double %fabs = #x7ff0000000000008 (SNaN)
Source value: #xfff0000000000008 (SNaN)
Target value: #x7ff0000000000008 (SNaN)


----------------------------------------
define double @select_fcmp_nnan_ole_zero.2(double %x) {
#0:
  %lezero = fcmp ole double %x, 0.000000
  %negx = fsub nnan double 0.000000, %x
  %fabs = select i1 %lezero, double %negx, double %x
  ret double %fabs
}
=>
define double @select_fcmp_nnan_ole_zero.2(double %x) {
#0:
  %fabs = fabs double %x
  ret double %fabs
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
double %x = #xfff0000000004000 (SNaN)

Source:
i1 %lezero = #x0 (0)
double %negx = poison
double %fabs = #xfff0000000004000 (SNaN)

Target:
double %fabs = #x7ff0000000004000 (SNaN)
Source value: #xfff0000000004000 (SNaN)
Target value: #x7ff0000000004000 (SNaN)


----------------------------------------
define half @select_fcmp_nnan_ugt_negzero.2(half %x) {
#0:
  %#1 = fcmp ugt half %x, 0x8000
  %negx = fsub nnan half 0x0000, %x
  %#2 = fadd half %negx, 0x0000
  %fabs = select i1 %#1, half %x, half %#2
  ret half %fabs
}
=>
define half @select_fcmp_nnan_ugt_negzero.2(half %x) {
#0:
  %fabs = fabs half %x
  ret half %fabs
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
half %x = #xfc04 (SNaN)

Source:
i1 %#1 = #x1 (1)
half %negx = poison
half %#2 = poison
half %fabs = #xfc04 (SNaN)

Target:
half %fabs = #x7c04 (SNaN)
Source value: #xfc04 (SNaN)
Target value: #x7c04 (SNaN)

Summary:
  0 correct transformations
  3 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

## Comments

### Comment 1 - dtcxzyw

Fixed by #121580

---

