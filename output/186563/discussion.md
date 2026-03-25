# DAGCombiner ext(ext) chain collapses index across scalability boundary

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/186563
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2026-03-19T11:14:32Z

## Body

Incorrect vector elements are returned with no diagnostic. The error magnitude scales with `vscale`; on wider SVE implementations (VLEN=512, vscale=4) the offset doubles.

PoC: [https://godbolt.org/z/z54vo5x3r](https://godbolt.org/z/z54vo5x3r)

## Vulnerable code location(s)

- [llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp lines 26947–26959](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp#L26947-L26959)

## Vulnerable code analysis

The combine `ext(ext(X, C), 0) → ext(X, C)` reuses the inner index `C` verbatim for the combined node, but `EXTRACT_SUBVECTOR` index semantics depend on the **result type's scalability**: a scalable result type interprets `C` as `C * vscale` elements, while a fixed result type interprets `C` as a literal element offset. When the inner extract produces a scalable vector and the outer extract produces a fixed-length vector, the two nodes use `C` with different scales, and the combined node uses the wrong interpretation.

Concrete example (`poc.ll`): `ext(nxv4i32 X, C=2) = nxv2i32` selects elements `{X[2·vscale]…}`, then `ext(nxv2i32, 0) = v2i32` takes the first 2 of those. The combine emits `ext(nxv4i32 X, 2) = v2i32`, which selects fixed elements `{X[2], X[3]}`. At `vscale=2` (VLEN=256) the result is `{X[2], X[3]}` instead of the correct `{X[4], X[5]}`.

The combine fires on AArch64 SVE because `isExtractSubvectorCheap(v2i32, nxv4i32, 2)` returns `true` (index equals `ResVT.getVectorMinNumElements()`). RISC-V is not affected — its `isExtractSubvectorCheap` explicitly returns `false` for scalable sources with non-zero indices.

The immediately adjacent combine at line 26978 already guards against this with `isFixedLengthVector()` checks on all three types. The ext-of-ext combine lacks an equivalent guard.

**Proposed one-line fix** (line 26949):

```cpp
(!V.getValueType().isScalableVector() || NVT.isScalableVector())
```






## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Yihan Yang (yijan4845)

<details>
Incorrect vector elements are returned with no diagnostic. The error magnitude scales with `vscale`; on wider SVE implementations (VLEN=512, vscale=4) the offset doubles.

PoC: [https://godbolt.org/z/z54vo5x3r](https://godbolt.org/z/z54vo5x3r)

## Vulnerable code location(s)

- [llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp lines 26947–26959](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp#L26947-L26959)

## Vulnerable code analysis

The combine `ext(ext(X, C), 0) → ext(X, C)` reuses the inner index `C` verbatim for the combined node, but `EXTRACT_SUBVECTOR` index semantics depend on the **result type's scalability**: a scalable result type interprets `C` as `C * vscale` elements, while a fixed result type interprets `C` as a literal element offset. When the inner extract produces a scalable vector and the outer extract produces a fixed-length vector, the two nodes use `C` with different scales, and the combined node uses the wrong interpretation.

Concrete example (`poc.ll`): `ext(nxv4i32 X, C=2) = nxv2i32` selects elements `{X[2·vscale]…}`, then `ext(nxv2i32, 0) = v2i32` takes the first 2 of those. The combine emits `ext(nxv4i32 X, 2) = v2i32`, which selects fixed elements `{X[2], X[3]}`. At `vscale=2` (VLEN=256) the result is `{X[2], X[3]}` instead of the correct `{X[4], X[5]}`.

The combine fires on AArch64 SVE because `isExtractSubvectorCheap(v2i32, nxv4i32, 2)` returns `true` (index equals `ResVT.getVectorMinNumElements()`). RISC-V is not affected — its `isExtractSubvectorCheap` explicitly returns `false` for scalable sources with non-zero indices.

The immediately adjacent combine at line 26978 already guards against this with `isFixedLengthVector()` checks on all three types. The ext-of-ext combine lacks an equivalent guard.

**Proposed one-line fix** (line 26949):

```cpp
(!V.getValueType().isScalableVector() || NVT.isScalableVector())
```





</details>


---

