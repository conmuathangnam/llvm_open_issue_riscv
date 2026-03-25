# [RISCV] Wrong register allocation for vmerge.vvm

**Author:** wangpc-pp
**URL:** https://github.com/llvm/llvm-project/issues/169905
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, release:cherry-pick-failed
**Closed Date:** 2025-12-03T02:55:39Z

## Body

This was found when testing `llvm-test-suite/SingleSource/UnitTests/matrix-types-spec.cpp`.

See also: https://godbolt.org/z/753478EjG.

We generate an illegal `vmerge.vvm v24, v24, v0, v0`. This is illegal because we treat v0 as two kinds of vectors: 1. M8/SEW64 vector. 2. Mask vector(SEW=1, M1). See also https://github.com/llvm/llvm-project/issues/80099.
```asm
transposeSpec(long*, long*):
        addi    a2, a1, 32
        vl2re64.v       v16, (a1)
        addi    a1, a1, 64
        vl2re64.v       v10, (a2)
        vl2re64.v       v8, (a1)
        vsetivli        zero, 2, e64, m1, ta, ma
        vmv1r.v v27, v17
        vslideup.vi     v27, v11, 1
        vmv1r.v v1, v8
        vmv1r.v v24, v16
        vmv1r.v v2, v8
        vslideup.vi     v24, v10, 1
        lui     a1, 1
        vmv1r.v v4, v9
        vmv1r.v v25, v16
        vrgather.vi     v26, v10, 1
        addi    a1, a1, -1756
        vmv1r.v v5, v9
        vmv1r.v v28, v17
        vmv.s.x v0, a1
        vrgather.vi     v29, v11, 1
        vsetivli        zero, 16, e64, m8, ta, ma
        vmerge.vvm      v24, v24, v0, v0
        vsetivli        zero, 2, e64, m1, ta, mu
        vmv.v.i v0, 1
        vslidedown.vi   v10, v16, 1, v0.t
        vslideup.vi     v17, v11, 1
        vrgather.vi     v11, v8, 1
        vmv.v.v v8, v17
        addi    a1, a0, 48
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v8, (a1)
        vsetivli        zero, 2, e64, m1, ta, ma
        vslidedown.vi   v8, v28, 1
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v24, (a0)
        vsetivli        zero, 2, e64, m1, ta, ma
        vslideup.vi     v8, v29, 1
        vrgather.vi     v9, v29, 1
        addi    a1, a0, 24
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v10, (a1)
        addi    a0, a0, 72
        vse64.v v8, (a0)
        ret
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Pengcheng Wang (wangpc-pp)

<details>
This was found when testing `llvm-test-suite/SingleSource/UnitTests/matrix-types-spec.cpp`.

See also: https://godbolt.org/z/753478EjG.

We generate an illegal `vmerge.vvm v24, v24, v0, v0`. This is illegal because we treat v0 as two kinds of vectors: 1. M8/SEW64 vector. 2. Mask vector(SEW=1, M1). See also https://github.com/llvm/llvm-project/issues/80099.
```asm
transposeSpec(long*, long*):
        addi    a2, a1, 32
        vl2re64.v       v16, (a1)
        addi    a1, a1, 64
        vl2re64.v       v10, (a2)
        vl2re64.v       v8, (a1)
        vsetivli        zero, 2, e64, m1, ta, ma
        vmv1r.v v27, v17
        vslideup.vi     v27, v11, 1
        vmv1r.v v1, v8
        vmv1r.v v24, v16
        vmv1r.v v2, v8
        vslideup.vi     v24, v10, 1
        lui     a1, 1
        vmv1r.v v4, v9
        vmv1r.v v25, v16
        vrgather.vi     v26, v10, 1
        addi    a1, a1, -1756
        vmv1r.v v5, v9
        vmv1r.v v28, v17
        vmv.s.x v0, a1
        vrgather.vi     v29, v11, 1
        vsetivli        zero, 16, e64, m8, ta, ma
        vmerge.vvm      v24, v24, v0, v0
        vsetivli        zero, 2, e64, m1, ta, mu
        vmv.v.i v0, 1
        vslidedown.vi   v10, v16, 1, v0.t
        vslideup.vi     v17, v11, 1
        vrgather.vi     v11, v8, 1
        vmv.v.v v8, v17
        addi    a1, a0, 48
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v8, (a1)
        vsetivli        zero, 2, e64, m1, ta, ma
        vslidedown.vi   v8, v28, 1
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v24, (a0)
        vsetivli        zero, 2, e64, m1, ta, ma
        vslideup.vi     v8, v29, 1
        vrgather.vi     v9, v29, 1
        addi    a1, a0, 24
        vsetivli        zero, 3, e64, m2, ta, ma
        vse64.v v10, (a1)
        addi    a0, a0, 72
        vse64.v v8, (a0)
        ret
```
</details>


---

### Comment 2 - topperc

Looks like the lmul=8 source that became v0m8 had undef lanes that allowed the register allocator to overlap it with the v0 for the mask.

---

### Comment 3 - wangpc-pp

This miscompilation occurred since llvm 20 and I suspect that this is related to the change to replace `V0` with `VMV0`. cc @lukel97 

---

### Comment 4 - lukel97

> This miscompilation occurred since llvm 20 and I suspect that this is related to the change to replace `V0` with `VMV0`. cc [@lukel97](https://github.com/lukel97)

Yes, I think we need to constrain the other register operands to vrm8nov0 in RISCVVMV0Elimination. Currently this:

```
  %47:vmv0 = COPY %45:vr; example.cpp:10:17
  %48:vrm8nov0 = PseudoVMERGE_VVM_M8 $noreg(tied-def 0), killed %35:vrm8, killed %42:vrm8, %47:vmv0, -1, 6; example.cpp:10:17
```

gets transformed to this:

```
  $v0 = COPY %45:vr; example.cpp:10:17
  %48:vrm8nov0 = PseudoVMERGE_VVM_M8 $noreg(tied-def 0), killed %35:vrm8, killed %42:vrm8, $v0, -1, 6; example.cpp:10:17
```

I wonder if we can share the logic to compute the EEW in RISCVVLOptimizer to only constrain the operands when their EEW != 1. 

---

### Comment 5 - wangpc-pp

/cherry-pick 76cb984

---

### Comment 6 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: 76cb984

https://github.com/llvm/llvm-project/actions/runs/19880867290

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

