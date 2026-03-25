# [HLSL][Matrix]  Failure to initalize vector via cast of a matrix to vector

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/169561
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2026-02-18T19:05:23Z

## Body

The code below is failing in `clang/lib/Sema/SemaInit.cpp` via a `FK_ConversionFailed` in clang when it works fine in DXC.

https://godbolt.org/z/r9G7fYzcj
```hlsl
export float4 fn(float2x2 m) {
    float4 v = m;
    return v;
}
```
Note the explicit cast version crashes today, but wont after: https://github.com/llvm/llvm-project/pull/168915
```hlsl
export float4 fn(float2x2 m) {
    float4 v = (float4)m;
    return v;
}
```

```bash
error: cannot initialize a variable of type 'float4' (aka 'vector<float, 4>') with an rvalue of type 'float2x2' (aka 'matrix<float, 2, 2>')
    2 |     float4 v = (float4)m;
      |            ^   ~~~~~~~~~~~

```

The bug is happening because we are not hitting this block in  `InitializationSequence::InitializeFrom`
```cpp
// For HLSL ext vector types we allow list initialization behavior for C++
  // functional cast expressions which look like constructor syntax. This is
  // accomplished by converting initialization arguments to InitListExpr.
  if (S.getLangOpts().HLSL && Args.size() > 1 &&
      (DestType->isExtVectorType() || DestType->isConstantMatrixType()) &&
      (SourceType.isNull() ||
       !Context.hasSameUnqualifiedType(SourceType, DestType))) {
    InitListExpr *ILE = new (Context)
        InitListExpr(S.getASTContext(), Args.front()->getBeginLoc(), Args,
                     Args.back()->getEndLoc());
    ILE->setType(DestType);
    Args[0] = ILE;
    TryListInitialization(S, Entity, Kind, ILE, *this,
                          TreatUnavailableAsInvalid);
    return;
  }
```
We can see if we did every thing would work fine:
https://godbolt.org/z/GMej3Kvvr

```hlsl
export float4 fn(float2x2 m) {
    float4 v = {m};
    return v;
}
```

Ie if we did vector initalization via matrix inside an initalizer list everthing would work just fine.

## Comments

### Comment 1 - Icohedron

> Note the explicit cast version crashes today, but wont after: [#168915](https://github.com/llvm/llvm-project/pull/168915)
> 
> export float4 fn(float2x2 m) {
>     float4 v = (float4)m;
>     return v;
> }

The explicit cast version still crashes today. I created a new issue to track it: #177487

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Farzon Lotfi (farzonl)

<details>
The code below is failing in `clang/lib/Sema/SemaInit.cpp` via a `FK_ConversionFailed` in clang when it works fine in DXC.

https://godbolt.org/z/r9G7fYzcj
```hlsl
export float4 fn(float2x2 m) {
    float4 v = m;
    return v;
}
```
Note the explicit cast version crashes today, but wont after: https://github.com/llvm/llvm-project/pull/168915
```hlsl
export float4 fn(float2x2 m) {
    float4 v = (float4)m;
    return v;
}
```

```bash
error: cannot initialize a variable of type 'float4' (aka 'vector&lt;float, 4&gt;') with an rvalue of type 'float2x2' (aka 'matrix&lt;float, 2, 2&gt;')
    2 |     float4 v = (float4)m;
      |            ^   ~~~~~~~~~~~

```

The bug is happening because we are not hitting this block in  `InitializationSequence::InitializeFrom`
```cpp
// For HLSL ext vector types we allow list initialization behavior for C++
  // functional cast expressions which look like constructor syntax. This is
  // accomplished by converting initialization arguments to InitListExpr.
  if (S.getLangOpts().HLSL &amp;&amp; Args.size() &gt; 1 &amp;&amp;
      (DestType-&gt;isExtVectorType() || DestType-&gt;isConstantMatrixType()) &amp;&amp;
      (SourceType.isNull() ||
       !Context.hasSameUnqualifiedType(SourceType, DestType))) {
    InitListExpr *ILE = new (Context)
        InitListExpr(S.getASTContext(), Args.front()-&gt;getBeginLoc(), Args,
                     Args.back()-&gt;getEndLoc());
    ILE-&gt;setType(DestType);
    Args[0] = ILE;
    TryListInitialization(S, Entity, Kind, ILE, *this,
                          TreatUnavailableAsInvalid);
    return;
  }
```
We can see if we did every thing would work fine:
https://godbolt.org/z/GMej3Kvvr

```hlsl
export float4 fn(float2x2 m) {
    float4 v = {m};
    return v;
}
```

Ie if we did vector initalization via matrix inside an initalizer list everthing would work just fine.
</details>


---

