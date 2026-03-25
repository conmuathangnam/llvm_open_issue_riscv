# New __ptrauth types are wrongly reported as __is_trivially_equality_comparable

**Author:** halbi2
**URL:** https://github.com/llvm/llvm-project/issues/137473
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-04-28T03:07:50Z

## Body

```
struct S1 {
  int * __ptrauth(1,1,50) p_;
  bool operator==(const S1&) const = default;
};
static_assert(!__is_trivially_equality_comparable(S1));
```
This test case fails. So libc++ `std::equal` will use `memcmp` to compare objects of type `S1` and will wrongly report that they are not-equal when they are supposed to be equal. This is a wrong-codegen bug.

## Comments

### Comment 1 - halbi2

@philnik777 @shafik could you please consider this bug also?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (halbi2)

<details>
```
struct S1 {
  int * __ptrauth(1,1,50) p_;
  bool operator==(const S1&amp;) const = default;
};
static_assert(!__is_trivially_equality_comparable(S1));
```
This test case fails. So libc++ `std::equal` will use `memcmp` to compare objects of type `S1` and will wrongly report that they are not-equal when they are supposed to be equal. This is a wrong-codegen bug.
</details>


---

### Comment 3 - asl

Tagging @ahatanak @ojhunt 

---

### Comment 4 - ojhunt

100% a bug, I've r+'d the PR and started the additional bots that apparently don't run by default

@asl  There's an additional ptrauth feature that isn't yet upstreamed: `__ptrauth_restricted_intptr` which allows pointer auth to be applied to pointer sized integer types which we'll need to ensure correct behavior of and add tests for later.  We should probably do an audit of all similar functionality that we have as our existing usage of `__ptrauth_restricted_intptr` (and possibly even just the raw `__ptrauth` qualifier) is clearly sufficiently restricted that no one has hit problems due to this. I'll also do some checks to see if the problem is just missed code from upstreaming.

---

