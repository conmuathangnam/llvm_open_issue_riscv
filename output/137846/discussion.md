# Clang 21 accepts invalid # 0 preprocessor lines leading to undefined behavior at compile time

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137846

## Body

## Summary

Clang 21 accepts invalid standalone # 0 preprocessor lines in C source files without emitting an error or a warning. When compiled, the program exhibits undefined behavior (e.g., prints unlimited spaces or crashes), despite the code being syntactically invalid.

## Program 

```
uint32_t square(uint32_t x)
# 0
# 0
```

## To reproduce Quickly

https://gcc.godbolt.org/z/c1daT34v6

It gets:

[Truncated]
Compiler returned: 143


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary

Clang 21 accepts invalid standalone # 0 preprocessor lines in C source files without emitting an error or a warning. When compiled, the program exhibits undefined behavior (e.g., prints unlimited spaces or crashes), despite the code being syntactically invalid.

## Program 

```
uint32_t square(uint32_t x)
# 0
# 0
```

## To reproduce Quickly

https://gcc.godbolt.org/z/c1daT34v6

It gets:

[Truncated]
Compiler returned: 143

</details>


---

### Comment 2 - shafik

Just one is required:

```cpp
uint32_t square(uint32_t x)
# 0
```

If we use `-pedantic` we see these are extension and treated as `#line` markers. Zero is not going to be valid but we are clearly doing something wrong here.

Looks like this started after clang-16: https://godbolt.org/z/aYK8dr8c8

---

