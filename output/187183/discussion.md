# [clang]Clang assertion failure with lambda capturing local array in invalid template constructor

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/187183

## Body

The following code causes crash on x86-64 clang(assertions)

**https://godbolt.org/z/qrezdrf8r**

```cpp
template<int T>
struct S {
  S();
};
using X = S<0>;

template<int T>
X::S()
{
  int e[1];
  test([&e]() { return e[0]; });
}
```
Clang crashes with an assertion failure when compiling an invalid out-of-class constructor definition for `S<0>` that is still declared as a function template and contains a lambda capturing a local array by reference.  
The compiler correctly diagnoses the template error, but then hits an assertion in the lambda capture handling instead of recovering gracefully.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code causes crash on x86-64 clang(assertions)

**https://godbolt.org/z/qrezdrf8r**

```cpp
template&lt;int T&gt;
struct S {
  S();
};
using X = S&lt;0&gt;;

template&lt;int T&gt;
X::S()
{
  int e[1];
  test([&amp;e]() { return e[0]; });
}
```
Clang crashes with an assertion failure when compiling an invalid out-of-class constructor definition for `S&lt;0&gt;` that is still declared as a function template and contains a lambda capturing a local array by reference.  
The compiler correctly diagnoses the template error, but then hits an assertion in the lambda capture handling instead of recovering gracefully.
</details>


---

