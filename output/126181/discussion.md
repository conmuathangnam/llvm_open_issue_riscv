# [FuncAttrs] `initializes` is incorrectly set on parameters with `byval`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/126181
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-02-10T09:34:05Z

## Body

Reproducer: https://godbolt.org/z/Mcx1nY4E7
```
; bin/opt -passes=function-attrs reduced.ll -S
%struct.a = type { i32, i64, i32, i32 }

@e = dso_local global { i32, [4 x i8], i64, i32, i32 } { i32 5, [4 x i8] zeroinitializer, i64 0, i32 1, i32 90986701 }, align 8

define i8 @ad(ptr byval(%struct.a) %ah) {
entry:
  store i32 0, ptr %ah, align 8
  %call = call i64 @af(ptr %ah)
  %0 = load i8, ptr %ah, align 1
  ret i8 %0
}

define i64 @af(ptr byval(%struct.a) %am) {
entry:
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %am, ptr noundef nonnull align 8 dereferenceable(24) @e, i64 24, i1 false)
  ret i64 0
}

```
```
define i8 @ad(ptr byval(%struct.a) captures(none) initializes((0, 4)) %ah) #0 {
  store i32 0, ptr %ah, align 8
  %call = call i64 @af(ptr %ah)
  %0 = load i8, ptr %ah, align 1
  ret i8 %0
}

define noundef i64 @af(ptr writeonly byval(%struct.a) captures(none) initializes((0, 24)) %am) #0 {
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %am, ptr noundef nonnull align 8 dereferenceable(24) @e, i64 24, i1 false)
  ret i64 0
}
```
After DSE:
```
define i8 @ad(ptr byval(%struct.a) captures(none) initializes((0, 4)) %ah) #0 {
  %call = call i64 @af(ptr %ah)
  %0 = load i8, ptr %ah, align 1
  ret i8 %0
}

define noundef i64 @af(ptr writeonly byval(%struct.a) captures(none) initializes((0, 24)) %am) #0 {
  ret i64 0
}
```

llvm version: d21fc58aeeaa7f0369a24dbe70a0360e0edbf76f


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Yingwei Zheng (dtcxzyw)

<details>
Reproducer: https://godbolt.org/z/PexPvPjf6
```
#include &lt;stdint.h&gt;
#include &lt;stdio.h&gt;
int8_t safe_mul_func_int8_t_s_s(int8_t si1, int8_t si2) {

  return (((si1 &gt; 0) &amp;&amp; (si2 &gt; 0) &amp;&amp; (si1 &gt; (INT8_MAX / si2))) ||
          ((si1 &gt; 0) &amp;&amp; (si2 &lt;= 0) &amp;&amp; (si2 &lt; (INT8_MIN / si1))) ||
          ((si1 &lt;= 0) &amp;&amp; (si2 &gt; 0) &amp;&amp; (si1 &lt; (INT8_MIN / si2))) ||
          ((si1 &lt;= 0) &amp;&amp; (si2 &lt;= 0) &amp;&amp; (si1 != 0) &amp;&amp; (si2 &lt; (INT8_MAX / si1))))
             ? 0
             : si1 * si2;
}
uint64_t safe_div_func_uint64_t_u_u(uint64_t ui1, uint64_t ui2) {
  return (ui2 == 0) ? 0 : (ui1 / ui2);
}
struct a {
  uint32_t b;
  uint64_t c;
  int32_t d;
  int32_t z;
} e = {5, 0, 1, 90986701};
int32_t *f;
int32_t h, r, p, q, s, m;
uint8_t g[3][3][4];
uint32_t o;
int64_t l;
struct a aa[1];
uint32_t n[1][1];
const int32_t *ab(int32_t *, struct a);
uint8_t ad(struct a);
int32_t *ac(int32_t *, struct a, uint32_t *, uint32_t *, int16_t);
int32_t *t(int16_t);
int64_t af(int16_t, struct a, uint16_t, int8_t);
int32_t u() {
  struct a ag;
  ab(f, ag);
  return 0;
}
const int32_t *ab(int32_t *, struct a) {
  ad(e);
  return &amp;r;
}
uint8_t ad(struct a ah) {
  int8_t ae[3];
  int i, j, k;
  for (i = 0; i &lt; 3; i++)
    ae[i] = 1;
  for (;;) {
    uint32_t *v = &amp;p;
    if (ah.z) {
      uint32_t *aj[2][4][4];
      for (i = j = k = 0; k &lt; 4; k++)
        aj[i][j][k] = 0;
      for (ah.b = 0;;) {
        int32_t ak;
        if (ae[ah.d]) {
          ac(t(af(0, ah, ah.b, e.z)), ah, aj[0][0][0], v, ah.c);
          return s;
        }
      }
    }
  }
}
int32_t *ac(int32_t *al, struct a y, uint32_t *, uint32_t *, int16_t ai) {
  if (1 &gt;= (aa[0] = y, ai))
    return al;
}
int32_t *t(int16_t) { return &amp;q; }
int64_t af(int16_t, struct a am, uint16_t, int8_t) {
  int32_t w = 7;
  am = e;
  for (h = 0; h &lt;= 2; h++)
    for (o = 0; o &lt;= 2; o++) {
      int32_t *an = &amp;w;
      int64_t *ao = &amp;l;
      uint8_t *x = &amp;g[1][2][3];
      w ^= g[2][1][3];
      if (safe_mul_func_int8_t_s_s(
              am.c, m == (*x ^= 1 &lt;= safe_mul_func_int8_t_s_s(
                                         safe_div_func_uint64_t_u_u(*ao, am.z),
                                         *an))))
        ++n[2][0];
    }
  return am.z;
}
int main() {
  int i;
  u();
  i = 0;
  printf("%d\n", aa[i].b);
}
```
```
&gt; clang -O0 test.c &amp;&amp; ./a.out
0
&gt; clang -O3 test.c &amp;&amp; ./a.out
5
```
```
%struct.a = type { i32, i64, i32, i32 }
@<!-- -->e = dso_local global { i32, [4 x i8], i64, i32, i32 } { i32 5, [4 x i8] zeroinitializer, i64 0, i32 1, i32 90986701 }, align 8
define dso_local zeroext i8 @<!-- -->ad(ptr noundef byval(%struct.a) align 8 %ah)
%call = call zeroext i8 @<!-- -->ad(ptr noundef byval(%struct.a) align 8 @<!-- -->e)
```
We should also emit the padding for `%struct.a`.
llvm version: d21fc58aeeaa7f0369a24dbe70a0360e0edbf76f
</details>


---

### Comment 2 - nikic

> We should also emit the padding for %struct.a.

Without commenting on whether we should or shouldn't, why is not having the padding a miscompile? As this is not a packed struct, for a i64:64 target, LLVM will implicitly insert the padding and it should have the correct size.

---

### Comment 3 - dtcxzyw

>  As this is not a packed struct, for a i64:64 target, LLVM will implicitly insert the padding and it should have the correct size.

Sorry. It is a DSE bug.


---

### Comment 4 - dtcxzyw

cc @haopliu 

---

### Comment 5 - nikic

@dtcxzyw I think your new IR is over-reduced. You have `initializes((0, 24)` on `@af`, so I think eliminating the store is correct.

---

### Comment 6 - dtcxzyw

> [@dtcxzyw](https://github.com/dtcxzyw) I think your new IR is over-reduced. You have `initializes((0, 24)` on `@af`, so I think eliminating the store is correct.

Fixed.

---

### Comment 7 - nikic

There are two ways we can fix this, depending on what we want the semantics to do:

 1. Don't infer initializes on byval arguments. initializes on byval refers to the original caller memory (or having both attributes is made a verifier error).
 2. Infer initializes on byval, but don't use it in DSE. initializes on byval refers to the callee copy. This matches the semantics of readonly on byval.

I think variant 2 is marginally more powerful. For example, we could do a backend optimization where byval + initializes will allocate the full size of byval on the stack but not copy over the parts covered by initializes.

---

