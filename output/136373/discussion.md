# Name used in parameter should conflict with placeholder used for automatic variable

**Author:** shafik
**URL:** https://github.com/llvm/llvm-project/issues/136373
**Status:** Closed
**Labels:** clang:frontend, c++26
**Closed Date:** 2025-05-02T18:56:29Z

## Body

Given [CWG3005](http://wg21.link/cwg3005) we should reject the following:

```cpp
void f(int _) {
    int _; // error
}
```

godbolt: https://godbolt.org/z/1fa4njchc

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Shafik Yaghmour (shafik)

<details>
Given [CWG3005](http://wg21.link/cwg3005) we should reject the following:

```cpp
void f(int _) {
    int _; // error
}
```

godbolt: https://godbolt.org/z/1fa4njchc
</details>


---

### Comment 2 - erichkeane

The point by the CWG3005 is that the parameter isnt actually a placeholder (aka name-independent).  It is actually a name, thus could conflict.

---

