# "Bad machine code: No live segment at use" after scheduling

**Author:** arsenm
**URL:** https://github.com/llvm/llvm-project/issues/129028
**Status:** Open
**Labels:** backend:AMDGPU, llvm:codegen, crash-on-valid

## Body

```
RUN: at line 1: /Users/matt/src/llvm-project/build_rel_with_debinfo/bin/llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx908 -verify-machineinstrs -o - /Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/scheduler-no-live-segment-at-use.ll
+ /Users/matt/src/llvm-project/build_rel_with_debinfo/bin/llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx908 -verify-machineinstrs -o - /Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/scheduler-no-live-segment-at-use.ll

# After Machine Instruction Scheduler
********** INTERVALS **********
AGPR0_LO16 [48r,56r:0) 0@48r
AGPR0_HI16 [48r,56r:0) 0@48r
SGPR8_LO16 [0B,16r:0) 0@0B-phi
SGPR8_HI16 [0B,16r:0) 0@0B-phi
SGPR9_LO16 [0B,16r:0) 0@0B-phi
SGPR9_HI16 [0B,16r:0) 0@0B-phi
%6 [16r,32r:0) 0@16r  weight:0.000000e+00
%12 [80r,80d:0) 0@80r  weight:0.000000e+00
%13 [96r,336r:0) 0@96r  weight:0.000000e+00
%14 [112r,352r:0) 0@112r  weight:0.000000e+00
%15 [128r,368r:0) 0@128r  weight:0.000000e+00
%16 [144r,384r:0) 0@144r  weight:0.000000e+00
%17 [160r,400r:0) 0@160r  weight:0.000000e+00
%18 [176r,416r:0) 0@176r  weight:0.000000e+00
%19 [192r,432r:0) 0@192r  weight:0.000000e+00
%20 [208r,448r:0) 0@208r  weight:0.000000e+00
%21 [224r,464r:0) 0@224r  weight:0.000000e+00
%22 [240r,480r:0) 0@240r  weight:0.000000e+00
%23 [256r,496r:0) 0@256r  weight:0.000000e+00
%24 [272r,512r:0) 0@272r  weight:0.000000e+00
%25 [288r,528r:0) 0@288r  weight:0.000000e+00
%26 [304r,544r:0) 0@304r  weight:0.000000e+00
%27 [320r,836r:0) 0@320r  weight:0.000000e+00
%28 [32r,872r:0) 0@32r  L0000000000000003 [32r,832r:0) 0@32r  L000000000000000C [32r,872r:0) 0@32r  weight:0.000000e+00
%33 [864r,896r:0)[896r,912r:1) 0@864r 1@896r  weight:0.000000e+00
%34 [832r,976r:0) 0@832r  weight:0.000000e+00
%35 [872r,976r:0) 0@872r  weight:0.000000e+00
%37 [912r,928r:0)[928r,944r:1) 0@912r 1@928r  L0000000000030000 [912r,912d:0)[928r,944r:1) 0@912r 1@928r  L00000000FFFCFFFF [912r,944r:0) 0@912r  weight:0.000000e+00
%38 [944r,976r:0)[976r,992r:1) 0@944r 1@976r  L00000000000000C0 [944r,976r:0)[976r,992r:1) 0@944r 1@976r  L00000000FFFFFF3F [944r,976r:0)[976r,976d:1) 0@944r 1@976r  weight:0.000000e+00
%42 [992r,1008r:0) 0@992r  weight:0.000000e+00
%74 [56r,336r:0)[336r,352r:1)[352r,368r:2)[368r,384r:3)[384r,400r:4)[400r,416r:5)[416r,432r:6)[432r,448r:7)[448r,464r:8)[464r,480r:9)[480r,496r:10)[496r,512r:11)[512r,528r:12)[528r,544r:13)[544r,836r:14)[836r,928r:15) 0@56r 1@336r 2@352r 3@368r 4@384r 5@400r 6@416r 7@432r 8@448r 9@464r 10@480r 11@496r 12@512r 13@528r 14@544r 15@836r  L0000000000000003 [56r,928r:0) 0@56r  L000000000000000C [336r,864r:0) 0@336r  L0000000000000030 [352r,864r:0) 0@352r  L00000000000000C0 [368r,864r:0) 0@368r  L0000000000000300 [384r,864r:0) 0@384r  L0000000000000C00 [400r,864r:0) 0@400r  L0000000000003000 [416r,864r:0) 0@416r  L000000000000C000 [432r,864r:0) 0@432r  L0000000000030000 [448r,864r:0) 0@448r  L00000000000C0000 [464r,864r:0) 0@464r  L0000000000300000 [480r,864r:0) 0@480r  L0000000000C00000 [496r,864r:0) 0@496r  L0000000003000000 [512r,864r:0) 0@512r  L000000000C000000 [528r,864r:0) 0@528r  L0000000030000000 [544r,864r:0) 0@544r  L00000000C0000000 [836r,864r:0) 0@836r  weight:0.000000e+00
RegMasks:
********** MACHINEINSTRS **********
# Machine code for function alloc_failure_with_split_vregs: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $sgpr8_sgpr9 in %6

0B	bb.0 (%ir-block.0):
	  liveins: $sgpr8_sgpr9
16B	  %6:sgpr_64(p4) = COPY $sgpr8_sgpr9
32B	  %28:sreg_64_xexec = S_LOAD_DWORDX2_IMM %6:sgpr_64(p4), 0, 0 :: (dereferenceable invariant load (s64) from %ir.v0.kernarg.offset1, align 16, addrspace 4)
48B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef], implicit-def $agpr0
56B	  %74.sub0:av_512 = COPY $agpr0
80B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def dead %12:agpr_32
96B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %13:agpr_32
112B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %14:agpr_32
128B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %15:agpr_32
144B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %16:agpr_32
160B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %17:agpr_32
176B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %18:agpr_32
192B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %19:agpr_32
208B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %20:agpr_32
224B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %21:agpr_32
240B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %22:agpr_32
256B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %23:agpr_32
272B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %24:agpr_32
288B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %25:agpr_32
304B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %26:agpr_32
320B	  INLINEASM &"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %27:agpr_32
336B	  undef %74.sub1:av_512 = COPY %13:agpr_32
352B	  %74.sub2:av_512 = COPY %14:agpr_32
368B	  %74.sub3:av_512 = COPY %15:agpr_32
384B	  %74.sub4:av_512 = COPY %16:agpr_32
400B	  %74.sub5:av_512 = COPY %17:agpr_32
416B	  %74.sub6:av_512 = COPY %18:agpr_32
432B	  %74.sub7:av_512 = COPY %19:agpr_32
448B	  %74.sub8:av_512 = COPY %20:agpr_32
464B	  %74.sub9:av_512 = COPY %21:agpr_32
480B	  %74.sub10:av_512 = COPY %22:agpr_32
496B	  %74.sub11:av_512 = COPY %23:agpr_32
512B	  %74.sub12:av_512 = COPY %24:agpr_32
528B	  %74.sub13:av_512 = COPY %25:agpr_32
544B	  %74.sub14:av_512 = COPY %26:agpr_32
832B	  %34:vgpr_32 = COPY %28.sub0:sreg_64_xexec
836B	  %74.sub15:av_512 = COPY %27:agpr_32
864B	  %33:areg_512 = COPY %74:av_512
872B	  %35:vgpr_32 = COPY %28.sub1:sreg_64_xexec
896B	  %33:areg_512 = V_MFMA_F32_16X16X1F32_mac_e64 %34:vgpr_32, %35:vgpr_32, %33:areg_512(tied-def 0), 0, 0, 0, implicit $mode, implicit $exec
912B	  %37:vreg_512 = COPY %33:areg_512
928B	  %37.sub8:vreg_512 = COPY %74.sub0:av_512
944B	  %38:areg_512 = COPY %37:vreg_512
976B	  %38:areg_512 = V_MFMA_F32_16X16X1F32_mac_e64 %34:vgpr_32, %35:vgpr_32, %38:areg_512(tied-def 0), 0, 0, 0, implicit $mode, implicit $exec
992B	  %42:vgpr_32 = COPY %38.sub3:areg_512
1008B	  $agpr1 = COPY %42:vgpr_32
1024B	  INLINEASM &"; use $0" [sideeffect] [attdialect], $0:[reguse], killed $agpr1
1040B	  S_ENDPGM 0

# End machine code for function alloc_failure_with_split_vregs.

*** Bad machine code: No live segment at use ***
- function:    alloc_failure_with_split_vregs
- basic block: %bb.0  (0x129876720) [0B;1056B)
- instruction: 56B	%74.sub0:av_512 = COPY $agpr0
- operand 0:   %74.sub0:av_512
- liverange:   [56r,336r:0)[336r,352r:1)[352r,368r:2)[368r,384r:3)[384r,400r:4)[400r,416r:5)[416r,432r:6)[432r,448r:7)[448r,464r:8)[464r,480r:9)[480r,496r:10)[496r,512r:11)[512r,528r:12)[528r,544r:13)[544r,836r:14)[836r,928r:15) 0@56r 1@336r 2@352r 3@368r 4@384r 5@400r 6@416r 7@432r 8@448r 9@464r 10@480r 11@496r 12@512r 13@528r 14@544r 15@836r
- v. register: %74
- at:          56B
LLVM ERROR: Found 1 machine code errors.
```

[scheduler-no-live-segment-at-use.ll.txt](https://github.com/user-attachments/files/19006360/scheduler-no-live-segment-at-use.ll.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Matt Arsenault (arsenm)

<details>
```
RUN: at line 1: /Users/matt/src/llvm-project/build_rel_with_debinfo/bin/llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx908 -verify-machineinstrs -o - /Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/scheduler-no-live-segment-at-use.ll
+ /Users/matt/src/llvm-project/build_rel_with_debinfo/bin/llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx908 -verify-machineinstrs -o - /Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/scheduler-no-live-segment-at-use.ll

# After Machine Instruction Scheduler
********** INTERVALS **********
AGPR0_LO16 [48r,56r:0) 0@<!-- -->48r
AGPR0_HI16 [48r,56r:0) 0@<!-- -->48r
SGPR8_LO16 [0B,16r:0) 0@<!-- -->0B-phi
SGPR8_HI16 [0B,16r:0) 0@<!-- -->0B-phi
SGPR9_LO16 [0B,16r:0) 0@<!-- -->0B-phi
SGPR9_HI16 [0B,16r:0) 0@<!-- -->0B-phi
%6 [16r,32r:0) 0@<!-- -->16r  weight:0.000000e+00
%12 [80r,80d:0) 0@<!-- -->80r  weight:0.000000e+00
%13 [96r,336r:0) 0@<!-- -->96r  weight:0.000000e+00
%14 [112r,352r:0) 0@<!-- -->112r  weight:0.000000e+00
%15 [128r,368r:0) 0@<!-- -->128r  weight:0.000000e+00
%16 [144r,384r:0) 0@<!-- -->144r  weight:0.000000e+00
%17 [160r,400r:0) 0@<!-- -->160r  weight:0.000000e+00
%18 [176r,416r:0) 0@<!-- -->176r  weight:0.000000e+00
%19 [192r,432r:0) 0@<!-- -->192r  weight:0.000000e+00
%20 [208r,448r:0) 0@<!-- -->208r  weight:0.000000e+00
%21 [224r,464r:0) 0@<!-- -->224r  weight:0.000000e+00
%22 [240r,480r:0) 0@<!-- -->240r  weight:0.000000e+00
%23 [256r,496r:0) 0@<!-- -->256r  weight:0.000000e+00
%24 [272r,512r:0) 0@<!-- -->272r  weight:0.000000e+00
%25 [288r,528r:0) 0@<!-- -->288r  weight:0.000000e+00
%26 [304r,544r:0) 0@<!-- -->304r  weight:0.000000e+00
%27 [320r,836r:0) 0@<!-- -->320r  weight:0.000000e+00
%28 [32r,872r:0) 0@<!-- -->32r  L0000000000000003 [32r,832r:0) 0@<!-- -->32r  L000000000000000C [32r,872r:0) 0@<!-- -->32r  weight:0.000000e+00
%33 [864r,896r:0)[896r,912r:1) 0@<!-- -->864r 1@<!-- -->896r  weight:0.000000e+00
%34 [832r,976r:0) 0@<!-- -->832r  weight:0.000000e+00
%35 [872r,976r:0) 0@<!-- -->872r  weight:0.000000e+00
%37 [912r,928r:0)[928r,944r:1) 0@<!-- -->912r 1@<!-- -->928r  L0000000000030000 [912r,912d:0)[928r,944r:1) 0@<!-- -->912r 1@<!-- -->928r  L00000000FFFCFFFF [912r,944r:0) 0@<!-- -->912r  weight:0.000000e+00
%38 [944r,976r:0)[976r,992r:1) 0@<!-- -->944r 1@<!-- -->976r  L00000000000000C0 [944r,976r:0)[976r,992r:1) 0@<!-- -->944r 1@<!-- -->976r  L00000000FFFFFF3F [944r,976r:0)[976r,976d:1) 0@<!-- -->944r 1@<!-- -->976r  weight:0.000000e+00
%42 [992r,1008r:0) 0@<!-- -->992r  weight:0.000000e+00
%74 [56r,336r:0)[336r,352r:1)[352r,368r:2)[368r,384r:3)[384r,400r:4)[400r,416r:5)[416r,432r:6)[432r,448r:7)[448r,464r:8)[464r,480r:9)[480r,496r:10)[496r,512r:11)[512r,528r:12)[528r,544r:13)[544r,836r:14)[836r,928r:15) 0@<!-- -->56r 1@<!-- -->336r 2@<!-- -->352r 3@<!-- -->368r 4@<!-- -->384r 5@<!-- -->400r 6@<!-- -->416r 7@<!-- -->432r 8@<!-- -->448r 9@<!-- -->464r 10@<!-- -->480r 11@<!-- -->496r 12@<!-- -->512r 13@<!-- -->528r 14@<!-- -->544r 15@<!-- -->836r  L0000000000000003 [56r,928r:0) 0@<!-- -->56r  L000000000000000C [336r,864r:0) 0@<!-- -->336r  L0000000000000030 [352r,864r:0) 0@<!-- -->352r  L00000000000000C0 [368r,864r:0) 0@<!-- -->368r  L0000000000000300 [384r,864r:0) 0@<!-- -->384r  L0000000000000C00 [400r,864r:0) 0@<!-- -->400r  L0000000000003000 [416r,864r:0) 0@<!-- -->416r  L000000000000C000 [432r,864r:0) 0@<!-- -->432r  L0000000000030000 [448r,864r:0) 0@<!-- -->448r  L00000000000C0000 [464r,864r:0) 0@<!-- -->464r  L0000000000300000 [480r,864r:0) 0@<!-- -->480r  L0000000000C00000 [496r,864r:0) 0@<!-- -->496r  L0000000003000000 [512r,864r:0) 0@<!-- -->512r  L000000000C000000 [528r,864r:0) 0@<!-- -->528r  L0000000030000000 [544r,864r:0) 0@<!-- -->544r  L00000000C0000000 [836r,864r:0) 0@<!-- -->836r  weight:0.000000e+00
RegMasks:
********** MACHINEINSTRS **********
# Machine code for function alloc_failure_with_split_vregs: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $sgpr8_sgpr9 in %6

0B	bb.0 (%ir-block.0):
	  liveins: $sgpr8_sgpr9
16B	  %6:sgpr_64(p4) = COPY $sgpr8_sgpr9
32B	  %28:sreg_64_xexec = S_LOAD_DWORDX2_IMM %6:sgpr_64(p4), 0, 0 :: (dereferenceable invariant load (s64) from %ir.v0.kernarg.offset1, align 16, addrspace 4)
48B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef], implicit-def $agpr0
56B	  %74.sub0:av_512 = COPY $agpr0
80B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def dead %12:agpr_32
96B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %13:agpr_32
112B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %14:agpr_32
128B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %15:agpr_32
144B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %16:agpr_32
160B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %17:agpr_32
176B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %18:agpr_32
192B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %19:agpr_32
208B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %20:agpr_32
224B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %21:agpr_32
240B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %22:agpr_32
256B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %23:agpr_32
272B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %24:agpr_32
288B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %25:agpr_32
304B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %26:agpr_32
320B	  INLINEASM &amp;"; def $0" [sideeffect] [attdialect], $0:[regdef:AGPR_32], def %27:agpr_32
336B	  undef %74.sub1:av_512 = COPY %13:agpr_32
352B	  %74.sub2:av_512 = COPY %14:agpr_32
368B	  %74.sub3:av_512 = COPY %15:agpr_32
384B	  %74.sub4:av_512 = COPY %16:agpr_32
400B	  %74.sub5:av_512 = COPY %17:agpr_32
416B	  %74.sub6:av_512 = COPY %18:agpr_32
432B	  %74.sub7:av_512 = COPY %19:agpr_32
448B	  %74.sub8:av_512 = COPY %20:agpr_32
464B	  %74.sub9:av_512 = COPY %21:agpr_32
480B	  %74.sub10:av_512 = COPY %22:agpr_32
496B	  %74.sub11:av_512 = COPY %23:agpr_32
512B	  %74.sub12:av_512 = COPY %24:agpr_32
528B	  %74.sub13:av_512 = COPY %25:agpr_32
544B	  %74.sub14:av_512 = COPY %26:agpr_32
832B	  %34:vgpr_32 = COPY %28.sub0:sreg_64_xexec
836B	  %74.sub15:av_512 = COPY %27:agpr_32
864B	  %33:areg_512 = COPY %74:av_512
872B	  %35:vgpr_32 = COPY %28.sub1:sreg_64_xexec
896B	  %33:areg_512 = V_MFMA_F32_16X16X1F32_mac_e64 %34:vgpr_32, %35:vgpr_32, %33:areg_512(tied-def 0), 0, 0, 0, implicit $mode, implicit $exec
912B	  %37:vreg_512 = COPY %33:areg_512
928B	  %37.sub8:vreg_512 = COPY %74.sub0:av_512
944B	  %38:areg_512 = COPY %37:vreg_512
976B	  %38:areg_512 = V_MFMA_F32_16X16X1F32_mac_e64 %34:vgpr_32, %35:vgpr_32, %38:areg_512(tied-def 0), 0, 0, 0, implicit $mode, implicit $exec
992B	  %42:vgpr_32 = COPY %38.sub3:areg_512
1008B	  $agpr1 = COPY %42:vgpr_32
1024B	  INLINEASM &amp;"; use $0" [sideeffect] [attdialect], $0:[reguse], killed $agpr1
1040B	  S_ENDPGM 0

# End machine code for function alloc_failure_with_split_vregs.

*** Bad machine code: No live segment at use ***
- function:    alloc_failure_with_split_vregs
- basic block: %bb.0  (0x129876720) [0B;1056B)
- instruction: 56B	%74.sub0:av_512 = COPY $agpr0
- operand 0:   %74.sub0:av_512
- liverange:   [56r,336r:0)[336r,352r:1)[352r,368r:2)[368r,384r:3)[384r,400r:4)[400r,416r:5)[416r,432r:6)[432r,448r:7)[448r,464r:8)[464r,480r:9)[480r,496r:10)[496r,512r:11)[512r,528r:12)[528r,544r:13)[544r,836r:14)[836r,928r:15) 0@<!-- -->56r 1@<!-- -->336r 2@<!-- -->352r 3@<!-- -->368r 4@<!-- -->384r 5@<!-- -->400r 6@<!-- -->416r 7@<!-- -->432r 8@<!-- -->448r 9@<!-- -->464r 10@<!-- -->480r 11@<!-- -->496r 12@<!-- -->512r 13@<!-- -->528r 14@<!-- -->544r 15@<!-- -->836r
- v. register: %74
- at:          56B
LLVM ERROR: Found 1 machine code errors.
```

[scheduler-no-live-segment-at-use.ll.txt](https://github.com/user-attachments/files/19006360/scheduler-no-live-segment-at-use.ll.txt)
</details>


---

### Comment 2 - arsenm

Hacking on the same testcase hits an assertion inside scheduling: [must-have-following-segment.ll.txt](https://github.com/user-attachments/files/19006529/must-have-following-segment.ll.txt)


```
Assertion failed: (INext != E && "Must have following segment"), function handleMoveDown, file LiveIntervals.cpp, line 1243.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx908 -o - /Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/remaining-virtual-register-operands.ll
1.	Running pass 'CallGraph Pass Manager' on module '/Users/matt/src/llvm-project/llvm/test/CodeGen/AMDGPU/remaining-virtual-register-operands.ll'.
2.	Running pass 'Machine Instruction Scheduler' on function '@alloc_failure_with_split_vregs'
 #0 0x00000001032694b4 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/llc+0x102fd14b4)
 #1 0x00000001032679d0 llvm::sys::RunSignalHandlers() (/usr/local/bin/llc+0x102fcf9d0)
 #2 0x0000000103269afc SignalHandler(int, __siginfo*, void*) (/usr/local/bin/llc+0x102fd1afc)
 #3 0x000000019a106de4 (/usr/lib/system/libsystem_platform.dylib+0x180482de4)
 #4 0x000000019a0cff70 (/usr/lib/system/libsystem_pthread.dylib+0x18044bf70)
 #5 0x0000000199fdc908 (/usr/lib/system/libsystem_c.dylib+0x180358908)
 #6 0x0000000199fdbc1c (/usr/lib/system/libsystem_c.dylib+0x180357c1c)
 #7 0x000000010333812c llvm::LiveIntervals::HMEditor::handleMoveDown(llvm::LiveRange&) (.cold.8) (/usr/local/bin/llc+0x1030a012c)
 #8 0x0000000102275960 llvm::LiveIntervals::HMEditor::handleMoveDown(llvm::LiveRange&) (/usr/local/bin/llc+0x101fdd960)
 #9 0x000000010227515c llvm::LiveIntervals::HMEditor::updateRange(llvm::LiveRange&, llvm::VirtRegOrUnit, llvm::LaneBitmask) (/usr/local/bin/llc+0x101fdd15c)
#10 0x0000000102273af4 llvm::LiveIntervals::HMEditor::updateAllRanges(llvm::MachineInstr*) (/usr/local/bin/llc+0x101fdbaf4)
#11 0x0000000102273720 llvm::LiveIntervals::handleMove(llvm::MachineInstr&, bool) (/usr/local/bin/llc+0x101fdb720)
#12 0x000000010237e248 llvm::ScheduleDAGMI::moveInstruction(llvm::MachineInstr*, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>) (/usr/local/bin/llc+0x1020e6248)
#13 0x0000000102383528 llvm::ScheduleDAGMILive::scheduleMI(llvm::SUnit*, bool) (/usr/local/bin/llc+0x1020eb528)
#14 0x0000000102382edc llvm::ScheduleDAGMILive::schedule() (/usr/local/bin/llc+0x1020eaedc)
#15 0x000000010088bdd0 llvm::GCNScheduleDAGMILive::runSchedStages() (/usr/local/bin/llc+0x1005f3dd0)
#16 0x000000010237cbbc llvm::impl_detail::MachineSchedulerBase::scheduleRegions(llvm::ScheduleDAGInstrs&, bool) (/usr/local/bin/llc+0x1020e4bbc)

```

---

### Comment 3 - arsenm

Possibly related https://github.com/llvm/llvm-project/issues/130884

---

