# [InstCombine] FMF flags should not be propagated

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121432
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, floating-point
**Closed Date:** 2025-01-06T08:23:47Z

## Body

Alive2: https://alive2.llvm.org/ce/z/ajCxTh
```
define float @src1(float noundef %x, float noundef %y, float noundef %z) {
  %s = call float @llvm.copysign.f32(float %y, float %z)
  %r = call nnan float @llvm.copysign.f32(float %x, float %s)
  ret float %r
}

define float @tgt1(float noundef %x, float noundef %y, float noundef %z) {
  %r = call nnan float @llvm.copysign.f32(float %x, float %z)
  ret float %r
}

define float @src2(float noundef %x, float noundef %y, float noundef %z) {
  %s = call float @llvm.copysign.f32(float %y, float %z)
  %r = call ninf float @llvm.copysign.f32(float %x, float %s)
  ret float %r
}

define float @tgt2(float noundef %x, float noundef %y, float noundef %z) {
  %r = call ninf float @llvm.copysign.f32(float %x, float %z)
  ret float %r
}

define float @src3(float noundef %x, float noundef %y, float noundef %z) {
  %s = call float @llvm.copysign.f32(float %y, float %z)
  %r = call nsz float @llvm.copysign.f32(float %x, float %s)
  ret float %r
}

define float @tgt3(float noundef %x, float noundef %y, float noundef %z) {
  %r = call nsz float @llvm.copysign.f32(float %x, float %z)
  ret float %r
}
```


