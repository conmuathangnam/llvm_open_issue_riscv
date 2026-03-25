# Clang++ crashes when instantiating a particular template class with a static constexpr data member.

**Author:** cassioneri
**URL:** https://github.com/llvm/llvm-project/issues/165988
**Status:** Open
**Labels:** clang:codegen, crash

## Body

This code makes clang++ to crash:
```
template <typename>
struct S {
  static constexpr char c{};
  static constexpr decltype(&c) p = nullptr;
};

auto f() {
  return S<int>::p;
}
```

GCC and MSVC have no issue to compile the code.

https://godbolt.org/z/edr4h4v99

If instead of being deduced, the type of `S::p` or the return type of `f` is explicitly given (`const char*`), then compilation succeeds.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Cassio Neri (cassioneri)

<details>
This code makes clang++ to crash:
```
template &lt;typename&gt;
struct S {
  static constexpr char c{};
  static constexpr decltype(&amp;c) p = nullptr;
};

auto f() {
  return S&lt;int&gt;::p;
}
```

GCC and MSVC have no issue to compile the code.

https://godbolt.org/z/edr4h4v99

If instead of being deduced, the type of `S::p` or the return type of `f` is explicitly given (`const char*`), then compilation succeeds.
</details>


---

### Comment 2 - MagentaTreehouse

Maybe related:
#75847
#124326

---

