# [OpenMP] UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Basic/OpenMPKinds.cpp:855!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139424
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, clang:openmp
**Closed Date:** 2025-05-14T12:06:12Z

## Body

Reproducer:
https://godbolt.org/z/nb864Yfxz
```cpp
void f(){
#pragma omp workshare
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/nb864Yfxz
```cpp
void f(){
#pragma omp workshare
}
```
</details>


---

### Comment 2 - AaronBallman

The issue is an unhandled enumeration here: https://github.com/llvm/llvm-project/blob/2de0c927ae82eb0ae1d7b10028bf4704cb226cc0/clang/lib/Basic/OpenMPKinds.cpp#L796

Curiously, there are zero tests with `workshare` in `clang/test`. CC @alexey-bataev 

---

### Comment 3 - alexey-bataev

Workshare should be reported as an error, C/C++ do not support workshare construct

---

### Comment 4 - alexey-bataev

When flang added support for workshare, they did not report as unsupported in clang

---

### Comment 5 - AaronBallman

> When flang added support for workshare, they did not report as unsupported in clang

Oof, I don't know how we protect against those sort of issues, either. It would be a bummer to require Flang developers to add the functionality to Clang (or vice versa) whenever touching OpenMP, but that's a pretty high bar.

---

### Comment 6 - AaronBallman

CC'ing some Flang OpenMP folks: @tblah @kparzysz 

---

### Comment 7 - kparzysz

Is it ok to emit `err_omp_unknown_directive`, or do we want a more detailed error message?

---

### Comment 8 - AaronBallman

> Is it ok to emit `err_omp_unknown_directive`, or do we want a more detailed error message?

I'm fine with an unknown directive diagnostic, but maybe @alexey-bataev has a different opinion.

---

### Comment 9 - alexey-bataev

I think that's the expected behaviour

---

