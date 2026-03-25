# [HLSL][Matrix] Assignment of matrix single element access to a scalar gives vector-to-scalar conversion warning

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/182599
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2026-02-21T00:01:32Z

## Body

Assigning a matrix single element access to a scalar variable results in Clang emitting a vector-to-scalar implicit conversion warning

https://hlsl.godbolt.org/z/Taqrq85bo

Compiling the following code with `clang-dxc -T lib_6_8`
```hlsl
export float fn(float4x4 f) {
    return f._m00;
}
```

Clang emits a warning:
```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
<source>:2:12: warning: implicit conversion turns vector to scalar: 'vector<float, 1>' (vector of 1 'float' value) to 'float' [-Wconversion]
    2 |     return f._m00;
      |     ~~~~~~ ^~~~~~
1 warning generated.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
Assigning a matrix single element access to a scalar variable results in Clang emitting a vector-to-scalar conversion warning

https://hlsl.godbolt.org/z/Taqrq85bo

Compiling the following code with `clang-dxc -T lib_6_8`
```hlsl
export float fn(float4x4 f) {
    return f._m00;
}
```

Clang emits a warning:
```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
&lt;source&gt;:2:12: warning: implicit conversion turns vector to scalar: 'vector&lt;float, 1&gt;' (vector of 1 'float' value) to 'float' [-Wconversion]
    2 |     return f._m00;
      |     ~~~~~~ ^~~~~~
1 warning generated.
```
</details>


---

