# [X86] Wrong code for inline assembly with memory operands with segment registers

**Author:** hvdijk
**URL:** https://github.com/llvm/llvm-project/issues/187893
**Status:** Open
**Labels:** backend:X86, miscompilation

## Body

```c
int foo(int __seg_gs *p) {
  int v;
  asm ("movl %1, %0" : "=r"(v) : "m"(*p));
  return v - *p;
}
```
Compiling with GCC produces
```asm
foo:
        movl %gs:(%rdi), %eax
        subl    %gs:(%rdi), %eax
        ret
```
Compiling with Clang produces
```asm
foo:
        movl    (%rdi), %eax
        subl    %gs:(%rdi), %eax
        retq
```
Note how the segment override is missing in the instruction coming from the inline assembly.

https://godbolt.org/z/e4bG3Mc5h

This is one of the issues affecting glibc for i386 getting built with Clang: see [`sysdeps/i386/nptl/tls.h`](https://sourceware.org/git/?p=glibc.git;a=blob;f=sysdeps/i386/nptl/tls.h;h=87dcf5dc1f47462e63eb14b089c7b9792bd1f577;hb=HEAD#l255)'s definition of `THREAD_GSCOPE_RESET_FLAG()`.

This looks to be handled correctly in Clang, the generated LLVM IR still contains the address space (the `addrspace(256)` in `tail call i32 asm "movl $1, $0", "=r,*m,~{dirflag},~{fpsr},~{flags}"(ptr addrspace(256) elementtype(i32) %0)`), but the backend does not translate this in a way that preserves that.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Harald van Dijk (hvdijk)

<details>
```c
int foo(int __seg_gs *p) {
  int v;
  asm ("movl %1, %0" : "=r"(v) : "m"(*p));
  return v - *p;
}
```
Compiling with GCC produces
```asm
foo:
        movl %gs:(%rdi), %eax
        subl    %gs:(%rdi), %eax
        ret
```
Compiling with Clang produces
```asm
foo:
        movl    (%rdi), %eax
        subl    %gs:(%rdi), %eax
        retq
```
Note how the segment override is missing in the instruction coming from the inline assembly.

https://godbolt.org/z/e4bG3Mc5h

This is one of the issues affecting glibc for i386 getting built with Clang: see [`sysdeps/i386/nptl/tls.h`](https://sourceware.org/git/?p=glibc.git;a=blob;f=sysdeps/i386/nptl/tls.h;h=87dcf5dc1f47462e63eb14b089c7b9792bd1f577;hb=HEAD#l255)'s definition of `THREAD_GSCOPE_RESET_FLAG()`.

This looks to be handled correctly in Clang, the generated LLVM IR still contains the address space (the `addrspace(256)` in `tail call i32 asm "movl $1, $0", "=r,*m,~{dirflag},~{fpsr},~{flags}"(ptr addrspace(256) elementtype(i32) %0)`), but the backend does not translate this in a way that preserves that.
</details>


---

### Comment 2 - hvdijk

This also affected x86_64, but glibc added a workaround without reporting the bug to LLVM, https://sourceware.org/pipermail/libc-alpha/2025-October/171874.html. Because of that workaround, fixing this bug means that glibc will build for i386 but fail to build for x86_64. cc @zatrazz , what's the best way forward here? Do you think you could get some autoconf logic in glibc to detect whether that workaround is necessary, so that when LLVM is fixed to support this, glibc will continue to build?

---

### Comment 3 - zatrazz

@hvdijk i386 is not currently support for glibc/clang build. I have WIP [branch with the same fix](https://github.com/zatrazz/glibc/commit/310b75a6ed210fc237258173e739f21055d49159) but I did not posted it to review (it has some ABI issue on how _Float128 is implemented on clang that I am not sure how to solve). 

But I agree that a configure switch is indeed better, if clang want to eventually support this gnu extension.

---

### Comment 4 - hvdijk

> i386 is not currently support for glibc/clang build.

I know this is only one of the issues, but this is the main one that was blocking me from looking into other issues.

> But I agree that a configure switch is indeed better, if clang want to eventually support this gnu extension.

Thanks. I already have a local patch that makes this work in LLVM, I'm just not able to clean it up to make it suitable as a PR just yet, so hopefully once that configure check is in, it won't take much longer.

---

