# Incorrect Optimization of malloc Calls: Erroneous Assumption of Unconditional Success of malloc Calls

**Author:** jonathan-gruber-jg
**URL:** https://github.com/llvm/llvm-project/issues/121176
**Status:** Closed
**Labels:** duplicate, miscompilation, llvm:instcombine
**Closed Date:** 2024-12-27T08:43:49Z

## Body

Given calls to `malloc` that can potentially fail, Clang incorrectly assumes they will unconditionally succeed. In some cases, Clang even elides such calls to `malloc` and optimizes the surrounding code around the assumption that the call(s) to `malloc` would unconditionally succeed if actually executed.

```c
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define TOO_LARGE_SIZE ((size_t)PTRDIFF_MAX + 1) /* Request size greater than PTRDIFF_MAX. */

int test_too_big_malloc() {
	int ret;

	void *const p = malloc(TOO_LARGE_SIZE);
	ret = p != NULL;
	free(p);

	return ret;
}
```

On all the (nonzero) optimization levels that I tested, Clang generates x86_64 assembly (Intel syntax) similar to the following, where it not only completely elides the calls to `malloc` and `free` but assumes that the call to `malloc` would have succeeded if actually executed:

```asm
test_too_big_malloc:
	mov	eax, 1
	ret
```

glibc's `malloc` _unconditionally_ fails for requests of sizes greater than `PTRDIFF_MAX`, so Clang compiles the above test case incorrectly for glibc in particular.

Host system: Arch Linux, x86_64.

Clang version: official Arch Linux package of clang, version 18.1.8-5.

Command line to reproduce results: clang -c <file>.c -O<opt-level>, where <opt-level> is g, 1, 2, 3, s, or z.

## Comments

### Comment 1 - dtcxzyw

Reproducer: https://alive2.llvm.org/ce/z/UR3K-e
IIUC optimizers are allowed to assume that a unescaped alloca is nonnull. For more details, please refer to https://github.com/llvm/llvm-project/blob/5807d0efb963ab591a1ae569b538724299d6acdc/llvm/lib/Transforms/InstCombine/InstructionCombining.cpp#L3308-L3317.

Seems like alive2 treats the allocation size as a signed integer: https://alive2.llvm.org/ce/z/7xJGhq
@nikic @nunoplopes 


---

### Comment 2 - thesamesam

See also: 
* https://github.com/llvm/llvm-project/issues/28790
* https://github.com/llvm/llvm-project/issues/49826
* https://github.com/llvm/llvm-project/issues/63142
* https://github.com/llvm/llvm-project/issues/114772

Anyway, the question of size to `malloc` came up at https://inbox.sourceware.org/gcc-patches/ZyTPHKCmYoJmMQ42@tucnak/.

---

