# asm goto miscompilation

**Author:** dvyukov
**URL:** https://github.com/llvm/llvm-project/issues/74483
**Status:** Closed
**Labels:** miscompilation, llvm:regalloc, confirmed, inline-asm
**Closed Date:** 2025-03-24T08:23:44Z

## Body

As far as I understand `asm goto` now supports output arguments on all paths. If I am reading this correctly, in this case `asm goto` corrupts local state (output variable has incorrect value). The code is:

```
#include <stdio.h>

long foo(long a, long b) {
    long x, y, z;
    asm goto(
      "movq %[a], %[x]; testq %[a], %[a]; jnz %l[label1]"
        : [x] "=&r"(x), [y] "=&r"(y), [z] "=&r"(z)
        : [a]"r"(a), [b]"r"(b)
        : "memory" : label1, label2);
    return 0;
    {
        label1:
        return x;
    }
    label2:
    return 1;
}

int main() {
  printf("%lu\n", foo(11, 22));
}
```

With `-O3` the program correctly prints 11:
https://godbolt.org/z/x17GnP4nK

But with `-O0` it prints 0:
https://godbolt.org/z/h7MM9YTfP

The generated code is:
```
foo(long, long):                               # @foo(long, long)
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -16(%rbp)
        movq    %rsi, -24(%rbp)
        movq    -16(%rbp), %rsi
        movq    -24(%rbp), %rdi
        movq    %rsi, %rax
        testq   %rsi, %rsi
        jne     .LBB0_2
        movq    %rdx, -96(%rbp)                 # 8-byte Spill
        movq    %rcx, -88(%rbp)                 # 8-byte Spill
        movq    %rax, -80(%rbp)                 # 8-byte Spill
        movq    %rdx, -72(%rbp)                 # 8-byte Spill
        movq    %rcx, -64(%rbp)                 # 8-byte Spill
        movq    %rax, -56(%rbp)                 # 8-byte Spill
        jmp     .LBB0_1
.LBB0_1:
        movq    -72(%rbp), %rax                 # 8-byte Reload
        movq    -64(%rbp), %rcx                 # 8-byte Reload
        movq    -56(%rbp), %rdx                 # 8-byte Reload
        movq    %rdx, -32(%rbp)
        movq    %rcx, -40(%rbp)
        movq    %rax, -48(%rbp)
        movq    $0, -8(%rbp)
        jmp     .LBB0_6
.LBB0_2:                                # Block address taken
        movq    %rdx, -96(%rbp)                 # 8-byte Spill
        movq    -80(%rbp), %rdx                 # 8-byte Reload
        movq    %rcx, -88(%rbp)                 # 8-byte Spill
        movq    -88(%rbp), %rcx                 # 8-byte Reload
        movq    %rax, -80(%rbp)                 # 8-byte Spill
        movq    -96(%rbp), %rax                 # 8-byte Reload
        movq    %rdx, -32(%rbp)
        movq    %rcx, -40(%rbp)
        movq    %rax, -48(%rbp)
        jmp     .LBB0_4
.LBB0_3:                                # Block address taken
        movq    %rdx, -96(%rbp)                 # 8-byte Spill
        movq    -80(%rbp), %rdx                 # 8-byte Reload
        movq    %rcx, -88(%rbp)                 # 8-byte Spill
        movq    -88(%rbp), %rcx                 # 8-byte Reload
        movq    %rax, -80(%rbp)                 # 8-byte Spill
        movq    -96(%rbp), %rax                 # 8-byte Reload
        movq    %rdx, -32(%rbp)
        movq    %rcx, -40(%rbp)
        movq    %rax, -48(%rbp)
        jmp     .LBB0_5
.LBB0_4:
        movq    -32(%rbp), %rax
        movq    %rax, -8(%rbp)
        jmp     .LBB0_6
.LBB0_5:
        movq    $1, -8(%rbp)
.LBB0_6:
        movq    -8(%rbp), %rax
        popq    %rbp
        retq
```


If we track the `x` variable, it's spilled to `-80(%rbp)`, but then loaded from `-32(%rbp)`:
```
foo(long, long):                               # @foo(long, long)
...
        movq    %rsi, %rax
...
        jne     .LBB0_2
...
.LBB0_2:                                # Block address taken
...
        movq    %rax, -80(%rbp)                 # 8-byte Spill
...
        jmp     .LBB0_4
...
.LBB0_4:
        movq    -32(%rbp), %rax
        movq    %rax, -8(%rbp)
        jmp     .LBB0_6
...
.LBB0_6:
        movq    -8(%rbp), %rax
        popq    %rbp
        retq
```



## Comments

### Comment 1 - dvyukov

cc @david-xl @nickdesaulniers 

---

### Comment 2 - nickdesaulniers

I think the issue is introduced during regallocfast (hence it only being reproducible at `-O0`):
```
bb.0.entry:
  successors: %bb.1, %bb.2, %bb.3
  liveins: $rdi, $rsi
  MOV64mr %stack.1.a.addr, 1, $noreg, 0, $noreg, killed renamable $rdi :: (store (s64) into %ir.a.addr)
  MOV64mr %stack.2.b.addr, 1, $noreg, 0, $noreg, killed renamable $rsi :: (store (s64) into %ir.b.addr)
  renamable $rsi = MOV64rm %stack.1.a.addr, 1, $noreg, 0, $noreg :: (dereferenceable load (s64) from %ir.a.addr)
  renamable $rdi = MOV64rm %stack.2.b.addr, 1, $noreg, 0, $noreg :: (dereferenceable load (s64) from %ir.b.addr)
  INLINEASM_BR &"movq $3, $0; testq $3, $3; jnz ${5:l}" [mayload] [maystore] [attdialect], $0:[regdef-ec:GR64_NOREX2], 
def early-clobber renamable $rax, $1:[regdef-ec:GR64_NOREX2], def early-clobber renamable $rcx, $2:[regdef-ec:GR64_NORE
X2], def early-clobber renamable $rdx, $3:[reguse:GR64_NOREX2], killed renamable $rsi, $4:[reguse:GR64_NOREX2], killed 
renamable $rdi, $5:[imm], %bb.2, $6:[imm], %bb.3, $7:[clobber], implicit-def dead early-clobber $df, $8:[clobber], impl
icit-def early-clobber $fpsw, $9:[clobber], implicit-def dead early-clobber $eflags, !6
  MOV64mr %stack.11, 1, $noreg, 0, $noreg, $rdx :: (store (s64) into %stack.11)
  MOV64mr %stack.10, 1, $noreg, 0, $noreg, $rcx :: (store (s64) into %stack.10)
  MOV64mr %stack.9, 1, $noreg, 0, $noreg, $rax :: (store (s64) into %stack.9)
  MOV64mr %stack.8, 1, $noreg, 0, $noreg, killed $rdx :: (store (s64) into %stack.8)
  MOV64mr %stack.7, 1, $noreg, 0, $noreg, killed $rcx :: (store (s64) into %stack.7)
  MOV64mr %stack.6, 1, $noreg, 0, $noreg, killed $rax :: (store (s64) into %stack.6)
  JMP_1 %bb.1
  ...
  bb.2.label1.split (machine-block-address-taken, inlineasm-br-indirect-target):
; predecessors: %bb.0
  successors: %bb.4
  liveins: $rax, $rcx, $rdx
  MOV64mr %stack.11, 1, $noreg, 0, $noreg, $rdx :: (store (s64) into %stack.11)
  $rdx = MOV64rm %stack.9, 1, $noreg, 0, $noreg :: (load (s64) from %stack.9)
  MOV64mr %stack.10, 1, $noreg, 0, $noreg, $rcx :: (store (s64) into %stack.10)
  $rcx = MOV64rm %stack.10, 1, $noreg, 0, $noreg :: (load (s64) from %stack.10)
  MOV64mr %stack.9, 1, $noreg, 0, $noreg, $rax :: (store (s64) into %stack.9)
  $rax = MOV64rm %stack.11, 1, $noreg, 0, $noreg :: (load (s64) from %stack.11)
  MOV64mr %stack.3.x, 1, $noreg, 0, $noreg, killed renamable $rdx :: (store (s64) into %ir.x)
  MOV64mr %stack.4.y, 1, $noreg, 0, $noreg, killed renamable $rcx :: (store (s64) into %ir.y)
  MOV64mr %stack.5.z, 1, $noreg, 0, $noreg, killed renamable $rax :: (store (s64) into %ir.z)
  JMP_1 %bb.4
  ...
  bb.4.label1:
; predecessors: %bb.2
  successors: %bb.6

  renamable $rax = MOV64rm %stack.3.x, 1, $noreg, 0, $noreg :: (load (s64) from %ir.x)
  MOV64mr %stack.0.retval, 1, $noreg, 0, $noreg, killed renamable $rax :: (store (s64) into %ir.retval)
  JMP_1 %bb.6
...
bb.6.return:
; predecessors: %bb.5, %bb.4, %bb.1

  renamable $rax = MOV64rm %stack.0.retval, 1, $noreg, 0, $noreg :: (load (s64) from %ir.retval)
  RET64 implicit killed $rax
```

That `$rdx = MOV64rm %stack.9, 1, $noreg, 0, $noreg :: (load (s64) from %stack.9)` in `bb.2.label1.split` loads garbage from an uninitialized stack slot.  Tracing that through, the value loaded into `rdx` eventually gets returned.  Instead I would have expected the stores to the stack slots to come first, then load/stores to the `%ir.*` values.

---

### Comment 3 - dvyukov

@nickdesaulniers It looks like this miscompilation affects release build as well.
I only reproduced it on a very large function so far, but I can provide an internal reproducer.
This is pretty nasty b/c currently I have:
```
#ifndef __OPTIMIZE__
  // Workaround llvm bug https://github.com/llvm/llvm-project/issues/74483
  ... more code that re-loads data here ...
#endif
```
which is fine-ish since it affected debug build only. But if I enable this work-around in release mode, it will slow down the hottest code.

---

### Comment 4 - nickdesaulniers

cc @MatzeB

The issue seems to stem from multiple outputs; the reloads (via `RegAllocFast::displacePhysReg`) are being interspersed with the spills added by 9cec2b246e719533723562950e56c292fe5dd5ad.

---

