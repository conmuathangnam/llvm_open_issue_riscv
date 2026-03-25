# [MachineLICM] Wrong codegen post-RA due to ignored EH landing pad clobbers

**Author:** uweigand
**URL:** https://github.com/llvm/llvm-project/issues/122315
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2025-04-25T20:27:29Z

## Body

Compiling the following test case on SystemZ:

```
declare i64 @personality(i64, i64, i64, ptr, ptr)
declare void @callee(i64, i64, i64, i64, i64)
declare void @panic()

define void @test() uwtable personality ptr @personality {
start:
  br label %bb1

bb1:
  call void @callee(i64 0, i64 0, i64 0, i64 0, i64 0)
  invoke void @panic()
          to label %bb2 unwind label %bb3

bb2:
  ret void

bb3:
  %lp = landingpad { ptr, i32 }
          catch ptr null
  br label %bb1
}
```

results in:

```
        stmg    %r6, %r15, 48(%r15)
        aghi    %r15, -160
        lghi    %r6, 0
.LBB0_1:                                # %bb1
                                        # =>This Inner Loop Header: Depth=1
        lghi    %r2, 0
        lghi    %r3, 0
        lghi    %r4, 0
        lghi    %r5, 0
        brasl   %r14, callee@PLT
.Ltmp0:
        brasl   %r14, panic@PLT
.Ltmp1:
        j       .LBB0_3
.LBB0_2:                                # %bb3
                                        #   in Loop: Header=BB0_1 Depth=1
.Ltmp2:
        j       .LBB0_1
.LBB0_3:                                # %bb2
        lmg     %r6, %r15, 208(%r15)
        br      %r14
```

The problem is that the initalization of `%r6` was pulled out of the loop.  However, the loop includes a path where the `panic` routine throws, and the exception is caught at `.Ltmp2`.  At this point the register `%r6` was clobbered by unwind library code - it is used as the "exception pointer register".

Looking at `MachineLICMImpl::HoistRegionPostRA`, there is special code to handle "funclet entry block" by acknowledging they'll implicitly clobber some registers:
```
    // Funclet entry blocks will clobber all registers
    if (const uint32_t *Mask = BB->getBeginClobberMask(TRI))
      applyBitsNotInRegMaskToRegUnitsMask(*TRI, RUClobbers, Mask);
```

However, this does not handle Linux-style EH landing pad blocks.

If I add the following code immediately after the above:
```
    // EH landing pads clobber exception pointer/selector registers
    if (BB->isEHPad()) {
      const MachineFunction &MF = *BB->getParent();
      if (MF.getFunction().hasPersonalityFn()) {
        auto PersonalityFn = MF.getFunction().getPersonalityFn();
        const TargetLowering &TLI = *MF.getSubtarget().getTargetLowering();
        if (unsigned Reg = TLI.getExceptionPointerRegister(PersonalityFn))
          for (MCRegUnitIterator RUI(Reg, TRI); RUI.isValid(); ++RUI)
            RUClobbers.set(*RUI);
        if (unsigned Reg = TLI.getExceptionSelectorRegister(PersonalityFn))
          for (MCRegUnitIterator RUI(Reg, TRI); RUI.isValid(); ++RUI)
            RUClobbers.set(*RUI);
      }
    }
```

the problem disappears.  Not sure if this is the best fix - maybe this case should be considered inside `getBeginClobberMask` in the first place?


