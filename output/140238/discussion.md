# [LoopInterchange] Incorrect exchange happens when the Levels of Dependence is less than the depth of the loop nest

**Author:** kasuga-fj
**URL:** https://github.com/llvm/llvm-project/issues/140238
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-06-05T07:44:03Z

## Body

Input:
```c
#include <stdio.h>

float A[4][4];
float V[16];

void g(float *dst, float *src) {
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      dst[i * 4 + j] = src[j * 4 + i] + A[i][j];
}

int main(void) {
  for (int i = 0; i < 16; i++) {
    A[i / 4][i % 4] = i;
    V[i] = i * i;
  }

  g(V, V);

  for (int i = 0; i < 16; i++)
    printf("V[%d]=%f\n", i, V[i]);
  return 0;
}
```

godbolt: https://godbolt.org/z/dq6Kad14a

The root cause is that the dependency matrix for the loops in `g` is `I I`, which is treated same as `= =` in legality check. An `I` dependence is used to fill a direction vector if its length is less than the depth of the loop nest.

https://github.com/llvm/llvm-project/blob/9f77c26ec641c7f0c353f74ee6ee072086e2f3d7/llvm/lib/Transforms/Scalar/LoopInterchange.cpp#L212-L214

In this case, since the alias check results in MayAlias, DependenceInfo skips the analysis and returns a default Dependence object, whose `Levels` is always 0.

https://github.com/llvm/llvm-project/blob/b712590ef4acfd9f0bea42aff695b22ca99ae5dd/llvm/lib/Analysis/DependenceAnalysis.cpp#L3605-L3609

A similar problem occurs when the dimension of arrays used in the target loops is less than the depth of the loop nest, such as in the following case

```c
float A[4][4][4];
float V[4];

void f() {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        V[i] += A[i][j][k];
}
```

godbolt: https://godbolt.org/z/Yss6oe3eb

In this case, exchanging the j- and k-loop fails because the IR structure is currently unsupported. However, it should be rejected by dependence reason, because that exchange changes the order of addition of float values.

For safety, I think `I` dependencies should be replaced with `*`

## Comments

### Comment 1 - kasuga-fj

cc: @sjoerdmeijer 
This should also be fixed before #124911. I think I'll submit a PR by the end of the month.

---

