# ICE: One more "dependent lookup context that isn't the current instantiation" assert

**Author:** snarkmaster
**URL:** https://github.com/llvm/llvm-project/issues/182975

## Body

This looks different from #175296 (open, but different ingredients) and #95778 (fixed in 2024). I hit it while writing real code.

It's live on trunk, and was introduced after 19: https://godbolt.org/z/8rhYTx3xc

## Essential ingredients

1. Class template with an **anonymous union** member
2. Constructor with a **`requires` clause** that accesses the union member via a parameter of the class's own type (e.g. `other.value`)
3. A **deduction guide** for the class template

## Minimal repro & error

```c++
template <typename T>
struct S {
    union {
        T value;
    };
    S(S&& other)
    requires requires { other.value; } = default;
};
S(int) -> S<int>;
```

```
clang/lib/Sema/SemaExprMember.cpp:1007:
Assertion `(SS.isEmpty() ? !BaseType->isDependentType() || computeDeclContext(BaseType)
           : !isDependentScopeSpecifier(SS) || computeDeclContext(SS))
           && "dependent lookup context that isn't the current instantiation?"' failed.
```

## Workaround

I used a `declval` dummy in the `requires` clause instead. This is less legible and likely costlier to build?

## Fix (against 20) due to @lanza 

The author unfortunately doesn't have time to put this up as a PR, but this was reviewed by @bcardosolopes as reasonable:

```patch
diff --git a/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp b/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
--- a/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
+++ b/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
@@ -459,16 +459,21 @@
     // requires-clause, if any.
     Expr *FunctionTrailingRC = nullptr;
     if (Expr *RC = CD->getTrailingRequiresClause()) {
-      MultiLevelTemplateArgumentList Args;
-      Args.setKind(TemplateSubstitutionKind::Rewrite);
-      Args.addOuterTemplateArguments(Depth1Args);
-      Args.addOuterRetainedLevel();
-      if (NestedPattern)
-        Args.addOuterRetainedLevels(NestedPattern->getTemplateDepth());
-      ExprResult E = SemaRef.SubstConstraintExprWithoutSatisfaction(RC, Args);
-      if (!E.isUsable())
-        return nullptr;
-      FunctionTrailingRC = E.get();
+      if (FTD) {
+        MultiLevelTemplateArgumentList Args;
+        Args.setKind(TemplateSubstitutionKind::Rewrite);
+        Args.addOuterTemplateArguments(Depth1Args);
+        Args.addOuterRetainedLevel();
+        if (NestedPattern)
+          Args.addOuterRetainedLevels(NestedPattern->getTemplateDepth());
+        ExprResult E =
+            SemaRef.SubstConstraintExprWithoutSatisfaction(RC, Args);
+        if (!E.isUsable())
+          return nullptr;
+        FunctionTrailingRC = E.get();
+      } else {
+        FunctionTrailingRC = RC;
+      }
     }

     // C++ [over.match.class.deduct]p1:
diff --git a/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp b/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp
new file mode 100644
--- /dev/null
+++ b/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp
@@ -0,0 +1,70 @@
+// RUN: %clang_cc1 -std=c++20 -fsyntax-only -verify %s
+// expected-no-diagnostics
+
+// Test that non-template constructors with trailing requires clauses
+// don't crash during deduction guide synthesis.
+
+// Anonymous union with constrained defaulted move constructor.
+template <typename T>
+struct AnonymousUnionConstrainedDefaultedMove {
+    union {
+        T value;
+    };
+    AnonymousUnionConstrainedDefaultedMove(
+        AnonymousUnionConstrainedDefaultedMove&& other)
+    requires requires { other.value; } = default;
+};
+
+AnonymousUnionConstrainedDefaultedMove(int)
+    -> AnonymousUnionConstrainedDefaultedMove<int>;
+
+// Direct member with constrained constructor.
+template <typename T>
+struct DirectMemberConstrainedCtor {
+    T value;
+    DirectMemberConstrainedCtor(DirectMemberConstrainedCtor&& other)
+    requires requires { other.value; } = default;
+};
+
+DirectMemberConstrainedCtor(int) -> DirectMemberConstrainedCtor<int>;
+
+// Non-defaulted constrained constructor.
+template <typename T>
+struct NonDefaultedConstrainedCtor {
+    T data;
+    NonDefaultedConstrainedCtor(NonDefaultedConstrainedCtor&& other)
+    requires requires { other.data; } : data(static_cast<T&&>(other.data)) {}
+};
+
+NonDefaultedConstrainedCtor(int) -> NonDefaultedConstrainedCtor<int>;
+
+// CTAD triggering deduction guide synthesis.
+template <typename T>
+struct CTADTrigger {
+    union {
+        T value;
+    };
+    CTADTrigger(T v) : value(v) {}
+    CTADTrigger(CTADTrigger&& other)
+    requires requires { other.value; } = default;
+};
+
+void testCTAD() {
+    CTADTrigger c(42);
+}
+
+// Ensure template constructors with requires clauses still work (CWG 2628).
+template <typename T>
+struct TemplateCtorWithRequiresStillWorks {
+    T value;
+    template <typename U>
+    requires requires { static_cast<T>(U{}); }
+    TemplateCtorWithRequiresStillWorks(U u) : value(static_cast<T>(u)) {}
+};
+
+TemplateCtorWithRequiresStillWorks(int)
+    -> TemplateCtorWithRequiresStillWorks<int>;
+
+void testTemplateCtor() {
+    TemplateCtorWithRequiresStillWorks t(42);
+}
```




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (snarkmaster)

<details>
This looks different from #<!-- -->175296 (open, but different ingredients) and #<!-- -->95778 (fixed in 2024). I hit it while writing real code.

It's live on trunk, and was introduced after 19: https://godbolt.org/z/8rhYTx3xc

## Essential ingredients

1. Class template with an **anonymous union** member
2. Constructor with a **`requires` clause** that accesses the union member via a parameter of the class's own type (e.g. `other.value`)
3. A **deduction guide** for the class template

## Minimal repro &amp; error

```
template &lt;typename T&gt;
struct S {
    union {
        T value;
    };
    S(S&amp;&amp; other)
    requires requires { other.value; } = default;
};
S(int) -&gt; S&lt;int&gt;;
```

```
clang/lib/Sema/SemaExprMember.cpp:1007:
Assertion `(SS.isEmpty() ? !BaseType-&gt;isDependentType() || computeDeclContext(BaseType)
           : !isDependentScopeSpecifier(SS) || computeDeclContext(SS))
           &amp;&amp; "dependent lookup context that isn't the current instantiation?"' failed.
```

## Workaround

I used a `declval` dummy in the `requires` clause instead. This is less legible and likely costlier to build?

## Fix (against 20) due to @<!-- -->lanza 

The author unfortunately doesn't have time to put this up as a PR, but this was reviewed by @<!-- -->bcardosolopes as reasonable:

```patch
diff --git a/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp b/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
--- a/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
+++ b/20/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp
@@ -459,16 +459,21 @@
     // requires-clause, if any.
     Expr *FunctionTrailingRC = nullptr;
     if (Expr *RC = CD-&gt;getTrailingRequiresClause()) {
-      MultiLevelTemplateArgumentList Args;
-      Args.setKind(TemplateSubstitutionKind::Rewrite);
-      Args.addOuterTemplateArguments(Depth1Args);
-      Args.addOuterRetainedLevel();
-      if (NestedPattern)
-        Args.addOuterRetainedLevels(NestedPattern-&gt;getTemplateDepth());
-      ExprResult E = SemaRef.SubstConstraintExprWithoutSatisfaction(RC, Args);
-      if (!E.isUsable())
-        return nullptr;
-      FunctionTrailingRC = E.get();
+      if (FTD) {
+        MultiLevelTemplateArgumentList Args;
+        Args.setKind(TemplateSubstitutionKind::Rewrite);
+        Args.addOuterTemplateArguments(Depth1Args);
+        Args.addOuterRetainedLevel();
+        if (NestedPattern)
+          Args.addOuterRetainedLevels(NestedPattern-&gt;getTemplateDepth());
+        ExprResult E =
+            SemaRef.SubstConstraintExprWithoutSatisfaction(RC, Args);
+        if (!E.isUsable())
+          return nullptr;
+        FunctionTrailingRC = E.get();
+      } else {
+        FunctionTrailingRC = RC;
+      }
     }

     // C++ [over.match.class.deduct]p1:
diff --git a/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp b/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp
new file mode 100644
--- /dev/null
+++ b/20/llvm-project/clang/test/SemaTemplate/constrained-deduction-guide-crash.cpp
@@ -0,0 +1,70 @@
+// RUN: %clang_cc1 -std=c++20 -fsyntax-only -verify %s
+// expected-no-diagnostics
+
+// Test that non-template constructors with trailing requires clauses
+// don't crash during deduction guide synthesis.
+
+// Anonymous union with constrained defaulted move constructor.
+template &lt;typename T&gt;
+struct AnonymousUnionConstrainedDefaultedMove {
+    union {
+        T value;
+    };
+    AnonymousUnionConstrainedDefaultedMove(
+        AnonymousUnionConstrainedDefaultedMove&amp;&amp; other)
+    requires requires { other.value; } = default;
+};
+
+AnonymousUnionConstrainedDefaultedMove(int)
+    -&gt; AnonymousUnionConstrainedDefaultedMove&lt;int&gt;;
+
+// Direct member with constrained constructor.
+template &lt;typename T&gt;
+struct DirectMemberConstrainedCtor {
+    T value;
+    DirectMemberConstrainedCtor(DirectMemberConstrainedCtor&amp;&amp; other)
+    requires requires { other.value; } = default;
+};
+
+DirectMemberConstrainedCtor(int) -&gt; DirectMemberConstrainedCtor&lt;int&gt;;
+
+// Non-defaulted constrained constructor.
+template &lt;typename T&gt;
+struct NonDefaultedConstrainedCtor {
+    T data;
+    NonDefaultedConstrainedCtor(NonDefaultedConstrainedCtor&amp;&amp; other)
+    requires requires { other.data; } : data(static_cast&lt;T&amp;&amp;&gt;(other.data)) {}
+};
+
+NonDefaultedConstrainedCtor(int) -&gt; NonDefaultedConstrainedCtor&lt;int&gt;;
+
+// CTAD triggering deduction guide synthesis.
+template &lt;typename T&gt;
+struct CTADTrigger {
+    union {
+        T value;
+    };
+    CTADTrigger(T v) : value(v) {}
+    CTADTrigger(CTADTrigger&amp;&amp; other)
+    requires requires { other.value; } = default;
+};
+
+void testCTAD() {
+    CTADTrigger c(42);
+}
+
+// Ensure template constructors with requires clauses still work (CWG 2628).
+template &lt;typename T&gt;
+struct TemplateCtorWithRequiresStillWorks {
+    T value;
+    template &lt;typename U&gt;
+    requires requires { static_cast&lt;T&gt;(U{}); }
+    TemplateCtorWithRequiresStillWorks(U u) : value(static_cast&lt;T&gt;(u)) {}
+};
+
+TemplateCtorWithRequiresStillWorks(int)
+    -&gt; TemplateCtorWithRequiresStillWorks&lt;int&gt;;
+
+void testTemplateCtor() {
+    TemplateCtorWithRequiresStillWorks t(42);
+}
```



</details>


---

### Comment 2 - shafik

@zyn0217 this bisects to b46fcb9fa32f24660b1b8858d5c4cbdb76ef9d8b

---

