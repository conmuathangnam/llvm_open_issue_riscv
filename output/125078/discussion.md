# [clang] `alloc_align` attribute crashes if applied to a function pointer

**Author:** MacDue
**URL:** https://github.com/llvm/llvm-project/issues/125078

## Body

Reproducer:
```
void aa_fn_ptr(int* (*fn_ptr)(int)  __attribute__((alloc_align(1))));
```
Compile with clang (any options)

CE: https://godbolt.org/z/z81edvhWs

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Benjamin Maxwell (MacDue)

<details>
Reproducer:
```
void aa_fn_ptr(int* (*member)(int)  __attribute__((alloc_align(1))));
```
Compile with clang (any options)

CE: https://godbolt.org/z/14zxWYdM4
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Benjamin Maxwell (MacDue)

<details>
Reproducer:
```
void aa_fn_ptr(int* (*member)(int)  __attribute__((alloc_align(1))));
```
Compile with clang (any options)

CE: https://godbolt.org/z/14zxWYdM4
</details>


---

