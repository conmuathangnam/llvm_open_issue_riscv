# [clang] crash when evaluating __is_bitwise_cloneable on an invalid type

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183665
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:19
**Closed Date:** 2026-03-13T07:45:02Z

## Body

Reproducer:
https://godbolt.org/z/EbzvePqbY
```cpp
struct ABC {
  ABCD ptr;
};

static_assert(__is_bitwise_cloneable(ABC) == true);
```

Output:
```console
<source>:2:3: error: unknown type name 'ABCD'; did you mean 'ABC'?
    2 |   ABCD ptr;
      |   ^~~~
      |   ABC
<source>:1:8: note: 'ABC' declared here
    1 | struct ABC {
      |        ^
<source>:2:8: error: field has incomplete type 'ABC'
    2 |   ABCD ptr;
      |        ^
<source>:1:8: note: definition of 'ABC' is not complete until the closing '}'
    1 | struct ABC {
      |        ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/EbzvePqbY
```cpp
struct ABC {
  ABCD ptr;
};

static_assert(__is_bitwise_cloneable(ABC) == true);
```

Output:
```console
&lt;source&gt;:2:3: error: unknown type name 'ABCD'; did you mean 'ABC'?
    2 |   ABCD ptr;
      |   ^~~~
      |   ABC
&lt;source&gt;:1:8: note: 'ABC' declared here
    1 | struct ABC {
      |        ^
&lt;source&gt;:2:8: error: field has incomplete type 'ABC'
    2 |   ABCD ptr;
      |        ^
&lt;source&gt;:1:8: note: definition of 'ABC' is not complete until the closing '}'
    1 | struct ABC {
      |        ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19:
https://godbolt.org/z/cjzrs51Ez

---

### Comment 3 - Xinlong-Chen

It seems that clang crashes when recursing into structs with invalid member types. 
I want to take this issue and solve this problem.

---

