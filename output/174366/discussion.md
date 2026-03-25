# Parenthesized members are incorrectly recognized as constructors

**Author:** Halalaluyafail3
**URL:** https://github.com/llvm/llvm-project/issues/174366

## Body

The following program demonstrates the issue:
```cpp
struct S{
    static S(foo);
};
int main(){}
```
`foo` isn't a valid _parameter-declaration-clause_, so this shouldn't be considered a constructor <https://eel.is/c++draft/class.ctor.general#1.sentence-1>. GCC, MSVC, and EDG accept this. Clang correctly parses this with another set of parentheses, e.g. `static S((foo));`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Halalaluyafail3)

<details>
The following program demonstrates the issue:
```cpp
struct S{
    static S(foo);
};
int main(){}
```
`foo` isn't a valid _parameter-declaration-clause_, so this shouldn't be considered a constructor &lt;https://eel.is/c++draft/class.ctor.general#<!-- -->1.sentence-1&gt;. GCC, MSVC, and EDG accept this. Clang correctly parses this with another set of parentheses, e.g. `static S((foo));`.
</details>


---

### Comment 2 - shafik

This behavior goes way back, looks like we have a test for a simpler case:

https://github.com/llvm/llvm-project/blob/32d46c9033ce5129906e4d164535271637b3aaa5/clang/test/SemaCXX/constructor.cpp#L14

---

### Comment 3 - zygoloid

We're doing this intentionally; see https://github.com/llvm/llvm-project/blob/ad5be31c30525b70eb0ccea6d0309856417dc7e6/clang/lib/Parse/ParseDecl.cpp#L6105-L6118

The rationale we're using is: either this is a data member whose type is the same as that of the enclosing class, which is invalid because the type is necessarily incomplete, or this was intended to be a constructor declaration, but it's invalid because the parameter type hasn't been declared. (But that incorrectly assumes that a data member is a non-static data member.)

It looks like we'll need to pass in some more information to `isConstructorDeclarator` to distinguish the `static` case from the non-`static` case. I think it's important that we continue to diagnose

```c++
struct S {
  S(foo);
};
```

... by saying that the type `foo` has not been declared rather than saying that `S` is incomplete.

---

