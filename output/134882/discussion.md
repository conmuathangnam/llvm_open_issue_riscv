# constexpr decomposition declaration make clang crash

**Author:** hewillk
**URL:** https://github.com/llvm/llvm-project/issues/134882
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash, constexpr
**Closed Date:** 2025-04-11T00:12:12Z

## Body

```cpp
struct foo { char a; int b; };
constexpr foo t{'a', 1};
constexpr auto [...m] = t;
```
https://godbolt.org/z/Ynqh9rnKr

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hewill Kang (hewillk)

<details>
```cpp
struct foo { char a; int b; };
constexpr foo t{'a', 1};
constexpr auto [...m] = t;
```
https://godbolt.org/z/Ynqh9rnKr
</details>


---

### Comment 2 - shafik

Looks like a regression in trunk: https://godbolt.org/z/h3zG9fe5d

---

### Comment 3 - shafik

Actually git bisect says this is a bug w/ the structured binding pack implementation, it bisects to: abc8812df02599fc413d9ed77b992f8236ed2af9

CC @ricejasonf @zwuis 

---

