# llvm/test/CodeGen/AMDGPU/shufflevector-physreg-copy.ll fails machine verifier after scheduling

**Author:** arsenm
**URL:** https://github.com/llvm/llvm-project/issues/130884
**Status:** Open
**Labels:** backend:AMDGPU, llvm:codegen

## Body

This test disables the verifier to work around a bug in liveness in the scheduler:

```
# After Machine Instruction Scheduler
********** INTERVALS **********
SGPR16_LO16 [0B,32r:0) 0@0B-phi
SGPR16_HI16 [0B,32r:0) 0@0B-phi
SGPR17_LO16 [0B,16r:0) 0@0B-phi
SGPR17_HI16 [0B,16r:0) 0@0B-phi
VGPR4_LO16 [96r,104r:0) 0@96r
VGPR4_HI16 [96r,104r:0) 0@96r
VGPR5_LO16 [96r,100r:0) 0@96r
VGPR5_HI16 [96r,100r:0) 0@96r
%10 [16r,32r:0)[32r,176r:1) 0@16r 1@32r  L0000000000000003 [32r,176r:0) 0@32r  L000000000000000C [16r,176r:0) 0@16r  weight:0.000000e+00
%11 [80r,176r:0) 0@80r  weight:0.000000e+00
%18 [100r,104r:0)[104r,176r:1) 0@100r 1@104r  L0000000000000003 [100r,176r:0) 0@100r  L000000000000000C [104r,176r:0) 0@104r  weight:0.000000e+00
RegMasks:
********** MACHINEINSTRS **********
# Machine code for function shufflevector_v2i32_10_physreg_even_vgpr_pair_copy: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $sgpr16 in %8, $sgpr17 in %9

0B	bb.0 (%ir-block.0):
	  liveins: $sgpr16, $sgpr17
16B	  undef %10.sub1:sgpr_64 = COPY $sgpr17
32B	  %10.sub0:sgpr_64 = COPY $sgpr16
80B	  %11:vgpr_32 = V_MOV_B32_e32 0, implicit $exec
96B	  INLINEASM &"; def $0, $1" [attdialect], $0:[regdef], implicit-def $vgpr4, $1:[regdef], implicit-def $vgpr5
100B	  %18.sub0:vreg_64 = COPY $vgpr5
104B	  undef %18.sub1:vreg_64 = COPY $vgpr4
176B	  GLOBAL_STORE_DWORDX2_SADDR %11:vgpr_32, %18:vreg_64, %10:sgpr_64, 0, 0, implicit $exec :: (store (s64) into %ir.ptr, addrspace 1)
192B	  SI_RETURN

# End machine code for function shufflevector_v2i32_10_physreg_even_vgpr_pair_copy.

*** Bad machine code: No live segment at use ***
- function:    shufflevector_v2i32_10_physreg_even_vgpr_pair_copy
- basic block: %bb.0  (0x11e01b120) [0B;208B)
- instruction: 100B	%18.sub0:vreg_64 = COPY $vgpr5
- operand 0:   %18.sub0:vreg_64
- liverange:   [100r,104r:0)[104r,176r:1) 0@100r 1@104r
- v. register: %18
- at:          100B
LLVM ERROR: Found 1 machine code errors.
```

bf2d1c46072a0461cb3ddcaefcafeccc2637995e should be reverted to restore the test

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Matt Arsenault (arsenm)

<details>
This test disables the verifier to work around a bug in liveness in the scheduler:

```
# After Machine Instruction Scheduler
********** INTERVALS **********
SGPR16_LO16 [0B,32r:0) 0@<!-- -->0B-phi
SGPR16_HI16 [0B,32r:0) 0@<!-- -->0B-phi
SGPR17_LO16 [0B,16r:0) 0@<!-- -->0B-phi
SGPR17_HI16 [0B,16r:0) 0@<!-- -->0B-phi
VGPR4_LO16 [96r,104r:0) 0@<!-- -->96r
VGPR4_HI16 [96r,104r:0) 0@<!-- -->96r
VGPR5_LO16 [96r,100r:0) 0@<!-- -->96r
VGPR5_HI16 [96r,100r:0) 0@<!-- -->96r
%10 [16r,32r:0)[32r,176r:1) 0@<!-- -->16r 1@<!-- -->32r  L0000000000000003 [32r,176r:0) 0@<!-- -->32r  L000000000000000C [16r,176r:0) 0@<!-- -->16r  weight:0.000000e+00
%11 [80r,176r:0) 0@<!-- -->80r  weight:0.000000e+00
%18 [100r,104r:0)[104r,176r:1) 0@<!-- -->100r 1@<!-- -->104r  L0000000000000003 [100r,176r:0) 0@<!-- -->100r  L000000000000000C [104r,176r:0) 0@<!-- -->104r  weight:0.000000e+00
RegMasks:
********** MACHINEINSTRS **********
# Machine code for function shufflevector_v2i32_10_physreg_even_vgpr_pair_copy: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $sgpr16 in %8, $sgpr17 in %9

0B	bb.0 (%ir-block.0):
	  liveins: $sgpr16, $sgpr17
16B	  undef %10.sub1:sgpr_64 = COPY $sgpr17
32B	  %10.sub0:sgpr_64 = COPY $sgpr16
80B	  %11:vgpr_32 = V_MOV_B32_e32 0, implicit $exec
96B	  INLINEASM &amp;"; def $0, $1" [attdialect], $0:[regdef], implicit-def $vgpr4, $1:[regdef], implicit-def $vgpr5
100B	  %18.sub0:vreg_64 = COPY $vgpr5
104B	  undef %18.sub1:vreg_64 = COPY $vgpr4
176B	  GLOBAL_STORE_DWORDX2_SADDR %11:vgpr_32, %18:vreg_64, %10:sgpr_64, 0, 0, implicit $exec :: (store (s64) into %ir.ptr, addrspace 1)
192B	  SI_RETURN

# End machine code for function shufflevector_v2i32_10_physreg_even_vgpr_pair_copy.

*** Bad machine code: No live segment at use ***
- function:    shufflevector_v2i32_10_physreg_even_vgpr_pair_copy
- basic block: %bb.0  (0x11e01b120) [0B;208B)
- instruction: 100B	%18.sub0:vreg_64 = COPY $vgpr5
- operand 0:   %18.sub0:vreg_64
- liverange:   [100r,104r:0)[104r,176r:1) 0@<!-- -->100r 1@<!-- -->104r
- v. register: %18
- at:          100B
LLVM ERROR: Found 1 machine code errors.
```
</details>


---

### Comment 2 - bababuck

@arsenm I still see this issue on `main` and I don't think anyone's working on it, could I get assigned? 

Clarification: I wasn't able to reproduce running `llvm-li`, but was able to reproduce running `llc -verify-machineinstrs -mtriple=amdgcn-amd-amdhsa -mcpu=gfx900 < test/CodeGen/AMDGPU/shufflevector-physreg-copy.ll` as mentioned https://github.com/llvm/llvm-project/commit/21b261102504c97fc0b81c101898c0f1c1a7e79c#commitcomment-153492325.

---

### Comment 3 - arsenm

Yes, the test forcibly disables the verifier so just running lit is insufficient 

---

### Comment 4 - bababuck

It seems like the bug is a result of the following IR:
```
  96B INLINEASM &"; def $0, $1" [attdialect], $0:[regdef], implicit-def $vgpr4, $1:[regdef], implicit-def $vgpr5
  112B undef %18.sub1:vreg_64 = COPY killed $vgpr4
  128B %18.sub0:vreg_64 = COPY killed $vgpr5
```
when it interacts with the machine scheduler. The second copy is moved in front of the first copy, but the second copy still is marked with `undef` triggering the error in the liveness verifier.

I have a patch that I will push once testing is completed.

---

