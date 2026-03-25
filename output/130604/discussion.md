# CWG3003: Naming a deducible template for class template argument deduction

**Author:** Endilll
**URL:** https://github.com/llvm/llvm-project/issues/130604
**Status:** Closed
**Labels:** c++17, clang:frontend, crash-on-invalid, regression:19
**Closed Date:** 2025-10-02T10:33:40Z

## Body

Clang crashes on the following code (https://godbolt.org/z/KcoqrEME7) since 19.1.0:
```cpp
template <typename T> struct A { A(T); };

template <typename T, template <typename> class TT = A>
using Alias = TT<T>;

template <typename T>
using Alias2 = Alias<T>;

void h() { Alias2 a(42); }
void h2() { Alias a(42); }
```

From https://github.com/cplusplus/CWG/issues/670, https://cplusplus.github.io/CWG/issues/3003.html.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-17

Author: Vlad Serebrennikov (Endilll)

<details>
Clang crashes on the following code (https://godbolt.org/z/KcoqrEME7) since 19.1.0:
```cpp
template &lt;typename T&gt; struct A { A(T); };

template &lt;typename T, template &lt;typename&gt; class TT = A&gt;
using Alias = TT&lt;T&gt;;

template &lt;typename T&gt;
using Alias2 = Alias&lt;T&gt;;

void h() { Alias2 a(42); }
void h2() { Alias a(42); }
```

From https://github.com/cplusplus/CWG/issues/670, https://cplusplus.github.io/CWG/issues/3003.html.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vlad Serebrennikov (Endilll)

<details>
Clang crashes on the following code (https://godbolt.org/z/KcoqrEME7) since 19.1.0:
```cpp
template &lt;typename T&gt; struct A { A(T); };

template &lt;typename T, template &lt;typename&gt; class TT = A&gt;
using Alias = TT&lt;T&gt;;

template &lt;typename T&gt;
using Alias2 = Alias&lt;T&gt;;

void h() { Alias2 a(42); }
void h2() { Alias a(42); }
```

From https://github.com/cplusplus/CWG/issues/670, https://cplusplus.github.io/CWG/issues/3003.html.
</details>


---

### Comment 3 - cor3ntin

@mizvekov 

---

