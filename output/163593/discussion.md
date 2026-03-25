# [HLSL] Missing LValue to RValue cast in implicit cast for HLSLAggregateSplatCast

**Author:** spall
**URL:** https://github.com/llvm/llvm-project/issues/163593
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-10-16T18:28:15Z

## Body

HLSLAggregateSplatCast should always contain an lvalue to rvalue cast. 

```
int1 A = {1};
int4 B = (int4)A;
```
In this example there is a missing LValue to RValue cast, so the truncation is resulting in an LValue. But before the HLSLVectorTruncation there should be an LValue to RValue cast.
```
-CStyleCastExpr 0x193fb1c0 <col:15, col:23> 'float4':'vector<float, 4>' <HLSLAggregateSplatCast>
  |     `-ImplicitCastExpr 0x193fb1a8 <col:23> 'float' lvalue <HLSLVectorTruncation> part_of_explicit_cast
  |       `-DeclRefExpr 0x193fb170 <col:23> 'float1':'vector<float, 1>' lvalue ParmVar 0x193faec8 'I4' 'float1':'vector<float, 1>'
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sarah Spall (spall)

<details>
HLSLAggregateSplatCast should always contain an lvalue to rvalue cast. 

```
int1 A = {1};
int4 B = (int4)A;
```
In this example there is a missing LValue to RValue cast, so the truncation is resulting in an LValue. But before the HLSLVectorTruncation there should be an LValue to RValue cast.
```
-CStyleCastExpr 0x193fb1c0 &lt;col:15, col:23&gt; 'float4':'vector&lt;float, 4&gt;' &lt;HLSLAggregateSplatCast&gt;
  |     `-ImplicitCastExpr 0x193fb1a8 &lt;col:23&gt; 'float' lvalue &lt;HLSLVectorTruncation&gt; part_of_explicit_cast
  |       `-DeclRefExpr 0x193fb170 &lt;col:23&gt; 'float1':'vector&lt;float, 1&gt;' lvalue ParmVar 0x193faec8 'I4' 'float1':'vector&lt;float, 1&gt;'
```
</details>


---

