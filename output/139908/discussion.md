# [AMDGPU] SIFoldOperands incorrectly folding constant splat reg sequence

**Author:** npmiller
**URL:** https://github.com/llvm/llvm-project/issues/139908
**Status:** Closed
**Labels:** backend:AMDGPU, miscompilation
**Closed Date:** 2025-05-19T19:44:46Z

## Body

## Description

The fold operands pass tries to fold constants into their uses even folding register sequences.

In some cases it does it when it shouldn't, for example see the following MIR:
```
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, %15, implicit $exec
```

Is getting folded into this:
```
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, 42, implicit $exec
```

As you can see the 42 was moved over the register sequence and directly into the comparison instruction. This is incorrect because now the comparison will only check against the value 42, instead of a 64 bit splat of two 42s.

There are some constant values for which this is valid, for example `-1`, or `0` would be fine, since the splat is essentially just extending them from 32 bits to 64 bits, which the immediate operand can handle. But this is not correct in the general case.

Some lit tests are checking the splats of `-1` get folded by the pass (for example `test/CodeGen/AMDGPU/GlobalISel/andn2.ll`).

This folding is handled in [`SIFoldOperands.cpp:919`](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/AMDGPU/SIFoldOperands.cpp#L919).

If the register sequence has more than one register it does the following check:
```cpp
      if (!TII->isInlineConstant(*Op, OpTy) ||
          !TII->isOperandLegal(*UseMI, UseOpIdx, Op))
        return false;
```

This only ensures that the specific value can be folded as an immediate, it doesn't check if the composition of the values in the register sequence can be represented as an immediate.

Additionally the folding is only performed for splats:
```cpp
    if (Imm != SubImm)
      return false; // Can only fold splat constants
```

So clearly this code needs additional checks to avoid invalid folds, but I'm not sure exactly what they should be. It's possible some instructions are able to handle these folded splats, but it's definitely being done for some instructions that can't.

Note: `isInlineConstant` checks that the value is between `-16` and `64`, so a larger constant value in the splat doesn't trigger the issue.

Note: this was discovered downstream in [intel/llvm](https://github.com/intel/llvm), during SYCL testing.

## Reproducer

```
; ModuleID = 'repro.bc'
target datalayout = "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32-p7:160:256:256:32-p8:128:128-p9:192:256:256:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5-G1-ni:7:8:9"
target triple = "amdgcn-amd-amdhsa"

define i32 @repro_kernel(i64 %in) {
entry:
  %.not = icmp eq i64 %in, 180388626474
  br i1 %.not, label %2, label %1
1:
  ret i32 1

2:
  ret i32 2
}
```

Incorrect assembly snippet from that IR file:
```
$ ./bin/llc repro.ll -o repro.s
$ cat repro.s
[...]
repro_kernel:                           ; @repro_kernel
; %bb.0:                                ; %entry
        s_waitcnt vmcnt(0) expcnt(0) lgkmcnt(0) 
        v_cmp_eq_u64_e32 vcc, 42, v[0:1]
        v_mov_b32_e32 v0, 1
        s_and_saveexec_b64 s[4:5], vcc
        s_cbranch_execz .LBB0_2                                                                
; %bb.1:                           
        v_mov_b32_e32 v0, 2        
.LBB0_2:                                ; %UnifiedReturnBlock
        s_or_b64 exec, exec, s[4:5]
        s_setpc_b64 s[30:31]
[...]
```
MIR snippet before the fold operands pass:
```
$ ./bin/llc repro.ll -stop-before=si-fold-operands -o before.mir
$ cat before.mir
[...]
body:             |
  bb.0.entry:
    successors: %bb.1(0x40000000), %bb.2(0x40000000)
    liveins: $vgpr0, $vgpr1
  
    %11:vgpr_32 = COPY $vgpr1
    %10:vgpr_32 = COPY $vgpr0
    %21:vreg_64 = REG_SEQUENCE %10, %subreg.sub0, %11, %subreg.sub1
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, %15, implicit $exec
    %12:vgpr_32 = V_MOV_B32_e32 1, implicit $exec
    %0:sreg_64 = SI_IF killed %16, %bb.2, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    S_BRANCH %bb.1
  
  bb.1 (%ir-block.3):
    successors: %bb.2(0x80000000)
  
    %18:vgpr_32 = V_MOV_B32_e32 2, implicit $exec
  
  bb.2.UnifiedReturnBlock:
    %1:vgpr_32 = PHI %12, %bb.0, %18, %bb.1
    SI_END_CF %0, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    $vgpr0 = COPY %1
    SI_RETURN implicit $vgpr0
[...]
```
MIR snippet after running the fold operands pass:
```
$ ./bin/llc before.mir --run-pass=si-fold-operands -o bad.mir
$ cat bad.mir
[...]
body:             |
  bb.0.entry:
    successors: %bb.1(0x40000000), %bb.2(0x40000000)
    liveins: $vgpr0, $vgpr1
  
    %11:vgpr_32 = COPY $vgpr1
    %10:vgpr_32 = COPY $vgpr0
    %21:vreg_64 = REG_SEQUENCE %10, %subreg.sub0, %11, %subreg.sub1
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, 42, implicit $exec
    %12:vgpr_32 = V_MOV_B32_e32 1, implicit $exec
    %0:sreg_64 = SI_IF killed %16, %bb.2, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    S_BRANCH %bb.1
  
  bb.1 (%ir-block.3):
    successors: %bb.2(0x80000000)
  
    %18:vgpr_32 = V_MOV_B32_e32 2, implicit $exec
  
  bb.2.UnifiedReturnBlock:
    %1:vgpr_32 = PHI %12, %bb.0, %18, %bb.1
    SI_END_CF %0, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    $vgpr0 = COPY %1
    SI_RETURN implicit $vgpr0
[...]
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Nicolas Miller (npmiller)

<details>
## Description

The fold operands pass tries to fold constants into their uses even folding register sequences.

In some cases it does it when it shouldn't, for example see the following MIR:
```
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, %15, implicit $exec
```

Is getting folded into this:
```
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, 42, implicit $exec
```

As you can see the 42 was moved over the register sequence and directly into the comparison instruction. This is incorrect because now the comparison will only check against the value 42, instead of a 64 bit splat of two 42s.

There are some constant values for which this is valid, for example `-1`, or `0` would be fine, since the splat is essentially just extending them from 32 bits to 64 bits, which the immediate operand can handle. But this is not correct in the general case.

Some lit tests are checking the splats of `-1` get folded by the pass (for example `test/CodeGen/AMDGPU/GlobalISel/andn2.ll`).

This folding is handled in [`SIFoldOperands.cpp:919`](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/AMDGPU/SIFoldOperands.cpp#L919).

If the register sequence has more than one register it does the following check:
```cpp
      if (!TII-&gt;isInlineConstant(*Op, OpTy) ||
          !TII-&gt;isOperandLegal(*UseMI, UseOpIdx, Op))
        return false;
```

This only ensures that the specific value can be folded as an immediate, it doesn't check if the composition of the values in the register sequence can be represented as an immediate.

Additionally the folding is only performed for splats:
```cpp
    if (Imm != SubImm)
      return false; // Can only fold splat constants
```

So clearly this code needs additional checks to avoid invalid folds, but I'm not sure exactly what they should be. It's possible some instructions are able to handle these folded splats, but it's definitely being done for some instructions that can't.

Note: `isInlineConstant` checks that the value is between `-16` and `64`, so a larger constant value in the splat doesn't trigger the issue.

Note: this was discovered downstream in [intel/llvm](https://github.com/intel/llvm), during SYCL testing.

## Reproducer

```
; ModuleID = 'repro.bc'
target datalayout = "e-p:64:64-p1:64:64-p2:32:32-p3:32:32-p4:64:64-p5:32:32-p6:32:32-p7:160:256:256:32-p8:128:128-p9:192:256:256:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-v2048:2048-n32:64-S32-A5-G1-ni:7:8:9"
target triple = "amdgcn-amd-amdhsa"

define i32 @<!-- -->repro_kernel(i64 %in) {
entry:
  %.not = icmp eq i64 %in, 180388626474
  br i1 %.not, label %2, label %1
1:
  ret i32 1

2:
  ret i32 2
}
```

Incorrect assembly snippet from that IR file:
```
$ ./bin/llc repro.ll -o repro.s
$ cat repro.s
[...]
repro_kernel:                           ; @<!-- -->repro_kernel
; %bb.0:                                ; %entry
        s_waitcnt vmcnt(0) expcnt(0) lgkmcnt(0) 
        v_cmp_eq_u64_e32 vcc, 42, v[0:1]
        v_mov_b32_e32 v0, 1
        s_and_saveexec_b64 s[4:5], vcc
        s_cbranch_execz .LBB0_2                                                                
; %bb.1:                           
        v_mov_b32_e32 v0, 2        
.LBB0_2:                                ; %UnifiedReturnBlock
        s_or_b64 exec, exec, s[4:5]
        s_setpc_b64 s[30:31]
[...]
```
MIR snippet before the fold operands pass:
```
$ ./bin/llc repro.ll -stop-before=si-fold-operands -o before.mir
$ cat before.mir
[...]
body:             |
  bb.0.entry:
    successors: %bb.1(0x40000000), %bb.2(0x40000000)
    liveins: $vgpr0, $vgpr1
  
    %11:vgpr_32 = COPY $vgpr1
    %10:vgpr_32 = COPY $vgpr0
    %21:vreg_64 = REG_SEQUENCE %10, %subreg.sub0, %11, %subreg.sub1
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, %15, implicit $exec
    %12:vgpr_32 = V_MOV_B32_e32 1, implicit $exec
    %0:sreg_64 = SI_IF killed %16, %bb.2, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    S_BRANCH %bb.1
  
  bb.1 (%ir-block.3):
    successors: %bb.2(0x80000000)
  
    %18:vgpr_32 = V_MOV_B32_e32 2, implicit $exec
  
  bb.2.UnifiedReturnBlock:
    %1:vgpr_32 = PHI %12, %bb.0, %18, %bb.1
    SI_END_CF %0, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    $vgpr0 = COPY %1
    SI_RETURN implicit $vgpr0
[...]
```
MIR snippet after running the fold operands pass:
```
$ ./bin/llc before.mir --run-pass=si-fold-operands -o bad.mir
$ cat bad.mir
[...]
body:             |
  bb.0.entry:
    successors: %bb.1(0x40000000), %bb.2(0x40000000)
    liveins: $vgpr0, $vgpr1
  
    %11:vgpr_32 = COPY $vgpr1
    %10:vgpr_32 = COPY $vgpr0
    %21:vreg_64 = REG_SEQUENCE %10, %subreg.sub0, %11, %subreg.sub1
    %14:sreg_32 = S_MOV_B32 42
    %15:sreg_64 = REG_SEQUENCE %14, %subreg.sub0, %14, %subreg.sub1
    %16:sreg_64 = V_CMP_EQ_U64_e64 killed %21, 42, implicit $exec
    %12:vgpr_32 = V_MOV_B32_e32 1, implicit $exec
    %0:sreg_64 = SI_IF killed %16, %bb.2, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    S_BRANCH %bb.1
  
  bb.1 (%ir-block.3):
    successors: %bb.2(0x80000000)
  
    %18:vgpr_32 = V_MOV_B32_e32 2, implicit $exec
  
  bb.2.UnifiedReturnBlock:
    %1:vgpr_32 = PHI %12, %bb.0, %18, %bb.1
    SI_END_CF %0, implicit-def dead $exec, implicit-def dead $scc, implicit $exec
    $vgpr0 = COPY %1
    SI_RETURN implicit $vgpr0
[...]
```
</details>


---

### Comment 2 - arsenm

I think I'm going to end up fixing this in the patch for #139317

---

