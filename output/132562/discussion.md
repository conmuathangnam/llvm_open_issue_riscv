# [clang][rejects-valid] unfound constrained partial specialization of class template with variadic NTTP of class type

**Author:** ericniebler
**URL:** https://github.com/llvm/llvm-project/issues/132562
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, regression:20
**Closed Date:** 2025-04-07T15:30:53Z

## Body

The following valid C++20 code is rejected by clang trunk:

```c++
struct Integer
{
  int value{};
};

template <Integer... X>
struct Type;

template <Integer... X>
  requires ((X.value == 0) ||...)
struct Type<X...>
{
};
```

result:

```
<source>:11:8: error: class template partial specialization is not more specialized than the primary template [-Winvalid-partial-specialization]
   11 | struct Type<X...>
      |        ^
<source>:7:8: note: template is declared here
    7 | struct Type;
      |        ^
1 error generated.
```

This bug requires the class type to be parameterized on a variadic pack of non-type template parameters of class type.

repro:
https://godbolt.org/z/fYKe1MEeK


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Niebler (ericniebler)

<details>
The following valid C++20 code is rejected by clang trunk:

```c++
struct Integer
{
  int value{};
};

template &lt;Integer... X&gt;
struct Type;

template &lt;Integer... X&gt;
  requires ((X.value == 0) ||...)
struct Type&lt;X...&gt;
{
};
```

result:

```
&lt;source&gt;:11:8: error: class template partial specialization is not more specialized than the primary template [-Winvalid-partial-specialization]
   11 | struct Type&lt;X...&gt;
      |        ^
&lt;source&gt;:7:8: note: template is declared here
    7 | struct Type;
      |        ^
1 error generated.
```

This bug requires the class type to be parameterized on a variadic pack of non-type template parameters of class type.

repro:
https://godbolt.org/z/fYKe1MEeK

</details>


---

### Comment 2 - zyn0217

https://godbolt.org/z/xnGqW84GK

---

### Comment 3 - zyn0217

@mizvekov It thinks deducing against the primary template using the partial specialization type `Type<X...>` is a non-deduced mismatch. This is odd.

```cpp
template <Integer... X>
struct Type;
```

---

### Comment 4 - mizvekov

One clue is that we now correctly diagnose this: https://godbolt.org/z/KfcqWMKx5, while the reverse is still accepted.

Are you looking into this problem, or should I take this?

---

### Comment 5 - zyn0217

No, I'm not. I see there's DeduceTemplateArugments involved so I decided to leave it to you 😄

---

### Comment 6 - mizvekov

This will be fixed by [#134461](https://github.com/llvm/llvm-project/pull/134461)

---

### Comment 7 - mizvekov

Fixed in main, but the patch turned out a bit large. It seems it might not be backported.

---

