# Regression with lambda return type deduction

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/146063
**Status:** Closed
**Labels:** c++17, clang:frontend, regression, rejects-valid, lambda, constexpr
**Closed Date:** 2025-07-03T17:59:19Z

## Body

The following used to compile without diagnostic in Clang 20.1:
```c++
template<class T, class U>
struct is_same {
  static constexpr bool value = false;
};
 
template<class T>
struct is_same<T, T> {
  static constexpr bool value = true;
};

template <typename T, int N>
struct A {
      static_assert([]() constexpr { // error: value of type 'void' is not contextually convertible to 'bool'
        if constexpr (is_same<T, int>::value)
          return true;
        else
          return (10 != N);
      }());
};                                                                                                                                            
int f1() {
    if constexpr (false) {
        A<int, 256> a;
    }
    return 0;
} 
```
but now gives an error on trunk: https://godbolt.org/z/fd5WY4xhY

Note, if you remove the `if constexpr (false)` from `f1()`, then the code compiles. If you add `-> bool` to the lambda, the code also compiles.

The code compiles without diagnostics in GCC, MSVC, and EDG.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-17

Author: Aaron Ballman (AaronBallman)

<details>
The following used to compile without diagnostic in Clang 20.1:
```
template&lt;class T, class U&gt;
struct is_same {
  static constexpr bool value = false;
};
 
template&lt;class T&gt;
struct is_same&lt;T, T&gt; {
  static constexpr bool value = true;
};

template &lt;typename T, int N&gt;
struct A {
      static_assert([]() constexpr { // error: value of type 'void' is not contextually convertible to 'bool'
        if constexpr (is_same&lt;T, int&gt;::value)
          return true;
        else
          return (10 != N);
      }());
};                                                                                                                                            
int f1() {
    if constexpr (false) {
        A&lt;int, 256&gt; a;
    }
    return 0;
} 
```
but now gives an error on trunk: https://godbolt.org/z/fd5WY4xhY

Note, if you remove the `if constexpr (false)` from `f1()`, then the code compiles. If you add `-&gt; bool` to the lambda, the code also compiles.

The code compiles without diagnostics in GCC, MSVC, and EDG.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: Aaron Ballman (AaronBallman)

<details>
The following used to compile without diagnostic in Clang 20.1:
```
template&lt;class T, class U&gt;
struct is_same {
  static constexpr bool value = false;
};
 
template&lt;class T&gt;
struct is_same&lt;T, T&gt; {
  static constexpr bool value = true;
};

template &lt;typename T, int N&gt;
struct A {
      static_assert([]() constexpr { // error: value of type 'void' is not contextually convertible to 'bool'
        if constexpr (is_same&lt;T, int&gt;::value)
          return true;
        else
          return (10 != N);
      }());
};                                                                                                                                            
int f1() {
    if constexpr (false) {
        A&lt;int, 256&gt; a;
    }
    return 0;
} 
```
but now gives an error on trunk: https://godbolt.org/z/fd5WY4xhY

Note, if you remove the `if constexpr (false)` from `f1()`, then the code compiles. If you add `-&gt; bool` to the lambda, the code also compiles.

The code compiles without diagnostics in GCC, MSVC, and EDG.
</details>


---

### Comment 3 - frederick-vs-ja

Slightly simplified ([Godbolt link](https://godbolt.org/z/rdvhrxTbf)):
```C++
template <typename>
struct A {
  static_assert([]() constexpr { return true; }());
};

void f1() {
  if constexpr (false) {
    A<int> a;
  }
}
```

This bug is very similar to #140934. I don't know why #140859 hasn't fixed it.

CC @cor3ntin

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
The following used to compile without diagnostic in Clang 20.1:
```
template&lt;class T, class U&gt;
struct is_same {
  static constexpr bool value = false;
};
 
template&lt;class T&gt;
struct is_same&lt;T, T&gt; {
  static constexpr bool value = true;
};

template &lt;typename T, int N&gt;
struct A {
      static_assert([]() constexpr { // error: value of type 'void' is not contextually convertible to 'bool'
        if constexpr (is_same&lt;T, int&gt;::value)
          return true;
        else
          return (10 != N);
      }());
};                                                                                                                                            
int f1() {
    if constexpr (false) {
        A&lt;int, 256&gt; a;
    }
    return 0;
} 
```
but now gives an error on trunk: https://godbolt.org/z/fd5WY4xhY

Note, if you remove the `if constexpr (false)` from `f1()`, then the code compiles. If you add `-&gt; bool` to the lambda, the code also compiles.

The code compiles without diagnostics in GCC, MSVC, and EDG.
</details>


---

### Comment 5 - efriedma-quic

Reduced more:

```
void f1() {
  if constexpr (false) {
    static_assert([]{});
  }
}
```

---

