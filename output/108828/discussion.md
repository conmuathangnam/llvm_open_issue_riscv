# Miscompilation (bad optimization) - results change at `-O1`

**Author:** ericastor
**URL:** https://github.com/llvm/llvm-project/issues/108828
**Status:** Closed
**Labels:** backend:X86, miscompilation, confirmed
**Closed Date:** 2025-01-15T06:29:41Z

## Body

I've had trouble minimizing this particular issue - `bugpoint` makes changes that start causing timeouts, and I don't know which passes are even at fault - but I've got an example of LLVM-IR code that produces different results (when run through either `lli` and `llc`) before and after optimization at `-O1`.

Before optimization, the attached file executes and exits with code 0; after optimization, it executes & exits with code 1.

Example code: [linked.ll.txt](https://github.com/user-attachments/files/17014703/linked.ll.txt)



## Comments

### Comment 1 - dtcxzyw

Bisected to SROAPass on ____sample__main_partition_0. Reducing :)


---

### Comment 2 - dtcxzyw

> Bisected to SROAPass on ____sample__main_partition_0. Reducing :)

The SROA transform is verified by alive2: https://alive2.llvm.org/ce/z/Xq9y5b.
I guess it is a x86-backend issue:
```
> bin/opt -O1 linked.ll -S -o opt.ll
> bin/lli linked.ll
> echo $?
0
> bin/lli opt.ll
> echo $?
1
> bin/lli -jit-kind=mcjit -O0 opt.ll
> echo $?
0
```


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Eric Astor (ericastor)

<details>
I've had trouble minimizing this particular issue - `bugpoint` makes changes that start causing timeouts, and I don't know which passes are even at fault - but I've got an example of LLVM-IR code that produces different results (when run through either `lli` and `llc`) before and after optimization at `-O1`.

Before optimization, the attached file executes and exits with code 0; after optimization, it executes &amp; exits with code 1.

Example code: [linked.ll.txt](https://github.com/user-attachments/files/17014703/linked.ll.txt)


</details>


---

### Comment 4 - abhishek-kaushik22

Not reproducible anymore

```
$ opt -O1 linked.ll -S -o opt.ll
$ lli opt.ll
$ echo $?
0
```

---

