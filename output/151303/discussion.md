# [InstCombine] Incorrect fabs + nsz fold

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/151303
**Status:** Open
**Labels:** miscompilation, llvm:instcombine

## Body

https://alive2.llvm.org/ce/z/FmBrma
```llvm
define float @src(float %a) {
  %i32 = bitcast float %a to i32
  %cmp = icmp slt i32 %i32, 0
  call void @llvm.assume(i1 %cmp)

  %b = fneg nsz float %a
  %c = call float @llvm.fabs(float %b)
  ret float %c
}
```
The fabs is optimized away here, even though `fneg nsz` can flip the sign bit of zero.

## Comments

### Comment 1 - nikic

cc @dtcxzyw I guess this is basically the same issue as in this comment: https://discourse.llvm.org/t/rfc-clarify-the-behavior-of-fp-operations-on-bit-strings-with-nsz-flag/85981/6?u=nikic

---

### Comment 2 - pedroMVicente

I am working on this issue.

---

