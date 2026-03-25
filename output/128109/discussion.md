# [HLSL][Sema] Misleading error message with `packoffset` on a struct or array

**Author:** hekota
**URL:** https://github.com/llvm/llvm-project/issues/128109
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-12-20T18:34:38Z

## Body

It seems that DXC only allows `packsoffset` annotations on a struct or array to be aligned to the buffer row (`packoffset(c#)` or `packoffset(c#.x)`).
Placing the struct on the `.y`, `.z`. or `.w` element results in an `error: register or offset bind not valid`.

Clang reports `error: packoffset cannot cross register boundary` which is confusing because the struct or array theoretically fits the buffer row (16 bytes).

```
struct S {
  float a;
};

cbuffer CB0 {
  S s : packoffset(c0.y);
  float f[2] : packoffset(c2.y);
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Helena Kotas (hekota)

<details>
It seems that DXC only allows `packsoffset` annotations on a struct or array to be aligned to the buffer row (`packoffset(c#)` or `packoffset(c#.x)`).
Placing the struct on the `.y`, `.z`. or `.w` element results in an `error: register or offset bind not valid`.

Clang reports `error: packoffset cannot cross register boundary` which is confusing because the struct or array theoretically fits the buffer row (16 bytes).

```
struct S {
  float a;
};

cbuffer CB0 {
  S s : packoffset(c0.y);
  float f[2] : packoffset(c2.y);
}
```
</details>


---

