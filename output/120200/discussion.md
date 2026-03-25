# [clang] [coroutine] Initialize return value before destruction of coroutine state

**Author:** NewSigma
**URL:** https://github.com/llvm/llvm-project/issues/120200
**Status:** Closed
**Labels:** clang:codegen, miscompilation, coroutines
**Closed Date:** 2025-12-23T01:52:28Z

## Body

Hi, clang community

I want to discuss an unusual usage of coroutine and CRTP. The following code gives the expected result using MSVC, but a unpredictable result using GCC and clang. Intert a printf into final_suspend helps clang give the expected result.

A similar discussion is on [GCC Bugzilla](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118074).

https://godbolt.org/z/5rGjKPhW1

clang version: 16.0.0 ~ trunk
Build option: -std=c++20 -O2
```
#include <cstdio>
#include <utility>
#include <coroutine>

template<class T>
struct CRCoro { // Curiously Recurring Coroutine
    struct RValueWrapper {
        T* p;

        operator T&&() const noexcept { return std::move(*p); }
    };

    using promise_type = T;

    T& getDerived() noexcept { return *static_cast<T*>(this); }

    auto get_return_object() noexcept { return RValueWrapper(&getDerived()); }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept {
        //printf("final_suspend\n");
        return {};
    }
    void return_value(T&& x) noexcept { getDerived() = std::move(x); }
    void unhandled_exception() {}
};

struct A : public CRCoro<A> {
    int a;
};

A func() {
    A aa{};
    aa.a = 5;
    co_return std::move(aa);
}

int main() {
    printf("%d\n", func().a);
    return 0;
}
```
Expected result(MSVC): 5
Possible result of clang: -387938488


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-coroutines

Author: None (NewSigma)

<details>
Hi, clang community

I want to discuss an unusual usage of coroutine and CRTP. The following code gives the expected result using MSVC, but a unpredictable result using GCC and clang. Intert a printf into final_suspend helps clang give the expected result.

A similar discussion is on [GCC Bugzilla](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118074).

https://godbolt.org/z/5rGjKPhW1

clang version: 16.0.0 ~ trunk
Build option: -std=c++20 -O2
```
#include &lt;cstdio&gt;
#include &lt;utility&gt;
#include &lt;coroutine&gt;

template&lt;class T&gt;
struct CRCoro { // Curiously Recurring Coroutine
    struct RValueWrapper {
        T* p;

        operator T&amp;&amp;() const noexcept { return std::move(*p); }
    };

    using promise_type = T;

    T&amp; getDerived() noexcept { return *static_cast&lt;T*&gt;(this); }

    auto get_return_object() noexcept { return RValueWrapper(&amp;getDerived()); }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept {
        //printf("final_suspend\n");
        return {};
    }
    void return_value(T&amp;&amp; x) noexcept { getDerived() = std::move(x); }
    void unhandled_exception() {}
};

struct A : public CRCoro&lt;A&gt; {
    int a;
};

A func() {
    A aa{};
    aa.a = 5;
    co_return std::move(aa);
}

int main() {
    printf("%d\n", func().a);
    return 0;
}
```
Expected result(MSVC): 5
Possible result of clang: -387938488

</details>


---

### Comment 2 - NewSigma

Second repro: https://godbolt.org/z/rz77335K7

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Weibo He (NewSigma)

<details>
Hi, clang community

I want to discuss an unusual usage of coroutine and CRTP. The following code gives the expected result using MSVC, but a unpredictable result using GCC and clang. Intert a printf into final_suspend helps clang give the expected result.

A similar discussion is on [GCC Bugzilla](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118074).

https://godbolt.org/z/5rGjKPhW1

clang version: 16.0.0 ~ trunk
Build option: -std=c++20 -O2
```
#include &lt;cstdio&gt;
#include &lt;utility&gt;
#include &lt;coroutine&gt;

template&lt;class T&gt;
struct CRCoro { // Curiously Recurring Coroutine
    struct RValueWrapper {
        T* p;

        operator T&amp;&amp;() const noexcept { return std::move(*p); }
    };

    using promise_type = T;

    T&amp; getDerived() noexcept { return *static_cast&lt;T*&gt;(this); }

    auto get_return_object() noexcept { return RValueWrapper(&amp;getDerived()); }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept {
        //printf("final_suspend\n");
        return {};
    }
    void return_value(T&amp;&amp; x) noexcept { getDerived() = std::move(x); }
    void unhandled_exception() {}
};

struct A : public CRCoro&lt;A&gt; {
    int a;
};

A func() {
    A aa{};
    aa.a = 5;
    co_return std::move(aa);
}

int main() {
    printf("%d\n", func().a);
    return 0;
}
```
Expected result(MSVC): 5
Possible result of clang: -387938488

</details>


---

