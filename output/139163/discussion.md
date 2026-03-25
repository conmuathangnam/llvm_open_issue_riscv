# [Clang] expression SFINAE in variable template partial specialization became a hard error

**Author:** yuxuanchen1997
**URL:** https://github.com/llvm/llvm-project/issues/139163
**Status:** Closed
**Labels:** c++14, clang:frontend, diverges-from:gcc, diverges-from:msvc
**Closed Date:** 2025-05-09T17:59:22Z

## Body

After https://github.com/llvm/llvm-project/pull/138731 being landed, clang now rejects the following program:
```cpp
template <class...>
using void_t = void;

template <class T>
T&& declval();

namespace invoke_detail {

template <typename F>
struct traits {
  template <typename... A>
  using result = decltype(declval<F>()(declval<A>()...));
};

template <typename F, typename... A>
using invoke_result_t = typename traits<F>::template result<A...>;

template <typename Void, typename F, typename... A>
inline constexpr bool is_invocable_v = false;

template <typename F, typename... A>
inline constexpr bool
    is_invocable_v<void_t<invoke_result_t<F, A...>>, F, A...> = true;

}

template <typename F, typename... A>
inline constexpr bool is_invocable_v =
    invoke_detail::is_invocable_v<void, F, A...>;

static_assert(!is_invocable_v<int>);
```

We are now treating this as a hard error:
```
<source>:12:27: error: called object type 'int' is not a function or function pointer
   12 |   using result = decltype(declval<F>()(declval<A>()...));
      |                           ^~~~~~~~~~~~
```

while clang-20, gcc, msvc accepted it. This change breaks folly invocability suite, which is used by large amount of currently working code. My understanding is that this code should not be considered ill-formed.

See this on compiler explorer: https://godbolt.org/z/EosEEnMsf

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Yuxuan Chen (yuxuanchen1997)

<details>
After https://github.com/llvm/llvm-project/pull/138731 being landed, clang now rejects the following program:
```cpp
template &lt;class...&gt;
using void_t = void;

template &lt;class T&gt;
T&amp;&amp; declval();

namespace invoke_detail {

template &lt;typename F&gt;
struct traits {
  template &lt;typename... A&gt;
  using result = decltype(declval&lt;F&gt;()(declval&lt;A&gt;()...));
};

template &lt;typename F, typename... A&gt;
using invoke_result_t = typename traits&lt;F&gt;::template result&lt;A...&gt;;

template &lt;typename Void, typename F, typename... A&gt;
inline constexpr bool is_invocable_v = false;

template &lt;typename F, typename... A&gt;
inline constexpr bool
    is_invocable_v&lt;void_t&lt;invoke_result_t&lt;F, A...&gt;&gt;, F, A...&gt; = true;

}

template &lt;typename F, typename... A&gt;
inline constexpr bool is_invocable_v =
    invoke_detail::is_invocable_v&lt;void, F, A...&gt;;

static_assert(!is_invocable_v&lt;int&gt;);
```

We are now treating this as a hard error:
```
&lt;source&gt;:12:27: error: called object type 'int' is not a function or function pointer
   12 |   using result = decltype(declval&lt;F&gt;()(declval&lt;A&gt;()...));
      |                           ^~~~~~~~~~~~
```

while clang-20, gcc, msvc accepted it. This change breaks folly invocability suite, which is used by large amount of currently working code. My understanding is that this code should not be considered ill-formed.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-14

Author: Yuxuan Chen (yuxuanchen1997)

<details>
After https://github.com/llvm/llvm-project/pull/138731 being landed, clang now rejects the following program:
```cpp
template &lt;class...&gt;
using void_t = void;

template &lt;class T&gt;
T&amp;&amp; declval();

namespace invoke_detail {

template &lt;typename F&gt;
struct traits {
  template &lt;typename... A&gt;
  using result = decltype(declval&lt;F&gt;()(declval&lt;A&gt;()...));
};

template &lt;typename F, typename... A&gt;
using invoke_result_t = typename traits&lt;F&gt;::template result&lt;A...&gt;;

template &lt;typename Void, typename F, typename... A&gt;
inline constexpr bool is_invocable_v = false;

template &lt;typename F, typename... A&gt;
inline constexpr bool
    is_invocable_v&lt;void_t&lt;invoke_result_t&lt;F, A...&gt;&gt;, F, A...&gt; = true;

}

template &lt;typename F, typename... A&gt;
inline constexpr bool is_invocable_v =
    invoke_detail::is_invocable_v&lt;void, F, A...&gt;;

static_assert(!is_invocable_v&lt;int&gt;);
```

We are now treating this as a hard error:
```
&lt;source&gt;:12:27: error: called object type 'int' is not a function or function pointer
   12 |   using result = decltype(declval&lt;F&gt;()(declval&lt;A&gt;()...));
      |                           ^~~~~~~~~~~~
```

while clang-20, gcc, msvc accepted it. This change breaks folly invocability suite, which is used by large amount of currently working code. My understanding is that this code should not be considered ill-formed.
</details>


---

### Comment 3 - yfeldblum

Note: the example code in the issue is a direct reduction from the code in folly here: https://github.com/facebook/folly/blob/v2025.05.05.00/folly/functional/Invoke.h#L134-L135 .

We automatically pull the latest commits to llvm and test them against our apps and backend services. This change has broken many of our internal test builds that test against the latest llvm commits.

---

