# [MachineCopyProp] WRONG code

**Author:** JonPsson1
**URL:** https://github.com/llvm/llvm-project/issues/124768
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2025-01-29T20:07:52Z

## Body

[wrong0.tar.gz](https://github.com/user-attachments/files/18575703/wrong0.tar.gz)

llc -mtriple=s390x-linux-gnu -o after-machine-cp.mir -mcpu=z15 -O1 ./wrong0.mir -run-pass=machine-cp

The reduced test case is somewhat silly as it originally involves a select instruction where both source operands hold the same register (the select has been expanded to a cmp/jump, as seen below). Nevertheless, before machine-cp:

```
  bb.14.cleanup22.i:
    liveins: $r2d, $r4d, $r6d, $r8d, $r10d, $r0h, $r5h, $r1l, $r3l, $r5l, $r12q
  
    renamable $r1l = AHIMuxK killed renamable $r1l, -1, implicit-def dead $cc
    CHIMux renamable $r5h, 9, implicit-def $cc
    $r14h = COPY killed renamable $r1l
    BRC 14, 6, %bb.26, implicit $cc
  
  bb.27.cleanup22.i:
    liveins: $r1l, $r2d, $r2l, $r2h, $r4d, $r4l, $r4h, $r6d, $r6l, $r6h, $r8d, $r8l, $r8h, $r10d, $r10l, $r10h, $r0h, $r13h, $r14h, $r3l, $r5l, $r12q, $r12d, $r12l, $r12h, $r13d, $r13l, $r5h, $cc
  
    $r14h = COPY renamable $r1l
  
  bb.26.cleanup22.i:
```
 After machine-cp, the COPY is no longer defining $r14h with the AHIMux definition, but with it's source, which seems broken.

```
  bb.16.cleanup22.i:
    successors: %bb.18(0x40000000), %bb.17(0x40000000)
    liveins: $r2d, $r4d, $r6d, $r8d, $r10d, $r0h, $r5h, $r1l, $r3l, $r5l, $r12q
  
    $r14h = AHIMuxK killed renamable $r1l, -1, implicit-def dead $cc
    CHIMux renamable $r5h, 9, implicit-def $cc
    BRC 14, 6, %bb.18, implicit $cc
  
  bb.17.cleanup22.i:
    successors: %bb.18(0x80000000)
    liveins: $r1l, $r2d, $r2l, $r2h, $r4d, $r4l, $r4h, $r6d, $r6l, $r6h, $r8d, $r8l, $r8h, $r10d, $r10l, $r10h, $r0h, $r13h, $r14h, $r3l, $r5l, $r12q, $r12d, $r12l, $r12h, $r13d, $r13l, $r5h, $cc
  
    $r14h = COPY renamable $r1l
  
  bb.18.cleanup22.i:
 
```

@ostannard @nikic @vladimirradosavljevic 

This is the same test case as in #124542 

## Comments

### Comment 1 - vladimirradosavljevic

MachineCP performs backward copy propagation optimization, which works as follows:
```
//    $R0 = OP ...
//    ... // No read/clobber of $R0 and $R1
//    $R1 = COPY $R0 // $R0 is killed
// Replace $R0 with $R1 and remove the COPY
//    $R1 = OP ...
//    ...
```

The key point here is that `$R0 is killed`, which in this test case is true for `$r14h = COPY killed renamable $r1l`. If the killed flag is not present in the copy, MachineCP will not perform this optimization.
The issue seems to be not with MachineCP itself, but before this optimization (maybe with the expansion of a selection instruction where the killed flag should be removed from the copy in `bb.14.cleanup22.i`).

---

### Comment 2 - JonPsson1

> MachineCP performs backward copy propagation optimization, which works as follows:
> 
> ```
> //    $R0 = OP ...
> //    ... // No read/clobber of $R0 and $R1
> //    $R1 = COPY $R0 // $R0 is killed
> // Replace $R0 with $R1 and remove the COPY
> //    $R1 = OP ...
> //    ...
> ```
> 
> The key point here is that `$R0 is killed`, which in this test case is true for `$r14h = COPY killed renamable $r1l`. If the killed flag is not present in the copy, MachineCP will not perform this optimization. The issue seems to be not with MachineCP itself, but before this optimization (maybe with the expansion of a selection instruction where the killed flag should be removed from the copy in `bb.14.cleanup22.i`).

Thanks for the analysis - I believe you are right and seems to relate just like you said to the expansion of a select instruction. I also think the MachineVerifier should have cought this, so I opened an issue as well for that.

Patch in progress for the select expansion...

---

