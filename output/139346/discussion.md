# [OpenACC] Assertion `tokenIsLikeStringLiteral(Tok, getLangOpts()) && "Not a string-literal-like token!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139346
**Status:** Closed
**Labels:** clang:frontend, crash, clang:openacc
**Closed Date:** 2025-05-12T15:51:17Z

## Body

Reproducer:
https://godbolt.org/z/57E5Thh9n
```cpp
void Func();
#pragma acc routine(Func) gang bind(0xF)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/57E5Thh9n
```cpp
void Func();
#pragma acc routine(Func) gang bind(0xF)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/57E5Thh9n
```cpp
void Func();
#pragma acc routine(Func) gang bind(0xF)
```
</details>


---

### Comment 3 - AaronBallman

CC @erichkeane 

---

