# [X86] __int128 parameters cause wrong location for subsequent i64

**Author:** aengelke
**URL:** https://github.com/llvm/llvm-project/issues/123935
**Status:** Closed
**Labels:** backend:X86, ABI
**Closed Date:** 2025-01-24T14:31:55Z

## Body

For this code:

```c++
long call8(long, __int128, __int128, __int128, long x) {
    // IR signature from Clang: define i64 @call8(i64, i64, i64, i64, i64, i128, i64)
    return x;
}
```

- GCC passes parameters in (rdi), (rsi, rdx), (rcx, r8), ([rsp+8], [rsp+16]), (r9).
- Clang/LLVM passes paremeters in (rdi), (rsi, rdx), (rcx, r8), ([rsp+8], [rsp+16]), ([rsp+24]).

Since fa1b6e6b34eb6382c451f3a06a7c52d7ac6ada1d, the i128 is no longer split between register and stack, but the SysV ABI also states:

> If there are no registers available for any eightbyte of an argument, the whole argument is passed on the stack. If registers have already been assigned for some eightbytes of such an argument, the assignments get reverted.

I (and GCC) read this as: the entire i128 gets passed on the stack and r9 becomes allocatable again. ([Godbolt](https://godbolt.org/z/zE3E3n7eP)) `CCAssignToStackWithShadow`allocates `r9`, but I think this should just be an `CCAssignToStack` that doesn't allocate r9.

Originally found by @pfent; cc @nikic 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Alexis Engelke (aengelke)

<details>
For this code:

```c++
long call8(long, __int128, __int128, __int128, long x) {
    // IR signature from Clang: define i64 @<!-- -->call8(i64, i64, i64, i64, i64, i128, i64)
    return x;
}
```

- GCC passes parameters in (rdi), (rsi, rdx), (rcx, r8), ([rsp+8], [rsp+16]), (r9).
- Clang/LLVM passes paremeters in (rdi), (rsi, rdx), (rcx, r8), ([rsp+8], [rsp+16]), ([rsp+24]).

Since fa1b6e6b34eb6382c451f3a06a7c52d7ac6ada1d, the i128 is no longer split between register and stack, but the SysV ABI also states:

&gt; If there are no registers available for any eightbyte of an argument, the whole argument is passed on the stack. If registers have already been assigned for some eightbytes of such an argument, the assignments get reverted.

I (and GCC) read this as: the entire i128 gets passed on the stack and r9 becomes allocatable again. ([Godbolt](https://godbolt.org/z/zE3E3n7eP)) `CCAssignToStackWithShadow`allocates `r9`, but I think this should just be an `CCAssignToStack` that doesn't allocate r9.

Originally found by @<!-- -->pfent; cc @<!-- -->nikic 
</details>


---

### Comment 2 - nikic

I agree this is not correct.

Just dropping the WithShadow is not going to work, because then we'll end up assigning the second half of the i128 to r9...

---

