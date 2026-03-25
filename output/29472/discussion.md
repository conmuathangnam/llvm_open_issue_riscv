# Incorrect barriers for legacy __sync_* on ARM64

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/29472
**Status:** Open
**Labels:** backend:AArch64, clang:codegen, bugzilla, miscompilation

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [29102](https://llvm.org/bz29102) |
| Version | trunk |
| OS | All |
| CC | @asl,@zygoloid,@TNorthover |

## Extended Description 
Hi,

r179005 was committed to LLVM in April, 2013. This commit removed several memory barriers around atomic operations on ARM64. The mailing list post [0] regarding the commit cites the Linux kernel as a source for this being a valid change. However, in February 2014, the Linux kernel's atomic operations were patched to contain full barriers [1]. GCC was also fixed sometime before 5.3.0 to emit full barriers. Finally, in the Mono project, we emitted the same code in our JIT's atomic intrinsics as LLVM currently does and found that it indeed suffered from the problem described in [1]. After fixing our JIT to emit the same code that the Linux kernel and GCC use, everything worked fine.

Based on all of the above, I'm inclined to think that r179005 is wrong.

[0] http://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20130408/170666.html
[1] http://lists.infradead.org/pipermail/linux-arm-kernel/2014-February/229588.html

## Comments

### Comment 1 - TNorthover

I've just been reading through the Mono history & comments, and it's convinced me to reopen this.

The issue is specifically with the legacy __sync_* operations. Those *do* appear to give extra barriers on AArch64 GCC, and the documentation (while written in horribly vague pre-C++11 terms) could be read to require them -- see https://gcc.gnu.org/onlinedocs/gcc/_005f_005fsync-Builtins.html.

GCC codegen for a generic loop is roughly:

    .L2:
        ldxr    w1, [x0]
        add     w1, w1, 4
        stlxr   w2, w1, [x0]
        cbnz    w2, .L2
        dmb     ish

This is also what Will implemented (and justified) in the referenced GCC thread on the Linux Kernel.

No matter what, this will involve Clang changes. There's a reasonable chance it could be implemented entirely in Clang (as an "atomicrmw release" followed by a fence) but I haven't thought in enough detail to convince even myself. Otherwise it would need the new super_seq_cst above, and also Clang changes to use it -- so I've put this in the Clang component.

Priority may also be an issue. This requires strong expertise and interest in the topic. That may be difficult to drum up for legacy intrinsics.

---

### Comment 2 - TNorthover

I've now had some fairly detailed discussions with people at ARM and the consensus is that this "full barrier" constraint is only in the Linux Kernel, not in C++ (which is the memory model LLVM uses).

So I'm going to close this as invalid for now.

Feel free to reopen if a C++ example turns up where we do the wrong thing, and reopen/retitle or refile to cover a super_seq_cst (name t.b.d.) ordering. Obviously it would need someone willing and able to drive the work though.

---

### Comment 3 - TNorthover

The bug is a bit vague, but to make things concrete let's take this:

    _Atomic(int) var;
    void foo() { ++var; }

Newer versions of GCC do not emit extra barriers here (I've tried 6.3 from Godbolt and 8.2 from my own PC). They just give a plain ldaxr/stlxr pair.

Also, Linux is well known to have its own ideas about memory ordering and barriers that don't necessarily match up with C++11 (which is what LLVM aims to model right now).

I'll check with Will (I know he's working on the C++11 and ARM memory models right now so hopefully he'll have some insight), and think some more about the effects of possible reorderings.

But at the moment it sounds like this might be a feature request for some kind of super_seq_cst memory order with stronger semantics than C++11 (that might apply by default to legacy __sync_* operations, I know they have volatile already for some strange reasons).

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Alex Rønne Petersen (alexrp)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [29102](https://llvm.org/bz29102) |
| Version | trunk |
| OS | All |
| CC | @<!-- -->asl,@<!-- -->zygoloid,@<!-- -->TNorthover |

## Extended Description 
Hi,

r179005 was committed to LLVM in April, 2013. This commit removed several memory barriers around atomic operations on ARM64. The mailing list post [0] regarding the commit cites the Linux kernel as a source for this being a valid change. However, in February 2014, the Linux kernel's atomic operations were patched to contain full barriers [1]. GCC was also fixed sometime before 5.3.0 to emit full barriers. Finally, in the Mono project, we emitted the same code in our JIT's atomic intrinsics as LLVM currently does and found that it indeed suffered from the problem described in [1]. After fixing our JIT to emit the same code that the Linux kernel and GCC use, everything worked fine.

Based on all of the above, I'm inclined to think that r179005 is wrong.

[0] http://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20130408/170666.html
[1] http://lists.infradead.org/pipermail/linux-arm-kernel/2014-February/229588.html
</details>


---

