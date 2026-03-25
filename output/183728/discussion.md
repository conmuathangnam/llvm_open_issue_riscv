# [clang] crashes with format_matches attribute after invalid declaration

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183728

## Body

Reproducer:
https://godbolt.org/z/336x7roqh
```c
int bar(char *) __attribute__((format_matches(printf, 1, "%s")));

void foo() {
  const *const fmt2 = fmt;
  bar(fmt2);
}
```

Output:
```console
<source>:4:16: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    4 |   const *const fmt2 = fmt;
      |   ~~~~~        ^
      |   int 
<source>:4:23: error: use of undeclared identifier 'fmt'; did you mean 'fmt2'?
    4 |   const *const fmt2 = fmt;
      |                       ^~~
      |                       fmt2
<source>:4:16: note: 'fmt2' declared here
    4 |   const *const fmt2 = fmt;
      |                ^
<source>:5:7: error: incompatible pointer types passing 'const int *const' to parameter of type 'char *' [-Wincompatible-pointer-types]
    5 |   bar(fmt2);
      |       ^~~~
<source>:1:15: note: passing argument to parameter here
    1 | int bar(char *) __attribute__((format_matches(printf, 1, "%s")));
      |               ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/336x7roqh
```c
int bar(char *) __attribute__((format_matches(printf, 1, "%s")));

void foo() {
  const *const fmt2 = fmt;
  bar(fmt2);
}
```

Output:
```console
&lt;source&gt;:4:16: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
    4 |   const *const fmt2 = fmt;
      |   ~~~~~        ^
      |   int 
&lt;source&gt;:4:23: error: use of undeclared identifier 'fmt'; did you mean 'fmt2'?
    4 |   const *const fmt2 = fmt;
      |                       ^~~
      |                       fmt2
&lt;source&gt;:4:16: note: 'fmt2' declared here
    4 |   const *const fmt2 = fmt;
      |                ^
&lt;source&gt;:5:7: error: incompatible pointer types passing 'const int *const' to parameter of type 'char *' [-Wincompatible-pointer-types]
    5 |   bar(fmt2);
      |       ^~~~
&lt;source&gt;:1:15: note: passing argument to parameter here
    1 | int bar(char *) __attribute__((format_matches(printf, 1, "%s")));
      |               ^
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/M8bP8vnqM

---

### Comment 3 - AaronBallman

This seems to be related to nullptr work done for C23 but I'm not certain *why*. This is a stack overflow happening deep within the default constructor of `PointerUnionMembers`... which is `PointerUnionMembers() = default;`.

However, the type of that object is insane (copied from MSVC debugger):
```
 	clang.exe!llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Type const *,clang::ExtQuals const *>,llvm::PointerIntPair<void *,1,int,llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const *,clang::ExtQuals const *>,llvm::PointerIntPairInfo<void *,1,llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const *,clang::ExtQuals const *>>>,1,clang::ExtQuals const *>::PointerUnionMembers<llvm::PointerUnion<clang::Type const *,clang::ExtQuals const *>,llvm::PointerIntPair<void *,1,int,llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const *,clang::ExtQuals const *>,llvm::PointerIntPairInfo<void *,1,llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const *,clang::ExtQuals const *>>>,1,clang::ExtQuals const *>() Line 70	C++
```
and I can't tell whether that's the same symptom or MSVC being confused.

---

### Comment 4 - AaronBallman

Oh, it's recursion within `checkFormatStringExpr()` itself, that makes *far* more sense. That function is called recursively in several places and it looks like the issue is with the [`DeclRefExpr` case](https://github.com/llvm/llvm-project/blob/b3c4d44c44237fb8df98f09390e391d16fd3f4b1/clang/lib/Sema/SemaChecking.cpp#L6999): `E` is the same pointer as `Init->IgnoreParenImpCasts()` so we recurse forever.

CC @vvuksanovic @apple-fcloutier 

---

### Comment 5 - apple-fcloutier

This is not specific to format_matches. What actually happens is that Clang recovers from `const char *const fmt2 = fmt` by typo-fixing it to `const char *const fmt2 = fmt2`, which leads to infinite recursion trying to figure out the value of `fmt2`. Here's the minimum-noise reproducer for this issue:

```
// https://godbolt.org/z/667j83Ghn
int bar(const char *, ...) __attribute__((format(printf, 1, 2)));

void foo() {
  const char *const fmt2 = fmt2;
  bar(fmt2);
}
```

This is a 1-length cycle and that would be easy to fix locally, but unfortunately, at least as far as the AST is concerned, initializers can have cycles longer than 1. It's not enough to check that `E->IgnoreParenImpCasts()` is different from E, we have to check that it's different from any other expression being looked at, which is a piece of state that somehow isn't captured in any of `checkFormatStringExpr`'s 15 arguments. For instance, in C++ mode, this should be legal (if not very useful) to write but it crashes clang:

```c++
extern const char *const fmt1;
extern const char *const fmt2;

const char *const fmt1 = fmt2;
const char *const fmt2 = fmt1;

int bar(const char *, ...) __attribute__((format(printf, 1, 2)));

void foo() {
  bar(fmt2);
}
```

In C, this is illegal (`fmt1 = fmt2`, `fmt2 = fmt1` are not constant initializers), but Sema diagnoses and continues anyway with that AST formed so it crashes the compiler too.

I will be unable to implement a fix for at least 3 weeks because my hands are full for this week and I will be on vacation for 2 weeks after that.

---

### Comment 6 - apple-fcloutier

(Also, @k-arrows said this goes back to clang-21 because `attribute((format_matches))` was added in clang-21; if you use regular `attribute((format))` instead, it crashes all the way back to clang 2.7.0, which is the earliest version that Godbolt has.)

---

### Comment 7 - k-arrows

> (Also, [@k-arrows](https://github.com/k-arrows) said this goes back to clang-21 because `attribute((format_matches))` was added in clang-21; if you use regular `attribute((format))` instead, it crashes all the way back to clang 2.7.0, which is the earliest version that Godbolt has.)

Confirmed:
https://godbolt.org/z/84W1P43aT

---

