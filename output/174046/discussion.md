# [VectorCombine] `scalarizeExtExtract` miscompiles i64 elements due to `(1ull << 64)`

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/174046
**Status:** Closed
**Labels:** miscompilation, llvm::vectorcombine
**Closed Date:** 2026-01-02T13:02:42Z

## Body

`VectorCombine::scalarizeExtExtract` computes the element mask as:

```cpp
EltBitMask = (1ull << SrcEltSizeInBits) - 1;
```

When `SrcEltSizeInBits == 64` (e.g. `<2 x i64>`), this is C++ UB and can produce `EltBitMask = 0`. As a result, VectorCombine may replace an `extractelement` with constant `0`, causing miscompilation. 

Alive2 proof: https://alive2.llvm.org/ce/z/5-Dyc4

Exposed from https://github.com/llvm/llvm-project/commit/994a6a39e13dcc335247a127a5da05905d1ac541, cc @uyoyo0 

