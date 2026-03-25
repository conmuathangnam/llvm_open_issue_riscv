# [InstCombine] Miscompilation in `simplifyIRemMulShl`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121584
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-01-04T17:20:55Z

## Body

Alive2: https://alive2.llvm.org/ce/z/rdeehL
```

----------------------------------------
define i8 @srem_XY_XZ_with_CY_gt_CZ_no_nuw_out1(i8 noundef %X) {
#0:
  %BO0 = mul nsw i8 noundef %X, 127
  %BO1 = shl nsw i8 noundef %X, 7
  %r = srem i8 %BO1, %BO0
  ret i8 %r
}
=>
define i8 @srem_XY_XZ_with_CY_gt_CZ_no_nuw_out1(i8 noundef %X) {
#0:
  %r = sub nsw i8 0, noundef %X
  ret i8 %r
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
i8 noundef %X = #xff (255, -1)

Source:
i8 %BO0 = #x81 (129, -127)
i8 %BO1 = #x80 (128, -128)
i8 %r = #xff (255, -1)

Target:
i8 %r = #x01 (1)
Source value: #xff (255, -1)
Target value: #x01 (1)

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

