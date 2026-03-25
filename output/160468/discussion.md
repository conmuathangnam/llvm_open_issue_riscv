# [clang] Heap-allocated volatile variables are diagnosed as volatile temporaries

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/160468

## Body

See https://godbolt.org/z/r1s5ds5v5
```c++
constexpr int foo() {
  for (int i = 0; i != 10; ++i) {
    auto *k = new volatile int{0};

    int *p = (int *)k;
    *p = 10;
    delete k;
  }
  return 1;
}
static_assert(foo() == 1);
```

```console
<source>:11:15: error: static assertion expression is not an integral constant expression
   11 | static_assert(foo() == 1);
      |               ^~~~~~~~~~
<source>:6:8: note: assignment to volatile temporary is not allowed in a constant expression
    6 |     *p = 10;
      |        ^
<source>:11:15: note: in call to 'foo()'
   11 | static_assert(foo() == 1);
      |               ^~~~~
note: volatile temporary created here
```

It's not a temporary though and the source location for the "created here" note is invalid.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See https://godbolt.org/z/r1s5ds5v5
```c++
constexpr int foo() {
  for (int i = 0; i != 10; ++i) {
    auto *k = new volatile int{0};

    int *p = (int *)k;
    *p = 10;
    delete k;
  }
  return 1;
}
static_assert(foo() == 1);
```

```console
&lt;source&gt;:11:15: error: static assertion expression is not an integral constant expression
   11 | static_assert(foo() == 1);
      |               ^~~~~~~~~~
&lt;source&gt;:6:8: note: assignment to volatile temporary is not allowed in a constant expression
    6 |     *p = 10;
      |        ^
&lt;source&gt;:11:15: note: in call to 'foo()'
   11 | static_assert(foo() == 1);
      |               ^~~~~
note: volatile temporary created here
```

It's not a temporary though and the source location for the "created here" note is invalid.
</details>


---

