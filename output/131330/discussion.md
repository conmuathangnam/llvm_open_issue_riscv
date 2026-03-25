# [Clang] Regression on not rejecting UB in constexpr reference initialization due to implementing P2280R4

**Author:** frederick-vs-ja
**URL:** https://github.com/llvm/llvm-project/issues/131330
**Status:** Closed
**Labels:** clang:frontend, accepts-invalid, constexpr
**Closed Date:** 2025-05-19T18:27:52Z

## Body

Currently, Clang accepts the following code snippet since C++23 mode. [Godbolt link](https://godbolt.org/z/5cxGrq6Gb).

```C++
constexpr int& r = r;
```

[[dcl.ref]/6](https://eel.is/c++draft/dcl.ref#6) (as clarified by the resolution of [CWG453](https://cplusplus.github.io/CWG/issues/453.html)) indicates that such reference binding has UB, and thus needs to be rejected in constant evaluation.

Perhaps this is due to something missed in #95474.

Codegen change can be observed by dropping `constexpr` ([Godbolt link](https://godbolt.org/z/6vo8dqTP5)), but this is conforming as such initialization raises UB.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: A. Jiang (frederick-vs-ja)

<details>
Currently, Clang accepts the following code snippet since C++23 mode. [Godbolt link](https://godbolt.org/z/5cxGrq6Gb).

```C++
constexpr int&amp; r = r;
```

[[dcl.ref]/6](https://eel.is/c++draft/dcl.ref#<!-- -->6) (as clarified by the resolution of [CWG453](https://cplusplus.github.io/CWG/issues/453.html)) indicates that such reference binding has UB, and thus needs to be rejected in constant evaluation.

Perhaps this is due to something missed in #<!-- -->95474.
</details>


---

### Comment 2 - cor3ntin

@shafik 

---

### Comment 3 - shafik

This is obviously a bug

---

