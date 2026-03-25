# Compare operator not defined for recursive data types on C++20

**Author:** zasdfgbnm
**URL:** https://github.com/llvm/llvm-project/issues/67056

## Body

Cross-posting https://gcc.gnu.org/bugzilla/show_bug.cgi?id=111504

The following code works on C++17 but not C++20:

```C++
#include <cstdint>
#include <type_traits>
#include <vector>

template <typename T1, typename T2>
static auto hasLessThanHelper(int)
    -> decltype(std::declval<T1>() < std::declval<T2>(), std::true_type{});

template <typename, typename>
static auto hasLessThanHelper(long) -> std::false_type;

template <typename T1, typename T2>
struct hasLessThan : decltype(hasLessThanHelper<T1, T2>(0)) {};

struct DynamicType {
  using T1 = int64_t;
  using T2 = std::vector<DynamicType>;
};

template <
    typename DT,
    typename = std::enable_if_t<
        (hasLessThan<typename DT::T1, typename DT::T1>::value ||
         hasLessThan<typename DT::T1, typename DT::T2>::value ||
         hasLessThan<typename DT::T2, typename DT::T1>::value ||
         hasLessThan<typename DT::T2, typename DT::T2>::value)>>
inline constexpr bool operator<(const DT& x, const DT& y) {
  // implementation omitted
  return true;
}

int main() {
  using DT = DynamicType;
  // This assert works on C++17, but fails on C++20
  static_assert(hasLessThan<std::vector<DT>, std::vector<DT>>::value);
}
```

## Comments

### Comment 1 - shafik

Implementations on consistent on rejecting this for C++20: https://godbolt.org/z/n9a9rqvhf

and consistent on accepting it or C++17: https://godbolt.org/z/TEosMb6e5

So, that indicates it is likely purposeful but I have to dig some more to understand why.

CC @erichkeane 

---

### Comment 2 - shafik

So it looks like the problem is this expression: https://godbolt.org/z/x9svWvTPv

```cpp
decltype(std::declval<std::vector<DT>>() < std::declval<std::vector<DT>>());
```

If we change the code like this: https://godbolt.org/z/9hccvdTes

```cpp
template <typename T1, typename T2>
static auto hasLessThanHelper(int)
    //-> decltype(std::declval<T1>() < std::declval<T2>(), std::true_type{});
    -> std::true_type;
```

it stops failing. It looks like during synthesizing the spaceship operator it ends up in a recursion and fails. 

---

### Comment 3 - zasdfgbnm

> It looks like during synthesizing the spaceship operator it ends up in a recursion and fails.

Yeah, looks like it. But I don't expect this to be a recursion because in the `enable_if` of `operator<`, the first condition is `hasLessThan<typename DT::T1, typename DT::T1>::value` which is just `hasLessThan<int64_t, int64_t>::value` which is a `true`, then `true || something1 || something2 || something3` should always be `true` and there is no need to determine the value of `something1`, `something2`, or `something3`.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

<details>
Cross-posting https://gcc.gnu.org/bugzilla/show_bug.cgi?id=111504

The following code works on C++17 but not C++20:

```C++
#include &lt;cstdint&gt;
#include &lt;type_traits&gt;
#include &lt;vector&gt;

template &lt;typename T1, typename T2&gt;
static auto hasLessThanHelper(int)
    -&gt; decltype(std::declval&lt;T1&gt;() &lt; std::declval&lt;T2&gt;(), std::true_type{});

template &lt;typename, typename&gt;
static auto hasLessThanHelper(long) -&gt; std::false_type;

template &lt;typename T1, typename T2&gt;
struct hasLessThan : decltype(hasLessThanHelper&lt;T1, T2&gt;(0)) {};

struct DynamicType {
  using T1 = int64_t;
  using T2 = std::vector&lt;DynamicType&gt;;
};

template &lt;
    typename DT,
    typename = std::enable_if_t&lt;
        (hasLessThan&lt;typename DT::T1, typename DT::T1&gt;::value ||
         hasLessThan&lt;typename DT::T1, typename DT::T2&gt;::value ||
         hasLessThan&lt;typename DT::T2, typename DT::T1&gt;::value ||
         hasLessThan&lt;typename DT::T2, typename DT::T2&gt;::value)&gt;&gt;
inline constexpr bool operator&lt;(const DT&amp; x, const DT&amp; y) {
  // implementation omitted
  return true;
}

int main() {
  using DT = DynamicType;
  // This assert works on C++17, but fails on C++20
  static_assert(hasLessThan&lt;std::vector&lt;DT&gt;, std::vector&lt;DT&gt;&gt;::value);
}
```
</details>


---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

<details>
Cross-posting https://gcc.gnu.org/bugzilla/show_bug.cgi?id=111504

The following code works on C++17 but not C++20:

```C++
#include &lt;cstdint&gt;
#include &lt;type_traits&gt;
#include &lt;vector&gt;

template &lt;typename T1, typename T2&gt;
static auto hasLessThanHelper(int)
    -&gt; decltype(std::declval&lt;T1&gt;() &lt; std::declval&lt;T2&gt;(), std::true_type{});

template &lt;typename, typename&gt;
static auto hasLessThanHelper(long) -&gt; std::false_type;

template &lt;typename T1, typename T2&gt;
struct hasLessThan : decltype(hasLessThanHelper&lt;T1, T2&gt;(0)) {};

struct DynamicType {
  using T1 = int64_t;
  using T2 = std::vector&lt;DynamicType&gt;;
};

template &lt;
    typename DT,
    typename = std::enable_if_t&lt;
        (hasLessThan&lt;typename DT::T1, typename DT::T1&gt;::value ||
         hasLessThan&lt;typename DT::T1, typename DT::T2&gt;::value ||
         hasLessThan&lt;typename DT::T2, typename DT::T1&gt;::value ||
         hasLessThan&lt;typename DT::T2, typename DT::T2&gt;::value)&gt;&gt;
inline constexpr bool operator&lt;(const DT&amp; x, const DT&amp; y) {
  // implementation omitted
  return true;
}

int main() {
  using DT = DynamicType;
  // This assert works on C++17, but fails on C++20
  static_assert(hasLessThan&lt;std::vector&lt;DT&gt;, std::vector&lt;DT&gt;&gt;::value);
}
```
</details>


---

### Comment 6 - frederick-vs-ja

> > It looks like during synthesizing the spaceship operator it ends up in a recursion and fails.
> 
> Yeah, looks like it. But I don't expect this to be a recursion because in the `enable_if` of `operator<`, the first condition is `hasLessThan<typename DT::T1, typename DT::T1>::value` which is just `hasLessThan<int64_t, int64_t>::value` which is a `true`, then `true || something1 || something2 || something3` should always be `true` and there is no need to determine the value of `something1`, `something2`, or `something3`.

I think you can write 
```C++
template <typename DT>
    requires hasLessThan<typename DT::T1, typename DT::T1>::value ||
        hasLessThan<typename DT::T1, typename DT::T2>::value ||
        hasLessThan<typename DT::T2, typename DT::T1>::value ||
        hasLessThan<typename DT::T2, typename DT::T2>::value
inline constexpr bool operator<(const DT& x, const DT& y) {
  // implementation omitted
  return true;
}
```
in C++20 and later modes, which should be short-circuit as expected ([Godbolt link](https://godbolt.org/z/dGKbzWo7Y)) because the constraints are decomposed.

`||` in the first template argument of `enable_if(_t)` doesn't perform short-circuit logic in instantiation, although it is short-circuit in usual expression evaluation.

IIUC, there's currently no bug of implementation on this.

---

