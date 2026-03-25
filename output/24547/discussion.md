# Interminable compilation when using `__is_constructible` intrinsic

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/24547

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [24173](https://llvm.org/bz24173) |
| Version | 3.6 |
| OS | MacOS X |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor |

## Extended Description 
The following code causes Clang to never finish compiling, eventually 
exhausting all the memory available on my machine:

```cpp
#include <type_traits>


#if 1
#   define IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#else
#   define IS_CONSTRUCTIBLE(...) is_constructible<__VA_ARGS__>::value
#endif

template <typename ...T>
struct is_constructible
    : std::integral_constant<bool, __is_constructible(T...)>
{ };

template <bool ...b>
struct fast_and
    : std::is_same<fast_and<b...>, fast_and<(b, true)...>>
{ };

template <typename ...Xn>
struct closure {
    template <typename ...Yn, typename = typename std::enable_if<
        fast_and<IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
    >::type>
    explicit constexpr closure(Yn&& ...yn);

    template <typename ...Yn, typename = typename std::enable_if<
        fast_and<IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
    >::type>
    constexpr closure(closure<Yn...>&& other);
};

template <typename ...Xs>
constexpr
closure<std::decay_t<Xs>...> f(Xs&& ...xs)
{ return closure<std::decay_t<Xs>...>{static_cast<Xs&&>(xs)...}; }

int main() {
    f(f(f(f(f(f(f(f(f(f(f(f(1))))))))))));
}
```

Note that using the second definition for `IS_CONSTRUCTIBLE` fixes the problem.
Any clue what is happening?

## Comments

### Comment 1 - frederick-vs-ja

A library-free reproducer ([Godbolt link](https://godbolt.org/z/haYjKPb86))
```C++
#ifdef WORKAROUND
template <class T, class... Args>
struct my_is_constructible {
  static constexpr bool value = __is_constructible(T, Args...);
};
#  define IS_CONSTRUCTIBLE(...) my_is_constructible<__VA_ARGS__>::value
#else
#  define IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#endif

template <class, class>
struct my_is_same {
  static constexpr bool value = false;
};
template <class T>
struct my_is_same<T, T> {
  static constexpr bool value = true;
};

template <bool... Bs>
struct fast_and : my_is_same<fast_and<Bs...>, fast_and<((void)Bs, true)...>> {};

template <bool>
struct my_enable_if {};
template <>
struct my_enable_if<true> {
  using type = void;
};

template <bool B>
using my_enable_if_t = typename my_enable_if<B>::type;

template <class... Xn>
struct closure {
  template <class... Yn, class = my_enable_if_t<fast_and<IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value>>
  explicit closure(Yn&&...);

  template <class... Yn, class = my_enable_if_t<fast_and<IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value>>
  closure(closure<Yn...>&&);
};

template <typename... Xs>
closure<Xs...> f(Xs... xs) {
  return closure<Xs...>{xs...};
}

int main() { f(f(f(f(f(f(f(f(f(f(f(f(1)))))))))))); }
```

It can be observed that other compilers also fail to accept this. The reasons seem very similar.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [24173](https://llvm.org/bz24173) |
| Version | 3.6 |
| OS | MacOS X |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor |

## Extended Description 
The following code causes Clang to never finish compiling, eventually 
exhausting all the memory available on my machine:

```cpp
#include &lt;type_traits&gt;


#if 1
#   define IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#else
#   define IS_CONSTRUCTIBLE(...) is_constructible&lt;__VA_ARGS__&gt;::value
#endif

template &lt;typename ...T&gt;
struct is_constructible
    : std::integral_constant&lt;bool, __is_constructible(T...)&gt;
{ };

template &lt;bool ...b&gt;
struct fast_and
    : std::is_same&lt;fast_and&lt;b...&gt;, fast_and&lt;(b, true)...&gt;&gt;
{ };

template &lt;typename ...Xn&gt;
struct closure {
    template &lt;typename ...Yn, typename = typename std::enable_if&lt;
        fast_and&lt;IS_CONSTRUCTIBLE(Xn, Yn&amp;&amp;)...&gt;::value
    &gt;::type&gt;
    explicit constexpr closure(Yn&amp;&amp; ...yn);

    template &lt;typename ...Yn, typename = typename std::enable_if&lt;
        fast_and&lt;IS_CONSTRUCTIBLE(Xn, Yn&amp;&amp;)...&gt;::value
    &gt;::type&gt;
    constexpr closure(closure&lt;Yn...&gt;&amp;&amp; other);
};

template &lt;typename ...Xs&gt;
constexpr
closure&lt;std::decay_t&lt;Xs&gt;...&gt; f(Xs&amp;&amp; ...xs)
{ return closure&lt;std::decay_t&lt;Xs&gt;...&gt;{static_cast&lt;Xs&amp;&amp;&gt;(xs)...}; }

int main() {
    f(f(f(f(f(f(f(f(f(f(f(f(1))))))))))));
}
```

Note that using the second definition for `IS_CONSTRUCTIBLE` fixes the problem.
Any clue what is happening?
</details>


---

### Comment 3 - frederick-vs-ja

Perhaps I was too rushed to label this as https://github.com/llvm/llvm-project/labels/crash-on-valid because the expected usage of this intrinsic is being wrapped in some library components, and it seems that users are not expected to directly use it.

---

