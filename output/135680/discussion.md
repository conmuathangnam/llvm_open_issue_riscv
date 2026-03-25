# [Clang] [Sema] `friend` functions that are incorrectly redefined as deleted are not diagnosed

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/135680

## Body

We currently accept this code (https://godbolt.org/z/e9vzx5M6x) even though we shouldn’t (GCC, MSVC, and EDG all reject this):
```c++
struct S {
    friend int f() { return 3; }
    friend int f() = delete;
};
```

(We also crash on this at the moment, but #135679 fixes the crash, though it still accepts this unfortunately.)


Interestingly, we *do* diagnose this (https://godbolt.org/z/3b5qG979r):
```c++
struct S {
    friend int f() = delete;
    friend int f() { return 3; }
};
```

I think we have to do *something* to diagnose the above in `ParseCXXInlineMethodDef()` after we parse the `= default`/`= delete` because we currently just return. It doesn’t seem to be as simple as just calling `CheckForFunctionRedefinition()` as that breaks a bunch of tests (mostly around defaulted `operator==` from what I could tell).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
We currently accept this code (https://godbolt.org/z/e9vzx5M6x) even though we shouldn’t (GCC, MSVC, and EDG all reject this):
```c++
struct S {
    friend int f() { return 3; }
    friend int f() = delete;
};
```

(We also crash on this at the moment, but #<!-- -->135679 fixes the crash, though it still accepts this unfortunately.)


Interestingly, we *do* diagnose this (https://godbolt.org/z/3b5qG979r):
```c++
struct S {
    friend int f() = delete;
    friend int f() { return 3; }
};
```

I think we have to do *something* to diagnose the above in `ParseCXXInlineMethodDef()` after we parse the `= default`/`= delete` because we currently just return. It doesn’t seem to be as simple as just calling `CheckForFunctionRedefinition()` as that breaks a bunch of tests (mostly around defaulted `operator==` from what I could tell).
</details>


---

