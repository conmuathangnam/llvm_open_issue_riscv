# False positive error: pack expansion contains parameter pack '_UTypes' that has a different length (1 vs. 2) from outer parameter packs

**Author:** Romain-Geissler-1A
**URL:** https://github.com/llvm/llvm-project/issues/61415
**Status:** Closed
**Labels:** clang:frontend, c++23, false-positive
**Closed Date:** 2024-12-19T05:12:02Z

## Body

Hi,

This is an issue similar #58452, actually the snippet I posted in this issue is the same. Using libstc++'s 13 tuple implementation is still not possible when using `-std=c++2b`:

```cpp
#include <tuple>

void g(std::tuple<std::tuple<int, int>>);

void f() 
{
    g(std::forward_as_tuple<std::tuple<int, int>>({0, 0}));
}
```

Godbolt output (https://godbolt.org/z/Meca376M6):
```console
In file included from <source>:1:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:691:2: error: pack expansion contains parameter pack '_UTypes' that has a different length (1 vs. 2) from outer parameter packs
        using __convertible = __and_<is_convertible<_UTypes, _Types>...>;
        ^~~~~
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:853:27: note: in instantiation of template type alias '__convertible' requested here
          = _TCC<true>::template __convertible<_Args...>::value;
                                 ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:948:12: note: in instantiation of static data member 'std::tuple<std::tuple<int, int> &&>::__convertible<int &, int &>' requested here
        explicit(!__convertible<_UElements&...>)
                  ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:2000:36: note: while substituting deduced template arguments into function template 'tuple' [with _UElements = <int, int>]
    { return tuple<_Elements&&...>(std::forward<_Elements>(__args)...); }
                                   ^
<source>:7:12: note: in instantiation of function template specialization 'std::forward_as_tuple<std::tuple<int, int>>' requested here
    g(std::forward_as_tuple<std::tuple<int, int>>({0, 0}));
           ^
In file included from <source>:1:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:691:2: error: pack expansion contains parameter pack '_UTypes' that has a different length (1 vs. 2) from outer parameter packs
        using __convertible = __and_<is_convertible<_UTypes, _Types>...>;
        ^~~~~
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:853:27: note: in instantiation of template type alias '__convertible' requested here
          = _TCC<true>::template __convertible<_Args...>::value;
                                 ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:959:12: note: in instantiation of static data member 'std::tuple<std::tuple<int, int> &&>::__convertible<const int, const int>' requested here
        explicit(!__convertible<const _UElements...>)
                  ^
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/13.0.1/../../../../include/c++/13.0.1/tuple:2000:36: note: while substituting deduced template arguments into function template 'tuple' [with _UElements = <int, int>]
    { return tuple<_Elements&&...>(std::forward<_Elements>(__args)...); }
                                   ^
<source>:7:12: note: in instantiation of function template specialization 'std::forward_as_tuple<std::tuple<int, int>>' requested here
    g(std::forward_as_tuple<std::tuple<int, int>>({0, 0}));
           ^
2 errors generated.
Compiler returned: 1
```

## Comments

### Comment 1 - shafik

CC @erichkeane 

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-c-2b

---

### Comment 3 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 4 - stbergmann

(might be a duplicate of #17042)

---

### Comment 5 - erichkeane

I'll need a minimal example here, I got lost on the initial one trying to figure out what is going on inside of Tuple.  Also, would be nice to know if perhaps this is just a libc++ bug instead :)

---

### Comment 6 - Romain-Geissler-1A

Ok I will try to reduce it today, so we stop relying on libstc++ headers.

---

### Comment 7 - stbergmann

I think a reduced example exhibiting the same underlying issue is still as discussed at [Pack expansion bug?](https://discourse.llvm.org/t/pack-expansion-bug/64910),
```console
$ cat test.cc
template<typename, typename> struct Pair;
template<typename...> struct Tuple;
template<typename... Ts> struct A {
    template<typename... Us> using B = Tuple<Pair<Ts, Us>...>;
};
template<typename... Ts> struct C {
    template<typename... Us> using D = typename A<Ts...>::template B<Us...>;
};
using E = C<int, int>::D<int, int>;
```
```console
$ clang++ -fsyntax-only test.cc
test.cc:4:30: error: pack expansion contains parameter pack 'Us' that has a different length (2 vs. 1) from outer parameter packs
    template<typename... Us> using B = Tuple<Pair<Ts, Us>...>;
                             ^~~~~
test.cc:7:68: note: in instantiation of template type alias 'B' requested here
    template<typename... Us> using D = typename A<Ts...>::template B<Us...>;
                                                                   ^
test.cc:9:11: note: in instantiation of template class 'C<int, int>' requested here
using E = C<int, int>::D<int, int>;
          ^
1 error generated.
```

---

### Comment 8 - Romain-Geissler-1A

Note, I didn't try to reduce my case since @stbergmann posted his own reduced C++ snippet. Apparently it's not a regression in clang, as it seems clang never accepted this code in any version (checking on Compiler Explorer). In practice, given the updates made on libstdc++ <tuple> side, it makes the just released clang 16 error when using libstdc++ tuples with C++23.

---

### Comment 9 - Romain-Geissler-1A

The problem still happens with clang 17 rc1.

---

### Comment 10 - Romain-Geissler-1A

Note: with gcc 13 now being released, this bug is more likely to be triggered in real life code. Anyone using libstdc++ >= 13 and C++ >= 23 and trying to use maps using std::tuple will trigger this bug.

---

### Comment 11 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/32252

---

### Comment 12 - vient

It triggers in real life code with clang 17.0.1 and libstdc++ 13 indeed.

---

### Comment 13 - Romain-Geissler-1A

Hi,

The original reproducer in the first message of this issue seems to no longer happen with the clang trunk (and is still broken with clang 17, so it's not some hidden clang workaround on libstdc++ side which "fixed" the issue).

However the reduced reproducer from @stbergmann here https://github.com/llvm/llvm-project/issues/61415#issuecomment-1470076987 is still broken right now with clang trunk.

Cheers,
Romain

---

### Comment 14 - Yanpas

Did anyone try clang 18.1?

---

### Comment 15 - Romain-Geissler-1A

No, and Compiler explorer doesn't have clang 18 yet. However it works with clang trunk since some months, so there is no reason it shouldn't work with clang 18 (the reproducer from the first message).

However it seems the reduced case from @stbergmann in https://github.com/llvm/llvm-project/issues/61415#issuecomment-1470076987 is still broken on trunk.

---

### Comment 16 - shafik

I think we should close this issue. 

CC @stbergmann can you confirm if your reduced case is the same as https://github.com/llvm/llvm-project/issues/32252


---

### Comment 17 - vient

Original test case works fine with Clang 18.1.
Both https://github.com/llvm/llvm-project/issues/61415#issuecomment-1470076987 and https://github.com/llvm/llvm-project/issues/32252 still get errors with Clang 18.1 though.

---

### Comment 18 - cor3ntin

@mizvekov 

---

