# Wrong DeclRefExpr for PackIndexingExpr

**Author:** steakhal
**URL:** https://github.com/llvm/llvm-project/issues/182691

## Body

https://godbolt.org/z/M5qYzKrKW
```c++
template <class = void>
int templated_fn() {
    struct S { int a; };
    auto [... members] = S{12};
    return members...[0];
}

void repro() {
    templated_fn();
}
```

In the AST, there is the primary template for the function. That is followed by the `T=void` instantiation. Finally the function named `repro`.

Notice that the `DeclRefExpr` of the `PackIndexingExpr` of the template instantiation has the same `BindingDecl` pointer as is present in the primary template decl.
This seems to be a bug, as otherwise what would be the purpose of the `BindingDecl` inside the template instantiation?

Here is the AST part from the template instantiation:
```
|     |-DeclStmt 0x1d142688 <line:4:5, col:31>
|     | `-DecompositionDecl 0x1d1421d8 <col:5, col:30> col:10 used 'S' cinit
|     |   |-CXXFunctionalCastExpr 0x1d142450 <col:26, col:30> 'S' functional cast to S <NoOp>
|     |   | `-InitListExpr 0x1d1422d0 <col:27, col:30> 'S'
|     |   |   `-IntegerLiteral 0x1d11cd30 <col:28> 'int' 12
|     |   `-BindingDecl 0x1d142188 <col:15> col:15 referenced members '<dependent type>...'
|     |     `-FunctionParmPackExpr 0x1d142610 <col:15> '<dependent type>...' lvalue
|     `-ReturnStmt 0x1d142740 <line:5:5, col:24>
|       `-ImplicitCastExpr 0x1d142728 <col:12, col:24> 'int' <LValueToRValue>
|         `-PackIndexingExpr 0x1d1426f8 <col:12, col:24> 'int' lvalue
|           |-DeclRefExpr 0x1d11ce08 <col:12> '<dependent type>' lvalue Binding 0x1d11cc18 'members' '<dependent type>...' <--- this should be 0x1d142188
|           `-ConstantExpr 0x1d1426d8 <col:23> '__size_t':'unsigned long'
```

Here is the full AST dump from compiler explorer:
```
|-FunctionTemplateDecl 0x1d11c7e0 <<source>:1:1, line:6:1> line:2:5 templated_fn
| |-TemplateTypeParmDecl 0x1d11c5e0 <line:1:11, col:19> col:17 class depth 0 index 0
| | `-TemplateArgument type 'void'
| |   `-BuiltinType 0x1d0c3fe0 'void'
| |-FunctionDecl 0x1d11c730 <line:2:1, line:6:1> line:2:5 templated_fn 'int ()'
| | `-CompoundStmt 0x1d11ceb8 <col:20, line:6:1>
| |   |-DeclStmt 0x1d11cb78 <line:3:5, col:24>
| |   | `-CXXRecordDecl 0x1d11c910 <col:5, col:23> col:12 referenced struct S definition
| |   |   |-DefinitionData aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor
| |   |   | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
| |   |   | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
| |   |   | |-MoveConstructor exists simple trivial needs_implicit
| |   |   | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| |   |   | |-MoveAssignment exists simple trivial needs_implicit
| |   |   | `-Destructor simple irrelevant trivial constexpr needs_implicit
| |   |   |-CXXRecordDecl 0x1d11ca28 <col:5, col:12> col:12 implicit struct S
| |   |   `-FieldDecl 0x1d11caf0 <col:16, col:20> col:20 a 'int'
| |   |-DeclStmt 0x1d11cdf0 <line:4:5, col:31>
| |   | `-DecompositionDecl 0x1d11cc68 <col:5, col:30> col:10 'auto' cinit
| |   |   |-CXXUnresolvedConstructExpr 0x1d11cd98 <col:26, col:30> 'S' 'S' list
| |   |   | `-InitListExpr 0x1d11cd50 <col:27, col:30> 'void'
| |   |   |   `-IntegerLiteral 0x1d11cd30 <col:28> 'int' 12
| |   |   `-BindingDecl 0x1d11cc18 <col:15> col:15 referenced members '<dependent type>...'
| |   `-ReturnStmt 0x1d11cea8 <line:5:5, col:24>
| |     `-PackIndexingExpr 0x1d11ce80 <col:12, col:24> '<dependent type>' lvalue
| |       |-DeclRefExpr 0x1d11ce08 <col:12> '<dependent type>' lvalue Binding 0x1d11cc18 'members' '<dependent type>...'
| |       `-ConstantExpr 0x1d11ce60 <col:23> '__size_t':'unsigned long'
| |         |-value: Int 0
| |         `-ImplicitCastExpr 0x1d11ce48 <col:23> '__size_t':'unsigned long' <IntegralCast>
| |           `-IntegerLiteral 0x1d11ce28 <col:23> 'int' 0
| `-FunctionDecl 0x1d11d0a8 <line:2:1, line:6:1> line:2:5 used templated_fn 'int ()' implicit_instantiation instantiated_from 0x1d11c730
|   |-TemplateArgument type 'void'
|   | `-BuiltinType 0x1d0c3fe0 'void'
|   `-CompoundStmt 0x1d142750 <col:20, line:6:1>
|     |-DeclStmt 0x1d142168 <line:3:5, col:24>
|     | `-CXXRecordDecl 0x1d11d290 <col:5, col:23> col:12 struct S definition instantiated_from 0x1d11c910
|     |   |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor
|     |   | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
|     |   | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
|     |   | |-MoveConstructor exists simple trivial needs_implicit
|     |   | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
|     |   | |-MoveAssignment exists simple trivial needs_implicit
|     |   | `-Destructor simple irrelevant trivial constexpr
|     |   |-CXXRecordDecl 0x1d142028 <col:5, col:12> col:12 implicit struct S
|     |   |-FieldDecl 0x1d1420d8 <col:16, col:20> col:20 referenced a 'int'
|     |   `-CXXDestructorDecl 0x1d142320 <col:12> col:12 implicit referenced constexpr ~S 'void () noexcept' inline default trivial
|     |-DeclStmt 0x1d142688 <line:4:5, col:31>
|     | `-DecompositionDecl 0x1d1421d8 <col:5, col:30> col:10 used 'S' cinit
|     |   |-CXXFunctionalCastExpr 0x1d142450 <col:26, col:30> 'S' functional cast to S <NoOp>
|     |   | `-InitListExpr 0x1d1422d0 <col:27, col:30> 'S'
|     |   |   `-IntegerLiteral 0x1d11cd30 <col:28> 'int' 12
|     |   `-BindingDecl 0x1d142188 <col:15> col:15 referenced members '<dependent type>...'
|     |     `-FunctionParmPackExpr 0x1d142610 <col:15> '<dependent type>...' lvalue
|     `-ReturnStmt 0x1d142740 <line:5:5, col:24>
|       `-ImplicitCastExpr 0x1d142728 <col:12, col:24> 'int' <LValueToRValue>
|         `-PackIndexingExpr 0x1d1426f8 <col:12, col:24> 'int' lvalue
|           |-DeclRefExpr 0x1d11ce08 <col:12> '<dependent type>' lvalue Binding 0x1d11cc18 'members' '<dependent type>...'
|           `-ConstantExpr 0x1d1426d8 <col:23> '__size_t':'unsigned long'
|             |-value: Int 0
|             `-ImplicitCastExpr 0x1d1426c0 <col:23> '__size_t':'unsigned long' <IntegralCast>
|               `-IntegerLiteral 0x1d11ce28 <col:23> 'int' 0
`-FunctionDecl 0x1d11cf58 <line:8:1, line:10:1> line:8:6 repro 'void ()'
  `-CompoundStmt 0x1d11d270 <col:14, line:10:1>
    `-CallExpr 0x1d11d248 <line:9:5, col:18> 'int'
      `-ImplicitCastExpr 0x1d11d230 <col:5> 'int (*)()' <FunctionToPointerDecay>
        `-DeclRefExpr 0x1d11d1d0 <col:5> 'int ()' lvalue Function 0x1d11d0a8 'templated_fn' 'int ()' (FunctionTemplate 0x1d11c7e0 'templated_fn')
```

PS: I've noticed this while implementing structured binding pack indexing for the Clang Static Analyzer.

## Comments

### Comment 1 - steakhal

/cc @cor3ntin

---

### Comment 2 - steakhal

I just realized that it's not about the BindingDecl, but that the immediate child of `PackIndexingExpr` in the instantiation still refers to the `DeclRefExpr` within the primary template.

---

### Comment 3 - steakhal

I think the bug might be inside `TransformPackIndexingExpr` somewhere.

---

### Comment 4 - steakhal

> I think the bug might be inside `TransformPackIndexingExpr` somewhere.

FWIW, I think `Sema::BuildPackIndexingExpr` should end with something like:
```diff
diff --git a/clang/lib/Sema/SemaTemplateVariadic.cpp b/clang/lib/Sema/SemaTemplateVariadic.cpp
@@ -1375,6 +1375,9 @@ ExprResult Sema::BuildPackIndexingExpr(Expr *PackExpression,
     }
   }
 
+  if (FullySubstituted)
+    PackExpression = ExpandedExprs[Index.value()];
+
   return PackIndexingExpr::Create(getASTContext(), EllipsisLoc, RSquareLoc,
                                   PackExpression, IndexExpr, Index,
                                   ExpandedExprs, FullySubstituted);
```

Because the `PackExpression` would still refer to the one in the primary template, but once we have `FullySubstituted` the pattern, we should refer to the correct `ExpandedExprs` instead.

---

### Comment 5 - steakhal

What also looks weird is that in the instantiation, we still have exprs and decls with `<dependent type>`.
For example, the `DeclStmt > DecompositionDecl  > BindingDecl`. I'm not 100% sure, but I don't think anything in this instantiation should be dependent anymore.

---

### Comment 6 - steakhal

/cc @ricejasonf

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Balázs Benics (steakhal)

<details>
https://godbolt.org/z/M5qYzKrKW
```c++
template &lt;class = void&gt;
int templated_fn() {
    struct S { int a; };
    auto [... members] = S{12};
    return members...[0];
}

void repro() {
    templated_fn();
}
```

In the AST, there is the primary template for the function. That is followed by the `T=void` instantiation. Finally the function named `repro`.

Notice that the `DeclRefExpr` of the `PackIndexingExpr` of the template instantiation has the same `BindingDecl` pointer as is present in the primary template decl.
This seems to be a bug, as otherwise what would be the purpose of the `BindingDecl` inside the template instantiation?

Here is the AST part from the template instantiation:
```
|     |-DeclStmt 0x1d142688 &lt;line:4:5, col:31&gt;
|     | `-DecompositionDecl 0x1d1421d8 &lt;col:5, col:30&gt; col:10 used 'S' cinit
|     |   |-CXXFunctionalCastExpr 0x1d142450 &lt;col:26, col:30&gt; 'S' functional cast to S &lt;NoOp&gt;
|     |   | `-InitListExpr 0x1d1422d0 &lt;col:27, col:30&gt; 'S'
|     |   |   `-IntegerLiteral 0x1d11cd30 &lt;col:28&gt; 'int' 12
|     |   `-BindingDecl 0x1d142188 &lt;col:15&gt; col:15 referenced members '&lt;dependent type&gt;...'
|     |     `-FunctionParmPackExpr 0x1d142610 &lt;col:15&gt; '&lt;dependent type&gt;...' lvalue
|     `-ReturnStmt 0x1d142740 &lt;line:5:5, col:24&gt;
|       `-ImplicitCastExpr 0x1d142728 &lt;col:12, col:24&gt; 'int' &lt;LValueToRValue&gt;
|         `-PackIndexingExpr 0x1d1426f8 &lt;col:12, col:24&gt; 'int' lvalue
|           |-DeclRefExpr 0x1d11ce08 &lt;col:12&gt; '&lt;dependent type&gt;' lvalue Binding 0x1d11cc18 'members' '&lt;dependent type&gt;...' &lt;--- this should be 0x1d142188
|           `-ConstantExpr 0x1d1426d8 &lt;col:23&gt; '__size_t':'unsigned long'
```

Here is the full AST dump from compiler explorer:
```
|-FunctionTemplateDecl 0x1d11c7e0 &lt;&lt;source&gt;:1:1, line:6:1&gt; line:2:5 templated_fn
| |-TemplateTypeParmDecl 0x1d11c5e0 &lt;line:1:11, col:19&gt; col:17 class depth 0 index 0
| | `-TemplateArgument type 'void'
| |   `-BuiltinType 0x1d0c3fe0 'void'
| |-FunctionDecl 0x1d11c730 &lt;line:2:1, line:6:1&gt; line:2:5 templated_fn 'int ()'
| | `-CompoundStmt 0x1d11ceb8 &lt;col:20, line:6:1&gt;
| |   |-DeclStmt 0x1d11cb78 &lt;line:3:5, col:24&gt;
| |   | `-CXXRecordDecl 0x1d11c910 &lt;col:5, col:23&gt; col:12 referenced struct S definition
| |   |   |-DefinitionData aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor
| |   |   | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
| |   |   | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
| |   |   | |-MoveConstructor exists simple trivial needs_implicit
| |   |   | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
| |   |   | |-MoveAssignment exists simple trivial needs_implicit
| |   |   | `-Destructor simple irrelevant trivial constexpr needs_implicit
| |   |   |-CXXRecordDecl 0x1d11ca28 &lt;col:5, col:12&gt; col:12 implicit struct S
| |   |   `-FieldDecl 0x1d11caf0 &lt;col:16, col:20&gt; col:20 a 'int'
| |   |-DeclStmt 0x1d11cdf0 &lt;line:4:5, col:31&gt;
| |   | `-DecompositionDecl 0x1d11cc68 &lt;col:5, col:30&gt; col:10 'auto' cinit
| |   |   |-CXXUnresolvedConstructExpr 0x1d11cd98 &lt;col:26, col:30&gt; 'S' 'S' list
| |   |   | `-InitListExpr 0x1d11cd50 &lt;col:27, col:30&gt; 'void'
| |   |   |   `-IntegerLiteral 0x1d11cd30 &lt;col:28&gt; 'int' 12
| |   |   `-BindingDecl 0x1d11cc18 &lt;col:15&gt; col:15 referenced members '&lt;dependent type&gt;...'
| |   `-ReturnStmt 0x1d11cea8 &lt;line:5:5, col:24&gt;
| |     `-PackIndexingExpr 0x1d11ce80 &lt;col:12, col:24&gt; '&lt;dependent type&gt;' lvalue
| |       |-DeclRefExpr 0x1d11ce08 &lt;col:12&gt; '&lt;dependent type&gt;' lvalue Binding 0x1d11cc18 'members' '&lt;dependent type&gt;...'
| |       `-ConstantExpr 0x1d11ce60 &lt;col:23&gt; '__size_t':'unsigned long'
| |         |-value: Int 0
| |         `-ImplicitCastExpr 0x1d11ce48 &lt;col:23&gt; '__size_t':'unsigned long' &lt;IntegralCast&gt;
| |           `-IntegerLiteral 0x1d11ce28 &lt;col:23&gt; 'int' 0
| `-FunctionDecl 0x1d11d0a8 &lt;line:2:1, line:6:1&gt; line:2:5 used templated_fn 'int ()' implicit_instantiation instantiated_from 0x1d11c730
|   |-TemplateArgument type 'void'
|   | `-BuiltinType 0x1d0c3fe0 'void'
|   `-CompoundStmt 0x1d142750 &lt;col:20, line:6:1&gt;
|     |-DeclStmt 0x1d142168 &lt;line:3:5, col:24&gt;
|     | `-CXXRecordDecl 0x1d11d290 &lt;col:5, col:23&gt; col:12 struct S definition instantiated_from 0x1d11c910
|     |   |-DefinitionData pass_in_registers aggregate standard_layout trivially_copyable pod trivial literal has_constexpr_non_copy_move_ctor
|     |   | |-DefaultConstructor exists trivial constexpr needs_implicit defaulted_is_constexpr
|     |   | |-CopyConstructor simple trivial has_const_param needs_implicit implicit_has_const_param
|     |   | |-MoveConstructor exists simple trivial needs_implicit
|     |   | |-CopyAssignment simple trivial has_const_param needs_implicit implicit_has_const_param
|     |   | |-MoveAssignment exists simple trivial needs_implicit
|     |   | `-Destructor simple irrelevant trivial constexpr
|     |   |-CXXRecordDecl 0x1d142028 &lt;col:5, col:12&gt; col:12 implicit struct S
|     |   |-FieldDecl 0x1d1420d8 &lt;col:16, col:20&gt; col:20 referenced a 'int'
|     |   `-CXXDestructorDecl 0x1d142320 &lt;col:12&gt; col:12 implicit referenced constexpr ~S 'void () noexcept' inline default trivial
|     |-DeclStmt 0x1d142688 &lt;line:4:5, col:31&gt;
|     | `-DecompositionDecl 0x1d1421d8 &lt;col:5, col:30&gt; col:10 used 'S' cinit
|     |   |-CXXFunctionalCastExpr 0x1d142450 &lt;col:26, col:30&gt; 'S' functional cast to S &lt;NoOp&gt;
|     |   | `-InitListExpr 0x1d1422d0 &lt;col:27, col:30&gt; 'S'
|     |   |   `-IntegerLiteral 0x1d11cd30 &lt;col:28&gt; 'int' 12
|     |   `-BindingDecl 0x1d142188 &lt;col:15&gt; col:15 referenced members '&lt;dependent type&gt;...'
|     |     `-FunctionParmPackExpr 0x1d142610 &lt;col:15&gt; '&lt;dependent type&gt;...' lvalue
|     `-ReturnStmt 0x1d142740 &lt;line:5:5, col:24&gt;
|       `-ImplicitCastExpr 0x1d142728 &lt;col:12, col:24&gt; 'int' &lt;LValueToRValue&gt;
|         `-PackIndexingExpr 0x1d1426f8 &lt;col:12, col:24&gt; 'int' lvalue
|           |-DeclRefExpr 0x1d11ce08 &lt;col:12&gt; '&lt;dependent type&gt;' lvalue Binding 0x1d11cc18 'members' '&lt;dependent type&gt;...'
|           `-ConstantExpr 0x1d1426d8 &lt;col:23&gt; '__size_t':'unsigned long'
|             |-value: Int 0
|             `-ImplicitCastExpr 0x1d1426c0 &lt;col:23&gt; '__size_t':'unsigned long' &lt;IntegralCast&gt;
|               `-IntegerLiteral 0x1d11ce28 &lt;col:23&gt; 'int' 0
`-FunctionDecl 0x1d11cf58 &lt;line:8:1, line:10:1&gt; line:8:6 repro 'void ()'
  `-CompoundStmt 0x1d11d270 &lt;col:14, line:10:1&gt;
    `-CallExpr 0x1d11d248 &lt;line:9:5, col:18&gt; 'int'
      `-ImplicitCastExpr 0x1d11d230 &lt;col:5&gt; 'int (*)()' &lt;FunctionToPointerDecay&gt;
        `-DeclRefExpr 0x1d11d1d0 &lt;col:5&gt; 'int ()' lvalue Function 0x1d11d0a8 'templated_fn' 'int ()' (FunctionTemplate 0x1d11c7e0 'templated_fn')
```

PS: I've noticed this while implementing structured binding pack indexing for the Clang Static Analyzer.
</details>


---

