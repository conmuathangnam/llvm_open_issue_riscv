# [Clang] Immediate-escalating expression attributed to wrong lambda

**Author:** katzdm
**URL:** https://github.com/llvm/llvm-project/issues/145776
**Status:** Closed
**Labels:** clang:frontend, lambda
**Closed Date:** 2025-08-19T13:59:24Z

## Body

The following should be well-formed, but [clang diagnoses](https://godbolt.org/z/57xfWbE71):

```cpp
void runtime_only() { }
consteval void comptime_only() {}

void fn() {
    []() {
        runtime_only();
        [](){ (void) comptime_only; }();
    }();
}
```

The _id-expression_ that names `comptime_only` is [immediate-escalating](https://eel.is/c++draft/expr.const#25.1). Clang currently attributes that expression to the outer lambda, despite its occurrence within the inner lambda. It seems to me that this stems from the `ExpressionEvaluationContext` corresponding to the inner lambda [being popped](https://github.com/llvm/llvm-project/blob/0c359d747daa97197d17d2c48d7964d49f7f1b98/clang/lib/Sema/SemaLambda.cpp#L2173) _after_ the `FunctionScopeInfo` corresponding to the same has [already been popped](https://github.com/llvm/llvm-project/blob/0c359d747daa97197d17d2c48d7964d49f7f1b98/clang/lib/Sema/SemaLambda.cpp#L1978). Naively re-sequencing these operations appears to cause other problems (e.g., failing assertions related to ODR-uses in `ActOnFinishFunctionBody`).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Daniel M. Katz (katzdm)

<details>
The following should be well-formed, but [clang diagnoses](https://godbolt.org/z/57xfWbE71):

```cpp
void runtime_only() { }
consteval void comptime_only() {}

void fn() {
    []() {
        runtime_only();
        [](){ (void) comptime_only; }();
    }();
}
```

The _id-expression_ that names `comptime_only` is [immediate-escalating](https://eel.is/c++draft/expr.const#<!-- -->25.1). Clang currently attributes that expression to the outer lambda, despite its occurrence within the inner lambda. It seems to me that this stems from the `ExpressionEvaluationContext` corresponding to the inner lambda [being popped](https://github.com/llvm/llvm-project/blob/0c359d747daa97197d17d2c48d7964d49f7f1b98/clang/lib/Sema/SemaLambda.cpp#L2173) _after_ the `FunctionScopeInfo` corresponding to the same has [already been popped](https://github.com/llvm/llvm-project/blob/0c359d747daa97197d17d2c48d7964d49f7f1b98/clang/lib/Sema/SemaLambda.cpp#L1978). Naively re-sequencing these operations appears to cause other problems (e.g., failing assertions related to ODR-uses in `ActOnFinishFunctionBody`).
</details>


---

### Comment 2 - shafik

This looks wrong to me as well.

CC @cor3ntin @zyn0217 

---

