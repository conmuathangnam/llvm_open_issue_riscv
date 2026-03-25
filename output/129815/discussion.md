# (When) does Clang respect noinline, and how?

**Author:** higher-performance
**URL:** https://github.com/llvm/llvm-project/issues/129815

## Body

This issue appears to exist with GCC and MSVC as well, but (in my various attempts) Clang appears to be the least willing to respect noinline.

Consider this code:
```
#include <utility>

#if defined(_MSC_VER)
#define NOINLINE [[msvc::noinline]]
#elif defined(__clang__)
#define NOINLINE [[gnu::noinline]]
#elif defined(__GNUC__)
#define NOINLINE [[gnu::noinline]]
#else
#error unable to prevent inlining
#endif

using R = int;
using P = R*;

NOINLINE        R bar1(P arg);
NOINLINE        R bar2(P arg) { return arg ? 1 : 0; }
NOINLINE static R bar3(P arg) { return arg ? 1 : 0; }

R foo1(int x) { return (x ? 0 : bar1(&x)); }
R foo2(int x) { return (x ? 0 : bar2(&x)); }
R foo3(int x) { return (x ? 0 : bar3(&x)); }
```
In this code, all the `fooN` are equivalent, and:
- Must contain calls to `barN` because all `barN` are noinline (mandatory)
- Should result in identical codegen (optional, but preferable)

Instead, [what I see](https://godbolt.org/z/G9zjYY1fe) is:
- With the exception of `foo2` & `foo3` on MSVC, no pair of `fooN` result in identical codegen on any compiler
- None of the compilers produce a `call` instruction in `foo3`, implying that `noinline` isn't guaranteeing the generation of a new stack frame
- Clang is the only compiler that **completely elides** any reference to any `barN` (see `foo3`), making the `noinline` function disappear entirely in some cases.

```
Clang               │ GCC                  │ MSVC
━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━━━┿━━━━━━━━━━━━━━━━━━━━
foo1:               │ foo1:                │ foo1:
  push rax          │   sub  rsp, 24       │   mov  [rsp+8], ecx
  mov  [rsp+4], edi │   xor  eax, eax      │   sub  rsp, 40
  xor  eax, eax     │   mov  [rsp+12], edi │   test ecx, ecx
  test edi, edi     │   test edi, edi      │   je   LABEL
  je   LABEL        │   je   LABEL         │   xor  eax, eax
  pop  rcx          │   add  rsp, 24       │   add  rsp, 40
  ret               │   ret                │   ret  0
LABEL:              │ LABEL:               │ LABEL:
  lea  rdi, [rsp+4] │   lea  rdi, [rsp+12] │   lea  rcx, [rsp]
  call bar1@PLT     │   call bar1          │   call bar1
  pop  rcx          │   add  rsp, 24       │   add  rsp, 40
  ret               │   ret                │   ret  0
────────────────────┼──────────────────────┼────────────────────
foo2:               │ foo2:                │ foo2:
  xor  eax, eax     │   test edi, edi      │   test ecx, ecx
  test edi, edi     │   jne  LABEL         │   je   LABEL
  je   LABEL        │   sub  rsp, 8        │   xor  eax, eax
  ret               │   lea  rdi, [rsp+4]  │   ret  0
LABEL:              │   call bar2          │ LABEL:
  push rax          │   add  rsp, 8        │   lea  rcx, [rsp]
  lea  rdi, [rsp+4] │   ret                │   jmp  bar2
  call bar2         │ LABEL:               │
  add  rsp, 8       │   xor  eax, eax      │
  ret               │   ret                │
────────────────────┼──────────────────────┼────────────────────
foo3:               │ foo3:                │ foo3:
  xor  eax, eax     │   test edi, edi      │   test ecx, ecx
  test edi, edi     │   jne  LABEL         │   je   LABEL
  sete al           │   lea  rdi, [rsp-4]  │   xor  eax, eax
  ret               │   jmp  bar2          │   ret  0
                    │ LABEL:               │ LABEL:
                    │   xor  eax, eax      │   lea  rcx, [rsp]
                    │   ret                │   jmp  bar3
```

Note that I haven't tried LTO yet, but I imagine that would produce interesting results as well.

This made me wonder:
- What _are_ the precise semantics of `noinline`? i.e. what guarantee(s) can users actually rely on when using `noinline`, if any?
- Are these behaviors bugs, or intended behavior?

