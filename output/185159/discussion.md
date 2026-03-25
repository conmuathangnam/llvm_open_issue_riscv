# [InlineFunction] `nofpclass` return attributes leak across return paths during inlining

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/185159
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2026-03-16T21:14:14Z

## Body

Here is a miscompilation bug when reviewing https://github.com/llvm/llvm-project/pull/179776:

```llvm
declare float @use.val(float)

define float @callee_nofpclass_inf_nan(i1 %c) {
entry:
  br i1 %c, label %bb1, label %bb2

bb1:
  %r1 = call nofpclass(nan) float @use.val(float 1.0)
  ret float %r1

bb2:
  %r2 = call nofpclass(inf) float @use.val(float 2.0)
  ret float %r2
}

define float @caller_okay_intersect_nofpclass(i1 %c) {
  %r = call nofpclass(zero) float @callee_nofpclass_inf_nan(i1 %c)
  ret float %r
}
```

It's currently transformed into:

```llvm
; ModuleID = 'src.ll'
source_filename = "src.ll"

declare float @use.val(float)

define float @callee_nofpclass_inf_nan(i1 %c) {
entry:
  br i1 %c, label %bb1, label %bb2

bb1:                                              ; preds = %entry
  %r1 = call nofpclass(nan) float @use.val(float 1.000000e+00)
  ret float %r1

bb2:                                              ; preds = %entry
  %r2 = call nofpclass(inf) float @use.val(float 2.000000e+00)
  ret float %r2
}

define float @caller_okay_intersect_nofpclass(i1 %c) {
  br i1 %c, label %bb1.i, label %bb2.i

bb1.i:                                            ; preds = %0
  %r1.i = call nofpclass(nan zero) float @use.val(float 1.000000e+00)
  br label %callee_nofpclass_inf_nan.exit

bb2.i:                                            ; preds = %0
  %r2.i = call nofpclass(nan inf zero) float @use.val(float 2.000000e+00)
  br label %callee_nofpclass_inf_nan.exit

callee_nofpclass_inf_nan.exit:                    ; preds = %bb2.i, %bb1.i
  %r1 = phi float [ %r1.i, %bb1.i ], [ %r2.i, %bb2.i ]
  ret float %r1
}
```

> Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**
The provided test case triggers the bug because it contains a callee function (`callee_nofpclass_inf_nan`) with multiple return instructions, each returning the result of a call with a different `nofpclass` attribute (`nofpclass(nan)` and `nofpclass(inf)`). The caller (`caller_okay_intersect_nofpclass`) calls this callee with a third `nofpclass` attribute (`nofpclass(zero)`). 

During inlining, the `InlineFunction` pass iterates over the basic blocks of the callee to process return instructions. It uses a shared `AttrBuilder` named `ValidPG` to accumulate attributes that can be safely propagated from the call site to the inlined calls. When processing the first return instruction (`bb1`), it adds the `nofpclass(nan)` attribute to `ValidPG`. Because `ValidPG` is modified in place and not reset for each return instruction, this attribute leaks into the processing of the second return instruction (`bb2`). As a result, the second inlined call incorrectly receives the combination of its own attribute, the call site's attribute, AND the leaked attribute from the first call (`nofpclass(nan inf zero)` instead of just `nofpclass(inf zero)`).

**Fix Weakness Analysis:**
The weakness in the fix lies in the stateful nature of the `ValidPG` `AttrBuilder` within the loop over the callee's basic blocks. The patch added support for accumulating `nofpclass` attributes in `ValidPG` (similar to how `range` attributes were already being handled, which also suffers from the same bug). 

```cpp
      Attribute CBNoFPClass = ValidPG.getAttribute(Attribute::NoFPClass);
      if (CBNoFPClass.isValid() && AL.hasRetAttr(Attribute::NoFPClass)) {
        ValidPG.addNoFPClassAttr(
            CBNoFPClass.getNoFPClass() |
            AL.getRetAttr(Attribute::NoFPClass).getNoFPClass());
      }
```

Because `ValidPG` is declared outside the loop and modified inside the loop, any attributes added to it during the processing of one return instruction will persist and be applied to all subsequent return instructions processed in the same inlining operation. This leads to overly restrictive attributes being applied to the inlined calls, which can cause valid programs to exhibit undefined behavior (UB) after optimization. The provided test case reveals this weakness by demonstrating that the attributes applied to the second inlined call are incorrectly influenced by the attributes of the first inlined call.

cc @arsenm 

## Comments

### Comment 1 - cardigan1008

I can propose a patch for it, please assign it to me. Thanks.

---

