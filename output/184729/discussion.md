# Invalid min/max behavior with -O2 following #170223

**Author:** rkirsling
**URL:** https://github.com/llvm/llvm-project/issues/184729
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-03-05T22:39:06Z

## Body

As of https://github.com/llvm/llvm-project/pull/170223, finding the min and max positions of an array, as in [CppPerformanceBenchmark's minmax_sequence.cpp](https://gitlab.com/chriscox/CppPerformanceBenchmarks/-/blob/master/minmax_sequence.cpp), can produce invalid results with `-O2`.

In the context of that benchmark, we see numerous lines of the following sort...
```
test int8_t minmax position sequence2 failed (got 4346 instead of 45)
```
...in which an invalid position has been found for the minimum element.

---

Here is a reduced repro case:
https://godbolt.org/z/xeE3PxoqY

It shouldn't be architecture-specific, but repro is quite sensitive:
- we must calculate the min and max positions together in a single loop, and use both of these
- we must _not_ use the actual min and max values
- we must not hard-code the array

## Comments

### Comment 1 - rkirsling

cc @fhahn @ayalz 

---

### Comment 2 - fhahn

Thanks, taking a look!

---

