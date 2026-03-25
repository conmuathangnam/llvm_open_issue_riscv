# [InstCombine] GEPNoWrapFlags is propagated incorrectly

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121459
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-01-03T15:19:59Z

## Body

Alive2: https://alive2.llvm.org/ce/z/3ME7S7
```
define ptr @src(i1 %c, ptr %p) {
  br i1 %c, label %if, label %else

if:
  %gep1 = getelementptr inbounds i32, ptr %p, i64 1
  br label %join

else:
  %gep2 = getelementptr i64, ptr %p, i64 2
  br label %join

join:
  %phi = phi ptr [ %gep1, %if ], [ %gep2, %else ]
  %gep = getelementptr i32, ptr %phi, i64 1
  ret ptr %gep
}
```
```

----------------------------------------
define ptr @src(i1 %c, ptr %p) {
#0:
  br i1 %c, label %if, label %else

else:
  %gep2 = gep ptr %p, 8 x i64 2
  br label %join

if:
  %gep1 = gep inbounds ptr %p, 4 x i64 1
  br label %join

join:
  %phi = phi ptr [ %gep1, %if ], [ %gep2, %else ]
  %gep = gep ptr %phi, 4 x i64 1
  ret ptr %gep
}
=>
define ptr @src(i1 %c, ptr %p) {
#0:
  br i1 %c, label %if, label %else

else:
  br label %join

if:
  br label %join

join:
  %#1 = phi i64 [ 4, %if ], [ 16, %else ]
  %#2 = gep inbounds nuw ptr %p, 1 x i64 %#1
  %gep = gep ptr %#2, 1 x i64 4
  ret ptr %gep
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
i1 %c = #x0 (0)
ptr %p = null

Source:
  >> Jump to %else
ptr %gep2 = pointer(non-local, block_id=0, offset=16) / Address=#x0000000000000010
  >> Jump to %join
ptr %phi = pointer(non-local, block_id=0, offset=16) / Address=#x0000000000000010
ptr %gep = pointer(non-local, block_id=0, offset=20) / Address=#x0000000000000014

SOURCE MEMORY STATE
===================
NON-LOCAL BLOCKS:
Block 0 >	size: 0	align: 1	alloc type: 0	alive: false	address: 0
Block 1 >	size: 0	align: 1	alloc type: 0	alive: true	address: 1

Target:
  >> Jump to %else
  >> Jump to %join
i64 %#1 = #x0000000000000010 (16)
ptr %#2 = poison
ptr %gep = poison
Source value: pointer(non-local, block_id=0, offset=20) / Address=#x0000000000000014
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```


