# Adding _Nonnull attribute causes template deduction to fail

**Author:** ilya-biryukov
**URL:** https://github.com/llvm/llvm-project/issues/134356
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-04-04T12:23:56Z

## Body

https://gcc.godbolt.org/z/MW94Ezd11

```cpp
template <class T> struct Base {};
template <class T> struct Derived : Base<T> {};

template <class T> void foo(Base<T> *_Nonnull);

template <class T> void bar(Base<T> *);


void test() {
    Derived<int> d;
    foo(&d); // fails.
    bar(&d); // okay.
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ilya Biryukov (ilya-biryukov)

<details>
https://gcc.godbolt.org/z/MW94Ezd11

```cpp
template &lt;class T&gt; struct Base {};
template &lt;class T&gt; struct Derived : Base&lt;T&gt; {};

template &lt;class T&gt; void foo(Base&lt;T&gt; *_Nonnull);

template &lt;class T&gt; void bar(Base&lt;T&gt; *);


void test() {
    Derived&lt;int&gt; d;
    foo(&amp;d); // fails.
    bar(&amp;d); // okay.
}
```
</details>


---

