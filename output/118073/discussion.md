# clang AST crashes on invalid decl type.Assertion `getType(Base)->isPointerType() || getType(Base)->isArrayType()' failed

**Author:** mainsea
**URL:** https://github.com/llvm/llvm-project/issues/118073
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2024-12-04T02:15:41Z

## Body


The provided test case contains compilation errors, and in `clang/lib/AST/APValue.cpp`, the method `QualType APValue::LValueBase::getType() const` is noted to handle `ValueDecl` in an inadequate way for similar situations.
>// FIXME: It's unclear where we're supposed to take the type from, and
    // this actually matters for arrays of unknown bound. Eg:
    //
    // extern int arr[]; void f() { extern int arr[3]; };
    // constexpr int *p = &arr[1]; // valid?
    //
    // For now, we take the most complete type we can find.

 The possible reason is that the obtained decl is an invalid decl, causing it to lose its `int[]` type attribute.

crash example like:
```cpp
extern int error_names_array[total_error_count + 2];
extern int error_names_array[];
static int *sqlstate_map = &error_names_array[1];
```

Is this a common way of handling the situation? When Clang detects a compilation error, certain unhandled cases can cause the Clang frontend to crash.However, in my opinion, it should at least not result in a crash.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: mainsea (mainsea)

<details>

The provided test case contains compilation errors, and in `clang/lib/AST/APValue.cpp`, the method `QualType APValue::LValueBase::getType() const` is noted to handle `ValueDecl` in an inadequate way for similar situations.
&gt;// FIXME: It's unclear where we're supposed to take the type from, and
    // this actually matters for arrays of unknown bound. Eg:
    //
    // extern int arr[]; void f() { extern int arr[3]; };
    // constexpr int *p = &amp;arr[1]; // valid?
    //
    // For now, we take the most complete type we can find.

 The possible reason is that the obtained decl is an invalid decl, causing it to lose its `int[]` type attribute.

crash example like:
```cpp
extern int error_names_array[total_error_count + 2];
extern int error_names_array[];
static int *sqlstate_map = &amp;error_names_array[1];
```

Is this a common way of handling the situation? When Clang detects a compilation error, certain unhandled cases can cause the Clang frontend to crash.However, in my opinion, it should at least not result in a crash.
</details>


---

### Comment 2 - shafik

ICE is always a bug but we have a duplicate here: https://github.com/llvm/llvm-project/issues/69468

---

