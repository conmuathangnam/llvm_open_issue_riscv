# [WebAssembly] Assertion failure in isVectorLoadExtDesirable when freeze node is between load and extend

**Author:** abadams
**URL:** https://github.com/llvm/llvm-project/issues/184676
**Status:** Closed
**Labels:** backend:WebAssembly, crash
**Closed Date:** 2026-03-11T02:15:55Z

## Body

This is causing occasional crashes in Halide CI while fuzzing arithmetic operations. The below is the analysis by Claude Code, so take with a grain of salt, but I have verified that the repro does indeed trigger the crash, and the analysis seems plausible to me.

-----

`WebAssemblyTargetLowering::isVectorLoadExtDesirable` unconditionally does `cast<LoadSDNode>(ExtVal->getOperand(0))`, assuming the operand of a vector extend is always a load. However, LLVM's DAG combiner in `tryToFoldExtOfLoad` (DAGCombiner.cpp:14779) correctly handles `freeze` nodes between loads and extends, but when it then calls `isVectorLoadExtDesirable`, the Wasm implementation crashes because the extend's operand is a `freeze` node, not a `LoadSDNode`.

This triggers an assertion failure:
```
cast<Ty>() argument of incompatible type!
```

## Reproducer

```llvm
; RUN: clang --target=wasm32-unknown-unknown -msimd128 -c -x ir %s -o /dev/null
; (or: llc -mtriple=wasm32-unknown-unknown -mattr=+simd128 %s -o /dev/null)

target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

define <4 x i16> @test(ptr %in) {
entry:
  %t = load <4 x i8>, ptr %in, align 1
  %t.fr = freeze <4 x i8> %t
  %w = zext <4 x i8> %t.fr to <4 x i16>
  ret <4 x i16> %w
}
```

The freeze doesn't need to be in the original IR — it's sufficient for LLVM's optimizer to insert one. For example, this also crashes at `-O2` because LLVM inserts a freeze for the poison semantics of `udiv`:

```llvm
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

define void @test(ptr %in, ptr %out) {
entry:
  %t = load <4 x i16>, ptr %in, align 2
  %d = udiv <4 x i16> splat (i16 2), %t
  store <4 x i16> %d, ptr %out, align 2
  ret void
}
```

All three type pairs checked by `isVectorLoadExtDesirable` are affected:
- `<8 x i8>` → `<8 x i16>`
- `<4 x i16>` → `<4 x i32>`
- `<2 x i32>` → `<2 x i64>`

## Root Cause

In `WebAssemblyISelLowering.cpp`:

```cpp
bool WebAssemblyTargetLowering::isVectorLoadExtDesirable(SDValue ExtVal) const {
  EVT ExtT = ExtVal.getValueType();
  EVT MemT = cast<LoadSDNode>(ExtVal->getOperand(0))->getValueType(0);  // BUG
  return (ExtT == MVT::v8i16 && MemT == MVT::v8i8) ||
         (ExtT == MVT::v4i32 && MemT == MVT::v4i16) ||
         (ExtT == MVT::v2i64 && MemT == MVT::v2i32);
}
```

The `cast<LoadSDNode>(ExtVal->getOperand(0))` assumes operand 0 of the extend is always a `LoadSDNode`. But the caller `tryToFoldExtOfLoad` in `DAGCombiner.cpp` (line 14777-14779) already handles the case where a `freeze` node sits between the load and extend:

```cpp
bool Frozen = N0.getOpcode() == ISD::FREEZE;
SDValue Freeze = Frozen ? N0 : SDValue();
auto *Load = dyn_cast<LoadSDNode>(Frozen ? N0.getOperand(0) : N0);
```

So `isVectorLoadExtDesirable` is called with an extend whose operand is a freeze, not a load.

## Suggested Fix

Look through the freeze node, and use `dyn_cast` with a null check:

```cpp
bool WebAssemblyTargetLowering::isVectorLoadExtDesirable(SDValue ExtVal) const {
  EVT ExtT = ExtVal.getValueType();
  SDValue N0 = ExtVal->getOperand(0);
  if (N0.getOpcode() == ISD::FREEZE)
    N0 = N0.getOperand(0);
  auto *LN0 = dyn_cast<LoadSDNode>(N0);
  if (!LN0)
    return false;
  EVT MemT = LN0->getValueType(0);
  return (ExtT == MVT::v8i16 && MemT == MVT::v8i8) ||
         (ExtT == MVT::v4i32 && MemT == MVT::v4i16) ||
         (ExtT == MVT::v2i64 && MemT == MVT::v2i32);
}
```

Alternatively, the `TargetLowering::isVectorLoadExtDesirable` interface documentation could note that the operand may be a freeze rather than a direct load, since the DAGCombiner already handles this case before calling the hook.

## Impact

This bug is triggered by the [Halide](https://github.com/halide/Halide) compiler when targeting WebAssembly with SIMD. Any IR that results in a `freeze` between a vector load and a widening extend will crash during instruction selection. The `freeze` is commonly inserted by LLVM's own optimizer for poison semantics around integer division.

Tracked on the Halide side as https://github.com/halide/Halide/issues/8928.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Andrew Adams (abadams)

<details>
This is causing occasional crashes in Halide CI while fuzzing arithmetic operations. The below is the analysis by Claude Code, so take with a grain of salt, but I have verified that the repro does indeed trigger the crash, and the analysis seems plausible to me.

-----

`WebAssemblyTargetLowering::isVectorLoadExtDesirable` unconditionally does `cast&lt;LoadSDNode&gt;(ExtVal-&gt;getOperand(0))`, assuming the operand of a vector extend is always a load. However, LLVM's DAG combiner in `tryToFoldExtOfLoad` (DAGCombiner.cpp:14779) correctly handles `freeze` nodes between loads and extends, but when it then calls `isVectorLoadExtDesirable`, the Wasm implementation crashes because the extend's operand is a `freeze` node, not a `LoadSDNode`.

This triggers an assertion failure:
```
cast&lt;Ty&gt;() argument of incompatible type!
```

## Reproducer

```llvm
; RUN: clang --target=wasm32-unknown-unknown -msimd128 -c -x ir %s -o /dev/null
; (or: llc -mtriple=wasm32-unknown-unknown -mattr=+simd128 %s -o /dev/null)

target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

define &lt;4 x i16&gt; @<!-- -->test(ptr %in) {
entry:
  %t = load &lt;4 x i8&gt;, ptr %in, align 1
  %t.fr = freeze &lt;4 x i8&gt; %t
  %w = zext &lt;4 x i8&gt; %t.fr to &lt;4 x i16&gt;
  ret &lt;4 x i16&gt; %w
}
```

The freeze doesn't need to be in the original IR — it's sufficient for LLVM's optimizer to insert one. For example, this also crashes at `-O2` because LLVM inserts a freeze for the poison semantics of `udiv`:

```llvm
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

define void @<!-- -->test(ptr %in, ptr %out) {
entry:
  %t = load &lt;4 x i16&gt;, ptr %in, align 2
  %d = udiv &lt;4 x i16&gt; splat (i16 2), %t
  store &lt;4 x i16&gt; %d, ptr %out, align 2
  ret void
}
```

All three type pairs checked by `isVectorLoadExtDesirable` are affected:
- `&lt;8 x i8&gt;` → `&lt;8 x i16&gt;`
- `&lt;4 x i16&gt;` → `&lt;4 x i32&gt;`
- `&lt;2 x i32&gt;` → `&lt;2 x i64&gt;`

## Root Cause

In `WebAssemblyISelLowering.cpp`:

```cpp
bool WebAssemblyTargetLowering::isVectorLoadExtDesirable(SDValue ExtVal) const {
  EVT ExtT = ExtVal.getValueType();
  EVT MemT = cast&lt;LoadSDNode&gt;(ExtVal-&gt;getOperand(0))-&gt;getValueType(0);  // BUG
  return (ExtT == MVT::v8i16 &amp;&amp; MemT == MVT::v8i8) ||
         (ExtT == MVT::v4i32 &amp;&amp; MemT == MVT::v4i16) ||
         (ExtT == MVT::v2i64 &amp;&amp; MemT == MVT::v2i32);
}
```

The `cast&lt;LoadSDNode&gt;(ExtVal-&gt;getOperand(0))` assumes operand 0 of the extend is always a `LoadSDNode`. But the caller `tryToFoldExtOfLoad` in `DAGCombiner.cpp` (line 14777-14779) already handles the case where a `freeze` node sits between the load and extend:

```cpp
bool Frozen = N0.getOpcode() == ISD::FREEZE;
SDValue Freeze = Frozen ? N0 : SDValue();
auto *Load = dyn_cast&lt;LoadSDNode&gt;(Frozen ? N0.getOperand(0) : N0);
```

So `isVectorLoadExtDesirable` is called with an extend whose operand is a freeze, not a load.

## Suggested Fix

Look through the freeze node, and use `dyn_cast` with a null check:

```cpp
bool WebAssemblyTargetLowering::isVectorLoadExtDesirable(SDValue ExtVal) const {
  EVT ExtT = ExtVal.getValueType();
  SDValue N0 = ExtVal-&gt;getOperand(0);
  if (N0.getOpcode() == ISD::FREEZE)
    N0 = N0.getOperand(0);
  auto *LN0 = dyn_cast&lt;LoadSDNode&gt;(N0);
  if (!LN0)
    return false;
  EVT MemT = LN0-&gt;getValueType(0);
  return (ExtT == MVT::v8i16 &amp;&amp; MemT == MVT::v8i8) ||
         (ExtT == MVT::v4i32 &amp;&amp; MemT == MVT::v4i16) ||
         (ExtT == MVT::v2i64 &amp;&amp; MemT == MVT::v2i32);
}
```

Alternatively, the `TargetLowering::isVectorLoadExtDesirable` interface documentation could note that the operand may be a freeze rather than a direct load, since the DAGCombiner already handles this case before calling the hook.

## Impact

This bug is triggered by the [Halide](https://github.com/halide/Halide) compiler when targeting WebAssembly with SIMD. Any IR that results in a `freeze` between a vector load and a widening extend will crash during instruction selection. The `freeze` is commonly inserted by LLVM's own optimizer for poison semantics around integer division.

Tracked on the Halide side as https://github.com/halide/Halide/issues/8928.

</details>


---

### Comment 2 - dschuff

The solution makes sense, and mirrors what other backends like AArch64 do.
I couldn't reproduce claude's udiv crash example and I don't really see how it would trigger this code (there's no zext). But the main example does reproduce and the PR fixes it.

---

