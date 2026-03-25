# [InstCombine] Incorrectly scales fixed-result `vector.extract` index when folding `get_active_lane_mask`

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/185271
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2026-03-09T11:51:48Z

## Body

Here is a miscompilation case when reviewing https://github.com/llvm/llvm-project/pull/183329:

```llvm
define <4 x i1> @bail_lhs_is_zero() vscale_range(4, 4) {
  %mask = call <vscale x 4 x i1> @llvm.get.active.lane.mask.nxv4i1.i32(i32 0, i32 15)
  %ext = call <4 x i1> @llvm.vector.extract.v4i1.nxv4i1(<vscale x 4 x i1> %mask, i64 4)
  ret <4 x i1> %ext
}

declare <vscale x 4 x i1> @llvm.get.active.lane.mask.nxv4i1.i32(i32, i32)
declare <4 x i1> @llvm.vector.extract.v4i1.nxv4i1(<vscale x 4 x i1>, i64)
```

With patch built on this patch, it's transformed into:

```llvm
define <4 x i1> @bail_lhs_is_zero() #0 {
  ret <4 x i1> zeroinitializer
}

declare <vscale x 4 x i1> @llvm.get.active.lane.mask.nxv4i1.i32(i32, i32) #1

declare <4 x i1> @llvm.vector.extract.v4i1.nxv4i1(<vscale x 4 x i1>, i64 immarg) #1

attributes #0 = { vscale_range(4,4) }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Compiler Explorer: https://godbolt.org/z/1sEGW8jhz

> Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**

The test case triggers the bug because it extracts a fixed-width subvector from the result of `get_active_lane_mask`, while the function has `vscale_range(4, 4)`. For a fixed-width vector.extract, the extract index is interpreted as a plain element index, so this extract starts at lane 4 and returns lanes 4–7. For `get_active_lane_mask(0, 15)`, those lanes are still active, so the extracted result is not all-false. However, the transform incorrectly folds it to zeroinitializer, changing the program’s meaning.

```
full mask:   [T T T T T T T T T T T T T T T F]
extract @ 4:         [T T T T]
wrong fold:          [F F F F]
```

**Fix Weakness Analysis:**

The weakness in the fix is that it uses `ExtractIdx * VScaleMin` to decide whether all extracted lanes are inactive. That reasoning is only valid when the extract result is itself scalable. In this test case, the result is fixed-width, so the starting lane is just `ExtractIdx`, not `ExtractIdx * vscale`. As a result, the fix applies scalable-index reasoning to a fixed-result extract and can incorrectly conclude that the extracted subvector is entirely inactive, leading to a wrong-code fold to zero.

cc @kmclaughlin-arm 

