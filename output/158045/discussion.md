# [InferAlignment] Miscompilation: unsafe raising load alignment for `inttoptr` constant causes UB

**Author:** zitongzhoueric
**URL:** https://github.com/llvm/llvm-project/issues/158045
**Status:** Closed
**Labels:** duplicate, miscompilation, llvm:instcombine
**Closed Date:** 2025-09-11T12:55:50Z

## Body

To reproduce: https://alive2.llvm.org/ce/z/gLQst2
Not sure if this is a known issue. 
```
----------------------------------------
define i8 @f() {
entry:
  %p = inttoptr i64 40 to ptr
  %v = load i8, ptr %p, align 1
  ret i8 %v
}
=>
define i8 @f() {
entry:
  %p = inttoptr i64 40 to ptr
  %v = load i8, ptr %p, align 8
  ret i8 %v
}
Transformation doesn't verify!

ERROR: Source is more defined than target

Example:

Source:
ptr %p = phy-ptr(addr=40) / Address=#x0000000000000028
i8 %v = poison

SOURCE MEMORY STATE
===================
NON-LOCAL BLOCKS:
Block 0 >	size: 0	align: 1	alloc type: 0	alive: false	address: #x0000000000000000
Block 1 >	size: 12	align: 1	alloc type: 0	alive: true	address: #x000000000000001d

Target:
ptr %p = phy-ptr(addr=40) / Address=#x0000000000000028
i8 %v = UB triggered!

TARGET MEMORY STATE
===================
NON-LOCAL BLOCKS:
Block 0 >	size: 0	align: 1	alloc type: 0	alive: false	address: #x0000000000000000
Block 1 >	size: 12	align: 1	alloc type: 0	alive: true	address: #x000000000000001d


Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

