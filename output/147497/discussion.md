# Clang 20+ dependent type,value Template-Template resolution differs from GCC,MSVC,Clang10-19

**Author:** rainerzufalldererste
**URL:** https://github.com/llvm/llvm-project/issues/147497

## Body

The following C++-20 code produces different (seemingly incorrect) template resolution between Clang 20 and GCC,MSVC,Clang10-19:

```c++
#include <stdio.h>

template <typename T, auto value>
struct different_type_value_templ
{
};

template <typename T, T value>
struct same_type_value_templ
{
};

template <typename... TArgs>
struct resolve;

template <typename T>
void resolve_fallback()
{
    puts("called with 'T' (fallback)");
}

template <template <typename T1, auto V2> typename T, typename T1, auto V2>
struct resolve<T<T1, V2>>
{
    static void handle()
    {
        puts("called with 'T<T1, V2>'");
    }
};

template <typename T>
concept can_handle_type_resolve = requires () {
    resolve<T>::handle();
};

template <typename T>
void do_resolve()
{
    if constexpr (can_handle_type_resolve<T>)
        resolve<T>::handle();
    else
        resolve_fallback<T>();
}

int main()
{
    // consistent:
    do_resolve<different_type_value_templ<int, 1>>();
    do_resolve<different_type_value_templ<bool, false>>();

    // inconsistent:
    do_resolve<same_type_value_templ<int, 1>>();
    do_resolve<same_type_value_templ<bool, false>>();
}
```

Should seemingly produce (as does MSVC, GCC, Clang 10-19.1)
```
called with 'T<T1, V2>'
called with 'T<T1, V2>'
called with 'T<T1, V2>'
called with 'T<T1, V2>'
```

but produces the following on clang 20.1 (w/ `-std=c++20`)
```
called with 'T<T1, V2>'
called with 'T<T1, V2>'
called with 'T' (fallback)
called with 'T' (fallback)
```

See: https://godbolt.org/z/o445cKv4c

## Comments

### Comment 1 - cor3ntin

@mizvekov 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Christoph Stiller (rainerzufalldererste)

<details>
The following C++-20 code produces different (seemingly incorrect) template resolution between Clang 20 and GCC,MSVC,Clang10-19:

```c++
#include &lt;stdio.h&gt;

template &lt;typename T, auto value&gt;
struct different_type_value_templ
{
};

template &lt;typename T, T value&gt;
struct same_type_value_templ
{
};

template &lt;typename... TArgs&gt;
struct resolve;

template &lt;typename T&gt;
void resolve_fallback()
{
    puts("called with 'T' (fallback)");
}

template &lt;template &lt;typename T1, auto V2&gt; typename T, typename T1, auto V2&gt;
struct resolve&lt;T&lt;T1, V2&gt;&gt;
{
    static void handle()
    {
        puts("called with 'T&lt;T1, V2&gt;'");
    }
};

template &lt;typename T&gt;
concept can_handle_type_resolve = requires () {
    resolve&lt;T&gt;::handle();
};

template &lt;typename T&gt;
void do_resolve()
{
    if constexpr (can_handle_type_resolve&lt;T&gt;)
        resolve&lt;T&gt;::handle();
    else
        resolve_fallback&lt;T&gt;();
}

int main()
{
    // consistent:
    do_resolve&lt;different_type_value_templ&lt;int, 1&gt;&gt;();
    do_resolve&lt;different_type_value_templ&lt;bool, false&gt;&gt;();

    // inconsistent:
    do_resolve&lt;same_type_value_templ&lt;int, 1&gt;&gt;();
    do_resolve&lt;same_type_value_templ&lt;bool, false&gt;&gt;();
}
```

Should seemingly produce (as does MSVC, GCC, Clang 10-19.1)
```
called with 'T&lt;T1, V2&gt;'
called with 'T&lt;T1, V2&gt;'
called with 'T&lt;T1, V2&gt;'
called with 'T&lt;T1, V2&gt;'
```

but produces the following on clang 20.1 (w/ `-std=c++20`)
```
called with 'T&lt;T1, V2&gt;'
called with 'T&lt;T1, V2&gt;'
called with 'T' (fallback)
called with 'T' (fallback)
```

See: https://godbolt.org/z/o445cKv4c
</details>


---

### Comment 3 - rainerzufalldererste

This case seems related, but unfortunately there's even less consensus between compilers here and MSVC fails with ICE:

```cpp
#include <stdio.h>
#include <type_traits>
#include <concepts>

template <typename T, auto value>
struct different_type_value_templ
{
};

template <typename T, T value>
struct same_type_value_templ
{
};

template <typename... TArgs>
struct resolve;

template <typename T>
void resolve_fallback()
{
    puts("called with 'T' (fallback)");
}

template <template <typename T1, auto V2> typename T, typename T1, auto V2>
struct resolve<T<T1, V2>>
{
    static void handle()
    {
        puts("called with 'T<T1, auto V2>'");
    }
};

template <template <typename T1, T1 V2> typename T, typename T1, T1 V2>
struct resolve<T<T1, V2>>
{
    static void handle()
    {
        puts("called with 'T<T1, T1 V2>'");
    }
};

template <typename T>
concept can_handle_type_resolve = requires (T t) {
    { resolve<T>::handle() } -> std::same_as<void>;
};

template <typename T>
void do_resolve()
{
    if constexpr (can_handle_type_resolve<T>)
        resolve<T>::handle();
    else
        resolve_fallback<T>();
}

int main()
{
    // consistent:
    do_resolve<different_type_value_templ<int, 1>>();
    do_resolve<different_type_value_templ<bool, false>>();
    do_resolve<different_type_value_templ<const bool, false>>();

    // inconsistent:
    do_resolve<same_type_value_templ<int, 1>>();
    do_resolve<same_type_value_templ<bool, false>>();
    do_resolve<same_type_value_templ<const bool, false>>();
}
```

GCC:
```
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
```

Clang 19:
```
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, auto V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, auto V2>'
```

Clang 20:
```
called with 'T' (fallback)
called with 'T' (fallback)
called with 'T<T1, auto V2>'
called with 'T<T1, T1 V2>'
called with 'T<T1, T1 V2>'
called with 'T' (fallback)
```

The ambiguity of the two options may result in the concept evaluating to false, but there's still a discrepancy between compilers & versions in a similar context here.

https://godbolt.org/z/axfsTGaoP

---

### Comment 4 - mizvekov

The original example is a duplicate of https://github.com/llvm/llvm-project/issues/130778

This is the functional difference between the template parameters being or not being constrained to be the same type.

When we allowed non-exact matches involving template template parameters as a DR in C++17, the design intention is that an argument to a template template parameter is only accepted when all uses of the template template parameter are also valid uses of the template argument (except for the packs on parameter matches non-pack arguments legacy rule).

By accepting the 'same type' case, the other compilers are potentially allowing the template argument to be misused, which can only be diagnosed at template instantiation time and not at definition time. So besides this being less type theoretically correct, this can be harder for other compilers to handle correctly (without crashing for example).

This needs to be solved through a core issue at some point, that's why these bugs are being left open for now, but both me and the committee have limited time available for handling these kinds of papers at the moment.

---

