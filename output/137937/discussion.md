# [ConstraintElim] `checkOrAndOpImpliedByOther` should drop `disjoint` flag

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/137937
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-05-14T15:37:43Z

## Body

Reproducer: https://alive2.llvm.org/ce/z/pUvC88
```
----------------------------------------
define i1 @src(i8 %b) {
entry:
  %c.1 = icmp slt i8 %b, 1
  %c.2 = icmp ne i8 %b, 0
  %or = or disjoint i1 %c.2, %c.1
  ret i1 %or
}
=>
define i1 @src(i8 %b) {
entry:
  %c.2 = icmp ne i8 %b, 0
  %or = or disjoint i1 %c.2, 1
  ret i1 %or
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
i8 %b = #x01 (1)

Source:
i1 %c.1 = #x0 (0)
i1 %c.2 = #x1 (1)
i1 %or = #x1 (1)

Target:
i1 %c.2 = #x1 (1)
i1 %or = poison
Source value: #x1 (1)
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```
Triggered by pre-commit fuzzing for https://github.com/llvm/llvm-project/pull/135784.


