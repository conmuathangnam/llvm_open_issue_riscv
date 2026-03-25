# [Clang] `constexpr` `std::string` fails to compile

**Author:** zzag
**URL:** https://github.com/llvm/llvm-project/issues/151126
**Status:** Closed
**Labels:** duplicate, clang:frontend, rejects-valid, libstdc++, constexpr
**Closed Date:** 2025-07-29T11:54:09Z

## Body

The following code snippet cannot be compiled with clang and libstdc++. However, it builds fine with gcc.

```cpp
#include <string>

using namespace std::string_literals;

int main()
{
    static_assert(u"🧸"s.size() == 2);
    return 0;
}
```

```
❯ clang++ literal.cpp -std=c++20
literal.cpp:7:19: error: static assertion expression is not an integral constant expression
    7 |     static_assert(u"🧸"s.size() == 2);
      |                   ^~~~~~~~~~~~~~~~~~
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:692:2: note: undefined function '_M_construct<const char16_t *>' cannot be used in a constant expression
  692 |         _M_construct(__s, __s + __n, std::forward_iterator_tag());
      |         ^
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:5006:14: note: in call to 'basic_string(&u"\U0001F9F8"[0], 2, std::allocator<char16_t>())'
 5006 |     { return basic_string<char16_t>{__str, __len}; }
      |              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
literal.cpp:7:19: note: in call to 'operator""s(&u"\U0001F9F8"[0], 2)'
    7 |     static_assert(u"🧸"s.size() == 2);
      |                   ^~~~~~
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:343:9: note: declared here
  343 |         _M_construct(_FwdIterator __beg, _FwdIterator __end,
      |         ^
1 error generated.

~
❯ g++ literal.cpp -std=c++20

~
❯ 
```

Distro: Arch Linux
clang version: 20.1.8 \
gcc and libstdc++ version: 15.1.1

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vlad Zahorodnii (zzag)

<details>
The following code snippet cannot be compiled with clang and libstdc++. However, it builds fine with gcc.

```cpp
#include &lt;string&gt;

using namespace std::string_literals;

int main()
{
    static_assert(u"🧸"s.size() == 2);
    return 0;
}
```

```
❯ clang++ literal.cpp -std=c++20
literal.cpp:7:19: error: static assertion expression is not an integral constant expression
    7 |     static_assert(u"🧸"s.size() == 2);
      |                   ^~~~~~~~~~~~~~~~~~
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:692:2: note: undefined function '_M_construct&lt;const char16_t *&gt;' cannot be used in a constant expression
  692 |         _M_construct(__s, __s + __n, std::forward_iterator_tag());
      |         ^
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:5006:14: note: in call to 'basic_string(&amp;u"\U0001F9F8"[0], 2, std::allocator&lt;char16_t&gt;())'
 5006 |     { return basic_string&lt;char16_t&gt;{__str, __len}; }
      |              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
literal.cpp:7:19: note: in call to 'operator""s(&amp;u"\U0001F9F8"[0], 2)'
    7 |     static_assert(u"🧸"s.size() == 2);
      |                   ^~~~~~
/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/bits/basic_string.h:343:9: note: declared here
  343 |         _M_construct(_FwdIterator __beg, _FwdIterator __end,
      |         ^
1 error generated.

~
❯ g++ literal.cpp -std=c++20

~
❯ 
```

Distro: Arch Linux
clang version: 20.1.8 \
gcc and libstdc++ version: 15.1.1
</details>


---

