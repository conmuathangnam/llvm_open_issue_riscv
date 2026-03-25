# clang accepts invalid code that with trailing comma

**Author:** hokein
**URL:** https://github.com/llvm/llvm-project/issues/125225
**Status:** Closed
**Labels:** clang:frontend, regression, accepts-invalid, confirmed
**Closed Date:** 2025-02-14T15:21:55Z

## Body

Clang doesn't diagnose the following invalid code (with the trailing comma), it treats as valid code:

 
```
void func(int);
void k() {
  func(1, );
}
```

This is a regression caused by https://github.com/llvm/llvm-project/pull/114684.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Haojian Wu (hokein)

<details>
Clang doesn't diagnose the following invalid code (with the trailing comma), it treats as valid code:

 
```
void func(int);
void k() {
  func(1, );
}
```

This is a regression caused by https://github.com/llvm/llvm-project/pull/114684.
</details>


---

