# Miscompile with __arm_locally_streaming with -march=armv8-a+sme

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/145635
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-06-26T12:26:35Z

## Body

Consider:

```
__arm_locally_streaming void f(int *p, int n) {
  #pragma clang loop vectorize_width(4, scalable)
  for (int i = 0; i < n; ++i) {
    p[i]++;
  }
}
```

Compile with `-march=armv8-a+sme`.  The resulting assembly starts with:

```
        sub     sp, sp, #96
        rdsvl   x9, #1
        lsr     x9, x9, #3
        str     x9, [sp, #16]
        mov     x9, x0
        bl      __arm_get_current_vg
[...]
```

The bl corrupts x30.

Demo at https://godbolt.org/z/Ga9zaWEen

Workarounds:

- Use `-march=armv8-a+sve+sme` or similar
- Use -mno-omit-leaf-frame-pointer

CC @sdesmalen-arm @MacDue 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Eli Friedman (efriedma-quic)

<details>
Consider:

```
__arm_locally_streaming void f(int *p, int n) {
  #pragma clang loop vectorize_width(4, scalable)
  for (int i = 0; i &lt; n; ++i) {
    p[i]++;
  }
}
```

Compile with `-march=armv8-a+sme`.  The resulting assembly starts with:

```
        sub     sp, sp, #<!-- -->96
        rdsvl   x9, #<!-- -->1
        lsr     x9, x9, #<!-- -->3
        str     x9, [sp, #<!-- -->16]
        mov     x9, x0
        bl      __arm_get_current_vg
[...]
```

The bl corrupts x30.

Demo at https://godbolt.org/z/Ga9zaWEen

Workarounds:

- Use `-march=armv8-a+sve+sme` or similar
- Use -mno-omit-leaf-frame-pointer

CC @<!-- -->sdesmalen-arm @<!-- -->MacDue 
</details>


---

### Comment 2 - MacDue

I'll take a look into this later today 👍 

---

