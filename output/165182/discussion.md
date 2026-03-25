# [Clang] Lifetime extension of temporaries in for-range-initializers is not applied in templates

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/165182
**Status:** Closed
**Labels:** clang:frontend, c++23
**Closed Date:** 2026-01-21T23:19:53Z

## Body

This is based on the example in [[stmt.ranged]](https://eel.is/c++draft/stmt.ranged#example-2):
```c++
const char* s = "1";

struct Foo {
  int& x;
  Foo(int& x) noexcept : x{x} {}
  ~Foo() noexcept { x = 42; }
  const char* begin() const noexcept { return s; }
  const char* end() const noexcept { return s + 1; }
};

const Foo& f1(const Foo& t) noexcept { return t; }
Foo g(int& x) noexcept { return Foo(x); }

// Lifetime extension is missing!
template <typename T>
int test1() {
    int x = 5;
    int sum = 0;
    for (int _ : f1(g(x))) sum += x; // 'x' should still be 5 here
    sum += x; // 'x' set to 42 by the destructor of 'g(x)'
    return sum; // should be 47
}
```
According to my understanding of this, we should be applying lifetime extension to `g(x)` here in `test1()`, but we don't. GCC seems to agree with me, because this function returns `47` when compiled with GCC, but `84` when compiled with Clang (yes, I did make sure to enable lifetime extension by passing `-std=c++23`). MSVC w/ `/std:c++latest` also returns `84`.

If we remove the template-head from `test1()`, then we suddenly get `47` again: https://clang.godbolt.org/z/jEYsE6fbb.

It seems that we’re forgetting about the lifetime extension at some point during template instantiation; compare the AST node before
```console
MaterializeTemporaryExpr 0x29052c50 <col:21, col:24> 'const Foo' lvalue extended by Var 0x29052d98 '__range1' 'const Foo &'
```
and after instantiation:
```console
MaterializeTemporaryExpr 0x29057b90 <col:21, col:24> 'const Foo' lvalue
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
This is based on the example in [[stmt.ranged]](https://eel.is/c++draft/stmt.ranged#example-2):
```c++
const char* s = "1";

struct Foo {
  int&amp; x;
  Foo(int&amp; x) noexcept : x{x} {}
  ~Foo() noexcept { x = 42; }
  const char* begin() const noexcept { return s; }
  const char* end() const noexcept { return s + 1; }
};

const Foo&amp; f1(const Foo&amp; t) noexcept { return t; }
Foo g(int&amp; x) noexcept { return Foo(x); }

// Lifetime extension is missing!
template &lt;typename T&gt;
int test1() {
    int x = 5;
    int sum = 0;
    for (int _ : f1(g(x))) sum += x; // 'x' should still be 5 here
    sum += x; // 'x' set to 42 by the destructor of 'g(x)'
    return sum; // should be 47
}
```
According to my understanding of this, we should be applying lifetime extension to `g(x)` here in `test1()`, but we don't. GCC seems to agree with me, because this function returns `47` when compiled with GCC, but `84` when compiled with Clang (yes, I did make sure to enable lifetime extension by passing `-std=c++23`). MSVC w/ `/std:c++latest` also returns `84`.

If we remove the template-head from `test1()`, then we suddenly get `47` again: https://clang.godbolt.org/z/jEYsE6fbb.

It seems that we’re forgetting about the lifetime extension at some point during template instantiation; compare the AST node before
```console
MaterializeTemporaryExpr 0x29052c50 &lt;col:21, col:24&gt; 'const Foo' lvalue extended by Var 0x29052d98 '__range1' 'const Foo &amp;'
```
and after instantiation:
```console
MaterializeTemporaryExpr 0x29057b90 &lt;col:21, col:24&gt; 'const Foo' lvalue
```

</details>


---

### Comment 2 - yronglin

@Sirraide Thanks report this! This seems a bug, I'll  invesgate this issue.

---

### Comment 3 - Sirraide

In case it helps, I ran into this problem while implementing expansions statements and managed to fix it  for expansion statements only (the problem persists for for loops); all the lifetime-extension code is here in tree transform: https://github.com/llvm/llvm-project/pull/169685/changes#diff-18fbe7dc892da230169dec77d851a0ce59bd18d4e16645aad4b4d84d8b8729f5

For reasons beyond my understanding, I also had to create _two_ `LexicalScope`s in codegen for lifetime extension to be applied properly, but I think the problem with this issue here is still in tree transform as the AST doesn’t even contain the lifetime extension anymore after instantiation

---

### Comment 4 - yronglin

@Sirraide Thanks! This feature is one I implemented in clang before, so I self-assigned this issue and created a PR to fix it, hoping it will unblock your work.

---

