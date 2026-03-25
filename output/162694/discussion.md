# [Clang] Missing deduction error diagnostic in C when array size is different

**Author:** to268
**URL:** https://github.com/llvm/llvm-project/issues/162694

## Body

Since [type inference](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3007.htm) was implemented in Clang for C23, it appears that we do not check if there is a mismatch between the declared type and the deduced type in C, as we do in C++.

For example: `auto s[3] = "abc"` is deduced as `char[4]` and therefore should not be compatible with `char[3]`, but we do not enforce this check in C mode and end up with `char[4]` instead of bailing out with `char[3]` != `char[4]`.

[Godbolt C example](https://godbolt.org/z/Gsvb175rz)
[Godbolt C++ example](https://godbolt.org/z/8EcE44Wed)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Guillot Tony (to268)

<details>
Since [type inference](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3007.htm) was implemented in Clang for C23, it appears that we do not check if there is a mismatch between the declared type and the deduced type in C, as we do in C++.

For example: `auto s[3] = "abc"` is deduced as `char[4]` and therefore should not be compatible with `char[3]`, but we do not enforce this check in C mode and end up with `char[4]` instead of bailing out with `char[3]` != `char[4]`.

[Godbolt C example](https://godbolt.org/z/Gsvb175rz)
[Godbolt C++ example](https://godbolt.org/z/8EcE44Wed)
</details>


---

