# [OpenMP] crash on invalid with metadirective

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139665
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, clang:openmp
**Closed Date:** 2025-05-14T14:19:26Z

## Body

Reproducer:
https://godbolt.org/z/aGxv1PG6d
```cpp
void f(){
#pragma omp metadirective(
}
```

Essentially the same as https://github.com/llvm/llvm-project/issues/139360, but not yet fixed

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/aGxv1PG6d
```cpp
void f(){
#pragma omp metadirective(
}
```

Essentially the same as https://github.com/llvm/llvm-project/issues/139360, but not yet fixed
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/aGxv1PG6d
```cpp
void f(){
#pragma omp metadirective(
}
```

Essentially the same as https://github.com/llvm/llvm-project/issues/139360, but not yet fixed
</details>


---

### Comment 3 - AaronBallman

I'm not certain of the best way to fix this because the existing parsing code looks suspect in a few ways: https://github.com/llvm/llvm-project/blob/9b63bdd15418e90d0c6889d26e490db89ae744d1/clang/lib/Parse/ParseOpenMP.cpp#L2772

We have a tentative parsing action involved in this parsing loop.

Sometimes, we issue an error and continue parsing. This seems like the root cause of our problem in this issue: https://github.com/llvm/llvm-project/blob/9b63bdd15418e90d0c6889d26e490db89ae744d1/clang/lib/Parse/ParseOpenMP.cpp#L2790

Sometimes, we issue an error and return the directive without touching the tentative parsing action: https://github.com/llvm/llvm-project/blob/9b63bdd15418e90d0c6889d26e490db89ae744d1/clang/lib/Parse/ParseOpenMP.cpp#L2800 This will cause a different assertion that should also be fixed: https://godbolt.org/z/YY3f59cjv

Sometimes, we issue and error and commit to the parse instead of reverting the failed parse: https://github.com/llvm/llvm-project/blob/9b63bdd15418e90d0c6889d26e490db89ae744d1/clang/lib/Parse/ParseOpenMP.cpp#L2808

When I try to fix this up by no longer continuing to parse after issuing the error, we get very different diagnostic output in `test/OpenMP/metadirective_messages.cpp` and perhaps that means my changes are incorrect. It could also mean follow-on parsing also needs to be fixed, though.

I think an OpenMP expert may need to come by to repair this one fully. CC @alexey-bataev @jhuber6 @shiltian 

---

### Comment 4 - alexey-bataev

Assume, the correct would be to emit the error in line 2790, skip to the end of the line and exit

---

### Comment 5 - AaronBallman

So something like:
```
      if (CKind == OMPC_unknown) {
        Diag(Tok, diag::err_omp_expected_clause) << "metadirective";
        TPA.Revert();
        SkipUntil(tok::annot_pragma_openmp_end);
        return StmtError();
      }
```
?

And should we do that for the other places which emit an error? If so, I get:
```
# | error: 'expected-warning' diagnostics expected but not seen:
# |   File F:\source\llvm-project\clang\test\OpenMP\metadirective_messages.cpp Line 46: isa trait 'some-unsupported-feature' is not known to the current target; verify the spelling or consider restricting the context selector with the 'arch' selector further
```
from metadirective_messages.cpp which seems like a regression?

---

### Comment 6 - alexey-bataev

> So something like:
> 
> ```
>       if (CKind == OMPC_unknown) {
>         Diag(Tok, diag::err_omp_expected_clause) << "metadirective";
>         TPA.Revert();
>         SkipUntil(tok::annot_pragma_openmp_end);
>         return StmtError();
>       }
> ```
> 
> ?
> 
> And should we do that for the other places which emit an error? If so, I get:
> 
> ```
> # | error: 'expected-warning' diagnostics expected but not seen:
> # |   File F:\source\llvm-project\clang\test\OpenMP\metadirective_messages.cpp Line 46: isa trait 'some-unsupported-feature' is not known to the current target; verify the spelling or consider restricting the context selector with the 'arch' selector further
> ```
> 
> from metadirective_messages.cpp which seems like a regression?

Try to return Directive instead of StmtError

---

### Comment 7 - AaronBallman

Okay, I think I've got some of the repairs done. I'm still not convinced this function is correct though; we are inconsistent with whether we commit or revert tokens after an error and that's pretty suspicious. But I also can't find failures with the approach (I have basically no expertise with OpenMP though, so I may not be sufficiently creative to trigger problems).

---

