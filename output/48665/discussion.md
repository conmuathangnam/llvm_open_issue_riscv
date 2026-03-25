# forming reference to nullptr is not rejected in constexpr

**Author:** ldalessa
**URL:** https://github.com/llvm/llvm-project/issues/48665
**Status:** Closed
**Labels:** c++20, clang:frontend, bugzilla, accepts-invalid, confirmed, constexpr
**Closed Date:** 2025-07-16T12:25:25Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [49321](https://llvm.org/bz49321) |
| Version | trunk |
| OS | All |
| CC | @randomnetcat,@zygoloid |

## Extended Description 
I think this UB should be rejected.

```cpp
constexpr bool foo(int* i) {
    int& j = *i;
    return true;
}

static_assert(foo(nullptr));
```

https://godbolt.org/z/cKo3jf

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Similarly we should reject:

```cpp
int arr[3];
constexpr bool f() {
  int &r = arr[3];
  return true;
}
static_assert(f(), "");
```

---

### Comment 2 - Endilll

Both examples are still accepted, but we issue `-Warray-bounds` diagnostic in Richard's example: https://godbolt.org/z/5ocT47qMG

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Luke D'Alessandro (ldalessa)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [49321](https://llvm.org/bz49321) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->randomnetcat,@<!-- -->zygoloid |

## Extended Description 
I think this UB should be rejected.

```cpp
constexpr bool foo(int* i) {
    int&amp; j = *i;
    return true;
}

static_assert(foo(nullptr));
```

https://godbolt.org/z/cKo3jf
</details>


---

### Comment 4 - cor3ntin

@zygoloid Probably stupid question,  what wording makes that ill-formed?
I can't find anything here https://eel.is/c++draft/dcl.init.ref

---

### Comment 5 - Endilll

> I can't find anything here https://eel.is/c++draft/dcl.init.ref

Since Tokyo we have [a note](https://eel.is/c++draft/dcl.ref#note-2) in [dcl.ref] about this:
> Because a null pointer value or a pointer past the end of an object does not point to an object, a reference in a well-defined program cannot refer to such things; see 7.6.2.2 [[expr.unary.op](https://wg21.link/expr.unary.op)].

Before resolution of [CWG453](https://cplusplus.github.io/CWG/issues/453.html) was adopted in Tokyo, for the longest time we had the following normative wording in [dcl.ref]/5:
> A reference shall be initialized to refer to a valid object or function.

---

