# ScalarEvolution eats more than 8MB stack space

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/159990
**Status:** Closed
**Labels:** llvm:SCEV, crash
**Closed Date:** 2025-10-19T18:08:00Z

## Body

Compiling this input results in a segfault on my machine when compiled with `-O3`:

```
#include <algorithm>
int a;
extern char b[];
long *c;
void e(bool d, bool f[][0][0]) {
#pragma clang loop unroll(enable)
  for (int g = 0; g < 10; g++)
#pragma clang loop unroll(enable)
    for (int h = 0; h < 10; h++) {
      b[h] = 1;
      for (short i = 0; i < 8 + 2; i += 2)
        for (short j(std::max((unsigned long long)1849463778,
                              (unsigned long long)d) >>
                     f[g][1][g]);
             j < 7; j += 4)
          a &= c[j];
    }
}
```

On my machine, the stack space size is 8MB, and with `ulimit -s unlimited`, compilation is not aborted.

It all start here:

```
#11749 0x0000aaaab24e146c in llvm::InductionDescriptor::isInductionPHI (Phi=0xaaaabab30858, TheLoop=0xaaaabac8cf80, SE=0xaaaaba91e480, D=..., Expr=0x0, CastsToIgnore=0x0) at  /llvm-project/llvm/lib/Analysis/IVDescriptors.cpp:1619
#11750 0x0000aaaab3c245fc in (anonymous namespace)::CanonicalizeFreezeInLoopsImpl::run (this=0xffffffff34c8) at  /llvm-project/llvm/lib/Transforms/Utils/CanonicalizeFreezeInLoops.cpp:160
#11751 0x0000aaaab3c252e4 in (anonymous namespace)::CanonicalizeFreezeInLoops::runOnLoop (this=0xaaaabb7342d0, L=0xaaaabac8cf80) at  /llvm-project/llvm/lib/Transforms/Utils/CanonicalizeFreezeInLoops.cpp:251
```

And then it endlessly disappears in various ScalarEvolution helper functions.  The full back trace is very long, I have attached it for completeness here:  [gdb.txt](https://github.com/user-attachments/files/22452759/gdb.txt)
It must be hitting a corner case of an inefficient algorithm or something like that, but I haven't looked further into yet. 



## Comments

### Comment 1 - sjoerdmeijer

CC: @nikic 

---

### Comment 2 - sjoerdmeijer

This is an interesting problem. I have briefly looked at this, but don't have a solution yet, just sharing some first observations:
- The crash happens in pass `CanonicalizeFreezeInLoops`, which starts by querying `InductionDescriptor::isInductionPHI` before it actually starts looking for Freeze instructions. This makes sense, but the observations is that for this example there are no freeze instructions at all. I have a suspicion that maybe doing a little bit more work upfront is actually cheaper: do a quick scan of all blocks/instructions in the loop, and bail out if there are no freeze instructions before doing more complicated SCEV analysis, because the callstack of isInductionPHI is actually quite deep and it is doing a lot. I tried that for this example, but it didn't solve the issue for this example, it just ran into the same problem the next time SE and SCEV was queried in a different pass. This is related to the next observations.
- The FullUnroller produces an absolute monster of a CFG. The reproducer and input code contains unroll pragmas, but the iteration counts are small, but somehow this results in a .ll with 10k lines of code. The initial IR is really small as you'd expect, but how it explodes to this size is beyond my imagination. I have a suspicion something problematic is going on with the full unroller, but haven't looked deeper and don't have the proof for this.
- Even if the we have an input file with 10k lines and a lot of loops, should SCEV be able to deal with this better? This pattern of SCEV calls is repeated hundreds/thousands of times: 

```
#11722 0x0000aaaab25d5aa0 in llvm::ScalarEvolution::LoopGuards::collectFromBlock (SE=..., Guards=..., Block=0xaaaaba8cb5d0, Pred=0xaaaabc846730, VisitedBlocks=..., Depth=1) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:15954
#11723 0x0000aaaab25d61b4 in llvm::ScalarEvolution::LoopGuards::collectFromPHI(llvm::ScalarEvolution&, llvm::ScalarEvolution::LoopGuards&, llvm::PHINode const&, llvm::SmallPtrSetImpl<llvm::BasicBlock const*>&, llvm::SmallDenseMap<llvm::BasicBlock const*, llvm::ScalarEvolution::LoopGuards, 4u, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::ScalarEvolution::LoopGuards> >&, unsigned int)::$_0::operator()(unsigned int) const (this=0xfffffffef2b8, IncomingIdx=0) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:15493
#11724 0x0000aaaab25d5ec0 in llvm::ScalarEvolution::LoopGuards::collectFromPHI (SE=..., Guards=..., Phi=..., VisitedBlocks=..., IncomingGuards=..., Depth=0) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:15527
#11725 0x0000aaaab25d5924 in llvm::ScalarEvolution::LoopGuards::collectFromBlock (SE=..., Guards=..., Block=0xaaaabab307c0, Pred=0xaaaaba8cb5d0, VisitedBlocks=..., Depth=0) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:15935
#11726 0x0000aaaab25c4c88 in llvm::ScalarEvolution::LoopGuards::collect (L=0xaaaabac8cf80, SE=...) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:15468
#11727 0x0000aaaab25b54d8 in llvm::ScalarEvolution::applyLoopGuards (this=0xaaaaba91e480, Expr=0xaaaaba7501b0, L=0xaaaabac8cf80) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:16112
#11728 0x0000aaaab25cd640 in llvm::ScalarEvolution::howManyLessThans(llvm::SCEV const*, llvm::SCEV const*, llvm::Loop const*, bool, bool, bool)::$_2::operator()() const (this=0xfffffffefcb8) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:13332
#11729 0x0000aaaab25c1a40 in llvm::ScalarEvolution::howManyLessThans (this=0xaaaaba91e480, LHS=0xaaaaba750c50, RHS=0xaaaaba7501b0, L=0xaaaabac8cf80, IsSigned=true, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:13356
#11730 0x0000aaaab25be054 in llvm::ScalarEvolution::computeExitLimitFromICmp (this=0xaaaaba91e480, L=0xaaaabac8cf80, Pred=..., LHS=0xaaaaba750c50, RHS=0xaaaaba7501b0, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:9377
#11731 0x0000aaaab25bd48c in llvm::ScalarEvolution::computeExitLimitFromICmp (this=0xaaaaba91e480, L=0xaaaabac8cf80, ExitCond=0xaaaabab30db0, ExitIfTrue=false, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:9222
#11732 0x0000aaaab25bc9a0 in llvm::ScalarEvolution::computeExitLimitFromCondImpl (this=0xaaaaba91e480, Cache=..., L=0xaaaabac8cf80, ExitCond=0xaaaabab30db0, ExitIfTrue=false, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:9077
#11733 0x0000aaaab25bc528 in llvm::ScalarEvolution::computeExitLimitFromCondCached (this=0xaaaaba91e480, Cache=..., L=0xaaaabac8cf80, ExitCond=0xaaaabab30db0, ExitIfTrue=false, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:9059
#11734 0x0000aaaab25bc200 in llvm::ScalarEvolution::computeExitLimitFromCond (this=0xaaaaba91e480, L=0xaaaabac8cf80, ExitCond=0xaaaabab30db0, ExitIfTrue=false, ControlsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:9015
#11735 0x0000aaaab25bc00c in llvm::ScalarEvolution::computeExitLimit (this=0xaaaaba91e480, L=0xaaaabac8cf80, ExitingBlock=0xaaaabab307c0, IsOnlyExit=true, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:8989
#11736 0x0000aaaab25ba378 in llvm::ScalarEvolution::computeBackedgeTakenCount (this=0xaaaaba91e480, L=0xaaaabac8cf80, AllowPredicates=false) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:8899
#11737 0x0000aaaab25b9770 in llvm::ScalarEvolution::getBackedgeTakenInfo (this=0xaaaaba91e480, L=0xaaaabac8cf80) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:8461
#11738 0x0000aaaab25b90cc in llvm::ScalarEvolution::getBackedgeTakenCount (this=0xaaaaba91e480, L=0xaaaabac8cf80, Kind=llvm::ScalarEvolution::ConstantMaximum) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:8396
#11739 0x0000aaaab2488d08 in llvm::ScalarEvolution::getConstantMaxBackedgeTakenCount (this=0xaaaaba91e480, L=0xaaaabac8cf80) at  /llvm-project/llvm/include/llvm/Analysis/ScalarEvolution.h:926
#11740 0x0000aaaab25b33ec in llvm::ScalarEvolution::getRangeRef (this=0xaaaaba91e480, S=0xaaaaba750c50, SignHint=llvm::ScalarEvolution::HINT_RANGE_UNSIGNED, Depth=0) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:6829
#11741 0x0000aaaab24e3f8c in llvm::ScalarEvolution::getUnsignedRange (this=0xaaaaba91e480, S=0xaaaaba750c50) at  /llvm-project/llvm/include/llvm/Analysis/ScalarEvolution.h:1016
#11742 0x0000aaaab25acbe0 in llvm::ScalarEvolution::proveNoWrapViaConstantRanges (this=0xaaaaba91e480, AR=0xaaaaba750c50) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:5135
#11743 0x0000aaaab25ae378 in llvm::ScalarEvolution::createSimpleAffineAddRec (this=0xaaaaba91e480, PN=0xaaaabab30858, BEValueV=0xaaaabab30cf0, StartValueV=0xaaaabb0a5418) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:5806
#11744 0x0000aaaab25af008 in llvm::ScalarEvolution::createAddRecFromPHI (this=0xaaaaba91e480, PN=0xaaaabab30858) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:5855
#11745 0x0000aaaab25b00ec in llvm::ScalarEvolution::createNodeForPHI (this=0xaaaaba91e480, PN=0xaaaabab30858) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:6089
#11746 0x0000aaaab25b7c00 in llvm::ScalarEvolution::createSCEV (this=0xaaaaba91e480, V=0xaaaabab30858) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:8155
#11747 0x0000aaaab25aba18 in llvm::ScalarEvolution::createSCEVIter (this=0xaaaaba91e480, V=0xaaaabab30858) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:7536
#11748 0x0000aaaab25a60b8 in llvm::ScalarEvolution::getSCEV (this=0xaaaaba91e480, V=0xaaaabab30858) at  /llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:4594
```

Because of the size of things, it is difficult to tell whether this is a bug in getSCEV, whether there's a bug in the CFG, and if SCEV should have some limits not to do the same things thousands of times. 

---

### Comment 3 - nikic

@sjoerdmeijer Does https://github.com/llvm/llvm-project/pull/152823 help this case?

---

### Comment 4 - sjoerdmeijer

> [@sjoerdmeijer](https://github.com/sjoerdmeijer) Does [#152823](https://github.com/llvm/llvm-project/pull/152823) help this case?

Thanks for pointing this out, I will try this locally and let know.

---

### Comment 5 - sjoerdmeijer

The problem has disappeared on trunk, which I bisected to commit 6d905e41bc82f1a6ae88484a700099fbe26460b6 that made this problem go away. 

I tried https://github.com/llvm/llvm-project/pull/152823 on top of a bad commit, but that didn't help.

We could close this, but I will leave this ticket open for now, I think it might be worth looking into how SE behaves now, which I haven't done yet. And like I said earlier, it might be worth looking separately into the full unroller because that blew my mind for this example. 

---

### Comment 6 - nikic

I think the unroller behavior here is basically expected, as you are forcing unrolling to something like 500 iterations. 100 outer iteration are forced, and `5*not_sure` iterations it probably unrolls by itself before getting to the outer loops.

---

### Comment 7 - sjoerdmeijer

Ok, let's close this.

---

