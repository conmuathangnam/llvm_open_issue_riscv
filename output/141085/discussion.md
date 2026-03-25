# [OpenMP] OpenMP atomic compare fails when including <array> with `using namespace std;`

**Author:** jhuber6
**URL:** https://github.com/llvm/llvm-project/issues/141085
**Status:** Closed
**Labels:** clang:frontend, clang:openmp
**Closed Date:** 2025-05-29T02:44:56Z

## Body

The following code causes a compilation failure only when `using namespace std;`. https://godbolt.org/z/adKe7x84c.

```c++
using namespace std;

#include <array>

template<typename T>
T atomicMin(T* acc, T value)
{
  T old;
#pragma omp atomic capture compare
  {
    old = *acc;
    if (value < *acc)
    {
      *acc = value;
    }
  }
  return old;
}
```
```console
$ clang++ main.cpp -fopenmp
main.cpp:1:17: warning: using directive refers to implicitly-defined namespace 'std'
    1 | using namespace std;
      |                 ^
main.cpp:12:15: error: the statement for 'atomic compare capture' must be a compound statement of form '{v = x; cond-up-stmt}', ''{cond-up-stmt v = x;}', '{if(x == e) {x = d;} else {v = x;}}', '{r = x == e; if(r) {x = d;}}', or '{r = x == e; if(r) {x = d;} else {v = x;}}', where 'cond-update-stmt' can have one of the following forms: 'if(expr ordop x) {x = expr;}', 'if(x ordop expr) {x = expr;}', 'if(x == e) {x = d;}', or 'if(e == x) {x = d;}' where 'x', 'r', and 'v' are lvalue expressions with scalar type, 'expr', 'e', and 'd' are expressions with scalar type, and 'ordop' is one of '<' or '>'
   12 |     if (value < *acc)
      |         ~~~~~~^~~~~~
main.cpp:12:15: note: expect binary operator in conditional expression
   12 |     if (value < *acc)
      |         ~~~~~~^~~~~~
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Joseph Huber (jhuber6)

<details>
The following code causes a compilation failure only when `using namespace std;`. https://godbolt.org/z/adKe7x84c.

```c++
using namespace std;

#include &lt;array&gt;

template&lt;typename T&gt;
T atomicMin(T* acc, T value)
{
  T old;
#pragma omp atomic capture compare
  {
    old = *acc;
    if (value &lt; *acc)
    {
      *acc = value;
    }
  }
  return old;
}
```
```console
$ clang++ main.cpp -fopenmp
main.cpp:1:17: warning: using directive refers to implicitly-defined namespace 'std'
    1 | using namespace std;
      |                 ^
main.cpp:12:15: error: the statement for 'atomic compare capture' must be a compound statement of form '{v = x; cond-up-stmt}', ''{cond-up-stmt v = x;}', '{if(x == e) {x = d;} else {v = x;}}', '{r = x == e; if(r) {x = d;}}', or '{r = x == e; if(r) {x = d;} else {v = x;}}', where 'cond-update-stmt' can have one of the following forms: 'if(expr ordop x) {x = expr;}', 'if(x ordop expr) {x = expr;}', 'if(x == e) {x = d;}', or 'if(e == x) {x = d;}' where 'x', 'r', and 'v' are lvalue expressions with scalar type, 'expr', 'e', and 'd' are expressions with scalar type, and 'ordop' is one of '&lt;' or '&gt;'
   12 |     if (value &lt; *acc)
      |         ~~~~~~^~~~~~
main.cpp:12:15: note: expect binary operator in conditional expression
   12 |     if (value &lt; *acc)
      |         ~~~~~~^~~~~~
```
</details>


---

### Comment 2 - jhuber6

```c++
using namespace std;
namespace std {
  struct pair operator<(pair, pair);
}
template <typename T> T atomicMin(T *acc, T value) {
  T old;
#pragma omp atomic capture compare
  {
    old = *acc;
    if (value < *acc) {
      *acc = value;
    }
  }

  return old;
}
```
Smaller reproducer, it's caused by importing the `<` operator overload for pairs or any other STL type that has a global `<` operator. What's the possible solution here? Is OpenMP allowed to use user-defined operators for this?

---

