# [Clang] -Winvalid-constexpr misbehaves in C++23 mode

**Author:** frederick-vs-ja
**URL:** https://github.com/llvm/llvm-project/issues/149041
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, constexpr, regression:21
**Closed Date:** 2025-07-17T20:14:35Z

## Body

In C++23/26 mode, Clang rejects the following function definition with `-Winvalid-constexpr` used, saying "constexpr function never produces a constant expression". [Godbolt link](https://godbolt.org/z/WaKPMjorP).
```C++
constexpr bool fun(const bool& b) { return b; }
```

This seems related to [P2280R4](https://wg21.link/p2280r4), but the incorrect diagnostic occurs only since Clang 21.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: A. Jiang (frederick-vs-ja)

<details>
In C++23/26 mode, Clang rejects the following function definition with `-Winvalid-constexpr` used, saying "constexpr function never produces a constant expression". [Godbolt link](https://godbolt.org/z/WaKPMjorP).
```C++
constexpr bool fun(const bool&amp; b) { return b; }
```

This seems related to [P2280R4](https://wg21.link/p2280r4), but the incorrect diagnostic occurs only since Clang 21.
</details>


---

### Comment 2 - cor3ntin

@shafik @efriedma-quic 

---

### Comment 3 - efriedma-quic

-Winvalid-constexpr, formally speaking, shouldn't be a pedantic error in C++23: the standard allows "invalid" constexpr functions.  The implementation of the flag is weird.  We should have two separate diagnostics: the default-error ExtWarn, and the off-by-default C++20-compat warning, and let the diagnostic system do the rest.

That said, there are other reasons to get this right.  Probably we just need to suppress the "read of variable 'b' whose value is not known" diagnostic when we're checking if an expression is potentially-constexpr.

---

