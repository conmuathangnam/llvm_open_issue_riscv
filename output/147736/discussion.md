# [C23] Clang should silently strip the `_Atomic` qualifier in enums with fixed underlying type

**Author:** MisterDA
**URL:** https://github.com/llvm/llvm-project/issues/147736
**Status:** Closed
**Labels:** c23, clang:frontend, confirmed
**Closed Date:** 2025-07-11T11:28:04Z

## Body

```c
/* ISO C23 6.7.3.3-5

   The underlying type of the enumeration is the unqualified,
   non-atomic version of the type specified by the type specifiers in
   the specifier qualifier list. */

enum e : _Atomic int { E };
enum e e;

enum f : const int { F };
enum f f;
```

```
<source>:7:18: error: non-integral type '_Atomic(int)' is an invalid underlying type
    7 | enum e : _Atomic int { E };
      |                  ^
1 error generated.
Compiler returned: 1
```

https://godbolt.org/z/Ka35GTTEr

GCC 15 accepts this. Clang 21 (trunk at the time of writing) rejects it.

cc @AaronBallman 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Antonin Décimo (MisterDA)

<details>
```c
/* ISO C23 6.7.3.3-5

   The underlying type of the enumeration is the unqualified,
   non-atomic version of the type specified by the type specifiers in
   the specifier qualifier list. */

enum e : _Atomic int { E };
enum e e;

enum f : const int { F };
enum f f;
```

```
&lt;source&gt;:7:18: error: non-integral type '_Atomic(int)' is an invalid underlying type
    7 | enum e : _Atomic int { E };
      |                  ^
1 error generated.
Compiler returned: 1
```

https://godbolt.org/z/Ka35GTTEr

GCC 15 accepts this.

cc @<!-- -->AaronBallman 
</details>


---

