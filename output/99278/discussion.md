# [Clang][C++26] Clang overly generalizes the change in P3144R2 to pointers to incomplete enumeration types

**Author:** frederick-vs-ja
**URL:** https://github.com/llvm/llvm-project/issues/99278
**Status:** Closed
**Labels:** clang:frontend, confirmed, c++26
**Closed Date:** 2024-12-09T09:20:18Z

## Body

Despite the title being "Deleting a Pointer to an Incomplete Type Should be Ill-formed", [P3144R2](https://wg21.link/p3144r2) only invalidated deleting pointers to incomplete class types.

So, currently the following definition is supposed to be well-formed. But Clang currently rejects it ([Godbolt link](https://godbolt.org/z/aMPasEfad)).
```C++
enum E {
    zero = decltype(delete static_cast<E*>(nullptr), 0){} // E is incomplete here
};
```

Not sure whether the standard or Clang should be fixed. As it's already very tricky to attempt to delete a pointer to an incomplete enumeration type, a CWG issue might be wanted to reject such delete-expressions.

Edit: [CWG2925](https://cplusplus.github.io/CWG/issues/2925.html) was closed as NAD, so presumably Clang should be fixed.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: A. Jiang (frederick-vs-ja)

<details>
Despite the title being "Deleting a Pointer to an Incomplete Type Should be Ill-formed", [P3144R2](https://wg21.link/p3144r2) only made deleting pointers to incomplete class types.

So, currently the following definition is supposed to be well-formed. But Clang currently rejects it ([Godbolt link](https://godbolt.org/z/aMPasEfad)).
```C++
enum E {
    zero = decltype(delete static_cast&lt;E*&gt;(nullptr), 0){} // E is incomplete here
};
```

Not sure whether the standard or Clang should be fixed. As it's already very tricky to attempt to delete a pointer to an incomplete enumeration type, a CWG issue might be wanted to reject such delete-expressions.
</details>


---

### Comment 2 - shafik

CC @cor3ntin 

---

### Comment 3 - AlisdairM

As author of P3144, this case was deliberately not touched by the paper, as it has always been well-formed, and not even deprecated.  The UB in prior standards was specific to incomplete class types, not scalar types such as an incomplete enumeration.  The purpose of P3144 was to remove the undefined behavior (or IFNDR in this case), by diagnosing it as ill-formed, not to change other well-defined behavior.  Yes, we chose to break the specific well-defined case for classes that, when complete, were trivial-ish, but there was never a suggestion to break well-defined code that did not risk UB/IFNDR.

---

