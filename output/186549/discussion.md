# [PowerPC] vectorized i128 eq changes load order across calls

**Author:** cuviper
**URL:** https://github.com/llvm/llvm-project/issues/186549
**Status:** Closed
**Labels:** backend:PowerPC, miscompilation
**Closed Date:** 2026-03-18T10:38:53Z

## Body

<details> 
  <summary>Input IR</summary>

```
; ModuleID = 'extracted.bc'
source_filename = "alike_mismatch.6040ac6454b9c6d4-cgu.0"
target datalayout = "e-m:e-Fn32-i64:64-i128:128-n32:64-S128-v256:256:256-v512:512:512"
target triple = "powerpc64le-unknown-linux-gnu"

@0 = external hidden unnamed_addr constant [40 x i8], align 1
@1 = external hidden unnamed_addr constant <{ ptr, [16 x i8] }>, align 8

; Function Attrs: uwtable
define hidden void @_ZN14alike_mismatch4main17h81f1d2566e2b675aE() unnamed_addr #0 {
  %1 = alloca [16 x i8], align 16
  %2 = alloca [16 x i8], align 16
  call void @llvm.lifetime.start.p0(i64 16, ptr nonnull %2)
  call void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr noalias noundef nonnull sret([16 x i8]) align 1 captures(none) dereferenceable(16) %2)
  %.sroa.0.0.copyload = load i128, ptr %2, align 16
  call void @llvm.lifetime.end.p0(i64 16, ptr nonnull %2)
  call void @llvm.lifetime.start.p0(i64 16, ptr nonnull %1)
  call void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr noalias noundef nonnull sret([16 x i8]) align 1 captures(none) dereferenceable(16) %1)
  %.sroa.01.0.copyload = load i128, ptr %1, align 16
  call void @llvm.lifetime.end.p0(i64 16, ptr nonnull %1)
  %3 = icmp eq i128 %.sroa.01.0.copyload, %.sroa.0.0.copyload
  br i1 %3, label %5, label %4, !prof !3

4:                                                ; preds = %0
  ret void

5:                                                ; preds = %0
  tail call void @_ZN4core9panicking5panic17h835aabb441d22b32E(ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) @0, i64 noundef 40, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24) @1) #3
  unreachable
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: uwtable
declare void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr dead_on_unwind noalias noundef writable sret([16 x i8]) align 1 captures(none) dereferenceable(16)) unnamed_addr #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: cold noinline noreturn uwtable
declare void @_ZN4core9panicking5panic17h835aabb441d22b32E(ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance), i64 noundef, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24)) unnamed_addr #2

attributes #0 = { uwtable "probe-stack"="inline-asm" "target-cpu"="ppc64le" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { cold noinline noreturn uwtable "probe-stack"="inline-asm" "target-cpu"="ppc64le" }
attributes #3 = { noinline noreturn }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{!"rustc version 1.94.0 (4a4ef493e 2026-03-02)"}
!3 = !{!"branch_weights", !"expected", i32 1, i32 2000}
```
</details>

This function makes two UUID calls and a comparison asserting inequality. In LLVM 21, llc generated this snippet:

```asm
        addi 3, 1, 32
        std 0, 96(1)
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        ld 30, 32(1)
        ld 29, 40(1)
        addi 3, 1, 32
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        ld 3, 32(1)
        ld 4, 40(1)
        xor 4, 4, 29
        xor 3, 3, 30
        or. 3, 3, 4
        beq     0, .LBB0_2
```

Note that the return value was written to the same stack location for each call, but that's fine since the first was loaded into registers before the second call.

LLVM 22 generates this:

```asm
        mr      3, 30
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        mr      3, 30
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        lxvd2x 34, 0, 30
        lxvd2x 35, 0, 30
        vcmpequb. 2, 3, 2
        blt     6, .LBB0_2
```

The value from the first call is lost, and both vector loads get the same second result.

**AI**: Claude Code identified commit 55aff64d2c6ef50d2ed725d7dd1fb34080486237 (#158657) as the relevant change, and I confirmed that immediately before and after that commit shows exactly the difference above. Claude also suggested that we could disable the optimization if the two load addresses are equal, but the patch it gave me didn't work and I didn't pursue it further yet. I think that would be generally insufficient anyway, as we would need to be sure that the call wouldn't clobber *any* part of the delayed load.

cc @diggerlin and @RolandF77 regarding that pull request.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Josh Stone (cuviper)

<details>
&lt;details&gt; 
  &lt;summary&gt;Input IR&lt;/summary&gt;

```
; ModuleID = 'extracted.bc'
source_filename = "alike_mismatch.6040ac6454b9c6d4-cgu.0"
target datalayout = "e-m:e-Fn32-i64:64-i128:128-n32:64-S128-v256:256:256-v512:512:512"
target triple = "powerpc64le-unknown-linux-gnu"

@<!-- -->0 = external hidden unnamed_addr constant [40 x i8], align 1
@<!-- -->1 = external hidden unnamed_addr constant &lt;{ ptr, [16 x i8] }&gt;, align 8

; Function Attrs: uwtable
define hidden void @<!-- -->_ZN14alike_mismatch4main17h81f1d2566e2b675aE() unnamed_addr #<!-- -->0 {
  %1 = alloca [16 x i8], align 16
  %2 = alloca [16 x i8], align 16
  call void @<!-- -->llvm.lifetime.start.p0(i64 16, ptr nonnull %2)
  call void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr noalias noundef nonnull sret([16 x i8]) align 1 captures(none) dereferenceable(16) %2)
  %.sroa.0.0.copyload = load i128, ptr %2, align 16
  call void @<!-- -->llvm.lifetime.end.p0(i64 16, ptr nonnull %2)
  call void @<!-- -->llvm.lifetime.start.p0(i64 16, ptr nonnull %1)
  call void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr noalias noundef nonnull sret([16 x i8]) align 1 captures(none) dereferenceable(16) %1)
  %.sroa.01.0.copyload = load i128, ptr %1, align 16
  call void @<!-- -->llvm.lifetime.end.p0(i64 16, ptr nonnull %1)
  %3 = icmp eq i128 %.sroa.01.0.copyload, %.sroa.0.0.copyload
  br i1 %3, label %5, label %4, !prof !3

4:                                                ; preds = %0
  ret void

5:                                                ; preds = %0
  tail call void @<!-- -->_ZN4core9panicking5panic17h835aabb441d22b32E(ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance) @<!-- -->0, i64 noundef 40, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24) @<!-- -->1) #<!-- -->3
  unreachable
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #<!-- -->1

; Function Attrs: uwtable
declare void @"_ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E"(ptr dead_on_unwind noalias noundef writable sret([16 x i8]) align 1 captures(none) dereferenceable(16)) unnamed_addr #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #<!-- -->1

; Function Attrs: cold noinline noreturn uwtable
declare void @<!-- -->_ZN4core9panicking5panic17h835aabb441d22b32E(ptr noalias noundef nonnull readonly align 1 captures(address, read_provenance), i64 noundef, ptr noalias noundef readonly align 8 captures(address, read_provenance) dereferenceable(24)) unnamed_addr #<!-- -->2

attributes #<!-- -->0 = { uwtable "probe-stack"="inline-asm" "target-cpu"="ppc64le" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #<!-- -->2 = { cold noinline noreturn uwtable "probe-stack"="inline-asm" "target-cpu"="ppc64le" }
attributes #<!-- -->3 = { noinline noreturn }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{!"rustc version 1.94.0 (4a4ef493e 2026-03-02)"}
!3 = !{!"branch_weights", !"expected", i32 1, i32 2000}
```
&lt;/details&gt;

This function makes two UUID calls and a comparison asserting inequality. In LLVM 21, llc generated this snippet:

```asm
        addi 3, 1, 32
        std 0, 96(1)
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        ld 30, 32(1)
        ld 29, 40(1)
        addi 3, 1, 32
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        ld 3, 32(1)
        ld 4, 40(1)
        xor 4, 4, 29
        xor 3, 3, 30
        or. 3, 3, 4
        beq     0, .LBB0_2
```

Note that the return value was written to the same stack location for each call, but that's fine since the first was loaded into registers before the second call.

LLVM 22 generates this:

```asm
        mr      3, 30
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        mr      3, 30
        bl _ZN4uuid2v428_$LT$impl$u20$uuid..Uuid$GT$6new_v417hac2e76167e28a9b6E
        nop
        lxvd2x 34, 0, 30
        lxvd2x 35, 0, 30
        vcmpequb. 2, 3, 2
        blt     6, .LBB0_2
```

The value from the first call is lost, and both vector loads get the same second result.

**AI**: Claude Code identified commit 55aff64d2c6ef50d2ed725d7dd1fb34080486237 (#<!-- -->158657) as the relevant change, and I confirmed that immediately before and after that commit shows exactly the difference above. Claude also suggested that we could disable the optimization if the two load addresses are equal, but the patch it gave me didn't work and I didn't pursue it further yet. I think that would be generally insufficient anyway, as we would need to be sure that the call wouldn't clobber *any* part of the delayed load.

cc @<!-- -->diggerlin and @<!-- -->RolandF77 regarding that pull request.
</details>


---

### Comment 2 - nikic

This is due to incorrect chain management. The input chains of the loads are preserved, but the output chains are ignored.

---

### Comment 3 - nikic

/cherry-pick 7404a5dbe0ca971e0f312a366019361fc9d576e0

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#187234

---

