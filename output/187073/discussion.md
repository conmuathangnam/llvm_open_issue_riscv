# [MLIR] Crash in tileToForallOpImpl when tileUsingSCF returns empty loops

**Author:** edg-l
**URL:** https://github.com/llvm/llvm-project/issues/187073
**Status:** Open
**Labels:** mlir, crash

## Body

## Description

`tileToForallOpImpl` in `LinalgTransformOps.cpp` crashes when `scf::tileUsingSCF` returns an empty `loops` vector. The code unconditionally accesses `tilingResult.loops.front()` at line 3686 without checking if `loops` is empty, causing undefined behavior.

This can happen when `transform.foreach_match` walks the IR and matches an op that has no parallel iteration dimensions to tile (e.g., a rank-0 `linalg.generic`). In this case, `tileUsingSCF` succeeds but produces no loop ops, and `normalizeForallLoopOp` is then called with a garbage `ForallOp` pointer.

## Steps to Reproduce

```
mlir-opt repro.mlir --transform-interpreter
```

<details>
<summary>repro.mlir</summary>

```mlir
#map_rank0 = affine_map<() -> ()>
#map_matmul_a = affine_map<(d0, d1, d2) -> (d0, d2)>
#map_matmul_b = affine_map<(d0, d1, d2) -> (d2, d1)>
#map_matmul_c = affine_map<(d0, d1, d2) -> (d0, d1)>

module attributes {transform.with_named_sequence} {
  func.func @repro(%arg0: tensor<768x2304xf32>,
                   %arg1: tensor<i64>,
                   %arg2: tensor<?x2304xf32>,
                   %arg3: tensor<?x768xf32>) -> tensor<?x2304xf32> {
    %empty = tensor.empty() : tensor<i64>
    %copy = linalg.generic {
        indexing_maps = [#map_rank0, #map_rank0],
        iterator_types = []}
        ins(%arg1 : tensor<i64>) outs(%empty : tensor<i64>) {
    ^bb0(%in: i64, %out: i64):
      linalg.yield %in : i64
    } -> tensor<i64>

    %matmul = linalg.generic {
        indexing_maps = [#map_matmul_a, #map_matmul_b, #map_matmul_c],
        iterator_types = ["parallel", "parallel", "reduction"]}
        ins(%arg3, %arg0 : tensor<?x768xf32>, tensor<768x2304xf32>)
        outs(%arg2 : tensor<?x2304xf32>) {
    ^bb0(%in: f32, %in2: f32, %out: f32):
      %mul = arith.mulf %in, %in2 : f32
      %add = arith.addf %out, %mul : f32
      linalg.yield %add : f32
    } -> tensor<?x2304xf32>

    return %matmul : tensor<?x2304xf32>
  }

  transform.named_sequence private @match_contraction_3d(
      %arg0: !transform.any_op {transform.readonly}) -> !transform.any_op {
    %0:2 = transform.match.structured %arg0 : (!transform.any_op) ->
        (!transform.any_op, !transform.param<i64>) {
    ^bb0(%arg1: !transform.any_op):
      %1 = transform.match.structured.rank %arg1 : (!transform.any_op) ->
          !transform.param<i64>
      transform.match.structured.yield %arg1, %1 :
          !transform.any_op, !transform.param<i64>
    }
    transform.yield %0#0 : !transform.any_op
  }

  transform.named_sequence private @tile_contraction_3d(
      %arg0: !transform.any_op {transform.consumed}) {
    %tiled_op, %forall_op = transform.structured.tile_using_forall %arg0
        tile_sizes [32, 32, 0]
        {scalable_sizes = array<i1: false, false, false>} :
        (!transform.any_op) -> (!transform.any_op, !transform.any_op)
    transform.yield
  }

  transform.named_sequence private @__transform_main(
      %arg0: !transform.any_op {transform.consumed}) {
    %updated_root = transform.foreach_match in %arg0
        @match_contraction_3d -> @tile_contraction_3d :
        (!transform.any_op) -> !transform.any_op
    transform.yield
  }
}
```
</details>

## Stack Trace

```
 #4 mlir::AbstractAttribute::getDialect() const
 #5 mlir::Attribute::getDialect() const
 #6 mlir::Attribute::getContext() const
 #7 mlir::Builder::Builder(mlir::MLIRContext*)
 #8 mlir::scf::ForallOp::getLoopLowerBounds()
 #14 mlir::scf::ForallOp::getMixedLowerBound()
 #15 normalizeForallLoopOp(mlir::RewriterBase&, mlir::scf::ForallOp)  LinalgTransformOps.cpp:3613
 #16 mlir::transform::tileToForallOpImpl(...)  LinalgTransformOps.cpp:3690
 #17 mlir::transform::TileUsingForallOp::apply(...)  LinalgTransformOps.cpp:3731
```

## Root Cause

In `tileToForallOpImpl` (LinalgTransformOps.cpp:3685-3692):

```cpp
if (mixedNumThreads.empty()) {
    auto generatedForallOp = cast<scf::ForallOp>(tilingResult.loops.front());
    // ...
}
```

`tilingResult.loops` can be empty when `tileUsingSCF` produces no loops (e.g. for a rank-0 op with an empty iteration domain). `front()` on empty vector is UB, producing a garbage pointer that is then dereferenced by `normalizeForallLoopOp`.

## Proposed Fix

```cpp
if (mixedNumThreads.empty() && !tilingResult.loops.empty()) {
```

## Related Issue

There appears to be a second, deeper crash in `TransformState::checkAndRecordHandleInvalidation` that can occur when `foreach_match` applies actions that modify the IR. This manifests as corrupted memory in the transform state's handle tracking. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Edgar (edg-l)

<details>
## Description

`tileToForallOpImpl` in `LinalgTransformOps.cpp` crashes when `scf::tileUsingSCF` returns an empty `loops` vector. The code unconditionally accesses `tilingResult.loops.front()` at line 3686 without checking if `loops` is empty, causing undefined behavior.

This can happen when `transform.foreach_match` walks the IR and matches an op that has no parallel iteration dimensions to tile (e.g., a rank-0 `linalg.generic`). In this case, `tileUsingSCF` succeeds but produces no loop ops, and `normalizeForallLoopOp` is then called with a garbage `ForallOp` pointer.

## Steps to Reproduce

```
mlir-opt repro.mlir --transform-interpreter
```

&lt;details&gt;
&lt;summary&gt;repro.mlir&lt;/summary&gt;

```mlir
#map_rank0 = affine_map&lt;() -&gt; ()&gt;
#map_matmul_a = affine_map&lt;(d0, d1, d2) -&gt; (d0, d2)&gt;
#map_matmul_b = affine_map&lt;(d0, d1, d2) -&gt; (d2, d1)&gt;
#map_matmul_c = affine_map&lt;(d0, d1, d2) -&gt; (d0, d1)&gt;

module attributes {transform.with_named_sequence} {
  func.func @<!-- -->repro(%arg0: tensor&lt;768x2304xf32&gt;,
                   %arg1: tensor&lt;i64&gt;,
                   %arg2: tensor&lt;?x2304xf32&gt;,
                   %arg3: tensor&lt;?x768xf32&gt;) -&gt; tensor&lt;?x2304xf32&gt; {
    %empty = tensor.empty() : tensor&lt;i64&gt;
    %copy = linalg.generic {
        indexing_maps = [#map_rank0, #map_rank0],
        iterator_types = []}
        ins(%arg1 : tensor&lt;i64&gt;) outs(%empty : tensor&lt;i64&gt;) {
    ^bb0(%in: i64, %out: i64):
      linalg.yield %in : i64
    } -&gt; tensor&lt;i64&gt;

    %matmul = linalg.generic {
        indexing_maps = [#map_matmul_a, #map_matmul_b, #map_matmul_c],
        iterator_types = ["parallel", "parallel", "reduction"]}
        ins(%arg3, %arg0 : tensor&lt;?x768xf32&gt;, tensor&lt;768x2304xf32&gt;)
        outs(%arg2 : tensor&lt;?x2304xf32&gt;) {
    ^bb0(%in: f32, %in2: f32, %out: f32):
      %mul = arith.mulf %in, %in2 : f32
      %add = arith.addf %out, %mul : f32
      linalg.yield %add : f32
    } -&gt; tensor&lt;?x2304xf32&gt;

    return %matmul : tensor&lt;?x2304xf32&gt;
  }

  transform.named_sequence private @<!-- -->match_contraction_3d(
      %arg0: !transform.any_op {transform.readonly}) -&gt; !transform.any_op {
    %0:2 = transform.match.structured %arg0 : (!transform.any_op) -&gt;
        (!transform.any_op, !transform.param&lt;i64&gt;) {
    ^bb0(%arg1: !transform.any_op):
      %1 = transform.match.structured.rank %arg1 : (!transform.any_op) -&gt;
          !transform.param&lt;i64&gt;
      transform.match.structured.yield %arg1, %1 :
          !transform.any_op, !transform.param&lt;i64&gt;
    }
    transform.yield %0#<!-- -->0 : !transform.any_op
  }

  transform.named_sequence private @<!-- -->tile_contraction_3d(
      %arg0: !transform.any_op {transform.consumed}) {
    %tiled_op, %forall_op = transform.structured.tile_using_forall %arg0
        tile_sizes [32, 32, 0]
        {scalable_sizes = array&lt;i1: false, false, false&gt;} :
        (!transform.any_op) -&gt; (!transform.any_op, !transform.any_op)
    transform.yield
  }

  transform.named_sequence private @<!-- -->__transform_main(
      %arg0: !transform.any_op {transform.consumed}) {
    %updated_root = transform.foreach_match in %arg0
        @<!-- -->match_contraction_3d -&gt; @<!-- -->tile_contraction_3d :
        (!transform.any_op) -&gt; !transform.any_op
    transform.yield
  }
}
```
&lt;/details&gt;

## Stack Trace

```
 #<!-- -->4 mlir::AbstractAttribute::getDialect() const
 #<!-- -->5 mlir::Attribute::getDialect() const
 #<!-- -->6 mlir::Attribute::getContext() const
 #<!-- -->7 mlir::Builder::Builder(mlir::MLIRContext*)
 #<!-- -->8 mlir::scf::ForallOp::getLoopLowerBounds()
 #<!-- -->14 mlir::scf::ForallOp::getMixedLowerBound()
 #<!-- -->15 normalizeForallLoopOp(mlir::RewriterBase&amp;, mlir::scf::ForallOp)  LinalgTransformOps.cpp:3613
 #<!-- -->16 mlir::transform::tileToForallOpImpl(...)  LinalgTransformOps.cpp:3690
 #<!-- -->17 mlir::transform::TileUsingForallOp::apply(...)  LinalgTransformOps.cpp:3731
```

## Root Cause

In `tileToForallOpImpl` (LinalgTransformOps.cpp:3685-3692):

```cpp
if (mixedNumThreads.empty()) {
    auto generatedForallOp = cast&lt;scf::ForallOp&gt;(tilingResult.loops.front());
    // ...
}
```

`tilingResult.loops` can be empty when `tileUsingSCF` produces no loops (e.g. for a rank-0 op with an empty iteration domain). `front()` on empty vector is UB, producing a garbage pointer that is then dereferenced by `normalizeForallLoopOp`.

## Proposed Fix

```cpp
if (mixedNumThreads.empty() &amp;&amp; !tilingResult.loops.empty()) {
```

## Related Issue

There appears to be a second, deeper crash in `TransformState::checkAndRecordHandleInvalidation` that can occur when `foreach_match` applies actions that modify the IR. This manifests as corrupted memory in the transform state's handle tracking. It may warrant a separate investigation.
</details>


---

