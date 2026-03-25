# Destroying delete requires accessible destructor

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/46818
**Status:** Closed
**Labels:** c++20, clang:frontend, bugzilla, confirmed
**Closed Date:** 2025-01-09T13:29:21Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [47474](https://llvm.org/bz47474) |
| Version | trunk |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @dwblaikie,@tambry,@zygoloid |

## Extended Description 
From StackOverflow (https://stackoverflow.com/q/63797383/2069064):
```cpp
#include <new>

struct S {
    void operator delete(S* p, std::destroying_delete_t);

private:
    ~S();
};

void f() {
    delete new S;
}
```
This fails to compile on clang, which requires the destructor of `S` to be accessible. But nothing here invokes the destructor of `S` (presumably the destroying delete would), so this should compile fine.

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

[expr.delete]p12 says that (unconditionally) "Access and ambiguity control are done for both the deallocation function and the destructor."

So while I agree that we shouldn't be performing an access check here, the standard will need to be fixed too.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [47474](https://llvm.org/bz47474) |
| Version | trunk |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->dwblaikie,@<!-- -->tambry,@<!-- -->zygoloid |

## Extended Description 
From StackOverflow (https://stackoverflow.com/q/63797383/2069064):
```cpp
#include &lt;new&gt;

struct S {
    void operator delete(S* p, std::destroying_delete_t);

private:
    ~S();
};

void f() {
    delete new S;
}
```
This fails to compile on clang, which requires the destructor of `S` to be accessible. But nothing here invokes the destructor of `S` (presumably the destroying delete would), so this should compile fine.
</details>


---

