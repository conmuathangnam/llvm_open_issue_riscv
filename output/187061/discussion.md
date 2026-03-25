# Miscompile after `[VPlan] Support arbitrary predicated early exits`

**Author:** rupprecht
**URL:** https://github.com/llvm/llvm-project/issues/187061
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-03-18T20:26:42Z

## Body

After #182396, the following miscompiles w/ `-O2 -fsanitize=float-cast-overflow -fno-sanitize-recover`:

```cc
#include <climits>
#include <cstdint>
#include <cstdio>

int clamp(float value, int low, int high) {
    if (value <= low) return low;
    if (value >= high) return high;
    return value;
}

uint16_t foo(float value) {
    static constexpr int8_t kExpBits = 5;
    static constexpr uint16_t kExpMask = (1 << kExpBits) - 1;
    static constexpr int8_t kMantissaBits = 11;
    static constexpr uint16_t kMantissaMask = (1 << kMantissaBits) - 1;

    for (int e = -16; e <= 15; e++) {
        int mantissa = e < 0 ? clamp(value * (1 << -e), INT_MIN, INT_MAX)
                             : clamp(value / (1 << e), INT_MIN, INT_MAX);

        if ((mantissa >= -1024) && (mantissa <= 1023))
            return ((e & kExpMask) << kMantissaBits) |
                   (mantissa & kMantissaMask);
    }
    return 0;
}

int main() { printf("Result: %d\n", foo(42.123)); }
```

It should print `Result: 58017`, but now it prints `Result: 42` (it seems to effectively just cast the input float to int). For comparison, gcc agrees on `Result: 58017`.

Live repro: https://compiler-explorer.com/z/rrGz8ofrv

I have not yet reduced this down to the IR level, but maybe seeing the original source provides better context anyway.

One observation is that it no longer miscompiles w/ this change, although logically it should be the same as above:
```cc
        int mantissa = clamp(e < 0 ? value * (1 << -e) : value / (1 << e),
                             INT_MIN, INT_MAX);
```

## Comments

### Comment 1 - fhahn

Thanks, will take a look tomorrow

---

