# malloc&co. are not noalias w.r.t errno

**Author:** amonakov
**URL:** https://github.com/llvm/llvm-project/issues/63142
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2026-03-08T17:56:10Z

## Body

Consider
```c
#include <stdlib.h>
#include <errno.h>

int f(long sz, void **out)
{
    int e = errno;
    *out = malloc(sz);
    return errno - e;
}
```
Starting from llvm-12 this unconditionally returns zero, but POSIX is clear that malloc may set errno on failure, and this is observable to the caller.

## Comments

### Comment 1 - tarvlad

llvm-16.0.5, glibc 2.37 : 
f(32 * INT_MAX, &some_var) returns 12 (correct behavior). Which version of glibc/libc you use?

Upd: (forgot to check with optimizations, sorry)

---

### Comment 2 - amonakov

The problem is apparent from the produced assembly at -O2 (return value is in EAX, which is unconditionally zero): https://godbolt.org/z/3r3z9xMKd
```asm
f:                                      # @f
        push    rbx
        mov     rbx, rsi
        call    malloc@PLT
        mov     qword ptr [rbx], rax
        xor     eax, eax
        pop     rbx
        ret
```

---

### Comment 3 - tarvlad

There is a problem in `llvm/lib/Transforms/Utils/BuildLibCalls.cpp` - for `malloc` function attribute `onlyAccessesInaccessibleMemory` is on -> it's wrong because `errno` variable accessible from any build module where `cerrno` is included. But, if we delete this attribute, we kill optimizing other accessible variables, because malloc can change _only_ `errno` and nothing more in accessible memory. Thinking about what to do with this.

---

### Comment 4 - nikic

Main problem is that there isn't really a good way for LLVM to distinguish an errno access from a different access, as it is not specified how that access is supposed to be performed. For example on musl `errno` is defined as `*__errno_location()`. We'd need libc-specific knowledge about errno accesses.

---

### Comment 5 - amonakov

It's valid to take the address of errno and pass it around as a pointer, so you just need to know through which types it can be accessed (at least `char`/`signed char`/`unsigned char` as well as signed and unsigned variants of the type of errno). Consider 
```c
#include <stdlib.h>

int f(long sz, void **out, int *eptr)
{
    int e = *eptr;
    *out = malloc(sz);
    return *eptr - e;
}
```
and f being called with `f(sz, &ptr, &errno)`.

---

