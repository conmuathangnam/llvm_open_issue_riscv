# [LoongArch] Miscompilation of llvm.fptoui.sat when input is f64 Nan with non-zero payload

**Author:** 12101111
**URL:** https://github.com/llvm/llvm-project/issues/179221
**Status:** Closed
**Labels:** miscompilation, release:backport, llvm:SelectionDAG
**Closed Date:** 2026-03-04T08:38:36Z

## Body

* Input: f64 or `<2 x double>` Nan with non-zero payload
* Expected Output: i32 0
* Actual Output: i32 -1 (0xffffffff)

The cases are found from wasm test suite

```wasm
(module
  (func (export "i32.trunc_sat_f64_u") (param $x f64) (result i32) (i32.trunc_sat_f64_u (local.get $x)))
  (func (export "i32x4.trunc_sat_f64x2_u_zero") (param v128) (result v128) (i32x4.trunc_sat_f64x2_u_zero (local.get 0)))
)

(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const nan)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const nan:0x4000000000000)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const -nan)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const -nan:0x4000000000000)) (i32.const 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 +nan +nan))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 -nan -nan))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 nan:0x444444 nan:0x444444))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 -nan:0x444444 -nan:0x444444))
                                                      (v128.const i32x4 0 0 0 0))

```

LLVM IR:

```llvm
; ModuleID = 'module'
source_filename = "module"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "loongarch64-unknown-linux-musl"

; Function Attrs: mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable
define protected i32 @func0(ptr noalias noundef nonnull readnone captures(none) %0, double %1) local_unnamed_addr #1 {
entry:
  %i32.trunc_sat_f64_u = tail call i32 @llvm.fptoui.sat.i32.f64(double %1) #3
  ret i32 %i32.trunc_sat_f64_u
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none)
declare i32 @llvm.fptoui.sat.i32.f64(double) #2

; Function Attrs: mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable
define protected <2 x i64> @func1(ptr noalias noundef nonnull readnone captures(none) %0, <2 x i64> %1) local_unnamed_addr #1 {
entry:
  %i32x4.truncsatu = bitcast <2 x i64> %1 to <2 x double>
  %i32x4.truncsatu.call_intrinsic = tail call <2 x i32> @llvm.fptoui.sat.v2i32.v2f64(<2 x double> %i32x4.truncsatu) #3
  %i32x4.truncsatu.shuffle = shufflevector <2 x i32> %i32x4.truncsatu.call_intrinsic, <2 x i32> zeroinitializer, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
  %i32x4.truncsatu.bitcast = bitcast <4 x i32> %i32x4.truncsatu.shuffle to <2 x i64>
  ret <2 x i64> %i32x4.truncsatu.bitcast
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none)
declare <2 x i32> @llvm.fptoui.sat.v2i32.v2f64(<2 x double>) #2

attributes #1 = { mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable "frame-pointer"="all" }
attributes #2 = { mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none) }
attributes #3 = { strictfp }

!llvm.module.flags = !{!0}

!0 = !{i32 8, !"PIC Level", i32 2}

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: 韩朴宇 (12101111)

<details>
* Input: f64 or `&lt;2 x double&gt;` Nan with non-zero payload
* Expected Output: i32 0
* Actual Output: i32 -1 (0xffffffff)

The cases are found from wasm test suite

```wasm
(module
  (func (export "i32.trunc_sat_f64_u") (param $x f64) (result i32) (i32.trunc_sat_f64_u (local.get $x)))
  (func (export "i32x4.trunc_sat_f64x2_u_zero") (param v128) (result v128) (i32x4.trunc_sat_f64x2_u_zero (local.get 0)))
)

(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const nan)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const nan:0x4000000000000)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const -nan)) (i32.const 0))
(assert_return (invoke "i32.trunc_sat_f64_u" (f64.const -nan:0x4000000000000)) (i32.const 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 +nan +nan))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 -nan -nan))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 nan:0x444444 nan:0x444444))
                                                      (v128.const i32x4 0 0 0 0))
(assert_return (invoke "i32x4.trunc_sat_f64x2_u_zero" (v128.const f64x2 -nan:0x444444 -nan:0x444444))
                                                      (v128.const i32x4 0 0 0 0))

```

LLVM IR:

```llvm
; ModuleID = 'module'
source_filename = "module"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "loongarch64-unknown-linux-musl"

; Function Attrs: mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable
define protected i32 @<!-- -->func0(ptr noalias noundef nonnull readnone captures(none) %0, double %1) local_unnamed_addr #<!-- -->1 {
entry:
  %i32.trunc_sat_f64_u = tail call i32 @<!-- -->llvm.fptoui.sat.i32.f64(double %1) #<!-- -->3
  ret i32 %i32.trunc_sat_f64_u
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.fptoui.sat.i32.f64(double) #<!-- -->2

; Function Attrs: mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable
define protected &lt;2 x i64&gt; @<!-- -->func1(ptr noalias noundef nonnull readnone captures(none) %0, &lt;2 x i64&gt; %1) local_unnamed_addr #<!-- -->1 {
entry:
  %i32x4.truncsatu = bitcast &lt;2 x i64&gt; %1 to &lt;2 x double&gt;
  %i32x4.truncsatu.call_intrinsic = tail call &lt;2 x i32&gt; @<!-- -->llvm.fptoui.sat.v2i32.v2f64(&lt;2 x double&gt; %i32x4.truncsatu) #<!-- -->3
  %i32x4.truncsatu.shuffle = shufflevector &lt;2 x i32&gt; %i32x4.truncsatu.call_intrinsic, &lt;2 x i32&gt; zeroinitializer, &lt;4 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 3&gt;
  %i32x4.truncsatu.bitcast = bitcast &lt;4 x i32&gt; %i32x4.truncsatu.shuffle to &lt;2 x i64&gt;
  ret &lt;2 x i64&gt; %i32x4.truncsatu.bitcast
}

; Function Attrs: mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none)
declare &lt;2 x i32&gt; @<!-- -->llvm.fptoui.sat.v2i32.v2f64(&lt;2 x double&gt;) #<!-- -->2

attributes #<!-- -->1 = { mustprogress nofree norecurse nosync nonlazybind strictfp willreturn memory(none) uwtable "frame-pointer"="all" }
attributes #<!-- -->2 = { mustprogress nocallback nocreateundeforpoison nofree nosync speculatable willreturn memory(none) }
attributes #<!-- -->3 = { strictfp }

!llvm.module.flags = !{!0}

!0 = !{i32 8, !"PIC Level", i32 2}

```
</details>


---

### Comment 2 - wangleiat

Fixed by: #180178

---

### Comment 3 - wangleiat

/cherry-pick cfca635efc11e4de10d560810fd4837510953c05

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#184565

---

