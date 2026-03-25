# [HLSL] Compiler crashes when cbuffer contains an array of empty structs

**Author:** hekota
**URL:** https://github.com/llvm/llvm-project/issues/183788
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, HLSL
**Closed Date:** 2026-03-10T23:31:19Z

## Body

Compiler crashes when cbuffer contains an array of empty structs:

```hlsl
struct A {
};

cbuffer CB {
  A a[2];
}

[numthreads(4,4,4)]
void main() {
}
```
https://godbolt.org/z/xbzEjEodc

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Helena Kotas (hekota)

<details>
Compiler crashes when cbuffer contains an array of empty structs:

```hlsl
struct A {
};

cbuffer CB {
  A a[2];
}

[numthreads(4,4,4)]
void main() {
}
```
https://godbolt.org/z/xbzEjEodc
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Helena Kotas (hekota)

<details>
Compiler crashes when cbuffer contains an array of empty structs:

```hlsl
struct A {
};

cbuffer CB {
  A a[2];
}

[numthreads(4,4,4)]
void main() {
}
```
https://godbolt.org/z/xbzEjEodc
</details>


---

