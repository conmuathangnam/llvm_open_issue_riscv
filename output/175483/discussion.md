# potentially incorrect `error: invalid application of 'sizeof' to an incomplete type` with `-std=c++23`

**Author:** firewave
**URL:** https://github.com/llvm/llvm-project/issues/175483

## Body

```cpp
#include <memory>

class C
{
public:
    C();
    ~C();

private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

C::C() = default;
C::~C() = default;
struct C::Impl {};
```

This fails with `-std=c++23`:
```
In file included from <source>:1:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/memory:80:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/unique_ptr.h:90:16: error: invalid application of 'sizeof' to an incomplete type 'C::Impl'
   90 |         static_assert(sizeof(_Tp)>0,
      |                       ^~~~~~~~~~~
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/unique_ptr.h:398:4: note: in instantiation of member function 'std::default_delete<C::Impl>::operator()' requested here
  398 |           get_deleter()(std::move(__ptr));
      |           ^
<source>:14:4: note: in instantiation of member function 'std::unique_ptr<C::Impl>::~unique_ptr' requested here
   14 | C::C() = default;
      |    ^
<source>:14:10: note: in defaulted default constructor for 'C' first required here
   14 | C::C() = default;
      |          ^
<source>:10:12: note: forward declaration of 'C::Impl'
   10 |     struct Impl;
      |            ^
```

It builds fine with all previous standards as well as GCC and MSVC using C++23.

https://godbolt.org/z/9rx9j3KhP

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Oliver Stöneberg (firewave)

<details>
```cpp
#include &lt;memory&gt;

class C
{
public:
    C();
    ~C();

private:
    struct Impl;
    std::unique_ptr&lt;Impl&gt; mImpl;
};

C::C() = default;
C::~C() = default;
struct C::Impl {};
```

This fails with `-std=c++23`:
```
In file included from &lt;source&gt;:1:
In file included from /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/memory:80:
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/unique_ptr.h:90:16: error: invalid application of 'sizeof' to an incomplete type 'C::Impl'
   90 |         static_assert(sizeof(_Tp)&gt;0,
      |                       ^~~~~~~~~~~
/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/bits/unique_ptr.h:398:4: note: in instantiation of member function 'std::default_delete&lt;C::Impl&gt;::operator()' requested here
  398 |           get_deleter()(std::move(__ptr));
      |           ^
&lt;source&gt;:14:4: note: in instantiation of member function 'std::unique_ptr&lt;C::Impl&gt;::~unique_ptr' requested here
   14 | C::C() = default;
      |    ^
&lt;source&gt;:14:10: note: in defaulted default constructor for 'C' first required here
   14 | C::C() = default;
      |          ^
&lt;source&gt;:10:12: note: forward declaration of 'C::Impl'
   10 |     struct Impl;
      |            ^
```

It builds fine with all previous standards as well as GCC and MSVC using C++23.

https://godbolt.org/z/9rx9j3KhP
</details>


---

### Comment 2 - frederick-vs-ja

Looks like a duplicate of #59966.

Reduced example ([Godbolt link](https://godbolt.org/z/hsnEzodcW)):
```C++
template<class T>
struct U {
    U() = default;
    constexpr ~U() {
        static_assert(sizeof(T) || true);
    }
};

class C
{
public:
    C();
    ~C();

private:
    struct Impl;
    U<Impl> mImpl;
};

C::C() = default;
C::~C() = default;

struct C::Impl {};
```

---

### Comment 3 - firewave

> Looks like a duplicate of [#59966](https://github.com/llvm/llvm-project/issues/59966).

Except my code compiles with C++20. There is probably more to this.

---

### Comment 4 - frederick-vs-ja

> > Looks like a duplicate of [#59966](https://github.com/llvm/llvm-project/issues/59966).
> 
> Except my code compiles with C++20. There is probably more to this.

The differences are merely due to that member functions of `unique_ptr` are made `constexpr` in C++23 ([P2273R3](https://wg21.link/p2273r3)). The bug (IMO) itself is only related to constexpr destructors which are available since C++20.

---

