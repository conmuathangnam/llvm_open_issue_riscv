# [HLSL] Hitting assert in initializer list code

**Author:** spall
**URL:** https://github.com/llvm/llvm-project/issues/173076
**Status:** Closed
**Labels:** clang:frontend, crash, HLSL
**Closed Date:** 2026-01-20T18:11:16Z

## Body

Hitting an assert in the initializer list code when compiling this program:
```
export void fn(int A) {
 int Foo[] = {};
}
```
https://godbolt.org/z/oed1YjYcj

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sarah Spall (spall)

<details>
Hitting an assert in the initializer list code when compiling this program:
```
export void fn(int A) {
 int Foo[] = {};
}
```
https://godbolt.org/z/oed1YjYcj
</details>


---

