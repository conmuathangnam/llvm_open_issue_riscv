# C++23 #include <format> from libstdc++ fails with "variable template ... cannot appear in its own initializer"

**Author:** stbergmann
**URL:** https://github.com/llvm/llvm-project/issues/139466
**Status:** Closed
**Labels:** duplicate, clang:frontend, libstdc++
**Closed Date:** 2025-05-11T20:30:56Z

## Body

Apparently since <https://github.com/llvm/llvm-project/commit/91f1830cb69cab96201adba6378d64095e251f95> "[Clang][Sema] Handle invalid variable template specialization whose type depends on itself (https://github.com/llvm/llvm-project/pull/134522)":  At least when building agaisnt `libstdc++-devel-15.1.1-1.fc42.x86_64`,
```
$ cat test.cc
#include <format>
```
```
$ clang++ -std=c++23 -fsyntax-only test.cc
In file included from test.cc:1:
/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/format:5165:7: error: variable template 'format_kind' declared with deduced type 'const auto' cannot appear in its own initializer
 5165 |       format_kind<_Rg> // you can specialize this for non-const input ranges
      |       ^
1 error generated.
```
(where the` relevant contents of  `/usr/include/c++/15/format` is
```
  template<typename _Rg>
    constexpr auto format_kind =
    __primary_template_not_defined(
      format_kind<_Rg> // you can specialize this for non-const input ranges
    );
```
).

## Comments

### Comment 1 - stbergmann

duplicate of <https://github.com/llvm/llvm-project/issues/139067> "[clang] fails to compile print("{}", 5s) when using libstdc++"

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Stephan Bergmann (stbergmann)

<details>
Apparently since &lt;https://github.com/llvm/llvm-project/commit/91f1830cb69cab96201adba6378d64095e251f95&gt; "[Clang][Sema] Handle invalid variable template specialization whose type depends on itself (https://github.com/llvm/llvm-project/pull/134522)":  At least when building agaisnt `libstdc++-devel-15.1.1-1.fc42.x86_64`,
```
$ cat test.cc
#include &lt;format&gt;
```
```
$ clang++ -std=c++23 -fsyntax-only test.cc
In file included from test.cc:1:
/usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/format:5165:7: error: variable template 'format_kind' declared with deduced type 'const auto' cannot appear in its own initializer
 5165 |       format_kind&lt;_Rg&gt; // you can specialize this for non-const input ranges
      |       ^
1 error generated.
```
(where the` relevant contents of  `/usr/include/c++/15/format` is
```
  template&lt;typename _Rg&gt;
    constexpr auto format_kind =
    __primary_template_not_defined(
      format_kind&lt;_Rg&gt; // you can specialize this for non-const input ranges
    );
```
).
</details>


---

