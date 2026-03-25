# [Clang] Missing source location in diagnostic for invalid conversion in function w/ explicit object parameter

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/135522
**Status:** Closed
**Labels:** clang:frontend, regression:20
**Closed Date:** 2025-04-15T16:04:37Z

## Body

Consider (https://godbolt.org/z/aa475YbW6):
```c++
struct X{};

template <typename T = void>
struct A {
    auto Error(this auto&& This) -> T {
        return T();
    }
};

struct B : A<X> {
    bool f();
};

bool B::f() {
    return Error();
}
```

Obviously, `X` isn’t convertible to `bool`, so we print an error, but the error is missing a source location:
```console
error: no viable conversion from returned value of type 'X' to function return type 'bool'
```

If I remove the explicit object parameter, we instead print
```console
<source>:15:12: error: no viable conversion from returned value of type 'X' to function return type 'bool'
   15 |     return Error();
      |            ^~~~~~~
```

This appears to be a regression since the source location *is* present in Clang 19.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
Consider (https://godbolt.org/z/aa475YbW6):
```c++
struct X{};

template &lt;typename T = void&gt;
struct A {
    auto Error(this auto&amp;&amp; This) -&gt; T {
        return T();
    }
};

struct B : A&lt;X&gt; {
    bool f();
};

bool B::f() {
    return Error();
}
```

Obviously, `X` isn’t convertible to `bool`, so we print an error, but the error is missing a source location:
```console
error: no viable conversion from returned value of type 'X' to function return type 'bool'
```

If I remove the explicit object parameter, we instead print
```console
&lt;source&gt;:15:12: error: no viable conversion from returned value of type 'X' to function return type 'bool'
   15 |     return Error();
      |            ^~~~~~~
```

This appears to be a regression since the source location *is* present in Clang 19.
</details>


---

### Comment 2 - tbaederr

Debug hint:
```diff
diff --git i/clang/lib/Sema/SemaOverload.cpp w/clang/lib/Sema/SemaOverload.cpp
index f46ef2c7f5bd..377747195e4d 100644
--- i/clang/lib/Sema/SemaOverload.cpp
+++ w/clang/lib/Sema/SemaOverload.cpp
@@ -15782,6 +15782,9 @@ ExprResult Sema::BuildCallToMemberFunction(Scope *S, Expr *MemExprE,
     TheCall =
         CallExpr::Create(Context, FnExpr.get(), Args, ResultType, VK, RParenLoc,
                          CurFPFeatureOverrides(), Proto->getNumParams());
+    TheCall->dumpColor();
+    assert(!TheCall->getExprLoc().isInvalid());
+
   } else {
     // Convert the object argument (for a non-static member function call).
     ExprResult ObjectArg = PerformImplicitObjectArgumentInitialization(
```

---

### Comment 3 - tbaederr

Actually, this makes it more obvious:
```diff
diff --git i/clang/lib/Sema/SemaOverload.cpp w/clang/lib/Sema/SemaOverload.cpp
index f46ef2c7f5bd..482f4d2231bd 100644
--- i/clang/lib/Sema/SemaOverload.cpp
+++ w/clang/lib/Sema/SemaOverload.cpp
@@ -6543,6 +6543,10 @@ static Expr *GetExplicitObjectExpr(Sema &S, Expr *Obj,
     Obj = UnaryOperator::Create(S.getASTContext(), Obj, UO_Deref, ObjType,
                                 VK_LValue, OK_Ordinary, SourceLocation(),
                                 /*CanOverflow=*/false, FPOptionsOverride());
+
+    assert(!Obj->getExprLoc().isInvalid());
+
+
   }
   return Obj;
 }
```
we're explicitly passing an invalid source location above.

---

### Comment 4 - Ankur-0429

https://github.com/llvm/llvm-project/blob/cbe8f3ad7621e402b050e768f400ff0d19c3aedd/clang/lib/Sema/SemaDecl.cpp#L15191

If I set this conditional to be true, the source location shows up now in Clang 23.

---

### Comment 5 - zyn0217

I reverted these lines and the source location comes back:

https://github.com/llvm/llvm-project/blob/150e7b14f9474bc4d6891faaae4de6b8c5f6c797/clang/lib/AST/Expr.cpp#L1648-L1660

@HighCommander4 

---

### Comment 6 - HighCommander4

Thanks for the ping. Investigating.

---

### Comment 7 - HighCommander4

A more minimal testcase:

```c++
struct A {
  auto Error(this auto) -> A;
  bool f() { return Error(); }
};
```

Analysis: the `CallExpr` for `Error()` is a call to an explicit object member function, and it has a first argument (so we take the branch of `CallExpr::getBeginLoc()` linked to in [this comment](https://github.com/llvm/llvm-project/issues/135522#issuecomment-2800668783)), but that argument is the implicit `*this` expression, so it does not have a valid begin location. In such a case, we should fall through and return the begin location of the callee.

---

### Comment 8 - HighCommander4

Proposed fix: https://github.com/llvm/llvm-project/pull/135757

---

