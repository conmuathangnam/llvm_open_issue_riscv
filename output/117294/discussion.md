# [Clang] Placement-new in constant evaluation can give indeterminate result

**Author:** frederick-vs-ja
**URL:** https://github.com/llvm/llvm-project/issues/117294

## Body

The following program shouldn't be accepted per [[expr.const]/5.18.2.1](https://eel.is/c++draft/expr.const#5.18.2.1) as corrected by [CWG2922](https://cplusplus.github.io/CWG/issues/2922.html) (since the constructed array type is unsuitable).

However, Clang currently accepts it and give different results in different compilation. [Godbolt link](https://godbolt.org/z/asjev19vj).

```C++
#include <new>
#include <typeinfo>

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1][2][3][4]();
    return s.a[0];
}();

template<int X>
struct tag {};

void unknown(const std::type_info&) noexcept;

int main()
{
    unknown(typeid(tag<N>));
}
```

Moreover, the following seems valid because the array type is suitable, but there are still indeterminate results. [Godbolt link](https://godbolt.org/z/5bPfns185).
(The correct `N` is `0`.)

```C++
#include <new>
#include <typeinfo>

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1](); // s.a is converted to the poiner to s.a[0]
    return s.a[0];
}();

template<int X>
struct tag {};

void unknown(const std::type_info&) noexcept;

int main()
{
    unknown(typeid(tag<N>));
}
```

This can be also trigged by the current resolution of [LWG3436](https://cplusplus.github.io/LWG/issue3436) (so labeled `c++20`).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: A. Jiang (frederick-vs-ja)

<details>
The following program shouldn't be accepted per [[expr.const]/5.18.2.1](https://eel.is/c++draft/expr.const#<!-- -->5.18.2.1) as corrected by [CWG2922](https://cplusplus.github.io/CWG/issues/2922.html) (since the constructed array type is unsuitable).

However, Clang currently accepts it and give different results in different compilation. [Godbolt link](https://godbolt.org/z/asjev19vj).

```C++
#include &lt;new&gt;
#include &lt;typeinfo&gt;

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1][2][3][4]();
    return s.a[0];
}();

template&lt;int X&gt;
struct tag {};

void unknown(const std::type_info&amp;) noexcept;

int main()
{
    unknown(typeid(tag&lt;N&gt;));
}
```

Moreover, the following seems valid because the array type is suitable, but there are still indeterminate results. [Godbolt link](https://godbolt.org/z/5bPfns185).
(The correct `N` is `0`.)

```C++
#include &lt;new&gt;
#include &lt;typeinfo&gt;

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1](); // s.a is converted to the poiner to s.a[0]
    return s.a[0];
}();

template&lt;int X&gt;
struct tag {};

void unknown(const std::type_info&amp;) noexcept;

int main()
{
    unknown(typeid(tag&lt;N&gt;));
}
```

This can be also trigged by the current resolution of [LWG3436](https://cplusplus.github.io/LWG/issue3436) (so labled `c++20`).
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: A. Jiang (frederick-vs-ja)

<details>
The following program shouldn't be accepted per [[expr.const]/5.18.2.1](https://eel.is/c++draft/expr.const#<!-- -->5.18.2.1) as corrected by [CWG2922](https://cplusplus.github.io/CWG/issues/2922.html) (since the constructed array type is unsuitable).

However, Clang currently accepts it and give different results in different compilation. [Godbolt link](https://godbolt.org/z/asjev19vj).

```C++
#include &lt;new&gt;
#include &lt;typeinfo&gt;

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1][2][3][4]();
    return s.a[0];
}();

template&lt;int X&gt;
struct tag {};

void unknown(const std::type_info&amp;) noexcept;

int main()
{
    unknown(typeid(tag&lt;N&gt;));
}
```

Moreover, the following seems valid because the array type is suitable, but there are still indeterminate results. [Godbolt link](https://godbolt.org/z/5bPfns185).
(The correct `N` is `0`.)

```C++
#include &lt;new&gt;
#include &lt;typeinfo&gt;

constexpr int N = []
{
    struct S {
        int a[1];
    };
    S s;
    ::new (s.a) int[1](); // s.a is converted to the poiner to s.a[0]
    return s.a[0];
}();

template&lt;int X&gt;
struct tag {};

void unknown(const std::type_info&amp;) noexcept;

int main()
{
    unknown(typeid(tag&lt;N&gt;));
}
```

This can be also trigged by the current resolution of [LWG3436](https://cplusplus.github.io/LWG/issue3436) (so labled `c++20`).
</details>


---

### Comment 3 - tbaederr

First example runs into an assertion: https://godbolt.org/z/7Gc79nvrj
Second one, too: https://godbolt.org/z/hhh1G1bd1

---

