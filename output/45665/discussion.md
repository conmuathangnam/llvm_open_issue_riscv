# __int128 not aligned to 16-byte boundary

**Author:** josephcsible
**URL:** https://github.com/llvm/llvm-project/issues/45665
**Status:** Closed
**Labels:** backend:X86, bugzilla, miscompilation
**Closed Date:** 2025-06-19T07:43:32Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46320](https://llvm.org/bz46320) |
| Version | 10.0 |
| OS | Linux |
| CC | @topperc,@RKSimon,@rotateright |

## Extended Description 
Consider this program:

```c
int main(void) {
    __int128 x = 0;
    __asm__ __volatile__(
        "movdqa  %%xmm3, %0"
        : 
        : "xm"(x)
        : "xmm3"
    );
}
```

When I compile it with `clang -O3` on x86-64 and run it, it segfaults. The problem is that it put x at address 0x402008, which is only 8-byte aligned, even though the System V ABI requires that `__int128`'s be 16-byte aligned, which `movdqa` depends on.

https://godbolt.org/z/kMJnSd

## Comments

### Comment 1 - topperc

I don't know what the right behavior for any of this is. I don't think gcc's exact behavior is fully documented. I think for "rm" it just says "the compiler chooses the most efficient one based on the current context."

I just committed a patch to make __int128 work with "x" on 64-bit targets.

I'm going to poke on fixing the datalayout string to include i128 so that we don't create incorrect alignment for the constant pool.

---

### Comment 2 - josephcsible

> `long x __attribute__((__aligned__(16))) = 0` doesn't help any of this
> because the middle end optimizes the stack location away and just passes the
> 0 to the inline assmebly. The x variable and its alignment ceases to exist.

Is that desirable behavior? I certainly find it surprising. I'd expect that a variable with an aligned attribute would have the alignment that I requested any time it exists in memory.

---

### Comment 3 - topperc

There's a lot of different things going on here.

llvm has a FIXME for supporting i128 with 'x' in X86TargetLowering::getRegForInlineAsmConstraint

If you use "xm", llvm will give priority to m. This gets passed from clang as the contraint being xm.

If you just use m then clang will set the constraint in IR as *m which means indirect memory which works correctly. In this case I believe clang created a pointer for llvm to use on the stack. This gets created with the correct alignment because the frontend knows the __int128 required alignment.

For the xm case, clang doesn't create a pointer. So when llvm uses the memory constraint it has to construct a memory location itself. It does this by first looking if the input value is a constant. If it is then a constant pool entry is created. If its not constant then a stack location is created and a store will be generated. For both of these cases datalayout is consulted to determine the alignment to use for the constant pool or stack. But datalayout doesn't have an entry for i128 so it uses the i64 entry.

"long x __attribute__((__aligned__(16))) = 0" doesn't help any of this because the middle end optimizes the stack location away and just passes the 0 to the inline assmebly. The x variable and its alignment ceases to exist.

---

### Comment 4 - josephcsible

I'm not 100% convinced that's what it is, because if I replace `__int128 x = 0;` with `long x __attribute__((__aligned__(16))) = 0;`, it still has the same problem.

---

### Comment 5 - josephcsible

I noticed I somehow transposed the assembly instruction between my report and Godbolt; the instruction here should have been `movdqa  %0, %%xmm3` (although it happens to still exhibit the bug even with that mistake).

Anyway, poking at the constraints a bit more makes this seem even weirder. In my original program, if I do `"x"(x)` instead of `"xm"(x)`, then it fails to compile with "couldn't allocate input reg for constraint 'x'", and if I do `"m"(x)` instead of `"xm"(x)`, then it emits this instead, which works fine:

```asm
 xorps  %xmm0,%xmm0
 movaps %xmm0,-0x18(%rsp)
 movdqa -0x18(%rsp),%xmm3
```

So now I have two more mysteries:

1. Why does Clang produce different output when I change the constraint from `"xm"` to `"m"` if it was choosing to use memory anyway with `"xm"`?
2. If Clang is aware enough of how XMM registers work that it can initialize the memory with them, why does it fail to use the `"x"` constraint and not just emit this?

```asm
 xorps  %xmm0,%xmm0
 movaps %xmm0,%xmm3
```

---

### Comment 6 - topperc

I think this ultimately comes down to the ABI alignment for i128 alignment not being in the DataLayout string.

---

### Comment 7 - topperc

The problem seems to be very subtle. It only happens when the variable is assigned by a constant. It doesn't happen to this

int foo(__int128 y) {
    __int128 x = y;
    __asm__ __volatile__(
        "movdqa  %0, %%xmm3"
        : 
        : "xm"(x)
        : "xmm3"
    );
}


LLVM seems to be using the m constraint and point it directly at the constant pool entry for 128 bits of 0.

---

### Comment 8 - nikic

This is fixed now that i128 alignment in the data layout is correct.

---

