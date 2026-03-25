# [InstCombine] ninf should not be propagated

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/136430
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, floating-point
**Closed Date:** 2025-04-28T09:24:50Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/cmneUX
```
define half @test_fcmp_select_maxnum(half %x) {
#0:
  %cmp2 = fcmp ogt half %x, 0x5bf8
  %sel2 = select nnan ninf nsz i1 %cmp2, half %x, half 0x5bf8
  ret half %sel2
}
=>
define half @test_fcmp_select_maxnum(half %x) {
#0:
  %sel2 = fmax ninf nsz half %x, 0x5bf8
  ret half %sel2
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
half %x = #xfc00 (-oo)

Source:
i1 %cmp2 = #x0 (0)
half %sel2 = #x5bf8 (255)

Target:
half %sel2 = poison
Source value: #x5bf8 (255)
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

