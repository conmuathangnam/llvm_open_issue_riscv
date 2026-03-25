# [InstCombine] Incorrect fold of the comparison of GEPs

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121581
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-01-04T09:23:58Z

## Body

Alive2: https://alive2.llvm.org/ce/z/HRsqeW
```
define i1 @test(ptr %x, ptr %y) {
  %gep1 = getelementptr i8, ptr %x, i32 1
  %gep2 = getelementptr i8, ptr %y, i32 1
  %cmp = icmp ult ptr %gep1, %gep2
  ret i1 %cmp
}
```
```

----------------------------------------
define i1 @test(ptr %x, ptr %y) {
#0:
  %gep1 = gep ptr %x, 1 x i32 1
  %gep2 = gep ptr %y, 1 x i32 1
  %cmp = icmp ult ptr %gep1, %gep2
  ret i1 %cmp
}
=>
define i1 @test(ptr %x, ptr %y) {
#0:
  %cmp = icmp ult ptr %x, %y
  ret i1 %cmp
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
ptr %x = pointer(non-local, block_id=0, offset=-1) / Address=#xf
ptr %y = null

Source:
ptr %gep1 = null
ptr %gep2 = pointer(non-local, block_id=0, offset=1) / Address=#x1
i1 %cmp = #x1 (1)

SOURCE MEMORY STATE
===================
NON-LOCAL BLOCKS:
Block 0 >	size: 0	align: 1	alloc type: 0	alive: false	address: 0
Block 1 >	size: 1	align: 2	alloc type: 0	alive: true	address: 2
Block 2 >	size: 0	align: 1	alloc type: 0	alive: true	address: 3

Target:
i1 %cmp = #x0 (0)
Source value: #x1 (1)
Target value: #x0 (0)

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

