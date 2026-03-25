# [BasicBlockUtils] SplitLandingPadPredecessors() breaks LoopInfo

**Author:** yrouban
**URL:** https://github.com/llvm/llvm-project/issues/163922
**Status:** Open
**Labels:** crash, llvm:transforms

## Body

SplitLandingPadPredecessors() results in an irreducible loop and makes LoopInfo invalid. Verification results in a crash with the following error:

Assertion `CB != OutsideLoopPreds[i] && "Loop has multiple entry points!"' failed.

Here is the reproduction test where the bad loop has 2 blocks: catch_dest.split-lp ,catch_dest

Before:
```
declare void @foo()

define void @split-lp-predecessors-test() personality ptr null {
entry:
  invoke void @foo()
          to label %loop unwind label %catch_dest

loop:
  invoke void @foo()
          to label %latch unwind label %catch_dest

latch:
  br label %loop

catch_dest:
  %lp = landingpad i32
          cleanup
  invoke void @foo()
          to label %exit unwind label %catch_dest

exit:
  ret void
}
```

After SplitBlockPredecessors(catch_dest, { loop }, "", DT, LI):
```
declare void @foo()

define void @split-lp-predecessors-test() personality ptr null {
entry:
  invoke void @foo()
          to label %loop unwind label %catch_dest.split-lp

loop:                                             ; preds = %latch, %entry
  invoke void @foo()
          to label %latch unwind label %catch_dest1

latch:                                            ; preds = %loop
  br label %loop

catch_dest1:                                      ; preds = %loop
  %lpad = landingpad i32
          cleanup
  br label %catch_dest

catch_dest.split-lp:                              ; preds = %entry, %catch_dest
  %lpad.split-lp = landingpad i32
          cleanup
  br label %catch_dest

catch_dest:                                       ; preds = %catch_dest.split-lp, %catch_dest1
  invoke void @foo()
          to label %exit unwind label %catch_dest.split-lp

exit:                                             ; preds = %catch_dest
  ret void
}
```



## Comments

### Comment 1 - yrouban

Here is a new unit test to reproduce the bug:
[llvm-BasicBlockUtils-Test-with-broken-LoopInfo-after-SplitLandingPadPredecessors.patch](https://github.com/user-attachments/files/22965982/llvm-BasicBlockUtils-Test-with-broken-LoopInfo-after-SplitLandingPadPredecessors.patch)

---

### Comment 2 - yrouban

Here is the fix:
[llvm-BasicBlockUtils-Fixed-LoopInfo-update-in-UpdateAnalysisInformation.patch](https://github.com/user-attachments/files/22966040/llvm-BasicBlockUtils-Fixed-LoopInfo-update-in-UpdateAnalysisInformation.patch)

---

