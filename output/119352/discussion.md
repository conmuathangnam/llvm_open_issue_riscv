# _Complex _BitInt is inconsistently/incorrectly lowered

**Author:** bevin-hansson
**URL:** https://github.com/llvm/llvm-project/issues/119352
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2024-12-19T14:06:26Z

## Body

See the following program: https://godbolt.org/z/YhdKov4x1

There's a number of inconsistencies in how this is emitted.

* `sizeof(_Complex _BitInt(144))` is 48, but the actual size of the declared variable is 64.
* A struct with two `_BitInt`s has a size of 48, both in sizeof and allocation.
* The code in the `set` function is wrong:
  ```ll
  %arrayidx.imagp = getelementptr inbounds i8, ptr %p, i64 32
  store i144 1, ptr %p, align 8
  store i144 1, ptr %arrayidx.imagp, align 8
  ```
  If we allocate a memory area of size 48 and then perform these stores, we will write outside of the allocation (32 + 18 = 50).
  
  Compare this to the `set2` function, which designates the imaginary member at offset 24.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Bevin Hansson (bevin-hansson)

<details>
See the following program: https://godbolt.org/z/YhdKov4x1

There's a number of inconsistencies in how this is emitted.

* `sizeof(_Complex _BitInt(144))` is 48, but the actual size of the declared variable is 64.
* A struct with two `_BitInt`s has a size of 48, both in sizeof and allocation.
* The code in the `set` function is wrong:
  ```
  %arrayidx.imagp = getelementptr inbounds i8, ptr %p, i64 32
  store i144 1, ptr %p, align 8
  store i144 1, ptr %arrayidx.imagp, align 8
  ```
  If we allocate a memory area of size 48 and then perform these stores, we will write outside of the allocation (32 + 18 = 50).
  
  Compare this to the `set2` function, which designates the imaginary member at offset 24.

</details>


---

### Comment 2 - tbaederr

@AaronBallman Are complex bitints even (supposed to be) a thing? Please say no

---

### Comment 3 - Fznamznon

According to the standard IMO only floating complex types are allowed:

```
 There are three complex types, designated as float _Complex, double _Complex, and long double
_Complex. (Complex types are a conditional feature that implementations may not support; see
6.10.10.4.) The real floating and complex types are collectively called the floating types.
```

From what I can google, accepting integer _Complex types is a gcc extension https://gcc.gnu.org/onlinedocs/gcc/Complex.html . However gcc doesn't accept _Complex _BitInt. We probably need to decide if we want to support _Complex _BitInt as clang extension?
AFAIK Aaron is on vacation, cc @erichkeane 


To codegen this correctly, approach taken in https://github.com/llvm/llvm-project/pull/91364  needs to be extended to take care of complex types.

---

### Comment 4 - erichkeane

> According to the standard IMO only floating complex types are allowed:
> 
> ```
>  There are three complex types, designated as float _Complex, double _Complex, and long double
> _Complex. (Complex types are a conditional feature that implementations may not support; see
> 6.10.10.4.) The real floating and complex types are collectively called the floating types.
> ```
> 
> From what I can google, accepting integer _Complex types is a gcc extension https://gcc.gnu.org/onlinedocs/gcc/Complex.html . However gcc doesn't accept _Complex _BitInt. We probably need to decide if we want to support _Complex _BitInt as clang extension? AFAIK Aaron is on vacation, cc [@erichkeane](https://github.com/erichkeane)
> 
> To codegen this correctly, approach taken in [#91364](https://github.com/llvm/llvm-project/pull/91364) needs to be extended to take care of complex types.

I would think that unless we have a good reason to (and I don't really see one yet?) or someone who is volunteering to do the work, that just rejecting complex `_BitInts` is likely the best way forward.  @Fznamznon : Could you perhaps prepare a patch to just reject `_Complex _BitInt`?

---

### Comment 5 - bevin-hansson

Should this ticket be closed, then?

---

### Comment 6 - erichkeane

Fixed in #119402

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Bevin Hansson (bevin-hansson)

<details>
See the following program: https://godbolt.org/z/YhdKov4x1

There's a number of inconsistencies in how this is emitted.

* `sizeof(_Complex _BitInt(144))` is 48, but the actual size of the declared variable is 64.
* A struct with two `_BitInt`s has a size of 48, both in sizeof and allocation.
* The code in the `set` function is wrong:
  ```ll
  %arrayidx.imagp = getelementptr inbounds i8, ptr %p, i64 32
  store i144 1, ptr %p, align 8
  store i144 1, ptr %arrayidx.imagp, align 8
  ```
  If we allocate a memory area of size 48 and then perform these stores, we will write outside of the allocation (32 + 18 = 50).
  
  Compare this to the `set2` function, which designates the imaginary member at offset 24.
</details>


---

