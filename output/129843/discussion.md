# LLVM 20 miscompiles `@llvm.ctpop.i128` for `aarch64_be`

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/129843
**Status:** Closed
**Labels:** backend:AArch64, regression, miscompilation, release:backport
**Closed Date:** 2025-03-06T06:47:15Z

## Body

Consider this Zig program:

```zig
pub fn main() void {
    var x: u128 = 0b11111111000110001100010000100001000011000011100101010001;
    _ = &x;
    @import("std").process.exit(@popCount(x));
}
```

Running it with `qemu-aarch64_be` will produce `24` with LLVM 19, but `0` with LLVM 20.

Isolating the `@llvm.ctpop.i128` a bit:

```llvm
; ModuleID = 'BitcodeBuffer'
source_filename = "repro"
target datalayout = "E-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64_be-unknown-linux4.19.0-unknown"

@builtin.zig_backend = internal unnamed_addr constant i64 2, align 8
@start.simplified_logic = internal unnamed_addr constant i1 false, align 1
@builtin.output_mode = internal unnamed_addr constant i2 -2, align 1

; Function Attrs: nosanitize_coverage nounwind skipprofile
define dso_local i32 @repro() #0 {
  %1 = alloca [16 x i8], align 16
  store i128 71803349708323153, ptr %1, align 16
  %2 = load i128, ptr %1, align 16
  %3 = call i128 @llvm.ctpop.i128(i128 %2)
  %4 = trunc i128 %3 to i8
  %5 = zext i8 %4 to i32
  ret i32 %5
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i128 @llvm.ctpop.i128(i128) #1

attributes #0 = { nosanitize_coverage nounwind skipprofile "frame-pointer"="all" "target-cpu"="generic" "target-features"="+enable-select-opt,+ete,+fp-armv8,+fuse-adrp-add,+fuse-aes,+neon,+trbe,+use-postra-scheduler,-addr-lsl-slow-14,-aes,-aggressive-fma,-alternate-sextload-cvt-f32-pattern,-altnzcv,-alu-lsl-fast,-am,-amvs,-arith-bcc-fusion,-arith-cbz-fusion,-ascend-store-address,-avoid-ldapur,-balance-fp-ops,-bf16,-brbe,-bti,-call-saved-x10,-call-saved-x11,-call-saved-x12,-call-saved-x13,-call-saved-x14,-call-saved-x15,-call-saved-x18,-call-saved-x8,-call-saved-x9,-ccdp,-ccidx,-ccpp,-chk,-clrbhb,-cmp-bcc-fusion,-cmpbr,-complxnum,-CONTEXTIDREL2,-cpa,-crc,-crypto,-cssc,-d128,-disable-latency-sched-heuristic,-disable-ldp,-disable-stp,-dit,-dotprod,-ecv,-el2vmsa,-el3,-exynos-cheap-as-move,-f32mm,-f64mm,-f8f16mm,-f8f32mm,-faminmax,-fgt,-fix-cortex-a53-835769,-flagm,-fmv,-force-32bit-jump-tables,-fp16fml,-fp8,-fp8dot2,-fp8dot4,-fp8fma,-fpac,-fprcvt,-fptoint,-fujitsu-monaka,-fullfp16,-fuse-address,-fuse-addsub-2reg-const1,-fuse-arith-logic,-fuse-crypto-eor,-fuse-csel,-fuse-literals,-gcs,-harden-sls-blr,-harden-sls-nocomdat,-harden-sls-retbr,-hbc,-hcx,-i8mm,-ite,-jsconv,-ldp-aligned-only,-lor,-ls64,-lse,-lse128,-lse2,-lsfe,-lsui,-lut,-mec,-mops,-mpam,-mte,-nmi,-no-bti-at-return-twice,-no-neg-immediates,-no-sve-fp-ld1r,-no-zcz-fp,-nv,-occmo,-outline-atomics,-pan,-pan-rwv,-pauth,-pauth-lr,-pcdphint,-perfmon,-pops,-predictable-select-expensive,-predres,-prfm-slc-target,-rand,-ras,-rasv2,-rcpc,-rcpc3,-rcpc-immo,-rdm,-reserve-lr-for-ra,-reserve-x1,-reserve-x10,-reserve-x11,-reserve-x12,-reserve-x13,-reserve-x14,-reserve-x15,-reserve-x18,-reserve-x2,-reserve-x20,-reserve-x21,-reserve-x22,-reserve-x23,-reserve-x24,-reserve-x25,-reserve-x26,-reserve-x27,-reserve-x28,-reserve-x3,-reserve-x4,-reserve-x5,-reserve-x6,-reserve-x7,-reserve-x9,-rme,-sb,-sel2,-sha2,-sha3,-slow-misaligned-128store,-slow-paired-128,-slow-strqro-store,-sm4,-sme,-sme2,-sme2p1,-sme2p2,-sme-b16b16,-sme-f16f16,-sme-f64f64,-sme-f8f16,-sme-f8f32,-sme-fa64,-sme-i16i64,-sme-lutv2,-sme-mop4,-sme-tmop,-spe,-spe-eef,-specres2,-specrestrict,-ssbs,-ssve-aes,-ssve-bitperm,-ssve-fp8dot2,-ssve-fp8dot4,-ssve-fp8fma,-store-pair-suppress,-stp-aligned-only,-strict-align,-sve,-sve2,-sve2-aes,-sve2-bitperm,-sve2-sha3,-sve2-sm4,-sve2p1,-sve2p2,-sve-aes,-sve-aes2,-sve-b16b16,-sve-bfscale,-sve-bitperm,-sve-f16f32mm,-tagged-globals,-the,-tlb-rmi,-tlbiw,-tme,-tpidr-el1,-tpidr-el2,-tpidr-el3,-tpidrro-el0,-tracev8.4,-uaops,-use-experimental-zeroing-pseudos,-use-fixed-over-scalable-if-equal-cost,-use-reciprocal-square-root,-v8.1a,-v8.2a,-v8.3a,-v8.4a,-v8.5a,-v8.6a,-v8.7a,-v8.8a,-v8.9a,-v8a,-v8r,-v9.1a,-v9.2a,-v9.3a,-v9.4a,-v9.5a,-v9.6a,-v9a,-vh,-wfxt,-xs,-zcm,-zcz,-zcz-fp-workaround,-zcz-gp" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.module.flags = !{}
```

Compiling this with `llc repro.ll -O0` with LLVM 19 and 20 yields this codegen diff:

```diff
--- repro.19.s  2025-03-05 08:29:31.485173087 +0100
+++ repro.20.s  2025-03-05 08:29:34.672295525 +0100
@@ -1,5 +1,5 @@
-       .text
        .file   "repro"
+       .text
        .globl  repro                           // -- Begin function repro
        .p2align        2
        .type   repro,@function
@@ -16,15 +16,16 @@
        mov     x8, xzr
        str     x8, [sp]
        ldr     x8, [sp, #8]
-       ldr     d1, [sp]
-                                        // implicit-def: $q0
-       fmov    d0, d1
+       ldr     d0, [sp]
+                                        // kill: def $q0 killed $d0
        mov     v0.d[1], x8
        rev64   v0.16b, v0.16b
        cnt     v0.16b, v0.16b
-       uaddlv  h0, v0.16b
-                                        // kill: def $q0 killed $h0
-       fmov    w0, s0
+       addv    b1, v0.16b
+                                        // implicit-def: $q0
+       fmov    s0, s1
+       rev32   v0.16b, v0.16b
+       mov     w0, v0.s[3]
        ldp     x29, x30, [sp, #16]             // 16-byte Folded Reload
        add     sp, sp, #32
        ret
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Alex Rønne Petersen (alexrp)

<details>
Consider this Zig program:

```zig
pub fn main() void {
    var x: u128 = 0b11111111000110001100010000100001000011000011100101010001;
    _ = &amp;x;
    @<!-- -->import("std").process.exit(@<!-- -->popCount(x));
}
```

Running it with `qemu-aarch64_be` will produce `24` with LLVM 19, but `0` with LLVM 20.

Isolating the `@<!-- -->llvm.ctpop.i128` a bit:

```llvm
; ModuleID = 'BitcodeBuffer'
source_filename = "repro"
target datalayout = "E-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64_be-unknown-linux4.19.0-unknown"

@<!-- -->builtin.zig_backend = internal unnamed_addr constant i64 2, align 8
@<!-- -->start.simplified_logic = internal unnamed_addr constant i1 false, align 1
@<!-- -->builtin.output_mode = internal unnamed_addr constant i2 -2, align 1

; Function Attrs: nosanitize_coverage nounwind skipprofile
define dso_local i32 @<!-- -->repro() #<!-- -->0 {
  %1 = alloca [16 x i8], align 16
  store i128 71803349708323153, ptr %1, align 16
  %2 = load i128, ptr %1, align 16
  %3 = call i128 @<!-- -->llvm.ctpop.i128(i128 %2)
  %4 = trunc i128 %3 to i8
  %5 = zext i8 %4 to i32
  ret i32 %5
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i128 @<!-- -->llvm.ctpop.i128(i128) #<!-- -->1

attributes #<!-- -->0 = { nosanitize_coverage nounwind skipprofile "frame-pointer"="all" "target-cpu"="generic" "target-features"="+enable-select-opt,+ete,+fp-armv8,+fuse-adrp-add,+fuse-aes,+neon,+trbe,+use-postra-scheduler,-addr-lsl-slow-14,-aes,-aggressive-fma,-alternate-sextload-cvt-f32-pattern,-altnzcv,-alu-lsl-fast,-am,-amvs,-arith-bcc-fusion,-arith-cbz-fusion,-ascend-store-address,-avoid-ldapur,-balance-fp-ops,-bf16,-brbe,-bti,-call-saved-x10,-call-saved-x11,-call-saved-x12,-call-saved-x13,-call-saved-x14,-call-saved-x15,-call-saved-x18,-call-saved-x8,-call-saved-x9,-ccdp,-ccidx,-ccpp,-chk,-clrbhb,-cmp-bcc-fusion,-cmpbr,-complxnum,-CONTEXTIDREL2,-cpa,-crc,-crypto,-cssc,-d128,-disable-latency-sched-heuristic,-disable-ldp,-disable-stp,-dit,-dotprod,-ecv,-el2vmsa,-el3,-exynos-cheap-as-move,-f32mm,-f64mm,-f8f16mm,-f8f32mm,-faminmax,-fgt,-fix-cortex-a53-835769,-flagm,-fmv,-force-32bit-jump-tables,-fp16fml,-fp8,-fp8dot2,-fp8dot4,-fp8fma,-fpac,-fprcvt,-fptoint,-fujitsu-monaka,-fullfp16,-fuse-address,-fuse-addsub-2reg-const1,-fuse-arith-logic,-fuse-crypto-eor,-fuse-csel,-fuse-literals,-gcs,-harden-sls-blr,-harden-sls-nocomdat,-harden-sls-retbr,-hbc,-hcx,-i8mm,-ite,-jsconv,-ldp-aligned-only,-lor,-ls64,-lse,-lse128,-lse2,-lsfe,-lsui,-lut,-mec,-mops,-mpam,-mte,-nmi,-no-bti-at-return-twice,-no-neg-immediates,-no-sve-fp-ld1r,-no-zcz-fp,-nv,-occmo,-outline-atomics,-pan,-pan-rwv,-pauth,-pauth-lr,-pcdphint,-perfmon,-pops,-predictable-select-expensive,-predres,-prfm-slc-target,-rand,-ras,-rasv2,-rcpc,-rcpc3,-rcpc-immo,-rdm,-reserve-lr-for-ra,-reserve-x1,-reserve-x10,-reserve-x11,-reserve-x12,-reserve-x13,-reserve-x14,-reserve-x15,-reserve-x18,-reserve-x2,-reserve-x20,-reserve-x21,-reserve-x22,-reserve-x23,-reserve-x24,-reserve-x25,-reserve-x26,-reserve-x27,-reserve-x28,-reserve-x3,-reserve-x4,-reserve-x5,-reserve-x6,-reserve-x7,-reserve-x9,-rme,-sb,-sel2,-sha2,-sha3,-slow-misaligned-128store,-slow-paired-128,-slow-strqro-store,-sm4,-sme,-sme2,-sme2p1,-sme2p2,-sme-b16b16,-sme-f16f16,-sme-f64f64,-sme-f8f16,-sme-f8f32,-sme-fa64,-sme-i16i64,-sme-lutv2,-sme-mop4,-sme-tmop,-spe,-spe-eef,-specres2,-specrestrict,-ssbs,-ssve-aes,-ssve-bitperm,-ssve-fp8dot2,-ssve-fp8dot4,-ssve-fp8fma,-store-pair-suppress,-stp-aligned-only,-strict-align,-sve,-sve2,-sve2-aes,-sve2-bitperm,-sve2-sha3,-sve2-sm4,-sve2p1,-sve2p2,-sve-aes,-sve-aes2,-sve-b16b16,-sve-bfscale,-sve-bitperm,-sve-f16f32mm,-tagged-globals,-the,-tlb-rmi,-tlbiw,-tme,-tpidr-el1,-tpidr-el2,-tpidr-el3,-tpidrro-el0,-tracev8.4,-uaops,-use-experimental-zeroing-pseudos,-use-fixed-over-scalable-if-equal-cost,-use-reciprocal-square-root,-v8.1a,-v8.2a,-v8.3a,-v8.4a,-v8.5a,-v8.6a,-v8.7a,-v8.8a,-v8.9a,-v8a,-v8r,-v9.1a,-v9.2a,-v9.3a,-v9.4a,-v9.5a,-v9.6a,-v9a,-vh,-wfxt,-xs,-zcm,-zcz,-zcz-fp-workaround,-zcz-gp" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

!llvm.module.flags = !{}
```

Compiling this with `llc repro.ll -O0` with LLVM 19 and 20 yields this codegen diff:

```diff
--- repro.19.s  2025-03-05 08:29:31.485173087 +0100
+++ repro.20.s  2025-03-05 08:29:34.672295525 +0100
@@ -1,5 +1,5 @@
-       .text
        .file   "repro"
+       .text
        .globl  repro                           // -- Begin function repro
        .p2align        2
        .type   repro,@<!-- -->function
@@ -16,15 +16,16 @@
        mov     x8, xzr
        str     x8, [sp]
        ldr     x8, [sp, #<!-- -->8]
-       ldr     d1, [sp]
-                                        // implicit-def: $q0
-       fmov    d0, d1
+       ldr     d0, [sp]
+                                        // kill: def $q0 killed $d0
        mov     v0.d[1], x8
        rev64   v0.16b, v0.16b
        cnt     v0.16b, v0.16b
-       uaddlv  h0, v0.16b
-                                        // kill: def $q0 killed $h0
-       fmov    w0, s0
+       addv    b1, v0.16b
+                                        // implicit-def: $q0
+       fmov    s0, s1
+       rev32   v0.16b, v0.16b
+       mov     w0, v0.s[3]
        ldp     x29, x30, [sp, #<!-- -->16]             // 16-byte Folded Reload
        add     sp, sp, #<!-- -->32
        ret
```
</details>


---

### Comment 2 - davemgreen

/cherry-pick ab811e75734a77247dae6df1579fa6f29394f200

---

### Comment 3 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: ab811e75734a77247dae6df1579fa6f29394f200

https://github.com/llvm/llvm-project/actions/runs/13684705916

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 4 - davemgreen

/cherry-pick b673a59c9ae5 ab811e75734a

---

### Comment 5 - llvmbot

/pull-request llvm/llvm-project#129996

---

