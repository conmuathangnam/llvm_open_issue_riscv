# Clang incorrectly adds tailcalls after `setjmp(...)` with `-fno-builtin`

**Author:** alanzhao1
**URL:** https://github.com/llvm/llvm-project/issues/122840
**Status:** Closed
**Labels:** clang:codegen, miscompilation
**Closed Date:** 2025-03-31T16:42:35Z

## Body

Originally reported by Chromium at https://issues.chromium.org/issues/380508212

Example:

```c
#include <setjmp.h>

struct JpegCommon {
  jmp_buf jmpbuf;
  int cinfo;
};

int jpeg_start_decompress(int *);

int jpeg_common_start_decompress(struct JpegCommon* jpeg_common) {
  if (setjmp(jpeg_common->jmpbuf) == -1) {
    return 0;
  }
  return jpeg_start_decompress(&jpeg_common->cinfo);
}
```

If this file is compiled with `-fno-builtin`, then Clang will optimize the call to `jpeg_start_decompress(...)` as a tailcall:

```asm
jpeg_common_start_decompress:
        push    rbx
        mov     rbx, rdi
        call    _setjmp@PLT
        cmp     eax, -1
        je      .LBB0_1
        add     rbx, 200
        mov     rdi, rbx
        pop     rbx
        jmp     jpeg_start_decompress@PLT
.LBB0_1:
        xor     eax, eax
        pop     rbx
        ret
```

This is incorrect. Control flow may resume at `setjmp(...)` from another function. Because of this, the contents of the execution stack must be preserved. The tail call optimization shown above is incorrect because `jpeg_start_decompress(...)` may mess up the stack for when we later enter `jpeg_common_start_decompress(...)` at `setjmp(...)` via a `longjmp(..)` call.

This incorrect codegen only occurs with `-fno-builtin` - if we don't pass `-fno-builtin`, Clang correctly emits a call instruction (thereby preserving the stack).

godbolt: https://godbolt.org/z/1anGj6e3s

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Alan Zhao (alanzhao1)

<details>
Originally reported by Chromium at https://g-issues.chromium.org/issues/380508212

Example:

```c
#include &lt;setjmp.h&gt;

struct JpegCommon {
  jmp_buf jmpbuf;
  int cinfo;
};

int jpeg_start_decompress(int *);

int jpeg_common_start_decompress(struct JpegCommon* jpeg_common) {
  if (setjmp(jpeg_common-&gt;jmpbuf) == -1) {
    return 0;
  }
  return jpeg_start_decompress(&amp;jpeg_common-&gt;cinfo);
}
```

If this file is compiled with `-fno-builtin`, then Clang will optimize the call to `jpeg_start_decompress(...)` as a tailcall:

```asm
jpeg_common_start_decompress:
        push    rbx
        mov     rbx, rdi
        call    _setjmp@<!-- -->PLT
        cmp     eax, -1
        je      .LBB0_1
        add     rbx, 200
        mov     rdi, rbx
        pop     rbx
        jmp     jpeg_start_decompress@<!-- -->PLT
.LBB0_1:
        xor     eax, eax
        pop     rbx
        ret
```

This is incorrect. Control flow may resume at `setjmp(...)` from another function. Because of this, the contents of the execution stack must be preserved. The tail call optimization shown above is incorrect because `jpeg_start_decompress(...)` may mess up the stack for when we later enter `jpeg_common_start_decompress(...)` at `setjmp(...)` via a `longjmp(..)` call.

This incorrect codegen only occurs with `-fno-builtin` - if we don't pass `-fno-builtin`, Clang correctly emits a call instruction (thereby preserving the stack).

godbolt: https://godbolt.org/z/1anGj6e3s
</details>


---

### Comment 2 - alanzhao1

The issue seems to be that clang doesn't recognize that the non-builtin `setjmp(...)` needs the `returns_twice` attribute

---

### Comment 3 - pinskia

GCC treats a few function special even without builtins for returns twice; `setjmp`, `sigsetjmp`, `savectx`, `vfork`, and `getcontext.`  All can have an optional prefix of `_` or `__` on it.
GCC has treated a subset of those that way since 1999 even.

---

### Comment 4 - jyknight

Is this really a bug? Maybe this is WAI, from the compiler side?

It just seems weird to say that the compiler is expected to continue to assign custom semantics to certain function names when you've explicitly asked _not_ to do so using `-fno-builtin`. Is there a good reason to even be using `-fno-builtin` when building libjpeg?

But, I suppose adding a hack to add `returns_twice` to `setjmp` (etc.) even under `-fno-builtins` shouldn't _break_ things -- even if code is using the name `setjmp` for some completely other purpose than the standard function, it'd just make that code less optimized than it should be.

---

### Comment 5 - alanzhao1

> Is there a good reason to even be using -fno-builtin when building libjpeg?

This is done for builds using the [centipede fuzzer](https://github.com/google/fuzztest/tree/main/centipede), which needs to intercept various calls.  https://crsrc.org/c/build/config/sanitizers/BUILD.gn;l=465-469

On the Chrome side, we are considering enabling builtins only for setjmp (and perhaps other functions) as a temporary workaround.

---

### Comment 6 - nico

> Is this really a bug? Maybe this is WAI, from the compiler side?

Wouldn't you say that it's weird that there's a driver-level flag (`-fno-builtins`) that if when used guarantees miscompiles in code using `setjmp`? Does anyone really want that?

---

### Comment 7 - jyknight

Ideally, libc headers would annotate these "magic" functions which require special handling with the returns_twice attribute, and not rely on the compiler detecting them by name. Then, it wouldn't matter.

I find it unfortunate (from a theoretical standpoint) that the compiler _ever_ detects library functions by name, instead of requiring the standard-library to apply appropriate annotations where special behavior is required. Because we do have this hack, we also need an '-fno-builtin' opt-out, for code which isn't using a normal standard library. And, ISTM that with that opt-out, it makes total sense to disable _all_ special name recognition.

But, yes, the result is a bad situation for users -- that I certainly agree with.

---

### Comment 8 - rnk

> Is this really a bug? Maybe this is WAI, from the compiler side?

I think users expect `-fno-builtin` to make the compiler more conservative, not less conservative (do more TCO in the presence of `setjmp`). The current behavior doesn't seem intentional, it's just a consequence of internal implementation details of `-fno-builtin`.

---

### Comment 9 - efriedma-quic

> The current behavior doesn't seem intentional

It's intentional to the extent that it's consistent with existing documentation for the flag, which says "Disable special handling and optimizations of well-known library functions".  And that's precisely what it does: the compiler pretends the name "setjmp" isn't special.  It's mostly intended for writing code without a C library; disabling optimizations is a side-effect.

That said, this is such a big footgun that maybe we want to do something anyway.

Maybe we could also consider adding a flag which is intended specifically for disabling optimizations, so users can control that without the other effects.

---

### Comment 10 - rnk

I see `-fno-builtin` as a GCC flag, and if GCC continues to treat `setjmp` as special in the presence of this flag, I would rely more on observed behavior than documented intent, and argue that yes, this is a big footgun that we should fix.

---

### Comment 11 - jyknight

Whether or not we change Clang, it would be really good if someone would submit a fix for the various libcs to explicitly annotate returns_twice functions.

Maybe then at least we wouldn't need to keep expanding the list of these in the future as more functions are added.

---

### Comment 12 - alanzhao1

> Whether or not we change Clang, it would be really good if someone would submit a fix for the various libcs to explicitly annotate returns_twice functions.

Let's start with our own llvm-libc: https://github.com/llvm/llvm-project/pull/124370

---

