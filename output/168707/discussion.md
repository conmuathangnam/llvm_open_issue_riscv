# Incorrect handling of &* in constexpr in C23

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/168707

## Body

Consider:
```
void f() {
  constexpr int *p = 0;
  constexpr int *q1 = &*p;
}
```
this is invalid in C++, but should be valid in C per 6.5.3.2p3:
> The unary & operator yields the address of its operand. If the operand has type "type", the result has
type "pointer to type". If the operand is the result of a unary * operator, neither that operator nor
the & operator is evaluated and the result is as if both were omitted, except that the constraints on
the operators still apply and the result is not an lvalue. Similarly, if the operand is the result of a []
operator, neither the & operator nor the unary * that is implied by the [] is evaluated and the result
is as if the & operator were removed and the [] operator were changed to a + operator.

Clang rejects, GCC accepts: https://godbolt.org/z/oW14TEa9P

This is a recent regression, Clang 21 accepts but trunk rejects,

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
Consider:
```
void f() {
  constexpr int *p = 0;
  constexpr int *q1 = &amp;*p;
}
```
this is invalid in C++, but should be valid in C per 6.5.3.2p3:
&gt; The unary &amp; operator yields the address of its operand. If the operand has type "type", the result has
type "pointer to type". If the operand is the result of a unary * operator, neither that operator nor
the &amp; operator is evaluated and the result is as if both were omitted, except that the constraints on
the operators still apply and the result is not an lvalue. Similarly, if the operand is the result of a []
operator, neither the &amp; operator nor the unary * that is implied by the [] is evaluated and the result
is as if the &amp; operator were removed and the [] operator were changed to a + operator.

Clang rejects, GCC accepts: https://godbolt.org/z/oW14TEa9P

This is a recent regression, Clang 21 accepts but trunk rejects,
</details>


---

### Comment 2 - AaronBallman

I looked into this a little bit and the issue happens in both the old constexpr engine and the new interpreter. I poked at it a bit and I think the old engine can be fixed with:
```diff
diff --git a/clang/lib/AST/ExprConstant.cpp b/clang/lib/AST/ExprConstant.cpp
index 11c5e1c6e90f..b1d3b4a04b6d 100644
--- a/clang/lib/AST/ExprConstant.cpp
+++ b/clang/lib/AST/ExprConstant.cpp
@@ -10094,6 +10094,19 @@ bool PointerExprEvaluator::VisitBinaryOperator(const BinaryOperator *E) {
 }
 
 bool PointerExprEvaluator::VisitUnaryAddrOf(const UnaryOperator *E) {
+  // C23 6.5.4.2p3:
+  // ... If the operand is the result of a unary * operator, neither that
+  // operator nor the & operator is evaluated and the result is as if both were
+  // omitted, except that the constraints on the operators still apply and the
+  // result is not an lvalue.
+  //
+  // This same exception applies in all versions of C, not just C23.
+  if (!Info.getLangOpts().CPlusPlus) {
+    if (const auto *SubE = dyn_cast<UnaryOperator>(E->getSubExpr());
+        SubE && SubE->getOpcode() == UO_Deref) {
+      return evaluatePointer(SubE->getSubExpr(), Result);
+    }
+  }
   return evaluateLValue(E->getSubExpr(), Result);
 }
 
@@ -10158,7 +10171,7 @@ bool PointerExprEvaluator::VisitCastExpr(const CastExpr *E) {
       if (VoidPtrCastMaybeOK &&
           (Info.getStdAllocatorCaller("allocate") ||
            IsDeclSourceLocationCurrent(Info.CurrentCall->Callee) ||
-           Info.getLangOpts().CPlusPlus26)) {
+           Info.getLangOpts().CPlusPlus26 || Info.getLangOpts().C23)) {
         // Permitted.
       } else {
         if (SubExpr->getType()->isVoidPointerType() &&
```
but I wasn't able to figure out the new interpreter changes before running out of time. Some good test cases for this are:
```c
void f() {
  constexpr int *p = 0;
  static_assert(p == nullptr);
  constexpr int *q1 = &*p;
  static_assert(q1 == nullptr);
  constexpr int *q2 = &*(void *)nullptr;
  static_assert(q2 == nullptr);
  constexpr int *q3 = &*(int *)nullptr;
  static_assert(q3 == nullptr);

  // These are both expected to fail. C23 6.7.2p6:
  //   If an object or subobject declared with storage-class specifier constexpr
  //   has pointer, integer, or arithmetic type, any explicit initializer value
  //   for it shall be null,126) an integer constant expression, or an arithmetic
  //   constant expression, respectively.
  // Because this is a pointer, it has to be null, even though a static variable
  // is permitted in an address constant expression.
  static int i = 12;
  constexpr int *ip = &i;
  constexpr int *ip2 = &*i;
  
  // This is also expected to fail; nullptr has type nullptr_t and that is not
  // a pointer type, so it cannot be dereferenced. That's why q2 above uses a
  // cast to void * despite that being an implicit conversion for nullptr.
  constexpr void *np = &*nullptr;
}
```

CC @tbaederr @Fznamznon it would be awesome if either of you could help get this one across the finish line given it's a Clang 21 regression (in case I can't get to it before going out for vacation).

---

### Comment 3 - tbaederr

@AaronBallman Are there no regressions/test changes needed from that patch?

---

### Comment 4 - AaronBallman

> [@AaronBallman](https://github.com/AaronBallman) Are there no regressions/test changes needed from that patch?

Yeah, there are (listed after the diff; I was testing behavior manually and hadn't gotten around to converting the local tests into lit tests).

---

### Comment 5 - Fznamznon

I think what @tbaederr probably meant is "Is there test failures in check-clang after the listed diff is applied?" (correct me if I'm wrong).

---

### Comment 6 - AaronBallman

> I think what [@tbaederr](https://github.com/tbaederr) probably meant is "Is there test failures in check-clang after the listed diff is applied?" (correct me if I'm wrong).

Ah, that I don't know. I didn't run the lit tests when puttering around with the code in the diff. It's possible that's the wrong fix, too!

---

### Comment 7 - tbaederr

I need these changes for the test cases from above:
```diff
diff --git i/clang/lib/AST/ByteCode/Compiler.cpp w/clang/lib/AST/ByteCode/Compiler.cpp
index 14b6bb7bf61d..9b8f28173f33 100644
--- i/clang/lib/AST/ByteCode/Compiler.cpp
+++ w/clang/lib/AST/ByteCode/Compiler.cpp
@@ -6771,7 +6771,7 @@ bool Compiler<Emitter>::VisitUnaryOperator(const UnaryOperator *E) {
     if (!this->visit(SubExpr))
       return false;

-    if (!SubExpr->getType()->isFunctionPointerType() && !this->emitCheckNull(E))
+    if (!SubExpr->getType()->isFunctionPointerType() && !Ctx.getLangOpts().C23 && !this->emitCheckNull(E))
       return false;

     if (classifyPrim(SubExpr) == PT_Ptr)
diff --git i/clang/lib/AST/ByteCode/Interp.h w/clang/lib/AST/ByteCode/Interp.h
index d8b8b209fa92..62084807a9d1 100644
--- i/clang/lib/AST/ByteCode/Interp.h
+++ w/clang/lib/AST/ByteCode/Interp.h
@@ -2742,7 +2742,7 @@ static inline bool PtrPtrCast(InterpState &S, CodePtr OpPC, bool SrcIsVoidPtr) {
           << diag::ConstexprInvalidCastKind::CastFrom << "'void *'"
           << S.Current->getRange(OpPC);
     }
-  } else {
+  } else if (!S.getLangOpts().C23) {
     const SourceInfo &E = S.Current->getSource(OpPC);
     S.CCEDiag(E, diag::note_constexpr_invalid_cast)
         << diag::ConstexprInvalidCastKind::ThisConversionOrReinterpret
```

---

### Comment 8 - zygoloid

I wonder if it'd be worth using a different AST representation for an `&` operator that the language rules specify is rewritten to some other expression, perhaps optionally tracking an "equivalent expression" in trailing storage? That might make it easier to get this consistently right across the various places that need to handle it (at least, both constant evaluators and codegen, and probably also some stuff in `SemaChecking` and elsewhere).

---

### Comment 9 - zygoloid

> I looked into this a little bit and the issue happens in both the old constexpr engine and the new interpreter. I poked at it a bit and I think the old engine can be fixed with:
[...]
> ```diff
> +    if (const auto *SubE = dyn_cast<UnaryOperator>(E->getSubExpr());
> +        SubE && SubE->getOpcode() == UO_Deref) {
> +      return evaluatePointer(SubE->getSubExpr(), Result);
> +    }
> ```

FWIW, I think this isn't sufficient -- I think we may also need to handle `&a[b]`, which is notionally rewritten to `&*(a + b)` and then to `a + b`.

---

### Comment 10 - AaronBallman

> I wonder if it'd be worth using a different AST representation for an `&` operator that the language rules specify is rewritten to some other expression, perhaps optionally tracking an "equivalent expression" in trailing storage? That might make it easier to get this consistently right across the various places that need to handle it (at least, both constant evaluators and codegen, and probably also some stuff in `SemaChecking` and elsewhere).

We currently have `CXXRewrittenBinaryOperator`, perhaps we want `RewrittenOperator` as a base class and then `RewrittenUnaryOperator` for `&*` in C and `RewrittenBinaryOperator` for binary operators in C++? I can't tell if I'm shoehorning though -- with the binary case, it's taking one binary form and converting into another but in the unary case it's taking two unary operators and rewriting into a noop.

---

