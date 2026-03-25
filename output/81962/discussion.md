# Clang incorrectly rejects getting address of conditionally noexcept function in a nested class template

**Author:** mingxwa
**URL:** https://github.com/llvm/llvm-project/issues/81962
**Status:** Closed
**Labels:** clang:frontend, rejects-valid, crash-on-valid
**Closed Date:** 2025-06-09T05:58:20Z

## Body

Accepted by VS 2022 17.10 Preview 1, GCC 13.2, rejected by Clang 17.0.6.

```cpp
template <bool NE>
struct Foo {
  template <class D>
  static void Fun() noexcept(NE) {}
};

int main() {
  using F = decltype(&Foo<true>::Fun<int>);
  F f{&Foo<true>::Fun<int>};
}
```

Output from `clang-cl /EHsc /nologo /W4 /std:c++latest /c test.cpp`:

```txt
test.cpp:9:8: error: address of overloaded function 'Fun' does not match required type 'void () noexcept'
    9 |   F f{&Foo<true>::Fun<int>};
      |        ^~~~~~~~~~~~~~~~~~~
test.cpp:4:15: note: candidate template ignored: could not match 'void ()' against 'void () noexcept'
    4 |   static void Fun() noexcept(NE) {}
      |               ^
1 error generated.
```

Output from `clang-cl -v`:

```txt
clang version 17.0.6
Target: x86_64-pc-windows-msvc
Thread model: posix
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mingxin Wang (mingxwa)

<details>
Accepted by VS 2022 17.10 Preview 1, GCC 13.2, rejected by Clang 17.0.6.

```cpp
template &lt;bool NE&gt;
struct Foo {
  template &lt;class D&gt;
  static void Fun() noexcept(NE) {}
};

int main() {
  using F = decltype(&amp;Foo&lt;true&gt;::Fun&lt;int&gt;);
  F f{&amp;Foo&lt;true&gt;::Fun&lt;int&gt;};
}
```

Output from `clang-cl /EHsc /nologo /W4 /std:c++latest /c test.cpp`:

```txt
test.cpp:9:8: error: address of overloaded function 'Fun' does not match required type 'void () noexcept'
    9 |   F f{&amp;Foo&lt;true&gt;::Fun&lt;int&gt;};
      |        ^~~~~~~~~~~~~~~~~~~
test.cpp:4:15: note: candidate template ignored: could not match 'void ()' against 'void () noexcept'
    4 |   static void Fun() noexcept(NE) {}
      |               ^
1 error generated.
```

Output from `clang-cl -v`:

```txt
clang version 17.0.6
Target: x86_64-pc-windows-msvc
Thread model: posix
```
</details>


---

### Comment 2 - frederick-vs-ja

Fixed in Clang 19: [Godbolt link](https://godbolt.org/z/vzb7G1WEW).

---

