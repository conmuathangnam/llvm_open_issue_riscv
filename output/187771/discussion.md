# [C++20] Invalid module export directive triggers an assertion

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/187771
**Status:** Open
**Labels:** c++20, clang:frontend, clang:modules, crash-on-invalid

## Body

Consider the following complete (invalid) translation unit:
```
export module foo
```
https://godbolt.org/z/6Ge3WMYdd

The lack of a semi colon causes a failed assertion. This appears to be a regression on trunk, too: https://godbolt.org/z/rod51r1or I think this may be related to https://github.com/llvm/llvm-project/pull/173130 and it could perhaps be a duplicate of #181358 but the call stacks are different.

CC @yronglin 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Aaron Ballman (AaronBallman)

<details>
Consider the following complete (invalid) translation unit:
```
export module foo
```
https://godbolt.org/z/6Ge3WMYdd

The lack of a semi colon causes a failed assertion. This appears to be a regression on trunk, too: https://godbolt.org/z/rod51r1or I think this may be related to https://github.com/llvm/llvm-project/pull/173130 and it could perhaps be a duplicate of #<!-- -->181358 but the call stacks are different.

CC @<!-- -->yronglin 
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
Consider the following complete (invalid) translation unit:
```
export module foo
```
https://godbolt.org/z/6Ge3WMYdd

The lack of a semi colon causes a failed assertion. This appears to be a regression on trunk, too: https://godbolt.org/z/rod51r1or I think this may be related to https://github.com/llvm/llvm-project/pull/173130 and it could perhaps be a duplicate of #<!-- -->181358 but the call stacks are different.

CC @<!-- -->yronglin 
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Aaron Ballman (AaronBallman)

<details>
Consider the following complete (invalid) translation unit:
```
export module foo
```
https://godbolt.org/z/6Ge3WMYdd

The lack of a semi colon causes a failed assertion. This appears to be a regression on trunk, too: https://godbolt.org/z/rod51r1or I think this may be related to https://github.com/llvm/llvm-project/pull/173130 and it could perhaps be a duplicate of #<!-- -->181358 but the call stacks are different.

CC @<!-- -->yronglin 
</details>


---

### Comment 4 - AaronBallman

It's unclear whether this is a separate issue or not, but consider this code:
```c++
export module foo

sdfsdfsdf // this should be an unknown type name, not part of the module name
```
https://godbolt.org/z/o5TTWhP73

We emit an "unexpected preprocessing directive" diagnostic for the `sdfsdfsdf` but that seems incorrect, it should not be part of the preprocessing directive because that should end on the newline (same as a real directive would), right? This behavior also changed between trunk and Clang 22.

---

### Comment 5 - yronglin

> Consider the following complete (invalid) translation unit:
> ```
> export module foo
> ```
> https://godbolt.org/z/6Ge3WMYdd
> 
> The lack of a semi colon causes a failed assertion. This appears to be a regression on trunk, too: https://godbolt.org/z/rod51r1or I > think this may be related to https://github.com/llvm/llvm-project/pull/173130 and it could perhaps be a duplicate of #181358 but > the call stacks are different.
> 
> CC @yronglin 

Thank you catch this issue!

Yes, it's a bug and I have found the root cause, I'll fix it in a new PR.

> It's unclear whether this is a separate issue or not, but consider this code:
> 
> export module foo
> 
> sdfsdfsdf // this should be an unknown type name, not part of the module name
> https://godbolt.org/z/o5TTWhP73
> 
> We emit an "unexpected preprocessing directive" diagnostic for the `sdfsdfsdf` but that seems incorrect, it should not be part of the preprocessing directive because that should end on the newline (same as a real directive would), right? This behavior also changed between trunk and Clang 22.

This behavior is not currently a bug, but rather a defined implementation behavior. The issue arose because we implemented [CWG2947](https://cplusplus.github.io/CWG/issues/2947.html) in https://github.com/llvm/llvm-project/pull/107168. After `module-name`, we look ahead one `pp-token`; if the next `pp-token`  is not ';' or '[', we generate a diagnostic. For more detailed information, please refer to the [conversation](https://github.com/llvm/llvm-project/pull/107168#pullrequestreview-3265019858).

> We emit an "unexpected preprocessing directive" diagnostic for the `sdfsdfsdf` but that seems incorrect

IIUC, it should be "unexpected preprocessing token", not "unexpected preprocessing directive".

CC @hubert-reinterpretcast @Bigcheese @ChuanqiXu9 


---

### Comment 6 - yronglin

I have created 2 PR of different approach:
1. https://github.com/llvm/llvm-project/pull/187845 Just add a check for the nullable `std::optional<Token>` object when we want to look ahead next pp-token after module name.
2. https://github.com/llvm/llvm-project/pull/187846 Move the checks from phase 4 to phase 7, but break diagnostic of the following case.
```cpp
// #define m(x) x
// #define LPAREN (
export module m
    LPAREN foo);     // Before: unexpected preprocessing token 'LPAREN' after module name .....
                              // After: unexpected '(' after module name ....
```

---

### Comment 7 - yronglin

https://github.com/llvm/llvm-project/issues/181358 is a separate issue, I have create a new PR to fix it: https://github.com/llvm/llvm-project/pull/187858

---

