# [clang] Clang trunk regression breaks SFINAE with zero-sized array template parameter

**Author:** forworldm
**URL:** https://github.com/llvm/llvm-project/issues/170040
**Status:** Closed
**Labels:** clang:frontend, regression
**Closed Date:** 2025-12-02T10:55:45Z

## Body

```C++
#include <cstdint>
#include <cstddef>

template<int N> struct Foo {
    operator uint16_t() const noexcept requires(N == 2) {
        return 0;
    }

    template<int I = 0, char (*)[(N == 8) || (I & 0)] = nullptr>
    operator uint64_t() const noexcept {
        return 0;
    }
};

int main() {
    Foo<2> foo;
    std::size_t bar = foo;
    return bar;
}
```

clang emits: `error: zero-length arrays are not permitted in C++`.
Earlier versions of Clang (up to 21) compiled the same code without issues.
see [godbolt](https://godbolt.org/z/P67TaT1WP)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: forworldm (forworldm)

<details>
```C++
#include &lt;cstdint&gt;
#include &lt;cstddef&gt;

template&lt;int N&gt; struct Foo {
    operator uint16_t() const noexcept requires(N == 2) {
        return 0;
    }

    template&lt;int I = 0, char (*)[(N == 8) || (I &amp; 0)] = nullptr&gt;
    operator uint64_t() const noexcept {
        return 0;
    }
};

int main() {
    Foo&lt;2&gt; foo;
    std::size_t bar = foo;
    return bar;
}
```

clang emits: `error: zero-length arrays are not permitted in C++`.
Earlier versions of Clang (up to 21) compiled the same code without issues.
see [godbolt](https://godbolt.org/z/P67TaT1WP)
</details>


---

