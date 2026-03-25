# [CodeGen, machine-latecleanup] Bad removal of kill flag.

**Author:** JonPsson1
**URL:** https://github.com/llvm/llvm-project/issues/117783
**Status:** Closed
**Labels:** llvm:codegen, llvm:regalloc
**Closed Date:** 2025-03-13T14:50:56Z

## Body

llc -mtriple=s390x-linux-gnu  ./tc_mlc.mir -o /dev/null -O3 -verify-machineinstrs  -run-pass=machine-latecleanup
[tc_mlc.tar.gz](https://github.com/user-attachments/files/17925278/tc_mlc.tar.gz)

This reduced test case has a redundant LHIMux (Load Immediate) in bb.1 which gets removed by MachineLateInstrsCleanup:

```
bb.0:
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  renamable $r0l = LHIMux -1
  J %bb.1

bb.1:
; predecessors: %bb.0

  renamable $r1d = LGFI 0
  ST128 killed renamable $r0q, $r15d, 168, $noreg
  renamable $r0l = LHIMux -1
  ST killed renamable $r0l, $r15d, 160, $noreg
  Return
```

```
# *** IR Dump After Machine Late Instructions Cleanup Pass (machine-latecleanup) ***:

bb.0:
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  renamable $r0l = LHIMux -1
  J %bb.1

bb.1:
; predecessors: %bb.0

  renamable $r1d = LGFI 0
  ST128 renamable $r0q, $r15d, 168, $noreg
  ST killed renamable $r0l, $r15d, 160, $noreg
  Return

# End machine code for function fun0.
```
The MachineVerifier here complains:

```
*** Bad machine code: Using an undefined physical register ***
- function:    fun0
- basic block: %bb.1  (0xa4fa328)
- instruction: ST killed renamable $r0l, $r15d, 160, $noreg
- operand 0:   killed renamable $r0l
```

I think the assumption was that in the input, the ST128 used and killed $r0q (r0+r1 as a quadword), and presumably then both r0 and r1 would have some modeled definition prior to it, so the kill flag is simply removed with the hope that this would make the IR legal. However the ST128 use of $r0q was only preceeded by a partial definition (r1d), which the MachineVerifier accepts:

```
      if (Reg.isPhysical()) {
        // Reserved registers may be used even when 'dead'.
        bool Bad = !isReserved(Reg);
        // We are fine if just any subregister has a defined value.
```
In the output, the ST128 uses $r0q which is (partially) defined in $r1d by the LGFI. The $r0l should now be in the live-in list of bb.1, but it is not, so the use of it in the ST produces the error.

MachineLateInstrsCleanup keeps track of register kills by querying `MI.findRegisterUseOperandIdx(Reg, TRI, true /*isKill*/)`. Unfortunately this doesn't work in this situation, as partial defs of physregs are allowed.

The kill flag on ST128 kills all subregs, IIUC, so it must be removed. Not sure exactly how to handle this, but maybe it is necessary to keep track of kills of superregs specially which has to be removed, but still continue in clearKillsForDef() until a kill-flag or def of reg/subreg is seen..?

@arsenm @nikic @nickdesaulniers @uweigand 


## Comments

### Comment 1 - arsenm

This is a post-RA pass that depends on kill flags? The solution is to stop using them. We should get serious about removing kill flags entirely. There should be no reason to use them after TwoAddressInstructions today. This pass should be rewritten to use backwards iteration, with precise liveness tracking with LiveRegUnits. 

---

### Comment 2 - JonPsson1

> This is a post-RA pass that depends on kill flags? The solution is to stop using them. We should get serious about removing kill flags entirely. There should be no reason to use them after TwoAddressInstructions today. This pass should be rewritten to use backwards iteration, with precise liveness tracking with LiveRegUnits.

Nope - this is only as a courtesy to later passes who might need them. As per previous discussions, I agree it is certainly not worth maintaining these kill flags unless truly needed. IIRC, it was only the legacy PostRA scheduler that used them, so maybe we could proceed and try to get rid of them.

Link above to my proposed patch.

---

