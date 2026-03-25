# clang incorrectly accepts non-constant consteval function calls in destructor

**Author:** kelbon
**URL:** https://github.com/llvm/llvm-project/issues/109096
**Status:** Closed
**Labels:** clang:frontend, accepts-invalid, consteval
**Closed Date:** 2025-10-14T14:39:46Z

## Body

https://godbolt.org/z/MzaPMraqK


This code incorrectly produces runtime error instead of compilation error in checking fmt::format string

```cpp

#include <fmt/core.h>

template<typename T>
struct scope_exit {
   T val;
constexpr ~scope_exit() {  val(); }
};

int main() {
  scope_exit x([]() { std::string s = fmt::format("{}"); });
}

```

## Comments

### Comment 1 - kelbon

https://godbolt.org/z/TTncxY6nP

produces runtime segfault with  std::format

---

### Comment 2 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/MfYaf848T)):
```C++
struct fmt_str_like {
  static void non_constexpr_error() {}

  consteval fmt_str_like(const char* ptr) {
    if (!ptr)
      non_constexpr_error();
  }
};

void fmt_like(fmt_str_like) {}

template<typename T>
struct scope_exit {
  T ftor_;

  constexpr ~scope_exit() {
    ftor_();
  }
};

int main() {
  scope_exit guard([]() { fmt_like(nullptr); });
}
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (kelbon)

<details>
https://godbolt.org/z/MzaPMraqK


This code incorrectly produces runtime error instead of compilation error in checking fmt::format string

```cpp

#include &lt;fmt/core.h&gt;

template&lt;typename T&gt;
struct scope_exit {
   T val;
constexpr ~scope_exit() {  val(); }
};

int main() {
  scope_exit x([]() { std::string s = fmt::format("{}"); });
}

```
</details>


---

### Comment 4 - kelbon

Any updates?

---

### Comment 5 - kelbon

clang 21 have this issue too

---

### Comment 6 - tbaederr

maybe @cor3ntin has an idea?

---

### Comment 7 - cor3ntin

Interestingly, it only happens in a template https://godbolt.org/z/qTs3fhY38

---

### Comment 8 - kelbon

@cor3ntin thanks for change, but from commit i dont understand, will std::format correctly compile in destructor or it will be just compile error?

---

### Comment 9 - cor3ntin

It will fail to compile (like gcc)

---

### Comment 10 - kelbon

> It will fail to compile (like gcc)

even when format is correct?

---

