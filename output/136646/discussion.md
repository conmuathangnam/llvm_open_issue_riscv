# [InstCombine] The sign bit of NaN is not preserved when folding `(X <= +/-0.0) ? (0.0 - X) : X to fabs(X)`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/136646
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-04-26T06:03:14Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/2JaLU9
```

----------------------------------------
define half @src(half %x) {
#0:
  %gtzero = fcmp ugt half %x, 0x0000
  %negx = fsub half 0x0000, %x
  %fabs = select i1 %gtzero, half %x, half %negx
  ret half %fabs
}
=>
define half @src(half %x) nofree willreturn memory(none) {
#0:
  %fabs = fabs half %x
  ret half %fabs
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
half %x = #xfd00 (SNaN)

Source:
i1 %gtzero = #x1 (1)
half %negx = #xff00 (QNaN)
half %fabs = #xfd00 (SNaN)

Target:
half %fabs = #x7d00 (SNaN)
Source value: #xfd00 (SNaN)
Target value: #x7d00 (SNaN)

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```
Related issues:
https://github.com/AliveToolkit/alive2/pull/1155
https://github.com/llvm/llvm-project/issues/59279


