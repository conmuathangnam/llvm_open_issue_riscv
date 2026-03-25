# ICE: is_invocable_v (access via :: of non static member)

**Author:** bernd5
**URL:** https://github.com/llvm/llvm-project/issues/46463
**Status:** Closed
**Labels:** c++17, clang:frontend, bugzilla, crash
**Closed Date:** 2024-12-03T07:04:30Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [47119](https://llvm.org/bz47119) |
| Version | trunk |
| OS | Windows NT |
| Attachments | [compiler output](https://user-images.githubusercontent.com/95090604/143761087-35d06837-3de0-4fe0-84a8-c2b382c98177.gz) |
| CC | @DougGregor,@bernd5,@ojeda,@zygoloid |

## Extended Description 


Clang++ front-end crashes with the following code:

Compilation command: clang++ -std=c++17 sample.cpp

See here on godbolt: https://godbolt.org/z/o1xjK8

Code:
=================
```
#include <type_traits>

struct S {
    void Func() {
    }
};

#include <iostream>
int main()
{
    auto lambda = [](const auto& cls) -> decltype(
        std::remove_const_t<std::remove_reference_t<decltype(cls)>>::Func()
    ) {};

    auto isInv = std::is_invocable_v<decltype(lambda), S>;
    if (!isInv){
        std::cout << "detected correctly";
    }
    return 0;
}
```

## Comments

### Comment 1 - bernd5

I posted some path - but please review the code.
I think that there is maybe something missing...

https://reviews.llvm.org/D85791

---

### Comment 2 - bernd5

To avoid the included code, it can be written like this:

template<class... >
using void_t = void;

template<class T>
T&& declval() noexcept;
	
auto l = [](auto s) -> decltype(decltype(s)::f()) {};

template<class, class = void_t<>>
struct has_f_fkt{ 
    constexpr static bool value = false;
};
 
template<class T >
struct has_f_fkt<T, void_t<decltype(l(declval<T>()))>> {
    constexpr static bool value = true;
};

struct A {
    void f() {}
};
struct B {
    static void f() {}
};
static_assert(has_f_fkt<A>::value == false);
static_assert(has_f_fkt<B>::value == true);

---

### Comment 3 - bernd5

But with C++20 it could be reduced to the following test case:


#include <type_traits>
struct S {
   void f() {}
};
constexpr bool hasStaticF = std::is_invocable_v<decltype(
    [](auto s) -> decltype(decltype(s)::f()) {}
), S>;

---

### Comment 4 - bernd5

Nice "minified" test-cases.
Clang should not crash in those cases but emit some error. Because f is not callable with "::".

Here you see a "bigger" sample which implements it twice - via template and via lambda: https://godbolt.org/z/q4W9cq

Currently just the code with the template works with clang.

---

### Comment 5 - ojeda

Also crashes (with a slightly different trace) with:

    struct S {
      void f() {}
    };
    auto l = [](auto s) -> decltype(s)::f() {}(S());

---

### Comment 6 - ojeda

Reduced testcase:

    struct S {
      void f() {}
    };
    auto l = [](auto s) -> decltype(decltype(s)::f()) {}(S());

---

### Comment 7 - bernd5

Link to stackoverflow:

https://stackoverflow.com/questions/63364309/ice-in-clang-with-is-invocable-v-is-my-code-valid

---

### Comment 8 - bernd5

The runtime type is a "FunctionDecl" which is the base class of CXXMethodDecl, but no CXXRecordDecl.

---

### Comment 9 - bernd5

It crashes at SemaExprMember.cpp on line 166 "contextClass = cast<CXXRecordDecl>(DC);"

This cast fails. But the real issue is that isStaticContext has the value false.

---

### Comment 10 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 11 - llvmbot

@llvm/issue-subscribers-c-17

---

### Comment 12 - MagentaTreehouse

The original repro is no longer crashing on trunk.

---

