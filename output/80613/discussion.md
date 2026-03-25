# llvm crash - segfault during template argument deduction

**Author:** eddieavd
**URL:** https://github.com/llvm/llvm-project/issues/80613
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-03-19T18:29:34Z

## Body

llvm crashes for the following reduced code:

```c++
class a {
  using size_type = long;
  template < typename Self > void at( this Self&&, size_type );
};
template < typename > class b : a{};
template < typename T = int > class d : b< T > {
  using some_type = T;

  some_type f = at( f );
};
d i e
```
if ``some_type`` does not depend on ``T``, llvm reports ``use of underclared identifier 'at'``, no crash

[full preprocessed input](https://github.com/eddieavd/llvm_crash_report_24_2_4/blob/master/repro/main-f875aa.cpp)
[reproducing compile command](https://github.com/eddieavd/llvm_crash_report_24_2_4/blob/master/repro/main-f875aa.sh)

[compiler explorer 1](https://godbolt.org/z/EWe6fP7b7), [compiler explorer 2](https://godbolt.org/z/Kxx3Y34Yq)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (eddieavd)

<details>
llvm crashes for the following reduced code:

```c++
class a {
  using size_type = long;
  template &lt; typename Self &gt; void at( this Self&amp;&amp;, size_type );
};
template &lt; typename &gt; class b : a{};
template &lt; typename T = int &gt; class d : b&lt; T &gt; {
  using some_type = T;

  some_type f = at( f );
};
d i e
```
if ``some_type`` does not depend on ``T``, llvm reports ``use of underclared identifier 'at'``, no crash

[full preprocessed input](https://github.com/eddieavd/llvm_crash_report_24_2_4/blob/master/repro/main-f875aa.cpp)
[reproducing compile command](https://github.com/eddieavd/llvm_crash_report_24_2_4/blob/master/repro/main-f875aa.sh)

[compiler explorer 1](https://godbolt.org/z/EWe6fP7b7), [compiler explorer 2](https://godbolt.org/z/Kxx3Y34Yq)
</details>


---

### Comment 2 - shafik

Looks similar to: https://github.com/llvm/llvm-project/issues/69838

CC @cor3ntin 

---

### Comment 3 - MagentaTreehouse

@shafik Looks like it's no longer crashing.

---

### Comment 4 - shafik

> [@shafik](https://github.com/shafik) Looks like it's no longer crashing.

It looks like it was fixed in clang-20

---

