# [LoongArch] Incorrect register allocation for `[G]CSRXCHG` - rj must not be R0 ro R1

**Author:** heiher
**URL:** https://github.com/llvm/llvm-project/issues/140842
**Status:** Closed
**Labels:** miscompilation, backend:loongarch
**Closed Date:** 2025-05-22T10:49:29Z

## Body

In the LoongArch target, the `[G]CSRXCHG` instruction has a special encoding constraint:

If the `rj` operand is assigned to register R0 or R1, the instruction encoding will be interpreted as `[G]CSRRD` or `[G]CSRWR` respectively, not `[G]CSRXCHG`.

Currently, LLVM’s register allocator may assign R0 or R1 to the `rj` operand of a `[G]CSRXCHG` instruction, which leads to incorrect code generation and unintended semantics.

Reproduce case:

```ll
declare i64 @llvm.loongarch.csrxchg.d(i64, i64, i32 immarg);

define dso_local i64 @csrxchg() {
entry:
  %0 = tail call i64 asm sideeffect "", "=r,r,i,{$r4},{$r5},{$r6},{$r7},{$r8},{$r9},{$r10},{$r11},{$r12},{$r13},{$r14},{$r15},{$r16},{$r17},{$r18},{$r19},{$r20},{$r23},{$r24},{$r25},{$r26},{$r27},{$r28},{$r29},{$r30},{$r31},0,~{memory}"(i32 4, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i64 0)
  %1 = tail call i64 @llvm.loongarch.csrxchg.d(i64 %0, i64 4, i32 0)
  %2 = tail call i64 asm sideeffect "", "=r,r,i,{$r4},{$r5},{$r6},{$r7},{$r8},{$r9},{$r10},{$r11},{$r12},{$r13},{$r14},{$r15},{$r16},{$r17},{$r18},{$r19},{$r20},{$r23},{$r24},{$r25},{$r26},{$r27},{$r28},{$r29},{$r30},{$r31},0,~{memory}"(i32 4, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i64 %1)
  ret i64 %2
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: hev (heiher)

<details>
In the LoongArch target, the `{G}CSRXCHG` instruction has a special encoding constraint:

If the `rj` operand is assigned to register R0 or R1, the instruction encoding will be interpreted as `{G}CSRRD` or `{G}CSRWR` respectively, not `{G}CSRXCHG`.

Currently, LLVM’s register allocator may assign R0 or R1 to the `rj` operand of a `{G}CSRXCHG` instruction, which leads to incorrect code generation and unintended semantics.

Reproduce case:

```ll
declare i64 @<!-- -->llvm.loongarch.csrxchg.d(i64, i64, i32 immarg);

define dso_local i64 @<!-- -->csrxchg() {
entry:
  %0 = tail call i64 asm sideeffect "", "=r,r,i,{$r4},{$r5},{$r6},{$r7},{$r8},{$r9},{$r10},{$r11},{$r12},{$r13},{$r14},{$r15},{$r16},{$r17},{$r18},{$r19},{$r20},{$r23},{$r24},{$r25},{$r26},{$r27},{$r28},{$r29},{$r30},{$r31},0,~{memory}"(i32 4, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i64 0)
  %1 = tail call i64 @<!-- -->llvm.loongarch.csrxchg.d(i64 %0, i64 4, i32 0)
  %2 = tail call i64 asm sideeffect "", "=r,r,i,{$r4},{$r5},{$r6},{$r7},{$r8},{$r9},{$r10},{$r11},{$r12},{$r13},{$r14},{$r15},{$r16},{$r17},{$r18},{$r19},{$r20},{$r23},{$r24},{$r25},{$r26},{$r27},{$r28},{$r29},{$r30},{$r31},0,~{memory}"(i32 4, i32 0, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i64 %1)
  ret i64 %2
}
```
</details>


---

