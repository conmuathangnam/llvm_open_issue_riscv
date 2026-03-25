# Bytecode interpreter: Crash with invalid base casts

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/134796

## Body

See:
```c++
struct C{
  int a;
};
struct S {
  constexpr S() {
    ((C*)this)->a = 12;
  }
};
constexpr S s;
```
We don't diagnose the invalid cast from `S*` to `C*` and instead crash in `Pointer::isField()` which is called via `GetPtrBasePop()`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See:
```c++
struct C{
  int a;
};
struct S {
  constexpr S() {
    ((C*)this)-&gt;a = 12;
  }
};
constexpr S s;
```
We don't diagnose the invalid cast from `S*` to `C*` and instead crash in `Pointer::isField()` which is called via `GetPtrBasePop()`.
</details>


---

