# [Clang] Error on an `enable_if` attribute in `ADT/StringRef.h` in C++23 mode

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/149188
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, regression:21
**Closed Date:** 2025-07-17T20:14:35Z

## Body

Attempting to create an `llvm::StringLiteral` in C++23 mode currently causes an error. Specifically, the code we complain about is this constructor in `ADT/StringRef.h`:
```c++
    template <size_t N>
    constexpr StringLiteral(const char (&Str)[N])
#if defined(__clang__) && __has_attribute(enable_if)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgcc-compat"
        __attribute((enable_if(__builtin_strlen(Str) == N - 1,
                               "invalid string literal")))
#pragma clang diagnostic pop
#endif
        : StringRef(Str, N - 1) {
    }
```
which can be reduced to (https://godbolt.org/z/xa3zs5qj6):
```c++
template <__SIZE_TYPE__ N>
constexpr void foo(const char (&Str)[N])
__attribute((enable_if(__builtin_strlen(Str), ""))) {}

void x() {
    foo("1234");
}
```
which causes the following error:
```console
<source>:3:14: error: 'enable_if' attribute expression never produces a constant expression
    3 | __attribute((enable_if(__builtin_strlen(Str), ""))) {}
      |              ^
<source>:6:5: note: in instantiation of function template specialization 'foo<5UL>' requested here
    6 |     foo("1234");
      |     ^
<source>:3:41: note: read of variable 'Str' whose value is not known
    3 | __attribute((enable_if(__builtin_strlen(Str), ""))) {}
      |                                         ^
<source>:2:33: note: declared here
    2 | constexpr void foo(const char (&Str)[N])
      |        
```

The `enable_if` attribute has been there since 2016. Interestingly, we compile this just fine in C++20 mode, and Clang 20 doesn’t complain about it at all even in C++23 mode. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
Attempting to create an `llvm::StringLiteral` in C++23 mode currently causes an error. Specifically, the code we complain about is this constructor in `ADT/StringRef.h`:
```c++
    template &lt;size_t N&gt;
    constexpr StringLiteral(const char (&amp;Str)[N])
#if defined(__clang__) &amp;&amp; __has_attribute(enable_if)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgcc-compat"
        __attribute((enable_if(__builtin_strlen(Str) == N - 1,
                               "invalid string literal")))
#pragma clang diagnostic pop
#endif
        : StringRef(Str, N - 1) {
    }
```
which can be reduced to (https://godbolt.org/z/xa3zs5qj6):
```c++
template &lt;__SIZE_TYPE__ N&gt;
constexpr void foo(const char (&amp;Str)[N])
    __attribute((enable_if(__builtin_strlen(Str) == N - 1,
                            "invalid string literal")))
    {}

void x() {
    foo("1234");
}
```
which causes the following error:
```console
&lt;source&gt;:3:14: error: 'enable_if' attribute expression never produces a constant expression
    3 | __attribute((enable_if(__builtin_strlen(Str), ""))) {}
      |              ^
&lt;source&gt;:6:5: note: in instantiation of function template specialization 'foo&lt;5UL&gt;' requested here
    6 |     foo("1234");
      |     ^
&lt;source&gt;:3:41: note: read of variable 'Str' whose value is not known
    3 | __attribute((enable_if(__builtin_strlen(Str), ""))) {}
      |                                         ^
&lt;source&gt;:2:33: note: declared here
    2 | constexpr void foo(const char (&amp;Str)[N])
      |        
```

The `enable_if` attribute has been there since 2016. Interestingly, we compile this just fine in C++20 mode, and Clang 20 doesn’t complain about it at all even in C++23 mode. 
</details>


---

### Comment 2 - Sirraide

CC @efriedma-quic That ‘read of variable 'Str' whose value is not known’ note looks related to something you did recently iirc.

---

### Comment 3 - efriedma-quic

I think this is the same as #149041.

---

### Comment 4 - Sirraide

> I think this is the same as [#149041](https://github.com/llvm/llvm-project/issues/149041).

Ah, I missed that one; yeah, that does seem like it’s the same problem.

---

