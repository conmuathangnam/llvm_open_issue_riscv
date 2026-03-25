# ComplexDeinterleavingPass: Instruction does not dominate all uses!

**Author:** alexey-bataev
**URL:** https://github.com/llvm/llvm-project/issues/178671
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2026-02-26T10:05:14Z

## Body

https://godbolt.org/z/eTPaaPT6a
https://godbolt.org/z/731o7aGdx

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Alexey Bataev (alexey-bataev)

<details>
https://godbolt.org/z/eTPaaPT6a
</details>


---

### Comment 2 - HolyMolyCowMan

Dumping the body after the `ComplexDeinterleavingPass` but before the crash shows:

```llvm
*** IR Dump After Complex Deinterleaving Pass (complex-deinterleaving) ***
define double @test(<vscale x 2 x i1> %0, i1 %1, <vscale x 2 x i1> %2) #0 {
.preheader159:
  %3 = call <vscale x 4 x double> @llvm.vector.interleave2.nxv4f64(<vscale x 2 x double> zeroinitializer, <vscale x 2 x double> zeroinitializer)
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %.preheader159
  %active.lane.mask = phi <vscale x 2 x i1> [ zeroinitializer, %.preheader159 ], [ %0, %vector.body ]
  %4 = phi <vscale x 4 x double> [ %3, %.preheader159 ], [ %26, %vector.body ]
  %5 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 0)
  %6 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 0)
  %7 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 2)
  %8 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 2)
  %9 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> %4, i64 0)
  %10 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> %4, i64 2)
  %11 = call <vscale x 2 x double> @llvm.aarch64.sve.fcmla.nxv2f64(<vscale x 2 x i1> splat (i1 true), <vscale x 2 x double> %9, <vscale x 2 x double> %5, <vscale x 2 x double> %6, i32 0)
  %12 = call <vscale x 2 x double> @llvm.aarch64.sve.fcmla.nxv2f64(<vscale x 2 x i1> splat (i1 true), <vscale x 2 x double> %10, <vscale x 2 x double> %7, <vscale x 2 x double> %8, i32 0)
  %13 = call <vscale x 4 x double> @llvm.vector.insert.nxv4f64.nxv2f64(<vscale x 4 x double> poison, <vscale x 2 x double> %11, i64 0)
  %14 = call <vscale x 4 x double> @llvm.vector.insert.nxv4f64.nxv2f64(<vscale x 4 x double> %13, <vscale x 2 x double> %12, i64 2)
  %15 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 0)
  %16 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 0)
  %17 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 2)
  %18 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> zeroinitializer, i64 2)
  %19 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> %14, i64 0)
  %20 = call <vscale x 2 x double> @llvm.vector.extract.nxv2f64.nxv4f64(<vscale x 4 x double> %14, i64 2)
  %21 = call <vscale x 2 x double> @llvm.aarch64.sve.fcmla.nxv2f64(<vscale x 2 x i1> splat (i1 true), <vscale x 2 x double> %19, <vscale x 2 x double> %15, <vscale x 2 x double> %16, i32 90)
  %22 = call <vscale x 2 x double> @llvm.aarch64.sve.fcmla.nxv2f64(<vscale x 2 x i1> splat (i1 true), <vscale x 2 x double> %20, <vscale x 2 x double> %17, <vscale x 2 x double> %18, i32 90)
  %23 = call <vscale x 4 x double> @llvm.vector.insert.nxv4f64.nxv2f64(<vscale x 4 x double> poison, <vscale x 2 x double> %21, i64 0)
  %24 = call <vscale x 4 x double> @llvm.vector.insert.nxv4f64.nxv2f64(<vscale x 4 x double> %23, <vscale x 2 x double> %22, i64 2)
  %25 = call <vscale x 4 x i1> @llvm.vector.interleave2.nxv4i1(<vscale x 2 x i1> %active.lane.mask, <vscale x 2 x i1> %active.lane.mask)
  %26 = select <vscale x 4 x i1> %25, <vscale x 4 x double> %24, <vscale x 4 x double> %4
  br i1 %1, label %vector.body, label %middle.block

middle.block:                                     ; preds = %vector.body
  %27 = extractvalue { <vscale x 2 x double>, <vscale x 2 x double> } %29, 1
  %28 = tail call double @llvm.vector.reduce.fadd.nxv2f64(double 0.000000e+00, <vscale x 2 x double> %27)
  br label %vector.body266

vector.body266:                                   ; preds = %vector.body266, %middle.block
  br i1 %1, label %vector.body266, label %.preheader.preheader

.preheader.preheader:                             ; preds = %vector.body266
  %29 = call { <vscale x 2 x double>, <vscale x 2 x double> } @llvm.vector.deinterleave2.nxv4f64(<vscale x 4 x double> %26)
  %30 = extractvalue { <vscale x 2 x double>, <vscale x 2 x double> } %29, 0
  %31 = tail call double @llvm.vector.reduce.fadd.nxv2f64(double 0.000000e+00, <vscale x 2 x double> %30)
  ret double %31
}
```

Notably, `.preheader.preheader` contains `%29 = call { <vscale x 2 x double>, <vscale x 2 x double> } @llvm.vector.deinterleave2.nxv4f64(<vscale x 4 x double> %26)` but `middle.block` (which is executed before) has `%27 = extractvalue { <vscale x 2 x double>, <vscale x 2 x double> } %29, 1`.

---

