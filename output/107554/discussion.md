# [Clang Diagnostics] When constexpr evaluation fails, the call stack shows the final value of function arguments, even if they are mutated

**Author:** MitalAshok
**URL:** https://github.com/llvm/llvm-project/issues/107554

## Body

Consider <https://godbolt.org/z/ao786daav>:

```c++
constexpr int f(int x) {
    ++x;
    return x / x;
}

constexpr int i = -1;
static_assert(f(i) == 1);
```

It has this diagnostic:

```
<source>:7:15: error: static assertion expression is not an integral constant expression
    7 | static_assert(f(i) == 1);
      |               ^~~~~~~~~
<source>:3:14: note: division by zero
    3 |     return x / x;
      |              ^ ~
<source>:7:15: note: in call to 'f(0)'
    7 | static_assert(f(i) == 1);
      |               ^~~~
```

Even though it was in a call to `f(0)`. The diagnostic makes it seem like `i == 0`. This does not happen with the experimental bytecode interpreter (<https://godbolt.org/z/G1exhxfKf>), which has `in call to 'f(-1)'`.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mital Ashok (MitalAshok)

<details>
Consider &lt;https://godbolt.org/z/ao786daav&gt;:

```c++
constexpr int f(int x) {
    ++x;
    return x / x;
}

constexpr int i = -1;
static_assert(f(i) == 1);
```

It has this diagnostic:

```
&lt;source&gt;:7:15: error: static assertion expression is not an integral constant expression
    7 | static_assert(f(i) == 1);
      |               ^~~~~~~~~
&lt;source&gt;:3:14: note: division by zero
    3 |     return x / x;
      |              ^ ~
&lt;source&gt;:7:15: note: in call to 'f(0)'
    7 | static_assert(f(i) == 1);
      |               ^~~~
```

Even though it was in a call to `f(0)`. The diagnostic makes it seem like `i == 0`. This does not happen with the experimental bytecode interpreter (&lt;https://godbolt.org/z/G1exhxfKf&gt;), which has `in call to 'f(-1)'`.

</details>


---

