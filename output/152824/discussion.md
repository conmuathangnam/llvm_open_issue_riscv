# [llvm] Incorrect simplification of fabs intrinsic

**Author:** ojhunt
**URL:** https://github.com/llvm/llvm-project/issues/152824
**Status:** Closed
**Labels:** miscompilation, floating-point, llvm:ir
**Closed Date:** 2025-08-10T14:55:06Z

## Body

The below test case fails at optimization levels greater than -O1
```cpp
#include <math.h>
#include <stdio.h>

__attribute__((noinline))
int test(double a, double b)
{
  int maxIdx = 0;
  double maxDot = 0.0;
  for (int i = 0; i < 2; ++i) {
    double dot = a * b;
    if (fabs(dot) > fabs(maxDot)) {
      maxIdx = i;
      maxDot = dot;
    }
  }
  return maxIdx;
}

int main(int argc, char *argv[])
{

  int i = test(1.0, -1.0);
  int failed = (i != 0);

  printf("Returned %d, should be 0: %s\n", i, (failed ? "failed" : "success"));

  return failed;
}
```

This is a regression introduced in #76360

~The bug is due to https://github.com/llvm/llvm-project/pull/76360/files#diff-532fd830a7b080a7121645e6c8dccb2f94a7e25450c74aa496b8600f8f4481ccR6220~

~comparing a std::optional<bool> to false rather than dereferencing it (maybe we need a way to warn about this? it's a super easy error)~

[Implicit conversion means the comparison I thought was wrong, was correct, and my PoC fix worked by functionally disabling signedness propagation entirely]

## Comments

### Comment 1 - nikic

Seems to go wrong during unroling: https://alive2.llvm.org/ce/z/sxJ5eo

---

### Comment 2 - nikic

Or rather in simplification during unrolling (https://alive2.llvm.org/ce/z/66EoES):
```llvm
define half @src(half noundef %mul) {
entry:
  %abs = call half @llvm.fabs.f16(half %mul)
  %cmp1 = fcmp ogt half %abs, 0.000000e+00
  %maxDot.1 = select i1 %cmp1, half %mul, half 0.000000e+00
  %abs2 = call half @llvm.fabs.f16(half %maxDot.1)
  ret half %abs2
}

define half @tgt(half noundef %mul) {
entry:
  %abs = call half @llvm.fabs.f16(half %mul)
  %cmp1 = fcmp ogt half %abs, 0xH0000
  %maxDot.1 = select i1 %cmp1, half %mul, half 0xH0000
  ret half %maxDot.1
}
```

The second abs should not be eliminated.

---

### Comment 3 - nikic

Presumably something is wrong in https://github.com/llvm/llvm-project/blob/fc44a4fcd3c54be927c15ddd9211aca1501633e7/llvm/lib/Analysis/ValueTracking.cpp#L4852-L4860 or the fcmpImpliesClass utility it calls.

---

### Comment 4 - dtcxzyw

@nikic @ojhunt Are you working on this issue?


---

### Comment 5 - nikic

I'm not working on it.

---

### Comment 6 - ojhunt

> [@nikic](https://github.com/nikic) [@ojhunt](https://github.com/ojhunt) Are you working on this issue?

I am not - I don't know anything like enough about the IR pipelines :D

---

### Comment 7 - dtcxzyw

Okay. I will post a fix tomorrow.

---

### Comment 8 - ojhunt

> Okay. I will post a fix tomorrow.

cool, I'd love to see how this kind of issue comes about in IR land (obviously everything looks quite different from in clang)

---

