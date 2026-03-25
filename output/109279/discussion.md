# Miscompilation of tail call on x86_64: RSP is erroneously increased

**Author:** cypheon
**URL:** https://github.com/llvm/llvm-project/issues/109279
**Status:** Open
**Labels:** backend:X86, miscompilation

## Body

In some cases, when calling a tailcc function in tail position, `rsp` is increased in a branch where it has never been decreased. This leads to corrupted stack pointer (and probably a segfault) in the following function.

Repro:
```llvm
declare tailcc void @f2()
declare tailcc void @f1(ptr, i64, ptr, i8, ptr, ptr, ptr, ptr, ptr, ptr)

define tailcc void @repro(i64 %0) {
  %cond = icmp ugt i64 %0, 0
  br i1 %cond, label %a, label %b
a:
  musttail call tailcc void @f1(ptr null, i64 16, ptr null, i8 1, ptr null, ptr null, ptr null, ptr null, ptr null, ptr null)
  ret void
b:
  musttail call tailcc void @f2()
  ret void
}
```

Resulting assembly:
```asm
repro:                                  # @repro
# %bb.0:
        test    rdi, rdi
        je      .LBB0_2
# %bb.1:                                # %a
        sub     rsp, 32
        xorps   xmm0, xmm0
        movaps  xmmword ptr [rsp + 8], xmm0
        mov     rax, qword ptr [rsp + 32]
        movaps  xmmword ptr [rsp + 24], xmm0
        mov     qword ptr [rsp], rax
        mov     esi, 16
        xor     edi, edi
        xor     edx, edx
        mov     ecx, 1
        xor     r8d, r8d
        xor     r9d, r9d
        jmp     f1@PLT                          # TAILCALL
.LBB0_2:                                # %b
        add     rsp, 32  # <- rsp should not be increased here, as it is only decreased in branch "%a"
        jmp     f2@PLT                          # TAILCALL
                                        # -- End function
```
https://godbolt.org/z/ze7r8j67o

As far as I can see, the issue does not occur on aarch64. There the stack pointer is decremented right at the beginning (before the branch), so the increment before the tail call is correct.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Johann Rudloff (cypheon)

<details>
In some cases, when calling a tailcc function in tail position, `rsp` is increased in a branch where it has never been decreased. This leads to corrupted stack pointer (and probably a segfault) in the following function.

Repro:
```llvm
declare tailcc void @<!-- -->f2()
declare tailcc void @<!-- -->f1(ptr, i64, ptr, i8, ptr, ptr, ptr, ptr, ptr, ptr)

define tailcc void @<!-- -->repro(i64 %0) {
  %cond = icmp ugt i64 %0, 0
  br i1 %cond, label %a, label %b
a:
  musttail call tailcc void @<!-- -->f1(ptr null, i64 16, ptr null, i8 1, ptr null, ptr null, ptr null, ptr null, ptr null, ptr null)
  ret void
b:
  musttail call tailcc void @<!-- -->f2()
  ret void
}
```

Resulting assembly:
```asm
repro:                                  # @<!-- -->repro
# %bb.0:
        test    rdi, rdi
        je      .LBB0_2
# %bb.1:                                # %a
        sub     rsp, 32
        xorps   xmm0, xmm0
        movaps  xmmword ptr [rsp + 8], xmm0
        mov     rax, qword ptr [rsp + 32]
        movaps  xmmword ptr [rsp + 24], xmm0
        mov     qword ptr [rsp], rax
        mov     esi, 16
        xor     edi, edi
        xor     edx, edx
        mov     ecx, 1
        xor     r8d, r8d
        xor     r9d, r9d
        jmp     f1@<!-- -->PLT                          # TAILCALL
.LBB0_2:                                # %b
        add     rsp, 32  # &lt;- rsp should not be increased here, as it is only decreased in branch "%a"
        jmp     f2@<!-- -->PLT                          # TAILCALL
                                        # -- End function
```
https://godbolt.org/z/ze7r8j67o

As far as I can see, the issue does not occur on aarch64. There the stack pointer is decremented right at the beginning (before the branch), so the increment before the tail call is correct.
</details>


---

### Comment 2 - cypheon

I was able to narrow this down to the interaction of the "shrink-wrap" optimisation combined with musttail tail calls.

Passing `--enable-shrink-wrap=false` to `llc` results in correctly compiled code.

---

### Comment 3 - ahmedbougacha

We also discovered this on `swifttail` musttail calls (which are generally treated the same as `tail`).
Looking into it a bit, X86ExpandPseudo emits the RSP add when lowering tail-calls, but as far as I can tell that's eventually based on a function-wide high-watermark delta (FI TCReturnAddrDelta), which indeed seems at odds with shrink-wrapping.
For now I crudely worked around this by disabling shrink-wrapping;  there might be a proper fix to be made in TC lowering, or at least some finer-grained disablement, but my knowledge of the x86 backend is very rusty, so I'll defer to others

---

