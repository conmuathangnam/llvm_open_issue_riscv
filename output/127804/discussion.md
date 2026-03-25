# [AArch64][GlobalISel] llvm.fptoui.sat.i32.f128 miscompile

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/127804
**Status:** Closed
**Labels:** miscompilation, llvm:globalisel, release:backport
**Closed Date:** 2025-02-20T12:28:51Z

## Body

Per https://github.com/rust-lang/compiler-builtins/pull/760, LLVM 20 appears to be miscompiling fptoui.sat.i32.f128 on AArch64 when GlobalISel is used.

Assembly for LLVM 19 and LLVM 20: https://llvm.godbolt.org/z/n3K4MEG7r

Likely related to https://github.com/llvm/llvm-project/pull/96297, cc @davemgreen

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Nikita Popov (nikic)

<details>
Per https://github.com/rust-lang/compiler-builtins/pull/760, LLVM 20 appears to be miscompiling fptoui.sat.i32.f128 on AArch64 when GlobalISel is used.

Assembly for LLVM 19 and LLVM 20: https://llvm.godbolt.org/z/n3K4MEG7r

Likely related to https://github.com/llvm/llvm-project/pull/96297, cc @<!-- -->davemgreen
</details>


---

### Comment 2 - nikic

Possibly I'm misreading the code, but I suspect that the select operands are inverted? https://github.com/llvm/llvm-project/blob/760ec2c38e0cd01c016c403301e8dc081e0fc85c/llvm/lib/CodeGen/GlobalISel/LegalizerHelper.cpp#L7818-L7821 This is doing `Src < MinFloat ? Src : MinFloat`, but surely it should be `Src < MinFloat ? MinFloat : Src`?

It doesn't really help that MaxC is referring to the minimum float...

---

### Comment 3 - davemgreen

Hi - Yeah it sounds like I translated the code from using fminnum/fmaxnum badly..

Do you want me to put together a patch?

---

### Comment 4 - nikic

> Hi - Yeah it sounds like I translated the code from using fminnum/fmaxnum badly..
> 
> Do you want me to put together a patch?

Yes, please :)

---

### Comment 5 - davemgreen

/cherry-pick 70ed381b1693697dec3efcaed161d3626d16cff1

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#128001

---

