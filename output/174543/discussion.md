# Clang modules crash on recursive reference in function template

**Author:** kamrann
**URL:** https://github.com/llvm/llvm-project/issues/174543
**Status:** Closed
**Labels:** clang:modules, crash
**Closed Date:** 2026-01-07T07:24:15Z

## Body

Regression on trunk, since clang 21:
```
export module m;
template < typename T >
void fun(T)
{
    fun(9);
}
```
```
Program terminated with signal: SIGSEGV
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Cameron Angus (kamrann)

<details>
Regression on trunk, since clang 21:
```
export module m;
template &lt; typename T &gt;
void fun(T)
{
    fun(9);
}
```
```
Program terminated with signal: SIGSEGV
```

</details>


---

### Comment 2 - kamrann

Thank you! This has also resolved the previously mentioned MSSTL std module crash.

---

