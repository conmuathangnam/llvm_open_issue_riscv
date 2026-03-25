# [Clang] Incorrect "Redeclaration of using-enum declaration"

**Author:** kezyr
**URL:** https://github.com/llvm/llvm-project/issues/147495
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, confirmed
**Closed Date:** 2025-07-10T15:13:46Z

## Body

This code compiles normaly on gcc, msvc but clangd says `Redeclaration of using-enum declaration`.
```c++
struct A {
  enum class B {BB};
  using enum B;
  struct C {
    using enum B;
  };
};
```
https://godbolt.org/z/9d48vcrnv


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Miro Rýzek (kezyr)

<details>
This code compiles normaly on gcc, msvc but clangd says `Redeclaration of using-enum declaration`.
```c++
struct A {
  enum class B {BB};
  using enum B;
  struct C {
    using enum B;
  };
};
```
https://godbolt.org/z/9d48vcrnv

</details>


---

### Comment 2 - shafik

Funny enough MSVC reject this, which should be equivalent but clang accepts: https://godbolt.org/z/qh8T9Phj8

```cpp
struct A {
  enum class B {BB};
  using enum B;
  struct C {
    using B::BB;
  };
};
```

---

