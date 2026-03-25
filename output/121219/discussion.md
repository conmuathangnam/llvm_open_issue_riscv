# Optimization of malloc/free Produces Undefined Behavior

**Author:** jonathan-gruber-jg
**URL:** https://github.com/llvm/llvm-project/issues/121219
**Status:** Closed
**Labels:** duplicate, miscompilation, llvm:instcombine
**Closed Date:** 2024-12-28T15:05:42Z

## Body

Optimization of `malloc` and `free` calls can produce undefined behavior.

Consider this minimal test case:
```c
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define TOO_LARGE_SIZE ((size_t)PTRDIFF_MAX + 1) /* Request size greater than PTRDIFF_MAX. */

ptrdiff_t test(void) {
	char *const p = malloc(TOO_LARGE_SIZE);

	if (!p) {
		return 0;
	}

	char *const q = p + TOO_LARGE_SIZE;

	const ptrdiff_t ret = q - p;

	free(p);

	return ret;
}
```

Critically, glibc's `malloc` unconditionally fails for requests larger than `PTRDIFF_MAX`, so, with glibc, the above function `test` would always return `0`. However, on all nonzero optimization levels, Clang assumes that the call to `malloc` will unconditionally succeed, effectively deleting the `if`-statement. As a result, the function invokes undefined behavior by computing a pointer subtraction whose result overflows (since `TOO_LARGE_SIZE` > `PTRDIFF_MAX`). If the `if`-statement were not deleted, the body of the `if`-statement would always be executed, thereby avoiding invoking undefined behavior.

On all nonzero optimization levels, Clang generates (x86_64, Intel syntax) assembly similar to the following:
```asm
test:
	movabs	rax, -9223372036854775808
	ret
```

Host system type: Arch Linux, x86_64 (with glibc, of course).
Clang version: official Arch Linux package clang 18.1.8-5.

## Comments

### Comment 1 - hstk30-hw

https://github.com/llvm/llvm-project/blob/48bf0a9457fd60d0872d9b9b4804a95c833a72e1/llvm/lib/Transforms/InstCombine/InstructionCombining.cpp#L3305-L3317

---

