# Clang does not realize erroneous overload with inherited constructor

**Author:** wangbo15
**URL:** https://github.com/llvm/llvm-project/issues/121331

## Body

The following code is rejected by GCC, MSVC, and EDG but accepted by Clang:

```c++
struct C
{
    C(int = 1){}
};

struct D
{
  D (C);
};

struct A : D
{
  A (const C & n) : D (C{100}) {}
  using D:: D;
};

int main(){
    A a=C{};
}
```

As the diagnostic shows, the user-defined ctor `A(const C & n)` of class `A` has the same parameter list as the inherited ctor, which leads to ambiguity:

```c++
<source>: In function 'int main()':
<source>:18:9: error: conversion from 'C' to 'A' is ambiguous
   18 |     A a=C{};
      |         ^~~
<source>:18:9: note: there are 6 candidates
<source>:8:3: note: candidate 1: 'D::D(C)'
    8 |   D (C);
      |   ^
<source>:14:13: note: inherited here
   14 |   using D:: D;
      |             ^
<source>:13:3: note: candidate 2: 'A::A(const C&)'
   13 |   A (const C & n) : D (C{100}) {}
```

Please see https://godbolt.org/z/vG9cqvMbc

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-1

Author: Bo Wang (wangbo15)

<details>
The following code is rejected by GCC, MSVC, and EDG but accepted by Clang:

```c++
struct C
{
    C(int = 1){}
};

struct D
{
  D (C);
};

struct A : D
{
  A (const C &amp; n) : D (C{100}) {}
  using D:: D;
};

int main(){
    A a=C{};
}
```

As the diagnostic shows, the user-defined ctor `A(const C &amp; n)` of class `A` has the same parameter list as the inherited ctor, which leads to ambiguity:

```c++
&lt;source&gt;: In function 'int main()':
&lt;source&gt;:18:9: error: conversion from 'C' to 'A' is ambiguous
   18 |     A a=C{};
      |         ^~~
&lt;source&gt;:18:9: note: there are 6 candidates
&lt;source&gt;:8:3: note: candidate 1: 'D::D(C)'
    8 |   D (C);
      |   ^
&lt;source&gt;:14:13: note: inherited here
   14 |   using D:: D;
      |             ^
&lt;source&gt;:13:3: note: candidate 2: 'A::A(const C&amp;)'
   13 |   A (const C &amp; n) : D (C{100}) {}
```

Please see https://godbolt.org/z/vG9cqvMbc
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Bo Wang (wangbo15)

<details>
The following code is rejected by GCC, MSVC, and EDG but accepted by Clang:

```c++
struct C
{
    C(int = 1){}
};

struct D
{
  D (C);
};

struct A : D
{
  A (const C &amp; n) : D (C{100}) {}
  using D:: D;
};

int main(){
    A a=C{};
}
```

As the diagnostic shows, the user-defined ctor `A(const C &amp; n)` of class `A` has the same parameter list as the inherited ctor, which leads to ambiguity:

```c++
&lt;source&gt;: In function 'int main()':
&lt;source&gt;:18:9: error: conversion from 'C' to 'A' is ambiguous
   18 |     A a=C{};
      |         ^~~
&lt;source&gt;:18:9: note: there are 6 candidates
&lt;source&gt;:8:3: note: candidate 1: 'D::D(C)'
    8 |   D (C);
      |   ^
&lt;source&gt;:14:13: note: inherited here
   14 |   using D:: D;
      |             ^
&lt;source&gt;:13:3: note: candidate 2: 'A::A(const C&amp;)'
   13 |   A (const C &amp; n) : D (C{100}) {}
```

Please see https://godbolt.org/z/vG9cqvMbc
</details>


---

### Comment 3 - zygoloid

It looks like Clang implements a pre-standard version of [[over.match.best.general]/2.7](https://eel.is/c++draft/over.match.best#general-2.7) without the "all parameters have the same type" portion. There's a [FIXME for this here](https://github.com/llvm/llvm-project/blob/0897373f1a329a7a02f8ce3c501a05d2f9c89390/clang/lib/Sema/SemaOverload.cpp#L10589).

---

### Comment 4 - Fedr

It looks similar to https://github.com/llvm/llvm-project/issues/53975

---

### Comment 5 - plopresti

This is at the root of https://github.com/tensorflow/tensorflow/issues/84977

The code in Tensorflow is erroneous, but Clang accepted it so nobody noticed.


---

