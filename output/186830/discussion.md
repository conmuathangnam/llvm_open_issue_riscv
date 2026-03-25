# [MLIR] Crash in ExpandStridedMetadata on expand_shape with multiple dynamic dims per reassociation group

**Author:** edg-l
**URL:** https://github.com/llvm/llvm-project/issues/186830
**Status:** Open
**Labels:** mlir, crash

## Body

## Description

`mlir-opt --pass-pipeline="builtin.module(func.func(expand-strided-metadata))"` segfaults when processing a `memref.expand_shape` that has more than one dynamic dimension in a single reassociation group.

## Reproducer

```mlir
func.func @crash(%arg0: memref<?x?x?xf32>, %s0: index, %s1: index, %s2: index, %s3: index) -> memref<?x?x?x?xf32> {
  %expand = memref.expand_shape %arg0 [[0, 1], [2], [3]] output_shape [%s0, %s1, %s2, %s3]
      : memref<?x?x?xf32> into memref<?x?x?x?xf32>
  return %expand : memref<?x?x?x?xf32>
}
```

```
mlir-opt test.mlir --pass-pipeline="builtin.module(func.func(expand-strided-metadata))"
```

## Root Cause

`getExpandedSizes` (`ExpandStridedMetadata.cpp:269`) and `getExpandedStrides` (`ExpandStridedMetadata.cpp:332`) both assume at most one dynamic dimension per reassociation group:

```cpp
assert(!dynSizeIdx && "There must be at most one dynamic size per group");
```

When a group has two dynamic dims (e.g., group `[0, 1]` in `memref<?x?x?x?xf32>`):

1. `expandedSizes[0]` is skipped (first dynamic dim), left as default-constructed null `OpFoldResult`
2. `dynSizeIdx` is overwritten to 1 (second dynamic dim, assert only fires in debug builds)
3. Only `expandedSizes[1]` is computed
4. The null `OpFoldResult` at index 0 is passed to `dispatchIndexOpFoldResult`, which does `cast<IntegerAttr>(cast<Attribute>(ofr)).getValue()` on it → segfault in `APInt` copy constructor

This assumption predates the addition of the `output_shape` operand to `expand_shape`, which allows multiple dynamic dims per group. The fix should use the `output_shape` operands to get the actual sizes when there are multiple dynamic dims.

## Stack Trace (abbreviated)

```
#4  llvm::APInt::APInt(llvm::APInt const&)
#5  mlir::IntegerAttr::getValue() const
#7  mlir::dispatchIndexOpFoldResult(...)  StaticValueUtils.cpp:49
#9  mlir::dispatchIndexOpFoldResults(...)  StaticValueUtils.cpp:73
#10 mlir::memref::ReinterpretCastOp::build(...)  MemRefOps.cpp:1795
#13 ReshapeFolder<memref::ExpandShapeOp, ...>::matchAndRewrite(...)  ExpandStridedMetadata.cpp:670
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Edgar (edg-l)

<details>
## Description

`mlir-opt --pass-pipeline="builtin.module(func.func(expand-strided-metadata))"` segfaults when processing a `memref.expand_shape` that has more than one dynamic dimension in a single reassociation group.

## Reproducer

```mlir
func.func @<!-- -->crash(%arg0: memref&lt;?x?x?xf32&gt;, %s0: index, %s1: index, %s2: index, %s3: index) -&gt; memref&lt;?x?x?x?xf32&gt; {
  %expand = memref.expand_shape %arg0 [[0, 1], [2], [3]] output_shape [%s0, %s1, %s2, %s3]
      : memref&lt;?x?x?xf32&gt; into memref&lt;?x?x?x?xf32&gt;
  return %expand : memref&lt;?x?x?x?xf32&gt;
}
```

```
mlir-opt test.mlir --pass-pipeline="builtin.module(func.func(expand-strided-metadata))"
```

## Root Cause

`getExpandedSizes` (`ExpandStridedMetadata.cpp:269`) and `getExpandedStrides` (`ExpandStridedMetadata.cpp:332`) both assume at most one dynamic dimension per reassociation group:

```cpp
assert(!dynSizeIdx &amp;&amp; "There must be at most one dynamic size per group");
```

When a group has two dynamic dims (e.g., group `[0, 1]` in `memref&lt;?x?x?x?xf32&gt;`):

1. `expandedSizes[0]` is skipped (first dynamic dim), left as default-constructed null `OpFoldResult`
2. `dynSizeIdx` is overwritten to 1 (second dynamic dim, assert only fires in debug builds)
3. Only `expandedSizes[1]` is computed
4. The null `OpFoldResult` at index 0 is passed to `dispatchIndexOpFoldResult`, which does `cast&lt;IntegerAttr&gt;(cast&lt;Attribute&gt;(ofr)).getValue()` on it → segfault in `APInt` copy constructor

This assumption predates the addition of the `output_shape` operand to `expand_shape`, which allows multiple dynamic dims per group. The fix should use the `output_shape` operands to get the actual sizes when there are multiple dynamic dims.

## Stack Trace (abbreviated)

```
#<!-- -->4  llvm::APInt::APInt(llvm::APInt const&amp;)
#<!-- -->5  mlir::IntegerAttr::getValue() const
#<!-- -->7  mlir::dispatchIndexOpFoldResult(...)  StaticValueUtils.cpp:49
#<!-- -->9  mlir::dispatchIndexOpFoldResults(...)  StaticValueUtils.cpp:73
#<!-- -->10 mlir::memref::ReinterpretCastOp::build(...)  MemRefOps.cpp:1795
#<!-- -->13 ReshapeFolder&lt;memref::ExpandShapeOp, ...&gt;::matchAndRewrite(...)  ExpandStridedMetadata.cpp:670
```
</details>


---

