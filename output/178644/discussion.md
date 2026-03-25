# [AArch64] llvm.experimental.cttz.elts.i64.v4i1 appears to be lowered incorrectly with SVE

**Author:** fhahn
**URL:** https://github.com/llvm/llvm-project/issues/178644
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, SVE
**Closed Date:** 2026-02-03T10:22:56Z

## Body

The IR snippet in https://godbolt.org/z/j9h98rcKT produces incorrect results with `-mcpu=neoverse-v2`, but works correctly with plain NEON. It looks like the major difference is that `llvm.experimental.cttz.elts.i64.v4i1 ` is lowered to SVE instructions, so I suspect this lowering is does not match the definition of the intrinsic.


To reproduce end-to-end, check out [aac5f40ab2fe91418e8727d4276bdcb5b08e1a70 from
](https://github.com/llvm/llvm-test-suite/commit/aac5f40ab2fe91418e8727d4276bdcb5b08e1a70) and build with `-mcpu=neoverse-v2` + run `SingleSource/UnitTests/Vectorizer/early-exit` 

This should print a mismatch:

```
Checking early_exit_find_step_1
Miscompare for interleave-forced: 4 != 8
```

## Comments

### Comment 1 - fhahn

cc @david-arm @huntergr-arm @sdesmalen-arm 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Florian Hahn (fhahn)

<details>
The IR snippet in https://godbolt.org/z/j9h98rcKT produces incorrect results with `-mcpu=neoverse-v2`, but works correctly with plain NEON. It looks like the major difference is that `llvm.experimental.cttz.elts.i64.v4i1 ` is lowered to SVE instructions, so I suspect this lowering is does not match the definition of the intrinsic.


To reproduce end-to-end, check out [aac5f40ab2fe91418e8727d4276bdcb5b08e1a70 from
](https://github.com/llvm/llvm-test-suite/commit/aac5f40ab2fe91418e8727d4276bdcb5b08e1a70) and build with `-mcpu=neoverse-v2` + run `SingleSource/UnitTests/Vectorizer/early-exit` 

This should print a mismatch:

```
Checking early_exit_find_step_1
Miscompare for interleave-forced: 4 != 8
```
</details>


---

### Comment 3 - david-arm

Thanks, we'll take a look! It's possible that the general predicate for `brkb    p1.b, p2/z, p1.b` is incorrect for fixed-width vectors if the predicate input p1 is empty. It seems like we create p2 with just `ptrue p2.h` when perhaps it should be `ptrue p2.h, vl4`?

---

