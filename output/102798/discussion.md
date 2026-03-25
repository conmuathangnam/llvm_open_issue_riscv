# [C23] `#embed` bytes with negative signed char values are wrapped around to `unsigned int` when output with `-E`

**Author:** MitalAshok
**URL:** https://github.com/llvm/llvm-project/issues/102798
**Status:** Closed
**Labels:** c23, clang:frontend, confirmed, embed
**Closed Date:** 2025-02-13T09:59:23Z

## Body

For example:

https://godbolt.org/z/8Tf5ncYWf

```c
//é
constexpr char t[] = {
#embed __FILE__
};
```

Compiled with `-std=c23 -E` outputs:

```c
# 1 "/app/example.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 432 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/app/example.c" 2

constexpr char t[] = {47, 47, 4294967235, 4294967209, 10, 99, 111, 110, <<snipped>> };
```

Which doesn't compile when fed back into the compiler

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mital Ashok (MitalAshok)

<details>
For example:

https://godbolt.org/z/8Tf5ncYWf

```c
//é
constexpr char t[] = {
#embed __FILE__
};
```

Compiled with `-std=c23 -E` outputs:

```c
# 1 "/app/example.c"
# 1 "&lt;built-in&gt;" 1
# 1 "&lt;built-in&gt;" 3
# 432 "&lt;built-in&gt;" 3
# 1 "&lt;command line&gt;" 1
# 1 "&lt;built-in&gt;" 2
# 1 "/app/example.c" 2

constexpr char t[] = {47, 47, 4294967235, 4294967209, 10, 99, 111, 110, &lt;&lt;snipped&gt;&gt; };
```

Which doesn't compile when fed back into the compiler
</details>


---

### Comment 2 - zygoloid

Should `-funsigned-char` affect the preprocessed output for this case?

---

### Comment 3 - Fznamznon

Hmm, since the intention now is to yield int (not unsigned int) values by #embed, I think this is a bug.
I think this cast might be guilty
https://github.com/llvm/llvm-project/blob/4915fddbb2d79b5d67794b88c23da8d296968d0e/clang/lib/Frontend/PrintPreprocessedOutput.cpp#L980

---

### Comment 4 - AaronBallman

> Should `-funsigned-char` affect the preprocessed output for this case?

The standard wording says: 

> The values of the integer constant expressions in the expanded sequence are determined by an
implementation-defined mapping of the resource’s data. Each integer constant expression’s value is
in the range from 0 to (2^embed element width) − 1, inclusive. If:
— the list of integer constant expressions is used to initialize an array of a type compatible with
unsigned char, or compatible with char if char cannot hold negative values; and,
— the embed element width is equal to CHAR_BIT (5.2.5.3.2),
then the contents of the initialized elements of the array are as-if the resource’s binary data is fread
(7.23.8.1) into the array at translation time.

With an attached footnote:

> For example, an embed element width of 8 will yield a range of values from 0 to 255, inclusive.

---

