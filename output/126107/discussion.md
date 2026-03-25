# [MachineCP] Assertion `Reg.isPhysical()' failed in MCRegUnitIterator

**Author:** aleks-tmb
**URL:** https://github.com/llvm/llvm-project/issues/126107
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-02-10T17:26:35Z

## Body

During our local testing, we encountered the assertion failure `Reg.isPhysical()` in the `MachineCP` pass.

Reduced reproducer:
 ```
---
name:            main
body:             |
  bb.0.entry:  
    liveins: $ymm7
    renamable $ymm6 = COPY killed renamable $ymm7
    CALL64r killed renamable $rax, csr_64_mostregs
    renamable $ymm6 = VPADDWZ256rr $ymm6, $ymm6
```
Steps to reproduce:
```
$ bin/llc  -run-pass machine-cp test.mir
llc: /root/llvm-project/llvm/include/llvm/MC/MCRegisterInfo.h:649: llvm::MCRegUnitIterator::MCRegUnitIterator(llvm::MCRegister, const llvm::MCRegisterInfo*): Assertion `Reg.isPhysical()' failed
```
Proof: https://godbolt.org/z/PnqdcKbfe

The problematic patch seems to be: https://github.com/llvm/llvm-project/commit/9e436c2daa446da05e9219f0e6a22f932ba8e3cb

Based on my investigation, the issue arises when we remove `renamable $ymm6 = COPY killed renamable $ymm7` due to regmask clobbering. However, during this process, we skip dropping the corresponding RegUnit from the tracking copies, since it's preserved:

```c++
        LLVM_DEBUG(dbgs() << "MCP: Removing copy due to regmask clobbering: ";
                   MaybeDead->dump());

        // Invalidate all entries in the copy map which are not preserved by
        // this register mask.
        for (unsigned RegUnit : TRI->regunits(Reg))
          if (!PreservedRegUnits.test(RegUnit))
            Tracker.clobberRegUnit(RegUnit, *TRI, *TII, UseCopyInstr);

        // erase() will return the next valid iterator pointing to the next
        // element after the erased one.
        DI = MaybeDeadCopies.erase(DI);
        MaybeDead->eraseFromParent();
```

As a result, we retain a pointer to the erased instruction in tracking copies, which leads to a failure when accessing it while processing `renamable $ymm6 = VPADDWZ256rr $ymm6, $ymm6`, clobbering any previous copy definitions.

@ostannard hi, could you please take a look?


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Aleksandr Popov (aleks-tmb)

<details>
During our local testing, we encountered the assertion failure `Reg.isPhysical()` in the `MachineCP` pass.

Reduced reproducer:
 ```
---
name:            main
body:             |
  bb.0.entry:  
    liveins: $ymm7
    renamable $ymm6 = COPY killed renamable $ymm7
    CALL64r killed renamable $rax, csr_64_mostregs
    renamable $ymm6 = VPADDWZ256rr $ymm6, $ymm6
```
Steps to reproduce:
```
$ bin/llc  -run-pass machine-cp test.mir
llc: /root/llvm-project/llvm/include/llvm/MC/MCRegisterInfo.h:649: llvm::MCRegUnitIterator::MCRegUnitIterator(llvm::MCRegister, const llvm::MCRegisterInfo*): Assertion `Reg.isPhysical()' failed
```
Proof: https://godbolt.org/z/PnqdcKbfe

The problematic patch seems to be: https://github.com/llvm/llvm-project/commit/9e436c2daa446da05e9219f0e6a22f932ba8e3cb

Based on my investigation, the issue arises when we remove `renamable $ymm6 = COPY killed renamable $ymm7` due to regmask clobbering. However, during this process, we skip dropping the corresponding RegUnit from the tracking copies, since it's preserved:

```c++
        LLVM_DEBUG(dbgs() &lt;&lt; "MCP: Removing copy due to regmask clobbering: ";
                   MaybeDead-&gt;dump());

        // Invalidate all entries in the copy map which are not preserved by
        // this register mask.
        for (unsigned RegUnit : TRI-&gt;regunits(Reg))
          if (!PreservedRegUnits.test(RegUnit))
            Tracker.clobberRegUnit(RegUnit, *TRI, *TII, UseCopyInstr);

        // erase() will return the next valid iterator pointing to the next
        // element after the erased one.
        DI = MaybeDeadCopies.erase(DI);
        MaybeDead-&gt;eraseFromParent();
```

As a result, we retain a pointer to the erased instruction in tracking copies, which leads to a failure when accessing it while processing `renamable $ymm6 = VPADDWZ256rr $ymm6, $ymm6`, clobbering any previous copy definitions.

@<!-- -->ostannard hi, could you please take a look?

</details>


---

### Comment 2 - ostannard

This will be fixed by #125868 (I've checked that the assertion does go away with that patch applied), so I'll assign this to @jsji.

---

