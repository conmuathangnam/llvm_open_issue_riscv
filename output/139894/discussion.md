# [OpenACC] UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenACCClause.cpp:2041!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139894
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, clang:openacc
**Closed Date:** 2025-05-14T14:03:19Z

## Body

Reproducer:
https://godbolt.org/z/fqMWx1Mdz
```cpp
#pragma acc foo gang(2)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/fqMWx1Mdz
```cpp
#pragma acc foo gang(2)
```
</details>


---

### Comment 2 - erichkeane

Ah, shoot!  When writing the parsing I did my best to 'continue', but forgot that when I went to do the sema implementation.  I did an audit of the clauses, and found 3 (vector, worker,gang) that all have a similar problem, so I'll get a patch together in the near future.

---

