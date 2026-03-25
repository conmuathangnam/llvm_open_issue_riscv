# [InstSimplify] wrong folding of pointer comparison `select (icmp eq ptr ...)`

**Author:** bongjunj
**URL:** https://github.com/llvm/llvm-project/issues/115574
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2025-02-02T11:04:25Z

## Body

https://github.com/llvm/llvm-project/blob/c93e001ca695e905cb965b36d63f7a348d1dd809/llvm/lib/Analysis/InstructionSimplify.cpp#L148-L167

Alive2 report: https://alive2.llvm.org/ce/z/aux2zY

```llvm
----------------------------------------
define ptr @smin_test8.2(ptr %a, ptr %b, ptr %c) {
#0:
  %cmp1 = icmp eq ptr %a, %b
  %umin1 = select i1 %cmp1, ptr %a, ptr %b
  %cmp2 = icmp slt ptr %b, %c
  %umin2 = select i1 %cmp2, ptr %b, ptr %c
  %cmp3 = icmp ult ptr %umin2, %a
  %umin3 = select i1 %cmp3, ptr %umin2, ptr %a
  %cmp4 = icmp slt ptr %c, %umin3
  %res = select i1 %cmp4, ptr %umin1, ptr %umin3
  ret ptr %res
}
=>
define ptr @smin_test8.2(ptr %a, ptr %b, ptr %c) {
#0:
  %cmp2 = icmp slt ptr %b, %c
  %umin2 = select i1 %cmp2, ptr %b, ptr %c
  %cmp3 = icmp ult ptr %umin2, %a
  %umin3 = select i1 %cmp3, ptr %umin2, ptr %a
  %cmp4 = icmp slt ptr %c, %umin3
  %res = select i1 %cmp4, ptr %b, ptr %umin3
  ret ptr %res
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:
ptr %a = pointer(non-local, block_id=0, offset=1) / Address=#x1
ptr %b = pointer(non-local, block_id=1, offset=0) / Address=#x1
ptr %c = pointer(non-local, block_id=0, offset=-8) / Address=#x8

Source:
i1 %cmp1 = #x1 (1)
ptr %umin1 = pointer(non-local, block_id=0, offset=1) / Address=#x1
i1 %cmp2 = #x0 (0)
ptr %umin2 = pointer(non-local, block_id=0, offset=-8) / Address=#x8
i1 %cmp3 = #x0 (0)
ptr %umin3 = pointer(non-local, block_id=0, offset=1) / Address=#x1
i1 %cmp4 = #x1 (1)
ptr %res = pointer(non-local, block_id=0, offset=1) / Address=#x1

SOURCE MEMORY STATE
===================
NON-LOCAL BLOCKS:
Block 0 >	size: 0	align: 1	alloc type: 0	alive: false	address: 0
Block 1 >	size: 1	align: 1	alloc type: 0	alive: true	address: 1
Block 2 >	size: 1	align: 1	alloc type: 0	alive: true	address: 2
Block 3 >	size: 3	align: 1	alloc type: 0	alive: true	address: 8

Target:
i1 %cmp2 = #x0 (0)
ptr %umin2 = pointer(non-local, block_id=0, offset=-8) / Address=#x8
i1 %cmp3 = #x0 (0)
ptr %umin3 = pointer(non-local, block_id=0, offset=1) / Address=#x1
i1 %cmp4 = #x1 (1)
ptr %res = pointer(non-local, block_id=1, offset=0) / Address=#x1
Source value: pointer(non-local, block_id=0, offset=1) / Address=#x1
Target value: pointer(non-local, block_id=1, offset=0) / Address=#x1

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

