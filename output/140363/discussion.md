# [clang] Assertion `E->isGLValue() || E->getType()->isFunctionType() || E->getType()->isVoidType() || isa<ObjCSelectorExpr>(E->IgnoreParens())' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140363

## Body

Reproducer:
https://godbolt.org/z/4Wro8dhfb
```cpp
typedef __SIZE_TYPE__ size_t;

namespace std {
struct nothrow_t {};
const nothrow_t nothrow = {};
} // namespace std

struct X1 {
  X1();
  void *operator new(size_t, std::nothrow_t) throw();
};

X1 *foo() {
  X1 *p = new (std::nothrow) X1;
  delete p;
  return new (std::nothrow) X1;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/4Wro8dhfb
```cpp
typedef __SIZE_TYPE__ size_t;

namespace std {
struct nothrow_t {};
const nothrow_t nothrow = {};
} // namespace std

struct X1 {
  X1();
  void *operator new(size_t, std::nothrow_t) throw();
};

X1 *foo() {
  X1 *p = new (std::nothrow) X1;
  delete p;
  return new (std::nothrow) X1;
}
```
</details>


---

