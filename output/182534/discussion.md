# Clang Crash on Invalid - opencl attribute

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/182534

## Body

- this bug was found from fuzzing
- crash on invalid input
- it only seems to occur with different combinations of opencl attributes (opencl_local, opencl_constant, opencl_global, etc.)

## Code
```c
int Nop();

void baz(__attribute__((opencl_global)) int (*Data)) {}

void fizz() {
  baz(Nop);
}
```
## CompilerExplorer
https://godbolt.org/z/c466Kx9hP

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-opencl

Author: Andy Benson (apbenson00)

<details>
- this bug was found from fuzzing
- crash on invalid input
- it only seems to occur with different combinations of opencl attributes (opencl_local, opencl_constant, opencl_global, etc.)

## Code
```c
int Nop();

void baz(__attribute__((opencl_global)) int (*Data)) {}

void fizz() {
  baz(Nop);
}
```
## CompilerExplorer
https://godbolt.org/z/c466Kx9hP
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
- this bug was found from fuzzing
- crash on invalid input
- it only seems to occur with different combinations of opencl attributes (opencl_local, opencl_constant, opencl_global, etc.)

## Code
```c
int Nop();

void baz(__attribute__((opencl_global)) int (*Data)) {}

void fizz() {
  baz(Nop);
}
```
## CompilerExplorer
https://godbolt.org/z/c466Kx9hP
</details>


---

### Comment 3 - NeKon69

Hello! i have started working on this, but i dont have any experience with clang codebase. Would be glad to get some kind of assistance.

So the difference between working compiler settings (with explicit -x cl) is that it has this check in `clang/lib/Parse/ParseExpr.cpp:1655`, which does not allow that behavior if we have specified openCL as a language.

Crash happens because for some reason we dont have type information about pointee, which is weird and I dont really know how to trace this. However i think the fix might be that we add some kind of check like in the file i mentioned above.

Anyways, i might be wrong, if so, please correct me. But i would be super happy to fix this bug nonetheless.

---

### Comment 4 - NeKon69

Oh nevermind. Turns out this check was there because opencl doesn't allow function pointers. which i didn't know about.. oh well.

---

