# [RISCV] Permutation intrinsics in Zvfhmin unnecessarily require Zvfh

**Author:** dzaima
**URL:** https://github.com/llvm/llvm-project/issues/143975
**Status:** Closed
**Labels:** backend:RISC-V
**Closed Date:** 2025-06-16T04:15:31Z

## Body

Basic 16-bit float permutation intrinsics (`vslideup` / `vslidedown` / `vrgather`), which can be in `Zvfhmin` (as they need just the `vfloat16*` type and a basic `e16` `Zve32x` instruction), unnecessarily also require `Zvfh`.

This happens both with `__attribute__((target))`: https://riscvc.godbolt.org/z/x6nnb6YoG
and with `-march`: https://riscvc.godbolt.org/z/b4dMrda3e

Copied-out code sample that should work but doesn't, for reference:
```c
#include<riscv_vector.h>

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vrgather(vfloat16m1_t op1, vuint16m1_t index, size_t vl) {
    return __riscv_vrgatherei16_vv_f16m1(op1, index, vl);
}

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vslideup(vfloat16m1_t dest, vfloat16m1_t src, size_t offset, size_t vl) {
    return __riscv_vslideup_vx_f16m1(dest, src, offset, vl);
}

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vslidedown(vfloat16m1_t src, size_t offset, size_t vl) {
    return __riscv_vslidedown_vx_f16m1(src, offset, vl);
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: dzaima (dzaima)

<details>
Basic 16-bit float permutation intrinsics (`vslideup` / `vslidedown` / `vrgather`), which can be in `Zvfhmin` (as they need just the `vfloat16*` type and a basic `e16` `Zve32x` instruction), unnecessarily also require `Zvfh`.

This happens both with `__attribute__((target))`: https://riscvc.godbolt.org/z/x6nnb6YoG
and with `-march`: https://riscvc.godbolt.org/z/b4dMrda3e

Copied-out code sample that should work but doesn't, for reference:
```c
#include&lt;riscv_vector.h&gt;

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vrgather(vfloat16m1_t op1, vuint16m1_t index, size_t vl) {
    return __riscv_vrgatherei16_vv_f16m1(op1, index, vl);
}

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vslideup(vfloat16m1_t dest, vfloat16m1_t src, size_t offset, size_t vl) {
    return __riscv_vslideup_vx_f16m1(dest, src, offset, vl);
}

__attribute__((target("arch=+v,+zvfhmin")))
vfloat16m1_t vslidedown(vfloat16m1_t src, size_t offset, size_t vl) {
    return __riscv_vslidedown_vx_f16m1(src, offset, vl);
}
```
</details>


---

