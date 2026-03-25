# powerpc64le: power9 codgen bug when generating setb instructions

**Author:** pmur
**URL:** https://github.com/llvm/llvm-project/issues/143989
**Status:** Closed
**Labels:** backend:PowerPC, miscompilation
**Closed Date:** 2025-08-08T07:24:24Z

## Body

The rust frontend ran into a bug when running tests on ppc64le with pgo enabled on [centos](https://kojihub.stream.centos.org/kojifiles/vol/koji02/packages/rust/1.86.0/1.el10/data/logs/ppc64le/build.log). The compilation of the compiler's `rustc_errors` crate with `-mcpu=power9` created a bug where a boolean value is clobbered, resulting in incorrect json values.

I was able to extract the llvm input from rust, and run `llvm-reduce` to produce a reasonably sized reproducer using the upstream llvm repo. 

The following codegen is suspect:
```
        andi. 3, 4, 1
        setb 3, 2
        cmpldi  4, 0
        std 28, 80(1)                           # 8-byte Folded Spill
        stw 3, 60(1)
```
Notably, `cr2` in the `setb` above has no live value (I am assuming this is following the ELFv2 ABI).

After the `machine-cp` pass, a copy from `cr0gt` to `cr2lt` goes missing. Likewise, the other bits of `cr2` appear to be undefined. Furthermore, `setb` potentially examines all bits of the input `cr`, so moving one bit is not sufficient for well-defined behavior.

Attached is the reduced llvm input [reduced.ll.txt](https://github.com/user-attachments/files/20717223/reduced.ll.txt). Likewise, I assembled it via:
```
llc -march=ppc64le -mcpu=pwr9 reduced.ll.txt
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Paul Murphy (pmur)

<details>
The rust frontend ran into a bug when running tests on ppc64le with pgo enabled on [centos](https://kojihub.stream.centos.org/kojifiles/vol/koji02/packages/rust/1.86.0/1.el10/data/logs/ppc64le/build.log). The compilation of the compiler's `rustc_errors` crate with `-mcpu=power9` created a bug where a boolean value is clobbered, resulting in incorrect json values.

I was able to extract the llvm input from rust, and run `llvm-reduce` to produce a reasonably sized reproducer using the upstream llvm repo. 

The following codegen is suspect:
```
        andi. 3, 4, 1
        setb 3, 2
        cmpldi  4, 0
        std 28, 80(1)                           # 8-byte Folded Spill
        stw 3, 60(1)
```
Notably, `cr2` in the `setb` above has no live value (I am assuming this is following the ELFv2 ABI).

After the `machine-cp` pass, a copy from `cr0gt` to `cr2lt` goes missing. Likewise, the other bits of `cr2` appear to be undefined. Furthermore, `setb` potentially examines all bits of the input `cr`, so moving one bit is not sufficient for well-defined behavior.

Attached is the reduced llvm input [reduced.ll.txt](https://github.com/user-attachments/files/20717223/reduced.ll.txt). Likewise, I assembled it via:
```
llc -march=ppc64le -mcpu=pwr9 reduced.ll.ltxt
```
</details>


---

### Comment 2 - pmur

Investigating this issue further, lowering `SPILL_CRBIT` on power9 needs to provide a hint the specific crbit of the crfield is being used.

While investigating, I noticed power10 lowering of `SPILL_CRBIT` also suffers from the same bug.

---

