# LoadStoreVectorizer: `getConstantOffsetComplexAddrs` ignores GEP B's stride

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/186327
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-03-19T16:10:32Z

## Body

The vectorizer silently replaces two semantically independent loads with a single vector load that reads the wrong memory location for one of them.

PoC: [https://godbolt.org/z/cso85Ko5K](https://godbolt.org/z/cso85Ko5K)

## Vulnerable code location(s)

[https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Transforms/Vectorize/LoadStoreVectorizer.cpp#L1470](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Transforms/Vectorize/LoadStoreVectorizer.cpp#L1470) 

## Vulnerable code analysis

`getConstantOffsetComplexAddrs` computes the byte offset between two GEP-based pointers to decide whether their memory accesses are vectorization candidates. At line 1470 only `GTIA`'s stride is fetched:

```cpp
uint64_t Stride = GTIA.getSequentialElementStride(DL);
// GTIB.getSequentialElementStride(DL) is never called
```

The early guard (lines 1452–1453) checks only operand count and base pointer identity; it does not compare source element types. For single-index GEPs the inner loop runs zero times (`E = 0`), so no per-type check ever executes.

**Trigger** (`poc.ll`):
```
%gep_a = getelementptr i8,  ptr %p, sext i32 %i to i64      ; stride = 1
%gep_b = getelementptr i64, ptr %p, sext i32 (%i+1) to i64  ; stride = 8
load i8, ptr %gep_a   ; \  same EqClassKey →
load i8, ptr %gep_b   ; /  vectorization candidates
```

Both loads share the same `EqClassKey` (same base object, address space, element size). The function computes `IdxDiff = 1`, multiplies by **GEP A's** stride of 1, and returns an offset of 1 byte. The real offset is `7×%i + 8` (non-constant). LSV then emits a single `load <2 x i8>` at `%gep_a`, silently reading the wrong address for Load B.

The SCEV-based shortcut in `getConstantOffset` correctly identifies the difference as non-constant and falls through to this broken path, providing no protection.

**Minimal fix** — add after line 1470:
```cpp
uint64_t StrideA = GTIA.getSequentialElementStride(DL);
uint64_t StrideB = GTIB.getSequentialElementStride(DL);
if (StrideA != StrideB)
  return std::nullopt;
```
Or equivalently, add an early check: `if (GEPA->getSourceElementType() != GEPB->getSourceElementType()) return std::nullopt;`

## Comments

### Comment 1 - he-weiwen

Can I take this?

---

