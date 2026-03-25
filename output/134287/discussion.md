# Clang accepts ambiguous inherited constructor

**Author:** plopresti
**URL:** https://github.com/llvm/llvm-project/issues/134287
**Status:** Closed
**Labels:** duplicate, clang:frontend
**Closed Date:** 2025-04-03T19:36:53Z

## Body

See https://stackoverflow.com/q/79553477/ and https://stackoverflow.com/a/71197138/

```
class Option;

class Base
{
public:
  explicit Base(Option opt);
};

class Derived : public Base
{
public:
  using Base::Base;
  explicit Derived(const Option &opt);
};

Derived
doit(const Option &opt)
{
  return Derived(opt);
}
```

[Live example on Godbolt](https://godbolt.org/z/ddPr6WYKc)

GCC and ICC reject the call to Derived(opt) as ambiguous. Clang allows it.

According to https://stackoverflow.com/a/71197138/:

> GCC is correct here: there is a tiebreaker that prefers direct over inherited constructors ([over.match.best.general]/2.7), but it applies only if they have the same parameter types (ignoring those whose default arguments are being used).



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (plopresti)

<details>
See https://stackoverflow.com/q/79553477/ and https://stackoverflow.com/a/71197138/

```
class Option;

class Base
{
public:
  explicit Base(Option opt);
};

class Derived : public Base
{
public:
  using Base::Base;
  explicit Derived(const Option &amp;opt);
};

Derived
doit(const Option &amp;opt)
{
  return Derived(opt);
}
```

[Live example on Godbolt](https://godbolt.org/z/ddPr6WYKc)

GCC and ICC reject the call to Derived(opt) as ambiguous. Clang allows it.

According to https://stackoverflow.com/a/71197138/:

&gt; GCC is correct here: there is a tiebreaker that prefers direct over inherited constructors ([over.match.best.general]/2.7), but it applies only if they have the same parameter types (ignoring those whose default arguments are being used).


</details>


---

