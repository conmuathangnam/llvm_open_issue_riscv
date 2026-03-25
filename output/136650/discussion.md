# [InstCombine] Poison safety violation in `foldLogicOfFCmps`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/136650
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-04-23T16:12:32Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/Qcvvgi
```
----------------------------------------
define i1 @src(half %x, half %y) {
#0:
  %copysign.x = copysign half %x, %y
  %ord = fcmp ord half %x, 0x0000
  %cmp = fcmp uge half %copysign.x, 0x7c00
  %#1 = select i1 %ord, i1 %cmp, i1 0
  ret i1 %#1
}
=>
define i1 @src(half %x, half %y) nofree willreturn memory(none) {
#0:
  %copysign.x = copysign half %x, %y
  %#1 = fcmp oeq half %copysign.x, 0x7c00
  ret i1 %#1
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
half %x = #x7c02 (SNaN)
half %y = poison

Source:
half %copysign.x = poison
i1 %ord = #x0 (0)
i1 %cmp = poison
i1 %#1 = #x0 (0)

Target:
half %copysign.x = poison
i1 %#1 = poison
Source value: #x0 (0)
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

