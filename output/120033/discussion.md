# Compilation Hang Issue with Deeply Nested Template Instantiation in LLVM-18 and Trunk

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/120033

## Body

This hang can be observed in both LLVM 18 and LLVM trunk versions.[https://godbolt.org/z/dKPhr395q](https://godbolt.org/z/dKPhr395q)

# Command 
`clang++ bug.C`

# Code
```cpp
template<template<int N, int M> class>
struct A;

template<class U>
struct B {
  template<class T>
  struct C {
    template<template<auto X, auto Y> class TT>
    using type = A<TT>;
  };
};

template struct B<>;

template<class U>
struct C {
  template<class T>
  struct D : C<typename B<T>::template C<T>::type<int>::template C<T>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type<int>::type>::C<int>;
```
# Version
`clang++ --version`
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
This hang can be observed in both LLVM 18 and LLVM trunk versions.[https://godbolt.org/z/dKPhr395q](https://godbolt.org/z/dKPhr395q)

# Command 
`clang++ bug.C`

# Code
```cpp
template&lt;template&lt;int N, int M&gt; class&gt;
struct A;

template&lt;class U&gt;
struct B {
  template&lt;class T&gt;
  struct C {
    template&lt;template&lt;auto X, auto Y&gt; class TT&gt;
    using type = A&lt;TT&gt;;
  };
};

template struct B&lt;&gt;;

template&lt;class U&gt;
struct C {
  template&lt;class T&gt;
  struct D : C&lt;typename B&lt;T&gt;::template C&lt;T&gt;::type&lt;int&gt;::template C&lt;T&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&lt;int&gt;::type&gt;::C&lt;int&gt;;
```
# Version
`clang++ --version`
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

</details>


---

### Comment 2 - efriedma-quic

Unlikely anyone would run into this outside of fuzzing (so maybe worth fixing, but not high priority).

Reduced:

```
template<class U>
struct B;

template<class T>
struct D : B<T>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
template type<int>::
type {};
```

---

