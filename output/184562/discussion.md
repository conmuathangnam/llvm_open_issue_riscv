# [Regression][Concepts] Clang 22 regression: concept default NTTP (g = a::e<f>::h) appears to be reused across instantiations, breaking || short-circuit in constraint checking

**Author:** craffael
**URL:** https://github.com/llvm/llvm-project/issues/184562
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, concepts, confirmed, regression:22
**Closed Date:** 2026-03-08T06:46:46Z

## Body

Reproducer obtained with CReduce (C++20, no headers):

```c++
  namespace a {
  template <typename b> struct c { using d = b; };
  template <typename b> using e = c<b>::d;
  } // namespace a

  template <class f, char g = a::e<f>::h>
  concept i = g < 3 || requires { typename f::template j<3>; };

  struct k { static constexpr char h = 2; };

  struct l {
    static constexpr char h = 3;
    template <char> using j = k;
  };

  template <i f> struct m {
    using n = m<typename f::template j<1>>;
  };

  m<l> n;
```

## How to build
- Fails: clang++-22 -std=c++20 -c repro.cpp
- Works: clang++-21 -std=c++20 -c repro.cpp
- Works: g++-15 -std=c++20 -c repro.cpp

## Actual behavior (clang++-22)
```
<source>:17:13: error: constraints not satisfied for class template 'm' [with f = typename l::template j<1>]
   17 |   using n = m<typename f::template j<1>>;
      |             ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
<source>:19:6: note: in instantiation of template class 'm<l>' requested here
   19 | m<l> n;
      |      ^
<source>:16:11: note: because 'typename l::template j<1>' (aka 'k') does not satisfy 'i'
   16 | template <i f> struct m {
      |           ^
<source>:8:13: note: because ''\x03' < 3' (3 < 3) evaluated to false
    8 | concept i = g < 3 || requires { typename f::template j<3>; };
      |             ^
<source>:8:54: note: and 'typename f::template j<3>' would be invalid: no member named 'j' in 'k'
    8 | concept i = g < 3 || requires { typename f::template j<3>; };
      |                                                      ^
```

## Expected behavior
  This should compile. 

##  Notes / workaround

  - Replacing the default with char g = f::h makes clang++-22 accept the code, so the issue seems tied to the alias chain a::e<f> = c<f>::d in the default NTTP.
  - static_assert(i<k>); by itself passes on clang++-22; the failure only appears in the nested constrained instantiation (m<l> → m<k>).

##  Versions

  - Failing: Ubuntu clang version 22.1.1 (Target: x86_64-pc-linux-gnu)
  - Working: Ubuntu clang version 21.1.8; g++-15 (GCC) 15.2.1
  - OS: Ubuntu 24.04.2 LTS


## Comments

### Comment 1 - frederick-vs-ja

A slightly more readable example ([Godbolt link](https://godbolt.org/z/Gs7o7Y6oe)):
```C++
template <typename T>
struct tid {
  using type = T;
};
template <typename T>
using tid_t = tid<T>::type;

template <class T, int N = ::tid_t<T>::value>
concept req = N < 3 || requires { typename T::template as_two<3>; };

struct two {
  static constexpr int value = 2;
};

struct three {
  static constexpr int value = 3;
  template <int>
  using as_two = two;
};

template <req T>
struct test {
  using type = test<typename T::template as_two<1>>;
};

test<three> x;
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (craffael)

<details>
Reproducer obtained with CReduce (C++20, no headers):

```c++
  namespace a {
  template &lt;typename b&gt; struct c { using d = b; };
  template &lt;typename b&gt; using e = c&lt;b&gt;::d;
  } // namespace a

  template &lt;class f, char g = a::e&lt;f&gt;::h&gt;
  concept i = g &lt; 3 || requires { typename f::template j&lt;3&gt;; };

  struct k { static constexpr char h = 2; };

  struct l {
    static constexpr char h = 3;
    template &lt;char&gt; using j = k;
  };

  template &lt;i f&gt; struct m {
    using n = m&lt;typename f::template j&lt;1&gt;&gt;;
  };

  m&lt;l&gt; n;
```

## How to build
- Fails: clang++-22 -std=c++20 -c repro.cpp
- Works: clang++-21 -std=c++20 -c repro.cpp
- Works: g++-15 -std=c++20 -c repro.cpp

## Actual behavior (clang++-22)
```
&lt;source&gt;:17:13: error: constraints not satisfied for class template 'm' [with f = typename l::template j&lt;1&gt;]
   17 |   using n = m&lt;typename f::template j&lt;1&gt;&gt;;
      |             ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:19:6: note: in instantiation of template class 'm&lt;l&gt;' requested here
   19 | m&lt;l&gt; n;
      |      ^
&lt;source&gt;:16:11: note: because 'typename l::template j&lt;1&gt;' (aka 'k') does not satisfy 'i'
   16 | template &lt;i f&gt; struct m {
      |           ^
&lt;source&gt;:8:13: note: because ''\x03' &lt; 3' (3 &lt; 3) evaluated to false
    8 | concept i = g &lt; 3 || requires { typename f::template j&lt;3&gt;; };
      |             ^
&lt;source&gt;:8:54: note: and 'typename f::template j&lt;3&gt;' would be invalid: no member named 'j' in 'k'
    8 | concept i = g &lt; 3 || requires { typename f::template j&lt;3&gt;; };
      |                                                      ^
```

## Expected behavior
  This should compile. 

##  Notes / workaround

  - Replacing the default with char g = f::h makes clang++-22 accept the code, so the issue seems tied to the alias chain a::e&lt;f&gt; = c&lt;f&gt;::d in the default NTTP.
  - static_assert(i&lt;k&gt;); by itself passes on clang++-22; the failure only appears in the nested constrained instantiation (m&lt;l&gt; → m&lt;k&gt;).

##  Versions

  - Failing: Ubuntu clang version 22.1.1 (Target: x86_64-pc-linux-gnu)
  - Working: Ubuntu clang version 21.1.8; g++-15 (GCC) 15.2.1
  - OS: Ubuntu 24.04.2 LTS

</details>


---

### Comment 3 - shafik

CC @mizvekov 

---

### Comment 4 - mizvekov

Likely to be related to the normalization changes, so CC @cor3ntin @zyn0217 

---

### Comment 5 - shafik

@zyn0217 this bisects to 4154c1863f42d0eaafcc6d405e57fde369a97ef2

---

