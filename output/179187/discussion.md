# LLVM miscompiles find_first_of loop on AArch64 with feature +sve2

**Author:** hazzlim
**URL:** https://github.com/llvm/llvm-project/issues/179187
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-02-19T09:07:14Z

## Body

LLVM trunk currently miscompiles find_first_of loops of the form:

```c++
for (; first != last; ++first)
  for (s_it = s_first; s_it != s_last; ++s_it)
    if (*first == *s_it)
      return first;
return last;
```

on AArch64, with feature +sve2, due to incorrect early-exit behaviour in the MATCH loop. This affects the libc++ implementation of std::find_first_of.


The FindFirstByte transformation in LoopIdiomVectorizePass (added by #101976) produces code like the following (https://godbolt.org/z/jhfdbj4nY):

```
        ptrue   p0.b, vl16
.LBB0_4:
        whilelo p1.b, x0, x1
        mov     x8, x2
        mov     x9, x2
        and     p1.b, p0/z, p0.b, p1.b
        ld1b    { z0.b }, p1/z, [x0]
.LBB0_5:
        whilelo p2.b, x8, x3
        and     p2.b, p0/z, p0.b, p2.b
        ld1b    { z1.b }, p2/z, [x9]
        mov     z2.b, b1
        sel     z1.b, p2, z1.b, z2.b
        mov     z1.q, q1
        match   p2.b, p1/z, z0.b, z1.b
        b.ne    .LBB0_8
        add     x9, x9, #16
        add     x8, x8, #16
        cmp     x9, x3
        b.lo    .LBB0_5
        add     x0, x0, #16
        cmp     x0, x1
        b.lo    .LBB0_4
        b       .LBB0_10
.LBB0_8:
        ptrue   p0.b
        brkb    p0.b, p0/z, p2.b
        incp    x0, p0.b
```

The inner loop at .LBB0_5 loops over the needle in 128-bit chunks, using the MATCH instruction to test for any haystack elements which are equal to any of the needle elements. However this will early-exit as soon as the MATCH instruction finds a matching element. If the length of the needle exceeds 128-bits (16B) then we may early-exit before the entire needle has been tested, which may produce an incorrect result.

Given the following needle and haystack:
```
// First element of the haystack array ('a') is present in needle.
const std::array<uint8_t, 16> h = {
    'a','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b'
};

// Needle length > 16, with 'a' only present at index > 16.
const std::array<uint8_t, 32> n = {
    'b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b',
    'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'
};
```

The correct result is the first element ('a'), because this is present in the needle. However because this element appears at index > 16 of the needle array, and because the second element `b` matches in the first 16 bytes of the needle array, we will incorrectly return a result of the second element due to early-exiting without checking the entire needle.

In order to produce the correct result, we need to OR the result of the MATCH instruction on every element of the needle into an 'accumulator', and then use this to test for a match (and find element with BRKB,INCP).

The correct code is something like the following:

```
        ptrue   p0.b, vl16
.LBB0_4:
        whilelo p1.b, x0, x1
        mov     x8, x2
        mov     x9, x2
        and     p1.b, p0/z, p0.b, p1.b
        ld1b    { z0.b }, p1/z, [x0]

// Initially all-false predicate 'accumulator'.
        pfalse  p4.b
.LBB0_5:
        whilelo p2.b, x8, x3
        and     p2.b, p0/z, p0.b, p2.b
        ld1b    { z1.b }, p2/z, [x9]
        mov     z2.b, b1
        sel     z1.b, p2, z1.b, z2.b
        mov     z1.q, q1
        match   p2.b, p1/z, z0.b, z1.b

// Or result of MATCH with accumulator.
        orr     p4.b, p0/z, p2.b, p4.b

        add     x9, x9, #16
        add     x8, x8, #16
        cmp     x9, x3
        b.lo    .LBB0_5

// Test 'accumulator' for early-exit after inner loop.
        ptest   p0, p4.b
        b.ne    .LBB0_8

        add     x0, x0, #16
        cmp     x0, x1
        b.lo    .LBB0_4
        b       .LBB0_10
.LBB0_8:
        ptrue   p0.b
        brkb    p0.b, p0/z, p4.b  // Uses 'accumulator' predicate.
        incp    x0, p0.b
```

The following program will reproduce the bug:

find_first_of_repro.cpp
```c++
#include <array>
#include <algorithm>
#include <iostream>
#include <cstdint>

const uint8_t* find_first_of_byte(const uint8_t* first, const uint8_t* last,
                                  const uint8_t* s_first, const uint8_t* s_last)
{
    for (; first != last; ++first) {
        uint8_t c = *first;
        for (const uint8_t* it = s_first; it != s_last; ++it) {
            if (c == *it)
                return first;
        }
    }
    return last;
}

int main() {
    // First element of the haystack array ('a') is present in needle.
    const std::array<uint8_t, 16> h = {

'a','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b'
    };

    // Needle length > 16, with 'a' only present at index > 16.
    const std::array<uint8_t, 32> n = {
        'b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b',
        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'
    };

    auto inline_it = find_first_of_byte(h.begin(), h.end(), n.begin(), n.end());
    size_t inline_idx = (inline_it == h.end()) ? size_t(-1) : size_t(inline_it - h.begin());
    std::cout << "[Inline impl] Index: " << inline_idx << " (expected 0)" << std::endl;

    auto stl_it = std::find_first_of(h.begin(), h.end(), n.begin(), n.end());
    size_t stl_idx = (stl_it == h.end()) ? size_t(-1) : size_t(stl_it - h.begin());
    std::cout << "[STL impl]    Index: " << stl_idx << " (expected 0)" << std::endl;

    return 0;
}
```

Compiled with no SVE2 the program produces the expected result:
```
> clang++-21 -O3 -march=armv8-a find_first_of_repro.cpp
> ./a.out

  [Inline impl] Index: 0 (expected 0)
  [STL impl]    Index: 0 (expected 0)

```

Compiled with SVE2 the program produces an incorrect result:
```
> clang++-21 -O3 -march=armv8-a+sve2 find_first_of_repro.cpp
> ./a.out

  [Inline impl] Index: 1 (expected 0)
  [STL impl]    Index: 1 (expected 0)

```


## Comments

### Comment 1 - hazzlim

@rj-jesus Tagging you as original author of the patch 

---

### Comment 2 - rj-jesus

Hi @hazzlim, thanks for the report, I'll have a look.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Hari Limaye (hazzlim)

<details>
LLVM trunk currently miscompiles find_first_of loops of the form:

```
for (; first != last; ++first)
  for (s_it = s_first; s_it != s_last; ++s_it)
    if (*first == *s_it)
      return first;
return last;
```

on AArch64, with feature +sve2, due to incorrect early-exit behaviour in the MATCH loop. This affects the libc++ implementation of std::find_first_of.


The FindFirstByte transformation in LoopIdiomVectorizePass (added by #<!-- -->101976) produces code like the following (https://godbolt.org/z/jhfdbj4nY):

```
        ptrue   p0.b, vl16
.LBB0_4:
        whilelo p1.b, x0, x1
        mov     x8, x2
        mov     x9, x2
        and     p1.b, p0/z, p0.b, p1.b
        ld1b    { z0.b }, p1/z, [x0]
.LBB0_5:
        whilelo p2.b, x8, x3
        and     p2.b, p0/z, p0.b, p2.b
        ld1b    { z1.b }, p2/z, [x9]
        mov     z2.b, b1
        sel     z1.b, p2, z1.b, z2.b
        mov     z1.q, q1
        match   p2.b, p1/z, z0.b, z1.b
        b.ne    .LBB0_8
        add     x9, x9, #<!-- -->16
        add     x8, x8, #<!-- -->16
        cmp     x9, x3
        b.lo    .LBB0_5
        add     x0, x0, #<!-- -->16
        cmp     x0, x1
        b.lo    .LBB0_4
        b       .LBB0_10
.LBB0_8:
        ptrue   p0.b
        brkb    p0.b, p0/z, p2.b
        incp    x0, p0.b
```

The inner loop at .LBB0_5 loops over the needle in 128-bit chunks, using the MATCH instruction to test for any haystack elements which are equal to any of the needle elements. However this will early-exit as soon as the MATCH instruction finds a matching element. If the length of the needle exceeds 128-bits (16B) then we may early-exit before the entire needle has been tested, which may produce an incorrect result.

Given the following needle and haystack:
```
// First element of the haystack array ('a') is present in needle.
const std::array&lt;uint8_t, 16&gt; h = {
    'a','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b'
};

// Needle length &gt; 16, with 'a' only present at index &gt; 16.
const std::array&lt;uint8_t, 32&gt; n = {
    'b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b',
    'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'
};
```

The correct result is the first element ('a'), because this is present in the needle. However because this element appears at index &gt; 16 of the needle array, and because the second element `b` matches in the first 16 bytes of the needle array, we will incorrectly return a result of the second element due to early-exiting without checking the entire needle.

In order to produce the correct result, we need to OR the result of the MATCH instruction on every element of the needle into an 'accumulator', and then use this to test for a match (and find element with BRKB,INCP).

The correct code is something like the following:

```
        ptrue   p0.b, vl16
.LBB0_4:
        whilelo p1.b, x0, x1
        mov     x8, x2
        mov     x9, x2
        and     p1.b, p0/z, p0.b, p1.b
        ld1b    { z0.b }, p1/z, [x0]

// Initially all-false predicate 'accumulator'.
        pfalse  p4.b
.LBB0_5:
        whilelo p2.b, x8, x3
        and     p2.b, p0/z, p0.b, p2.b
        ld1b    { z1.b }, p2/z, [x9]
        mov     z2.b, b1
        sel     z1.b, p2, z1.b, z2.b
        mov     z1.q, q1
        match   p2.b, p1/z, z0.b, z1.b

// Or result of MATCH with accumulator.
        orr     p4.b, p0/z, p2.b, p4.b

        add     x9, x9, #<!-- -->16
        add     x8, x8, #<!-- -->16
        cmp     x9, x3
        b.lo    .LBB0_5

// Test 'accumulator' for early-exit after inner loop.
        ptest   p0, p4.b
        b.ne    .LBB0_8

        add     x0, x0, #<!-- -->16
        cmp     x0, x1
        b.lo    .LBB0_4
        b       .LBB0_10
.LBB0_8:
        ptrue   p0.b
        brkb    p0.b, p0/z, p4.b  // Uses 'accumulator' predicate.
        incp    x0, p0.b
```

The following program will reproduce the bug:

find_first_of_repro.cpp
```
#include &lt;array&gt;
#include &lt;algorithm&gt;
#include &lt;iostream&gt;
#include &lt;cstdint&gt;

const uint8_t* find_first_of_byte(const uint8_t* first, const uint8_t* last,
                                  const uint8_t* s_first, const uint8_t* s_last)
{
    for (; first != last; ++first) {
        uint8_t c = *first;
        for (const uint8_t* it = s_first; it != s_last; ++it) {
            if (c == *it)
                return first;
        }
    }
    return last;
}

int main() {
    // First element of the haystack array ('a') is present in needle.
    const std::array&lt;uint8_t, 16&gt; h = {

'a','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b'
    };

    // Needle length &gt; 16, with 'a' only present at index &gt; 16.
    const std::array&lt;uint8_t, 32&gt; n = {
        'b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b',
        'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'
    };

    auto inline_it = find_first_of_byte(h.begin(), h.end(), n.begin(), n.end());
    size_t inline_idx = (inline_it == h.end()) ? size_t(-1) : size_t(inline_it - h.begin());
    std::cout &lt;&lt; "[Inline impl] Index: " &lt;&lt; inline_idx &lt;&lt; " (expected 0)" &lt;&lt; std::endl;

    auto stl_it = std::find_first_of(h.begin(), h.end(), n.begin(), n.end());
    size_t stl_idx = (stl_it == h.end()) ? size_t(-1) : size_t(stl_it - h.begin());
    std::cout &lt;&lt; "[STL impl]    Index: " &lt;&lt; stl_idx &lt;&lt; " (expected 0)" &lt;&lt; std::endl;

    return 0;
}
```

Compiled with no SVE2 the program produces the expected result:
```
&gt; clang++-21 -O3 -march=armv8-a find_first_of_repro.cpp
&gt; ./a.out

  [Inline impl] Index: 0 (expected 0)
  [STL impl]    Index: 0 (expected 0)

```

Compiled with SVE2 the program produces an incorrect result:
```
&gt; clang++-21 -O3 -march=armv8-a+sve2 find_first_of_repro.cpp
&gt; ./a.out

  [Inline impl] Index: 1 (expected 0)
  [STL impl]    Index: 1 (expected 0)

```

</details>


---

### Comment 4 - fhahn

 @hazzlim would be great if you could add such a test to llvm-test-suite!

---

### Comment 5 - hazzlim

> [@hazzlim](https://github.com/hazzlim) would be great if you could add such a test to llvm-test-suite!

Good idea, will do :) 

---

### Comment 6 - hazzlim

@fhahn 
Do you know if there is a way to disable loop-idiom-vectorization via a pragma? Or if it is expected to be disabled by `_Pragma("clang loop vectorize(disable)")`, as it is not currently: https://godbolt.org/z/YEPK6PPE3

---

### Comment 7 - rj-jesus

Not sure if it helps, but you can disable it with `-mllvm -disable-loop-idiom-vectorize-all` (or `-mllvm -disable-loop-idiom-vectorize-find-first-byte` for the specific transformation): https://godbolt.org/z/sYdfKEvo7

---

### Comment 8 - david-arm

> [@fhahn](https://github.com/fhahn) Do you know if there is a way to disable loop-idiom-vectorization via a pragma? Or if it is expected to be disabled by `_Pragma("clang loop vectorize(disable)")`, as it is not currently: https://godbolt.org/z/YEPK6PPE3

It's a target loop optimisation pass and there aren't pragmas for this. As @rj-jesus says you'd have to disable it on the command line I think.

---

### Comment 9 - MacDue

> [@fhahn](https://github.com/fhahn) Do you know if there is a way to disable loop-idiom-vectorization via a pragma? Or if it is expected to be disabled by `_Pragma("clang loop vectorize(disable)")`, as it is not currently: https://godbolt.org/z/YEPK6PPE3

You could add `__attribute__((optnone))` to the function: https://godbolt.org/z/71xqWefKa

---

### Comment 10 - hazzlim

Ah thanks all - I was thinking/hoping there'd be something pragma-shaped that would allow us to toggle it off for the "scalar" version of a function being tested in the llvm-test suite, like here https://github.com/llvm/llvm-test-suite/blob/b93f949ca6c38da7cf2eea708b5a86a4a5c9f30b/SingleSource/UnitTests/Vectorizer/common.h#L69-L85

`__attribute__((optnone))` would work, but seems a bit heavy handed!

---

