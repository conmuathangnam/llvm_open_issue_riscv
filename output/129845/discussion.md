# accepts-invalid with C++23 constexpr-unknown with struct containing reference

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/129845
**Status:** Closed
**Labels:** clang:frontend, accepts-invalid, c++23, constexpr
**Closed Date:** 2025-03-10T01:38:57Z

## Body

```
int &ff();
int &x = ff();
struct A { int& x; };
constexpr A g = {x};
const A* gg = &g;
```

Should be rejected, currently accepted.  (And related variations miscompile.)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eli Friedman (efriedma-quic)

<details>
```
int &amp;ff();
int &amp;x = ff();
struct A { int&amp; x; };
constexpr A g = {x};
const A* gg = &amp;g;
```

Should be rejected, currently accepted.  (And related variations miscompile.)
</details>


---

### Comment 2 - frederick-vs-ja

A simpler example ([Godbolt link](https://godbolt.org/z/cq4PsEG6o)):
```C++
int &ff();
int &x = ff();
constexpr int &y = x;
```

---

### Comment 3 - cor3ntin

@shafik 

---

