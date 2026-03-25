# [clang] Generic lambda with type_identity no longer works inside concepts [22-regression]

**Author:** maflcko
**URL:** https://github.com/llvm/llvm-project/issues/163057
**Status:** Closed
**Labels:** clang:frontend, regression, concepts, lambda
**Closed Date:** 2025-10-14T16:01:13Z

## Body

Reproducer:

```
template <class T>
struct type_identity {
    using type = T;
};

template <typename Inner>
struct Cat {};

template <typename T>
concept CatConcept = requires {
    []<class Inner>(type_identity<Cat<Inner>>) {}(type_identity<T>{});
};

template <typename Dummy>
struct Feeder {
    template <CatConcept Dummy2>
    void feed() noexcept {}
};

int main() { Feeder<int>{}.feed<Cat<int>>(); }
```

Error:

```
<source>:20:28: error: no matching member function for call to 'feed'
   20 | int main() { Feeder<int>{}.feed<Cat<int>>(); }
      |              ~~~~~~~~~~~~~~^~~~~~~~~~~~~~
<source>:17:10: note: candidate template ignored: constraints not satisfied [with Dummy2 = Cat<int>]
   17 |     void feed() noexcept {}
      |          ^
<source>:16:15: note: because 'Cat<int>' does not satisfy 'CatConcept'
   16 |     template <CatConcept Dummy2>
      |               ^
<source>:11:5: note: because '[]<class Inner>(type_identity<Cat<Inner>>) {
}(type_identity<T>{})' would be invalid: no matching function for call to object of type '(lambda at <source>:11:5)'
   11 |     []<class Inner>(type_identity<Cat<Inner>>) {}(type_identity<T>{});
      |     ^
1 error generated.
Compiler returned: 1
```

Godbolt: https://godbolt.org/z/bMYnWs6Kx

The code works with clang-21 and GCC.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (maflcko)

<details>
Reproducer:

```
template &lt;class T&gt;
struct type_identity {
    using type = T;
};

template &lt;typename Inner&gt;
struct Cat {};

template &lt;typename T&gt;
concept CatConcept = requires {
    []&lt;class Inner&gt;(type_identity&lt;Cat&lt;Inner&gt;&gt;) {}(type_identity&lt;T&gt;{});
};

template &lt;typename Dummy&gt;
struct Feeder {
    template &lt;CatConcept Dummy2&gt;
    void feed() noexcept {}
};

int main() { Feeder&lt;int&gt;{}.feed&lt;Cat&lt;int&gt;&gt;(); }
```

Error:

```
&lt;source&gt;:20:28: error: no matching member function for call to 'feed'
   20 | int main() { Feeder&lt;int&gt;{}.feed&lt;Cat&lt;int&gt;&gt;(); }
      |              ~~~~~~~~~~~~~~^~~~~~~~~~~~~~
&lt;source&gt;:17:10: note: candidate template ignored: constraints not satisfied [with Dummy2 = Cat&lt;int&gt;]
   17 |     void feed() noexcept {}
      |          ^
&lt;source&gt;:16:15: note: because 'Cat&lt;int&gt;' does not satisfy 'CatConcept'
   16 |     template &lt;CatConcept Dummy2&gt;
      |               ^
&lt;source&gt;:11:5: note: because '[]&lt;class Inner&gt;(type_identity&lt;Cat&lt;Inner&gt;&gt;) {
}(type_identity&lt;T&gt;{})' would be invalid: no matching function for call to object of type '(lambda at &lt;source&gt;:11:5)'
   11 |     []&lt;class Inner&gt;(type_identity&lt;Cat&lt;Inner&gt;&gt;) {}(type_identity&lt;T&gt;{});
      |     ^
1 error generated.
Compiler returned: 1
```

Godbolt: https://godbolt.org/z/bMYnWs6Kx

The code works with clang-21 and GCC.
</details>


---

