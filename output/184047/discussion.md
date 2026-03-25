# Clang 22.1.0 crash when evaluating nested requires-clause using concept inside requires-expression

**Author:** msvetkin
**URL:** https://github.com/llvm/llvm-project/issues/184047
**Status:** Closed
**Labels:** clang:frontend, concepts, crash
**Closed Date:** 2026-03-04T13:25:21Z

## Body

Clang 22.1.0 crashes (segmentation fault, exit code 139) while evaluating a concept used inside a nested requires expression.

The crash occurs when a concept is referenced inside a requires { requires ...; } construct, while the same concept works correctly when used directly or aliased through another concept.

Clang 21 compiles this code correctly.
The code also compiles successfully with GCC and MSVC.

Godbolt link: https://godbolt.org/z/GPKW5jcz3
Example:

```cpp
template <typename T, int N>
concept decomposable = requires {
    []<template <typename...> class U, typename... Args>
        requires(sizeof...(Args) >= N)
    (U<Args...>*) {}(static_cast<T*>(nullptr));
};

template<typename T>
struct foo {};

static_assert(decomposable<int, 1> == false);
static_assert(decomposable<foo<int>, 1> == true);

template<typename T>
concept decomposable_works = decomposable<T, 1>;
static_assert(decomposable_works<foo<double>>); // if changed to foo<int>, no crash

template<typename T>
concept decomposable_fails = requires {
    requires decomposable<T, 1>;
};

static_assert(decomposable_fails<foo<int>>); // crash here
```


Additionally, the crash is sensitive to instantiation order:

If this line `static_assert(decomposable_works<foo<double>>);` is changed to `static_assert(decomposable_works<foo<int>>);` then the program compiles successfully even with Clang 22.1.0. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mikhail Svetkin (msvetkin)

<details>
Clang 22.1.0 crashes (segmentation fault, exit code 139) while evaluating a concept used inside a nested requires expression.

The crash occurs when a concept is referenced inside a requires { requires ...; } construct, while the same concept works correctly when used directly or aliased through another concept.

Clang 21 compiles this code correctly.
The code also compiles successfully with GCC and MSVC.

Godbolt link: https://godbolt.org/z/GPKW5jcz3
Example:

```cpp
template &lt;typename T, int N&gt;
concept decomposable = requires {
    []&lt;template &lt;typename...&gt; class U, typename... Args&gt;
        requires(sizeof...(Args) &gt;= N)
    (U&lt;Args...&gt;*) {}(static_cast&lt;T*&gt;(nullptr));
};

template&lt;typename T&gt;
struct foo {};

static_assert(decomposable&lt;int, 1&gt; == false);
static_assert(decomposable&lt;foo&lt;int&gt;, 1&gt; == true);

template&lt;typename T&gt;
concept decomposable_works = decomposable&lt;T, 1&gt;;
static_assert(decomposable_works&lt;foo&lt;double&gt;&gt;); // if changed to foo&lt;int&gt;, no crash

template&lt;typename T&gt;
concept decomposable_fails = requires {
    requires decomposable&lt;T, 1&gt;;
};

static_assert(decomposable_fails&lt;foo&lt;int&gt;&gt;); // crash here
```


Additionally, the crash is sensitive to instantiation order:

If this line `static_assert(decomposable_works&lt;foo&lt;double&gt;&gt;);` is changed to `static_assert(decomposable_works&lt;foo&lt;int&gt;&gt;);` then the program compiles successfully even with Clang 22.1.0. 
</details>


---

### Comment 2 - zyn0217

https://godbolt.org/z/TEcoefhzG

We have some bugs with the lambda context and that we wouldn't check the inner constraint anymore.


---

