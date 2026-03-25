# [Clang] Unhelpful ambiguous `__attribute__((overloadable))` diagnostic

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/157353

## Body

Consider (https://godbolt.org/z/b3jGaMPPb):
```c
struct a {};
struct b {};

__attribute__((overloadable)) void f(struct a*) { return; }
__attribute__((overloadable)) void f(struct b*) { return; }

void g() {
    const struct a x = {};
    f(&x);
}
```
Because passing incompatible pointers to a function and discarding `const` are not errors in C, we consider both of these to be ambiguous; the result of this is a diagnostic that is rather unhelpful:
```console
<source>:9:5: error: call to 'f' is ambiguous
    9 |     f(&x);
      |     ^
<source>:4:36: note: candidate function
    4 | __attribute__((overloadable)) void f(struct a*) { return; }
      |                                    ^
<source>:5:36: note: candidate function
    5 | __attribute__((overloadable)) void f(struct b*) { return; }
      |                                    ^
```
I wonder if we could just disallow such conversions if `__attribute__((overloadable))` is used.

Ideally, we’d make `-Wincompatible-pointer-types-discards-qualifiers` default to an error, but I have a feeling this would run into the same problems that `-Wreturn-type` did, that is there being too much code that doesn’t care and just does this anyway irrespective of the warnings.

However, GCC 14 did make `-Wincompatible-pointer-types` an error, and I think it’d be reasonable for us to do the same since that’s a lot more egregious (whereas `-Wincompatible-pointer-types-discards-qualifiers` could simply happen because a lot of C code isn’t `const`-correct).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c

Author: None (Sirraide)

<details>
Consider (https://godbolt.org/z/b3jGaMPPb):
```c
struct a {};
struct b {};

__attribute__((overloadable)) void f(struct a*) { return; }
__attribute__((overloadable)) void f(struct b*) { return; }

void g() {
    const struct a x = {};
    f(&amp;x);
}
```
Because passing incompatible pointers to a function and discarding `const` are not errors in C, we consider both of these to be ambiguous; the result of this is a diagnostic that is rather unhelpful:
```console
&lt;source&gt;:9:5: error: call to 'f' is ambiguous
    9 |     f(&amp;x);
      |     ^
&lt;source&gt;:4:36: note: candidate function
    4 | __attribute__((overloadable)) void f(struct a*) { return; }
      |                                    ^
&lt;source&gt;:5:36: note: candidate function
    5 | __attribute__((overloadable)) void f(struct b*) { return; }
      |                                    ^
```
I wonder if we could just disallow such conversions if `__attribute__((overloadable))` is used.

Ideally, we’d make `-Wincompatible-pointer-types-discards-qualifiers` default to an error, but I have a feeling this would run into the same problems that `-Wreturn-type` did, that is there being too much code that doesn’t care and just does this anyway irrespective of the warnings.

However, GCC 14 did make `-Wincompatible-pointer-types` an error, and I think it’d be reasonable for us to do the same since that’s a lot more egregious (whereas `-Wincompatible-pointer-types-discards-qualifiers` could simply happen because a lot of C code isn’t `const`-correct).
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
Consider (https://godbolt.org/z/b3jGaMPPb):
```c
struct a {};
struct b {};

__attribute__((overloadable)) void f(struct a*) { return; }
__attribute__((overloadable)) void f(struct b*) { return; }

void g() {
    const struct a x = {};
    f(&amp;x);
}
```
Because passing incompatible pointers to a function and discarding `const` are not errors in C, we consider both of these to be ambiguous; the result of this is a diagnostic that is rather unhelpful:
```console
&lt;source&gt;:9:5: error: call to 'f' is ambiguous
    9 |     f(&amp;x);
      |     ^
&lt;source&gt;:4:36: note: candidate function
    4 | __attribute__((overloadable)) void f(struct a*) { return; }
      |                                    ^
&lt;source&gt;:5:36: note: candidate function
    5 | __attribute__((overloadable)) void f(struct b*) { return; }
      |                                    ^
```
I wonder if we could just disallow such conversions if `__attribute__((overloadable))` is used.

Ideally, we’d make `-Wincompatible-pointer-types-discards-qualifiers` default to an error, but I have a feeling this would run into the same problems that `-Wreturn-type` did, that is there being too much code that doesn’t care and just does this anyway irrespective of the warnings.

However, GCC 14 did make `-Wincompatible-pointer-types` an error, and I think it’d be reasonable for us to do the same since that’s a lot more egregious (whereas `-Wincompatible-pointer-types-discards-qualifiers` could simply happen because a lot of C code isn’t `const`-correct).
</details>


---

