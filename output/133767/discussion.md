# [HLSL] Assigning array from cbuffer fails

**Author:** llvm-beanz
**URL:** https://github.com/llvm/llvm-project/issues/133767
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-04-21T15:18:58Z

## Body

Now that cbuffer values are declared in a unique address space we're seeing a failure assigning an array from a cbuffer:

```hlsl
cbuffer CBArrays : register(b0) {
  float c1[2];
  int4 c2[2][2];
  bool c3[2];
}

struct Arrays {
  float c1[2];
  int4 c2[2][2];
  bool c3[2];
};

RWStructuredBuffer<Arrays> Out : register(u0);

[numthreads(1,1,1)]
void main() {
  Out[0].c1 = c1;
  Out[0].c2 = c2;
  Out[0].c3 = c3;
}
```

[Compiler Explorer](https://hlsl.godbolt.org/z/ne6avb8vd)

## Comments

### Comment 1 - damyanp

Some more investigation required before we can estimate this.

We need to address this for the DML shaders.

---

### Comment 2 - spall

The assignment fails because in Sema Checking because the ToType and the FromType do not have the same Qualified Type; the FromType has an address space qualifier. 
Expected solution: 
- Update Sema Checking to add an LValue to RValue cast in this case.
- Update Codegen to ensure that the LValue to RValue cast loads from the address space to a temporary.
- Add tests.

---

### Comment 3 - spall

Blocked on a design discussion

---

### Comment 4 - spall

No longer blocked on a design discussion.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Chris B (llvm-beanz)

<details>
Now that cbuffer values are declared in a unique address space we're seeing a failure assigning an array from a cbuffer:

```hlsl
cbuffer CBArrays : register(b0) {
  float c1[2];
  int4 c2[2][2];
  bool c3[2];
}

struct Arrays {
  float c1[2];
  int4 c2[2][2];
  bool c3[2];
};

RWStructuredBuffer&lt;Arrays&gt; Out : register(u0);

[numthreads(1,1,1)]
void main() {
  Out[0].c1 = c1;
  Out[0].c2 = c2;
  Out[0].c3 = c3;
}
```

[Compiler Explorer](https://hlsl.godbolt.org/z/ne6avb8vd)
</details>


---

