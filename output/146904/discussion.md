# [Clang] `_Nonnull` attribute on return value isn't propagated from declaration to definition

**Author:** bricknerb
**URL:** https://github.com/llvm/llvm-project/issues/146904

## Body

Reproduce: https://gcc.godbolt.org/z/7q3Kd55f6

```cpp
auto foo() -> int* _Nonnull;
auto foo() -> int* {
    return nullptr;
}
```

I would expect a warning saying that definition doesn't match declaration or a warning on returning null on a `_Nonnull` return type.

## Comments

### Comment 1 - shafik

CC @AaronBallman 


---

### Comment 2 - AaronBallman

Yeah, either the nullability should be propagated or it should be diagnosed as an incompatible redeclaration. CC @ymand -- I know your team has been working on nullability stuff recently, is this one on your radar?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Boaz Brickner (bricknerb)

<details>
Reproduce: https://gcc.godbolt.org/z/7q3Kd55f6

```cpp
auto foo() -&gt; int* _Nonnull;
auto foo() -&gt; int* {
    return nullptr;
}
```

I would expect a warning saying that definition doesn't match declaration or a warning on returning null on a `_Nonnull` return type.
</details>


---

