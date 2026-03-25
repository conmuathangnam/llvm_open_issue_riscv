# AArch64 backend folds fcmp nnan nsz + select to fminnm, causing confusion between +0.0 and -0.0

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/137998
**Status:** Closed
**Labels:** miscompilation, floating-point, llvm:analysis
**Closed Date:** 2025-06-02T10:21:16Z

## Body

we're lowing this:
```llvm
define float @f(float %0, float %1) {
  %3 = fcmp nnan nsz ole float %0, %1
  %4 = select i1 %3, float %0, float %1
  ret float %4
}
```
to:
```
_f: 
	fminnm	s0, s0, s1
	ret
```

but this appears to be incorrect for `f(0.0, -0.0)`. in the LLVM code, %3 = true, so we return 0.0. but the ARM version returns -0.0 for the same inputs. 

cc @nunoplopes @arsenm @dtcxzyw 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
we're lowing this:
```llvm
define float @<!-- -->f(float %0, float %1) {
  %3 = fcmp nnan nsz ole float %0, %1
  %4 = select i1 %3, float %0, float %1
  ret float %4
}
```
to:
```
_f: 
	fminnm	s0, s0, s1
	ret
```

but this appears to be incorrect for `f(0.0, -0.0)`. in the LLVM code, %3 = true, so we return 0.0. but the ARM version returns -0.0 for the same inputs. 

cc @<!-- -->nunoplopes @<!-- -->arsenm @<!-- -->dtcxzyw 
</details>


---

### Comment 2 - arsenm

The logic appears to only account for literal zeros: https://github.com/llvm/llvm-project/blob/2a5ee2501d6249933b0dd4f81a4ec56e146b9669/llvm/lib/Analysis/ValueTracking.cpp#L8895

---

### Comment 3 - dtcxzyw

`matchDecomposedSelectPattern` takes FMF from fcmp:
https://github.com/llvm/llvm-project/blob/c02496901d979ddc7cd83aec34e408adba81e433/llvm/lib/Analysis/ValueTracking.cpp#L9221-L9223

But we need to check `nsz` flag on select: https://alive2.llvm.org/ce/z/Jo3g3P


---

