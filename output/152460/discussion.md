# [Clang][Sema] Unreachable executed when using `opencl_generic` attribute in SYCL

**Author:** Maetveis
**URL:** https://github.com/llvm/llvm-project/issues/152460

## Body

The following code, when compiled with `clang -fsycl test.cpp`
```cpp
// test.cpp
int [[clang::opencl_generic]] *a;
```
makes clang crash with the message:
> Invalid address space
> UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:6577!

[See it on Compiler Explorer](https://godbolt.org/z/q3no3voz6)

Clang should not crash, it should either accept or reject the input with a proper message.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mészáros Gergely (Maetveis)

<details>
The following code, when compiled with `clang -fsycl test.cpp`
```cpp
int [[clang::opencl_generic]] *a;
```
makes clang crash with the message:
&gt; Invalid address space
&gt; UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:6577!

[See it on Compiler Explorer](https://godbolt.org/z/q3no3voz6)

Clang should not crash, it should either accept or reject the input with a proper message.
</details>


---

