# memory leak due to non-destructed object

**Author:** mrussoLuxoft
**URL:** https://github.com/llvm/llvm-project/issues/124306
**Status:** Closed
**Labels:** duplicate, c++, clang:codegen, miscompilation
**Closed Date:** 2025-08-11T16:36:49Z

## Body

This web page:

[cppquiz](https://cppquiz.org/quiz/question/323?result=OK&answer=bcad&did_answer=Answer)

shows an error of Clang about destruction order in case of exception while destructing local objects after having constructed the returned object.

It seems no one reported yet to Clang, is it ?


The code recalls an example from the standard, in [ [except.ctor]-p2 ](https://timsong-cpp.github.io/cppwp/std23/except.ctor#2).

I have elaborated a bit more with some std::cout and some few additional stuff, here: [godbolt](https://godbolt.org/z/a454Ge5oY)



Original code is:

```
struct A { };

struct Y { ~Y() noexcept(false) { throw 0; } };

A f() {
  try {
    A a;
    Y y;
    A b;
    return {};      // #1
  } catch (...) {
  }
  return {};        // #2
}
```

and the problem is that after the exception throw by y-destruction, the returned object constructed in #1 is not destroyed, also determining **a memory leak** as shown on godbolt link.



My code elaboration is:

```
#include <iostream>
#include <stdexcept>

unsigned d = 0;
struct A {
    A(char c) : c_(c), p(new int()) {std::cout << "constr. " << c_ << '\n';}
    ~A() { std::cout << "destr. " << c_ << '\n'; delete p;}
    char c_;
    int *p;
};

struct Y { ~Y() noexcept(false) { throw std::runtime_error(""); } };

A f() {
    try {
        A a('a');
        Y y;
        A b('b');
        return {'c'};
    } catch (...) {
    }
    return {'d'};
}

int main()
{
    A x = f();
}
```

I also thank Vincenzo Simone to have shared that quiz link and the fact that compilers fail.

## Comments

### Comment 1 - keinflue

Duplicate of https://github.com/llvm/llvm-project/issues/12658.

And for reference: This is [defect report/CWG issue 2176](https://cplusplus.github.io/CWG/issues/2176.html), currently listed as status "Unknown" at https://clang.llvm.org/cxx_dr_status.html.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-1

Author: None (mrussoLuxoft)

<details>
This web page:

[cppquiz](https://cppquiz.org/quiz/question/323?result=OK&amp;answer=bcad&amp;did_answer=Answer)

shows an error of Clang about destruction order in case of exception while destructing local objects after having constructed the returned object.

It seems no one reported yet to Clang, is it ?


The code recalls an example from the standard, in [ [except.ctor]-p2 ](https://timsong-cpp.github.io/cppwp/std23/except.ctor#<!-- -->2).

I have elaborated a bit more with some std::cout and some few additional stuff, here: [godbolt](https://godbolt.org/z/a454Ge5oY)



Original code is:

```
struct A { };

struct Y { ~Y() noexcept(false) { throw 0; } };

A f() {
  try {
    A a;
    Y y;
    A b;
    return {};      // #<!-- -->1
  } catch (...) {
  }
  return {};        // #<!-- -->2
}
```

and the problem is that after the exception throw by y-destruction, the returned object constructed in #<!-- -->1 is not destroyed, also determining **a memory leak** as shown on godbolt link.



My code elaboration is:

```
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

unsigned d = 0;
struct A {
    A(char c) : c_(c), p(new int()) {std::cout &lt;&lt; "constr. " &lt;&lt; c_ &lt;&lt; '\n';}
    ~A() { std::cout &lt;&lt; "destr. " &lt;&lt; c_ &lt;&lt; '\n'; delete p;}
    char c_;
    int *p;
};

struct Y { ~Y() noexcept(false) { throw std::runtime_error(""); } };

A f() {
    try {
        A a('a');
        Y y;
        A b('b');
        return {'c'};
    } catch (...) {
    }
    return {'d'};
}

int main()
{
    A x = f();
}
```

I also thank Vincenzo Simone to have shared that quiz link and the fact that compilers fail.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (mrussoLuxoft)

<details>
This web page:

[cppquiz](https://cppquiz.org/quiz/question/323?result=OK&amp;answer=bcad&amp;did_answer=Answer)

shows an error of Clang about destruction order in case of exception while destructing local objects after having constructed the returned object.

It seems no one reported yet to Clang, is it ?


The code recalls an example from the standard, in [ [except.ctor]-p2 ](https://timsong-cpp.github.io/cppwp/std23/except.ctor#<!-- -->2).

I have elaborated a bit more with some std::cout and some few additional stuff, here: [godbolt](https://godbolt.org/z/a454Ge5oY)



Original code is:

```
struct A { };

struct Y { ~Y() noexcept(false) { throw 0; } };

A f() {
  try {
    A a;
    Y y;
    A b;
    return {};      // #<!-- -->1
  } catch (...) {
  }
  return {};        // #<!-- -->2
}
```

and the problem is that after the exception throw by y-destruction, the returned object constructed in #<!-- -->1 is not destroyed, also determining **a memory leak** as shown on godbolt link.



My code elaboration is:

```
#include &lt;iostream&gt;
#include &lt;stdexcept&gt;

unsigned d = 0;
struct A {
    A(char c) : c_(c), p(new int()) {std::cout &lt;&lt; "constr. " &lt;&lt; c_ &lt;&lt; '\n';}
    ~A() { std::cout &lt;&lt; "destr. " &lt;&lt; c_ &lt;&lt; '\n'; delete p;}
    char c_;
    int *p;
};

struct Y { ~Y() noexcept(false) { throw std::runtime_error(""); } };

A f() {
    try {
        A a('a');
        Y y;
        A b('b');
        return {'c'};
    } catch (...) {
    }
    return {'d'};
}

int main()
{
    A x = f();
}
```

I also thank Vincenzo Simone to have shared that quiz link and the fact that compilers fail.
</details>


---

