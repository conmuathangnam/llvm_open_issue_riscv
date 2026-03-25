# [Backend][X86] Incorrect output + missed optimization for function marked as clang::musttail

**Author:** huangjd
**URL:** https://github.com/llvm/llvm-project/issues/56891
**Status:** Closed
**Labels:** backend:X86, miscompilation, missed-optimization
**Closed Date:** 2026-01-14T22:58:19Z

## Body

As of commit 4bd9d98bc2be3548418dc67accc662cbeb10a2e1

When compiling this source code 

```c
struct A {
    int a[5];
};

extern int G(A a);

int F(A a) {
    [[clang::musttail]] return G(a);
}
```

with `-O3 -target x86_64-linux`, clang generates the following assembly (only showing relevant part)
```asm
_Z1F1A:                                 # @_Z1F1A
	.cfi_startproc
# %bb.0:                                # %entry
	movl	24(%rsp), %eax
	movl	%eax, 16(%rsp)
	movaps	8(%rsp), %xmm0
	movups	%xmm0, (%rsp)
	movaps	%xmm0, 8(%rsp)
	movl	%eax, 24(%rsp)
	jmp	_Z1G1A@PLT                      # TAILCALL
.Lfunc_end0:
```

`movups	%xmm0, (%rsp)` overwrites the return address of caller `F`, which will result in a crash.

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-bug

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-backend-x86

---

### Comment 3 - huangjd

In addition, there should not be any attempt to copy the argument as this is a sibcall. 

---

### Comment 4 - huangjd

Proposed fix at [D131034](https://reviews.llvm.org/D131034)

---

