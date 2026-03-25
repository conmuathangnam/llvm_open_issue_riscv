# "reference to local variable declared in enclosing function" linked with template instantiation

**Author:** nitnelave
**URL:** https://github.com/llvm/llvm-project/issues/140622
**Status:** Closed
**Labels:** clang:frontend, confirmed, diverges-from:gcc, diverges-from:msvc, diverges-from:edg
**Closed Date:** 2025-05-20T12:04:36Z

## Body

```cpp
template <typename To>
struct L {
  int operator()(const char*) const { return 1; }
};
template <typename To>
constexpr L<To> l;

int main() {
  constexpr auto LOCAL = "hello";
  LOCAL, l<float>("");
}
```
`clang++ -std=c++23`
```
<source>:10:3: error: reference to local variable 'LOCAL' declared in enclosing function 'main'
   10 |   LOCAL, l<float>("");
      |   ^
<source>:10:10: note: in instantiation of variable template specialization 'l<float>' requested here
   10 |   LOCAL, l<float>("");
      |          ^
<source>:9:18: note: 'LOCAL' declared here
    9 |   constexpr auto LOCAL = "hello";
      |                  ^
<source>:10:3: warning: left operand of comma operator has no effect [-Wunused-value]
   10 |   LOCAL, l<float>("");
      |   ^~~~~
1 warning and 1 error generated.
Compiler returned: 1
```

Tested with clang 20, clang trunk, see https://godbolt.org/z/hhxhqrxbo. GCC accepts it.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: nitnelave (nitnelave)

<details>
```cpp
template &lt;typename To&gt;
struct L {
  int operator()(const char*) const { return 1; }
};
template &lt;typename To&gt;
constexpr L&lt;To&gt; l;

int main() {
  constexpr auto LOCAL = "hello";
  LOCAL, l&lt;float&gt;("");
}
```
`clang++ -std=c++23`
```
&lt;source&gt;:10:3: error: reference to local variable 'LOCAL' declared in enclosing function 'main'
   10 |   LOCAL, l&lt;float&gt;("");
      |   ^
&lt;source&gt;:10:10: note: in instantiation of variable template specialization 'l&lt;float&gt;' requested here
   10 |   LOCAL, l&lt;float&gt;("");
      |          ^
&lt;source&gt;:9:18: note: 'LOCAL' declared here
    9 |   constexpr auto LOCAL = "hello";
      |                  ^
&lt;source&gt;:10:3: warning: left operand of comma operator has no effect [-Wunused-value]
   10 |   LOCAL, l&lt;float&gt;("");
      |   ^~~~~
1 warning and 1 error generated.
Compiler returned: 1
```

Tested with clang 20, clang trunk, see https://godbolt.org/z/hhxhqrxbo. GCC accepts it.
</details>


---

### Comment 2 - Ignition

smaller testcase
```cpp
template <typename> struct S{};

// 1. Broken, constexpr dependant type without initializer
template <typename T>
constexpr S<T> g;

// 2. OK, constexpr non-dependant type without initializer
// template <typename>
// constexpr S<int> g;

// 3. OK, constexpr dependant type with initializer
// template <typename T>
// constexpr S<T> g{};

// 4. OK, non-constexpr dependant type without initializer
// template <typename T>
// S<T> g;

int main() {
  constexpr auto x = 42;
  x, g<int>;
}
```

---

### Comment 3 - shafik

This is a weird one: https://godbolt.org/z/d3Gf5Mcbe

Goes back to clang-6: https://godbolt.org/z/d3noK9a3n

If `x` is not *const* or *constexpr* it is fine: https://godbolt.org/z/aGYjMdex8

---

