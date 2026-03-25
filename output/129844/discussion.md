# Miscompile with C++23 constexpr-unknown constant evaluation

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/129844
**Status:** Closed
**Labels:** clang:codegen, miscompilation, c++23, constexpr
**Closed Date:** 2025-03-10T01:38:56Z

## Body

Testcase:

```
int &ff();
int &x = ff();
struct A { int& x; };
struct B { A x[20]; };
B f() { return {x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x}; }
```

Generates the following, which is wrong because the references point to x itself, not the referenced lvalue.

```
[...]
@constinit = private constant [20 x %struct.A] [%struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }, %struct.A { ptr @x }], align 8
[...]
define dso_local void @_Z1fv(ptr dead_on_unwind noalias writable sret(%struct.B) align 8 %agg.result) #2 {
entry:
  %x = getelementptr inbounds nuw %struct.B, ptr %agg.result, i32 0, i32 0
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %x, ptr align 8 @constinit, i64 160, i1 false)
  ret void
}
```

This is synthetic; I have no idea how likely it is for anyone to actually run into this.  Filing as a followup to discussion on #128409.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Eli Friedman (efriedma-quic)

<details>
Testcase:

```
int &amp;ff();
int &amp;x = ff();
struct A { int&amp; x; };
struct B { A x[20]; };
B f() { return {x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x}; }
```

This is synthetic; I have no idea how likely it is for anyone to actually run into this.  Filing as a followup to discussion on #<!-- -->128409.
</details>


---

### Comment 2 - tbaederr

This is also accepted by gcc and clang 19, why should it be rejected?

---

### Comment 3 - efriedma-quic

Updated description to be more clear.

---

