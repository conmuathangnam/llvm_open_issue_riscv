# [Clang] UCN in preprocessor-pasted identifier not resolved to unicode

**Author:** fuhsnn
**URL:** https://github.com/llvm/llvm-project/issues/145240
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-06-24T16:56:17Z

## Body

With the snippet:
```
#define CAT(a,b) a##b

char foo\u00b5;
char*p = &CAT(foo, \u00b5);
```
Clang resolves the variable name as `"fooµ"`, and the concatenated reference to it next line as `"foo\u00b5"`, then error on undeclared reference. Other implementations instead treat both the same: https://godbolt.org/z/M1fejPK36

Wasn't encountered in real world, just a what-if test I came up for my implementation.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hsiang-Ying Fu (fuhsnn)

<details>
With the snippet:
```
#define CAT(a,b) a##b

char foo\u00b5;
char*p = &amp;CAT(foo, \u00b5);
```
Clang resolves the variable name as `"fooµ"`, and the concatenated reference to it next line as `"foo\u00b5"`, then error on undeclared reference. Other implementations instead treat both the same: https://godbolt.org/z/M1fejPK36

Wasn't encountered in real world, just a what-if test I came up for my implementation.
</details>


---

### Comment 2 - yronglin

Thank you report this issue! I'd like try to fix it.

---

### Comment 3 - shafik

CC @cor3ntin @tahonermann 

---

