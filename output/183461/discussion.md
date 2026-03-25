# [HLSL] Explicitly casting an array to a scalar performs an HLSLElementwiseCast

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/183461
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2026-03-02T18:50:31Z

## Body

Explicitly casting an array to a scalar performs an elementwise conversion (`HLSLElementwiseCast`) in Clang but not in DXC.
The [hlsl-spec](https://github.com/microsoft/hlsl-specs/blob/58965078aa95b157767b035f5566c76318c70dfd/specs/language/conversions.tex#L124-L125) also states that an elementwise conversion is aggregate -> aggregate, and does not mention aggregate -> scalar.

Repro: https://hlsl.godbolt.org/z/qGoTPMshK

Given the HLSL:
```hlsl
void fn() {
    int A[4] = {1, 2, 3, 4};
    int I = (int)A;
}
```

The following AST is produced for the VarDecl `int I = (int)A;`:
```
      `-VarDecl <col:5, col:18> col:9 I 'int' cinit
        `-CStyleCastExpr <col:13, col:18> 'int' <HLSLElementwiseCast>
          `-ImplicitCastExpr <col:18> 'int[4]' <HLSLArrayRValue> part_of_explicit_cast
            `-DeclRefExpr <col:18> 'int[4]' lvalue Var 0x1c037468 'A' 'int[4]'
```

While this seems harmless and produces the expected behavior, it affects the implementation of the new constant expression evaluator which now has to explicitly account for the possibility of `HLSLElementwiseCast` emitting a scalar instead of an aggregate type.

Note that DXC produces the following AST for the same VarDecl:
```
      `-VarDecl <col:5, col:18> col:9 I 'int' cinit
        `-ImplicitCastExpr <col:13, col:18> 'int' <LValueToRValue>
          `-CStyleCastExpr <col:13, col:18> 'int' lvalue <NoOp>
            `-ImplicitCastExpr <col:18> 'int' <FlatConversion>
              `-ImplicitCastExpr <col:18> 'int [4]' <LValueToRValue>
                `-DeclRefExpr <col:18> 'int [4]' lvalue Var 0x5a64f371e528 'A' 'int [4]'
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
Explicitly casting an array to a scalar performs an elementwise conversion (`HLSLElementwiseCast`) in Clang but not in DXC.
The [hlsl-spec](https://github.com/microsoft/hlsl-specs/blob/58965078aa95b157767b035f5566c76318c70dfd/specs/language/conversions.tex#L124-L125) also states that an elementwise conversion is aggregate -&gt; aggregate, and does not mention aggregate -&gt; scalar.

Repro: https://hlsl.godbolt.org/z/qGoTPMshK

Given the HLSL:
```hlsl
void fn() {
    int A[4] = {1, 2, 3, 4};
    int I = (int)A;
}
```

The following AST is produced for the VarDecl `int I = (int)A;`:
```
      `-VarDecl &lt;col:5, col:18&gt; col:9 I 'int' cinit
        `-CStyleCastExpr &lt;col:13, col:18&gt; 'int' &lt;HLSLElementwiseCast&gt;
          `-ImplicitCastExpr &lt;col:18&gt; 'int[4]' &lt;HLSLArrayRValue&gt; part_of_explicit_cast
            `-DeclRefExpr &lt;col:18&gt; 'int[4]' lvalue Var 0x1c037468 'A' 'int[4]'
```

DXC produces the following AST for the same VarDecl:
```
      `-VarDecl &lt;col:5, col:18&gt; col:9 I 'int' cinit
        `-ImplicitCastExpr &lt;col:13, col:18&gt; 'int' &lt;LValueToRValue&gt;
          `-CStyleCastExpr &lt;col:13, col:18&gt; 'int' lvalue &lt;NoOp&gt;
            `-ImplicitCastExpr &lt;col:18&gt; 'int' &lt;FlatConversion&gt;
              `-ImplicitCastExpr &lt;col:18&gt; 'int [4]' &lt;LValueToRValue&gt;
                `-DeclRefExpr &lt;col:18&gt; 'int [4]' lvalue Var 0x5a64f371e528 'A' 'int [4]'
```

While this seems harmless and produces the expected behavior, it affects the implementation of the new constant expression evaluator which now has to explicitly account for the possibility of `HLSLElementwiseCast` emitting a scalar instead of an aggregate type.
</details>


---

### Comment 2 - farzonl

This is the same cast. in both but this might just be a spec issue we need to fix

---

