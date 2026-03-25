# [clang] Incomplete treatment for non-deduced context with nested template

**Author:** frederick-vs-ja
**URL:** https://github.com/llvm/llvm-project/issues/121208

## Body

Currently, Clang doesn't accept the following code snippet, while other implementations do ([demo](https://godbolt.org/z/Yccjc95Eb)).

```C++
template <template <typename> class>
struct B;

struct C {
  template <typename>
  struct Nested;
};

template <typename T>
void f(T*, B<T::template Nested>*);

void g(C *cp, B<C::template Nested>* bcnp) {
  f(cp, 0);
}
```

When `0` is replaced with `{}` (but not `nullptr` or any real null pointer constant), Clang also accept it ([demo](https://godbolt.org/z/19P6MK15P)).

The following similar example is rejected by all implemenations ([demo](https://godbolt.org/z/5oej5ohGT)), and shows that Clang also considers that `T` is not deducible from `B<T::template Nested>*` (and thus `B<T::template Nested>*` forms non-deduced context for `T`). Although the current standard wording is possibly defective on this (see cplusplus/CWG#660).

```C++
template <template <typename> class>
struct B;

struct C {
  template <typename>
  struct Nested;
};

template <typename T>
void f(T*, B<T::template Nested>*);

void g(C *cp, B<C::template Nested>* bcnp) {
  f({}, bcnp);
}
```

Given that `T` is (or should be  considered) in the non-deduced context in `B<T::template Nested>*`, Clang should not try to match `B<T::template Nested>*` against `int` (or `std::nullptr_t`), and should just take `0` (or `nullptr`) as null pointer constant instead.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: A. Jiang (frederick-vs-ja)

<details>
Currently, Clang doesn't accept the following code snippet, while other implementations do ([demo](https://godbolt.org/z/Yccjc95Eb)).

```C++
template &lt;template &lt;typename&gt; class&gt;
struct B;

struct C {
  template &lt;typename&gt;
  struct Nested;
};

template &lt;typename T&gt;
void f(T*, B&lt;T::template Nested&gt;*);

void g(C *cp, B&lt;C::template Nested&gt;* bcnp) {
  f(cp, 0);
}
```

When `0` is replaced with `{}` (but not `nullptr` or any real null pointer constant), Clang also accept it ([demo](https://godbolt.org/z/19P6MK15P)).

The following similar example is rejected by all implemenations ([demo](https://godbolt.org/z/5oej5ohGT)), and shows that Clang also considers that `T` is not deducible from `B&lt;T::template Nested&gt;*` (and thus `B&lt;T::template Nested&gt;*` forms non-deduced context for `T`). Although the current standard wording is possibly defective on this (see cplusplus/CWG#<!-- -->660).

```C++
template &lt;template &lt;typename&gt; class&gt;
struct B;

struct C {
  template &lt;typename&gt;
  struct Nested;
};

template &lt;typename T&gt;
void f(T*, B&lt;T::template Nested&gt;*);

void g(C *cp, B&lt;C::template Nested&gt;* bcnp) {
  f({}, bcnp);
}
```

Given that `T` is (or should be  considered) in the non-deduced context in `B&lt;T::template Nested&gt;*`, Clang should not try to match `B&lt;T::template Nested&gt;*` against `int` (or `std::nullptr_t`), and should just take `0` (or `nullptr`) as null pointer constant instead.
</details>


---

### Comment 2 - hubert-reinterpretcast

> Although the current standard wording is possibly defective on this (see [cplusplus/CWG#660](https://github.com/cplusplus/CWG/issues/660)).

The following case does not rely (in the wording: https://wg21.link/temp.spec.partial.match#3) on the definition of non-deduced context.

```cpp
template <template <typename> class>
struct B;

template <typename T>
struct B<T::template Nested>;
```

It seems both Clang and GCC have a bug here: https://godbolt.org/z/qvfGYhWT4

---

### Comment 3 - shafik

CC @erichkeane 

---

