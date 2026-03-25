# OOM for clang++-19 and clang++-20

**Author:** FedericoSerafini
**URL:** https://github.com/llvm/llvm-project/issues/136840

## Body

During the compilation of the following code,
both clang++-19 and clang++-20 allocate memory until it runs out:

```
extern struct c x;

struct c {
  c &a = (*new c, x);
  c() {}
} x;

```


## Comments

### Comment 1 - SchrodingerZhu

stack traces
```c++
    frame #136345: 0x00007ffff5914983 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] Visit at SemaExpr.cpp:19880:16
    frame #136346: 0x00007ffff591497e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] MarkDeclarationsReferencedInExpr at SemaExpr.cpp:19912:58
    frame #136347: 0x00007ffff5914952 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at SemaExpr.cpp:18053:11
    frame #136348: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] callback_fn<(lambda at /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18052:64)> at STLFunctionalExtras.h:45:12
    frame #136349: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at STLFunctionalExtras.h:68:12
    frame #136350: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Stack.h:46:7
    frame #136351: 0x00007ffff5914920 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Sema.cpp:568:3
    frame #136352: 0x00007ffff591490b libclang-cpp.so.19.1`::MarkFunctionReferenced() at SemaExpr.cpp:18052:9
    frame #136353: 0x00007ffff685654b libclang-cpp.so.19.1`::VisitCXXConstructExpr() [inlined] visitUsedDecl at SemaExpr.cpp:19874:7
    frame #136354: 0x00007ffff685653b libclang-cpp.so.19.1`::VisitCXXConstructExpr() at UsedDeclVisitor.h:84:14
    frame #136355: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136356: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136357: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136358: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136359: 0x00007ffff5a0568c libclang-cpp.so.19.1`::Visit() [inlined] Visit at SemaExpr.cpp:19880:16
    frame #136360: 0x00007ffff5a05642 libclang-cpp.so.19.1`::Visit() [inlined] VisitCXXDefaultInitExpr at UsedDeclVisitor.h:99:14
    frame #136361: 0x00007ffff5a05634 libclang-cpp.so.19.1`::Visit() at StmtNodes.inc:736:1
    frame #136362: 0x00007ffff5914983 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] Visit at SemaExpr.cpp:19880:16
    frame #136363: 0x00007ffff591497e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] MarkDeclarationsReferencedInExpr at SemaExpr.cpp:19912:58
    frame #136364: 0x00007ffff5914952 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at SemaExpr.cpp:18053:11
    frame #136365: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] callback_fn<(lambda at /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18052:64)> at STLFunctionalExtras.h:45:12
    frame #136366: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at STLFunctionalExtras.h:68:12
    frame #136367: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Stack.h:46:7
    frame #136368: 0x00007ffff5914920 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Sema.cpp:568:3
    frame #136369: 0x00007ffff591490b libclang-cpp.so.19.1`::MarkFunctionReferenced() at SemaExpr.cpp:18052:9
    frame #136370: 0x00007ffff685654b libclang-cpp.so.19.1`::VisitCXXConstructExpr() [inlined] visitUsedDecl at SemaExpr.cpp:19874:7
    frame #136371: 0x00007ffff685653b libclang-cpp.so.19.1`::VisitCXXConstructExpr() at UsedDeclVisitor.h:84:14
    frame #136372: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136373: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136374: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136375: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
    frame #136376: 0x00007ffff5a0568c libclang-cpp.so.19.1`::Visit() [inlined] Visit at SemaExpr.cpp:19880:16
    frame #136377: 0x00007ffff5a05642 libclang-cpp.so.19.1`::Visit() [inlined] VisitCXXDefaultInitExpr at UsedDeclVisitor.h:99:14
    frame #136378: 0x00007ffff5a05634 libclang-cpp.so.19.1`::Visit() at StmtNodes.inc:736:1
    frame #136379: 0x00007ffff5914983 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] Visit at SemaExpr.cpp:19880:16
    frame #136380: 0x00007ffff591497e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] MarkDeclarationsReferencedInExpr at SemaExpr.cpp:19912:58
    frame #136381: 0x00007ffff5914952 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at SemaExpr.cpp:18053:11
    frame #136382: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] callback_fn<(lambda at /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18052:64)> at STLFunctionalExtras.h:45:12
    frame #136383: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] operator() at STLFunctionalExtras.h:68:12
    frame #136384: 0x00007ffff591494e libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Stack.h:46:7
    frame #136385: 0x00007ffff5914920 libclang-cpp.so.19.1`::MarkFunctionReferenced() [inlined] runWithSufficientStackSpace at Sema.cpp:568:3
    frame #136386: 0x00007ffff591490b libclang-cpp.so.19.1`::MarkFunctionReferenced() at SemaExpr.cpp:18052:9
    frame #136387: 0x00007ffff685654b libclang-cpp.so.19.1`::VisitCXXConstructExpr() [inlined] visitUsedDecl at SemaExpr.cpp:19874:7
    frame #136388: 0x00007ffff685653b libclang-cpp.so.19.1`::VisitCXXConstructExpr() at UsedDeclVisitor.h:84:14
    frame #136389: 0x00007ffff5a0551b libclang-cpp.so.19.1`::VisitStmt() at EvaluatedExprVisitor.h:106:15
```

---

### Comment 2 - shafik

Looks like this started after clang-15: https://godbolt.org/z/vo8o67867

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Federico Serafini (FedericoSerafini)

<details>
During the compilation of the following code,
both clang++-19 and clang++-20 allocate memory until it runs out:

```
extern struct c x;

struct c {
  c &amp;a = (*new c, x);
  c() {}
} x;

```

</details>


---

### Comment 4 - shafik

@cor3ntin git bisect points to ca619613801233ef2def8c3cc7d311d5ed0033cb as the cause

---

### Comment 5 - shafik

ping @cor3ntin 

---

