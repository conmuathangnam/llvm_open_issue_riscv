# Accepts-invalid with dependent name not prefixed with typename

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/17283

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [16909](https://llvm.org/bz16909) |
| Version | unspecified |
| OS | Windows NT |
| CC | @hyp,@benlangmuir,@DougGregor |

## Extended Description 
Testcase:
```cpp
template <typename,typename>
struct base {
  template <typename> struct derived;
};
template <typename T, typename U, typename V> base<T, U>::derived<V> foo();
```
We should print an error about the missing `typename` and template keywords.

## Comments

### Comment 1 - llvmbot

This is also tracked as rdar://problem/19438432.

Ben proposed a patch:
http://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20150126/122055.html

---

### Comment 2 - benlangmuir

This can also cause assertion failures or generate invalid IR:
```cpp
template<typename T> struct A {
  template<typename U> struct B {};
};
struct C {};
template<typename T> A<T>::B<T> begin(const T &);

void test() {
  auto __begin = begin(5); // asserts because we didn't deduce the type of __begin
  for (auto &&x : C()); // without assertions, will produce invalid LLVM IR
}
```

---

### Comment 3 - Endilll

Diagnostics is emitted since Clang 5: https://godbolt.org/z/1K786Tan4
Crash doesn't reproduce with assertion builds of Clang 3.5 and 3.6: https://godbolt.org/z/dErrdM4hd

---

### Comment 4 - efriedma-quic

The following still crashes:

```cpp
template<typename T> struct A {
  template<typename U> struct B {};
};
struct C {};
template<typename T> A<T>::B<T> begin(const T &);

void test() {
  auto __begin = begin(5); // asserts because we didn't deduce the type of __begin
}
```

---

### Comment 5 - Endilll

Crash appears to be fixed in Clang 5: https://godbolt.org/z/bbsvE68MT
But it's back since Clang 16: https://godbolt.org/z/hMMofvGM7

---

### Comment 6 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 7 - efriedma-quic

I think what happened is that we added a check to reject this... but when we implemented the C++20 typename rules in https://reviews.llvm.org/D53847, the check was relaxed, so this issue showed up again.

CC @alanzhao1 @cor3ntin @erichkeane 

---

### Comment 8 - cor3ntin

@sdkrystian Could you look at this one?

---

### Comment 9 - sdkrystian

@cor3ntin So, the problem here is that we parse & annotate the _nested-name-specifier_ `A<T>::` and the subsequent _template-id_ `B<T>` as if they were they were the _declarator-id_ of a constructor declaration. I actually have a patch which fixes another embodiment of this bug (#96364), so I can apply what I've done there to fix this issue as well.

---

### Comment 10 - AaronBallman

> [@cor3ntin](https://github.com/cor3ntin) So, the problem here is that we parse & annotate the _nested-name-specifier_ `A<T>::` and the subsequent _template-id_ `B<T>` as if they were they were the _declarator-id_ of a constructor declaration. I actually have a patch which fixes another embodiment of this bug ([#96364](https://github.com/llvm/llvm-project/pull/96364)), so I can apply what I've done there to fix this issue as well.

FWIW, this issue still happens on trunk: https://godbolt.org/z/6jhGacv5n CC @sdkrystian 

---

### Comment 11 - efriedma-quic

I tried the "obvious" thing, following the lead of #96364, but it doesn't really work... I mean, it kind of works, but there's a big issue: if we parse twice, that means we also generate all the diagnostics twice.  Which is not what we want.

```diff
diff --git a/clang/lib/Parse/ParseDecl.cpp b/clang/lib/Parse/ParseDecl.cpp
index 9ca3e2b..61019a9 100644
--- a/clang/lib/Parse/ParseDecl.cpp
+++ b/clang/lib/Parse/ParseDecl.cpp
@@ -4035,27 +4035,45 @@ void Parser::ParseDeclarationSpecifiers(
         }
       }

-      // In C++, check to see if this is a scope specifier like foo::bar::, if
-      // so handle it as such.  This is important for ctor parsing.
-      if (getLangOpts().CPlusPlus) {
-        // C++20 [temp.spec] 13.9/6.
-        // This disables the access checking rules for function template
-        // explicit instantiation and explicit specialization:
-        // - `return type`.
-        SuppressAccessChecks SAC(*this, IsTemplateSpecOrInst);
-
-        const bool Success = TryAnnotateCXXScopeToken(EnteringContext);
-
-        if (IsTemplateSpecOrInst)
-          SAC.done();
-
-        if (Success) {
-          if (IsTemplateSpecOrInst)
-            SAC.redelay();
+      // In C++, this could be a scope specifier followed by an identifier
+      // naming a constructor.  But we need to be careful because it could
+      // also just be a dependent type.
+      if (getLangOpts().CPlusPlus && (DSContext == DeclSpecContext::DSC_top_level ||
+           DSContext == DeclSpecContext::DSC_class)) {
+        TentativeParsingAction TPA(*this, /*Unannotated=*/true);
+        CXXScopeSpec SS;
+
+        if (ParseOptionalCXXScopeSpecifier(SS, /*ObjectType=*/nullptr,
+                                           /*ObjectHasErrors=*/false,
+                                           /*EnteringContext*/true,
+                                           /*MayBePseudoDestructor=*/nullptr,
+                                           /*IsTypename=*/false, /*LastII=*/nullptr,
+                                           /*OnlyNamespace=*/false,
+                                           /*InUsingDeclaration=*/false,
+                                           /*Disambiguation=*/true)) {
           DS.SetTypeSpecError();
+          TPA.Commit();
           goto DoneWithDeclSpec;
         }

+        if (!SS.isEmpty()) {
+          AnnotateScopeToken(SS, true);
+          if (NextToken().is(tok::identifier) &&
+              Actions.isCurrentClassName(*NextToken().getIdentifierInfo(), getCurScope(),
+                                       &SS) &&
+            isConstructorDeclarator(/*Unqualified=*/false,
+                                    /*DeductionGuide=*/false,
+                                    DS.isFriendSpecified(),
+                                    &TemplateInfo)) {
+            TPA.Commit();
+            goto DoneWithDeclSpec;
+          }
+        }
+        TPA.Revert();
+      }
+
+      if (getLangOpts().CPlusPlus) {
+        TryAnnotateCXXScopeToken(/*EnteringContext*/false);
         if (!Tok.is(tok::identifier))
           continue;
       }
```

---

### Comment 12 - efriedma-quic

Related testcases:

```cpp
template<typename T> class A { class B; };
template<typename T> class A<T>::B::C f();
template<typename T> void f(enum A<T>::B::C);
```

---

