# [LoopVectorize] Wrong code at -O2

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/180781
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-02-12T22:17:57Z

## Body

Reproducer: https://godbolt.org/z/Y6sYGTfxE
Testcase:
```c
#include <stdint.h>
#include <stdio.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
uint64_t BS_CHECKSUM;
int16_t count = 6;
int main()
{
    uint16_t BS_VAR_1 = 40854u;
    BS_VEC(uint64_t, 4) BS_VAR_2 = {};
    uint16_t BS_TEMP_595 = count < 5ull ? count : 5ull;
#pragma clang loop interleave_count(2)
    for (uint16_t BS_INC_0 = 0; BS_INC_0 < BS_TEMP_595; BS_INC_0 += 1)
    {
        BS_VAR_2 &= 0;
        BS_VAR_1 ^= BS_INC_0;
        BS_VAR_2 = (BS_VEC(uint64_t, 4)){ 0, BS_INC_0, 0, 0 };
    }
    BS_CHECKSUM = BS_VAR_1;
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
```

`opt-bisect` points to `LoopVectorize`.

## Comments

### Comment 1 - fhahn

Should be working as expected again: https://godbolt.org/z/7fKxzqevY

---

