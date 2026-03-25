# rem/div-related miscompile from riscv64 and aarch64 backends

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/145360
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-06-23T21:29:18Z

## Body

I believe we're lowering this incorrectly:
```llvm
define i32 @f(i32 %0, ptr %1) {
  %3 = srem i32 %0, 256
  store i32 %3, ptr %1, align 4
  %4 = sdiv exact i32 %0, 256
  ret i32 %4
}
```
let's invoke `f` with `%0=2` and `%1` being a valid pointer to 4 bytes. in this case we should store 2 into memory and then return poison.

however, from the riscv64+m backend we get:
```
f:
        sraiw   a0, a0, 8
        sw      zero, 0(a1)
        ret
```
the aarch64 output is functionally identical

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
I believe we're lowering this incorrectly:
```llvm
define i32 @<!-- -->f(i32 %0, ptr %1) {
  %3 = srem i32 %0, 256
  store i32 %3, ptr %1, align 4
  %4 = sdiv exact i32 %0, 256
  ret i32 %4
}
```
let's invoke `f` with `%0=2` and `%1` being a valid pointer to 4 bytes. in this case we should store 2 into memory and then return poison.

however, from the riscv64+m backend we get:
```
f:
        sraiw   a0, a0, 8
        sw      zero, 0(a1)
        ret
```
the aarch64 output is functionally identical
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: John Regehr (regehr)

<details>
I believe we're lowering this incorrectly:
```llvm
define i32 @<!-- -->f(i32 %0, ptr %1) {
  %3 = srem i32 %0, 256
  store i32 %3, ptr %1, align 4
  %4 = sdiv exact i32 %0, 256
  ret i32 %4
}
```
let's invoke `f` with `%0=2` and `%1` being a valid pointer to 4 bytes. in this case we should store 2 into memory and then return poison.

however, from the riscv64+m backend we get:
```
f:
        sraiw   a0, a0, 8
        sw      zero, 0(a1)
        ret
```
the aarch64 output is functionally identical
</details>


---

### Comment 3 - efriedma-quic

Yes, DAGCombiner::visitSDIV accidentally accidentally propagates the "exact" flag from the sdiv to the srem after it calls visitSDIVLike.

---

