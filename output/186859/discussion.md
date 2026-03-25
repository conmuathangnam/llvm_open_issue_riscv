# [HLSL][Matrix] Unable to splat a 1x1 matrix to a larger matrix

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/186859

## Body

1x1 matrices in clang can not be used to initialize a larger matrix via a matrix splat. 

While it is currently allowed in DXC, it is not currently allowed in Clang, and the hlsl-spec also does not state that it is possible: https://microsoft.github.io/hlsl-specs/specs/4-Conv.html

<img width="1216" height="321" alt="Image" src="https://github.com/user-attachments/assets/8d8a9c4e-beaa-4b19-a016-2badc0fc5ef0" />

https://godbolt.org/z/ebGW1b8sv

```hlsl
float4x4 Ex4_OneByOneMatrixToBigger(float1x1 m11)
{
    float4x4 M = (float4x4)m11;   // spec-style: every element = m11[0][0]
    // Fallback: splat the scalar explicitly
    // float s = m11[0][0];
    // float4x4 M = float4x4(
    //   s,s,s,s,
    //   s,s,s,s,
    //   s,s,s,s,
    //   s,s,s,s);
    return M;
}
```

```
<source>:17:18: error: conversion between matrix types 'float4x4' (aka 'matrix<float, 4, 4>') and 'matrix<float, 1, 1>' of different size is not allowed
   17 |     float4x4 M = (float4x4)m11;   // spec-style: every element = m11[0][0]
      |                  ^~~~~~~~~~~~~
1 error generated.
Compiler returned: 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
1x1 matrices in clang can not be used to initialize a larger matrix via a matrix splat. 

https://godbolt.org/z/ebGW1b8sv

```hlsl
float4x4 Ex4_OneByOneMatrixToBigger(float1x1 m11)
{
    float4x4 M = (float4x4)m11;   // spec-style: every element = m11[0][0]
    // Fallback: splat the scalar explicitly
    // float s = m11[0][0];
    // float4x4 M = float4x4(
    //   s,s,s,s,
    //   s,s,s,s,
    //   s,s,s,s,
    //   s,s,s,s);
    return M;
}
```

```
&lt;source&gt;:17:18: error: conversion between matrix types 'float4x4' (aka 'matrix&lt;float, 4, 4&gt;') and 'matrix&lt;float, 1, 1&gt;' of different size is not allowed
   17 |     float4x4 M = (float4x4)m11;   // spec-style: every element = m11[0][0]
      |                  ^~~~~~~~~~~~~
1 error generated.
Compiler returned: 1
```
</details>


---

