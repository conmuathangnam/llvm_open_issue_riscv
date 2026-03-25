# The intersection of a concept which isn't satisfied and deducing this crashes clang 19

**Author:** ojrosten
**URL:** https://github.com/llvm/llvm-project/issues/121648
**Status:** Closed
**Labels:** duplicate, clang:frontend, concepts, crash
**Closed Date:** 2025-01-04T17:37:13Z

## Body

The following code crashes the compiler:

```
template<class T>
concept check_for = requires(T t) { check(t); };

struct foo{
    template<class Self>
    void check(this Self& self, int);

    static_assert(check_for<int>);
};
```

https://godbolt.org/z/cTd3bna8G

It looks like this is fixed on trunk. However, I haven't been able to track down what fixed it so thought this bug report may be useful.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Oliver Rosten (ojrosten)

<details>
The following code crashes the compiler:

```
template&lt;class T&gt;
concept check_for = requires(T t) { check(t); };

struct foo{
    template&lt;class Self&gt;
    void check(this Self&amp; self, int);

    static_assert(check_for&lt;int&gt;);
};
```

https://godbolt.org/z/cTd3bna8G

It looks like this is fixed on trunk. However, I haven't been able to track down what fixed it so thought this bug report may be useful.
</details>


---

### Comment 2 - DimitryAndric

This was fixed for #102025, with `llvmorg-20-init-02565-g`7332713b8ee ("[Clang] prevent null explicit object argument from being deduced (#104328)") by @a-tarasyuk CC @cor3ntin @mizvekov.


---

