# AMDGPU: Using an undefined physical register in testcase using compare over kill

**Author:** arsenm
**URL:** https://github.com/llvm/llvm-project/issues/131298
**Status:** Closed
**Labels:** backend:AMDGPU, llvm:codegen, crash-on-valid
**Closed Date:** 2025-04-30T14:02:47Z

## Body

This testcase, derived from llvm/test/CodeGen/AMDGPU/skip-if-dead.ll fails the verifier:

```
; RUN: llc -mtriple=amdgcn -mcpu=tahiti -verify-machineinstrs < %s | FileCheck -check-prefixes=GCN,WAVE64,SI %s

define amdgpu_ps void @phi_use_def_before_kill(float inreg %x, i32 inreg %y) #0 {
bb:
  %tmp = fadd float %x, 1.000000e+00
  %tmp1 = fcmp olt float 0.000000e+00, %tmp
  %tmp2 = select i1 %tmp1, float -1.000000e+00, float 0.000000e+00
  %cmp.tmp2 = fcmp olt float %tmp2, 0.000000e+00
  %uniform.cond = icmp eq i32 %y, 0
  call void @llvm.amdgcn.kill(i1 %cmp.tmp2)
  br i1 %uniform.cond, label %phibb, label %bb8

phibb:                                            ; preds = %bb8, %bb
  %tmp5 = phi float [ %tmp2, %bb ], [ 4.000000e+00, %bb8 ]
  %tmp6 = fcmp oeq float %tmp5, 0.000000e+00
  br i1 %tmp6, label %bb10, label %end

bb8:                                              ; preds = %bb
  store volatile i32 8, ptr addrspace(1) poison, align 4
  br label %phibb

bb10:                                             ; preds = %phibb
  store volatile i32 9, ptr addrspace(1) poison, align 4
  br label %end

end:                                              ; preds = %bb10, %phibb
  ret void
}

declare void @llvm.amdgcn.exp.f32(i32 immarg, i32 immarg, float, float, float, float, i1 immarg, i1 immarg) #1
declare void @llvm.amdgcn.kill(i1) #2
declare void @llvm.amdgcn.s.setreg(i32 immarg, i32) #3
declare float @llvm.amdgcn.image.sample.l.2darray.f32.f32.v8i32.v4i32(i32 immarg, float, float, float, float, <8 x i32>, <4 x i32>, i1 immarg, i32 immarg, i32 immarg) #4
declare <4 x float> @llvm.amdgcn.image.sample.c.1d.v4f32.f32.v8i32.v4i32(i32 immarg, float, float, <8 x i32>, <4 x i32>, i1 immarg, i32 immarg, i32 immarg) #5

attributes #0 = { nounwind }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #2 = { nocallback nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind willreturn }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(read) }
attributes #5 = { convergent nocallback nofree nosync nounwind willreturn memory(read) }

```


```
# After Instruction Selection
# Machine code for function phi_use_def_before_kill: IsSSA, TracksLiveness
Function Live Ins: $sgpr0 in %2, $sgpr1 in %3

bb.0.bb:
  successors: %bb.5(0x80000000); %bb.5(100.00%)
  liveins: $sgpr0, $sgpr1
  %3:sgpr_32 = COPY $sgpr1
  %2:sgpr_32 = COPY $sgpr0
  %5:sgpr_32 = S_MOV_B32 1065353216
  %7:vgpr_32 = COPY killed %5:sgpr_32
  %6:vgpr_32 = nofpexcept V_ADD_F32_e64 0, %2:sgpr_32, 0, %7:vgpr_32, 0, 0, implicit $mode, implicit $exec
  %8:sgpr_32 = S_MOV_B32 0
  %9:sreg_64_xexec = nofpexcept V_CMP_GT_F32_e64 0, %6:vgpr_32, 0, %8:sgpr_32, 0, implicit $mode, implicit $exec
  %10:sgpr_32 = S_MOV_B32 -1082130432
  %12:vgpr_32 = COPY %8:sgpr_32
  %13:vgpr_32 = COPY killed %10:sgpr_32
  %11:vgpr_32 = V_CNDMASK_B32_e64 0, %12:vgpr_32, 0, %13:vgpr_32, killed %9:sreg_64_xexec, implicit $exec
  %0:sgpr_32 = IMPLICIT_DEF
  %14:sreg_32 = S_MOV_B32 0
  S_CMP_LG_U32 %3:sgpr_32, killed %14:sreg_32, implicit-def $scc
  SI_KILL_F32_COND_IMM_TERMINATOR %6:vgpr_32, 0, 2, implicit-def dead $vcc, implicit $exec

bb.5.bb:
; predecessors: %bb.0
  successors: %bb.2(0x50000000), %bb.1(0x30000000); %bb.2(62.50%), %bb.1(37.50%)

  S_CBRANCH_SCC1 %bb.2, implicit $scc
  S_BRANCH %bb.1

bb.1.phibb:
; predecessors: %bb.2, %bb.5
  successors: %bb.3(0x50000000), %bb.4(0x30000000); %bb.3(62.50%), %bb.4(37.50%)

  %36:vgpr_32 = PHI %11:vgpr_32, %bb.5, %37:vgpr_32, %bb.2
  %24:sgpr_32 = S_MOV_B32 0
  %26:sgpr_32 = COPY killed %24:sgpr_32
  %25:sreg_64 = nofpexcept V_CMP_EQ_F32_e64 0, %36:vgpr_32, 0, %26:sgpr_32, 0, implicit $mode, implicit $exec
  %27:sreg_64 = S_AND_B64 $exec, killed %25:sreg_64, implicit-def dead $scc
  $vcc = COPY %27:sreg_64
  S_CBRANCH_VCCNZ %bb.3, implicit $vcc
  S_BRANCH %bb.4

bb.2.bb8:
; predecessors: %bb.5
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  %16:sreg_64 = IMPLICIT_DEF
  %17:sreg_32 = COPY %16.sub1:sreg_64
  %18:sreg_64 = IMPLICIT_DEF
  %19:sreg_32 = COPY %18.sub0:sreg_64
  %20:sreg_32 = S_MOV_B32 61440
  %21:sreg_32 = S_MOV_B32 -1
  %22:sgpr_128 = REG_SEQUENCE killed %19:sreg_32, %subreg.sub0, killed %17:sreg_32, %subreg.sub1, killed %21:sreg_32, %subreg.sub2, killed %20:sreg_32, %subreg.sub3
  %23:vgpr_32 = V_MOV_B32_e32 8, implicit $exec
  BUFFER_STORE_DWORD_OFFSET killed %23:vgpr_32, killed %22:sgpr_128, 0, 0, 0, 0, implicit $exec :: (volatile store (s32) into `ptr addrspace(1) poison`, addrspace 1)
  %15:sgpr_32 = S_MOV_B32 1082130432
  %37:vgpr_32 = V_MOV_B32_e32 1082130432, implicit $exec
  S_BRANCH %bb.1

bb.3.bb10:
; predecessors: %bb.1
  successors: %bb.4(0x80000000); %bb.4(100.00%)

  %28:sreg_64 = IMPLICIT_DEF
  %29:sreg_32 = COPY %28.sub1:sreg_64
  %30:sreg_64 = IMPLICIT_DEF
  %31:sreg_32 = COPY %30.sub0:sreg_64
  %32:sreg_32 = S_MOV_B32 61440
  %33:sreg_32 = S_MOV_B32 -1
  %34:sgpr_128 = REG_SEQUENCE killed %31:sreg_32, %subreg.sub0, killed %29:sreg_32, %subreg.sub1, killed %33:sreg_32, %subreg.sub2, killed %32:sreg_32, %subreg.sub3
  %35:vgpr_32 = V_MOV_B32_e32 9, implicit $exec
  BUFFER_STORE_DWORD_OFFSET killed %35:vgpr_32, killed %34:sgpr_128, 0, 0, 0, 0, implicit $exec :: (volatile store (s32) into `ptr addrspace(1) poison`, addrspace 1)

bb.4.end:
; predecessors: %bb.1, %bb.3

  S_ENDPGM 0

# End machine code for function phi_use_def_before_kill.

*** Bad machine code: Using an undefined physical register ***
- function:    phi_use_def_before_kill
- basic block: %bb.5 bb (0x14608b290)
- instruction: S_CBRANCH_SCC1 %bb.2, implicit $scc
- operand 1:   implicit $scc
LLVM ERROR: Found 1 machine code errors.

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Matt Arsenault (arsenm)

<details>
This testcase, derived from llvm/test/CodeGen/AMDGPU/skip-if-dead.ll fails the verifier:

```
; RUN: llc -mtriple=amdgcn -mcpu=tahiti -verify-machineinstrs &lt; %s | FileCheck -check-prefixes=GCN,WAVE64,SI %s

define amdgpu_ps void @<!-- -->phi_use_def_before_kill(float inreg %x, i32 inreg %y) #<!-- -->0 {
bb:
  %tmp = fadd float %x, 1.000000e+00
  %tmp1 = fcmp olt float 0.000000e+00, %tmp
  %tmp2 = select i1 %tmp1, float -1.000000e+00, float 0.000000e+00
  %cmp.tmp2 = fcmp olt float %tmp2, 0.000000e+00
  %uniform.cond = icmp eq i32 %y, 0
  call void @<!-- -->llvm.amdgcn.kill(i1 %cmp.tmp2)
  br i1 %uniform.cond, label %phibb, label %bb8

phibb:                                            ; preds = %bb8, %bb
  %tmp5 = phi float [ %tmp2, %bb ], [ 4.000000e+00, %bb8 ]
  %tmp6 = fcmp oeq float %tmp5, 0.000000e+00
  br i1 %tmp6, label %bb10, label %end

bb8:                                              ; preds = %bb
  store volatile i32 8, ptr addrspace(1) poison, align 4
  br label %phibb

bb10:                                             ; preds = %phibb
  store volatile i32 9, ptr addrspace(1) poison, align 4
  br label %end

end:                                              ; preds = %bb10, %phibb
  ret void
}

declare void @<!-- -->llvm.amdgcn.exp.f32(i32 immarg, i32 immarg, float, float, float, float, i1 immarg, i1 immarg) #<!-- -->1
declare void @<!-- -->llvm.amdgcn.kill(i1) #<!-- -->2
declare void @<!-- -->llvm.amdgcn.s.setreg(i32 immarg, i32) #<!-- -->3
declare float @<!-- -->llvm.amdgcn.image.sample.l.2darray.f32.f32.v8i32.v4i32(i32 immarg, float, float, float, float, &lt;8 x i32&gt;, &lt;4 x i32&gt;, i1 immarg, i32 immarg, i32 immarg) #<!-- -->4
declare &lt;4 x float&gt; @<!-- -->llvm.amdgcn.image.sample.c.1d.v4f32.f32.v8i32.v4i32(i32 immarg, float, float, &lt;8 x i32&gt;, &lt;4 x i32&gt;, i1 immarg, i32 immarg, i32 immarg) #<!-- -->5

attributes #<!-- -->0 = { nounwind }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #<!-- -->2 = { nocallback nofree nounwind }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn }
attributes #<!-- -->4 = { nocallback nofree nosync nounwind willreturn memory(read) }
attributes #<!-- -->5 = { convergent nocallback nofree nosync nounwind willreturn memory(read) }

```


```
# After Instruction Selection
# Machine code for function phi_use_def_before_kill: IsSSA, TracksLiveness
Function Live Ins: $sgpr0 in %2, $sgpr1 in %3

bb.0.bb:
  successors: %bb.5(0x80000000); %bb.5(100.00%)
  liveins: $sgpr0, $sgpr1
  %3:sgpr_32 = COPY $sgpr1
  %2:sgpr_32 = COPY $sgpr0
  %5:sgpr_32 = S_MOV_B32 1065353216
  %7:vgpr_32 = COPY killed %5:sgpr_32
  %6:vgpr_32 = nofpexcept V_ADD_F32_e64 0, %2:sgpr_32, 0, %7:vgpr_32, 0, 0, implicit $mode, implicit $exec
  %8:sgpr_32 = S_MOV_B32 0
  %9:sreg_64_xexec = nofpexcept V_CMP_GT_F32_e64 0, %6:vgpr_32, 0, %8:sgpr_32, 0, implicit $mode, implicit $exec
  %10:sgpr_32 = S_MOV_B32 -1082130432
  %12:vgpr_32 = COPY %8:sgpr_32
  %13:vgpr_32 = COPY killed %10:sgpr_32
  %11:vgpr_32 = V_CNDMASK_B32_e64 0, %12:vgpr_32, 0, %13:vgpr_32, killed %9:sreg_64_xexec, implicit $exec
  %0:sgpr_32 = IMPLICIT_DEF
  %14:sreg_32 = S_MOV_B32 0
  S_CMP_LG_U32 %3:sgpr_32, killed %14:sreg_32, implicit-def $scc
  SI_KILL_F32_COND_IMM_TERMINATOR %6:vgpr_32, 0, 2, implicit-def dead $vcc, implicit $exec

bb.5.bb:
; predecessors: %bb.0
  successors: %bb.2(0x50000000), %bb.1(0x30000000); %bb.2(62.50%), %bb.1(37.50%)

  S_CBRANCH_SCC1 %bb.2, implicit $scc
  S_BRANCH %bb.1

bb.1.phibb:
; predecessors: %bb.2, %bb.5
  successors: %bb.3(0x50000000), %bb.4(0x30000000); %bb.3(62.50%), %bb.4(37.50%)

  %36:vgpr_32 = PHI %11:vgpr_32, %bb.5, %37:vgpr_32, %bb.2
  %24:sgpr_32 = S_MOV_B32 0
  %26:sgpr_32 = COPY killed %24:sgpr_32
  %25:sreg_64 = nofpexcept V_CMP_EQ_F32_e64 0, %36:vgpr_32, 0, %26:sgpr_32, 0, implicit $mode, implicit $exec
  %27:sreg_64 = S_AND_B64 $exec, killed %25:sreg_64, implicit-def dead $scc
  $vcc = COPY %27:sreg_64
  S_CBRANCH_VCCNZ %bb.3, implicit $vcc
  S_BRANCH %bb.4

bb.2.bb8:
; predecessors: %bb.5
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  %16:sreg_64 = IMPLICIT_DEF
  %17:sreg_32 = COPY %16.sub1:sreg_64
  %18:sreg_64 = IMPLICIT_DEF
  %19:sreg_32 = COPY %18.sub0:sreg_64
  %20:sreg_32 = S_MOV_B32 61440
  %21:sreg_32 = S_MOV_B32 -1
  %22:sgpr_128 = REG_SEQUENCE killed %19:sreg_32, %subreg.sub0, killed %17:sreg_32, %subreg.sub1, killed %21:sreg_32, %subreg.sub2, killed %20:sreg_32, %subreg.sub3
  %23:vgpr_32 = V_MOV_B32_e32 8, implicit $exec
  BUFFER_STORE_DWORD_OFFSET killed %23:vgpr_32, killed %22:sgpr_128, 0, 0, 0, 0, implicit $exec :: (volatile store (s32) into `ptr addrspace(1) poison`, addrspace 1)
  %15:sgpr_32 = S_MOV_B32 1082130432
  %37:vgpr_32 = V_MOV_B32_e32 1082130432, implicit $exec
  S_BRANCH %bb.1

bb.3.bb10:
; predecessors: %bb.1
  successors: %bb.4(0x80000000); %bb.4(100.00%)

  %28:sreg_64 = IMPLICIT_DEF
  %29:sreg_32 = COPY %28.sub1:sreg_64
  %30:sreg_64 = IMPLICIT_DEF
  %31:sreg_32 = COPY %30.sub0:sreg_64
  %32:sreg_32 = S_MOV_B32 61440
  %33:sreg_32 = S_MOV_B32 -1
  %34:sgpr_128 = REG_SEQUENCE killed %31:sreg_32, %subreg.sub0, killed %29:sreg_32, %subreg.sub1, killed %33:sreg_32, %subreg.sub2, killed %32:sreg_32, %subreg.sub3
  %35:vgpr_32 = V_MOV_B32_e32 9, implicit $exec
  BUFFER_STORE_DWORD_OFFSET killed %35:vgpr_32, killed %34:sgpr_128, 0, 0, 0, 0, implicit $exec :: (volatile store (s32) into `ptr addrspace(1) poison`, addrspace 1)

bb.4.end:
; predecessors: %bb.1, %bb.3

  S_ENDPGM 0

# End machine code for function phi_use_def_before_kill.

*** Bad machine code: Using an undefined physical register ***
- function:    phi_use_def_before_kill
- basic block: %bb.5 bb (0x14608b290)
- instruction: S_CBRANCH_SCC1 %bb.2, implicit $scc
- operand 1:   implicit $scc
LLVM ERROR: Found 1 machine code errors.

```
</details>


---

