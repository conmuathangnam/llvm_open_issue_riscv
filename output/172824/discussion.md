# Wrong code at -O1/O2 possibly due to backend bug since clang 21.1.0

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/172824
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2026-01-08T13:47:51Z

## Body

Reproducer: https://godbolt.org/z/bGEMsr81M

I inspected the IR produced by `opt`, and it looks correct to me. And I can reproduce it on X86, AArch64, and RISC-V. I suspect it's the problem of the generic codegen.

Testcase:
```c
#include <stdint.h>
#include <stdio.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
uint64_t BS_CHECKSUM;
BS_VEC(int8_t, 32)
backsmith_snippet_1116(BS_VEC(uint8_t, 32), uint16_t, BS_VEC(int64_t, 8))
{}
static int32_t g_3, g_169;
int func_1_BS_COND_4;
int func_1_BS_COND_9;
static uint32_t func_1()
{
    uint64_t LOCAL_CHECKSUM = 0;
BS_LABEL_6:
    for (; g_3;)
    {
        int32_t l_1874[1][1][3];
        int i, j, k;
        l_1874[i][j][k] = 1;
        for (; 0; g_169)
            if (0)
            {
                for (uint32_t BS_TEMP_34; BS_TEMP_34; BS_TEMP_34++)
                {
                    LOCAL_CHECKSUM ^= 9;
                BS_LABEL_4:
                }
                __builtin_convertvector(
                    __builtin_shufflevector(
                        backsmith_snippet_1116((BS_VEC(uint8_t, 32)){}, 0,
                                               (BS_VEC(int64_t, 8)){}),
                        backsmith_snippet_1116((BS_VEC(uint8_t, 32)){}, 0,
                                               (BS_VEC(int64_t, 8)){}),
                        0, 7, 3, 3, 3, 1, 3, 6),
                    BS_VEC(uint32_t, 8));
                __asm goto("" : : : : BS_LABEL_6);
            }
            else
            BS_LABEL_7:
                if (l_1874[0][0][1]) break;
    }
    if (func_1_BS_COND_4) goto BS_LABEL_7;
    switch (func_1_BS_COND_9)
    case 2064568:
        goto BS_LABEL_4;
    BS_CHECKSUM += LOCAL_CHECKSUM;
    return 0;
}
int main()
{
    func_1();
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
```

Commands:
```bash
> clang -O0 a.c && a.out
BackSmith Checksum = 0x0000000000000000
> clang -O1 a.c && a.out
BackSmith Checksum = 0x00007ffe970f5af8
```

## Comments

### Comment 1 - XChy

It looks like the emitted X86 asm is wrong, as `rsi` is not initialized to zero:
```asm
        add     rsi, qword ptr [rip + BS_CHECKSUM]
        mov     qword ptr [rip + BS_CHECKSUM], rsi
```

cc @RKSimon 

---

### Comment 2 - RKSimon

@XChy have you run this through UBSan?

---

### Comment 3 - antoniofrighetto

I don't think there's any undefined behaviour in the code. I don't think this is as an issue in codegen either. Looking at the IR, from a quick look, I think InstructionCombine may be the culprit. I'm taking a look.

---

### Comment 4 - XChy

> [@XChy](https://github.com/XChy) have you run this through UBSan?

Yes. It passes the UBsan, ASan, and Memory Sanitizer.

---

### Comment 5 - antoniofrighetto

> > [@XChy](https://github.com/XChy) have you run this through UBSan?
> 
> Yes. It passes the UBsan, ASan, and Memory Sanitizer.

IIRC MSan and UBSan used not to catch some uninitialized reads. It looks like there are some of them when compiling with `-Werror=uninitialized` (https://llvm.godbolt.org/z/cPeGn8dWe), would you mind taking a look at them?

---

### Comment 6 - XChy

@antoniofrighetto, I have checked the C code. The code in `for (; g_3;) {xxx}` is dead, as global variables like `g_3` are always initialized to zero if not specified. Thus, the uninitialized reads in the loop are never run.

Without the dead loop and some dead jumps, the runtime behaviour should be like:
```c
#include <stdint.h>
#include <stdio.h>
uint64_t BS_CHECKSUM;
static uint32_t func_1()
{
    uint64_t LOCAL_CHECKSUM = 0;
    // dead code
    BS_CHECKSUM += LOCAL_CHECKSUM;
    return 0;
}
int main()
{
    func_1();
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
```

---

### Comment 7 - XChy

I ran the IR at -O2 with [llubi](https://github.com/dtcxzyw/llvm-ub-aware-interpreter), and the result is correct. It implies that the problem is in the backend.

```bash
> llubi --verbose a.ll
Entering function main
  br label %for.cond.i jump to %for.cond.i
    phi i64 %LOCAL_CHECKSUM.1.i -> i64 0
  %0 = load i32, ptr @func_1_BS_COND_4, align 4, !tbaa !5 -> i32 0
  %tobool26.not.i = icmp eq i32 %0, 0 -> T
  br i1 %tobool26.not.i, label %if.end28.i, label %for.cond.i.backedge jump to %if.end28.i
  %1 = load i32, ptr @func_1_BS_COND_9, align 4, !tbaa !5 -> i32 0
  %cond.i = icmp eq i32 %1, 2064568 -> F
  br i1 %cond.i, label %BS_LABEL_4.i, label %func_1.exit jump to %func_1.exit
  %2 = load i64, ptr @BS_CHECKSUM, align 8, !tbaa !12 -> i64 0
  %add.i = add i64 %2, %LOCAL_CHECKSUM.1.i -> i64 0
  store i64 %add.i, ptr @BS_CHECKSUM, align 8, !tbaa !12
  %call1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %add.i)
    Printf: BackSmith Checksum = 0x0000000000000000
   -> i32 40
  ret i32 0
```

---

### Comment 8 - antoniofrighetto

Machine IR after Virtual Register Rewriter on clang-20:
```
0B	bb.0.entry:
	  successors: %bb.1(0x80000000); %bb.1(100.00%)

16B	  renamable $esi = MOV32r0 implicit-def dead $eflags, implicit-def $rsi

64B	bb.1.for.cond.i (machine-block-address-taken, inlineasm-br-indirect-target):
	; predecessors: %bb.0, %bb.1, %bb.3
	  successors: %bb.4(0x30000000), %bb.1(0x50000000); %bb.4(37.50%), %bb.1(62.50%)
	  liveins: $rsi
112B	  CMP32mi $rip, 1, $noreg, @func_1_BS_COND_4, $noreg, 0, implicit-def $eflags :: (dereferenceable load (s32) from @func_1_BS_COND_4)
128B	  JCC_1 %bb.4, 4, implicit killed $eflags
144B	  JMP_1 %bb.1
```
Machine IR after Virtual Register Rewriter on clang trunk:
```
0B	bb.0.entry:
	  successors: %bb.1(0x80000000); %bb.1(100.00%)

64B	bb.1.for.cond.i (inlineasm-br-indirect-target):
	; predecessors: %bb.0, %bb.1, %bb.3
	  successors: %bb.4(0x30000000), %bb.1(0x50000000); %bb.4(37.50%), %bb.1(62.50%)

80B	  renamable $rsi = IMPLICIT_DEF
112B	  CMP32mi $rip, 1, $noreg, @func_1_BS_COND_4, $noreg, 0, implicit-def $eflags :: (dereferenceable load (s32) from @func_1_BS_COND_4)
128B	  JCC_1 %bb.4, 4, implicit killed $eflags
144B	  JMP_1 %bb.1
```
It looks like 1) `$rsi` is indeed left undefined on latest Clang 2) Somehow `machine-block-address-taken` attribute has been lost. This has started appearing since https://github.com/llvm/llvm-project/pull/143958. @nikic Do you believe the following code is correct even in control-flows with asm gotos? I see a PN (which initializes local checksum on entry path) getting broken (though not sure if this is just exposing the issue).

https://github.com/llvm/llvm-project/blob/cbb2aa9b2dac6d55a24781aa25247a9e82849d5e/llvm/lib/Transforms/InstCombine/InstructionCombining.cpp#L3694-L3702



---

### Comment 9 - nikic

@antoniofrighetto I don't think that code has any interaction with asm goto.

---

### Comment 10 - antoniofrighetto

Machine IR still in SSA form:
```
bb.0.entry:
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  %6:gr32 = MOV32r0 implicit-def dead $eflags
  %5:gr64 = SUBREG_TO_REG 0, killed %6:gr32, %subreg.sub_32bit

bb.1.for.cond.i (inlineasm-br-indirect-target):
; predecessors: %bb.0, %bb.1, %bb.3
  successors: %bb.4(0x30000000), %bb.1(0x50000000); %bb.4(37.50%), %bb.1(62.50%)

  %0:gr64 = PHI %5:gr64, %bb.0, undef %7:gr64, %bb.1, %2:gr64, %bb.3
  CMP32mi $rip, 1, $noreg, @func_1_BS_COND_4, $noreg, 0, implicit-def $eflags :: (dereferenceable load (s32) from @func_1_BS_COND_4, !tbaa !5)
  JCC_1 %bb.4, 4, implicit killed $eflags
  JMP_1 %bb.1
```
Machine IR exiting SSA form, after PHI-Elimination:
```
bb.0.entry:
  successors: %bb.1(0x80000000); %bb.1(100.00%)

  %6:gr32 = MOV32r0 implicit-def dead $eflags
  %5:gr64 = SUBREG_TO_REG 0, killed %6:gr32, %subreg.sub_32bit
  %16:gr64 = COPY killed %5:gr64

bb.1.for.cond.i (inlineasm-br-indirect-target):
; predecessors: %bb.0, %bb.1, %bb.3
  successors: %bb.4(0x30000000), %bb.1(0x50000000); %bb.4(37.50%), %bb.1(62.50%)

  %16:gr64 = IMPLICIT_DEF
  %0:gr64 = COPY killed %16:gr64
  CMP32mi $rip, 1, $noreg, @func_1_BS_COND_4, $noreg, 0, implicit-def $eflags :: (dereferenceable load (s32) from @func_1_BS_COND_4, !tbaa !5)
  JCC_1 %bb.4, 4, implicit killed $eflags
  JMP_1 %bb.1
```
The PN has been removed, and a copy (of `%5`) has been added to bb.1 predecessor, i.e., bb.0; so has been done for bb.1 itself, for the self-loop. However, I'm not clear on how the copy is emitted for bb.1, it looks like `%16` is (always) redefined to undef. Conversely, I would have expected something similar to:
```
%0:gr64 = COPY killed %16:gr64
%16:gr64 = IMPLICIT_DEF
```
At this point, RegisterCoalescer (correctly, I think) determines that the definition in bb.0 are all dead. I'd be tempted to say the miscompilation occurs during phi-nodes elimination, though it would be nice if someone could confirm this.


---

