# Clang ICE SIGSEV in invalid code only in trunk

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/140887
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-05-22T10:47:35Z

## Body

I have checked in my local clang, that was downloaded from main branch a few weeks ago and there it does not SIGSEV, so it must be a recent modification that have arised this issue.

## Stack dump

Since in my local clang-21 it does not SIGSEV i have not been able to get it, but as it happened in other cases it must be a recursion check.

## Program

```
#include <stdlib.h>
#include <stddef.h>
int main()
{
        struct A {
                struct A(){};

        };
        struct A a;

}
```

## To quickly reproduce

https://godbolt.org/z/TE95s44Wr

## Comments

### Comment 1 - mariete1223

With a more extended example it arises in clang-19 using -std=c2x flag, i upload it just in case it helps:

```
#include <stdlib.h>
#include <stddef.h>
struct A {
        constexpr A(bool cond = true) { if (cond) std::cout << "cond"; };
        constexpr ~A() {};
};
constexpr A a;
```

and its ICE is a recursive check of:

```
#238 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#239 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#240 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#241 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#242 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#243 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#244 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#245 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#246 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#247 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#248 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#249 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#250 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#251 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#252 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#253 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#254 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#255 0x0000562a1879452f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
```

But in above case there is no constexpr so i think it must be something different.

---

### Comment 2 - AaronBallman

I think I inadvertently introduced that issue with c555c8d554f0875967861f6bb7da9159444d1860; am investigating further.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
I have checked in my local clang, that was downloaded from main branch a few weeks ago and there it does not SIGSEV, so it must be a recent modification that have arised this issue.

## Stack dump

Since in my local clang-21 it does not SIGSEV i have not been able to get it, but as it happened in other cases it must be a recursion check.

## Program

```
#include &lt;stdlib.h&gt;
#include &lt;stddef.h&gt;
int main()
{
        struct A {
                struct A(){};

        };
        struct A a;

}
```

## To quickly reproduce

https://godbolt.org/z/TE95s44Wr
</details>


---

### Comment 4 - AaronBallman

Ah, they're related-but-different issues. In both cases, there's a missing check for an invalid decl to prevent us from hitting recursion (we're treating the field as though it has the same type as the record itself). A more simple reproducer for the first issue is:
```
struct S {
  struct S s;
};

void func() {
  struct S s;
}
```

---

### Comment 5 - AaronBallman

A simpler reproducer for the second issue is:
```
struct A {
  A();
};
constexpr A a;
```

---

