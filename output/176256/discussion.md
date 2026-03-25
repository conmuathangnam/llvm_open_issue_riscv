# [clang] C++23 parentheses-less lambda crash: Assertion `Loc.isValid() && "point of instantiation must be valid!"'

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/176256
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, c++23
**Closed Date:** 2026-01-17T15:43:52Z

## Body

$ cat /tmp/a.cc
```cpp
template <typename> struct a {};

void f() {
  a<void> token = [] -> decltype(token) { return {}; }();
}
```

```console
$ clang++ -fsyntax-only -std=c++23 /tmp/a.cc
clang++: ../../clang/include/clang/AST/DeclTemplate.h:1977: void clang::ClassTemplateSpecializationDecl::setPointOfInstantiation(SourceLocation): Assertion `Loc.isValid() && "point of instantiati
on must be valid!"' failed.    
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Arthur Eubanks (aeubanks)

<details>
```
$ cat /tmp/a.cc
template &lt;typename&gt; struct a {};

void f() {
  a&lt;void&gt; token = [] -&gt; decltype(token) { return {}; }();
}
$ clang++ -fsyntax-only -std=c++23 /tmp/a.cc
clang++: ../../clang/include/clang/AST/DeclTemplate.h:1977: void clang::ClassTemplateSpecializationDecl::setPointOfInstantiation(SourceLocation): Assertion `Loc.isValid() &amp;&amp; "point of instantiati
on must be valid!"' failed.    
```
</details>


---

### Comment 2 - nico

I think I have a fix.

---

### Comment 3 - shafik

Goes back to clang-13: https://godbolt.org/z/1dboor5Y9

before that it was a hard error.

---

