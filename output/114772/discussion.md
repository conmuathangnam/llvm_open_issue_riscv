# Invalid optimization: clang makes invalid assumptions about malloc

**Author:** bhaible
**URL:** https://github.com/llvm/llvm-project/issues/114772
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2026-03-08T17:56:09Z

## Body

When optimizing, clang apparently assumes that a `malloc` invocation does not change `errno`. This assumption is wrong: on all platforms, `malloc` sets `errno` (usually to `ENOMEM`) when it returns NULL.

**How to reproduce:**

Save this as foo.c.
```
#include <stdlib.h> /* malloc */
#include <errno.h> /* errno */
#include <stdio.h> /* printf */
#include <stdint.h> /* SIZE_MAX */

int
main ()
{
  errno = 0;
  void *volatile p = malloc (SIZE_MAX / 10);
  int err = errno;
  printf ("errno=%d\n", err);
}
```
Then:
```
$ clang -O2 foo.c && ./a.out
errno=0
$ clang foo.c && ./a.out
errno=12
```
```
$ clang --version
clang version 19.1.0 (/home/runner/work/llvm-project/llvm-project/clang a4bf6cd7cfb1a1421ba92bca9d017b49936c55e4)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - bhaible

Likewise for `calloc`.

---

### Comment 2 - antoniofrighetto

IR before GVN: https://llvm.godbolt.org/z/qchWcEfYo. Seems like MemDep doesn't take into account that `%call` may be clobbered by the call to malloc, although I'm not completely sure it's MemDep role to establish that. Either way, I think GVN shouldn't simplify to values coming from global variables w/ thread-local storage, but we don't seem to treat errno as such. Maybe cc/ @nikic

---

### Comment 3 - nikic

@antoniofrighetto We mark malloc as `memory(inaccessiblemem: readwrite)`, which is not correct wrt errno. I expect that not doing this would be a huge pessimization though, because it basically means that malloc can now clobber all escaped memory (like, say, a function argument) because we can't exclude that it may be a pointer to errno.

---

### Comment 4 - antoniofrighetto

> @antoniofrighetto We mark malloc as `memory(inaccessiblemem: readwrite)`, which is not correct wrt errno.

Oh, sure thing, thanks. I wonder if it could still make sense to somehow treat errno differently / have specialized logic here when searching for available values to materialize, though it sounds a bit of a workaround.



---

### Comment 5 - bhaible

> I wonder if it could still make sense to somehow treat errno differently

I would hope, yes. The majority of pointers are not pointers to `int` and therefore cannot alias `errno`.

---

### Comment 6 - nikic

We recently adopted something like this by emitting int TBAA for FP libcalls. I don't think we'd want to do the same for malloc though, it's even more fishy in that case.

I'd be open to adding an errno location to MemoryEffects and have that interact with other TBAA. (cc @andykaylor as we had a related discussion at the conference.)

> I would hope, yes. The majority of pointers are not pointers to `int` and therefore cannot alias `errno`.

Even if it's not a majority, I expect a significant fraction are, esp. in benchmarky code :) There's also the issue that TBAA is a C concept, so languages that don't have strict aliasing would pessimize much more. Though hopefully they already use a different allocator than malloc.

---

### Comment 7 - amonakov

> Even if it's not a majority, I expect a significant fraction are, esp. in benchmarky code :)

People are not judging LLVM purely on the basis of performance of resulting [wrong] code, so that should not be the overriding concern.

There was an example in an earlier report [[link to comment](https://github.com/llvm/llvm-project/issues/63142#issuecomment-1615142514)] showing that user code is allowed to take the address of errno and pass that pointer around:

```c
int f(size_t sz, void **out, int *eptr)
{
    int e = *eptr;
    *out = malloc(sz);
    return *eptr - e;
}
// In another translation unit:
int test(void *ptr)
{
  errno = 0;
  return f(SIZE_MAX, &ptr, &errno);
}
```
So for TBAA malloc should interfere with signed/unsigned variants of the type of errno, plus all character types as usual.

---

### Comment 8 - antoniofrighetto

> I'd be open to adding an errno location to MemoryEffects and have that interact with other TBAA.

Just FTR, seems like TBAA is already returning ModRef for the dependency in this instance, I guess if we introduce a new ErrnoMem location, we may want AA to return ModRef if inaccessiblemem_or_errnomemonly here:

https://github.com/llvm/llvm-project/blob/f445e39ab271d07733f0f45048badd9e58905aec/llvm/lib/Analysis/AliasAnalysis.cpp#L229-L234

I'm taking a look.

---

### Comment 9 - andykaylor

> Just FTR, seems like TBAA is already returning ModRef for the dependency in this instance, I guess if we introduce a new ErrnoMem location, we may want AA to return ModRef if inaccessiblemem_or_errnomemonly here

The problem is that we don't know which pointers may be pointing to errno, as @amonakov demonstrated. So it can theoretically alias with any int pointer (or any pointer if we aren't enforcing strict aliasing). To be strictly correct, we'd need to get rid of the restriction that malloc doesn't write to accessible memory.

That's more restrictive than most people probably need, but that's what you get with errno. That's why -fno-math-errno exists, and maybe we need something similar for non-math errno usage.

I don't like the idea of setting TBAA on function calls to model their internal behavior. The thing that prompted me to bring this up with @nikic was a case where we were doing that for a math library call that takes a pointer argument, which made the TBAA information ambiguous. More generally, I don't like it because we haven't, to my knowledge, clearly defined and documented the meaning of TBAA metadata on a function call.

---

### Comment 10 - nikic

I've written a small RFC for adding an errno memory effect location: https://discourse.llvm.org/t/rfc-modelling-errno-memory-effects/82972 Let me know what you think.

---

