# [LV]Miscompile in LV with partial.reductions

**Author:** alexey-bataev
**URL:** https://github.com/llvm/llvm-project/issues/176954
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2026-01-22T09:11:14Z

## Body

https://godbolt.org/z/ova3o4Y4d

This code returns incorrect result after loop vectorizer

@fhahn @davemgreen 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Alexey Bataev (alexey-bataev)

<details>
https://godbolt.org/z/ova3o4Y4d

This code returns incorrect result after loop vectorizer

@<!-- -->fhahn @<!-- -->davemgreen 
</details>


---

### Comment 2 - davemgreen

@sdesmalen-arm @MacDue Any ideas?
Current IR from LV + simplifycfg + instcombine: https://godbolt.org/z/8he1Tc991

---

### Comment 3 - alexey-bataev

Priginal reproducer:
```
Program test
  Logical,Parameter :: a(100) = .True.
  Logical :: b(100)
  b = a
  ! if(100.ne.count(b,kind=1)) stop
  print *, count(b,kind=1)
End

```
Expected result is 100, but after loop vectorizer it returns something else

---

### Comment 4 - sdesmalen-arm

> [@sdesmalen-arm](https://github.com/sdesmalen-arm) [@MacDue](https://github.com/MacDue) Any ideas? Current IR from LV + simplifycfg + instcombine: https://godbolt.org/z/8he1Tc991

I've found the culprit, the lowering of partial reduce of v16i8 -> v2i32 seems incorrect. I'll fix it in a bit!

---

